#include "readUSART.h"

void init_USART(void)
{
    unsigned char i1, i2;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;    // USART port clock enable
    
    USART_PORT->MODER &= ~(                 // Clear pin function bits
        (3u<<(2*USART_TX_pin))
        | (3u<<(2*USART_RX_pin))
    );
    USART_PORT->MODER |= (                  // Set pin function bits (alternate function)
        (2u<<(2*USART_TX_pin))
        | (2u<<(2*USART_RX_pin))
    );
    
    i1 = USART_TX_pin/8;                    // Used to select AFR[reg] for TX_pin
    i2 = USART_RX_pin>>3;                   // Same as above for RX_pin
    
    // Alternate function select bits
    USART_PORT->AFR[i1] &= ~(0x0f<<(4*(USART_TX_pin-(i1*8))));    // Clear pin function
    USART_PORT->AFR[i1] |= (0x07<<(4*(USART_TX_pin-(i1*8))));     // Set USART as alternate function for TX_pin
    USART_PORT->AFR[i2] &= ~(0x0f<<(4*(USART_RX_pin-(i2*8))));    // Clear pin function
    USART_PORT->AFR[i2] |= (0x07<<(4*(USART_RX_pin-(i2*8))));     // Set USART as alternate function for RX_pin
    
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;   // USART clock enable
    USART_MODULE->CR1 |= (                  // USART config
        USART_CR1_TE                        // Transmit enable
        | USART_CR1_RE                      // Receive enable
        | USART_CR1_UE                      // USART main enable bit
    );
                
    // Calculate BRR register value for baudrate
    uint32_t peripheral_clock = 45000000;   // 45MHz based on PLL config
    USART_MODULE->BRR = peripheral_clock / BAUDRATE;
}

void send_usart(unsigned char d)
{
    while(!(USART_MODULE->SR & USART_SR_TC));  // Wait until transmission complete
    USART_MODULE->DR = d;                      // Write byte to USART data register
}

unsigned char receive_usart(void)
{
    while(!(USART_MODULE->SR & USART_SR_RXNE)); // Wait until data is received
    return (unsigned char)USART_MODULE->DR;      // Return the received data
}

uint8_t is_usart_data_available(void)
{
    return (USART_MODULE->SR & USART_SR_RXNE) ? 1 : 0;  // Check if receive data register not empty
}