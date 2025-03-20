#include "usart.h"
#include "adc.h"
#include "led.h"
#include <stdio.h>  // Include for snprintf to format strings




// === Global Variables ===
volatile unsigned short adc_value = 0;  // Latest ADC value
volatile uint8_t new_adc_sample = 0;    // Flag: New ADC sample ready
volatile uint32_t ms_counter = 0;       // Global millisecond counter

//volatile uint8_t adc_ready = 0;         // Flag to indicate when ADC value is ready


#define SAMPLE_RATE 1000       // 1 kHz sampling rate
#define PEAK_THRESHOLD 2500    // Adjust based on ADC values (0-4095)
#define LOW_THRESHOLD 500
#define MIN_PEAK_INTERVAL 300  // Minimum time between peaks (300 ms = 200 BPM max)

volatile uint16_t prev_adc_value = 0;
volatile uint32_t last_peak_time = 0;
volatile uint16_t heart_rate = 0;  // BPM





// === Timer 3 Interrupt Service Routine ===
void TIM3_IRQHandler(void) {
    if (TIM3->SR & TIM_SR_UIF) {   // Check update interrupt flag
        TIM3->SR &= ~TIM_SR_UIF;   // Clear interrupt flag

        ms_counter++;  // Increment global time counter (1 ms per step)

        new_adc_sample = 1;  // Set flag for processing in main loop
    }
}




// === Heart Rate Processing Function ===
void process_heart_rate(void) {
    if (new_adc_sample) {  // Check if new ADC sample is available
        new_adc_sample = 0;  // Reset flag

        adc_value = read_adc();  // Read ADC from photodiode
        
        // Detect heartbeat peak (rising edge detection)
        if (adc_value > PEAK_THRESHOLD && prev_adc_value <= PEAK_THRESHOLD) {
            Toggle_LED2();  // Flash LED on heartbeat
					
				  
            
            if (ms_counter - last_peak_time > MIN_PEAK_INTERVAL) {
                // Calculate BPM
                heart_rate = 60000 / (ms_counter - last_peak_time);
                last_peak_time = ms_counter;
            }
					
        }
        
        prev_adc_value = adc_value; // Update last ADC value
    }
}




int main(void)
{
    init_USART();  // Initialize USART
    init_ADC();    // Initialize ADC
    Init_Timer3(); // Initialize Timer 3
	
	  Init_LED();
	  Init_Timer2();
    
    char bpm_str[16];  // To hold the string representation of the ADC value

    while (1)
    {
        process_heart_rate();  // Handle ADC reading & BPM calculation

        // Send BPM over USART
        snprintf(bpm_str, sizeof(bpm_str), "BPM: %u\r\n", heart_rate);
        for (int i = 0; bpm_str[i] != '\0'; i++) {
            send_usart(bpm_str[i]);  // Send each character over USART
        }

        __NOP();  // No Operation (waiting for next interrupt)
    }
}
