#include <stm32f429xx.h>

//Variables
volatile unsigned int j;							//declare a volatile unsigned integer i
volatile int x;												//declare a volatile integer x
volatile unsigned int tick = 0;				//declare a volatile unsigned integer named tick and initialise it as 0
unsigned int press_start = 0;					//declare an unsigned integer named press_start and initialise it as 0
unsigned int press_duration = 0;			//declare an unsigned integer named press_duration and initialise it as 0
int button_was_pressed = 0;						//declare an integer named button_was_pressed and initialise it as 0


//Clock
//Only enable GPIOB clock
#define enable_clockB()			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
//Only enable GPIOC clock
#define enable_clockC()			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;


//Buttons
// Configure user button (PC13) as an input
#define read_userBTN()			GPIOC->MODER &= ~(0x03 << (2*13));



//LEDs
#define LED1_ON()			GPIOB->ODR |= (0x01 << 0);							//Only turn LED1 on
#define LED1_OFF()			GPIOB->ODR &= ~(0x01 << 0);							//Only turn LED1 off
#define LED2_ON()			GPIOB->ODR |= (0x01 << 7);							//Only turn LED2 on
#define LED2_OFF()			GPIOB->ODR &= ~(0x01 << 7);							//Only turn LED2 off
#define set_LED1()			GPIOB->MODER |= (0x01 << (2*0));			//Only set GPIOB bit 0
#define clr_LED1()			GPIOB->MODER &= ~(0x02 << (2*0));			//Only clear GPIOB bit 1
#define set_LED2()			GPIOB->MODER |= (0x01 << (2*7));			//Only set GPIOB bit 14
#define clr_LED2()			GPIOB->MODER &= ~(0x02 << (2*7));			//Only clear GPIOB bit 15
