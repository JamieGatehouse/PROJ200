#include "adc.h"

void init_ADC(void)
{
	 RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;  // Enable GPIOC clock (for PC3)
    
    // Set PC3 to analog mode for ADC
    ADC_input_port->MODER |= (3u << (2 * ADC_input_pin));  // Analog mode for PC3

    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;  // Enable ADC1 clock
    

    // Set the ADC conversion sequence to use Channel 13
    ADC1->SQR1 &= ~ADC_SQR1_L;          // 1 conversion in sequence
    ADC1->SQR3 &= ~ADC_SQR3_SQ1;        // Clear channel select bits
    ADC1->SQR3 |= ADC_Channel;          // Set Channel 13 (PC3)
    
    // Enable ADC
    ADC1->CR2 |= ADC_CR2_ADON;          // Enable ADC
	
}


unsigned short read_adc(void)
{
	ADC1->CR2|=ADC_CR2_SWSTART;				//start ADC conversion
	while((ADC1->SR&ADC_SR_EOC)==0){__NOP();}	//wait for ADC conversion complete
	return ADC1->DR;									//return converted value
}



void Init_Timer3(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;  // Enable Timer 3 clock
    TIM3->PSC = (SystemCoreClock / 1000000) - 1;  // Prescale to 1 MHz (1 µs steps)
    TIM3->ARR = 999;  // 1000 µs = 1 ms (1 kHz)
    TIM3->DIER |= TIM_DIER_UIE;  // Enable update interrupt
    TIM3->CR1 |= TIM_CR1_CEN;    // Start timer
    NVIC_EnableIRQ(TIM3_IRQn);   // Enable Timer 3 IRQ
}