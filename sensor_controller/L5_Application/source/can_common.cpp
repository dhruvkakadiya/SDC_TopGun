/*
 * File         : can_common.cpp
 * Author       : Dhruv Kakadiya
 * Date         : September 13, 2015
 */

#include "can_common.hpp"
#define HEART_BEAT_MASTER 0
#define HEARTBEAT_SLAVE 1


//QueueHandle_t master_can_task= xQueueCreate(10,sizeof(motor_direction));

uint8_t error_flag;
can_msg_t message1,message2,tmp_can_message;

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
    //send_message.data.bytes[0] =13;
    if(test_data(send_message) || 1) {
        if(false == CAN_tx(can1, &send_message, 0)) {
            //SET_ERROR(ERROR_TX_FAILED);
            LOG_ERROR("CAN_tx failed\n");
            //printf("[0x%02X] CAN_tx failed\n", error_flag);
            //CAN_reset_bus(can1); // reset CAN bus, because bus might be in bus off state
            return false;
        }
        else
            return true;
    }
    else
        return false;
}

bool receive_data(){
    if(false == CAN_rx(can1, &message2, 0)) {
        SET_ERROR(ERROR_TX_FAILED);
        LOG_ERROR("CAN_rx failed\n");
        //printf("[0x%02X] CAN_rx failed\n", error_flag);
        //CAN_reset_bus(can1); // reset CAN bus, because bus might be in bus off state
        return false;
    }
    else
        return true;
}

#if 1
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
#endif

void can_tx_init(void) {
    LE.init();
    CAN_init(can1,100,10,50,can_bus_off_callback,0);
    LOG_INFO("CAN1 is initialized\n");
    //CAN_reset_bus(can1);
}

void can_rx_init(void) {
    const can_std_id_t slist[]      = { CAN_gen_sid(can1, RESET_ID), CAN_gen_sid(can1, SENSOR_SYNC_ID) };
    CAN_setup_filter(slist, 2, NULL, 0, NULL, 0, NULL, 0);
    CAN_reset_bus(can1);
}

void can_init(void) {
    can_tx_init();
    can_rx_init();
}


