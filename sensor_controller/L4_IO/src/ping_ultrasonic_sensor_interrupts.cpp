/*
 * ultrasonic_sensor.cpp
 *
 *  Created on: Sep 10, 2015
 *      Author: Divya,Hemanth K N and Akshay Vijaykumar
 */

#include <ping_ultrasonic_sensor_interrupts.hpp>
#include <stdio.h>
#include "adc0.h"
#include "utilities.h" // for TIME_US() in PING
#include "eint.h"   //for EINT3XXXXXX used in PING
#include "io.hpp"


static double latest_valuef = 0;  //front
static double latest_valuer = 0;  //right
static double latest_valuel = 0;  //left

static int front_obstacle_zone = 0;
static int right_obstacle_zone = 0;
static int left_obstacle_zone = 0;

int j = 0;

int flag_for_interrupt = 0;


typedef enum
{
    threshold_zero = 0,
    threshold_near = 10,
    threshold_middle = 150,
    threshold_far = 220,
} obs_thre;

typedef enum
{
    P = 0, //pass--no_obstacle:)
    F, // Far
    M, // Middle
    N // Near
} distance_obstacle;

//Divya editing master module for ultrasonic sensor
//Creating a class instance of all the ultrasonic modules

#if SINGLE_SENSOR
        Ultra_Sonic_4ping back_ultrasonic(P2_0,P2_1,0);
#endif

#if (THREE_PIN)   //three pin ping sensors

    three_pin front_ultrasonic(P2_1,P2_1,0);
    three_pin right_ultrasonic(P2_3,P2_3,1);
    three_pin left_ultrasonic(P2_5,P2_5,2);
#endif
#if (!THREE_PIN)   //four pin ping sensors
Ultra_Sonic_4ping front_ultrasonic(P2_0, P2_1, 0);
Ultra_Sonic_4ping right_ultrasonic(P2_2, P2_3, 1);
Ultra_Sonic_4ping left_ultrasonic(P2_4, P2_5, 2);
#endif

char zoneMessage[4] =
{ 'P', 'F', 'M', 'N' };

/* TogglePower
 * This function allows the caller to turn the sensor on or off.
 * */
void UltrasonicSensor::TogglePower(bool power)
{
    if (power) // If power on is requested and it is not already on
    {
        GpioPower.setHigh();
    }
    else // If power off is requested and it is not already off
    {
        GpioPower.setLow();
    }
}

/* Recalibrate
 * Performs a power cycle of the Sensor
 * */
void UltrasonicSensor::Recalibrate()
{
    TogglePower(OFF);
    TogglePower(ON);
}

/* ReadRange
 * Return the Raw Range value obtained from Sensor
 * */
float UltrasonicSensor::ReadRange(void)
{
    /* The function does the following:
     * Read the ADC Step value for a particular voltage
     * Calculate the voltage value by multiplying with ADC Step Voltage
     * The value is divided by voltage per inch.
     * */
    float usonic_adc_reading = 0;
    float usonic_sensor_reading_v = 0; // reading of sensor in volts
    float usonic_sensor_reading_inch = 0; // reading of sensor in inches

    usonic_adc_reading = (float) adc0_get_reading(
            USONIC_FRONTSENSOR_ADCCHANNEL);
    usonic_sensor_reading_v = (usonic_adc_reading * ADC_VOLTAGE_PER_STEP);
    usonic_sensor_reading_inch = (usonic_sensor_reading_v
            / USONIC_VOLTAGE_PER_INCH);

    if (getFilterUsage())
    {
        /* Send this data to Filter */
        avg_filter.addValue(usonic_sensor_reading_inch);
    }

    return usonic_sensor_reading_inch;
}

/* GetFilteredRangeValue
 * Returns the Filtered range output using an average filter
 * */

float UltrasonicSensor::GetFilteredRangeValue(void)
{
    /* If Filter has been used for the sensor, the output of the filter will be read from here
     * Else, this function returns 0 */

    if (getFilterUsage())
    {
        return avg_filter.getValue();
    }
    return 0;
}

