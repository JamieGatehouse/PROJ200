#ifndef READUSART_H_
#define READUSART_H_

#include <stm32f4xx.h>

// Function prototypes
void init_USART3(void);  // Initialize USART3
void send_usart(uint8_t data);  // Send a byte via USART
uint8_t receive_usart(void);  // Receive a byte via USART
uint8_t is_usart_data_available(void);  // Check if data is available in USART

#endif /* READUSART_H_ */
