#include <stm32f4xx.h>
#include "PLL_Config.c"
#include "heartrate.h"
#include "readUSART.h"


unsigned short ADC_DATA;
int main(void)
{
	/*
	//for heartrate
	//PLL_Config();
	SystemCoreClockUpdate();
	
	init_ADC();		//config ADC
	
	
	//LED
	
	
	Init_LED();										// Initialise LED
	Init_Timer2();	
	while(1)
	{
		
		ADC_DATA=read_adc();			//read value from ADC
		
	
	}
	*/
	
	//for read USART
	unsigned char received_data;
    
    // Initialize system clock if needed
    // PLL_Config();
    // SystemCoreClockUpdate();
    
    // Initialize USART
    init_USART();
    
    while(1)
    {
        // Check if data is available
        if(is_usart_data_available())
        {
            // Read data from USART
            received_data = receive_usart();
            
            // Echo the received character back to PuTTY
            send_usart(received_data);
            
            // You can add your processing logic here
            // For example, perform actions based on received commands
        }
    }
}
