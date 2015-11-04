/*
 * motor_controller.hpp
 *
 *  Created on: Nov 1, 2015
 *      Author: Dhruv
 */

#ifndef L5_APPLICATION_MOTOR_CONTROLLER_HPP_
#define L5_APPLICATION_MOTOR_CONTROLLER_HPP_

#include "can_msg_id.hpp"
#include "io.hpp"
#include "task.h"
#include "utilities.h"
#include "lpc_pwm.hpp"
#include "can_common.hpp"
#include <stdio.h>

extern PWM pwmServo;
extern PWM pwmDC;

typedef struct Motor {

    public:
        Motor(){
            pwmServo.set(7.5);
            pwmDC.set(8.0);
        }
};

void set_motors_pwm(void);

#endif /* L5_APPLICATION_MOTOR_CONTROLLER_HPP_ */
