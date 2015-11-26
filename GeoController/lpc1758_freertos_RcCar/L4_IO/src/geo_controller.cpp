/*
 * geo_controller.cpp
 *
 *  Created on: Oct 25, 2015
 *      Author: Chitrang
 */

#include <string.h>
#include "geo_controller.hpp"
#include "can_msg_id.h"
#include "io.hpp"
#include "can.h"
#include "soft_timer.hpp"
#include "imu.hpp"
#include "lpc_sys.h"
#include "GPS.hpp"
#include "stdio.h"
#include "stdlib.h"
#include <math.h>

#define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)
#define radiansToDegrees(angleRadians) (angleRadians * 180.0 / M_PI)

static bool power_up_sync_geo_controller( void );
static can_std_id_t can_id_sync_ack;       // Ack from master
static can_std_id_t can_id_rst;
static bool bus_off_flag = false;
extern geo_location gps_data_dec;
extern uint8_t speed_gps;
static can_std_id_t can_id_loc_update;     // Location update from master

bool geo_controller_init( void )
{

    can_std_id_t dummy;
    bool status = false;

    can_id_sync_ack = CAN_gen_sid(GEO_CNTL_CANBUS, MASTER_SYNC_ACK_ID);
    can_id_rst = CAN_gen_sid(GEO_CNTL_CANBUS, RESET_ID);
    dummy = CAN_gen_sid(GEO_CNTL_CANBUS, 0xFFFF );      // Dummy entry for pairing
    can_id_loc_update = CAN_gen_sid(GEO_CNTL_CANBUS, GEO_LOC_UPDATE_ID);

    // Initialize the can bus
    status = CAN_init(GEO_CNTL_CANBUS, GEO_CNTL_BAUD, GEO_CNTL_CANRXQ, GEO_CNTL_CANTXQ , bus_off_cb, data_ovr_cb);
    if( !status )
    {
        LOG_ERROR("ERROR!!! Cannot initialize GEO controller CAN Bus!!");
        return status;
    }
    // Setup full can filters
    status = CAN_fullcan_add_entry(GEO_CNTL_CANBUS, can_id_rst, can_id_sync_ack);

    status = CAN_fullcan_add_entry(GEO_CNTL_CANBUS, can_id_loc_update, dummy);

    if( !status )
    {
        LOG_ERROR("ERROR!!! Cannot add FullCAN entries to GEO controller CAN Bus!!");
        return status;
    }

    // Enable the bus
    CAN_reset_bus(GEO_CNTL_CANBUS);
    // Sync with the master controller by sending power_up_sync
    //  status = power_up_sync_geo_controller();
    return status;
}

void bus_off_cb( uint32_t icr_data )
{
    // XXX: This callback occurs from inside an ISR, so cannot log or print anything
    // Also, bus reset should happen at maybe 1 or 10Hz, but not immediately
    //LOG_ERROR("ERROR!!! GEO Controller CAN bus in off state. Resetting bus");
    bus_off_flag = true;
}

void data_ovr_cb( uint32_t icr_data )
{
    //LOG_WARN("WARNING!!! GEO Controller CAN bus overflow!!!");
    CAN_reset_bus(GEO_CNTL_CANBUS);
}

