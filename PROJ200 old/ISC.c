#include "ISC.h"
#include "DELAY.h"
//I2C.h containing declarations and #defines with suitable comments describing them	1		
//I2C.c containing structured routines and comments describing the algorithm (>50% of code commented)	1		
//Initialisation setup GPIO / Registers for I2C	2		
//Read IMU 3DOF  / identify movement	2		
//Use Interrupts to manage priority	2		
//Capture waveform showing SPI transfer for known data and label	2		


void I2C_Config (void)
{

	
	// Enable the I2C CLOCK and GPIO CLOCK
	RCC->APB1ENR |= (1<<21);  // enable I2C CLOCK
	RCC->AHB1ENR |= (1<<1);  // Enable GPIOB CLOCK
	
	
	// Configure the I2C PINs for ALternate Functions
	GPIOB->MODER |= (2<<16) | (2<<18);  // Bits (17:16)= 1:0 --> Alternate Function for Pin PB8; Bits (19:18)= 1:0 --> Alternate Function for Pin PB9
	GPIOB->OTYPER |= (1<<8) | (1<<9);  //  Bit8=1, Bit9=1  output open drain
	GPIOB->OSPEEDR |= (3<<16) | (3<<18);  // Bits (17:16)= 1:1 --> High Speed for PIN PB8; Bits (19:18)= 1:1 --> High Speed for PIN PB9
	GPIOB->PUPDR |= (1<<16) | (1<<18);  // Bits (17:16)= 0:1 --> Pull up for PIN PB8; Bits (19:18)= 0:1 --> pull up for PIN PB9
	GPIOB->AFR[1] |= (4<<0) | (4<<4);  // Bits (3:2:1:0) = 0:1:0:0 --> AF4 for pin PB8;  Bits (7:6:5:4) = 0:1:0:0 --> AF4 for pin PB9
	
	
	// Reset the I2C 
	I2C1->CR1 |= (1<<15);
	I2C1->CR1 &= ~(1<<15);
	
	// Program the peripheral input clock in I2C_CR2 Register in order to generate correct timings
	I2C1->CR2 |= (45<<0);  // PCLK1 FREQUENCY in MHz
	
	// Configure the clock control registers
	I2C1->CCR = 225<<0;  // check calculation in PDF
	
	// Configure the rise time register
	I2C1->TRISE = 46;  // check PDF again
	
	// Program the I2C_CR1 register to enable the peripheral
	I2C1->CR1 |= (1<<0);  // Enable I2C
}

void I2C_Start (void)
{


	I2C1->CR1 |= (1<<10);  // Enable the ACK
	I2C1->CR1 |= (1<<8);  // Generate START
	while (!(I2C1->SR1 & (1<<0)));  // Wait fror SB bit to set
}

void I2C_Write (uint8_t data)
{

	while (!(I2C1->SR1 & (1<<7)));  // wait for TXE bit to set
	I2C1->DR = data;
	while (!(I2C1->SR1 & (1<<2)));  // wait for BTF bit to set
}


void I2C_Address (uint8_t Address)
{

	
	I2C1->DR = Address;  //  send the address
	while (!(I2C1->SR1 & (1<<1)));  // wait for ADDR bit to set
	uint8_t temp = I2C1->SR1 | I2C1->SR2;  // read SR1 and SR2 to clear the ADDR bit
}

void I2C_Stop (void)
{
	I2C1->CR1 |= (1<<9);  // Stop I2C
}



void I2C_WriteMulti (uint8_t *data, uint8_t size)
{

	
	while (!(I2C1->SR1 & (1<<7)));  // wait for TXE bit to set 
	while (size)
	{
		while (!(I2C1->SR1 & (1<<7)));  // wait for TXE bit to set 
		I2C1->DR = (uint32_t )*data++;  // send data
		size--;
	}
	
	while (!(I2C1->SR1 & (1<<2)));  // wait for BTF to set
}




void I2C_Read (uint8_t Address, uint8_t *buffer, uint8_t size)
{
	
	int remaining = size;
	if (size == 1)
	{
		
		I2C1->DR = Address;  //  send the address
		
		while (!(I2C1->SR1 & (1<<1)));  // wait for ADDR bit to set
		I2C1->CR1 &= ~(1<<10);  // clear the ACK bit 
		uint8_t temp = I2C1->SR1 | I2C1->SR2;  // read SR1 and SR2 to clear the ADDR bit.... EV6 condition
		I2C1->CR1 |= (1<<9);  // Stop I2C

			
		while (!(I2C1->SR1 & (1<<6)));  // wait for RxNE to set
		buffer[size-remaining] = I2C1->DR;  // Read the data from the DATA REGISTER
		
	}
		
	else 
	{
		
		I2C1->DR = Address;  //  send the address
		
		while (!(I2C1->SR1 & (1<<1)));  // wait for ADDR bit to set
		uint8_t temp = I2C1->SR1 | I2C1->SR2;  // read SR1 and SR2 to clear the ADDR bit
		
		while (remaining>2)
		{
			
			while (!(I2C1->SR1 & (1<<6)));  // wait for RxNE to set
			buffer[size-remaining] = I2C1->DR;  // copy the data into the buffer			
			I2C1->CR1 |= 1<<10;  // Set the ACK bit to Acknowledge the data received
			
			remaining--;
		}
		
		
		while (!(I2C1->SR1 & (1<<6)));  // wait for RxNE to set
		   buffer[size-remaining] = I2C1->DR;
		
		   I2C1->CR1 &= ~(1<<10);  // clear the ACK bit 
	     I2C1->CR1 |= (1<<9);  // Stop I2C
		
	    	remaining--;
	  	
		// Read the Last BYTE
		while (!(I2C1->SR1 & (1<<6)));  // wait for RxNE to set
		buffer[size-remaining] = I2C1->DR;  // copy the data into the buffer
	}	
	
}