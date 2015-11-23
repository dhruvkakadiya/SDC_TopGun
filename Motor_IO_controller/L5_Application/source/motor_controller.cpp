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
#define SPEED_FEEDBACK      1

#define CAN_ERROR_LED           4
#define SPEED_FACTOR_ERROR      4

#define SPEED_VAR_FACTOR    0.05
#define HARD_LEFT           9.0
#define S_LEFT              8.0
#define HARD_RIGHT          5.7
#define S_RIGHT             6.5
#define STRAIGHT            7.5

#define DC_STOP             7.0
#define DC_SLOW             6.4
#define DC_NORMAL           6.350
#define DC_TURBO            6.2

#define DC_THRESH_SLOW             6.3
#define DC_THRESH_NORMAL           6.2
#define DC_THRESH_TURBO            6.1

// For RPM sensor configuration
#define ADC04_PINSELECT_VALUE           (  0x00000003 )
#define ADC04_PINSELECT_SHIFT           ( 28 )

extern DRIVER_TX_MOTORIO_DIRECTION_t motor_msg;

float radius = 0.06;
float dc_slow = 6.4;
float dc_stop = 7.0;
float dc_normal = 6.350;
float dc_turbo = 6.2;

bool white_mark = false;
int white_mark_count = 0;
int check_time = 0;
float speed_factor = DC_STOP;
int desired_count = 0;

//As per waveform and real testing(percent range - 6.0(right) - 7.5(center) - 9.3(left))
//As per waveform only(percent range - 5.5(forward) - 8.5(stop) - 10.5(backward))

void rpm_init(void) {
    LPC_PINCON->PINSEL3 |= (ADC04_PINSELECT_VALUE << ADC04_PINSELECT_SHIFT);
    adc0_init();
}

void motor_init(void){

    LD.init();
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
    rpm_init();

    printf("motor_init is done\n");
}

void set_motors_pwm(void) {

    motor_direction md; // Copy received CAN msg into proper data structure
    md.turn = motor_msg.MOTORIO_DIRECTION_turn_cmd;
    md.speed = motor_msg.MOTORIO_DIRECTION_speed_cmd;

#if SERVO_ON
    if(md.turn==left) {                                       // turn hard left
        MotorControl.setServo(HARD_LEFT);
    }
    else if(md.turn==s_left) {                                // turn slight left
        MotorControl.setServo(S_LEFT);
    }
    else if(md.turn==straight) {                              // keep straight
        MotorControl.setServo(STRAIGHT);
    }
    else if(md.turn==s_right) {                               // turn slight right
        MotorControl.setServo(S_RIGHT);
    }
    else if(md.turn==right) {                                 // turn hard right
        MotorControl.setServo(HARD_RIGHT);
    }
#endif

#if DC_ON
    //--------------------------- Speed of DC Motor ------------------------
    LD.setNumber(white_mark_count);     // Show white_mark_count on Segment display

    if(md.speed == stop) {                                    // Stop motor
        speed_factor = dc_stop;
        desired_count = 0;
    }
    else if(md.speed==slow) {                                 // Slow speed
        speed_factor = dc_slow;
        desired_count = 3;

    }
    else if(md.speed==normal) {                               // Normal Speed
        speed_factor = dc_normal;
        desired_count = 4;

    }
    else if(md.speed==turbo) {                                // Turbo Speed
        speed_factor = dc_turbo;
        desired_count = 5;
    }

/////////////////////////////////////////////Speed Encoder////////////////////////////////////////////////////////

    #if SPEED_FEEDBACK
        if(check_time>1000){
            check_time = 0;
            switch((int)md.speed){

                case stop:
                    // Do nothing
                    break;

                case slow:
                    if(desired_count>white_mark_count){
                        speed_factor -= SPEED_VAR_FACTOR;
                        dc_slow -= SPEED_VAR_FACTOR;
                    }
                    else if(desired_count<white_mark_count){
                        if(white_mark_count-desired_count<3){
                            speed_factor += SPEED_VAR_FACTOR;
                            dc_slow += SPEED_VAR_FACTOR;
                        }
                        else if(white_mark_count-desired_count<5){
                            speed_factor += SPEED_VAR_FACTOR+0.05;
                            dc_slow += SPEED_VAR_FACTOR+0.05;
                        }
                        else {
                            speed_factor += SPEED_VAR_FACTOR+0.15;
                            dc_slow += SPEED_VAR_FACTOR+0.15;
                        }
                    }
                    break;

                case normal:
                    if(desired_count>white_mark_count){
                        speed_factor -= SPEED_VAR_FACTOR;
                        dc_normal -= SPEED_VAR_FACTOR;
                    }
                    else if(desired_count<white_mark_count){
                        if(white_mark_count-desired_count<3){
                            speed_factor += SPEED_VAR_FACTOR;
                            dc_normal += SPEED_VAR_FACTOR;
                        }
                        else if(white_mark_count-desired_count<5){
                            speed_factor += SPEED_VAR_FACTOR+0.05;
                            dc_normal += SPEED_VAR_FACTOR+0.05;
                        }
                        else {
                            speed_factor += SPEED_VAR_FACTOR+0.15;
                            dc_normal += SPEED_VAR_FACTOR+0.15;
                        }
                    }
                    break;

                case turbo:
                    if(desired_count>white_mark_count){
                        speed_factor -= SPEED_VAR_FACTOR;
                        dc_turbo -= SPEED_VAR_FACTOR;
                    }
                    else if(desired_count<white_mark_count){
                        if(white_mark_count-desired_count<3){
                            speed_factor += SPEED_VAR_FACTOR;
                            dc_turbo += SPEED_VAR_FACTOR;
                        }
                        else if(white_mark_count-desired_count<5){
                            speed_factor += SPEED_VAR_FACTOR+0.05;
                            dc_turbo += SPEED_VAR_FACTOR+0.05;
                        }
                        else {
                            speed_factor += SPEED_VAR_FACTOR+0.15;
                            dc_turbo += SPEED_VAR_FACTOR+0.15;
                        }
                    }
                    break;
            }
            white_mark_count = 0;
        }
    #endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////

    if(check_validity_speed_factor(speed_factor,(int)md.speed) || 1){
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
    set_motors_pwm();
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
