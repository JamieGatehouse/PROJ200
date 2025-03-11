#include <stm32f429xx.h>		//INCLUDE THE HEADER FILE FOR THIS MCU FAMILY
 					//this file contains the definitions for register addresses and values etc...
#include "PLL_Config.c"
#include "LED.c"
#include "USART.c"
#include "ADC.c"
#include "DELAY.c"
#include "ISC.c"


int main(void) {
    // Initialize system clock and peripherals
    SystemCoreClockUpdate();
    PLL_Config();	
	  
	// RED LED
	  GPIOB_Init();
    Init_Timer2();

	
	  //ADC
	  ADC_LED_Init();	
	  init_ADC();
    TIM3_Init();

	  //TIMER 
	  TIM4_Init();
	  TIM6Config();
    PWM_Init();
	  //IWDG_Init(10000);         // Initialize the watchdog with a 1-second timeout 
	 
	  // USART
	  init_USART();
	  
		//I2C
		I2C_Config();
	
	   while (1) {
         
       TIM4_IRQHandler(); 
			 // Set Blue LED to half brightness (PB7)
			
   
			 USART_SendString("\x1B[32m .. \x1B[0m");
			  __NOP();
			 
			 CONTROL_LED_FUNCTION();
			 //ADC VOLTAGE OUTPUTS
			
			 
		   //IWDG_Reset();           // Reset the watchdog periodically to prevent a system reset
			  
			
      }
}


