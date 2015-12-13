/*
 * lcd_test_new.cpp
 *
 *  Created on: Nov 1, 2015
 *      Author: ANUJKORAT
 */

#include "lcd_test_new.hpp"
#include "io.hpp"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "utilities.h"
#include "_can_dbc/generated_motorio_code.h"

bool flag_geo = false;
bool flag_geo_page = false;
bool flag_sensors = false;
bool flag_sensors_page = false;
bool flag_motor = false;
bool flag_motor_page = false;
bool flag_home = false;
bool flag_home_page = false;
char lcd_char;

lcd_screen_t lcdscreen = home;      // Enum variable for current screen on LCD
extern GEO_TX_GEO_SPEED_ANGLE_t geo_msg;
extern DRIVER_TX_MOTORIO_DIRECTION_t motor_msg;
extern SENSOR_TX_SENSOR_SONARS_t sensor_msg;
extern GEO_TX_GEO_LOC_DATA_t geo_loc_msg;
extern SENSOR_TX_SENSOR_LIGHT_BAT_t sensor_bat_msg;

extern can_msg_t motor_lcd,geo_lcd,sensor_lcd,geo_loc_lcd;

char Bytes6[6]={0};                 // 6 bytes received from the LCD
char ack=0;                         // Acknowledge byte from LCD


void flag_change(bool* flagp){
    flag_geo = false;
    flag_home = false;
    flag_sensors = false;
    flag_motor = false;
    *flagp = true;
}

void flag_page_change(bool* flagp){
    flag_geo_page = false;
    flag_home_page = false;
    flag_sensors_page = false;
    flag_motor_page = false;
    *flagp = true;
}

/* Function to initialize UART for LCD */
void lcd_init(void) {
    U2.init(LCD_UART_BAUD,LCD_UART_RXQSIZE,LCD_UART_TXQSIZE);
}

void put_comm(char a,char b,char c, char d,char e) {
    char comm[6]={a,b,c,d,e,0};
    for(int i=0;i<5;i++) {
        comm[5]^=comm[i];
    }

    for(int i=0;i<6;i++) {
        U2.putChar(comm[i],10);
    }
    U2.getChar(&ack,10);
}

/* Function to print string on LCD */
void put_string(char array[], uint8_t object_no, uint8_t num){
    uint8_t checksum=0x02^object_no^num;
    U2.putChar(0x02,10);
    U2.putChar(object_no,10);
    U2.putChar(num,10);

    for(int i=0;i<num;i++){
        U2.putChar(array[i],10);
        checksum^=array[i];
    }

    U2.putChar(checksum,10);
    U2.getChar(&ack,10);
}

