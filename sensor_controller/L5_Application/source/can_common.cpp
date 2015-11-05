/*
 * File         : can_common.cpp
 * Author       : Dhruv Kakadiya
 * Date         : September 13, 2015
 */

#include "can_common.hpp"

can_msg_t received_msg,tmp_can_message;

static bool bus_off_state = false;
bool test_data(can_msg_t can_message){
    if(can_message.data.qword == tmp_can_message.data.qword){
        return false;
    }
    else {
        tmp_can_message = can_message;
        return true;
    }
}

bool transmit_data(can_msg_t send_message){
    send_message.frame_fields.is_29bit = 0;
    send_message.frame_fields.data_len = 8;       // Send 8 bytes
    if(test_data(send_message) || 1) {
        if(false == CAN_tx(can1, &send_message, 0)) {
            LOG_ERROR("CAN_tx failed\n");
            return false;
        }
        else
            return true;
    }
    else
        return false;
}

bool receive_data(){
    if(false == CAN_rx(can1, &received_msg, 0)) {
        LOG_ERROR("CAN_rx failed\n");
        return false;
    }
    else
        return true;
}

static void can_bus_off_callback(uint32_t dummy)
{
    bus_off_state = true;
    LE.set(1, true);
}

void check_bus_off(void)
{
    if(bus_off_state)
    {
        CAN_reset_bus(can1);
        bus_off_state = false;
        LE.set(1, false);
    }
}

void can_tx_init(void) {
    LE.init();
    CAN_init(can1,100,10,50,can_bus_off_callback,0);
    LOG_INFO("CAN1 is initialized\n");
}

void can_rx_init(void) {
    const can_std_id_t slist[]      = { CAN_gen_sid(can1, RESET_ID), CAN_gen_sid(can1, SENSOR_SYNC_ID) };
    CAN_setup_filter(slist, 2, NULL, 0, NULL, 0, NULL, 0);
}

void can_init(void) {
    can_tx_init();
    can_rx_init();
    CAN_reset_bus(can1);
}
