/*
 * @file name: servo_motor.c
 * @Descripttion: 
 * @Author: zgw
 * @email: liang.zhang@tuya.com
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-04-19 15:10:56
 * @LastEditors: zgw
 * @LastEditTime: 2021-04-19 17:55:21
 */

#include "servo_motor.h"
#include "uart.h"

#define MOTOR_PORT_1                        GPIOA
#define MOTOR_PIN_1                         GPIO_PIN_15
#define MOTOR_PORT_1_P                      GPIOD
#define MOTOR_PIN_1_P                       GPIO_PIN_0
#define MOTOR_PORT_1_N                      GPIOD
#define MOTOR_PIN_1_N                       GPIO_PIN_7

#define MOTOR_PORT_2                        GPIOB
#define MOTOR_PIN_2                         GPIO_PIN_9
#define MOTOR_PORT_2_P                      GPIOD
#define MOTOR_PIN_2_P                       GPIO_PIN_1
#define MOTOR_PORT_2_N                      GPIOD
#define MOTOR_PIN_2_N                       GPIO_PIN_2


#define MOTOR_PORT_3                        GPIOB
#define MOTOR_PIN_3                         GPIO_PIN_10
#define MOTOR_PORT_3_P                      GPIOD
#define MOTOR_PIN_3_P                       GPIO_PIN_3
#define MOTOR_PORT_3_N                      GPIOD
#define MOTOR_PIN_3_N                       GPIO_PIN_4


#define MOTOR_PORT_4                        GPIOB
#define MOTOR_PIN_4                         GPIO_PIN_11
#define MOTOR_PORT_4_P                      GPIOD
#define MOTOR_PIN_4_P                       GPIO_PIN_8
#define MOTOR_PORT_4_N                      GPIOD
#define MOTOR_PIN_4_N                       GPIO_PIN_9 

#define MOTOR_CHANNEL_1                     TIMER_CH_0
#define MOTOR_CHANNEL_2                     TIMER_CH_1
#define MOTOR_CHANNEL_3                     TIMER_CH_2
#define MOTOR_CHANNEL_4                     TIMER_CH_3

#define SERVO_PORT_1                        GPIOC
#define SERVO_PIN_1                         GPIO_PIN_6

#define SERVO_PORT_2                        GPIOC
#define SERVO_PIN_2                         GPIO_PIN_7

#define BLADE_MOTOR_PORT                    GPIOC
#define BLADE_MOTOR_PIN                     GPIO_PIN_8

DEVICE_INFO device_info = {
						.direction = stop,
						.position = high,
						.blade_speed = medium_speed,
};

void timer_config(void);

/**
    \brief      init pwm
    \param[in]  none
    \param[out] none
    \retval     none
  */
