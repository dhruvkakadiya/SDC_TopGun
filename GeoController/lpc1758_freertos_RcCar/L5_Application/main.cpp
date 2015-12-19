/*
 *     SocialLedge.com - Copyright (C) 2013
 *
 *     This file is part of free software framework for embedded processors.
 *     You can use it and/or distribute it as long as this copyright header
 *     remains unmodified.  The code is free for personal use and requires
 *     permission to use in a commercial product.
 *
 *      THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 *      OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 *      MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *      I SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 *      CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *     You can reach the author of this software at :
 *          p r e e t . w i k i @ g m a i l . c o m
 */

/**
 * @file
 * @brief This is the application entry point.
 *          FreeRTOS and stdio printf is pre-configured to use uart0_min.h before main() enters.
 *          @see L0_LowLevel/lpc_sys.h if you wish to override printf/scanf functions.
 *
 */
#include "tasks.hpp"
#include "examples/examples.hpp"
#include "geo_controller.hpp"
#include "io.hpp"
#include "imu.hpp"
#include "stdio.h"
#include "stdlib.h"
#include "GPIO.hpp"
#include "queue.h"
#include "GPS.hpp"
#include "uart2.hpp"
#include "utilities.h"
#include "printf_lib.h"

QueueHandle_t gps_queue1;
volatile bool flag_received = false;
char gps_global_string[100];


geo_location gps_data_dec;

uint8_t speed_gps;

 extern "C"
 {
    void UART2_IRQHandle()
    {
        static  char GPS_buffer1[100];
        static int i_new=0;
        while(!(LPC_UART2-> LSR & 0x01));
        GPS_buffer1[i_new]= LPC_UART2 -> RBR;
        if(GPS_buffer1[i_new]=='\n') {
           GPS_buffer1[i_new]='\0';
           flag_received = true;
           if(xQueueSendFromISR(gps_queue1, &GPS_buffer1 , 0)) {

           }
           else{

           }
           i_new=0;
        }
        else{
        i_new++;
        }
    }
 }

/**
 * The main() creates tasks or "threads".  See the documentation of scheduler_task class at scheduler_task.hpp
 * for details.  There is a very simple example towards the beginning of this class's declaration.
 *
 * @warning SPI #1 bus usage notes (interfaced to SD & Flash):
 *      - You can read/write files from multiple tasks because it automatically goes through SPI semaphore.
 *      - If you are going to use the SPI Bus in a FreeRTOS task, you need to use the API at L4_IO/fat/spi_sem.h
 *
 * @warning SPI #0 usage notes (Nordic wireless)
 *      - This bus is more tricky to use because if FreeRTOS is not running, the RIT interrupt may use the bus.
 *      - If FreeRTOS is running, then wireless task may use it.
 *        In either case, you should avoid using this bus or interfacing to external components because
 *        there is no semaphore configured for this bus and it should be used exclusively by nordic wireless.
 */

 void gps_uart(void *p)
 {
     while(1){
         if(xQueueReceiveFromISR(gps_queue1,&gps_global_string,0)) {
             if(flag_received==true)
              {

                 static int lat_degree = 0;
                 static float lat_minute = 0;
                 static int long_degree = 0;
                 static float long_minute = 0;
                 static float lat_dec;
                 static float long_dec;

                 if(gps_global_string[18] == 'A')
                    {

                     lat_degree = get_lat_degree1();
                     lat_minute = get_lat_minute1();
                     long_degree =get_long_degree1();
                     long_minute =get_long_minute1();
                     speed_gps = (uint8_t)(1.150779)*get_speed_GPS1();

                     lat_dec = get_decimal1(lat_degree, lat_minute);
                     long_dec = get_decimal1(long_degree, long_minute);

                     gps_data_dec.latitude = lat_dec;
                     gps_data_dec.longitude = (-1)*long_dec;
                     flag_received = false;

                    }

                 if(gps_global_string[18] == 'V') {
                     gps_data_dec.latitude = 0.0;
                     gps_data_dec.longitude = 0.0;
                     flag_received = false;
                 }
              }
         }

     }
 }


