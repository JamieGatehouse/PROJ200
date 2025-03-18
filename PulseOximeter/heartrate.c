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



void Init_Timer3(void) {
    
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;  // Enable Timer 3 clock

    TIM3->PSC = 200-1;  
    TIM3->ARR = 900-1; 
    // 180Mhz/(200+1)*(900+1) ~ 1kHz sampling rate
    TIM3->DIER |= TIM_DIER_UIE; // Enable update interrupt
    TIM3->CR1 |= TIM_CR1_CEN;   // Enable timer

    NVIC_EnableIRQ(TIM3_IRQn);  // Enable TIM3 interrupt in NVIC
	
}

void TIM3_IRQHandler(void)
{
	 if (TIM3->SR & TIM_SR_UIF) {  // Check for update interrupt flag
        TIM3->SR &= ~TIM_SR_UIF;  // Clear interrupt flag
		   
    		//ADC DATA 
    }

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