void servo_motor_init(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
	
		/*Configure PD1~8 Output motor Positive and Negative pin to drive wheels_1~4*/
	  gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, MOTOR_PIN_1_P | MOTOR_PIN_1_N | MOTOR_PIN_2_P | MOTOR_PIN_2_N | MOTOR_PIN_3_P | MOTOR_PIN_3_N | MOTOR_PIN_4_P | MOTOR_PIN_4_N);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,MOTOR_PIN_1_P | MOTOR_PIN_1_N | MOTOR_PIN_2_P | MOTOR_PIN_2_N | MOTOR_PIN_3_P | MOTOR_PIN_3_N | MOTOR_PIN_4_P | MOTOR_PIN_4_N);
    gpio_bit_reset(GPIOD, MOTOR_PIN_1_P | MOTOR_PIN_1_N | MOTOR_PIN_2_P | MOTOR_PIN_2_N | MOTOR_PIN_3_P | MOTOR_PIN_3_N | MOTOR_PIN_4_P | MOTOR_PIN_4_N);
	
    /*Configure PA15(TIMER1_CH0) Output PWM pulse to drive wheels_1*/
    gpio_mode_set(MOTOR_PORT_1, GPIO_MODE_AF, GPIO_PUPD_NONE, MOTOR_PIN_1);
    gpio_output_options_set(MOTOR_PORT_1, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,MOTOR_PIN_1);
    gpio_af_set(MOTOR_PORT_1, GPIO_AF_1, MOTOR_PIN_1);

    /*Configure PB9(TIMER1_CH1) Output PWM pulse to drive wheels_2*/
    gpio_mode_set(MOTOR_PORT_2, GPIO_MODE_AF, GPIO_PUPD_NONE, MOTOR_PIN_2);
    gpio_output_options_set(MOTOR_PORT_2, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,MOTOR_PIN_2);
    gpio_af_set(MOTOR_PORT_2, GPIO_AF_1, MOTOR_PIN_2);

    /*Configure PB10(TIMER1_CH2) Output PWM pulse to drive wheels_3*/
    gpio_mode_set(MOTOR_PORT_3, GPIO_MODE_AF, GPIO_PUPD_NONE, MOTOR_PIN_3);
    gpio_output_options_set(MOTOR_PORT_3, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,MOTOR_PIN_3);
    gpio_af_set(MOTOR_PORT_3, GPIO_AF_1, MOTOR_PIN_3);

    /*Configure PB11(TIMER1_CH3) Output PWM pulse to drive wheels_4*/
    gpio_mode_set(MOTOR_PORT_4, GPIO_MODE_AF, GPIO_PUPD_NONE, MOTOR_PIN_4);
    gpio_output_options_set(MOTOR_PORT_4, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,MOTOR_PIN_4);
    gpio_af_set(MOTOR_PORT_4, GPIO_AF_1, MOTOR_PIN_4);

    /*Configure PC6(TIMER2_CH0) Output PWM pulse to drive servo no.1*/
    gpio_mode_set(SERVO_PORT_1, GPIO_MODE_AF, GPIO_PUPD_NONE, SERVO_PIN_1);
    gpio_output_options_set(SERVO_PORT_1, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,SERVO_PIN_1);
    gpio_af_set(SERVO_PORT_1, GPIO_AF_2, SERVO_PIN_1);
		
		/*Configure PC7(TIMER2_CH1) Output PWM pulse to drive servo no.2*/
    gpio_mode_set(SERVO_PORT_2, GPIO_MODE_AF, GPIO_PUPD_NONE, SERVO_PIN_2);
    gpio_output_options_set(SERVO_PORT_2, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,SERVO_PIN_2);
    gpio_af_set(SERVO_PORT_2, GPIO_AF_2, SERVO_PIN_2);
		
		/*Configure PC8(TIMER2_CH2) Output PWM pulse to drive blade motor*/
    gpio_mode_set(BLADE_MOTOR_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, BLADE_MOTOR_PIN);
    gpio_output_options_set(BLADE_MOTOR_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,BLADE_MOTOR_PIN);
    gpio_af_set(BLADE_MOTOR_PORT, GPIO_AF_2, BLADE_MOTOR_PIN);
		
    timer_config();


}


void timer_config(void)
{
    uint16_t i = 0;
	
    /* TIMER1 configuration: generate PWM signals with different duty cycles:
       TIMER1CLK = SystemCoreClock / 120 = 1MHz */
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER1);
    rcu_periph_clock_enable(RCU_TIMER2);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    timer_struct_para_init(&timer_initpara);
    timer_deinit(TIMER1);
    timer_deinit(TIMER2);

    /* TIMER1 configuration */
    timer_initpara.prescaler         = 119;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 500; /* 500*(1/1MHZ) = 500us */
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER1,&timer_initpara);

    /* TIMER2 configuration */
    timer_initpara.prescaler         = 119;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 20000; /* 20000*(1/1MHZ) = 20ms */
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER2,&timer_initpara);

    timer_channel_output_struct_para_init(&timer_ocintpara);
    timer_ocintpara.ocpolarity  = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    for(i = 0; i < 4; i++) {
			
        timer_channel_output_config(TIMER1,i,&timer_ocintpara);
        timer_channel_output_pulse_value_config(TIMER1,i,0);
        timer_channel_output_mode_config(TIMER1,i,TIMER_OC_MODE_PWM0);
        timer_channel_output_shadow_config(TIMER1,i,TIMER_OC_SHADOW_DISABLE);
    }

		for(i = 0; i < 3; i++) {
			
        timer_channel_output_config(TIMER2,i,&timer_ocintpara);
        timer_channel_output_pulse_value_config(TIMER2,i,0);
        timer_channel_output_mode_config(TIMER2,i,TIMER_OC_MODE_PWM0);
        timer_channel_output_shadow_config(TIMER2,i,TIMER_OC_SHADOW_DISABLE);
    }

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER1);
    timer_auto_reload_shadow_enable(TIMER2);

    /* TIMER enable */
    timer_enable(TIMER1);
    timer_enable(TIMER2);
}

