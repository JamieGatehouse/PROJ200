#include "led.h"

void Init_LED(void)
{
	//ENABLE PORT(S)
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;		//GPIO A clock enable
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN;		//GPIO B clock enable
	
	
	//CONFIGURE PORT PIN FUNCTIONS
	clr_GPIOA();
	GPIOB->MODER&=~(3u<<(2*0));						//clear pin functions on GPIOB
	GPIOB->MODER|=(1u<<(2*0));	
	GPIOA->MODER|=(1u<<(2*4));					   //RED LED
	GPIOA->MODER|=(1u<<(2*5));             //infra-red LED
}	
	

void Toggle_LED (void)
{
	GPIOA->ODR^=(1u<<4);								//XOR GPIOB output data register to invert the selected pin
	GPIOA->ODR^=(1u<<5);
}

void Toggle_LED2 (void)    //Test heartrate
{
	GPIOB->ODR^=(1u<<0);   
}


void Init_Timer2(void)
{
	 RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;  // Enable Timer 3 clock
    TIM2->PSC = (SystemCoreClock / 1000000) - 1;  // Prescale to 1 MHz (1 µs steps)
    TIM2->ARR = 999;  // 1000 µs = 1 ms (1 kHz)
    TIM2->DIER |= TIM_DIER_UIE;  // Enable update interrupt
    TIM2->CR1 |= TIM_CR1_CEN;    // Start timer
    NVIC_EnableIRQ(TIM2_IRQn);   // Enable Timer 3 IRQ
}

void TIM2_IRQHandler(void)			//TIMER 2 INTERRUPT SERVICE ROUTINE
{
	TIM2->SR&=~TIM_SR_UIF;				//clear interrupt flag in status register
	Toggle_LED();									//FLASH LED
	
}

