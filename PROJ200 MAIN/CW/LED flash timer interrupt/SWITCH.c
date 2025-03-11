#include "SWITCH.h"

//declare a variable x




void LED_INIT(void)
{
	//Enable clock B
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	//Set pin PB0 to output for LED1
	GPIOB->MODER |= (0x01<<(2*0));			//ensure the first bit in the regester is one
	GPIOB->MODER &= ~(0x02<<(2*0));			//ensure the second bit in the regester is zero
	//Set pin PB7 to output for LED2
	GPIOB->MODER |= (0x01<<(2*7));			//ensure the first bit in the regester is one
	GPIOB->MODER &= ~(0x02<<(2*7));			//ensure the second bit in the regester is zero
	//Set pin PB14 to output for LED3
	GPIOB->MODER |= (0x01<<(2*14));			//ensure the first bit in the regester is one
	GPIOB->MODER &= ~(0x02<<(2*14));			//ensure the second bit in the regester is zero
	
}


void BTN_INIT(void)
{
	//Enable clock C
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	//set pin PC13 to input for the user button
	GPIOC->MODER &= ~(0x03<<(2*13));			//ensure mode of PC13 is set to 00 for an input
}

void LED_ON(void)
{
	GPIOB->ODR |= (1<<0);			//only turn LED1 on
	GPIOB->ODR |= (1<<7);			//only turn LED2 on
	GPIOB->ODR |= (1<<14);			//only turn LED3 on
}


void LED_OFF(void)
{
	GPIOB->ODR &= ~(1<<0);			//only turn LED1 off
	GPIOB->ODR &= ~(1<<7);			//only turn LED2 off
	GPIOB->ODR &= ~(1<<14);			//only turn LED3 off
}

void LED_MODE(void)
{
	GPIOB->ODR |= (1<<14);			//only turn LED3 on
	GPIOB->ODR &= ~(1<<0);			//only turn LED1 off
	GPIOB->ODR &= ~(1<<7);			//only turn LED2 off
}


void delay(void)
{
	for(j=0; j<100000; j++);			//set variable i to 1 and loop until the value of i reaches 1 million
}