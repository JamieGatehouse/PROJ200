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
	RCC->APB1ENR|=RCC_APB1ENR_TIM2EN;		//timer 2 clock enabled
	TIM2->DIER|=TIM_DIER_UIE;						//timer uptdate interrupt enabled
																			//APB clock is Fcy/2 = 180MHz/2 = 90MHz
	TIM2->PSC=256-1;										//divide APB clock by 256 = 90MHz/256 = 351kHz
	TIM2->ARR=6515;										//counter reload value, gives a timer period of 100ms when F_APB = 90MHz and PSC = 256
	TIM2->CNT=0;												//zero timer counter
	NVIC->ISER[0]|=(1u<<28);						//timer 2 global interrupt enabled
	TIM2->CR1|=TIM_CR1_CEN;							//start timer counter
}

void TIM2_IRQHandler(void)			//TIMER 2 INTERRUPT SERVICE ROUTINE
{
	TIM2->SR&=~TIM_SR_UIF;				//clear interrupt flag in status register
	Toggle_LED();									//FLASH LED
	
}


