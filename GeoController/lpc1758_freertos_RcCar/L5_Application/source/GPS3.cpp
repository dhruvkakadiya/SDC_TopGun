/*
 * GPS3.cpp
 *
 *  Created on: Dec 4, 2015
 *      Author: Chitrang
 */

#include "UART_dev.hpp"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

extern char gps_global_string[100];

void uart2init()
{
    // In our inter-board communication we have used UART 2 for transmission and reception. So, We will unable UART 2.
    LPC_SC->PCONP |= (1<<24);  // We have to Power on the UART 2 peripheral. To do so we have to enable bit 24 of PCONP register.
    LPC_SC->PCLKSEL1 &= ~((1<<16)|(1<<17));    // Peripheral Clock = CPUClock/4. (48 Mhz/4= 12 Mhz)
    LPC_PINCON->PINSEL4 &= ~((1<<19)|(1<<18)|(1<<17)|(1<<16)); // First We clear the values of the bits 16 to 19 to Zero. Now we want the GPIO pins as UART 2. So we have to Select 01 for the selection lines. 16:17 = 0:1 and 18:19= 0:1
    LPC_PINCON->PINSEL4 |= ((1<<19) | (1<<17));  // We have to set this bits to meet the requirements of selection lines.
    LPC_UART2-> LCR  |= 0xFFFFFF83;    // To access DLL and DLM we have to set 7th bit of the LCR. We are enabling Dlatch by setting this bit.
    // 0 and 1 bits of LCR is 11. 8 bit data format. 1 stop bit. No parity.
    LPC_UART2 -> DLL = 71;         // To set the baud rate 9600 bps we have to set divisor latch least significant bits to 96.
    LPC_UART2 -> DLM = 0;// and most significant bits to 0.
    LPC_UART2-> FDR = 0xA1;
    LPC_UART2-> LCR &=~(1<<7) ;    // We have to reset the Dlatch bit before starting the communication.
    LPC_UART2->FCR = (1 << 0) | (1 << 6);
    LPC_UART2->FCR |= (1 << 1) | (1 << 2);
    LPC_UART2->IER = (1 << 0);
}

float get_decimal1(int deg, float minute){
    return deg + minute/60;
}



int get_lat_degree1(void){

    char buffer[10] = {'\0'};
    buffer[0] = gps_global_string[20];
    buffer[1] = gps_global_string[21];
    return atoi(buffer);
}

float get_lat_minute1(void){

    char buffer[10] = {'\0'};
    int j = 22;
    int i = 0;
    for(i = 0; i < 7 ; i++){
  buffer[i] = gps_global_string[j + i];
    }

    return atof(buffer);
}


int get_long_degree1(void){
    char buffer[10]={'\0'};

    buffer[0] = gps_global_string[32];
    buffer[1] = gps_global_string[33];
    buffer[2] = gps_global_string[34];

    return atoi(buffer);
}

float get_long_minute1(void){

    char buffer[10]={'\0'};
    int i =0;
    for(i = 0; i< 7 ; i++){
    buffer[i] = gps_global_string[35 + i];
    }
    return atof(buffer);
}

float get_speed_GPS1(void){

    char buffer[10] = {'\0'};

    int i =0;
    for(i = 0; i< 4 ; i++){
    buffer[i] = gps_global_string[45 + i];
    }

   return atof(buffer);
}


