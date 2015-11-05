/*
 * sensor_controller.cpp
 *
 *  Created on: Nov 1, 2015
 *      Author: Dhruv
 */

#include "sensor_controller.hpp"
#include "eint.h"

#define FILTER_LENGTH           1
#define FRONT_LEFT_LED          1
#define FRONT_CENTER_LED        2
#define FRONT_RIGHT_LED         3
#define SENSOR_LED_INDICATION   1

QueueHandle_t sensor_task= xQueueCreate(10,sizeof(dist_sensor));

Sensor sen_front_center;
Sensor sen_front_left;
Sensor sen_front_right;
Sensor sen_left;
Sensor sen_right;
Sensor sen_back;

void can_sensor_tx_task(void) {
    can_msg_t sensor_msg;
    sensor_msg.msg_id = DISTANCE_SENSOR_ID;
    dist_sensor ds;
    if(!xQueueReceive(sensor_task, &ds, 1)) {
            //puts("Failed to receive item within 1 ms\n");
            return;
    }
   else {
        memcpy(&(sensor_msg.data.qword), &ds, sizeof(dist_sensor));
        if(transmit_data(sensor_msg)) {

        }
        else
        {

        }
    }
}

// This function is for 3 pin ultrasonic sensor and port1
void read_sensor_data1(Sensor* s1,int port_no,int pin_no){

#define port LPC_GPIO1
    s1->limit_time.reset(25);
    s1->limit_time.restart();
    while(1){
        if(s1->flag_u) {
            port->FIODIR |= 1<<pin_no;
            port->FIOSET = 1<<pin_no;
            delay_us(10);
            port->FIOCLR = 1<<pin_no;
            port->FIODIR &= ~(1<<pin_no);
            s1->flag_u = false;
        }
        if((port->FIOPIN & (1 << pin_no)) && !s1->flag_u_high) {
            lpc_timer_enable(lpc_timer0,1);
            lpc_timer_set_value(lpc_timer0,0);
            s1->flag_u_high = true;
        }
        if(!(port->FIOPIN & (1 << pin_no)) && s1->flag_u_high){
            s1->timer_val = lpc_timer_get_value(lpc_timer0);
            s1->dist_val = 340.0 * (float)s1->timer_val * 0.0001/2.0;
            if(s1->dist_val<100) {
                s1->szone = (uint8_t)near;
               // LE.on(1);
            }
            else if(s1->dist_val<200) {
                s1->szone = (uint8_t)mid;
            }
            else if(s1->dist_val<350) {
                s1->szone = (uint8_t)far;
                //LE.off(1);
            }
            else {
                s1->szone = (uint8_t)no_obstacle;
               // LE.off(1);
            }
           // printf("center: %f\n",s1->dist_val);
            s1->flag_u = true;
            s1->flag_u_high = false;
            break;
        }
        if(s1->limit_time.expired()){
            s1->flag_u = true;
            s1->flag_u_high = false;
            break;
        }
    }
}

// This function is for 4 pin ultrasonic sensor and port1
void read_sensor_data1(Sensor* s1,int port_trigger, int trigger_pin,int port_echo, int echo_pin){

    #define port LPC_GPIO1
    #define port_echo1 LPC_GPIO1
    s1->limit_time.reset(30);
    s1->limit_time.restart();
    while(1)
    {
        if(s1->flag_u) {

            port->FIODIR |= (1 << trigger_pin);  // set as output
            port->FIOSET = (1 << trigger_pin);
            delay_us(10);
            port->FIOCLR = (1 << trigger_pin);
            port_echo1 -> FIODIR &= ~(1 << echo_pin); // set echo pin as input
            s1->flag_u = false;
        }

        if((port_echo1->FIOPIN & (1 << echo_pin)) && !s1->flag_u_high) {
            lpc_timer_enable(lpc_timer0,1);
            lpc_timer_set_value(lpc_timer0,0);
            s1->flag_u_high = true;
        }
        if(!(port_echo1->FIOPIN & (1 << echo_pin)) && s1->flag_u_high){
            s1->timer_val = lpc_timer_get_value(lpc_timer0);
            s1->dist_val += 340.0 * (float)s1->timer_val * 0.0001/2.0;
            s1->filter_count++;
            if(s1->filter_count>FILTER_LENGTH){
                s1->dist_val/=FILTER_LENGTH;

                if(s1->dist_val<150) {
                    s1->szone = (uint8_t)near;
                    //LE.on(1);
                }
                else if(s1->dist_val<200) {
                    s1->szone = (uint8_t)mid;
                }
                else if(s1->dist_val<300) {
                    s1->szone = (uint8_t)far;
                    //LE.off(1);
                }
                else {
                    s1->szone = (uint8_t)no_obstacle;
                    //LE.off(1);
                }
                s1->filter_count = 0;
                s1->dist_val = 0;
               // printf("right: %f\n",s1->dist_val);
            }

            s1->flag_u = true;
            s1->flag_u_high = false;
            break;
        }
        if(s1->limit_time.expired()){
            s1->flag_u = true;
            s1->flag_u_high = false;
            break;
        }
    }
}

void get_sensor_data(void) {
    static int sen_count=0;
    if(sen_count%2) {
        read_sensor_data1(&sen_front_right,1,22,1,20);
        read_sensor_data1(&sen_front_left,1,28,1,23);
    }
    else {
        read_sensor_data1(&sen_front_center,1,19);
    }

    dist_sensor sensor_data;
    sensor_data.front_center=sen_front_center.szone;
    sensor_data.front_right=sen_front_right.szone;
    sensor_data.front_left=sen_front_left.szone;
    sensor_data.left=0;//sen_left.szone;
    sensor_data.right=0;//sen_right.szone;
    sensor_data.back=0;//sen_back.szone;

    if(!xQueueSend(sensor_task, &sensor_data, 1)) {

    }

#if 0
    if(sen_front_right.szone==near || sen_front_center.szone==near || sen_front_left.szone==near ||
                    sen_right.szone==near || sen_left.szone==near)
        LE.on(1);
    else
        LE.off(1);
#endif

    sen_count++;
    if(sen_count>1000){
        sen_count=0;
    }

#if SENSOR_LED_INDICATION
    if(sen_front_left.szone > mid )
        LE.on(FRONT_LEFT_LED);
    else
        LE.off(FRONT_LEFT_LED);

    if(sen_front_center.szone > mid )
        LE.on(FRONT_CENTER_LED);
    else
        LE.off(FRONT_CENTER_LED);

    if(sen_front_right.szone > mid )
        LE.on(FRONT_RIGHT_LED);
    else
        LE.off(FRONT_RIGHT_LED);
#endif
}
