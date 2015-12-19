/*
 * bluetooth_controller.cpp
 *
 *  Created on: Oct 30, 2015
 *      Author: Anush Shankar
 */

#include "stdio.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "can.h"
#include "bluetooth_controller.hpp"
#include "can_msg_id.h"
#include "file_logger.h"
#include "soft_timer.hpp"

static can_msg_t tx_mssg;
static can_msg_t master_rx_mssg;
bool can_bus_off_flag = false;

void bus_off_cb( uint32_t icr_data )
{
    can_bus_off_flag = true;
}

void data_ovr_cb( uint32_t icr_data )
{

}

bool bluetooth_controller_sync()
{
    bool can_init_stat = false;

    bool sync_tx_status = false;

    bool sync_ack_status = true;

    bool status = false;

    SoftTimer can_rx_timer(BT_SYNC_TIME);

    //Initialize can bus for sync frame transmission
    can_init_stat = CAN_init(can_controller, can_baud_kbps, can_rx_queue_size, can_tx_queue_size, bus_off_cb, data_ovr_cb);

    PRINT("can init started\n");

    master_sync *ref_obj;
    if(!can_init_stat)
    {
        LOG_ERROR("Bluetooth controller CAN bus could not be initialiazed");
        return can_init_stat;
    }
    else
        PRINT("\ncan init successful");

    const can_std_id_t slist[]      = {CAN_gen_sid(can_controller, MASTER_SYNC_ACK_ID), CAN_gen_sid(can_controller, CHECKPOINT_REQ_ID), CAN_gen_sid(can_controller, GEO_LOC_DATA_ID), CAN_gen_sid(can_controller, 0xFFFF)};
    CAN_setup_filter(slist, 4, NULL, 0, NULL, 0, NULL, 0);
    //CAN_bypass_filter_accept_all_msgs();
    CAN_reset_bus(can_controller);

    tx_mssg.msg_id = BLUETOOTH_SYNC_ID;
    tx_mssg.frame_fields.data_len = 0;
    tx_mssg.frame_fields.is_29bit = 0;
    tx_mssg.data.qword = 0;

    do
    {
        PRINT("\n can tx status = %d",sync_tx_status);

        sync_tx_status = CAN_tx(can_controller, &tx_mssg,BT_CNTRL_TIMEOUT_CAN);

        PRINT("\ncan tx started");
        PRINT("\n can tx status = %d",sync_tx_status);

        if(sync_tx_status)
        {

            PRINT("\ncan tx successful no errors");

        }

        else
        {
            LOG_ERROR("Bluetooth can message cannot be sent");

           PRINT("\ncan tx unsuccessfull");

        }

        can_rx_timer.restart();
        while(!can_rx_timer.expired());

        status = CAN_rx(can_controller,&master_rx_mssg, BT_CNTRL_TIMEOUT_CAN);

        PRINT("\ncan rx started");

        if(status)
        {
            ref_obj = (master_sync*)&(master_rx_mssg.data.bytes[0]);

            PRINT("\ncan rx successful");

            if((master_rx_mssg.msg_id == MASTER_SYNC_ACK_ID) && (ref_obj->ack_bluetooth == 1))
                sync_ack_status = true;
        }

    }while(sync_ack_status == false);

    PRINT("\nack received successful\n");

    return sync_ack_status;
}






