/*
 * ultrasonic_sensor.hpp
 *
 *  Created on: Sep 10, 2015
 *      Author: Hemanth K N and Akshay Vijaykumar
 */


#ifndef L4_IO_PING_ULTRASONIC_SENSOR_INTERRUPTS_HPP_
#define L4_IO_PING_ULTRASONIC_SENSOR_INTERRUPTS_HPP_

/* MODULE USAGE :
 * In the 10hz periodic task, Call US_FRONT.ReadRange();
 * This is followed by US_FRONT.GetFilteredRangeValue());
 * ReadRange would read the current value while GetFilteredRangeValue would
 * provide a filtered value based on the past 5 range readings.
 *
 * Make sure to include ultrasonic_sensor.hpp in the period_callbacks.cpp file
 * */


#include "gpio.hpp"
#include "LPC17xx.h"               // For Pin Select
#include "singleton_template.hpp"  // Singleton Template
#include "circular_buffer.hpp"
#include "FreeRTOS.h"
#include "semphr.h"
#include "projdefs.h"
#include "file_logger.h"
#include "stdio.h"
#include "can.h"    //sync up and heart beat
#include "can_msg_id.hpp" //sync up and heart beat
#include "io.hpp"
#include <cstring>   //for memcpy


#define HEARTBEAT ( 0 )    //1Hz periodic
#define ZONE_INFO ( 1 )
#define SENSOR_SEND_ON_CAN ( 1 )

#define SINGLE_SENSOR ( 0 )     //testing only back sensor 4 pin ping sensor

#define THREE_PIN    (1)      //Pinging three pin sensor

/* Custom Debug Print Function */
#if 0
#define PRINTME(...) LOG_DEBUG(__VA_ARGS__)
#else
#define PRINTME(...)
#endif

#define USONIC_SUPPLY_VCC               ( 3.3 )
#define USONIC_FRONTSENSOR_ADCCHANNEL   ( 4 )
#define USONIC_VOLTAGE_PER_INCH         ( (float)( (float)USONIC_SUPPLY_VCC/512) )    // From datasheet in v
#define ADC04_PINSELECT_VALUE           (  0x00000003 )
#define ADC04_PINSELECT_SHIFT           ( 28 )
#define ADC_VOLTAGE_PER_STEP            ( (float)( (float) 3/4096 ) )    // 12-bit ADC has 4096 steps across 3000mVolts

#define OFF                             ( false )
#define ON                              ( true )

#define OVERWRITE_OLD_DATA              ( true )

#define FILTER_ACCESS_TIMEOUT           ( 1 )

#define MAX_SENSOR_COUNT              (3)   //PING SENSOR

#define PING_CAN                      (can1) //Ping to can1
#define PING_BAUD                     (100)  //baud rate
#define PING_TIMEOUT                  (0)    //timeout
#define PING_HEARTBEAT_ERROR_LED      (4)    //LED4 for heartbeat error
#define PING_ZONE_SENDING_ERROR_LED   (4)    //LED3 for zone sending error
#define PING_CAN_BO                (1)    //LED1 for can bus off

#define RIGHT_LED ( 1)
#define FRONT_LED ( 2 )
#define LEFT_LED ( 3 )
#define MAX_VALUE     (400)
#define TEST_SIX                       (0) //Testing 6 sensors


// This enumeration matches the distance of the obstacle for PING SENSOR


//char zoneMessage[4][10] = {"N", "M", "F", "P"};

//This enumeration matches the threshold of the obstacle for PING SENSOR
#define TEST_INPUTS 1

#if 0// TEST_INPUTS

typedef enum{
    threshold_zero    = 0 ,
    threshold_nearest = 60,
    threshold_near    = 120,
    threshold_middle  = 190,
    threshold_far      = 300,
} obs_thre;

//#else

typedef enum{
    threshold_zero    = 0 ,
    threshold_nearest = 50,
    threshold_near    = 100,
    threshold_middle  = 300,
    threshold_far      = 400,
} obs_thre;
#endif

/* Sensor Filter Template
 * Performs Average Filtering.
 * This class can be inherited with other sensor classes for filtering
 * */

template <typename INTYPE, typename OUTTYPE>
class Sensor_Filter
{
    private:
        CircularBuffer <INTYPE> cBuf; /* Circular Buffer holding Sensor Values */
        double currentAverage;
        SemaphoreHandle_t filterAccess;
        bool stableStream;

