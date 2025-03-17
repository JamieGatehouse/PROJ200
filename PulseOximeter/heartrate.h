#include "stm32f429xx.h"



//ADC
#define TRAF_YEL1_PIN     3
#define ADC_input_port		GPIOC
#define ADC_input_pin			0
#define ADC_Channel				10
void Control_LEDs(uint16_t ldr_value); 
void init_ADC(void);
unsigned short read_adc(void);



//LED
#define clr_GPIOD() GPIOD->BSRR&=~(3u<<(2*0));
void Init_Timer2(void);
void LED_Init(void);
void Toggle_LED1();
