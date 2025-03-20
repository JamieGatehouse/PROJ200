#include "readUSART.h"
#include "adc.h"
#include <stdio.h>  // Include for snprintf to format strings

volatile unsigned short adc_value = 0;  // To store the ADC value from the interrupt
volatile uint8_t adc_ready = 0;         // Flag to indicate when ADC value is ready
volatile uint8_t new_adc_sample = 0;  // Flag to indicate new ADC data available

#define SAMPLE_RATE 1000  // 1 kHz sampling
#define BUFFER_SIZE 100   // Store last 100 samples

volatile uint16_t adc_buffer[BUFFER_SIZE];
volatile uint8_t buffer_index = 0;
volatile uint32_t last_peak_time = 0;
volatile uint16_t heart_rate = 0;
volatile uint32_t ms_counter = 0; // Global counter that increments every 1ms


void TIM3_IRQHandler(void) {
    if (TIM3->SR & TIM_SR_UIF) {
        TIM3->SR &= ~TIM_SR_UIF;  // Clear interrupt flag
        ms_counter++;  // Increment global millisecond counter

        new_adc_sample = 1;  // Set flag to process ADC data in the main loop
    }
}

int main(void)
{
    init_USART();  // Initialize USART
    init_ADC();    // Initialize ADC
    Init_Timer3(); // Initialize Timer 3
    
    char adc_str[16];  // To hold the string representation of the ADC value

    while (1)
    {
        // Check if the ADC value is ready to be processed
        if (adc_ready)
        {
            // Prepare the ADC value string to send over USART
            snprintf(adc_str, sizeof(adc_str), "ADC Value: %u\r\n", adc_value);  // Format ADC value

            // Send the formatted string over USART
            for (int i = 0; adc_str[i] != '\0'; i++) {
                send_usart(adc_str[i]);  // Send each character over USART
            }

            adc_ready = 0;  // Reset the flag after processing
        }

        // The main loop can do other tasks or simply wait for the interrupt to update the ADC value
        __NOP();  // No Operation (for the main loop, the work is handled by the interrupt)
    }
}



