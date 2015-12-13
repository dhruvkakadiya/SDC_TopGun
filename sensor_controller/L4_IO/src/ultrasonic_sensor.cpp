/*
 * ultrasonic_sensor.cpp
 *
 *  Created on: Oct 14, 2015
 *      Author: Divya
 */


#include <ultrasonic_sensor.hpp>
#include <stdio.h>
#include "adc0.h"
#include "utilities.h" // for TIME_US() in PING
#include "eint.h"   //for EINT3XXXXXX used in PING
#include "io.hpp"

typedef enum{
    threshold_zero    = 0 ,
    threshold_nearest = 60,
    threshold_near    = 120,
    threshold_middle  = 190,
    threshold_far      = 300,
} obs_thre;

typedef enum {
    P = 0, //pass--no_obstacle:)
    F,  // Far
    M,  // Middle
    N  // Near
} distance_obstacle;

//Divya editing master module for ultrasonic sensor
//Creating a class instance of all the ultrasonic modules

Ultra_Sonic_4ping front_ultrasonic(P2_0, P2_1, 0);
Ultra_Sonic_4ping right_ultrasonic(P2_2, P2_3, 1);
Ultra_Sonic_4ping left_ultrasonic(P2_4, P2_5, 2);

char zoneMessage[4] = {'P', 'F', 'M', 'N'};

/* TogglePower
 * This function allows the caller to turn the sensor on or off.
 * */
void UltrasonicSensor::TogglePower(bool power)
{
    if ( power )  // If power on is requested and it is not already on
    {
        GpioPower.setHigh();
    }
    else         // If power off is requested and it is not already off
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
    float usonic_sensor_reading_v = 0;     // reading of sensor in volts
    float usonic_sensor_reading_inch = 0;     // reading of sensor in inches

    usonic_adc_reading = (float)adc0_get_reading(USONIC_FRONTSENSOR_ADCCHANNEL);
    usonic_sensor_reading_v = ( usonic_adc_reading * ADC_VOLTAGE_PER_STEP );
    usonic_sensor_reading_inch = ( usonic_sensor_reading_v / USONIC_VOLTAGE_PER_INCH );

    if(getFilterUsage())
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

    if(getFilterUsage())
    {
        return avg_filter.getValue();
    }
    return 0;
}


//UltraSonic ping 4-pin sensor Divya editing
//Declaring all the static variables
SemaphoreHandle_t Ultra_Sonic_4ping :: Trig_Sem;//indicating the static variables created in .hpp file belong to this class
uint64_t  Ultra_Sonic_4ping :: up_time;
uint64_t Ultra_Sonic_4ping :: down_time;
uint64_t Ultra_Sonic_4ping :: diff_time;
double  Ultra_Sonic_4ping :: distance_value;
QueueHandle_t Ultra_Sonic_4ping:: xQueue[MAX_SENSOR_COUNT];
int Ultra_Sonic_4ping :: index = 0;
int Ultra_Sonic_4ping :: zone;
bool Ultra_Sonic_4ping:: max_time_flag = false;

//Constructor for the 4-pin ping sensor
Ultra_Sonic_4ping :: Ultra_Sonic_4ping( LPC1758_GPIO_Type pTrig_out_pin,  LPC1758_GPIO_Type pEcho_in_pin, int index1):
    trig_out(pTrig_out_pin),
    echo_in(pEcho_in_pin),
    instance(index1)
{
   Trig_Sem = xSemaphoreCreateBinary();  //first give the Sem at initialization
   xSemaphoreGive(Trig_Sem);             //send trig() will first check if its taken
   trig_out.setAsOutput();
   echo_in.setAsInput();
   xQueue[instance] = xQueueCreate(2 , sizeof(double));

   switch(pEcho_in_pin)
   {
       case (P2_1) :  //  if(index1 == 0)
        {
           echo_pin = 1;
        }
        break;

        case (P2_3) :  // if(index1 == 1)
        {
           echo_pin = 3;
        }
        break;

        case (P2_5) :   // if(index1 == 2)
        {
          echo_pin = 5;
        }
        break;
   }

   eint3_enable_port2(echo_pin,eint_rising_edge,echo_high_callback);
   eint3_enable_port2(echo_pin,eint_falling_edge,echo_low_callback);
}

