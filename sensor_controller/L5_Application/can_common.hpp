/*
 * File         : can_common.hpp
 * Author       : Dhruv Kakadiya
 * Date         : September 13, 2015
 */
#ifndef CAN_COMMON_HPP
#define CAN_COMMON_HPP

#include <stdio.h>
#include "tasks.hpp"
#include "io.hpp"
#include "can.h"
#include "file_logger.h"
#include "can_msg_id.hpp"

#define SENSOR_CNTL_CANBUS               (can1)      // Use can1
#define SENSOR_CNTL_CANBAUD              (100)       // 100kbps baud rate
#define SENSOR_CNTL_CANTXQ               ( 50 )       // Transmit q size 5
#define SENSOR_CNTL_CANRXQ               ( 10 )       // Receive q size 5
#define SENSOR_CNTL_CAN_TIMEOUT          ( 0 )       // Timeout for sending can message
#define SENSOR_CNTL_SYNC_TIME            ( 500 )     // Allow 500 ms wait for ack
#define SENSOR_CNTL_SYNC_MAX_TIME        ( GEO_CNTL_SYNC_TIME * 20 ) // Wait 10s max
#define SENSOR_INIT_LED_TIME             ( 500 )

#define SENSOR_CAN_ERR_LED               ( 4 )
#define RESET                            ( 4)
#define BUS_OFF_LED                      ( 4 )
#define SENSOR_HB_LED                    ( 4 )

#define ERROR_LED       4
#define CAN_RX_LED      4
#define CAN_TX_LED      4

/* basic structure to create/store 11bit CAN msg id,
 * used for creating filter list and for creating 11bit ids
 */
typedef union {
        struct{
                uint16_t src        : 3; // Source ID       (LSB)
                uint16_t dst        : 3; // Destination ID
                uint16_t msg_num    : 5; // Message Number  (MSB)
        };
        uint16_t raw;
} __attribute__((packed)) can_msg_id_t;

/* used in error_data_t */
typedef enum {
    ERROR_NO_ERROR          = 0x00,
    ERROR_BUS_OFF           = 0x01,
    ERROR_DATA_OVERRUN      = 0x02,
    ERROR_TX_FAILED         = 0x04,
    ERROR_RX_FAILED         = 0x08,
    ERROR_MSG_QUEUE_FULL    = 0x10,
    ERROR_MSG_CORRUPT       = 0x20,
    ERROR_UNKNOWN           = 0xFF          // unspecified error
} error_code_t;

#define SET_ERROR(error_code) \
        error_flag |= error_code; \
        (error_flag)? LE.on(ERROR_LED): LE.off(ERROR_LED);

#define RESET_ERROR(error_code) \
        error_flag &= ~(error_code); \
        (error_flag)? LE.on(ERROR_LED): LE.off(ERROR_LED);

bool can_init(void);
bool test_data(can_msg_t can_message);
bool transmit_data(can_msg_t send_message);
bool receive_data();
void check_bus_off(void);
void sensor_send_heartbeat(void);
void sensor_get_master_reset(void);

#endif
