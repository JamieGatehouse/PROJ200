#include "usart.h"
#include "adc.h"
#include <stdio.h>  // Include for snprintf to format strings

void TIM3_IRQHandler(void)
{
    if (TIM3->SR & TIM_SR_UIF)  // Check if the update interrupt flag is set
    {
        TIM3->SR &= ~TIM_SR_UIF;  // Clear the interrupt flag

        unsigned short adc_value = read_adc();  // Read ADC value

        // Prepare the ADC value string to send over USART
        char adc_str[16];  // To hold the string representation of the ADC value
        snprintf(adc_str, sizeof(adc_str), "ADC Value: %u\r\n", adc_value);  // Format ADC value

        // Send the formatted string over USART
        for (int i = 0; adc_str[i] != '\0'; i++) {
            send_usart(adc_str[i]);  // Send each character
        }
    }
}

int main(void)
{
    init_USART();  // Initialize USART
    init_ADC();    // Initialize ADC
    Init_Timer3(); // Initialize Timer 3
	 
  	uint16_t adc_value;

    while (1)
    {
        // Main loop does nothing; all work is done by the interrupt
        __NOP();  // No Operation (for the main loop, the work is handled by the interrupt)
    }
}