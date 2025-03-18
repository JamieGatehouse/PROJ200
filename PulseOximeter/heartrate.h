#include "stm32f429xx.h"



//ADC
#define TRAF_YEL1_PIN     3
#define ADC_input_port		GPIOA
#define ADC_input_pin			2
#define ADC_Channel				10
void Control_LEDs(uint16_t ldr_value); 
void init_ADC(void);
unsigned short read_adc(void);
void Init_Timer3(void);


//LED

#define clr_GPIOA() GPIOA->BSRR&=~(3u<<(2*0));
void Toggle_LED (void);
void Init_LED(void);
void Init_Timer2(void);
