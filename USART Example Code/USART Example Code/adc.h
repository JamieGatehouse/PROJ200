#ifndef ADC_H_
#define ADC_H_

#include "stm32f429xx.h"



//ADC
#define TRAF_YEL1_PIN     3
#define ADC_input_port		GPIOC
#define ADC_input_pin			2
#define ADC_Channel				13  //PC3 or A2  (pcb output)

void init_ADC(void);
unsigned short read_adc(void);
void Init_Timer3(void);

#endif