        /* This function calculates the average of values in cBuf */
        void updateAverage( INTYPE data)
        {
            double tempVal = 0;
            float mulFactor = 0;
            INTYPE oldElement = 0;

            if( !stableStream )
            {
                /* Add data to Buffer */
                cBuf.push_back(data);
                mulFactor = 1/(double)(cBuf.size());

                /* If number of elements in the buffer < total buffer capacity
                 * Calculate the average using loops */

                for(int i = 0; i < (int)cBuf.size(); i++)
                {
                    tempVal += cBuf[i];
                }

                tempVal = tempVal * mulFactor;

                currentAverage = tempVal;

                stableStream = (cBuf.size() == cBuf.capacity());
            }
            else
            {
                /* Efficient way of calculating average in case the buffer is full
                 * Every calculation will update the current average value.
                 * To get new average:
                 * new_average =
                 *         current_average +
                 *         ( new_element - oldest_element ) * mulFactor
                 *
                 * The CircularBuffer at this point is Full
                 * */

                oldElement = cBuf.peek_front();
                /* Adding new data Element */

                cBuf.push_back(data, OVERWRITE_OLD_DATA);

                /* We now have a pointer to the old element in the buffer */
                tempVal = currentAverage;

                tempVal = tempVal + ( (  data - oldElement ) / ( cBuf.capacity() ) );

                currentAverage = tempVal;

            }

        }

    public:

        Sensor_Filter(uint32_t bufSize = 3):
            cBuf(bufSize),
            currentAverage(0),
            stableStream(false)
            //bufSize = 5 => To comply Ultra_Sonic_4ping's constructor
        {
            filterAccess = xSemaphoreCreateMutex();
            /* Nothing to be done */
        }

        void addValue(INTYPE data)
        {
            if( xSemaphoreTake(filterAccess, FILTER_ACCESS_TIMEOUT) )
            {
                /*  Push Value into FIFO and update Average */
                updateAverage(data);
                xSemaphoreGive(filterAccess);
            }
            else
            {
                printf("Semaphore not available for writing to filter\n");
            }
        }

        OUTTYPE getValue(void)
        {
            OUTTYPE tempVar = 0;

            if( xSemaphoreTake(filterAccess, FILTER_ACCESS_TIMEOUT) )
            {
                /* Return the current Average Value */
                tempVar = static_cast<OUTTYPE> (currentAverage);

                xSemaphoreGive(filterAccess);
            }

            else
            {
                printf("Semaphore not available for reading from filter\n");
            }

            return tempVar;


        }
};

/* Ultrasonic Sensor Class
 * */
class UltrasonicSensor
{
    private:
        GPIO GpioPower;     /* GPIO pin used as the Power Supply to the Ultrasonic Sensor */
        Sensor_Filter <double, double> avg_filter; /* Sensor Filter applied to Ultrasonic Sensor */
        bool isFilterUsed; /* Holds information of usage of a filter */

    protected:

        UltrasonicSensor(LPC1758_GPIO_Type Power_Pin, bool filterOpt = false, int filterSize = 0):
            GpioPower(Power_Pin),
            avg_filter(filterSize),
            isFilterUsed(filterOpt)
        {
            /* Configuring the GpioPower pin as Output */
            GpioPower.setAsOutput();
        }


    public:
        /* Status Variables */

        inline bool getFilterUsage(void)
        {
            return isFilterUsed;
        }

        virtual void TogglePower(bool power);             // Power On/Off the sensor
        virtual void Recalibrate();                       // Recalibrate the sensor [ power cycle ]
        virtual float ReadRange(void);                    // Return range from the sensor
        virtual float GetFilteredRangeValue(void);        // Return range value from the sensor

};

/* Ultrasonic Sensor at Front
 * Uses P2_0 to Power it on.
 * Uses ADC0.4.
 * */
class UsonicFrontSensor : public UltrasonicSensor, public SingletonTemplate<UsonicFrontSensor>
{
    private:
        UsonicFrontSensor():
            UltrasonicSensor(P2_0, true, 10)   /* Interface front sensor power to PIO2.0; Filtering True; 10 Elements */
            {
                init();
            }

