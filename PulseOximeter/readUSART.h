#ifndef USART_H_
#define USART_H_
#include <stm32f4xx.h>

// USART configuration
#define USART_MODULE    USART3
#define USART_PORT      GPIOD
#define USART_TX_pin    8
#define USART_RX_pin    9
#define BAUDRATE        115200  // Change to 115200

// Function prototypes
void init_USART(void);
void send_usart(unsigned char d);
unsigned char receive_usart(void);
uint8_t is_usart_data_available(void);

#endif /* USART_H_ */