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

    //Divya editing master module for ultrasonic sensor
    //Creating a class instance of all the ultrasonic modules
    Ultra_Sonic_4ping front_ultrasonic(P2_0, P2_1, 0);
    Ultra_Sonic_4ping right_ultrasonic(P2_2, P2_3, 1);
    Ultra_Sonic_4ping left_ultrasonic(P2_4, P2_5, 2);
char zoneMessage[4][10] = {"N", "M", "F", "P"};



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
uint32_t Ultra_Sonic_4ping :: diff_time;
float  Ultra_Sonic_4ping :: distance_value;
Sensor_Filter <double, double> Ultra_Sonic_4ping:: avg_filter;
QueueHandle_t Ultra_Sonic_4ping:: xQueue[MAX_SENSOR_COUNT];
int Ultra_Sonic_4ping :: index = 0;
int Ultra_Sonic_4ping :: zone;


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
   xQueue[instance] = xQueueCreate(2,sizeof(float));

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
        trig_out.set(false);
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
    down_time = sys_get_uptime_us();

    //Calculate the time of the ECHO pulse

    diff_time = down_time - up_time;

    distance_value = (float)(diff_time)*(1.0/58);  //Distance in cms

    xQueueSendFromISR(xQueue[Ultra_Sonic_4ping :: index], &distance_value,0);

    Ultra_Sonic_4ping :: index = Ultra_Sonic_4ping :: index  + 1;

    if(Ultra_Sonic_4ping::index == MAX_SENSOR_COUNT)
    {
        Ultra_Sonic_4ping::index = 0;
    }

    xSemaphoreGiveFromISR(Trig_Sem,false);

}

float Ultra_Sonic_4ping :: ping_get_from_filter()
{
    float d = 0.0;

    d = avg_filter.getValue();

    return d;
}

bool Ultra_Sonic_4ping :: recieve_from_queue(void)
{
    //this will fetch the value from the queue and put it in a buffer(pass-by-ref),0 delay / ticks
    xQueueReceive( xQueue[instance], &buff_for_recieve, 0);
    return true;
}

float Ultra_Sonic_4ping :: get_buffer_value()
{
    return buff_for_recieve;
}

void Ultra_Sonic_4ping :: add_queue_value_to_filter(float a)
{
    avg_filter.addValue(a);
}

int Ultra_Sonic_4ping :: get_zone(float avg)
{

    if( (  avg >= threshold_zero) && ( avg <= threshold_nearest))
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


        static float latest_valuef = 0;
        static float latest_valuer = 0;
        static float latest_valuel = 0;

        static int front_obstacle_zone = 0;
        static  int right_obstacle_zone = 0;
        static  int left_obstacle_zone = 0;

         obs_thre threshold;



      //****************************SENSOR******************************//

        /* Method of working :
         * Ping Sensor 0; Read All Queues
        */


      //TRIGGER TO ONE OF THE SENSORS
      if(Ultra_Sonic_4ping::index == 0)
      {
          front_ultrasonic.send_trig();
     //     printf("front sensor selected\n");

      }
      else if(Ultra_Sonic_4ping::index == 1)
      {
          right_ultrasonic.send_trig();
     //     printf("right sensor selected\n");

       }
      else if(Ultra_Sonic_4ping::index == 2)
      {
          left_ultrasonic.send_trig();
     //     printf("left sensor selected\n");
      }

      if(front_ultrasonic.recieve_from_queue())  //if the queue has a value->returns true
      {
          if(Ultra_Sonic_4ping :: index == 0)
          {
              latest_valuef = front_ultrasonic.get_buffer_value();
          }
      }
 //     front_obstacle_zone = front_ultrasonic.get_zone(latest_valuef);

    //end of front sensor
    //////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////
    //start of right sensor

      if(right_ultrasonic.recieve_from_queue())
      {
          if(Ultra_Sonic_4ping :: index == 1)
          {
                 latest_valuer = right_ultrasonic.get_buffer_value();
          }
      }
 //     right_obstacle_zone = right_ultrasonic.get_zone(latest_valuer);

    //end of right sensor
    //////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////
    //start of left sensor

      if(left_ultrasonic.recieve_from_queue())
      {
          if(Ultra_Sonic_4ping :: index == 2)
          {
              latest_valuel = left_ultrasonic.get_buffer_value();
          }
      }
 //     left_obstacle_zone = left_ultrasonic.get_zone(latest_valuel);

    //////////////////////////////////end of left sensor

      printf("R: %f -- M: %f --L:%f\n",latest_valuer,latest_valuef,latest_valuel);
 //    printf("[%s] -- [%s] -- [%s]   \n", zoneMessage[right_obstacle_zone],zoneMessage[front_obstacle_zone],
 //             zoneMessage[left_obstacle_zone]);

}