//UltraSonic ping 3-pin sensor Divya editing
//Declaring all the static variables
SemaphoreHandle_t three_pin::Trig_Sem; //indicating the static variables created in .hpp file belong to this class
uint64_t three_pin::up_time;
uint64_t three_pin::down_time;
uint64_t three_pin::diff_time;
double three_pin::distance_value;
QueueHandle_t three_pin::xQueue[MAX_SENSOR_COUNT];
int three_pin::index = 0;
int three_pin::zone;
bool three_pin::max_time_flag = false;
int three_pin:: j =0;

//Constructor for the 3-pin ping sensor
three_pin::three_pin(LPC1758_GPIO_Type pTrig_out_pin,
        LPC1758_GPIO_Type pEcho_in_pin, int index1) :
        instance(index1), trig_out(pTrig_out_pin), echo_in(pEcho_in_pin)
{
    Trig_Sem = xSemaphoreCreateBinary(); //first give the Sem at initialization
    xSemaphoreGive(Trig_Sem); //send trig() will first check if its taken

   // echo_in.setAsInput();
    xQueue[instance] = xQueueCreate(2, sizeof(double));
    buff_for_recieve = 0;
    switch (pEcho_in_pin)
    {
        case (P2_1): //  if(index1 == 0)
        {
            echo_pin = 1;
            break;
        }

#if (!SINGLE_SENSOR)
        case (P2_3): // if(index1 == 1)
        {
            echo_pin = 3;
            break;
        }


        case (P2_5): // if(index1 == 2)
        {
            echo_pin = 5;
            break;
        }

#endif
        default:
            {

            } break;

    }

    enable3_interrupt();
}

void three_pin::enable3_interrupt()
{
    eint3_enable_port2(echo_pin, eint_rising_edge, echo3_high_callback);
    eint3_enable_port2(echo_pin, eint_falling_edge, echo3_low_callback);
}

#if 0
void three_pin::disable_interrupt()
{
    NVIC_DisableIRQ(EINT3_IRQn);
}
#endif

bool three_pin::send3_trig()
{

    if (xSemaphoreTake(Trig_Sem, 0))
    {

       // printf("Pinging %d\n", three_pin::index);

        flag_for_interrupt = 0;

        trig_out.setAsOutput();

        //reset the GPIO pins
        trig_out.setLow();
        delay_us(5);

        trig_out.setHigh();
        delay_us(10);
        trig_out.setLow();

        echo_in.setAsInput();

        flag_for_interrupt =  1;

    }

    return true;
}

void three_pin::echo3_high_callback()
{
    if(flag_for_interrupt == 1){
       up_time = sys_get_uptime_us();
    }
}
void three_pin::echo3_low_callback()
{
    if(flag_for_interrupt){
        down_time = sys_get_uptime_us();

        //Calculate the time of the ECHO pulse

        diff_time = down_time - up_time;

        distance_value = (double) (diff_time) * (1.0 / 58.0); //Distance in cms

        if (distance_value > MAX_VALUE)
        {
            distance_value = MAX_VALUE;
        }

        xQueueSendFromISR(xQueue[Ultra_Sonic_4ping::index], &distance_value, 0);

#if !SINGLE_SENSOR
        three_pin::index = three_pin::index + 1;

        if (three_pin::index == MAX_SENSOR_COUNT)
        {
            three_pin::index = 0;
        }
#endif
        }
        xSemaphoreGiveFromISR(Trig_Sem, false);

}

double three_pin::ping3_get_from_filter()
{
    double d = 0.0;

    d = avg_filter.getValue();

    return d;
}
bool three_pin::recieve3_from_queue(void)
{
    //this will fetch the value from the queue and put it in a buffer(pass-by-ref), 0 delay / ticks
    xQueueReceive(xQueue[instance], &buff_for_recieve, 0);

    return true;
}
double three_pin::get3_buffer_value()
{
    return buff_for_recieve;
}

