#include "adc.h"

void init_ADC(void)
{
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOCEN;	//GPIOC clock enable
	
	ADC_input_port->MODER|=(3u<<(2*ADC_input_pin));	//ADC input pin is analogue mode
	
	RCC->APB2ENR|=RCC_APB2ENR_ADC1EN;		//ADC clock enable
	ADC1->SQR1&=~ADC_SQR1_L;						//set number of conversions per sequence to 1
	ADC1->SQR3&=~ADC_SQR3_SQ1;					//clear channel select bits
	ADC1->SQR3|=ADC_Channel;						//set channel 13 for PC3
	ADC1->CR2|=ADC_CR2_ADON;						//enable ADC
	
}


unsigned short read_adc(void)
{
	ADC1->CR2|=ADC_CR2_SWSTART;				//start ADC conversion
	while((ADC1->SR&ADC_SR_EOC)==0){__NOP();}	//wait for ADC conversion complete
	return ADC1->DR;									//return converted value
}



void Init_Timer3(void) {
    
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;  // Enable Timer 3 clock

    TIM3->PSC = 200-1;  
    TIM3->ARR = 900-1; 
    // 180Mhz/(200+1)*(900+1) ~ 1kHz sampling rate
    TIM3->DIER |= TIM_DIER_UIE; // Enable update interrupt
    TIM3->CR1 |= TIM_CR1_CEN;   // Enable timer

    NVIC_EnableIRQ(TIM3_IRQn);  // Enable TIM3 interrupt in NVIC
	
}