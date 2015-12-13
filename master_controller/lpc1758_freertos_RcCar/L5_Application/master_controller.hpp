/*
 * master_controller.hpp
 *
 *  Created on: Oct 29, 2015
 *      Author: Hemanth K N
 */

#ifndef L5_APPLICATION_MASTER_CONTROLLER_HPP_
#define L5_APPLICATION_MASTER_CONTROLLER_HPP_

#include "can.h"
#include "math.h"

#define MASTER_LED_INIT_TIME    (500)
#define MASTER_CNTL_CANBUS      (can1)
#define MASTER_CNTL_BAUD        (100)
#define MASTER_CNTL_RXQ         ( 8 )       // We will be sending maximum of 4 messages at a time, doubling q depth
#define MASTER_CNTL_TXQ         ( 8 )       // Not sure if this will be useful in fullCAN

#define DUMMY_ID                ( 0xFFFF )

#define MASTER_BT_HB_THRESH         ( 1 ) // If bluetooth heartbeat misses too many times, reset
#define MASTER_GEO_HB_THRESH        ( 1 )
#define MASTER_SENSOR_HB_THRESH     ( 1 )  // Heart-beats are checked only once a second. If we don't know obstacles for a second its a problem
#define MASTER_MOTORIO_HB_THRESH    ( 1 )  // Fingers crossed
#define MASTER_TASK_OVERRUN_DELAY   ( 2000 )
#define MASTER_CNTL_CAN_DELAY       ( 0 )
#define MASTER_CNTL_HB_LED          ( 2 )
#define MASTER_CNTL_NOHB_LED        ( 3 )
#define MASTER_CAN_ERR_LED          ( 4 )

#define MAX_LATS    (50)
#define MAX_LONGS   (50)

#define RESET   ( 1 )
#define NORESET ( 0 )

#define HEARTBEAT ( 0 )
#define DEBUG ( 0 )
#define ZONE_CALCULATION  ( 0 )
#define OBSTACLE_AVOIDANCE ( 1 )

#define NAV_TRUE    (true)
#define NAV_FALSE   (false)


// 1 - No Bluetooth App; 0 - Control via Bluetooth App
#define BT_APP ( 1 )

// Zone Thresholds
#define ZONE_NEAR_THRESHOLD ( 50 )
#define ZONE_MID_THRESHOLD ( 150 )
#define ZONE_FAR_THRESHOLD ( 200 )

#define EARTH_RADIUS_KM 6373
#define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)

#define GEO_LOC_MIA_MAX_COUNT   100
#define GEO_HEADING_BEARING_MIA_MAX_COUNT 100
#define MIN_DISTANCE_TO_CHKPT   15

enum MASTER_ACK {
    NACK = 0,
    ACK
};

enum NAV_STAT{
    STOPPED=0,
    PAUSED,
    RX_CHKPTS,
    RX_COMPLETE,
    NAVIGATING,
    INVALID
};

/*
 * We divide the zone (visualized as a 360 degree area into 30 regions - These zones will be part of 7 groups.
 */
enum ZONE_EDGE {
    ZONE_EDGE0 = 0,
    ZONE_EDGE1 = 12,
    ZONE_EDGE2 = 36,
    ZONE_EDGE3 = 96,
    ZONE_EDGE4 = 180,
    ZONE_EDGE5 = 264,
    ZONE_EDGE6 = 312,
    ZONE_EDGE7 = 348,
    ZONE_EDGE8 = 359
};

/*
 * The naming convention defines the sort of action that will occur in each zone
 *
 * STR - Straight
 * SR - Slight Right
 * HR - Hard Right
 * UR - U-turn Right
 * UL - U-turn Left
 * HL - Hard Left
 * SL - Sight Left
 */
enum ZONE_NAVI {
    ZONE_NAVI_STR = 0,
    ZONE_NAVI_SR,
    ZONE_NAVI_HR,
    ZONE_NAVI_UR,
    ZONE_NAVI_UL,
    ZONE_NAVI_HL,
    ZONE_NAVI_SL
};

/* Function Prototypes */
bool master_controller_init( void );
void check_heartbeat( void );
bool avoid_obstacle(void);
bool update_from_app(void);
bool navigation_mode(void);
void navigate_to_next_chkpt(void);
void check_bus_off(void);

char printRange(uint8_t zone);
char printMotorDirection(uint8_t data);
char printMotorSpeed(uint8_t data);

int getZoneInformation(float value);
ZONE_NAVI getNavigationZone(uint32_t difference);

#endif /* L5_APPLICATION_MASTER_CONTROLLER_HPP_ */
