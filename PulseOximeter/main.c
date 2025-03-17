#include <stm32f4xx.h>
#include "PLL_Config.c"
#include "heartrate.h"


unsigned short ADC_DATA;
int main(void)
{
	//PLL_Config();
	SystemCoreClockUpdate();
	
	init_ADC();		//config ADC
	
	
	while(1)
	{
		
		ADC_DATA=read_adc();			//read value from ADC
	
	}
}
