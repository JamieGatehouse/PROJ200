#include "lcd.h"
#include <stdio.h>

void lcd_delayus(unsigned int us)		//blocking delay for LCD, argument is approximate number of micro-seconds to delay
{
	unsigned char i;
	while(us--)
	{
		for(i=0; i<SystemCoreClock/4000000; i++){__NOP();}
	}
}

void WaitLcdBusy(void)
{
	//lcd_delayus(3000);		//3ms blocking delay
	
	// Poll busy flag until no longer busy
	unsigned char busyFlag;
	do {
		busyFlag = read_LCD_busy_flag();
	} while (busyFlag & 0x80); // 1000_0000 means busy is set
}

void set_LCD_data(unsigned char d)
{
	LCD_PORT->BSRR=(0xff<<(LCD_D0_pin+16));	//clear data lines
	LCD_PORT->BSRR=(d<<LCD_D0_pin);					//put data on lines
}

void LCD_strobe(void)		//10us high pulse on LCD enable line
{
	lcd_delayus(10);	// wait 10us
	set_LCD_E();			// set enable line to HIGH
	lcd_delayus(10);  // wait 10 us
	clr_LCD_E();			// clear enable line to LOW
}

void cmdLCD(unsigned char cmd)		//sends a byte to the LCD control register
{
	WaitLcdBusy();				//wait for LCD to be not busy
	clr_LCD_RS();					//control command
	clr_LCD_RW();					//write command
	set_LCD_data(cmd);		//set data on bus
	LCD_strobe();					//apply command
}

void putLCD(unsigned char put)	//sends a char to the LCD display
{
	WaitLcdBusy();				//wait for LCD to be not busy
	set_LCD_RS();					//text command
	clr_LCD_RW();					//write command
	set_LCD_data(put);		//set data on bus
	LCD_strobe();					//apply command
}

// Single read of the busy flag in control register
// Use 10us delay when strobbing enable line
unsigned char read_LCD_busy_flag(void) {
	unsigned char busyFlag; // local var to store busy flag data
	
	// Setup instruction to LED
	
	// Set the data pins as input
	LCD_PORT->MODER &= ~(0xffff << (LCD_D0_pin * 2));
	// set RW to read mode (1) so we can read control data onto data pins
	set_LCD_RW();
	// Clear RS to signal read of command register
	clr_LCD_RS();
	
	// Signal instruction to LED
	
	lcd_delayus(10);	// wait 10us
	// set enable line to HIGH
	set_LCD_E();
	lcd_delayus(10);
	// Small delay to allow data to stablise
	// Read 8 bits of IDR onto data lines with 0xFF mask
	busyFlag = (LCD_PORT->IDR >> LCD_D0_pin) & 0xFF;
	// Now clear enable line
	clr_LCD_E();
	
	// Set data lines back to output
	LCD_PORT->MODER |= (0x5555 << (LCD_D0_pin * 2));
	
	return busyFlag;
}

void initLCD(void)
{
	SystemCoreClockUpdate();
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIODEN;	//enable LCD port clock


	//CONFIG LCD GPIO PINS
	LCD_PORT->MODER&=~(					//clear pin direction settings
		(3u<<(2*LCD_RS_pin))
		|(3u<<(2*LCD_RW_pin))
		|(3u<<(2*LCD_E_pin))
		|(0xffff<<(2*LCD_D0_pin))
		);


	LCD_PORT->MODER|=(				//reset pin direction settings to digital outputs
		(1u<<(2*LCD_RS_pin))
		|(1u<<(2*LCD_RW_pin))
		|(1u<<(2*LCD_E_pin))
		|(0x5555<<(2*LCD_D0_pin))
	);


	//LCD INIT COMMANDS
	clr_LCD_RS();					//all lines default low
	clr_LCD_RW();
	clr_LCD_E();
	
	lcd_delayus(25000);		//25ms startup delay
	cmdLCD(0x38);	//Function set: 2 Line, 8-bit, 5x7 dots
	cmdLCD(0x0c);	//Display on, Cursor blinking command
	cmdLCD(0x01);	//Clear LCD
	cmdLCD(0x06);	//Entry mode, auto increment with no shift
}

// Display string on LCD
void LCD_String(const char *str) {
    while (*str) {
        putLCD(*str++);
    }
}

// Display heart rate on LCD (second line)
void LCD_DisplayHeartRate(uint16_t hr) {
    char buffer[17]; // 16 chars + null terminator
    
    // Format string with heart rate
    snprintf(buffer, sizeof(buffer), "HR: %u BPM", hr);
    
    // Position to line 2
    cmdLCD(LCD_LINE2);
    
    // Display the string
    LCD_String(buffer);
}

// Display sample rate on LCD (first line)
void LCD_DisplaySampleRate(uint16_t sr) {
    char buffer[17]; // 16 chars + null terminator
    
    // Format string with sample rate
    snprintf(buffer, sizeof(buffer), "SR: %u Hz", sr);
    
    // Position to line 1
    cmdLCD(LCD_LINE1);
    
    // Display the string
    LCD_String(buffer);
}

// Update both heart rate and sample rate on LCD
void LCD_UpdateDisplay(uint16_t hr, uint16_t sr) {
    // Clear display
    LCD_CLR();
    lcd_delayus(2000); // Short delay after clearing
    
    // Update sample rate on line 1
    LCD_DisplaySampleRate(sr);
    
    // Update heart rate on line 2
    LCD_DisplayHeartRate(hr);
}