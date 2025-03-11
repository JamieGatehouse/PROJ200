#ifndef __DELAY_H
#define __DELAY_H

#include <stm32f429xx.h>
void Delay(uint32_t count);             // simple delay function

void TIM4_Init(void);                //timer4 set for PWM


void PWM_Init(void);

void TIM6Config(void);
void Delay_us (uint16_t us);
void Delay_ms (uint16_t ms);

//watchdog
void IWDG_Init(uint32_t timeout_ms);
void IWDG_Reset();
#endif