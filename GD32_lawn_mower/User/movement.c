#include "movement.h"
#include "servo_motor.h"
#include "FreeRTOS.h"
#include "task.h"

#define PI 3.14159265358979323846264338327950288f

static int32_t width_remain_mm = 0;

int e[4]={0},e1[4]={0},e2[4]={0}; //pid 偏差
float uk[4]={0.0},uk1[4]={0.0},duk[4]={0.0};//pid输出
float Kp=0.8,Ki=0.3,Kd=0.9; //pid控制系数
int out[4] = {0};
uint16_t standard_speed = 200;
uint32_t pulse_test[4] = {0};
uint16_t forward_sampling_time_flag = 0;
uint16_t turning_sampling_time_flag = 0;

MOVE_STATE_T move_state = {
						.todo_type = to_stop,
						.current_angle = 0,
						.target_angle = 0,
	          .range_length_mm = 1000,
	          .range_width_mm = 1000,
	          .run_step_flag = 0,
            .bow_mode_switch = pdFALSE,
};

static MOVING_DIRECTION angle_correction(void);
static uint32_t pulse_to_distance(uint32_t pulse_num);
static void forward_correction(uint32_t *pulse_num, uint32_t *pulse_num_old, uint32_t standard_increment);
static void change_to_do(TODO_TYPE type);
static void todo_judge(void);

void movement_system_init(void)
{
    servo_motor_init();

	  rcu_periph_clock_enable(RCU_SYSCFG);
	
	  gpio_mode_set(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_0);
	  nvic_irq_enable(EXTI0_IRQn, 2U, 0U);
    syscfg_exti_line_config(EXTI_SOURCE_GPIOC, EXTI_SOURCE_PIN0);
	  exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_RISING);
    exti_interrupt_flag_clear(EXTI_0);
	
	  gpio_mode_set(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_1);
	  nvic_irq_enable(EXTI1_IRQn, 2U, 0U);
    syscfg_exti_line_config(EXTI_SOURCE_GPIOC, EXTI_SOURCE_PIN1);
	  exti_init(EXTI_1, EXTI_INTERRUPT, EXTI_TRIG_RISING);
    exti_interrupt_flag_clear(EXTI_1);

	  gpio_mode_set(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_2);
	  nvic_irq_enable(EXTI2_IRQn, 2U, 0U);
    syscfg_exti_line_config(EXTI_SOURCE_GPIOC, EXTI_SOURCE_PIN2);
	  exti_init(EXTI_2, EXTI_INTERRUPT, EXTI_TRIG_RISING);
    exti_interrupt_flag_clear(EXTI_2);
		
	  gpio_mode_set(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_3);
	  nvic_irq_enable(EXTI3_IRQn, 2U, 0U);
    syscfg_exti_line_config(EXTI_SOURCE_GPIOC, EXTI_SOURCE_PIN3);
	  exti_init(EXTI_3, EXTI_INTERRUPT, EXTI_TRIG_RISING);
    exti_interrupt_flag_clear(EXTI_3);

}

void movement_logic_handle(void)
{   
    MOVE_STATE_T *p = NULL;
	  p = &move_state;
	  uint8_t i = 0;
	  MOVING_DIRECTION turning_state;
    switch(p->todo_type) {
		
		case to_stop: 
			
				car_moving(stop,0);
        blade_speed_set(init);
		    if(p->bow_mode_switch){
					
					  blade_speed_set(device_info.blade_speed);
					  change_to_do(on_the_move);
					  car_moving(forward,200);
					
					  p->target_angle = p->current_angle;
					  p->run_step_flag = length_right;

					  width_remain_mm = p->range_width_mm;
						return;
				}
				
        break;		
		case on_the_move:
			  
		    if(forward_sampling_time_flag == 20) { //20*20ms = 400ms
						for(i = 0;i < 4;i++) {
								pulse_test[i] = p->encoder_pulse[i]-p->encoder_pulse_old[i];							
						}

						forward_correction(p->encoder_pulse,p->encoder_pulse_old,390);
						 
						for(i = 0;i < 4;i++) {
								p->encoder_pulse_old[i] = p->encoder_pulse[i];							
						}				
				    forward_sampling_time_flag = 0;
				}
        forward_sampling_time_flag++;
        
        todo_judge();
					
        break;	
		case turning: 
			
			  turning_state = angle_correction();

        if(turning_state == right) {
			      car_full_turn(right,150);
					  turning_sampling_time_flag = 0;
        }else if(turning_state == left) {
				    car_full_turn(left,150);
					  turning_sampling_time_flag = 0;
				}else if(turning_state == forward) {
				    car_moving(stop,0);
					  turning_sampling_time_flag++;
				}        				 
				
				if(turning_sampling_time_flag >= 25) {
						p->todo_type = on_the_move;
					  car_moving(forward,200);
						forward_sampling_time_flag = 0;
						turning_sampling_time_flag = 0;
						for(i = 0;i < 4;i++) {
						    p->encoder_pulse[i] = 0;
                p->encoder_pulse_old[i] = 0;							
						}
				}

        break;	
		
		case manual_control:
		
        break;
		
		default:
				break;		
		}
	
}

