/*
 * File         : can_common.cpp
 * Author       : Dhruv Kakadiya
 * Date         : September 13, 2015
 */

#include "can_common.hpp"

#define SAME_DATA_TRANSMISSION      1
can_msg_t received_msg,tmp_can_message;

static bool bus_off_state = false;
static bool powerup_sync_sensor_controller( void );

static void can_bus_off_callback(uint32_t dummy)
{
    bus_off_state = true;
    LE.set(ERROR_LED, true);
}

void data_over_callback( uint32_t icr_data )
{
    //LOG_WARN("WARNING!!! Sensor Controller CAN bus overflow!!!");
    CAN_reset_bus(SENSOR_CNTL_CANBUS);
}

void check_bus_off(void)
{
    if(bus_off_state)
    {
        CAN_reset_bus(SENSOR_CNTL_CANBUS);
        bus_off_state = false;
        LE.set(ERROR_LED, false);
    }
}

bool can_init(void) {
    bool status = false;
    LE.init();
    CAN_init(SENSOR_CNTL_CANBUS,SENSOR_CNTL_CANBAUD,SENSOR_CNTL_CANRXQ,SENSOR_CNTL_CANTXQ,can_bus_off_callback,0);
    LOG_INFO("SENSOR_CNTL_CANBUS is initialized\n");
    const can_std_id_t slist[]      = { CAN_gen_sid(SENSOR_CNTL_CANBUS, RESET_ID),
                                        CAN_gen_sid(SENSOR_CNTL_CANBUS, SENSOR_SYNC_ID) };
    CAN_setup_filter(slist, 2, NULL, 0, NULL, 0, NULL, 0);
    CAN_reset_bus(SENSOR_CNTL_CANBUS);

    // Sync with the master controller by sending power_up_sync
    status = true;//powerup_sync_sensor_controller();
    return status;
}

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
    if(test_data(send_message) || SAME_DATA_TRANSMISSION) {
        if(false == CAN_tx(SENSOR_CNTL_CANBUS, &send_message, 0)) {
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
    if(false == CAN_rx(SENSOR_CNTL_CANBUS, &received_msg, 0)) {
        LOG_ERROR("CAN_rx failed\n");
        return false;
    }
    else
        return true;
}

// If get reset command from Master then reboot the whole system
void sensor_get_master_reset( void )
{
    LOG_ERROR("ERROR!!! Received a reset request from master\n");
    sys_reboot();
}

// PowerUp Sync function for Sensor Controller
bool powerup_sync_sensor_controller( void )
{
    can_msg_t sensor_sync_msg;
    uint8_t sync_miss_count = 0;
    master_sync* sync_msg;
    bool sync_ack = false;
    bool status = false;

    SoftTimer can_rx_timer(SENSOR_CNTL_SYNC_TIME);

    sensor_sync_msg.msg_id = SENSOR_SYNC_ID;
    sensor_sync_msg.frame_fields.is_29bit = 0;
    sensor_sync_msg.frame_fields.data_len = 0;     // No data
    sensor_sync_msg.data.qword = 0;

    do
    {
        // Send sync message
        status = CAN_tx(SENSOR_CNTL_CANBUS, &sensor_sync_msg, SENSOR_CNTL_CAN_TIMEOUT);

        if( !status )
        {
            LOG_ERROR("ERROR!!! Unable to send SENSOR controller sync message\n");
            LE.on(SENSOR_CAN_ERR_LED);
        }

        else
            LE.off(SENSOR_CAN_ERR_LED);

        // No need to delay here
        // XXX: Cannot use FreeRTOS functions until the OS runs
        // vTaskDelayMs(SENSOR_CNTL_SYNC_TIME);


        can_rx_timer.restart();
        while( !can_rx_timer.expired() );


        // Check for ack
        status = CAN_rx(SENSOR_CNTL_CANBUS, &received_msg, 0);

        // XXX: Since OS is not up, maybe you can use SoftTime (polling timer)
        if( status )
        {
            // We have a new message. Check if Sensor is acked

            // XXX: Suggest "shared" structures rather than memcpy
            sync_msg = (master_sync *)&received_msg.data.bytes[0];
            if( sync_msg->ack_geo )
                sync_ack = true;

        }

        sync_miss_count++;

    }while( ( sync_ack == false ) );    // try forever

#if 0
    if( !sync_ack )
        LOG_ERROR("ERROR!!! Sync timeout from Sensor controller. Stalling!!!\n");
#endif

    return sync_ack;
}

void sensor_send_heartbeat( void )
{
    // Heart Beat to Master
    can_msg_t heartbeat_sensor_msg; // Can Message
    bool can_status = false;

    heartbeat_sensor_msg.msg_id = GEO_HEARTBEAT_ID; // Sensor Heartbeat ID
    heartbeat_sensor_msg.frame_fields.is_29bit = 0;
    heartbeat_sensor_msg.frame_fields.data_len = 0;

    can_status = CAN_tx(SENSOR_CNTL_CANBUS, &heartbeat_sensor_msg, SENSOR_CNTL_CAN_TIMEOUT);

    if( !can_status )
    {
        LOG_ERROR("ERROR!!! Sensor controller Heartbeat message not sent!!");
        LE.off(SENSOR_HB_LED);
    }
    else
    {
        LE.toggle(SENSOR_HB_LED);
    }

    if(bus_off_state) {
        CAN_reset_bus(SENSOR_CNTL_CANBUS);
        bus_off_state = false;
    }
}