void three_pin::add3_queue_value_to_filter(void)
{
    avg_filter.addValue(buff_for_recieve);

    return;
}
int three_pin::get3_zone(float avg)
{

    if ((avg >= threshold_zero) && (avg <= threshold_near))
    {
        zone = N;
    }
    else if ((avg > threshold_near) && (avg <= threshold_middle))
    {
        zone = M;
    }
    else if ((avg > threshold_middle) && (avg <= threshold_far))
    {
        zone = F;
    }
    else if (avg > threshold_far)
    {
        zone = P;
    }

    return zone;
}

void three_pin::display3_zone(int a)
{
    printf("Zone is:");
    switch (a)
    {
        case (0):
            printf("N\n");
            break;

        case (1):
            printf("M\n");
            break;

        case (2):
            printf("F\n");
            break;

        case (3):
            printf("P\n");
            break;
    }
}
three_pin * ping3_get_current_instance()
{
   // printf("Ping selected : %d",three_pin::index);

        if(three_pin::index == 0)
        {
            return &front_ultrasonic;
        }
       else if (three_pin::index == 1)
        {//printf("Ping selected : %d",Ultra_Sonic_4ping::index);
            return &right_ultrasonic;
        }
        else if (three_pin::index == 2)
        {//printf("Ping selected : %d",Ultra_Sonic_4ping::index);
                return &left_ultrasonic;
        }

        return NULL;
}
three_pin* ping3_get_prev_instance()
{

       if(three_pin::index-1 == 0)
       {
           return &front_ultrasonic;
       }
      else if (three_pin::index-1 == -1)
       {
            return &right_ultrasonic;
       }

       else if (three_pin::index-1 == -1)
       {
            return &left_ultrasonic;
       }


       return NULL;

}

void three_pin_sensor()
{
//TRIGGER TO ONE OF THE SENSORS
    three_pin *p = NULL;

    p = ping3_get_current_instance();


    if(p-> pinNotLow())
    {
         //printf("Echo Pin Not Low\n");
         three_pin::max_time_flag = true;
          return;
    }

    p->send3_trig();


    p = ping3_get_prev_instance();

    p->recieve3_from_queue();

    p->add3_queue_value_to_filter();


   switch(p->instance)
    {
        case(0) :
           {
            latest_valuef = p->ping3_get_from_filter();
            printf("[%f]\n",latest_valuef);
#if ZONE_INFO
            front_obstacle_zone = p->get3_zone(latest_valuef);
#endif
               break;
           }

        case(1) :
                latest_valuer = p->ping3_get_from_filter();
      //  printf("[%f]\n",latest_valuer);
#if ZONE_INFO
                right_obstacle_zone = p->get3_zone(latest_valuer);
#endif
                break;

        case(2) :
                latest_valuel = p->ping3_get_from_filter();
#if ZONE_INFO
                left_obstacle_zone = p->get3_zone(latest_valuel);
#endif
                break;
     }//switch
#if  ZONE_INFO
 //   printf("[%f][%f][%f]\n", latest_valuer, latest_valuef, latest_valuel);
//    printf("[%c][%c][%c]\n", zoneMessage[right_obstacle_zone],zoneMessage[front_obstacle_zone], zoneMessage[left_obstacle_zone]);

    if(right_obstacle_zone == N)
    {
        LE.on(RIGHT_LED);
    }
    else
    {
        LE.off(RIGHT_LED);
    }

    if(left_obstacle_zone == N)
    {
        LE.on(LEFT_LED);
    }
    else
    {
        LE.off(LEFT_LED);
    }

    if (front_obstacle_zone == N)
    {
        LE.on(FRONT_LED);
    }
    else
    {
        LE.off(FRONT_LED);
    }
#endif
}//end three_pin_function
/////////////////////////////////////////////////////////////////////////////////////



