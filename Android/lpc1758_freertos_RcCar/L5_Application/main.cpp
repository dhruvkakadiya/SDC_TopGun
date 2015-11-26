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
 * 			FreeRTOS and stdio printf is pre-configured to use uart0_min.h before main() enters.
 * 			@see L0_LowLevel/lpc_sys.h if you wish to override printf/scanf functions.
 *
 */
#include "tasks.hpp"
#include "examples/examples.hpp"
#include "uart3.hpp"
#include "stdio.h"
#include "semphr.h"
#include "can.h"
#include "bluetooth_controller.hpp"
#include "can_msg_id.h"
#include "gpio.hpp"
#include "char_dev.hpp"

char ch[1000];
char *bt_str = ch;
float way_pt_array[100];
can_msg_t can_waypt_mssg;
can_msg_t bt_can_hb;
can_msg_t kill_mssg;
can_msg_t can_mssg_bt;
uint8_t way_pt_num;
uint8_t way_pt[2];
chk_point_data *waypt_ptr;
bool sync_stat = false;
bool kill_car = true;

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

class bt_uart_task : public scheduler_task
{
    public:
        bt_uart_task(uint8_t priority) :
            scheduler_task("bt_uart_task", bt_task_mem, priority)
        {
        }

        bool init(void)
        {
            Uart3 &bt_uart = Uart3::getInstance();
            bt_uart.init(baud_rate, bt_rx_size, bt_tx_size);
            bt_can_hb.msg_id = BLUETOOTH_HEARTBEAT_ID;
            bt_can_hb.frame_fields.data_len = 0;
            bt_can_hb.frame_fields.is_29bit = 0;
            bt_can_hb.data.qword = 0;
            can_mssg_bt.msg_id = CHECKPOINT_DATA_ID;
            can_mssg_bt.frame_fields.data_len = 8;
            can_mssg_bt.frame_fields.is_29bit = 0;
            waypt_ptr = (chk_point_data*)&(can_mssg_bt.data.bytes[0]);

            return true;
        }

        bool run(void *p)
        {
           bool status_bt_can_tx = false;

           Uart3 &bt_uart = Uart3::getInstance();
           bool uart_stat = false;

           uart_stat = bt_uart.gets(bt_str, bt_data_len, portMAX_DELAY );

           if(uart_stat)
           {
               char *data_ptr = bt_str + 2;
               char *end;
               int loop_var = 0;
               while((*(bt_str + loop_var)) != ' ')
               {
                   way_pt[loop_var] = (uint8_t)(*(bt_str + loop_var) - 48);
                   loop_var++;
               }
               if(loop_var == 2)
               {
                   way_pt_num = (way_pt[0] * 10) + way_pt[1];

               }
               else
                   way_pt_num = way_pt[0];

               PRINT("\nNUM OF WAY POINTS  = %d",way_pt_num);

               if(way_pt_num == 0)
               {
                   kill_car = !kill_car;
                   kill_mssg.msg_id = KILL_SWITCH_ID;
                   kill_mssg.frame_fields.data_len = 8;
                   kill_mssg.frame_fields.is_29bit = 0;

                   if(kill_car == false)
                       kill_mssg.data.qword = start_car_mssg;
                   else
                       kill_mssg.data.qword = stop_car_mssg;

                   PRINT("\n CAN MSSG SENT : %d",kill_mssg.data.bytes[0]);

                   status_bt_can_tx = CAN_tx(can_controller, &kill_mssg, 10);

                   if(status_bt_can_tx == true)
                   {
                       PRINT("\nbt data sent on can bus successful");
                       status_bt_can_tx =  false;
                   }
                   else
                   {
                       PRINT("\nbt data send on can bus unsuccessful");
                   }

               }
               else
               {
                  can_waypt_mssg.msg_id = CHECKPOINT_SEND_ID;
                  can_waypt_mssg.frame_fields.data_len = 8;
                  can_waypt_mssg.frame_fields.is_29bit = 0;

                   for(int i = 0; i < (way_pt_num *2);i++)
                   {
                     way_pt_array[i] = strtof(data_ptr,&end);
                     data_ptr = end;
                     PRINT("\nlat long value is %f\n", way_pt_array[i]);
                   }

                   can_waypt_mssg.data.bytes[0] = way_pt_num;
                   status_bt_can_tx = CAN_tx(can_controller, &can_waypt_mssg, 10);
                   PRINT("Waypoint number can mssg send : %d", status_bt_can_tx);
               }
           }

          return true;
        }
};

int main(void)
{
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
    sync_stat = bluetooth_controller_sync();

    scheduler_add_task(new terminalTask(PRIORITY_HIGH));

    /* Consumes very little CPU, but need highest priority to handle mesh network ACKs */
    scheduler_add_task(new wirelessTask(PRIORITY_CRITICAL));

    scheduler_add_task(new bt_uart_task(PRIORITY_CRITICAL));

#if heart_beat_enable
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

    if(sync_stat ==  true)
        scheduler_start(); ///< This shouldn't return

    return -1;
}
