#include "DAC.h"




void DAC_Init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_DACEN;
	
	GPIOA->MODER |= GPIO_MODER_MODER4;
	
	DAC->CR |= DAC_CR_EN1;
}

void Set_Voltage(uint16_t voltage)
{
	voltage &= 0x0FFF;
	
	DAC->DHR12R1 = voltage;
}

void Timer6_Init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN; // Enable Timer 6 clock
    TIM6->PSC = 8399;  // Prescaler (PSC + 1 = 8400) -> 10 kHz timer clock
    TIM6->ARR = 99;     // Auto-reload: 10 kHz / 100 samples = 100 Hz update rate
    TIM6->DIER |= TIM_DIER_UIE; // Enable update interrupt
    TIM6->CR1 |= TIM_CR1_CEN;   // Start timer
    NVIC_EnableIRQ(TIM6_DAC_IRQn); // Enable interrupt in NVIC
}

void Generate_Sine_Wave(void) {
    for (int i = 0; i < SINE_SAMPLES; i++) {
        sine_wave[i] = (uint16_t)((DAC_MAX / 2) * (1.0 + sin(2 * PI * i / SINE_SAMPLES)));
    }
}

void TIM6_DAC_IRQHandler(void) {
    if (TIM6->SR & TIM_SR_UIF) { // Check interrupt flag
        TIM6->SR &= ~TIM_SR_UIF; // Clear flag
        DAC->DHR12R1 = sine_wave[index]; // Update DAC output
        index = (index + 1) % SINE_SAMPLES; // Increment index and wrap around
    }
}

