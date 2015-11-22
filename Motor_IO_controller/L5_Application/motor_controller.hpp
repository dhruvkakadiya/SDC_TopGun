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


class MotorController : public SingletonTemplate<MotorController>
{
    public:

        void setDC(float v) { mDriveMotor.set(v); }
        void setServo(float v) { mSteerMotor.set(v); }

    private:
        PWM mDriveMotor;
        PWM mSteerMotor;

        MotorController() : mDriveMotor(PWM::pwm2), mSteerMotor(PWM::pwm1)
        {

        }

        friend class SingletonTemplate<MotorController>; ///< Friend class used for Singleton Template
};

#define MotorControl MotorController::getInstance()

#ifdef __cplusplus
extern "C"{
#endif

void set_motors_pwm(void);
void motor_init(void);
void drive_TopGun(void);
void motor_send_heartbeat(void);
void motor_check_master_reset(void);
bool check_validity_speed_factor(float s_factor, int speed );

#ifdef __cplusplus
}
#endif

#endif /* L5_APPLICATION_MOTOR_CONTROLLER_HPP_ */