bool power_up_sync_geo_controller( void )
{
    can_msg_t geo_sync_msg;
    can_fullcan_msg_t *geo_syncack_msg_ptr = NULL;
    can_fullcan_msg_t geo_syncack_msg_copy;
    uint8_t sync_miss_count = 0;
    bool sync_ack = false;
    bool status = false;
    master_sync* sync_msg;
    SoftTimer can_rx_timer(GEO_CNTL_SYNC_TIME);

    geo_syncack_msg_ptr = CAN_fullcan_get_entry_ptr(can_id_sync_ack);
    if( geo_syncack_msg_ptr == NULL )
    {
        LOG_ERROR("ERROR!!! Cannot get FullCan msg pointer for Geo controller power-up sync\n");
        return false;
    }

    geo_sync_msg.msg_id = GEO_SYNC_ID;
    geo_sync_msg.frame_fields.is_29bit = 0;
    geo_sync_msg.frame_fields.data_len = 0;     // No data
    geo_sync_msg.data.qword = 0;


    do
    {
        // Send sync message
        status = CAN_tx(GEO_CNTL_CANBUS, &geo_sync_msg, GEO_CNTL_CAN_TIMEOUT);

        if( !status )
        {
            LOG_ERROR("ERROR!!! Unable to send Geo controller sync message\n");
            LE.on(GEO_CAN_ERR_LED);
        }

        else
            LE.off(GEO_CAN_ERR_LED);

        // No need to delay here
        // XXX: Cannot use FreeRTOS functions until the OS runs
        // vTaskDelayMs(GEO_CNTL_SYNC_TIME);


        can_rx_timer.restart();
        while( !can_rx_timer.expired() );


        // Check for ack
        status = CAN_fullcan_read_msg_copy( geo_syncack_msg_ptr, &geo_syncack_msg_copy );

        // XXX: Since OS is not up, maybe you can use SoftTime (polling timer)
        if( status )
        {
            // We have a new message. Check if Geo is acked

            // XXX: Suggest "shared" structures rather than memcpy
            //memcpy((void *)&sync_msg, (void *)&geo_syncack_msg_copy.data.qword, sizeof(sync_msg));
            sync_msg = (master_sync *)&geo_syncack_msg_copy.data.bytes[0];
            if( sync_msg->ack_geo )
                sync_ack = true;

        }

        sync_miss_count++;

    }while( ( sync_ack == false ) );    // try forever

#if 0
    if( !sync_ack )
        LOG_ERROR("ERROR!!! Sync timeout from Geo controller. Stalling!!!\n");
#endif

    return sync_ack;
}