/* Function to print different data on LCD */
void lcd_print(){

     static bool headlights_on = false;
     bool flag_headlight = false;

     if(Bytes6[1]==0x06 && Bytes6[2]==0x00) {
         lcdscreen=Geo;
         flag_change(&flag_geo);
     }
     else if(Bytes6[1]==0x06 && Bytes6[2]==0x01) {
         lcdscreen=Sensors;
         flag_change(&flag_sensors);
     }
     else if(Bytes6[1]==0x06 && Bytes6[2]==0x02) {
          lcdscreen=Motor;
          flag_change(&flag_motor);
     }
     else if(Bytes6[1]==0x1e && (Bytes6[2]==0x02 || Bytes6[2]==0x04 || Bytes6[2]==0x01)) {
          lcdscreen=home;
          flag_change(&flag_home);
     }
     if(flag_geo && !flag_geo_page) {
         put_comm(0x01,0x0a,0x02,0x00,0x00); // switch to form2
         flag_page_change(&flag_geo_page);
     }
     else if(flag_sensors && !flag_sensors_page) {
         put_comm(0x01,0x0a,0x03,0x00,0x00); // switch to form3
         flag_page_change(&flag_sensors_page);
     }
     else if(flag_motor && !flag_motor_page) {
          put_comm(0x01,0x0a,0x04,0x00,0x00); // switch to form4
          flag_page_change(&flag_motor_page);
      }
     else if(flag_home && !flag_home_page) {
          put_comm(0x01,0x0a,0x00,0x00,0x00); // switch to form0
          flag_page_change(&flag_home_page);
      }
/*
     if(lcdscreen == Geo){
         put_comm(0x01, 0x07, 0x01, 0x00, geo_msg.GEO_ANGLE_heading_cmd);
         static char string[30]={0};
         sprintf(string,"%f", geo_loc_msg.GEO_LOC_LAT_cmd);
         put_string(string, 0,(uint8_t)strlen(string));
         sprintf(string,"%f", geo_loc_msg.GEO_LOC_LONG_cmd);
         put_string(string, 1,(uint8_t)strlen(string));
     }
     else if(lcdscreen == Sensors){
         printf("left:%d\n",sensor_msg.SENSOR_SONARS_front_left);
         printf("right:%d\n",sensor_msg.SENSOR_SONARS_front_right);
         printf("center:%d\n",sensor_msg.SENSOR_SONARS_front_center);
         //printf("ds_center:%d\n",dist_sensor_can_msg.data.bytes[0]);
         put_comm(0x01, 0x0b, 0x00, 0x00, sensor_msg.SENSOR_SONARS_front_center);
         put_comm(0x01, 0x0b, 0x02, 0x00, sensor_msg.SENSOR_SONARS_front_left);
         put_comm(0x01, 0x0b, 0x01, 0x00, sensor_msg.SENSOR_SONARS_front_right);
         put_comm(0x01, 0x0b, 0x04, 0x00, sensor_msg.SENSOR_SONARS_left);
         put_comm(0x01, 0x0b, 0x03, 0x00, sensor_msg.SENSOR_SONARS_right);
         put_comm(0x01, 0x0b, 0x05, 0x00, sensor_msg.SENSOR_SONARS_back);
     }
     else if(lcdscreen == home){
         put_comm(0x01, 0x1a, 0x00, 0x00, sensor_bat_msg.SENSOR_BAT_cmd); // setting the battery meter here
         if(Bytes6[1]==0x21 && Bytes6[2]==0x00){
             headlights_on = headlights_on? false:true;
             if(headlights_on)
                 put_comm(0x01, 0x13, 0x00, 0x00, 0x01);
             else
                 put_comm(0x01, 0x13, 0x00, 0x00, 0x00);
             Bytes6[1] = 0x00;
         }
     }
     else if(lcdscreen == Motor){
         //printf("speed:%d\n",motor_msg.MOTORIO_DIRECTION_speed_cmd);
         //printf("turn:%d\n",motor_msg.MOTORIO_DIRECTION_turn_cmd);
         put_comm(0x01, 0x0b, 0x06, 0x00, motor_msg.MOTORIO_DIRECTION_speed_cmd);
         put_comm(0x01, 0x10, 0x00, 0x00, motor_msg.MOTORIO_DIRECTION_turn_cmd);
         if(motor_msg.MOTORIO_DIRECTION_turn_cmd==back)
             put_comm(0x01, 0x0b, 0x07, 0x00, motor_msg.MOTORIO_DIRECTION_turn_cmd);
         else
             put_comm(0x01, 0x10, 0x00, 0x00, 0);
     }*/
     if(lcdscreen == Geo){

          geo_loc* geo_loc_Ptr = (geo_loc*)(&geo_loc_lcd.data);
          geo_spd_angle* gsaPtr = (geo_spd_angle*)(&geo_lcd.data);
          //printf("lat:%f\n",(float)geo_loc_data->latitude);
          //printf("long:%f\n",(float)geo_loc_data->longitude);
          put_comm(0x01, 0x07, 0x01, geo_lcd.data.bytes[2], geo_lcd.data.bytes[1]);
          static char string[30]={0};

          sprintf(string,"%f", (float)geo_loc_Ptr->latitude);
          put_string(string, 0,(uint8_t)strlen(string));                //Print Latitude on LCD
          sprintf(string,"%f", (float)geo_loc_Ptr->longitude);
          put_string(string, 1,(uint8_t)strlen(string));                //Print Longitude on LCD
          sprintf(string,"%i", gsaPtr->bearing);
          put_string(string, 2,(uint8_t)strlen(string));                //Print desired Bearing of car on LCD
          sprintf(string,"%f", (float)(gsaPtr->distance)/10000);
          put_string(string, 3,(uint8_t)strlen(string));                //Print Car's Distance from next checkpoint on LCD
      }
      else if(lcdscreen == Sensors){
          //printf("left:%d\n",sensor_lcd.data.bytes[0]);
          //printf("right:%d\n",sensor_lcd.data.bytes[1]);
          //printf("center:%d\n",sensor_lcd.data.bytes[2]);
          //printf("ds_center:%d\n",dist_sensor_can_msg.data.bytes[0]);
          put_comm(0x01, 0x0b, 0x00, 0x00, sensor_lcd.data.bytes[2]);       //Print Front_Center sensor data on LCD
          put_comm(0x01, 0x0b, 0x02, 0x00, sensor_lcd.data.bytes[0]);       //Print Front_Left sensor data on LCD
          put_comm(0x01, 0x0b, 0x01, 0x00, sensor_lcd.data.bytes[1]);       //Print Front_Right sensor data on LCD
          put_comm(0x01, 0x0b, 0x04, 0x00, sensor_lcd.data.bytes[3]);       //Print Left sensor data on LCD
          put_comm(0x01, 0x0b, 0x03, 0x00, sensor_lcd.data.bytes[4]);       //Print Right sensor data on LCD
          put_comm(0x01, 0x0b, 0x05, 0x00, sensor_lcd.data.bytes[5]);       //Print Back sensor data on LCD
      }
      else if(lcdscreen == home){
          put_comm(0x01, 0x1a, 0x00, 0x00, sensor_bat_msg.SENSOR_BAT_cmd); // setting the battery meter here
          if(Bytes6[1]==0x21 && Bytes6[2]==0x00){
              headlights_on = headlights_on? false:true;
              if(headlights_on)
                  put_comm(0x01, 0x13, 0x00, 0x00, 0x01);
              else
                  put_comm(0x01, 0x13, 0x00, 0x00, 0x00);
              Bytes6[1] = 0x00;
          }
      }
      else if(lcdscreen == Motor){
          //printf("speed:%d\n",motor_msg.MOTORIO_DIRECTION_speed_cmd);
          //printf("turn:%d\n",motor_msg.MOTORIO_DIRECTION_turn_cmd);
          put_comm(0x01, 0x0b, 0x06, 0x00, motor_lcd.data.bytes[0]);        //Print Motor Speed data on LCD
          put_comm(0x01, 0x10, 0x00, 0x00, motor_lcd.data.bytes[1]);        //Print Motor turn data on LCD
          if(motor_msg.MOTORIO_DIRECTION_turn_cmd==back)
              put_comm(0x01, 0x0b, 0x07, 0x00, motor_lcd.data.bytes[1]);
          else
              put_comm(0x01, 0x10, 0x00, 0x00, 0);
      }

}

/* Function to receive touch screen data from LCD */
void lcd_receive(){

      U2.getChar(&lcd_char,0);
      if (lcd_char == 0x07)
      {
          Bytes6[0]=lcd_char;
          for(int i=1;i<6;i++) {
              U2.getChar(Bytes6+i,0);
          }
      }
}