int main(void)
{
    gps_queue1 = xQueueCreate(2, 100 * sizeof(char));
    GPIO myPin(P2_5);   // Control P1.19
    myPin.setAsOutput();
    myPin.setHigh();

  /* GPIO myPin_IPU(P2_2);   // Control P1.19
      myPin_IPU.setAsOutput();
      myPin_IPU.setLow();
      vTaskDelay(10);
      myPin_IPU.setHigh();
      */

   char b[] = "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n";
   char c[] =  "$PMTK220,100*2F\r\n";
   int i = 0;
   uart2init();
   for( i = 0; b[i] != '\0'; i++)
   {
       LPC_UART2 ->THR = b[i];
       delay_ms(1);
   }
    i =0;
   for( i = 0; c[i] != '\0'; i++)
      {
          LPC_UART2 ->THR = c[i];
          delay_ms(1);
      }
   NVIC_EnableIRQ(UART2_IRQn);

#if 0
    while(1){
        //printf("hello\n");
        if(flag_received==true)
        {
            printf("%s\n",GPS_new_buffer1);
            flag_received = false;
        }

        else{
           // printf("Failed:\n");
        }
        //delay_ms(10);
       // vTaskDelay(10);
    }
#endif

   xTaskCreate(gps_uart, "GPS_UART", 2048, NULL,1,NULL);

   /**
     * A few basic tasks for this bare-bone system :
     *      1.  Terminal task provides gateway to interact with the board through UART terminal.
     *      2.  Remote task allows you to use remote control to interact with the board.
     *      3.  Wireless task responsible to receive, retry, and handle mesh network.
     *
     * Disable remote task if you are not using it.  Also, it needs SYS_CFG_ENABLE_TLM
     * such that it can save remote control codes to non-volatile memory.  IR remote
     * control codes can be learned by typing the "learn" terminal command.
     */
    scheduler_add_task(new terminalTask(PRIORITY_HIGH));

    /* Consumes very little CPU, but need highest priority to handle mesh network ACKs */
   // scheduler_add_task(new wirelessTask(PRIORITY_CRITICAL));

    /* Call controller init routine before starting periodic call backs */
   // bool status = geo_controller_init();


    /* If init failed there is no point in continuing */
    // XXX: Keep trying to sync forever, no point dying here
    // DONE: The init function will try to sync forever. This case should never happen
  //  init_timer.reset();
  //  if( !status )
  //  {
  //      printf("ERROR!!!! Geo Controller - This should never happen\n");

      //  while( 1 )
      //  {
      //      LE.toggle(1);
      //      init_timer.restart();
      //      while( !init_timer.expired());
     //   }
   // }




   scheduler_add_task(new IMUTask(PRIORITY_CRITICAL));
 //  scheduler_add_task(new GPSTask(PRIORITY_CRITICAL));

    /* Call controller init routine before starting periodic call backs */
    geo_controller_init();

    /* Change "#if 0" to "#if 1" to run period tasks; @see period_callbacks.cpp */
    #if 1
    scheduler_add_task(new periodicSchedulerTask());
    #endif

    /* The task for the IR receiver */
    // scheduler_add_task(new remoteTask  (PRIORITY_LOW));

    /* Your tasks should probably used PRIORITY_MEDIUM or PRIORITY_LOW because you want the terminal
     * task to always be responsive so you can poke around in case something goes wrong.
     */

    /**
     * This is a the board demonstration task that can be used to test the board.
     * This also shows you how to send a wireless packets to other boards.
     */
    #if 0
        scheduler_add_task(new example_io_demo());
    #endif

    /**
     * Change "#if 0" to "#if 1" to enable examples.
     * Try these examples one at a time.
     */
    #if 0
        scheduler_add_task(new example_task());
        scheduler_add_task(new example_alarm());
        scheduler_add_task(new example_logger_qset());
        scheduler_add_task(new example_nv_vars());
    #endif

    /**
     * Try the rx / tx tasks together to see how they queue data to each other.
     */
    #if 0
        scheduler_add_task(new queue_tx());
        scheduler_add_task(new queue_rx());
    #endif

    /**
     * Another example of shared handles and producer/consumer using a queue.
     * In this example, producer will produce as fast as the consumer can consume.
     */
    #if 0
        scheduler_add_task(new producer());
        scheduler_add_task(new consumer());
    #endif

    /**
     * If you have RN-XV on your board, you can connect to Wifi using this task.
     * This does two things for us:
     *   1.  The task allows us to perform HTTP web requests (@see wifiTask)
     *   2.  Terminal task can accept commands from TCP/IP through Wifly module.
     *
     * To add terminal command channel, add this at terminal.cpp :: taskEntry() function:
     * @code
     *     // Assuming Wifly is on Uart3
     *     addCommandChannel(Uart3::getInstance(), false);
     * @endcode
     */
    #if 0
        Uart3 &u3 = Uart3::getInstance();
        u3.init(WIFI_BAUD_RATE, WIFI_RXQ_SIZE, WIFI_TXQ_SIZE);
        scheduler_add_task(new wifiTask(Uart3::getInstance(), PRIORITY_LOW));
    #endif

    scheduler_start(); ///< This shouldn't return
    return -1;
}
