#ifndef __LED_H
#define __LED_H
#include <stm32f429xx.h>



// Function prototypes

void GPIOB_Init(void);
void Toggle_LED1(void);
void Toggle_LED2 (void);

#define clr_GPIOB() GPIOB->BSRR&=~(3u<<(2*0));
#define clr_GPIOE() GPIOE->BSRR&=~(3u<<(0*2));

#endif