//UltraSonic ping 4-pin sensor Divya editing
//Declaring all the static variables
SemaphoreHandle_t Ultra_Sonic_4ping::Trig_Sem; //indicating the static variables created in .hpp file belong to this class
uint64_t Ultra_Sonic_4ping::up_time;
uint64_t Ultra_Sonic_4ping::down_time;
uint64_t Ultra_Sonic_4ping::diff_time;
double Ultra_Sonic_4ping::distance_value;
QueueHandle_t Ultra_Sonic_4ping::xQueue[MAX_SENSOR_COUNT];
int Ultra_Sonic_4ping::index = 0;
int Ultra_Sonic_4ping::zone;
bool Ultra_Sonic_4ping::max_time_flag = false;
int Ultra_Sonic_4ping:: i =0;

//Constructor for the 4-pin ping sensor
Ultra_Sonic_4ping::Ultra_Sonic_4ping(LPC1758_GPIO_Type pTrig_out_pin,
        LPC1758_GPIO_Type pEcho_in_pin, int index1) :
        instance(index1), trig_out(pTrig_out_pin), echo_in(pEcho_in_pin)
{
    Trig_Sem = xSemaphoreCreateBinary(); //first give the Sem at initialization
    xSemaphoreGive(Trig_Sem); //send trig() will first check if its taken
    trig_out.setAsOutput();
    echo_in.setAsInput();
    xQueue[instance] = xQueueCreate(2, sizeof(double));
    buff_for_recieve = 0;
    switch (pEcho_in_pin)
    {
        case (P2_1): //  if(index1 == 0)
        {
            echo_pin = 1;
            break;
        }

#if (!SINGLE_SENSOR)
        case (P2_3): // if(index1 == 1)
        {
            echo_pin = 3;
            break;
        }


        case (P2_5): // if(index1 == 2)
        {
            echo_pin = 5;
            break;
        }
/*
        case (P2_7):  //:if(index1 == 3)
        {
            echo_pin = 7;
            break;
        }

        case (P0_30):  //:if(index1 == 4)
                {
                    echo_pin = 30;
                    break;
                }
*/
#endif
        default:
            {

            } break;

    }

    eint3_enable_port2(echo_pin, eint_rising_edge, echo_high_callback);
    eint3_enable_port2(echo_pin, eint_falling_edge, echo_low_callback);
    //eint3_enable_port0(echo_pin, eint_rising_edge, echo_high_callback);
    //eint3_enable_port0(echo_pin, eint_rising_edge, echo_high_callback);
}

bool Ultra_Sonic_4ping::send_trig()
{
    if (xSemaphoreTake(Trig_Sem, 0))
    {
        //reset the GPIO pins
        trig_out.setLow();
        delay_us(5);

        trig_out.setHigh();
        delay_us(10);
        trig_out.setLow();

    }

    return true;
}

void Ultra_Sonic_4ping::echo_high_callback()
{
    up_time = sys_get_uptime_us();
}

void Ultra_Sonic_4ping::echo_low_callback()
{

    down_time = sys_get_uptime_us();

    //Calculate the time of the ECHO pulse

    diff_time = down_time - up_time;

    distance_value = (double) (diff_time) * (1.0 / 58.0); //Distance in cms

    if (distance_value > MAX_VALUE)
    {
        distance_value = MAX_VALUE;
    }


    xQueueSendFromISR(xQueue[Ultra_Sonic_4ping::index], &distance_value, 0);

#if !SINGLE_SENSOR
    Ultra_Sonic_4ping::index = Ultra_Sonic_4ping::index + 1;

    if (Ultra_Sonic_4ping::index == MAX_SENSOR_COUNT)
    {
        Ultra_Sonic_4ping::index = 0;
    }
#endif

    xSemaphoreGiveFromISR(Trig_Sem, false);

}

double Ultra_Sonic_4ping::ping_get_from_filter()
{
    double d = 0.0;

    d = avg_filter.getValue();

    return d;
}

bool Ultra_Sonic_4ping::recieve_from_queue(void)
{
    //this will fetch the value from the queue and put it in a buffer(pass-by-ref), 0 delay / ticks
    xQueueReceive(xQueue[instance], &buff_for_recieve, 0);

    return true;
}

double Ultra_Sonic_4ping::get_buffer_value()
{
    return buff_for_recieve;
}

