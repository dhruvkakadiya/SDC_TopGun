/*
 * sensor_controller.cpp
 *
 *  Created on: Nov 1, 2015
 *      Author: Dhruv
 */

#include "sensor_controller.hpp"

QueueHandle_t sensor_task= xQueueCreate(10,sizeof(dist_sensor));
Sensor sen_front_center;
Sensor sen_front_left;
Sensor sen_front_right;
Sensor sen_left;
Sensor sen_right;
Sensor sen_back;

void can_sensor_tx_task(void) {
    can_msg_t sensor_msg;
    dist_sensor *ds=new dist_sensor;
    if(!xQueueReceive(sensor_task, ds, 1)) {
            puts("Failed to receive item within 1 ms\n");
            return;
    }
   else{
        sensor_msg.msg_id = 0x0C;
        sensor_msg.data.bytes[0] = ds->front_left;
        sensor_msg.data.bytes[1] = ds->front_right;
        sensor_msg.data.bytes[2] = ds->front_center;
        sensor_msg.data.bytes[3] = ds->left;
        sensor_msg.data.bytes[4] = ds->right;
        sensor_msg.data.bytes[5] = ds->back;
        printf("Center sensor:%d\n",sensor_msg.data.bytes[2]);
        if(transmit_data()) {
        LE.on(1);
        }
        else
        {
            LE.off(2);
        }
    }
    /*

    if(obstacle) {
        message1.msg_id = 0x0C;
        message1.data.qword = 0x1; // Write all 8 bytes of data at once
        if(transmit_data()) {
            LE.on(1);
        }
    }
    else {
        message1.msg_id = 0x0C;
        message1.data.qword = 0x0; // Write all 8 bytes of data at once
        if(transmit_data()) {
            LE.off(1);
        }
    }
    */
}



void get_sensor_data2(Sensor* s1,int port_no,int pin_no){

#define port LPC_GPIO2
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
                s1->szone = near;
               // LE.on(1);
            }
            else if(s1->dist_val<200) {
                s1->szone = mid;
            }
            else if(s1->dist_val<350) {
                s1->szone = far;
                //LE.off(1);
            }
            else {
                s1->szone = no_obstacle;
               // LE.off(1);
            }
            //printf("Distance value: %f\n",s1->dist_val);
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

// Below function is for four pin Ultrasonic Sensors.
void get_sensor_data1(Sensor* s1,int port_trigger, int trigger_pin,int port_echo, int echo_pin){

    #define port LPC_GPIO1
    #define port_echo1 LPC_GPIO1
    s1->limit_time.reset(30);
    s1->limit_time.restart();
    while(1)
    {
        if(s1->flag_u) {

            port->FIODIR |= (1 << trigger_pin);  // set as output
            port->FIOSET= (1 << trigger_pin);
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
            s1->dist_val = 340.0 * (float)s1->timer_val * 0.0001/2.0;
            if(s1->dist_val<100) {
                s1->szone = near;
                //LE.on(1);
            }
            else if(s1->dist_val<200) {
                s1->szone = mid;
            }
            else if(s1->dist_val<300) {
                s1->szone = far;
                //LE.off(1);
            }
            else {
                s1->szone = no_obstacle;
                //LE.off(1);
            }
            //printf("Distance value: %f\n",s1->dist_val);
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


// Below function is for four pin Ultrasonic Sensors.
void get_sensor_data2(Sensor* s1, int port_trigger, int trigger_pin,int port_echo, int echo_pin){

    #define port LPC_GPIO2
    #define port_echo1 LPC_GPIO2

    s1->limit_time.reset(30);
    s1->limit_time.restart();
    while(1)
    {

        if(s1->flag_u) {

            port->FIODIR |= (1 << trigger_pin);  // set as output
            port->FIOSET= (1 << trigger_pin);
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
            s1->dist_val = 340.0 * (float)s1->timer_val * 0.0001/2.0;
            if(s1->dist_val<100) {
                s1->szone = near;
                //LE.on(1);
            }
            else if(s1->dist_val<200) {
                s1->szone = mid;
            }
            else if(s1->dist_val<300) {
                s1->szone = far;
                //LE.off(1);
            }
            else {
                s1->szone = no_obstacle;
                //LE.off(1);
            }
          //  printf("Distance value: %f\n",s1->dist_val);
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
        get_sensor_data2(&sen_front_center,2,6,2,7);
        get_sensor_data2(&sen_front_left,2,4,2,3);
        get_sensor_data2(&sen_front_right,2,5);
    }
    else {

        //  get_sensor_data1(&sen_right,1,29,1,19);
        //  get_sensor_data1(&sen_left,1,23,1,28);
        //printf("Sen right: %d",sen_right.szone);
        //printf("Sen left: %d",sen_left.szone);
        //get_sensor_data(&sen_back,2,3);

    dist_sensor ds;
    ds.front_center=sen_front_center.szone;
    ds.front_right=sen_front_right.szone;
    ds.front_left=sen_front_left.szone;
    ds.left=0;//sen_left.szone;
    ds.right=0;//sen_right.szone;
    ds.back=0;//sen_back.szone;
    if(!xQueueSend(sensor_task, &ds, 2)) {
             puts("Failed to send item to queue within 2ms");
        }
    }
    if(sen_front_right.szone==near || sen_front_center.szone==near || sen_front_left.szone==near ||
                    sen_right.szone==near || sen_left.szone==near)
                LE.on(1);
            else
                LE.off(1);

    sen_count++;
    if(sen_count==1000){
        sen_count=0;
    }
    /*
    if(sen_left.szone==near)
        LE.on(1);
    else
        LE.off(1);

    if(sen_right.szone==near)
            LE.on(2);
        else
            LE.off(2);
*/

}
