/*
 * can_msg_id.hpp
 *
 *  Created on: Nov 1, 2015
 *      Author: Dhruv
 */

#ifndef L5_APPLICATION_CAN_MSG_ID_HPP_
#define L5_APPLICATION_CAN_MSG_ID_HPP_

#ifdef __cplusplus
extern "C"{
#endif
#include <stdint.h>
#include <stdbool.h>
#include "can.h"

// CAN message IDS

/// XXX: Recommend enumeration for this
enum CAN_MSG_ID
{
    KILL_SWITCH_ID = 0,
    RESET_ID,
    MASTER_SYNC_ACK_ID,
    MOTORIO_SYNC_ID,
    SENSOR_SYNC_ID,
    BLUETOOTH_SYNC_ID,
    GEO_SYNC_ID,
    MOTORIO_HEARTBEAT_ID,
    SENSOR_HEARTBEAT_ID,
    BLUETOOTH_HEARTBEAT_ID,
    GEO_HEARTBEAT_ID,
    RUN_MODE_ID,
    DISTANCE_SENSOR_ID,
    MOTOR_DIRECTIONS_ID,
    CHECKPOINT_REQ_ID,
    CHECKPOINT_SEND_ID,
    CHECKPOINT_DATA_ID,
    GEO_LOC_UPDATE_ID,
    GEO_SPEED_ANGLE_ID,
    GEO_LOC_DATA_ID,
    LIGHT_BATTERY_SENSOR_ID,

    // Add any message IDs before this
    CAN_MSG_MAX_ID
};

/*
 *  Data structures for CAN message IDs
 *
 *  The following messages have no data fields
 *
 *  1. Kill switch
 *  2. MOTORIO_SYNC
 *  3. SENSOR_SYNC
 *  4. BLUETOOTH_SYNC
 *  5. GEO_SYNC
 *  6. CHECKPOINT_REQ_ID
 */

/*
 * Reset message for each controller
 * Sent by: Master controller
 * Received by: All controllers
 */
typedef struct{

    uint64_t reset_motorio:8;     // Acknowledge motorio controller
    uint64_t reset_sensor:8;      // Acknowledge sensor controller
    uint64_t reset_geo:8;         // Acknowledge geo controller
    uint64_t reset_bluetooth:8;   // Acknowledge bluetooth module

}__attribute__((__packed__)) rst_msg;


/*
 * Master acknowledges each controller
 * Sent by: Master controller
 * Received by: All controllers
 */
typedef struct{

    uint64_t ack_motorio:8;     // Acknowledge motorio controller
    uint64_t ack_sensor:8;          // Acknowledge sensor controller
    uint64_t ack_geo:8;         // Acknowledge geo controller
    uint64_t ack_bluetooth:8;       // Acknowledge bluetooth module

}__attribute__((__packed__)) master_sync;


/*
 * Heart-beat message for each controller
 * Sent by: All controllers
 * Received by: Master controller
 */
typedef struct{

    uint64_t counter:8;         // Continuity counter that increments with every beat

}__attribute__((__packed__)) heart_beat;

/*
 * Run mode for the vehicle.
 * TODO: Define enum for each mode
 *
 * Sent by: Bluetooth module [ original data from the Android app ]
 * Received by: Master controller
 */

typedef struct{

    uint64_t mode:8;                // Run modes - 1. Navigation mode, 2. Free run mode and 3. Manual mode

}__attribute__((__packed__)) run_mode;

/*
 *  Ultrasonic sensor readings for obstacle avoidance
 *  Sent by: Sensor controller
 *  Received by: Master controller
 */
typedef struct{

    uint64_t front_left:8;          // Front left sensor reading
    uint64_t front_right:8;     // Front right sensor reading
    uint64_t front_center:8;        // Front centre sensor reading
    uint64_t left:8;                // Left sensor reading
    uint64_t right:8;               // Right sensor reading
    uint64_t back:8;                // Back sensor reading

}__attribute__((__packed__)) dist_sensor;

/*
 * Indication to motor IO for driving.
 *
 * Sent by: Master controller
 * Received by: MotorIO controller
 */
typedef struct{

    uint64_t speed:8;               // Indicate speed for DC motor
    uint64_t turn:8;                // Indicate turn angle for servo motor

}__attribute__((__packed__)) motor_direction;

/*
 * Indicate number of check-points in the route. Then send each check point using check-point data
 *
 * Sent by: Bluetooth module [ After receiving data from Android app ]
 * Received by: Master controller
 */
typedef struct{

    uint64_t num_of_points;     // Number of check-points to be loaded

}__attribute__((__packed__)) chk_point_snd;

/*
 * Check-point data sent for each check-point
 *
 * Sent by: Bluetooth module
 * Received by: Master controller
 */
typedef struct{

    float latitude;
    float longitude;

}__attribute__((__packed__)) chk_point_data;

/*
 * Send destination lattitude and longitude to the geo controller. Here destination will be the next check-point
 *
 * Sent by: Master controller
 * Received by: Geo controller
 */
typedef chk_point_data geo_loc; // use geo_loc instead of chk_point_data

/*
 * Indicate speed measured by GPS, the current heading and bearing from IMU
 *
 * Sent by: Geo controller
 * Received by: Master controller and IO controller
 */
typedef struct{

    uint64_t speed:8;               // Speed as measured by the GPS sensor
    uint64_t heading:16;            // Heading from the Geo controller
    uint64_t bearing:16;            // Bearing calculated by the Geo controller
    uint64_t distance:24;

}__attribute__((__packed__)) geo_spd_angle;

/*
 * Light and battery sensor readings
 *
 * Sent by: Sensor controller
 * Received by: IO controller
 */
typedef struct{

    uint64_t light_sensor:8;        // Light sensor reading
    uint64_t batt_sensor:8;     // Battery level sensor reading

}__attribute__((__packed__)) lght_batt_reading;

typedef enum {
    stop = 0,
    slow,
    normal,
    fast,
    turbo
} speed_mode;

typedef enum{
    no_obstacle=0,far,mid,near
}sensor_zone;

typedef enum {
    left=0,s_left,straight,s_right,right,back
} direction_t;

#ifdef __cplusplus
}
#endif

#endif /* L5_APPLICATION_CAN_MSG_ID_HPP_ */
