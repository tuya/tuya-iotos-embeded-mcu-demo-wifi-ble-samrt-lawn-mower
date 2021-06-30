#include "uart.h"

void uart_init(void)
{   
	  /* USART interrupt configuration */
    nvic_irq_enable(USART0_IRQn, 0, 0);
    nvic_irq_enable(USART1_IRQn, 1, 1);

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);
	
    /* connect port to USART0_Tx */
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_9);

    /* connect port to USART0_Rx */
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_10);
	
    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_9);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_10);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_10);

    /* USART configure */
    usart_deinit(USART0);
    usart_baudrate_set(USART0,115200U);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);
		
		/* enable USART clock */
    rcu_periph_clock_enable(RCU_USART1);
	
    /* connect port to USART0_Tx */
    gpio_af_set(GPIOD, GPIO_AF_7, GPIO_PIN_5);

    /* connect port to USART0_Rx */
    gpio_af_set(GPIOD, GPIO_AF_7, GPIO_PIN_6);
	
    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_5);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_5);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_6);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_6);

    /* USART configure */
    usart_deinit(USART1);
    usart_baudrate_set(USART1,9600U);
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
    usart_enable(USART1);
		
		/* enable USART0 receive interrupt */
    usart_interrupt_enable(USART0, USART_INT_RBNE);
	
	  /* enable USART1 receive interrupt */
    usart_interrupt_enable(USART1, USART_INT_RBNE);
}


void usart0_sendbyte(unsigned char data)
{
    usart_data_transmit(USART0, data);
	
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE)){
    }
}

void usart1_sendbyte(unsigned char data)
{
    usart_data_transmit(USART1, data);
	
    while(RESET == usart_flag_get(USART1, USART_FLAG_TBE)){
    }
}