void Ultra_Sonic_4ping::add_queue_value_to_filter(void)
{
    avg_filter.addValue(buff_for_recieve);

    return;
}

int Ultra_Sonic_4ping::get_zone(float avg)
{

    if ((avg >= threshold_zero) && (avg <= threshold_near))
    {
        zone = N;
    }
    else if ((avg > threshold_near) && (avg <= threshold_middle))
    {
        zone = M;
    }
    else if ((avg > threshold_middle) && (avg <= threshold_far))
    {
        zone = F;
    }
    else if (avg > threshold_far)
    {
        zone = P;
    }

    return zone;
}

void Ultra_Sonic_4ping::display_zone(int a)
{
    printf("Zone is:");
    switch (a)
    {
        case (0):
            printf("N\n");
            break;

        case (1):
            printf("M\n");
            break;

        case (2):
            printf("F\n");
            break;

        case (3):
            printf("P\n");
            break;
    }
}

Ultra_Sonic_4ping * ping_get_current_instance()
{
#if (0)
        if(Ultra_Sonic_4ping::index == 0)
        {//printf("Ping selected : %d",Ultra_Sonic_4ping::index);
            return &front_ultrasonic;
        }
        else if (Ultra_Sonic_4ping::index == 1)
        {//printf("Ping selected : %d",Ultra_Sonic_4ping::index);
            return &right_ultrasonic;
        }
        else if (Ultra_Sonic_4ping::index == 2)
        {//printf("Ping selected : %d",Ultra_Sonic_4ping::index);
                return &left_ultrasonic;
        }
#if TEST_SIX
        else if (Ultra_Sonic_4ping::index == 3)
        {
                    return &middle_right_ultrasonic ;
        }
        else if (Ultra_Sonic_4ping::index == 4)
        {
                    return &middle_left_ultrasonic ;
        }
        else if (Ultra_Sonic_4ping::index == 5)
        {
                    return &back_ultrasonic;
        }
#endif
#endif
        return NULL;
}

Ultra_Sonic_4ping* ping_get_prev_instance()
{
#if (0)
       if(Ultra_Sonic_4ping::index-1 == 0)
       {
           return &front_ultrasonic;
       }
       else if (Ultra_Sonic_4ping::index-1 == 1)
       {
            return &right_ultrasonic;
       }

       else if (Ultra_Sonic_4ping::index-1 == -1)
       {
            return &left_ultrasonic;
       }

#if TEST_SIX
       else if (Ultra_Sonic_4ping::index-1 == 2)
       {
            return &middle_right_ultrasonic ;
       }
       else if (Ultra_Sonic_4ping::index-1 == 3)
       {
            return &middle_right_ultrasonic ;
       }
       else if (Ultra_Sonic_4ping::index-1 == 4)
       {
            return &middle_left_ultrasonic ;
       }
       else if (Ultra_Sonic_4ping::index-1 == -1)
       {
            return &back_ultrasonic;
       }
#endif

#endif
       return NULL;
}
/*
void single_back_ping_sensor()
{
    static double latest_valueb = 0;  //back
    static double back_distance = 0;
    static int back_obstacle_zone = 0;

   //TRIGGER TO ONE OF THE SENSORS
       Ultra_Sonic_4ping *p = NULL;

       p = ping_get_current_instance();

       if(p-> pinNotLow())
       {
             Ultra_Sonic_4ping::max_time_flag = true;
             return;
       }

       p->send_trig();

       p = ping_get_prev_instance();

       p->recieve_from_queue();

       p->add_queue_value_to_filter();


      switch(p->instance)
       {
           case(0) :
              {
               latest_valueb = p->ping_get_from_filter();
   #if ZONE_INFO
               back_obstacle_zone = p->get_zone(latest_valueb);
   #endif
                  break;
              }
       }

}
*/

