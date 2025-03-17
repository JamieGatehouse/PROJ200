#include "heartrate.h"



void init_ADC(void)
{
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOCEN;	//GPIOC clock enable
	
	ADC_input_port->MODER|=(3u<<(2*ADC_input_pin));	//ADC input pin is analogue mode
	
	RCC->APB2ENR|=RCC_APB2ENR_ADC1EN;		//ADC clock enable
	ADC1->SQR1&=~ADC_SQR1_L;						//set number of conversions per sequence to 1
	ADC1->SQR3&=~ADC_SQR3_SQ1;					//clear channel select bits
	ADC1->SQR3|=ADC_Channel;						//set channel
	ADC1->CR2|=ADC_CR2_ADON;						//enable ADC
	
}


unsigned short read_adc(void)
{
	ADC1->CR2|=ADC_CR2_SWSTART;				//start ADC conversion
	while((ADC1->SR&ADC_SR_EOC)==0){__NOP();}	//wait for ADC conversion complete
	return ADC1->DR;									//return converted value
}


void Control_LEDs(uint16_t ldr_value)
	{
    
    // Turn on LED if ADC value is above a threshold (e.g., 2048 for 12-bit ADC)
   
    if (ldr_value > 2048) GPIOC->ODR^=(1u<<TRAF_YEL1_PIN);  // Turn on TRAF_YEL1 (PC3)
    else GPIOC->ODR&=~(1u<<TRAF_YEL1_PIN);                                 // Turn off TRAF_YEL1
              
}
	


//LED

void Init_LED(void)
{
	//ENABLE PORT(S)
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;		//GPIO B clock enable
	
	//CONFIGURE PORT PIN FUNCTIONS
	clr_GPIOA();
	GPIOA->MODER|=(1u<<(2*4));					   //RED LED
	GPIOA->MODER|=(1u<<(2*5));             //infra-red LED
}	
	

void Toggle_LED (void)
{
	GPIOA->ODR^=(1u<<4);								//XOR GPIOB output data register to invert the selected pin
	GPIOA->ODR^=(1u<<5);
}


void Init_Timer2(void)
{
	RCC->APB1ENR|=RCC_APB1ENR_TIM2EN;		//timer 2 clock enabled
	TIM2->DIER|=TIM_DIER_UIE;						//timer uptdate interrupt enabled
																			//APB clock is Fcy/2 = 180MHz/2 = 90MHz
	TIM2->PSC=256-1;										//divide APB clock by 256 = 90MHz/256 = 351kHz
	TIM2->ARR=3515;										//counter reload value, gives a timer period of 100ms when F_APB = 90MHz and PSC = 256
	TIM2->CNT=0;												//zero timer counter
	NVIC->ISER[0]|=(1u<<28);						//timer 2 global interrupt enabled
	TIM2->CR1|=TIM_CR1_CEN;							//start timer counter
}

void TIM2_IRQHandler(void)			//TIMER 2 INTERRUPT SERVICE ROUTINE
{
	TIM2->SR&=~TIM_SR_UIF;				//clear interrupt flag in status register
	Toggle_LED();									//FLASH LED
}
