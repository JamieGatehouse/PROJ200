#include "DELAY.h"
#include "USART.h"

//USART.h containing declarations and #defines with suitable comments describing them	1		
//USART.c containing structured routines and comments describing the algorithm (>50% of code commented)	1		
//Set Baud 230,400, 8 data, 1stop [1] Even parity [1]	1		
//Receive chars – Display debug data	1		
//Transmit Chars to select mode (Heartrate/Oxygen Level/Temp/Humidity/Movement)	2		
//Use ANSI ESC sequences to control cursor location, colour and other options	2		
//Display graphics for voltages	2		


void USART_SendData(char Data) { // send char
   while(!(USART3->SR & USART_SR_TC)); //wait for transmission complete
   USART3->DR = (Data & 0x01FF); //write byte to usart data register
}


void USART_SendString (const char *text) { // send string
   unsigned int idx = 0;
     
	while (text[idx]) { // end of string is a null = 0
   USART_SendData(text[idx++]);
   
	}
}




	




void init_USART(void)
{
	unsigned char i1,i2;
		RCC->AHB1ENR|=RCC_AHB1ENR_GPIODEN;		//usart port clock enable
	
	clr_USART_pins();
	USART_PORT->MODER|=(										//reset pin function bits (alternate function)
		(2u<<(2*USART_TX_pin))
		|(2u<<(2*USART_RX_pin))
			);
	
	i1=USART_TX_pin/8;											// used to select AFR[reg] for TX_pin ... pins 0-7 AFR[0] pins 8-15 AFR[1]
	i2=USART_RX_pin>>3;											// same as above for RX_pin (uses shift to divide)

		// ALTERNATE FUNCTION SELECT BITS
	USART_PORT->AFR[i1]&=~(0x0f<<(4*(USART_TX_pin-(i1*8))));    //clear pin function
	USART_PORT->AFR[i1]|=(0x07<<(4*(USART_TX_pin-(i1*8))));			// set usart as alternate function for TX_pin
	USART_PORT->AFR[i2]&=~(0x0f<<(4*(USART_RX_pin-(i2*8))));		// clear pin function
	USART_PORT->AFR[i2]|=(0x07<<(4*(USART_RX_pin-(i2*8))));			//set usart as alternate function for RX_pin
	
 // Enable USART clock
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN; // USART clock enable

    // Configure USART baud rate for 230400
    uint32_t SystemClock = 180000000; // System clock frequency (180 MHz)
    uint32_t USARTDIV = SystemClock / (16 * BAUDRATE); // Calculate USARTDIV
    USART_MODULE->BRR = USARTDIV; // Set BRR register

    // Configure USART for 8 data bits, no parity, 1 stop bit
    USART_MODULE->CR1 &= ~USART_CR1_M; // 8 data bits (clear M bit)
		USART_MODULE->CR1 &= ~USART_CR1_PS;  //EVEN parity
    USART_MODULE->CR2 &= ~USART_CR2_STOP; // 1 stop bit (clear STOP bits)

    // Enable USART
    USART_MODULE->CR1 |= (
        USART_CR1_TE // Transmit enable
        | USART_CR1_RE // Receive enable
        | USART_CR1_UE // USART main enable bit
    );
																									
}





