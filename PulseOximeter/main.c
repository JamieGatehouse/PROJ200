#include <stm32f4xx.h>
#include "PLL_Config.c"
#include "heartrate.h"
#include "readUSART.h"
#include "buffer.h"


// Declare a circular buffer instance
CircularBuffer oximeter_buffer;




unsigned short ADC_DATA;
int main(void)
{
	
	//for heartrate
	//PLL_Config();
	SystemCoreClockUpdate();
	
	init_ADC();		//config ADC
	Init_Timer3();	
	
	//LED
	
	
	Init_LED();										// Initialise LED
	Init_Timer2();	
	//while(1)
	//{
		
	//	ADC_DATA=read_adc();			//read value from ADC
		
	
	//}
	
	
	
	  uint8_t received_data;

    // Configure the PLL to run the microcontroller at 180 MHz
    PLL_Config();

    // Initialize USART3
    init_USART3();

    // Initialize the buffer
    init_buffer(&oximeter_buffer);  // Pass the buffer to initialize

    while (1)
    {
        // Check if data is available in USART
        if (is_usart_data_available())
        {
            // Read the received data from USART
            received_data = receive_usart();

            // Write the received data to the buffer
            write_to_buffer(&oximeter_buffer, received_data);

            // Optionally, echo the received data back to the sender
            send_usart(received_data);
        }
    
    }
	
}
