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
//#include "can_common.hpp"
#include "sensor_controller.hpp"
#define sensor_controller 1
#define master_controller 0
/// This is the stack size used for each of the period tasks
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);

void period_1Hz(void)
{
    check_bus_off();
    //get_sensor_data();
    //sensor_task1();
    //LE.toggle(1);
    //sensor_task_1hz();
    //can_world_task();
}

void period_10Hz(void)
{
    //motor_task1();
#if sensor_controller
    //get_ultrasonic_data();
    get_sensor_data();
    //sensor_task1();
    can_sensor_tx_task();
#endif
    //can_motor_rx_task();
    //LE.toggle(2);
    //drive_car();
    //sensor_task_10hz();
}

void period_100Hz(void)
{
#if master_controller
    can_master_rx_task();
    can_master_tx_task();
#endif
    //LE.toggle(3);
}

void period_1000Hz(void)
{
    //LE.toggle(4);
}
