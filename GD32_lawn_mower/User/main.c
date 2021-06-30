#include "gd32f4xx.h"
#include "systick.h"
#include "wifi.h"
#include "servo_motor.h"
#include "movement.h"
#include "uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "map.h"
#include "QMC5883L.h"
#include "battery_detection.h"

extern MAP_POINT_t map_point[4];
/*-----------------------------------------------------------*/
TaskHandle_t StartTask_Handler;
void start_task(void *pvParameters);

TaskHandle_t WifiUartTask_Handler;
void wifi_uart_task(void *pvParameters);
 
TaskHandle_t GpsDateTask_Handler;
void gps_data_task(void *pvParameters);

TaskHandle_t MoveControlTask_Handler;
void move_control_task(void *pvParameters);

uint16_t adctest = 0;
/*-----------------------------------------------------------*/
typedef struct {
    short x;
		short y;
		short z;
}QMC_DATA_T;
QMC_DATA_T qmc_data = {0};
/*-----------------------------------------------------------*/
#define USART_FH_len 6	//帧头长度
char USART_FH[USART_FH_len]={'$','G','N','G','G','A'}; //帧头
#define USART_FT_len 2	//帧尾长度
uint8_t USART_FT[USART_FT_len]={0X0D,0X0A}; //帧尾

uint8_t data_buf[128] = {0};
uint8_t rx_buf[128] = {0};
uint8_t buff_len = 0;


/*-----------------------------------------------------------*/
void system_init(void);


/*-----------------------------------------------------------*/
static void log_write_data(unsigned char *in, unsigned short len)
{
	
    while(len --) {
        usart1_sendbyte(*in);
        in ++;
    }
}


int main(void){
	
	  system_init();
	
    wifi_protocol_init();

    xTaskCreate((TaskFunction_t )start_task,         
				(const char*    )"start_task",        
				(uint16_t       )512,     
				(void*          )NULL,              
				(UBaseType_t    )1,  
				(TaskHandle_t*  )&StartTask_Handler); 
		
  	vTaskStartScheduler(); 
}

void system_init(void)
{

    rcu_periph_clock_enable(RCU_GPIOG);
    /* configure LED3 GPIO port */ 
    gpio_mode_set(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
    /* reset LED3 GPIO pin */
    gpio_bit_reset(GPIOG, GPIO_PIN_3);
	
    /* configure systick */
    systick_config();
	
	  /*init motor and steering gear*/	
    movement_system_init();
	
    /* configure uart */
    uart_init();

}



void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();         

    xTaskCreate((TaskFunction_t )wifi_uart_task,             
                (const char*    )"wifi_uart_task",           
                (uint16_t       )512,        
                (void*          )NULL,                  
                (UBaseType_t    )3,        
                (TaskHandle_t*  )&WifiUartTask_Handler);   

    xTaskCreate((TaskFunction_t )gps_data_task,     
                (const char*    )"gps_data_task",   
                (uint16_t       )512,
                (void*          )NULL,
                (UBaseType_t    )3,
                (TaskHandle_t*  )&GpsDateTask_Handler); 
								
		xTaskCreate((TaskFunction_t )move_control_task,     
                (const char*    )"move_control_task",   
                (uint16_t       )512*2,
                (void*          )NULL,
                (UBaseType_t    )2,
                (TaskHandle_t*  )&MoveControlTask_Handler); 	
								
    vTaskDelete(StartTask_Handler); 
				
    taskEXIT_CRITICAL();         
}


void wifi_uart_task(void *pvParameters)
{   
	
    while(1) {
        vTaskDelay(500);
        wifi_uart_service();
    }
}


void gps_data_task(void *pvParameters)
{
    MAP_POINT_t *p;
	  p = &map_point[point_1];
    uint8_t data_len = 0;
    while(1) {
        vTaskDelay(100);
			
        data_len = strlen((char*)data_buf);
        if(data_len != 0){
			      gps_data_pick(point_1, data_buf, data_len);
				    memset(data_buf,0,data_len);
		    }
    }
}

void move_control_task(void *pvParameters)
{   
    uint8_t data_buff[100] = {0};
    float_xyz Mag_angle;
		i16_xyz test;
		uint8_t test_flag = 50;
	  vTaskDelay(200);
	  QMC_Init();
		blade_speed_set(init);
		vTaskDelay(500);
		QMC5883L_GetAngle(&Mag_angle);
		move_state.current_angle = (int16_t)Mag_angle.x;
		vTaskDelay(500);

    while(1) {	
        
			  if(test_flag){
					  vTaskDelay(20);
					  test_flag--;
				}else if(test_flag == 0) {
						vTaskDelay(20);
			      movement_logic_handle();
				}
						QMC5883L_GetAngle(&Mag_angle);
            move_state.current_angle = (int16_t)Mag_angle.x;
			
		}

}


void USART1_IRQHandler(void)
{
    if((RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE)) && 
       (RESET != usart_flag_get(USART1, USART_FLAG_RBNE))){	
			 
        rx_buf[buff_len++] = (uint8_t)usart_data_receive(USART1);

				if(rx_buf[0] != USART_FH[0]) {
						rx_buf[0] = 0;
						buff_len = 0;
				}

				if(buff_len >= USART_FH_len) {

						if(strncmp((char*)rx_buf,USART_FH,USART_FH_len) == 0) {
								if(strncmp(&rx_buf[buff_len-2],(char*)USART_FT,USART_FT_len) == 0) {
										memcpy(data_buf,rx_buf,buff_len);
										memset(rx_buf,0,buff_len);
										buff_len = 0;
								}
						}else {
								memset(rx_buf,0,USART_FH_len);
								buff_len = 0;
						}
				}
    }
}