void car_moving(MOVING_DIRECTION direction, uint16_t speed_value)
{
    uint8_t i;
    switch(direction) {
			
		case forward:
			
			  gpio_bit_set(GPIOD, MOTOR_PIN_1_P | MOTOR_PIN_2_P | MOTOR_PIN_3_P | MOTOR_PIN_4_P);
		    gpio_bit_reset(GPIOD, MOTOR_PIN_1_N | MOTOR_PIN_2_N | MOTOR_PIN_3_N | MOTOR_PIN_4_N);
		
		    for(i = 0; i < 4; i++) {
					timer_channel_output_pulse_value_config(TIMER1,i,speed_value);
        }
				
				break;
		case right:
			
				gpio_bit_set(GPIOD, MOTOR_PIN_1_P | MOTOR_PIN_2_P | MOTOR_PIN_3_P | MOTOR_PIN_4_P);
				gpio_bit_reset(GPIOD, MOTOR_PIN_1_N | MOTOR_PIN_2_N | MOTOR_PIN_3_N | MOTOR_PIN_4_N);		
		
		    timer_channel_output_pulse_value_config(TIMER1,MOTOR_CHANNEL_1,speed_value + 60);
		    timer_channel_output_pulse_value_config(TIMER1,MOTOR_CHANNEL_3,speed_value + 60);
				
		    timer_channel_output_pulse_value_config(TIMER1,MOTOR_CHANNEL_2,speed_value);
		    timer_channel_output_pulse_value_config(TIMER1,MOTOR_CHANNEL_4,speed_value);
		
				break;
		case left:
			
				gpio_bit_set(GPIOD, MOTOR_PIN_1_P | MOTOR_PIN_2_P | MOTOR_PIN_3_P | MOTOR_PIN_4_P);
				gpio_bit_reset(GPIOD, MOTOR_PIN_1_N | MOTOR_PIN_2_N | MOTOR_PIN_3_N | MOTOR_PIN_4_N);	

				timer_channel_output_pulse_value_config(TIMER1,MOTOR_CHANNEL_1,speed_value);
		    timer_channel_output_pulse_value_config(TIMER1,MOTOR_CHANNEL_3,speed_value);
		
        timer_channel_output_pulse_value_config(TIMER1,MOTOR_CHANNEL_2,speed_value + 50);
		    timer_channel_output_pulse_value_config(TIMER1,MOTOR_CHANNEL_4,speed_value + 50);
		
		    break;
		case backward:
			
				gpio_bit_reset(GPIOD, MOTOR_PIN_1_P | MOTOR_PIN_2_P | MOTOR_PIN_3_P | MOTOR_PIN_4_P);
		    gpio_bit_set(GPIOD, MOTOR_PIN_1_N | MOTOR_PIN_2_N | MOTOR_PIN_3_N | MOTOR_PIN_4_N);
		
		    for(i = 0; i < 4; i++) {
					timer_channel_output_pulse_value_config(TIMER1,i,speed_value);
        }
				
				break;
		case stop:
			
			  gpio_bit_reset(GPIOD, MOTOR_PIN_1_P | MOTOR_PIN_2_P | MOTOR_PIN_3_P | MOTOR_PIN_4_P);
			  gpio_bit_reset(GPIOD, MOTOR_PIN_1_N | MOTOR_PIN_2_N | MOTOR_PIN_3_N | MOTOR_PIN_4_N);
		
				for(i = 0; i < 4; i++) {
					timer_channel_output_pulse_value_config(TIMER1,i,speed_value);
        }
				
			  break;
		default:
				break;
		}
}

