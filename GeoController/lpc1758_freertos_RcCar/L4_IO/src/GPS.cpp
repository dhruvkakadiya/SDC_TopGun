/*
 * GPS.cpp
 *
 *  Created on: Oct 25, 2015
 *      Author: Chitrang
 */
#include "GPS.hpp"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "utilities.h"
#include "can_msg_id.h"

#define PMTK_SET_NMEA_UPDATE_10HZ  "$PMTK220,100*2F" //for changing the update rate to 1HZ
#define PMTK_SET_NMEA_UPDATE_200_MILLIHERTZ  "$PMTK220,5000*1B" //for changing to 200KHZ
#define PMTK_API_SET_FIX_CTL_5HZ  "$PMTK300,200,0,0,0,0*2F"
#define PMTK_SET_BAUD_9600 "$PMTK251,9600*17"
#define PMTK_SET_BAUD_57600 "$PMTK251,57600*2C"
#define PMTK_SET_NMEA_OUTPUT_RMCONLY "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"

geo_location gps_data_dec; // Structure to fill the GPS data(Integer Format)

uint8_t speed_gps;

Uart2 &uart2_ref = Uart2::getInstance();

bool gps_init(void){

        uart2_ref.init(9600,100,100);
        uart2_ref.putline(PMTK_SET_BAUD_9600,1);
        uart2_ref.putline(PMTK_SET_NMEA_UPDATE_10HZ,1);
    //  uart2_ref.putline(PMTK_API_SET_FIX_CTL_5HZ,99999999);
   //   uart2_ref.putline(PMTK_SET_BAUD_57600,99999999);
        uart2_ref.putline(PMTK_SET_NMEA_OUTPUT_RMCONLY,1);
     // uart2_ref.putChar(clear_byte1,9999999);
      //uart2_ref.putChar(clear_byte2,9999999);
        return true;
}

bool gps_data(){

    static int lat_degree = 0;
    static float lat_minute = 0;
    static int long_degree = 0;
    static float long_minute = 0;
    static float lat_dec;
    static float long_dec;

    lat_degree = get_lat_degree();
    lat_minute =get_lat_minute();
    long_degree =get_long_degree();
    long_minute =get_long_minute();
    speed_gps = (uint8_t)(1.150779)*get_speed_GPS();


     lat_dec = get_decimal(lat_degree, lat_minute);
     long_dec = get_decimal(long_degree, long_minute);

     gps_data_dec.latitude = lat_dec;
     gps_data_dec.longitude = (-1)*long_dec;

/*
     diff = lat_dec - (int)lat_dec;
     gps_data_byte.gpsbyte1 = (int8_t) (diff*(100));
     diff = (diff*100) - (int) (diff*(100));
     gps_data_byte.gpsbyte2 = (int8_t) (diff * (100));
     diff = (diff*100) - (int) (diff*(100));
     diff = (diff*100) + 0.5;
     gps_data_byte.gpsbyte3 = (int8_t)diff;

     diff = long_dec - (int)long_dec;
     gps_data_byte.gpsbyte4 = (int8_t) (diff*(100));
     diff = (diff*100) - (int) (diff*(100));
     gps_data_byte.gpsbyte5 = (int8_t) (diff * (100));
     diff = (diff*100) - (int) (diff*(100));
     diff = (diff*100) + 0.5;
     gps_data_byte.gpsbyte6 = (int8_t)diff;

     gps_data_byte.gpsbyte7 = (int8_t) (speed_mph);
     diff = speed_mph - (int)speed_mph;
     gps_data_byte.gpsbyte8 = (int8_t) (diff*(100));
*/

  //$GPRMC,194509.000,A,4042.6142,N,07400.4168,W,2.03,221.11,160412,,,A*77
  //printf(" %d %lf %d %lf, %lf \n", lat_degree, lat_minute, long_degree, long_minute, speed_mph);
 //   printf(" %lf %lf \n", lat_dec, long_dec);

/* This code is for the master side to recover the gps data
    static double lat_dec_rx;
    static double long_dec_rx;
    static double speed_rc_car;

    lat_dec_rx = ((double)gps_data_byte.gpsbyte1/100) + ((double)gps_data_byte.gpsbyte2/10000) + ((double)gps_data_byte.gpsbyte3/1000000) + (37.0);
    long_dec_rx = ((double)gps_data_byte.gpsbyte4/100) + ((double)gps_data_byte.gpsbyte5/10000) + ((double)gps_data_byte.gpsbyte6/1000000) + (121.0);
    speed_rc_car = ((double)gps_data_byte.gpsbyte7) + ((double)gps_data_byte.gpsbyte8/100);

    printf("lat:%lf    long:%lf    spd:%lf\n",lat_dec_rx,long_dec_rx,speed_rc_car);

*/
   // vTaskDelay(200);
    return true;
}

float get_decimal(int deg, float minute){
    return deg + minute/60;

}

int get_lat_degree(void){
    char a;
        char buffer[10]={'\0'};
        while(a!='C'){
            uart2_ref.getChar(&a,1);
        }
        while(a!='A'){
            uart2_ref.getChar(&a,1);
         //   if(a == 'V')
          //      return 0;
        }

        uart2_ref.getChar(&a,1);
        int count=0;
        while(count<2){
            uart2_ref.getChar(&a,1);
            buffer[count++]=a;
        }

        return atoi(buffer);
}

double get_lat_minute(void){
    char a;
    char buffer[10]={'\0'};
    int count=0;
    while(count<7){
        uart2_ref.getChar(&a,1);
        buffer[count++]=a;
    }
    return atof(buffer);
}


int get_long_degree(void){
    char a;
    char buffer[10]={'\0'};
    while(a!='N'){
        uart2_ref.getChar(&a,1);
    }
    uart2_ref.getChar(&a,1);
    int count=0;
    while(count<3){
        uart2_ref.getChar(&a,1);
        buffer[count++]=a;
    }
    return atoi(buffer);
}

double get_long_minute(void){
    char a;
    char buffer[10]={'\0'};
    int count=0;
    while(count<7){
        uart2_ref.getChar(&a,1);
        buffer[count++]=a;
    }
    return atof(buffer);
}

float get_speed_GPS(void){
    char a;
    char buffer[10] = {'\0'};
    while(a!='W'){
            uart2_ref.getChar(&a,1);
        }
    uart2_ref.getChar(&a,1);
    int count=0;
        while(count<4){
            uart2_ref.getChar(&a,1);
            buffer[count++]=a;
        }
        return atof(buffer);

}