        void init()
        {
            /*
             *  We have interfaced this sensor to ADC0.4
             *  Setting the corresponding PinMux
             */

            LPC_PINCON->PINSEL3 |= (ADC04_PINSELECT_VALUE << ADC04_PINSELECT_SHIFT);

            // Power on the sensor
            TogglePower(ON);

        }

        friend class SingletonTemplate<UsonicFrontSensor>;

};


//Divya editing UltraSonic Ping sensor (4 pin)
class Ultra_Sonic_4ping
{
    public ://constructor for a 4-pin ping sensor
            Ultra_Sonic_4ping( LPC1758_GPIO_Type trig_pin, LPC1758_GPIO_Type echo_pins, int index1);
            bool send_trig();
            static void echo_high_callback(void );  //Callback function for the Rising edge
            static void echo_low_callback(void );   //Callback function for the falling edge
            double ping_get_from_filter(void);//
            bool recieve_from_queue(void);          //Returns a false if the queue is empty
                                                    //Returns a true if the queue is returning
            double get_buffer_value();               //Returns a float buffer in which the queue element is being put
            void add_queue_value_to_filter(void);//Add the queue value to the filter
            static int get_zone(float avg);            //Returns the Zone of the obstacle
            static void display_zone(int a);
            static int index;
            static double distance_value;


            inline bool pinNotLow(void)
            {
                return echo_in.read();
            }
            static bool max_time_flag;
            int instance;

    private :
             GPIO trig_out;
             GPIO echo_in;
             int echo_pin;
             static SemaphoreHandle_t Trig_Sem;
             Sensor_Filter <double, double> avg_filter;
             //Parameters for ping callbacks functions
             static uint64_t up_time;
             static uint64_t down_time;
             static uint64_t  diff_time;
             //Queue for sharing the distance values
             //static QueueHandle_t xQueue1,xQueue2,xQueue3;
             static QueueHandle_t xQueue[MAX_SENSOR_COUNT];
             double buff_for_recieve;
           //  int instance;
             static int zone;
             static int i;


};

//Get the sensor readings with interrupts
void interrupt_based_ping_sensor();

//Power sync up
void ping_powerupsync(void);

//Heartbeat
void ping_heartbeat(void);
bool bus_reset();

void test_bus_off_cb(uint32_t d);
void data_ovr_cb(uint32_t d);


// Macro to get Singleton Instance
#define US_FRONT UsonicFrontSensor::getInstance()

//other helper function -- current index
Ultra_Sonic_4ping* ping_get_current_instance();
int ping_get_prev_instance(Ultra_Sonic_4ping** q);

//Divya editing UltraSonic Ping sensor (4 pin)
class three_pin
{
    public ://constructor for a 4-pin ping sensor
            three_pin( LPC1758_GPIO_Type trig_pin,LPC1758_GPIO_Type echo_pins, int index1);
            bool send3_trig();
            static void echo3_high_callback(void );  //Callback function for the Rising edge
            static void echo3_low_callback(void );   //Callback function for the falling edge
            double ping3_get_from_filter(void);//
            bool recieve3_from_queue(void);          //Returns a false if the queue is empty
                                                    //Returns a true if the queue is returning
            double get3_buffer_value();               //Returns a float buffer in which the queue element is being put
            void add3_queue_value_to_filter(void);//Add the queue value to the filter
            static int get3_zone(float avg);            //Returns the Zone of the obstacle
            static void display3_zone(int a);
            static int index;
            static double distance_value;

            void enable3_interrupt();
            void disable3_interrupt();

            inline bool pinNotLow(void)
            {
                return echo_in.read();
            }
            static bool max_time_flag;
            int instance;

    private :
             GPIO trig_out;
             GPIO echo_in;
             int echo_pin;
             static SemaphoreHandle_t Trig_Sem;
             Sensor_Filter <double, double> avg_filter;
             //Parameters for ping callbacks functions
             static uint64_t up_time;
             static uint64_t down_time;
             static uint64_t  diff_time;
             //Queue for sharing the distance values
             //static QueueHandle_t xQueue1,xQueue2,xQueue3;
             static QueueHandle_t xQueue[MAX_SENSOR_COUNT];
             double buff_for_recieve;
           //  int instance;
             static int zone;
             static int j;

};
three_pin* ping3_get_current_instance();
int ping3_get_prev_instance(three_pin** q);

void three_pin_sensor();
#endif /* L4_IO_3ULTRASONIC_SENSOR_INTERRUPTS_HPP_ */
