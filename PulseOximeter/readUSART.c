#include "readUSART.h"

#define BAUDRATE 230400

void init_USART3(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;  // Enable GPIO port D clock
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN; // Enable USART3 clock

    // Configure USART3 pins (PD8 -> TX, PD9 -> RX)
    GPIOD->MODER &= ~(0x03 << (2 * 8));  // Clear PD8 (TX) mode
    GPIOD->MODER |= (0x02 << (2 * 8));   // Set PD8 (TX) to alternate function
    GPIOD->MODER &= ~(0x03 << (2 * 9));  // Clear PD9 (RX) mode
    GPIOD->MODER |= (0x02 << (2 * 9));   // Set PD9 (RX) to alternate function

    // Set USART3 alternate function (AF7)
    GPIOD->AFR[1] |= (0x07 << (4 * (8 - 8)));  // Set AF7 (USART3) for PD8 (TX)
    GPIOD->AFR[1] |= (0x07 << (4 * (9 - 8)));  // Set AF7 (USART3) for PD9 (RX)

    // Configure USART3 parameters
    USART3->BRR = SystemCoreClock / BAUDRATE;  // Set baud rate (based on PLL)
    USART3->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; // Enable TX, RX, and USART

    // Enable USART3 interrupt (optional, if you need interrupt-based reception)
    USART3->CR1 |= USART_CR1_RXNEIE; // Enable RXNE interrupt for USART3
}

void send_usart(uint8_t data)
{
    while (!(USART3->SR & USART_SR_TXE));  // Wait for TX buffer to be empty
    USART3->DR = data;  // Send the data byte
}

uint8_t receive_usart(void)
{
    while (!(USART3->SR & USART_SR_RXNE));  // Wait until data is received
    return (uint8_t)USART3->DR;  // Return received data
}

uint8_t is_usart_data_available(void)
{
    return (USART3->SR & USART_SR_RXNE) != 0;  // Check if RXNE flag is set
}
