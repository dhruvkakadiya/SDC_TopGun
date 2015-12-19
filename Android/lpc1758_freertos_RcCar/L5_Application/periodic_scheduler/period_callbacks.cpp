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
 * This contains the period callback functions for the periodic scheduler
 *
 * @warning
 * These callbacks should be used for hard real-time system, and the priority of these
 * tasks are above everything else in the system (above the PRIORITY_CRITICAL).
 * The period functions SHOULD NEVER block and SHOULD NEVER run over their time slot.
 * For example, the 1000Hz take slot runs periodically every 1ms, and whatever you
 * do must be completed within 1ms.  Running over the time slot will reset the system.
 */

#include <stdint.h>
#include "stdio.h"
#include "io.hpp"
#include "periodic_callback.h"
#include "can.h"
#include "can_msg_id.h"
#include "bluetooth_controller.hpp"

#define BT_CAN_HB_WAIT 0

extern bool can_bus_off_flag;
extern can_msg_t bt_can_hb;
extern float way_pt_array[100];
float geo_data_arr[2];
extern uint8_t way_pt_num;
can_msg_t waypt_ack_mssg;
can_msg_t geo_data_msg;
bool waypt_ack = false;
bool data_send_flag = false;
int bt_send_loop = 0;
extern can_msg_t can_mssg_bt;
extern chk_point_data *waypt_ptr;
geo_loc *geo_ptr;

/// This is the stack size used for each of the period tasks
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);

void period_1Hz(void)
{
    if(can_bus_off_flag)
        {
                CAN_reset_bus(can_controller);
                can_bus_off_flag = false;
        }
        else
        {
            CAN_tx(can_controller,&bt_can_hb,BT_CAN_HB_WAIT);
        }
           geo_ptr = (geo_loc*)&(geo_data_msg.data.bytes[0]);
           CAN_rx(can_controller, &geo_data_msg, 0);
           geo_data_arr[0] = geo_ptr->latitude;
           geo_data_arr[1] = geo_ptr->longitude;

}

void period_10Hz(void)
{
    if(!waypt_ack)
    {
        if(CAN_rx(can_controller, &waypt_ack_mssg, 0))
        {
            if((waypt_ack_mssg.msg_id == CHECKPOINT_REQ_ID) && (waypt_ack_mssg.data.bytes[0] == 1))
            {
                PRINT("\nACK RECEIVED");
                waypt_ack = true;
                data_send_flag = true;
                bt_send_loop = 0;
            }
        }
     }
    else if(data_send_flag)
    {
        if(bt_send_loop < (way_pt_num * 2))
        {
            waypt_ptr->latitude = way_pt_array[bt_send_loop];
            waypt_ptr->longitude = way_pt_array[bt_send_loop + 1];
            bt_send_loop += 2;
            CAN_tx(can_controller, &can_mssg_bt, 0);
            PRINT("\nway pt data sent");
            }
        else
        {
            data_send_flag = false;
            waypt_ack = false;
        }
    }
}

void period_100Hz(void)
{

}

void period_1000Hz(void)
{

}
