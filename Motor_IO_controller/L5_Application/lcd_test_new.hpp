/*
 * lcd_test_new.hpp
 *
 *  Created on: Nov 1, 2015
 *      Author: ANUJKORAT
 */

#ifndef L5_APPLICATION_LCD_TEST_NEW_HPP_
#define L5_APPLICATION_LCD_TEST_NEW_HPP_

#include "can_msg_id.hpp"

#define LCD_UART_BAUD           115200
#define LCD_UART_RXQSIZE        100
#define LCD_UART_TXQSIZE        100

/*
 *lcd_screen_t is enum type for user input on LCD
 *User will select the particular screen he wants on home screen
 */
typedef enum{
    home,
    Geo,
    Sensors,
    Motor,

}lcd_screen_t;


/*
 * Printing on the lcd every 1000ms (1Hz), for now 100ms (10Hz)
 * Called by periodic scheduler
 */
void lcd_print();

/*
 * Sending any command to LCD
 * e.g.changing the screen on LCD, setting any value on a digital meter
 * A total of six bytes for one command, sixth byte if the checksum
 * Checksum is calculated in the function itself
 */
void put_comm(char a,char b,char c, char d,char e);

/*
 * Sending a character string to LCD
 * array[] is the pointer to the input string
 * num is the size of the string
 */
void put_string(char array[],uint8_t num);

// lcd_test(uint8_t priority);

// bool init(void);
// bool run(void *p);

/*
 * Put the address of the global variable of a particular flag to get it set
 * Other all global variables of the same flag type will get reset
 */
void lcd_init(void);
void flag_change(bool* flagp);
void flag_page_change(bool* flagp);
void lcd_receive(void);




#endif /* L5_APPLICATION_LCD_TEST_NEW_HPP_ */
