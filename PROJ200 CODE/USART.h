#ifndef _USART_H_
#define _USART_H_
#include <stm32f4xx.h>



#define clr_USART_pins() USART_PORT->BSRR&=~((3u<<(2*USART_TX_pin))|(3u<<(2*USART_RX_pin)));
#define USART_MODULE	USART3
#define USART_PORT		GPIOD
#define USART_TX_pin	8
#define USART_RX_pin	9
#define BAUDRATE		230400

void USART_SendData(char Data);
void USART_SendString (const char *text);
void init_USART(void);

#endif



