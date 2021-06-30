#ifndef __MOVEMENT_H_
#define __MOVEMENT_H_


#include "gd32f4xx.h"

typedef enum{
	  to_stop = 0,
    on_the_move,
    turning,
	  manual_control,
}TODO_TYPE;

typedef enum{
	  length_right = 0,
    width_right,
    length_left,
	  width_left,
}RUN_STEP;

typedef struct{
    TODO_TYPE todo_type;
    int16_t current_angle;
	  int16_t target_angle;
	  uint32_t encoder_pulse[4];
	  uint32_t encoder_pulse_old[4];
	  uint32_t range_length_mm;
	  uint32_t range_width_mm;
	  RUN_STEP run_step_flag;  //FALSE:run length    TRUE:run width
    _Bool bow_mode_switch;
}MOVE_STATE_T;

extern MOVE_STATE_T move_state;

void movement_system_init(void);
void movement_logic_handle(void);

#endif
