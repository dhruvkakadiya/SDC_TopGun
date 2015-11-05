/*
 * File         : can_common.cpp
 * Author       : Dhruv Kakadiya
 * Date         : September 13, 2015
 */

#include "can_common.hpp"
#define BUS_OFF_LED                     1
#define RECEIVE_CAN_MOTOR_IO_QUEUE      5
#define TRANSMIT_CAN_MOTOR_IO_QUEUE     5

can_msg_t no_motor_msg,tmp_can_msg;
extern can_msg_t motor_msg;

static bool bus_off_state = false;

bool test_data(can_msg_t can_message){
    if(can_message.data.qword == tmp_can_msg.data.qword){
        return false;
    }
    else {
        tmp_can_msg = can_message;
        return true;
    }
}

bool transmit_data(can_msg_t transmit_msg){
    transmit_msg.frame_fields.is_29bit = 0;
    transmit_msg.frame_fields.data_len = 8;
    if(test_data(transmit_msg)) {
        if(false == CAN_tx(can1, &transmit_msg, 0)) {
            //SET_ERROR(ERROR_TX_FAILED);
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
    // XXX: Receive to a message
    can_msg_t received_msg;
    if(false == CAN_rx(can1, &received_msg, 0)) {
        //SET_ERROR(ERROR_TX_FAILED);
        LOG_ERROR("CAN_rx failed\n");
        return false;
    }
    else {
        if (received_msg.msg_id == MOTOR_DIRECTIONS_ID) {
            motor_msg = received_msg;
        }
        else{
            motor_msg = no_motor_msg;
        }
        return true;
    }
}

static void can_bus_off_callback(uint32_t dummy)
{
    bus_off_state = true;
    LE.set(BUS_OFF_LED, true);
}

void check_bus_off(void)
{
    if(bus_off_state)
    {
        CAN_reset_bus(can1);
        bus_off_state = false;
        LE.set(BUS_OFF_LED, false);
    }
}

void can_tx_init(void) {
    LE.init();
    CAN_init(can1,100,1,5,can_bus_off_callback,0);
    LOG_INFO("CAN1 is initialized\n");
}

void can_rx_init(void) {
    // Set necessary message filter
    const can_std_id_t slist[]      = { CAN_gen_sid(can1, RESET_ID), CAN_gen_sid(can1, MOTORIO_SYNC_ID),
                                        CAN_gen_sid(can1, RUN_MODE_ID), CAN_gen_sid(can1, MOTOR_DIRECTIONS_ID)
                                       };
    CAN_setup_filter(slist, 4, NULL, 0, NULL, 0, NULL, 0);
}

void can_init(void) {
    no_motor_msg.data.bytes[0] = (uint8_t) 0 ;      // Stop speed
    no_motor_msg.data.bytes[1] = (uint8_t) 2 ;      // Straight direction
    can_tx_init();
    can_rx_init();
    CAN_reset_bus(can1);
}
