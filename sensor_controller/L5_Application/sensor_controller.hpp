/*
 * sensor_controller.hpp
 *
 *  Created on: Nov 1, 2015
 *      Author: Dhruv
 */

#ifndef L5_APPLICATION_SENSOR_CONTROLLER_HPP_
#define L5_APPLICATION_SENSOR_CONTROLLER_HPP_

#include "can_msg_id.hpp"
#include "lpc_timers.h"
#include "gpio.hpp"
#include "io.hpp"
#include "task.h"
#include "utilities.h"
#include "soft_timer.hpp"
#include "can_common.hpp"
#include <stdio.h>
#include <string.h>

typedef struct Sensor {
        float dist_val;
        uint8_t szone;
        bool flag_u;
        bool flag_u_high;
        uint32_t timer_val;
        SoftTimer limit_time;
        int filter_count;

    public:
        Sensor(){
            dist_val=0;
            szone=no_obstacle;
            flag_u = true;
            flag_u_high = false;
            timer_val = 0;
            filter_count=0;
        }
};

void read_sensor_data1(Sensor* s1,int port_no,int pin_no);
void read_sensor_data1(Sensor* s1,int port_trigger, int trigger_pin,int port_echo, int echo_pin);
void send_sensor_data(void);

#endif /* L5_APPLICATION_SENSOR_CONTROLLER_HPP_ */
