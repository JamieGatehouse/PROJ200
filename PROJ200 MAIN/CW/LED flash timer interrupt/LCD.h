#ifndef _LCD_H_
#define _LCD_H_
#define LCD_PORT	GPIOD
#define LCD_RS_pin	11 // select command (0) or data register (1)
#define LCD_RW_pin	12 // set write (0) or read (1)
#define LCD_E_pin		13 // strobe pin

#define LCD_D0_pin	0 // data pin



#define LCD_LINE1		0x80
#define LCD_LINE2		0xc0

#define set_LCD_RS()	LCD_PORT->BSRR=(1u<<LCD_RS_pin)
#define clr_LCD_RS()	LCD_PORT->BSRR=(1u<<(LCD_RS_pin+16))
#define set_LCD_RW()	LCD_PORT->BSRR=(1u<<LCD_RW_pin)
#define clr_LCD_RW()	LCD_PORT->BSRR=(1u<<(LCD_RW_pin+16))
#define set_LCD_E()		LCD_PORT->BSRR=(1u<<LCD_E_pin)
#define clr_LCD_E()		LCD_PORT->BSRR=(1u<<(LCD_E_pin+16))

#define LCD_CLR()		cmdLCD(0x01)

#define set_LCD_bus_input()		LCD_PORT->MODER&=~(0xffff<<(2*LCD_D0_pin))
#define set_LCD_bus_output()	LCD_PORT->MODER|=(0x5555<<(2*LCD_D0_pin))

#include <stm32f4xx.h>

//blocking delay for LCD
void lcd_delayus(unsigned int us);

// 3ms wait
void WaitLcdBusy(void);

// clear and send char to LCD display
void set_LCD_data(unsigned char d);

//10us high pulse on LCD enable line ( square wave )
void LCD_strobe(void);

//sends a byte to the LCD control register
void cmdLCD(unsigned char cmd);

//sends a char to the LCD display
void putLCD(unsigned char put);

// Read the busy flag from LED unit
unsigned char read_LCD_busy_flag(void);

// turn on the led display
void initLCD(void);

void scrollTextA(const char *message);

#endif
