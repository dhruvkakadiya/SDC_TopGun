/*
 * motor_controller.cpp
 *
 *  Created on: Nov 1, 2015
 *      Author: Dhruv
 */
#include "motor_controller.hpp"
#include "_can_dbc/generated_motorio_code.h"
#include "adc0.h"

#define DC_ON               1
#define SERVO_ON            1

#define SLOW_LED                2
#define CAN_ERROR_LED           4
#define SPEED_FACTOR_ERROR      1

#define HARD_LEFT           9.0
#define S_LEFT              8.0
#define HARD_RIGHT          5.7
#define S_RIGHT             6.5
#define STRAIGHT            7.5

#define DC_STOP             7.0
#define DC_SLOW             6.4
#define DC_NORMAL           6.350
#define DC_TURBO            6.2

#define SPEED_FEEDBACK      0
//#define DC_STOP             7.0
#define DC_THRESH_SLOW             6.3
#define DC_THRESH_NORMAL           6.0
#define DC_THRESH_TURBO            5.8

// For RPM sensor configuration
#define ADC04_PINSELECT_VALUE           (  0x00000003 )
#define ADC04_PINSELECT_SHIFT           ( 28 )

extern DRIVER_TX_MOTORIO_DIRECTION_t motor_msg;
bool white_mark = false;
int white_mark_count = 0;
int check_time = 0;
float speed_factor = DC_STOP;
int desired_count = 0;
//extern int no_motor_msg_count;
//As per waveform and real testing(percent range - 6.0(right) - 7.5(center) - 9.3(left))
//As per waveform only(percent range - 5.5(forward) - 8.5(stop) - 10.5(backward))

void motor_init(void){

    int c=0;
    float factor = 5.5;
    MotorControl.setServo(STRAIGHT);
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
    MotorControl.setServo(STRAIGHT); // Set servo straight again

    while(c < 20){
        MotorControl.setDC(DC_STOP);
        delay_ms(50);
        c++;
    }

    printf("motor_init is done\n");
}

void set_motors_pwm(void) {

    motor_direction md1;//= (motor_direction*) &motor_msg.data.qword; // Copy received CAN msg into proper data structure
    md1.turn = motor_msg.MOTORIO_DIRECTION_turn_cmd;
    md1.speed = motor_msg.MOTORIO_DIRECTION_speed_cmd;

#if SERVO_ON
    if(md1.turn==left) {                                       // turn hard left
        MotorControl.setServo(HARD_LEFT);
    }
    else if(md1.turn==s_left) {                                // turn slight left
        MotorControl.setServo(S_LEFT);
    }
    else if(md1.turn==straight) {                              // keep straight
        MotorControl.setServo(STRAIGHT);
    }
    else if(md1.turn==s_right) {                               // turn slight right
        MotorControl.setServo(S_RIGHT);
    }
    else if(md1.turn==right) {                                 // turn hard right
        MotorControl.setServo(HARD_RIGHT);
    }
#endif

#if DC_ON
    //--------------------------- Speed of DC Motor ------------------------
    if(md1.speed == stop) {                                    // Stop motor
        speed_factor = DC_STOP;
        desired_count = 0;
        //LE.on(1);
    }
    else if(md1.speed==slow) {                                 // Slow speed
        //LE.toggle(SLOW_LED);// Normal Speed
        speed_factor = DC_SLOW;
        desired_count = 4;
    }
    else if(md1.speed==normal) {                               // Normal Speed
        speed_factor = DC_NORMAL;
        desired_count = 8;
    }
    else if(md1.speed==turbo) {                                // Turbo Speed
        speed_factor = DC_TURBO;
        desired_count = 12;
    }

    #if SPEED_FEEDBACK
        if(check_time>500){
            if(white_mark_count<desired_count){
                speed_factor += 0.05 ;
            }
            else if(white_mark_count>desired_count) {
                speed_factor -= 0.05 ;
            }
            check_time = 0;
        }
    #endif

    if(check_validity_speed_factor(speed_factor,md1.speed) || (!SPEED_FEEDBACK)){
        MotorControl.setDC(speed_factor);
    }
    else{
        MotorControl.setDC(DC_STOP);
        LE.toggle(SPEED_FACTOR_ERROR);
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
    set_motors_pwm();
}

void rpm_init(void) {
    LPC_PINCON->PINSEL3 |= (ADC04_PINSELECT_VALUE << ADC04_PINSELECT_SHIFT);
    adc0_init();
}

void check_rpm(void) {
    uint16_t tmp_reading = adc0_get_reading(4);
    if(tmp_reading>1900 && !white_mark){
        white_mark_count += 1;
        white_mark = true;
    }
    else if(tmp_reading<1900 && white_mark){
        white_mark = false;
    }
    check_time += 1 ;
}

bool check_validity_speed_factor(float s_factor, int speed ){
    switch(speed){
        case slow:
            if(s_factor>DC_THRESH_SLOW)
                return false;
            else
                return true;
            break;

        case normal:
            if(s_factor>DC_THRESH_NORMAL)
                return false;
            else
                return true;
            break;

        case turbo:
            if(s_factor>DC_THRESH_TURBO)
                return false;
            else
                return true;
            break;
    }
    return true;
}
