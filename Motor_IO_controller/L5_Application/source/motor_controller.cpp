/*
 * motor_controller.cpp
 *
 *  Created on: Nov 1, 2015
 *      Author: Dhruv
 */
#include "motor_controller.hpp"

#define DC_ON               1
#define SERVO_ON            1

#define SLOW_LED            2
#define CAN_ERROR_LED       4

#define HARD_LEFT           9.0
#define S_LEFT              8.0
#define HARD_RIGHT          5.7
#define S_RIGHT             6.5
#define STRAIGHT            7.5

#define DC_STOP             7.0
#define DC_SLOW             6.5
#define DC_NORMAL           6.4
#define DC_TURBO            6.0

can_msg_t motor_msg;

//As per waveform and real testing(percent range - 6.0(right) - 7.5(center) - 9.3(left))
//As per waveform only(percent range - 5.5(forward) - 8.5(stop) - 10.5(backward))

void motor_init(void){

    static int c=0;
    float factor = 5.5;
    MotorControl.setServo(7.5);
    delay_ms(100);

    while(factor<9.3){
        MotorControl.setServo(factor);
        factor+=0.1;
        delay_ms(50);
    }

    while(factor>5.5){
        MotorControl.setServo(factor);
        factor-=0.1;
        delay_ms(100);
    }
    MotorControl.setServo(7.5); // Set servo straight again

    while(c < 20){
        MotorControl.setDC(7.0);
        delay_ms(50);
        c++;
    }

    printf("motor_init is done\n");
}

void set_motors_pwm(void) {

    motor_direction *md1= (motor_direction*) &motor_msg.data.qword; // Copy received CAN msg into proper data structure

    // XXX: Let the master controller provide gradients, such as:
    // 0-10 for back motor
    // 0-10 for steer motor with 5 being neutral
#if SERVO_ON
    if(md1->turn==left) {                                       // turn hard left
        MotorControl.setServo(HARD_LEFT);
        //printf("Motor Task : Turn hard left \n");
    }
    else if(md1->turn==s_left) {                                // turn slight left
        MotorControl.setServo(S_LEFT);
        //printf("Motor Task : Turn Slight left \n");
    }
    else if(md1->turn==straight) {                              // keep straight
        MotorControl.setServo(STRAIGHT);
            //printf("Motor Task : Keep Straight \n");
    }
    else if(md1->turn==s_right) {                               // turn slight right
        MotorControl.setServo(S_RIGHT);
        //printf("Motor Task : Turn Slight Right \n");
    }
    else if(md1->turn==right) {                                 // turn hard right
        MotorControl.setServo(HARD_RIGHT);
        //printf("Motor Task : Turn hard Right \n");
    }
#endif

#if DC_ON
    //--------------------------- Speed of DC Motor ------------------------
    if(md1->speed == stop) {                                    // Stop motor
        MotorControl.setDC(DC_STOP);
        LE.on(4);
    }
    else if(md1->speed==slow) {                                 // Slow speed
        LE.toggle(SLOW_LED);// Normal Speed
        MotorControl.setDC(DC_SLOW);
    }
    else if(md1->speed==normal) {                               // Normal Speed
        MotorControl.setDC(DC_NORMAL);
    }
    else if(md1->speed==turbo) {                                // Turbo Speed
        MotorControl.setDC(DC_TURBO);
    }
#endif

    delay_ms(20);                           // Don't make delay less otherwise you'll have problem.
                                            // Required this much delay to set proper pwm for motors
}

void drive_TopGun(void) {
    //message2.data.qword = 0;
    /**
     * motor ctrl msg   : 100ms
     *                    300ms --> Motor stop
     *  if (!msg_rx) {
     *      motor_msg.data = 0;
     *  }
     *
     *  set_motors_pwm();
     */
    if(receive_data()){                                 // Check if got motor_direction message
               set_motors_pwm();
    }
    else{
        MotorControl.setDC(DC_STOP);
        LE.toggle(CAN_ERROR_LED);   // Toggle LED 4 if Motor controller is not getting any data over CAN bus
    }
}
