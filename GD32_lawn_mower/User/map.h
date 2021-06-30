#ifndef __MAP_H_
#define __MAP_H_


#include "gd32f4xx.h"

typedef struct {
    uint8_t lat_dddmm[12];
    uint8_t lng_ddmm[12]; 
    uint8_t lat_dd[12];
    uint8_t lng_dd[12]; 	
}MAP_POINT_t;

typedef enum {
    point_1 = 0,
    point_2,
    point_3,
    point_4,
}point_number_t;

uint8_t gps_data_pick(point_number_t point_num, uint8_t *data, uint8_t data_len);

#endif
