/*
 * master_controller.hpp
 *
 *  Created on: Oct 29, 2015
 *      Author: Hemanth K N
 */

#ifndef L5_APPLICATION_MASTER_CONTROLLER_HPP_
#define L5_APPLICATION_MASTER_CONTROLLER_HPP_

#include "can.h"

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

#define HEARTBEAT ( 1 )
#define DEBUG ( 1 )
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

enum MASTER_ACK {
    NACK = 0,
    ACK
};

enum NAV_STAT{
    STOPPED=0,
    RX_CHKPTS,
    RX_COMPLETE,
    NAVIGATING,
    INVALID
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

#endif /* L5_APPLICATION_MASTER_CONTROLLER_HPP_ */