static void change_to_do(TODO_TYPE type)
{
    MOVE_STATE_T *p = NULL;
	  p = &move_state;
	  uint8_t i =0;
	
    p->todo_type = type;
		forward_sampling_time_flag = 0;
		turning_sampling_time_flag = 0;
	
		for(i = 0;i < 4;i++) {
			  p->encoder_pulse[i] = 0;
			  p->encoder_pulse_old[i] = 0;							
	  }
	  
}


static MOVING_DIRECTION angle_correction(void)
{
    int16_t target,current = 0;
	  target = move_state.target_angle;
	  current = move_state.current_angle;
    if(target < current) {
			  if(current - target <= 20) {
					  return forward;
				}
		    if(current - target <= 1800) {
				    return left;
				}else{
				    return right;
				}
		}else if(target > current) {
				if(target - current <= 20) {
				    return forward;
				}
		    if(target - current <= 1800) {
				    return right;
				}else{
				    return left;
				}
		}else if(current == target) {
		    return forward;
		}
}

static void todo_judge(void)
{
    MOVE_STATE_T *p = NULL;
	  p = &move_state;
	  
    switch(p->run_step_flag) {
			
		case length_right:
			  if(pulse_to_distance(p->encoder_pulse[0]) >= (p->range_length_mm - 10)) {
						if((p->current_angle + 900) > 3600) {
								p->target_angle = p->current_angle + 900 - 3600;
						}else{
								p->target_angle = p->current_angle + 900;
						}

						change_to_do(turning);
						p->run_step_flag = width_right;

				}
		    break;
		case width_right:
			  if(pulse_to_distance(p->encoder_pulse[0]) >= 100) {
						if((p->current_angle + 900) > 3600) {
								p->target_angle = p->current_angle + 900 - 3600;
						}else{
								p->target_angle = p->current_angle + 900;
						}
						change_to_do(turning);
						p->run_step_flag = length_left;
						width_remain_mm -= 100;

				}
		    break;
		case length_left:
			  if(pulse_to_distance(p->encoder_pulse[0]) >= (p->range_length_mm - 10)) {
						if(p->current_angle < 900) {
								p->target_angle = 3600 - (900 - p->current_angle);
						}else{
								p->target_angle = p->current_angle - 900;
						}
						change_to_do(turning);
						p->run_step_flag = width_left;

				}
		    break;
		case width_left:
			  if(pulse_to_distance(p->encoder_pulse[0]) >= 100) {
						if(p->current_angle < 900) {
								p->target_angle = 3600 - (900 - p->current_angle);
						}else{
								p->target_angle = p->current_angle - 900;
						}
						change_to_do(turning);
						p->run_step_flag = length_right;
						width_remain_mm -= 100;
						
				}
		    break;
		default:
				break;
		}
		
		if(width_remain_mm <= 0) {
        change_to_do(to_stop);
			  move_state.bow_mode_switch = pdFALSE;
		}

}

static void forward_correction(uint32_t *pulse_num, uint32_t *pulse_num_old, uint32_t standard_increment)
{
    uint8_t i = 0;
	    
	  for(i = 0;i < 4;i++) {
		    e[i] = standard_increment - (pulse_num[i] - pulse_num_old[i]);
			  duk[i] = (Kp*(e[i]-e1[i])+Ki*e[i])/100;
			  uk[i] = uk1[i] + duk[i];
			  out[i] = (int)uk[i];
				uk1[i] = uk[i];
				e2[i] = e1[i];
				e1[i] = e[i];
		    single_motor_speed_set(i, (uint16_t)(200+(out[i]*5)));
		}
    return;
}

static uint32_t pulse_to_distance(uint32_t pulse_num)
{
    uint32_t distance = 0;
    distance = (uint32_t)((pulse_num/800)*(PI*65));
    return distance;
}