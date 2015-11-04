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

#define ERROR_LED 1
#define CAN_RX_LED      2
#define CAN_TX_LED      3

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
bool transmit_data();
bool receive_data();
void can_tx_init(void);
void can_rx_init(void);
void can_init(void);

void check_bus_off(void);

class Heartbeat: public scheduler_task {
    private:
        can_msg_t heart_tx_message,heart_rx_message;
    public:
        Heartbeat(uint8_t priority);
        bool init(void);
        bool run(void *p);
};

#endif
