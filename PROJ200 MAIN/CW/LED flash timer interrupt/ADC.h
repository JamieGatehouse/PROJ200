#ifndef _ADC_H
#define _ADC_H

#include "stm32f429xx.h"

// Define LED pins
#define TRAF_RED1_PIN  2  // PC2
#define TRAF_YEL1_PIN  3 // PC3
#define TRAF_GRN1_PIN  6  // PC6

#define ADC_LDR_pin 0 //PC0
#define ADC_POT_pin 0  //PA0
#define ADC_MIC_pin 3  //PA3

#define ADC_LDR_port		GPIOC
#define ADC_POT_port    GPIOA
#define ADC_MIC_port    GPIOA

#define ADC_LDR_Channel				10
#define ADC_POT_Channel				0  
#define ADC_MIC_Channel				3  
        
//void Toggle_LED (void);
void ADC_LED_Init(void);


void CONTROL_LED_FUNCTION (void);


void init_ADC_LDR(void);
void init_ADC_POT(void);
void init_ADC_MIC(void);

void TIM3_Init(void);              //timer3 set for ADC for 1kHz

unsigned short LDR_value;
unsigned short POT_value;
unsigned short MIC_value;
uint16_t read_adc(uint16_t channel);

void output_dac(unsigned short d);


#endif


