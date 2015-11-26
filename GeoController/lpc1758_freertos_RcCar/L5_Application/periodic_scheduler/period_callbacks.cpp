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
#include "geo_controller.hpp"
#include "GPS.hpp"
#include <stdio.h>



/// This is the stack size used for each of the period tasks
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);


void period_1Hz(void)
{


   // geo_send_heartbeat();
   // geo_check_master_reset();

}

void period_10Hz(void)
{

    /*
     * TODO: For GPS team
     * Don't change the order of these calls.
     * If you change then call gps getters in both functions
     */
    gps_data();
    //  geo_send_gps();
    geo_location current_checkpoint_data;
    GEO_TO_SEND a;
    if(receive_master_checkpoint(current_checkpoint_data))
    {
        a = GEO_DATA_TO_SEND;
        geo_send_gps(a);
        geo_send_heading(a, current_checkpoint_data);
    }
    else
    {
     a = GEO_DO_NOT_SEND;
     geo_send_gps(a);
     geo_send_heading(a,current_checkpoint_data);
    }

}

void period_100Hz(void)
{
    /* Nothing will be done here */
}

void period_1000Hz(void)
{
    /*Nothing will be done here */
}