bool Ultra_Sonic_4ping ::send_trig()
{
    if( xSemaphoreTake(Trig_Sem,0))
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

void Ultra_Sonic_4ping :: echo_high_callback()
{
    up_time = sys_get_uptime_us();
}

void Ultra_Sonic_4ping :: echo_low_callback()
{

/*    if(Ultra_Sonic_4ping::max_time_flag ==  true)
    {
        distance_value = MAX_VALUE;
        Ultra_Sonic_4ping::max_time_flag = false;
    }
    else
    { */
        down_time = sys_get_uptime_us();

        //Calculate the time of the ECHO pulse

        diff_time = down_time - up_time;

        distance_value = (double)(diff_time)*(1.0/58.0);  //Distance in cms

        if(distance_value > MAX_VALUE)
        {
            distance_value = MAX_VALUE;
        }
   // }

    xQueueSendFromISR(xQueue[Ultra_Sonic_4ping :: index], &distance_value,0);

#if !SINGLE_SENSOR
    Ultra_Sonic_4ping :: index = Ultra_Sonic_4ping :: index  + 1;

    if(Ultra_Sonic_4ping::index == MAX_SENSOR_COUNT)
    {
        Ultra_Sonic_4ping::index = 0;
    }
#endif

    xSemaphoreGiveFromISR(Trig_Sem,false);

}

double Ultra_Sonic_4ping :: ping_get_from_filter()
{
    double d = 0.0;

    d = avg_filter.getValue();

    return d;
}

bool Ultra_Sonic_4ping :: recieve_from_queue(void)
{
    //this will fetch the value from the queue and put it in a buffer(pass-by-ref), 0 delay / ticks
    xQueueReceive( xQueue[instance], &buff_for_recieve, 0);

    return true;
}

double Ultra_Sonic_4ping :: get_buffer_value()
{
    return buff_for_recieve;
}

void Ultra_Sonic_4ping :: add_queue_value_to_filter(void)
{
    avg_filter.addValue(buff_for_recieve);

    return;
}

int Ultra_Sonic_4ping :: get_zone(float avg)
{

    if(( avg >= threshold_zero) && ( avg <= threshold_nearest))
     {
            zone = N;
     }
     else if(( avg > threshold_nearest) && ( avg <= threshold_near))
     {
            zone = M;
     }
     else if(( avg > threshold_near) && ( avg <= threshold_middle))
     {
             zone = F;
     }
     else if(( avg > threshold_middle) && ( avg <= threshold_far))
     {
            zone = P;
     }
     else if(avg > threshold_far)
     {
             zone = P;
     }

    return zone;
}

void Ultra_Sonic_4ping :: display_zone(int a)
{
    printf("Zone is:");
    switch(a)
    {
        case(0) : printf("N\n");
                  break;

        case(1) : printf("M\n");
                  break;

        case(2) : printf("F\n");
                  break;

        case(3) : printf("P\n");
                  break;
    }
}

void  interrupt_based_ping_sensor()
{
    static double latest_valuef = 0;
    static double latest_valuer = 0;
    static double latest_valuel = 0;

#if ZONE_INFO
    static int front_obstacle_zone = 0;
    static int right_obstacle_zone = 0;
    static int left_obstacle_zone = 0;

 //    obs_thre threshold;
#endif

      //****************************SENSOR******************************//

        /* Method of working :
         * Ping Sensor 0; Read All Queues
        */

      //TRIGGER TO ONE OF THE SENSORS
      if(Ultra_Sonic_4ping::index == 0)
      {

        if(front_ultrasonic.pinNotLow())
        {
           //printf("Echo Pin Not Low\n");
           Ultra_Sonic_4ping::max_time_flag = true;
           return;
        }

        front_ultrasonic.send_trig();
      }

#if !SINGLE_SENSOR
      else if(Ultra_Sonic_4ping::index == 1)
      {
          if(right_ultrasonic.pinNotLow())
          {
             //printf("Echo Pin Not Low\n");
             Ultra_Sonic_4ping::max_time_flag = true;
             return;
          }

          right_ultrasonic.send_trig();
       }

      else if(Ultra_Sonic_4ping::index == 2)
      {
          if(left_ultrasonic.pinNotLow())
          {
             //printf("Echo Pin Not Low\n");
             Ultra_Sonic_4ping::max_time_flag = true;
             return;
          }

          left_ultrasonic.send_trig();
      }
#endif


#if 1
 //     if(front_ultrasonic.recieve_from_queue())  //if the queue has a value->returns true
 //     {
          if((Ultra_Sonic_4ping :: index - 1) == 0)
          {
              front_ultrasonic.recieve_from_queue();

              front_ultrasonic.add_queue_value_to_filter();

              latest_valuef = front_ultrasonic.ping_get_from_filter();
          }
  //    }

#if ZONE_INFO
   front_obstacle_zone = front_ultrasonic.get_zone(latest_valuef);
#endif

    //end of front sensor
    //////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////
    //start of right sensor

#if !SINGLE_SENSOR


          if((Ultra_Sonic_4ping :: index - 1) == 1)
          {
                 right_ultrasonic.recieve_from_queue();

                 right_ultrasonic.add_queue_value_to_filter();

                 latest_valuer = right_ultrasonic.ping_get_from_filter();
          }


#if ZONE_INFO
     right_obstacle_zone = right_ultrasonic.get_zone(latest_valuer);
#endif

#endif
    //end of right sensor
    //////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////
    //start of left sensor

#if !SINGLE_SENSOR

          if((Ultra_Sonic_4ping :: index - 1) == -1)
          {
              left_ultrasonic.recieve_from_queue();

              left_ultrasonic.add_queue_value_to_filter();

              latest_valuel = left_ultrasonic.ping_get_from_filter();
          }


#if ZONE_INFO
      left_obstacle_zone = left_ultrasonic.get_zone(latest_valuel);
#endif

#endif
    //////////////////////////////////end of left sensor
#endif

#if 0 // ZONE_INFO
      //printf("[%c][%c][%c]\n", zoneMessage[left_obstacle_zone],zoneMessage[front_obstacle_zone], zoneMessage[right_obstacle_zone]);

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

//#else
    // printf("[%f][%f][%f]\n", latest_valuer, latest_valuef, latest_valuel);

#endif

#if SENSOR_SEND_ON_CAN

      dist_sensor all_sensor;
      can_msg_t sensor_msg;
      bool status = false;

#if ZONE_INFO

      all_sensor.front_center = (uint8_t) front_obstacle_zone;
      all_sensor.front_left   = (uint8_t) left_obstacle_zone;
      all_sensor.front_right  = (uint8_t) right_obstacle_zone;

     sensor_msg.msg_id = DISTANCE_SENSOR_ID;

      sensor_msg.frame_fields.is_29bit = false;      //11-bit
      sensor_msg.frame_fields.data_len = sizeof(dist_sensor);

      memcpy(&sensor_msg.data.qword, &all_sensor, sizeof(dist_sensor));

      status = CAN_tx(PING_CAN, &sensor_msg, PING_TIMEOUT);

      if(!status)
      {
          LOG_ERROR("Sending Distance values -- FAILED ");
   //       LE.on( PING_ZONE_SENDING_ERROR_LED );    //LE.on(3);
      }
      else
      {
          LE.toggle(  PING_ZONE_SENDING_ERROR_LED  );
      }

#else

      all_sensor.front_center = latest_valuef ;
      all_sensor.front_left   = latest_valuel ;
      all_sensor.front_right  = latest_valuer ;

      sensor_msg.msg_id = DISTANCE_SENSOR_ID ;
      sensor_msg.frame_fields.is_29bit = 0;      //11-bit
      sensor_msg.frame_fields.data_len = sizeof(dist_sensor);
      memcpy(&sensor_msg.data.qword,&all_sensor,sizeof(dist_sensor));

      status = CAN_tx(PING_CAN,&sensor_msg,PING_TIMEOUT);

      if(status == false)
      {
          LOG_ERROR("Sending Distance values -- FAILED ");
          LE.toggle(  PING_ZONE_SENDING_ERROR_LED  );
      }
      else
      {
          LE.off(  PING_ZONE_SENDING_ERROR_LED  );
      }

#endif
#endif
}
bool bus_reset()
{
    CAN_reset_bus(PING_CAN);
    return true;
}

void ping_heartbeat(void)
{
        //Heartbeat to master
        can_msg_t sensor_heart_msg;
        bool status = false;

        sensor_heart_msg.msg_id = SENSOR_HEARTBEAT_ID ;
        sensor_heart_msg.frame_fields.is_29bit = 0;
        sensor_heart_msg.frame_fields.data_len = 0;

        //can2,sensor_heartbeat_id,0 timout
        status = CAN_tx(PING_CAN, &sensor_heart_msg, PING_TIMEOUT);

        if(status == false)
        {
            LOG_ERROR("SENDING PING HEARTBEAT FAILED!!");
        }
        else
        {
            LE.toggle(  PING_HEARTBEAT_ERROR_LED  );  //LED 4
        }
}

void ping_powerupsync(void)
{
    can_msg_t ping_sync_msg;
    bool sync_ack = false;
    bool status   = false;

    ping_sync_msg.msg_id = SENSOR_SYNC_ID ;
    ping_sync_msg.frame_fields.is_29bit = 0;
    ping_sync_msg.frame_fields.data_len = 0;

    do
    {
        status = CAN_tx(PING_CAN,&ping_sync_msg,PING_TIMEOUT);

        if(status == false)
        {
            LOG_ERROR("SENDING PING SYNC FAILED!");
        }

        delay_ms(500);

    }while (sync_ack == false);


}

extern bool bus_off;
void test_bus_off_cb(uint32_t d)
{
   bus_off = true;
   LE.on(PING_CAN_BO);
}

void data_ovr_cb(uint32_t d)
{
    return;
}

