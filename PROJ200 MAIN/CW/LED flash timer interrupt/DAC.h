#include <stm32f429xx.h>		//INCLUDE THE HEADER FILE FOR THIS MCU FAMILY
#include <math.h>


#define SINE_SAMPLES 100
#define DAC_MAX 4095
#define PI 3.14159265359


volatile uint16_t sine_wave[SINE_SAMPLES];
volatile uint8_t index = 0;

volatile int period;


void DAC_Init(void);

void Set_Voltage(uint16_t voltage);


void Timer6_Init(void);

void Generate_Sine_Wave(void);
void TIM6_DAC_IRQHandler(void);