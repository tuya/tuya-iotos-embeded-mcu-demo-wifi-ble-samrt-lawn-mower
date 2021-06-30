#ifndef __UART_H_
#define __UART_H_

#include "gd32f4xx.h"


void uart_init(void);
void usart0_sendbyte(unsigned char data);
void usart1_sendbyte(unsigned char data);

#endif

