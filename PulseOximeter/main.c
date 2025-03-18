#include <stm32f4xx.h>
#include "PLL_Config.c"
#include "heartrate.h"
#include "readUSART.h"
#include "buffer.h"
#include "adc.h"
#include "led.h"



// Declare a circular buffer instance
CircularBuffer oximeter_buffer;

//Sampling threshold:
#define SAMPLE_RATE 1000       // 1 kHz sampling rate
#define PEAK_THRESHOLD 2000    // Adjust based on ADC values (0-4095)
#define MIN_PEAK_INTERVAL 300  // Minimum time between peaks (300 ms = 200 BPM max)

volatile uint16_t adc_value = 0;
volatile uint16_t prev_adc_value = 0;
volatile uint32_t last_peak_time = 0;
volatile uint32_t current_time = 0;
volatile uint8_t peak_detected = 0;
volatile uint16_t heart_rate = 0;  // BPM



int main(void)
{
	
	 // Configure the PLL to run the microcontroller at 180 MHz
	 PLL_Config();
	 SystemCoreClockUpdate();
	
	 //config ADC
	 init_ADC();		         
	 Init_Timer3();	
	
	
	
	 // Initialise LED
   Init_LED();								
	 Init_Timer2();	
	
	
	
	
	  uint8_t received_data;

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



void TIM3_IRQHandler(void)
{
	 if (TIM3->SR & TIM_SR_UIF) {  // Check for update interrupt flag
        TIM3->SR &= ~TIM_SR_UIF;  // Clear interrupt flag
		    
		    adc_value = read_adc();
		    current_time++;           // Increment time (1 ms per step)

        // Detect rising edge (heartbeat peak)
        if (adc_value > PEAK_THRESHOLD && prev_adc_value <= PEAK_THRESHOLD) {
            if (current_time - last_peak_time > MIN_PEAK_INTERVAL) {
                // Calculate BPM
                heart_rate = 60000 / (current_time - last_peak_time);
                last_peak_time = current_time;
                peak_detected = 1;
							  Toggle_LED2();       // HEARTRATE led
							  
            }
        }
				 prev_adc_value = adc_value; // Update previous ADC value
    }

}


