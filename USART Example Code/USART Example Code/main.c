#include "usart.h"
#include "adc.h"
#include "led.h"
#include "lcd.h"
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

#define AVERAGE_WINDOW 10  // Smooth BPM over 4 beats
volatile uint16_t bpm_buffer[AVERAGE_WINDOW] = {0};
volatile uint8_t bpm_index = 0;

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
    if (new_adc_sample) {
        new_adc_sample = 0;
        adc_value = read_adc();
        if (adc_value > PEAK_THRESHOLD && prev_adc_value <= PEAK_THRESHOLD) {
            Toggle_LED2();  
            if (ms_counter - last_peak_time > MIN_PEAK_INTERVAL) {
                uint16_t new_bpm = 60000 / (ms_counter - last_peak_time);
                last_peak_time = ms_counter;
                bpm_buffer[bpm_index++] = new_bpm;
                if (bpm_index >= AVERAGE_WINDOW) bpm_index = 0;
                // Compute average BPM
                uint16_t sum = 0;
                for (uint8_t i = 0; i < AVERAGE_WINDOW; i++) sum += bpm_buffer[i];
                heart_rate = sum / AVERAGE_WINDOW;
            }
        }
        prev_adc_value = adc_value;
    }
}

int main(void)
{
    init_USART();  // Initialize USART
    init_ADC();    // Initialize ADC
    Init_Timer3(); // Initialize Timer 3
	
    Init_LED();
    Init_Timer2();
    
    // Initialize LCD
    initLCD();
    
    // Calculate the actual sample rate based on Timer3 settings
    uint16_t sample_rate = 1000000 / (TIM3->ARR + 1);  // Sample rate in Hz
    
    // Initial display of SR and HR
    LCD_UpdateDisplay(heart_rate, sample_rate);
    
    // Track when we last updated the LCD
    uint32_t last_lcd_update = 0;
    
    char bpm_str[32];  // To hold the string representation of the ADC value
    while (1)
    {
        process_heart_rate();  // Handle ADC reading & BPM calculation
        
        // Send BPM over USART
        snprintf(bpm_str, sizeof(bpm_str), "ADC data: %u\r\n", adc_value);
        for (int i = 0; bpm_str[i] != '\0'; i++) {
            send_usart(bpm_str[i]);  // Send each character over USART
        }
        
        // Update LCD every 500ms to avoid flicker
        if (ms_counter - last_lcd_update >= 500) { // Update every 500ms
            last_lcd_update = ms_counter;
            
            // Update heart rate on LCD (second line)
            LCD_DisplayHeartRate(heart_rate);
            
            // We don't need to update sample rate repeatedly as it's fixed
            // LCD_DisplaySampleRate(sample_rate);
        }
        
        __NOP();  // No Operation (waiting for next interrupt)
    }
}
