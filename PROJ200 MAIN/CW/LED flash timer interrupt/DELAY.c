#include "DELAY.h"
#include "ADC.h"
#include "LED.h"
//DELAY.h containing declarations and #defines with suitable comments describing them	1		
//DELAY.c containing structured routines and comments describing the algorithm (>50% of code commented)	1		
//180Mhz operation - PLL_Config	1		
//Accurate Delay routines using TIMERs	1		
//PWM to control LED Brightness	2		
//Interrupts (UART, Timers, ADC, DAC) 	2		
//Watchdog timer used to prevent lock up / Low power mode	2		





// PWM Initialization
void PWM_Init(void) {
    // Configure PWM outputs for Green (TIM4_CH1 = PB7) and Blue (TIM4_CH2 = PB0) LEDs
    GPIOB->MODER |= (0x2 << (7 * 2)); // Alternate function mode
    GPIOB->AFR[0] |= (2 << (7 * 4)); // AF2 for PB7 (TIM4_CH1)
}




// Timer Initialization
void TIM4_Init(void) {
    // Enable TIM4 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

    // Configure TIM4 for 1 kHz PWM
    TIM4->PSC = 84 - 1; // Prescaler for 1 MHz clock
    TIM4->ARR = 100 - 1; // Auto-reload value for 1 kHz
    TIM4->CCMR1 = (6 << TIM_CCMR1_OC1M_Pos) | (6 << TIM_CCMR1_OC2M_Pos); // PWM mode
    TIM4->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E; // Enable capture/compare
    TIM4->CR1 |= TIM_CR1_CEN; // Enable timer
}

void TIM4_IRQHandler(void)			//TIMER 4 INTERRUPT SERVICE ROUTINE FOR PWM
{
	TIM4->SR&=~TIM_SR_UIF;				//clear interrupt flag in status register
  TIM4->CCR2 = 50;
	
	
}



void TIM3_IRQHandler(void)			//TIMER 3 INTERRUPT SERVICE ROUTINE FOR ADC
{
	TIM3->SR&=~TIM_SR_UIF;				//clear interrupt flag in status register

	
}




//I2C
void TIM6_Delay (void)
{
	

// 1. Enable Timer clock
	RCC->APB1ENR |= (1<<4);  // Enable the timer6 clock
	
// 2. Set the prescalar and the ARR
	TIM6->PSC = 90-1;  // 90MHz/90 = 1 MHz ~~ 1 uS delay
	TIM6->ARR = 0xffff;  // MAX ARR value
	
                                                   // Enable the Timer, and wait for the update Flag to set
	TIM6->CR1 |= (1<<0);                             // Enable the Counter
	while (!(TIM6->SR & (1<<0)));                    // Update interrupt flag, This bit is set by hardware when the registers are updated
}



void Delay_us (uint16_t us)
{

	TIM6->CNT = 0;
	while (TIM6->CNT < us);    // delay of 1 us, i.e TIM6
}



void Delay_ms (uint16_t ms)
{
	for (uint16_t i=0; i<ms; i++)
	{
		Delay_us (1000); // delay of 1 ms
	}
}




//watchdog


void IWDG_Init(void) {
    // Enable the IWDG (independent watchdog) peripheral
    IWDG->KR = 0x5555;  // Unlock the IWDG

    // Set the prescaler value to divide the LSI clock (usually 32kHz)
    // Assuming LSI frequency is 32 kHz
    IWDG->PR = 0x04;

    // Set the reload value (this determines the timeout period)
    // For example, a reload value of 0x0FFF gives a timeout period of ~1 second
    IWDG->RLR = 0x0FFF;  // Set the reload value

    // Enable the IWDG
    IWDG->KR = 0xAAAA;  // Start the watchdog timer
}

// Function to reset the Independent Watchdog (i.e., to feed the dog)
void IWDG_Reset(void) {
   Delay_ms(1000);
	IWDG->KR = 0xAAAA;  // Reset the watchdog counter
}



