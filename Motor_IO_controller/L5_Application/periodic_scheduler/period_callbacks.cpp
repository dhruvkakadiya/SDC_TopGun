/*
 *     SocialLedge.com - Copyright (C) 2013
 *
 *     This file is part of free software framework for embedded processors.
 *     You can use it and/or distribute it as long as this copyright header
 *     remains unmodified.  The code is free for personal use and requires
 *     permission to use in a commercial product.
 *
 *      THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 *      OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 *      MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *      I SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 *      CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *     You can reach the author of this software at :
 *          p r e e t . w i k i @ g m a i l . c o m
 */

/**
 * @file
 * This contains the period callback functions for the periodic scheduler
 *
 * @warning
 * These callbacks should be used for hard real-time system, and the priority of these
 * tasks are above everything else in the system (above the PRIORITY_CRITICAL).
 * The period functions SHOULD NEVER block and SHOULD NEVER run over their time slot.
 * For example, the 1000Hz take slot runs periodically every 1ms, and whatever you
 * do must be completed within 1ms.  Running over the time slot will reset the system.
 */


#include <stdint.h>
#include "periodic_callback.h"
#include "motor_controller.hpp"
#include "can_common.hpp"
// Include file from auto generated code:
#include <_can_dbc/generated_motorio_code.h>
// Include file for telemetry --> ALso need to turn on #define at sys_config.h (SYS_CFG_ENABLE_TLM)
#include "tlm/c_tlm_comp.h"
#include "tlm/c_tlm_var.h"

#define IF_PREET            0

/// This is the stack size used for each of the period tasks
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);
extern can_msg_t motor_msg;

uint8_t calibation_value = 0;

/// Necessary variables for CAN auto generated code
    /// Actual message the whole program uses
    SENSOR_TX_SENSOR_SONARS_t sensor_sonars_msg;

    // EXACT NAME: MIA replacement
    const SENSOR_TX_SENSOR_SONARS_t SONARS__MIA_MSG = { 3, 3, 3, 3, 3, 3};

    // EXACT NAME: Timeout when MIA is replaced
    const uint32_t SONARS__MIA_MS = 1000;
///


/// Called once before the RTOS is started, this is a good place to initialize things once
bool period_init(void)
{
    bool status = false;

    do {
    status = can_init();             // Initialize CAN bus
    }
    while(!status);                 // If CAN bus is not ready then no need to go further

    motor_init();                   // Initialize PWM sequence for DC and Servo motor
    return true;                    // Must return true upon success
}

/// Register any telemetry variables
bool period_reg_tlm(void)
{
    // Make sure "SYS_CFG_ENABLE_TLM" is enabled at sys_config.h to use Telemetry
    tlm_component* comp = tlm_component_add("MOTORIO");
    TLM_REG_VAR(comp, motor_msg,tlm_char); // Macro to register received motor_msg
    return true; // Must return true upon success
}

void period_1Hz(void)
{
    check_bus_off();            // To check if CAN bus off is there then RESET the CAN bus
}

void period_10Hz(void)
{
    drive_TopGun();             // Receive CAN data from Master and set PWM accordingly
}

void period_100Hz(void)
{
    receive_data();
}

void period_1000Hz(void)
{

}


#if IF_PREET
void preet_can_receive_function() {
    // Whichever way you receive a message:
    can_msg_t msg;
    CAN_rx(can1, &msg, 1000);

    switch (msg.msg_id) {
        case (DISTANCE_SENSOR_ID):
        {
            msg_hdr_t hdr = { msg.msg_id, (uint8_t)msg.frame_fields.data_len };
            SENSOR_TX_SENSOR_SONARS_decode(&sensor_sonars_msg,(uint64_t*)&msg.data,&hdr); // NULL
            break;
        }
    }

    // HANDLE MIAs:
    if (SENSOR_TX_SENSOR_SONARS_handle_mia(&sensor_sonars_msg, 10)) {
        // Message has gone missing
    }

    // Send messages:
    MOTORIO_TX_MOTORIO_DIRECTION_t mc;
    mc.MOTORIO_DIRECTION_speed_cmd = normal;
    mc.MOTORIO_DIRECTION_turn_cmd = straight;
    msg_hdr_t h = MOTORIO_TX_MOTORIO_DIRECTION_encode((uint64_t*)&msg.data, &mc);
    msg.msg_id = h.mid;
    msg.frame_fields.data_len = h.dlc;
    CAN_tx(can1, &msg, 0);

}
#endif
