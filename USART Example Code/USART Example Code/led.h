#ifndef LED_H_
#define LED_H_

#include "stm32f429xx.h"



#define clr_GPIOA() GPIOA->BSRR&=~(3u<<(2*0));
void Toggle_LED (void);
void Toggle_LED2 (void);
void Init_LED(void);
void Init_Timer2(void);



#endif