void interrupt_based_ping_sensor()
{
    //****************************SENSOR******************************//

    /* Method of working :
     * Ping Sensor 0; Read All Queues
     */

    //TRIGGER TO ONE OF THE SENSORS
    Ultra_Sonic_4ping *p = NULL;

    p = ping_get_current_instance();


    if(p-> pinNotLow())
    {
         //printf("Echo Pin Not Low\n");
         Ultra_Sonic_4ping::max_time_flag = true;
          return;
    }

    p->send_trig();

    p = ping_get_prev_instance();

    p->recieve_from_queue();

    p->add_queue_value_to_filter();


   switch(p->instance)
    {
        case(0) :
           {
            latest_valuef = p->ping_get_from_filter();
#if ZONE_INFO
            front_obstacle_zone = p->get_zone(latest_valuef);
#endif
               break;
           }

        case(1) :
                latest_valuer = p->ping_get_from_filter();
#if ZONE_INFO
                right_obstacle_zone = p->get_zone(latest_valuer);
#endif
                break;

        case(2) :
                latest_valuel = p->ping_get_from_filter();
#if ZONE_INFO
                left_obstacle_zone = p->get_zone(latest_valuel);
#endif
                break;

#if TEST_SIX
      case(3) :   latest_valuemr = p->ping_get_from_filter();
#if ZONE_INFO
    middle_right_obstacle_zone = p->get_zone(latest_valuemr);
#endif
                  break;

      case(5) :   latest_valueml = p->ping_get_from_filter();
#if ZONE_INFO
    middle_left_obstacle_zone = p->get_zone(latest_valueml);
#endif
                 break;

      case(6) :   latest_valueb = p->ping_get_from_filter();
#if ZONE_INFO
    back_obstacle_zone = p->get_zone(latest_valueb);
#endif
      break;

#endif
    } //endcase

#if (!SENSOR_SEND_ON_CAN)
    #if (!ZONE_INFO)
      printf("[%f][%f][%f]\n", latest_valuer, latest_valuef, latest_valuel);
    #endif

    #if TEST_SIX
        #if(!ZONE_INFO)
              printf("[%f][%f][%f][%f][%f][%f]\n", latest_valuer, latest_valuef, latest_valuel,
                      latest_valuemr,latest_valueml,latest_valueb);
        #endif
    #endif
#endif



#if  ZONE_INFO
 //   printf("[%f][%f][%f]\n", latest_valuer, latest_valuef, latest_valuel);
//    printf("[%c][%c][%c]\n", zoneMessage[right_obstacle_zone],zoneMessage[front_obstacle_zone], zoneMessage[left_obstacle_zone]);

    if(right_obstacle_zone == N)
    {
        LE.on(RIGHT_LED);
    }
    else
    {
        LE.off(RIGHT_LED);
    }

    if(left_obstacle_zone == N)
    {
        LE.on(LEFT_LED);
    }
    else
    {
        LE.off(LEFT_LED);
    }

    if (front_obstacle_zone == N)
    {
        LE.on(FRONT_LED);
    }
    else
    {
        LE.off(FRONT_LED);
    }

#if TEST_SIX
        if(middle_right_obstacle_zone == N)
        {
            LD.setLeftDigit(03);  //for displaying the middle right
        }
        else
        {
            LD.setLeftDigit(33);
        }

        if(middle_left_obstacle_zone == N)
        {
                LD.setLeftDigit(04);  //for displaying the middle left
        }
        else
        {
            LD.setLeftDigit(44);
        }

        if(back_obstacle_zone == N)
        {
                LD.setLeftDigit(05);  //for displaying the back
        }
        else
        {
                LD.setLeftDigit(55);
        }

#endif

#endif
//}

//void send_to_can()
//{
#if SENSOR_SEND_ON_CAN

    dist_sensor all_sensor;
    can_msg_t sensor_msg;
    bool status = false;

#if ZONE_INFO

    all_sensor.front_center = (uint8_t) front_obstacle_zone;
    all_sensor.front_left = (uint8_t) left_obstacle_zone;
    all_sensor.front_right = (uint8_t) right_obstacle_zone;

#if TEST_SIX
    all_sensor.left = (uint8_t) middle_left_obstacle_zone;
    all_sensor.right = (uint8_t) middle_right_obstacle_zone;
    all_sensor.back = (uint8_t) back_obstacle_zone;
#endif

    sensor_msg.msg_id = DISTANCE_SENSOR_ID;

    sensor_msg.frame_fields.is_29bit = false; //11-bit
    sensor_msg.frame_fields.data_len = sizeof(dist_sensor);

    memcpy(&sensor_msg.data.qword, &all_sensor, sizeof(dist_sensor));
//sending the values at 100Hz
        j = j + 1;
        if(j == 99){
            status = CAN_tx(PING_CAN, &sensor_msg, PING_TIMEOUT);
            j = 0;
        }

  //  printf("[%c][%c][%c]\n", zoneMessage[left_obstacle_zone],zoneMessage[front_obstacle_zone], zoneMessage[right_obstacle_zone]);
    if (!status)
    {
       // LOG_ERROR("Sending Distance values -- FAILED ");
        //       LE.on( PING_ZONE_SENDING_ERROR_LED );    //LE.on(3);
    }
    else
    {
        LE.off( PING_ZONE_SENDING_ERROR_LED );
    }

#else //if !ZONE_INFO

    all_sensor.front_center = latest_valuef;
    all_sensor.front_left = latest_valuel;
    all_sensor.front_right = latest_valuer;
#if TEST_SIX
    all_sensor.right = latest_valuemr;
    all_sensor.left = latest_valueml;
    all_sensor.back = latest_valueb;
#endif
    sensor_msg.msg_id = DISTANCE_SENSOR_ID;
    sensor_msg.frame_fields.is_29bit = 0; //11-bit
    sensor_msg.frame_fields.data_len = sizeof(dist_sensor);
    memcpy(&sensor_msg.data.qword,&all_sensor,sizeof(dist_sensor));

    status = CAN_tx(PING_CAN,&sensor_msg,PING_TIMEOUT);


    if(status == false)
    {
        LOG_ERROR("Sending Distance values -- FAILED ");
        LE.off( PING_ZONE_SENDING_ERROR_LED );
    }
    else
    {
        LE.toggle( PING_ZONE_SENDING_ERROR_LED );
     }

#endif //if SENSOR_SEND_ON_CAN
#endif

} //end interrupt_based_ping_sensor
bool bus_reset()
{
    CAN_reset_bus(PING_CAN);
  //  LE.toggle(4);
    return true;
}

