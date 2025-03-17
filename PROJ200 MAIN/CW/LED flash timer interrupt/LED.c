#include "LED.h"
#include "DELAY.h"


// GPIO Initialization
void LED_Init(void) {
    // Enable GPIO clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIODEN;

    // Configure Red (PB14), BLue (PB7), and Green (PB0) LEDs
     clr_GPIOB();
	   GPIOB->MODER|=(1u<<(2*14));		//set new pin functions on GPIOB
	   GPIOB->MODER|=(1u<<(2*0));
     GPIOB->OTYPER &= ~(0x8001 << 0); // Push-pull output for PB0, PB7, PB14
     GPIOB->OSPEEDR |= (0x3F << (0 * 2)) | (0x3 << (14 * 2)); // High speed
	
	   //finger led
	   GPIOD->MODER|=(1u<<(2*13))
	   GPIOD->OTYPER &= ~(0x8001 << 0); // Push-pull output for PB0, PB7, PB14
     GPIOD->OSPEEDR |= (0x3F << (13 * 2)); // High speed


	
    	//CONFIGURE PORT PIN FUNCTIONS
	  clr_GPIOE();
   	GPIOE->MODER&=~(3u<<(0*2));						//clear pin functions on GPIOC
   	GPIOE->MODER|=(1u<<(12*0));			//RED GREEN BLUE BAR
	  GPIOE->MODER|=(1u<<(15*2));     //7-SEGMENT display
  
  
}







// TIMER FOR RED LED
void Init_Timer2(void)
{
	RCC->APB1ENR|=RCC_APB1ENR_TIM2EN;		//timer 2 clock enabled
	TIM2->DIER|=TIM_DIER_UIE;						//timer uptdate interrupt enabled
																			//APB clock is Fcy/2 = 180MHz/2 = 90MHz
	TIM2->PSC=256-1;										//divide APB clock by 256 = 90MHz/256 = 351kHz
	TIM2->ARR=10000;										//counter reload value, gives a timer period of 100ms when F_APB = 90MHz and PSC = 256
	TIM2->CNT=0;												//zero timer counter
	NVIC->ISER[0]|=(1u<<28);						//timer 2 global interrupt enabled
	TIM2->CR1|=TIM_CR1_CEN;							//start timer counter
}


void TIM2_IRQHandler(void)			//TIMER 2 INTERRUPT SERVICE ROUTINE
{
	TIM2->SR&=~TIM_SR_UIF;				//clear interrupt flag in status register
	Toggle_LED1();									//FLASH LED
		
}

void Toggle_LED1 (void) {
	
	GPIOD->ODR^=(1u<<13);
	 
}