void car_half_turn(MOVING_DIRECTION direction, uint16_t speed_value)
{
		gpio_bit_set(GPIOD, MOTOR_PIN_1_P | MOTOR_PIN_2_P | MOTOR_PIN_3_P | MOTOR_PIN_4_P);
		gpio_bit_reset(GPIOD, MOTOR_PIN_1_N | MOTOR_PIN_2_N | MOTOR_PIN_3_N | MOTOR_PIN_4_N);
	
    switch(direction) {
			
		case right:  
				
		    timer_channel_output_pulse_value_config(TIMER1,MOTOR_CHANNEL_1,speed_value + 100);
		    timer_channel_output_pulse_value_config(TIMER1,MOTOR_CHANNEL_3,speed_value + 100);
				
		    timer_channel_output_pulse_value_config(TIMER1,MOTOR_CHANNEL_2,speed_value);
		    timer_channel_output_pulse_value_config(TIMER1,MOTOR_CHANNEL_4,speed_value);
				break;
		case left:
			
				timer_channel_output_pulse_value_config(TIMER1,MOTOR_CHANNEL_1,speed_value);
		    timer_channel_output_pulse_value_config(TIMER1,MOTOR_CHANNEL_3,speed_value);
		
        timer_channel_output_pulse_value_config(TIMER1,MOTOR_CHANNEL_2,speed_value + 100);
		    timer_channel_output_pulse_value_config(TIMER1,MOTOR_CHANNEL_4,speed_value + 100);
		
				break;
		default:
				break;
		}
}

void car_full_turn(MOVING_DIRECTION direction, uint16_t speed_value)
{
    uint8_t i;
    switch(direction) {
			
		case right:  
			
			  gpio_bit_set(GPIOD, MOTOR_PIN_1_P | MOTOR_PIN_3_P);
		    gpio_bit_reset(GPIOD, MOTOR_PIN_1_N | MOTOR_PIN_3_N);
		
			  gpio_bit_reset(GPIOD, MOTOR_PIN_2_P | MOTOR_PIN_4_P);
		    gpio_bit_set(GPIOD, MOTOR_PIN_2_N | MOTOR_PIN_4_N);
				
		    for(i = 0; i < 4; i++) {
					timer_channel_output_pulse_value_config(TIMER1,i,speed_value);
        }
				
				break;
		case left:
			
			  gpio_bit_set(GPIOD, MOTOR_PIN_2_P | MOTOR_PIN_4_P);
		    gpio_bit_reset(GPIOD, MOTOR_PIN_2_N | MOTOR_PIN_4_N);
		
			  gpio_bit_reset(GPIOD, MOTOR_PIN_1_P | MOTOR_PIN_3_P);
		    gpio_bit_set(GPIOD, MOTOR_PIN_1_N | MOTOR_PIN_3_N);
				
		    for(i = 0; i < 4; i++) {
					timer_channel_output_pulse_value_config(TIMER1,i,speed_value);
        }
		
				break;
		default:
				break;
		}
}

void single_motor_speed_set(uint8_t channel, uint16_t speed_value)
{
	
    switch(channel) {
			
		case 0:  
					
		    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_0,speed_value+20);

				break;
		
		case 1:
			
		    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_1,speed_value);	
		
				break;
		
		case 2:
			
		    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_2,speed_value+20);
		
				break;
		
		case 3:
	
		    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_3,speed_value);
		
		    break;	
		
		default:
				break;			
		}
}

void blade_position_set(BLADE_POSITION value)
{
    switch(value) {
			
		case low:  
					
		    timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_0,3100);
        timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_1,3000);			
				break;
		
		case medium:
			
		    timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_0,2800);
        timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_1,2700);	
				break;
		
		case high:
			
			  timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_0,2600);
			  timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_1,2500);
				break;
		
		default:
				break;			
		}
}

void blade_speed_set(BLADE_SPEED speed)
{
    switch(speed) {
		
		case init:
			
			  timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_2,1500);					
			  break;
			
		case off:
			
        timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_2,0);
			  break;
		
		case low_speed:  
					
			  timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_2,1800);	
				break;
		
		case medium_speed:
			
			  timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_2,1900);		
				break;
		
		case high_speed:
			
			  timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_2,2000);	
				break;
		
		default:
				break;			
		}
}