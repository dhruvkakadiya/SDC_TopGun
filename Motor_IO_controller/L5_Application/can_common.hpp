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

#define MOTORIO_CNTL_CANBUS               (can1)      // Use can1
#define MOTORIO_CNTL_CANBAUD              (100)       // 100kbps baud rate
#define MOTORIO_CNTL_CANTXQ               ( 5 )       // Transmit q size 5
#define MOTORIO_CNTL_CANRXQ               ( 5 )       // Receive q size 5
#define MOTORIO_CNTL_CAN_TIMEOUT          ( 0 )       // Timeout for sending can message
#define MOTORIO_CNTL_SYNC_TIME            ( 500 )     // Allow 500 ms wait for ack
#define MOTORIO_CNTL_SYNC_MAX_TIME        ( GEO_CNTL_SYNC_TIME * 20 ) // Wait 10s max
#define MOTORIO_INIT_LED_TIME             ( 500 )

#define MOTORIO_CAN_ERR_LED               ( 1 )
#define RESET                             ( 2 )
#define BUS_OFF_LED                       ( 3 )
#define MOTORIO_HB_LED                    ( 4 )

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

bool test_data(can_msg_t can_message);
bool transmit_data(can_msg_t transmit_msg);
bool receive_data();
bool can_init(void);
void motor_io_send_heartbeat(void);
void motor_io_get_master_reset(void);
//bool powerup_sync_motor_io_controller( void );
void check_bus_off(void);

#endif