void ping_heartbeat(void)
{
    //Heartbeat to master
    can_msg_t sensor_heart_msg;
    bool status = false;

    sensor_heart_msg.msg_id = SENSOR_HEARTBEAT_ID;
    sensor_heart_msg.frame_fields.is_29bit = 0;
    sensor_heart_msg.frame_fields.data_len = 0;

    //can2,sensor_heartbeat_id,0 timeout
    status = CAN_tx(PING_CAN, &sensor_heart_msg, PING_TIMEOUT);
    LD.setNumber(11);

    if (status == false)
    {
        LOG_ERROR("SENDING PING HEARTBEAT FAILED!!");
    }
    else
    {
        LE.toggle( PING_HEARTBEAT_ERROR_LED ); //LED 4
    }
}

void ping_powerupsync(void)
{
    can_msg_t ping_sync_msg;
    bool sync_ack = false;
    bool status = false;

    ping_sync_msg.msg_id = SENSOR_SYNC_ID;
    ping_sync_msg.frame_fields.is_29bit = 0;
    ping_sync_msg.frame_fields.data_len = 0;

    do
    {
        status = CAN_tx(PING_CAN, &ping_sync_msg, PING_TIMEOUT);

        if (status == false)
        {
            LOG_ERROR("SENDING PING SYNC FAILED!");
        }

        delay_ms(500);

    } while (sync_ack == false);

}

extern bool bus_off;
void test_bus_off_cb(uint32_t d)
{
    bus_off = true;
 //   LE.on(PING_CAN_BO);
}

void data_ovr_cb(uint32_t d)
{
    return;
}