#ifdef __cplusplus
extern "C"{
#endif

void geo_send_gps(GEO_TO_SEND a)
{
 //   printf("HI inside CAN GPS\n");

    can_msg_t gps_msg;
    bool can_status = false;
    if(a == GEO_DATA_TO_SEND)
    {
        gps_msg.msg_id = GEO_LOC_DATA_ID;
        gps_msg.frame_fields.is_29bit = 0;
        gps_msg.frame_fields.data_len = sizeof(gps_data_dec);
        memcpy(&gps_msg.data.qword, &gps_data_dec, sizeof(gps_data_dec));
        //  printf("CAN GPS TRNS: %x \n\n", gps_msg.data.qword);

        can_status = CAN_tx(GEO_CNTL_CANBUS, &gps_msg, GEO_CNTL_CAN_TIMEOUT);

        if( !can_status )
        {
            LOG_ERROR("ERROR!!! Geo controller CAN message: GPS data not sent!!");
            LE.on(GEO_CAN_ERR_LED);
        }

        else
            LE.off(GEO_CAN_ERR_LED);
    }
    if(a == GEO_DO_NOT_SEND)
    {
        geo_location  dummy_gps_data;
        dummy_gps_data.latitude = 0.0;
        dummy_gps_data.longitude = 0.0;

        gps_msg.msg_id = GEO_LOC_DATA_ID;
        gps_msg.frame_fields.is_29bit = 0;
        gps_msg.frame_fields.data_len = sizeof(dummy_gps_data);
        memcpy(&gps_msg.data.qword, &dummy_gps_data, sizeof(dummy_gps_data));
        //  printf("CAN GPS TRNS: %x \n\n", gps_msg.data.qword);

        can_status = CAN_tx(GEO_CNTL_CANBUS, &gps_msg, GEO_CNTL_CAN_TIMEOUT);

        if( !can_status )
        {
            LOG_ERROR("ERROR!!! Geo controller CAN message: GPS data not sent!!");
            LE.on(GEO_CAN_ERR_LED);
        }

        else
            LE.off(GEO_CAN_ERR_LED);
    }

}

uint16_t calculateBearing(geo_location& geo_location_ref)
{
    float startLat = degreesToRadians(gps_data_dec.latitude);
    float startLong = degreesToRadians(gps_data_dec.longitude);
    float endLat = degreesToRadians(geo_location_ref.latitude);
    float endLong = degreesToRadians(geo_location_ref.longitude);

    float dLong = endLong - startLong;
    float y = sin(dLong) * cos(endLat);
    float x = cos(startLat) * sin(endLat) - sin(startLat)* cos(endLat) * cos(dLong);

    float brng = atan2(y,x);
    uint16_t brng1 = radiansToDegrees(brng);
    return brng1;

}

void geo_send_heading(GEO_TO_SEND a, geo_location& geo_location_ref)
{
    can_msg_t geo_msg;
    geo_spd_angle geo_data;
    bool can_status = false;
    if(a == GEO_DATA_TO_SEND)
    {
        static imu& imu_handle = IMUInterface;  // Handle to singleton IMU object
        uint16_t imu_heading = 0;       // 2-byte angle between 0 and 360. Compromise with precision
        imu_heading = static_cast<uint16_t>(imu_handle.getHeading());

        // Call func to calculate bearing
        geo_data.bearing = calculateBearing(geo_location_ref);   // put bearing here
        geo_data.heading = imu_heading;
        geo_data.speed = speed_gps;
    }

    if(a == GEO_DO_NOT_SEND)
    {
        // Call func to calculate bearing
        geo_data.bearing = 0;   // put bearing here
        geo_data.heading = 0;
        geo_data.speed = 0;
    }
    geo_msg.msg_id = GEO_SPEED_ANGLE_ID;
    geo_msg.frame_fields.is_29bit = 0;
    geo_msg.frame_fields.data_len = sizeof(geo_data);
    memcpy(&geo_msg.data.qword,&geo_data, sizeof(geo_data));
    // printf("CAN IMU TRNS: %x \n\n", geo_msg.data.qword);


    // CAN_Tx() will only time out when TX queue is full and that will only
    // happen when CAN Bus turns off too long for us to empty the TX queue
    can_status = CAN_tx(GEO_CNTL_CANBUS, &geo_msg, GEO_CNTL_CAN_TIMEOUT);

    if( !can_status )
    {
        LOG_ERROR("ERROR!!! Geo controller CAN message: IMU data not sent!!");
        LE.on(GEO_CAN_ERR_LED);
    }

    else
        LE.off(GEO_CAN_ERR_LED);

}

void geo_send_heartbeat( void )
{
    // Heart Beat to Master
    can_msg_t heartbeat_geo_msg; // Can Message
    bool can_status = false;

    heartbeat_geo_msg.msg_id = GEO_HEARTBEAT_ID; // Geo Heartbeat ID
    heartbeat_geo_msg.frame_fields.is_29bit = 0;
    heartbeat_geo_msg.frame_fields.data_len = 0;

    can_status = CAN_tx(GEO_CNTL_CANBUS, &heartbeat_geo_msg, GEO_CNTL_CAN_TIMEOUT);

    if( !can_status )
    {
        LOG_ERROR("ERROR!!! Geo controller Heartbeat message not sent!!");
        LE.off(GEO_HB_LED);
    }
    else
    {
        LE.toggle(GEO_HB_LED);
    }

    if(bus_off_flag) {
        CAN_reset_bus(GEO_CNTL_CANBUS);
        bus_off_flag = false;
    }
}

bool receive_master_checkpoint(geo_location &ref_checkpoint_data)
{
    can_fullcan_msg_t *geo_gps_msg_ptr = NULL;
    can_fullcan_msg_t geo_gps_msg_copy;

    bool can_status = false;
    can_msg_t geo_checkpoint;
    geo_location* gps_data_master;

    geo_gps_msg_ptr = CAN_fullcan_get_entry_ptr(can_id_loc_update);
        if( geo_gps_msg_ptr == NULL )
        {
            LOG_ERROR("ERROR!!! Cannot get FullCan msg pointer for Geo controller power-up sync\n");
            return false;
        }

        can_status = CAN_fullcan_read_msg_copy( geo_gps_msg_ptr, &geo_gps_msg_copy );

               // XXX:
               if( can_status )
               {
                   // We have a new message. Which is a check point from the master.
                   // XXX: Suggest "shared" structures rather than memcpy
                   gps_data_master = (geo_location *)&geo_gps_msg_copy.data;
                   ref_checkpoint_data.latitude = gps_data_master->latitude;
                   ref_checkpoint_data.longitude = gps_data_master->longitude;
                   return true;
               }
}

void geo_check_master_reset( void )
{
    // Read reset messages from master
    can_fullcan_msg_t *can_rst_msg_ptr = NULL;
    can_fullcan_msg_t can_rst_msg;
    rst_msg *geo_rst_msg;

    can_rst_msg_ptr = CAN_fullcan_get_entry_ptr(can_id_rst);

    bool status = CAN_fullcan_read_msg_copy(can_rst_msg_ptr, &can_rst_msg);

    if( !status )
        return;         // There is no reset message

    LE.off(GEO_CAN_ERR_LED);

    geo_rst_msg = (rst_msg *)&(can_rst_msg.data.bytes[0]);

    if( geo_rst_msg->reset_geo == RESET )
    {

        LOG_ERROR("ERROR!!! Received a reset request from master\n");
        sys_reboot();
    }
}

#ifdef __cplusplus
}
#endif
