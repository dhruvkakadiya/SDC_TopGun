/*
 * GPS.hpp
 *
 *  Created on: Oct 25, 2015
 *      Author: Chitrang
 */

#ifndef GPS_HPP_
#define GPS_HPP_

#include "scheduler_task.hpp"
#include "uart2.hpp"
#include "can_msg_id.h"

extern geo_location gps_data_dec;
extern uint8_t speed_gps;
extern Uart2 &uart2_ref;

bool gps_init(void);
bool gps_data(void);
int get_long_degree(void);
double get_long_minute(void);
int get_lat_degree(void);
double get_lat_minute(void);
float get_decimal(int deg, float minute);
float get_speed_GPS(void);


#endif /* GPS_HPP_ */
