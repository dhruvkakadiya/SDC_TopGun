/*
 * GPS3.hpp
 *
 *  Created on: Dec 4, 2015
 *      Author: Chitrang
 */

#ifndef L5_APPLICATION_GPS3_HPP_
#define L5_APPLICATION_GPS3_HPP_

#include "UART_dev.hpp"


void uart2init();
float get_decimal1(int, float);
int get_lat_degree1(void);
float get_lat_minute1(void);
int get_long_degree1(void);
float get_long_minute1(void);
float get_speed_GPS1(void);


#endif /* L5_APPLICATION_GPS3_HPP_ */
