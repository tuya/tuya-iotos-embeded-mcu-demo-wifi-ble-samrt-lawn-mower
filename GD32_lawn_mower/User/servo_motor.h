#ifndef __SERVO_MOTOR_H_
#define __SERVO_MOTOR_H_



#include "gd32f4xx.h"

typedef enum{
    forward = 0,
    backward,
    right,
    left,	
	  stop,
}MOVING_DIRECTION;

typedef enum{
    low = 0,
    medium,
    high,
}BLADE_POSITION;

typedef enum{
	  init = 0,
    off,
    low_speed,
    medium_speed,
	  high_speed,
}BLADE_SPEED;

typedef struct{
    MOVING_DIRECTION direction;
	  BLADE_POSITION position;
	  BLADE_SPEED blade_speed;
}DEVICE_INFO;

extern DEVICE_INFO device_info;

void servo_motor_init(void);
void car_moving(MOVING_DIRECTION direction, uint16_t speed_value);
void car_half_turn(MOVING_DIRECTION direction, uint16_t speed_value);
void car_full_turn(MOVING_DIRECTION direction, uint16_t speed_value);
void single_motor_speed_set(uint8_t channel, uint16_t speed_value);
void blade_position_set(BLADE_POSITION value);
void blade_speed_set(BLADE_SPEED speed);
#endif

