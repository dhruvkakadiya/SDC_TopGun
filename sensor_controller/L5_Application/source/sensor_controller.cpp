/*
 * sensor_controller.cpp
 *
 *  Created on: Nov 1, 2015
 *      Author: Dhruv
 */

#include "sensor_controller.hpp"
#include "eint.h"

#define FILTER_LENGTH  1

QueueHandle_t sensor_task= xQueueCreate(10,sizeof(dist_sensor));
SemaphoreHandle_t Trig_Sem;

Sensor sen_front_center;
Sensor sen_front_left;
Sensor sen_front_right;
Sensor sen_left;
Sensor sen_right;
Sensor sen_back;

int up_time,down_time,down_time1,diff_time;
float buff_for_receive;
static float distance_value;
//Queue for sharing the distance values
static QueueHandle_t xQueue1;

GPIO trig_out(P0_30);
GPIO echo_in(P0_29);

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
        //printf("front_left: %d\n",(uint8_t)ds.front_left);
        //printf("front_right: %d\n",(uint8_t)ds.front_right);
        //printf("front_center: %d\n",(uint8_t)ds.front_center);

        //printf("Center sensor:%d\n",sensor_msg.data.bytes[2]);
        if(transmit_data(sensor_msg)) {
        //    LE.on(1);
        }
        else
        {
          //  LE.off(2);
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
        //read_sensor_data1(&sen_front_center,1,19);
        //read_sensor_data1(&sen_front_right,1,22,1,20);

    }
    else{
        read_sensor_data1(&sen_front_center,1,19);
        //read_sensor_data0(&sen_front_left,0,30,0,29);
        //LE.toggle(4);
    }

    dist_sensor ds1;
    ds1.front_center=sen_front_center.szone;
    ds1.front_right=sen_front_right.szone;
    ds1.front_left=sen_front_left.szone;
    ds1.left=0;//sen_left.szone;
    ds1.right=0;//sen_right.szone;
    ds1.back=0;//sen_back.szone;
    //printf("front_center: %c\n",ds1.front_center);
    //printf("front_right: %c\n",ds1.front_right);
    //printf("rhs: %d\n", sen_front_center.szone);
    //printf("lhs: %d\n", ds1.front_center);
    if(!xQueueSend(sensor_task, &ds1, 2)) {
//             puts("Failed to send item to queue within 2ms");
        }
//    if(sen_front_right.szone==near || sen_front_center.szone==near || sen_front_left.szone==near ||
  //                  sen_right.szone==near || sen_left.szone==near)
    //    LE.on(1);
    //else
     //   LE.off(1);

    sen_count++;
    if(sen_count>1000){
        sen_count=0;
    }

    if(sen_front_center.szone > mid )
    {
        LE.on(1);
        //printf("FCS: %d\n", sen_front_center.szone);
    }
    else
        LE.off(1);

    if(sen_front_right.szone > mid )
            LE.on(2);
        else
            LE.off(2);

    if(sen_front_left.szone > mid )
        LE.on(4);
    else
        LE.off(4);


}
void echo_high_callback()
{
    up_time = (int)sys_get_uptime_us();
    //printf("uptime: %d\n",up_time);
}

void echo_low_callback()
{
    down_time = (int)sys_get_uptime_us();

    xQueueSendFromISR(xQueue1,&down_time,0);

    xSemaphoreGiveFromISR(Trig_Sem,false);

}

void ultra_sensor_init(){
    Trig_Sem = xSemaphoreCreateBinary();  //first give the Sem at initialization
    xSemaphoreGive(Trig_Sem);             //send trig() will first check if its taken
    eint3_enable_port0(29,eint_rising_edge,echo_high_callback);
    eint3_enable_port0(29,eint_falling_edge,echo_low_callback);
    trig_out.setAsOutput();
    echo_in.setAsInput();
    xQueue1 = xQueueCreate(2,sizeof(float));
}

bool send_trig()
{
    if( xSemaphoreTake(Trig_Sem,0))
    {
        //reset the GPIO pins
        trig_out.set(false);
        delay_us(5);

        trig_out.setHigh();
        delay_us(10);
       trig_out.setLow();

    }
    return true;
}

bool receive_from_queue()
{
    //this will fetch the value from the queue and put it in a buffer(pass-by-ref),0 delay / ticks
    xQueueReceive( xQueue1, &down_time1, 25);

    //Calculate the time of the ECHO pulse
    diff_time = down_time1 - up_time;
    distance_value = (float)(diff_time)*(1.0/58);  //Distance in cms

    return true;
}

void get_ultrasonic_data(){
    if(send_trig()){
        receive_from_queue();
        printf("Distance: %f\n",distance_value);
       // vTaskDelay(10);
    }
}
