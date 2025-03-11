#include <stm32f429xx.h>		//INCLUDE THE HEADER FILE FOR THIS MCU FAMILY
 					//this file contains the definitions for register addresses and values etc...
#include "PLL_Config.c"
#include "LED.c"
#include "USART.c"
#include "ADC.c"
#include "DELAY.c"
#include "ISC.c"
#include "DAC.c"
#include "LCD.c"
#include "SWITCH.c"


int main(void) {
    // Initialize system clock and peripherals
    SystemCoreClockUpdate();
    PLL_Config();	
	  
	// RED LED
	  LED_Init();
    Init_Timer2();      //RED LED TIMER

	
	  //ADC
	  ADC_LED_Init();	
	  init_ADC();
    TIM3_Init();         //ADC TIMER

	  //TIMER 
	  TIM4_Init();         //Blue LED 50%
	  TIM6_Delay();        //Delay_us timer
    PWM_Init();
	  IWDG_Init();         // Initialize the watchdog with a 1-second timeout 
	 
	  // USART
	  init_USART();      
	  //USART_SendString("HELLO");
		
		//I2C
		I2C_Config();
		
		//DAC
		DAC_Init();
		Generate_Sine_Wave();
    Timer6_Init();
		
		
		//LCD
		 
    initLCD();
		
	
	   while (1) {
         
			 
			// CONTROL_LED_FUNCTION();     //Update to leds and DAC
			
			 
			 TIM4_IRQHandler(); 
			 // Set Blue LED to half brightness (PB7)
			 
			
			 
			 // Scroll a message across the second line of the screen
	     const char *message = "MODE: A:HEARTRATE B:OXIMETER C:TEMPERATURE";
       scrollTextA(message);
       
			  
   
			
			//  __NOP();
			 
			 
       
			 
			
			 
		   //IWDG_Reset();           // Reset the watchdog periodically to prevent a system reset
			  
			
      }
}


