#include "ADC.h"
#include "DELAY.h"
//ADC.h containing declarations and #defines with suitable comments describing them	1		
//ADC.c containing structured routines and comments describing the algorithm (>50% of code commented)	1		
//Initialise ADC to Sampling 12bit, 1Ksps (input 0-3v) [1] using Timer [1]	2		
//ADC sampling POT output voltage	2		
//ADC sampling LDR output voltage	2		
//ADC sampling MIC output voltage	2		



void ADC_LED_Init(void)
{
	

		//ENABLE PORT(S)
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOCEN;		//GPIO C clock enable
	
	//CONFIGURE PORT PIN FUNCTIONS
	GPIOC->MODER&=~(3u<<(2*2));						//clear pin functions on GPIOC
	GPIOC->MODER|=(1u<<(3*2));			//set new pin functions on GPIOC
	GPIOC->MODER|=(1u<<(6*2));
	GPIOC->MODER|=(1u<<(2*2));
}	

//TIMER FOR ADC
void TIM3_Init(void) {
    // Enable the Timer3 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // Set the prescaler to achieve a 1 kHz update rate
    TIM3->PSC = 8400 - 1; // 84 MHz / 8400 = 10 kHz
    TIM3->ARR = 10 - 1;   // 10 kHz / 10 = 1 kHz

    // Configure Timer3 to trigger the ADC
    TIM3->CR2 |= TIM_CR2_MMS_1; // Update event as TRGO

    // Enable Timer3
    TIM3->CR1 |= TIM_CR1_CEN;
}


void init_ADC(void)
{
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOCEN; //|=RCC_AHB1ENR_GPIOAEN;	//GPIOC clock enable
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;
	ADC_LDR_port->MODER|=(3u<<(2*ADC_LDR_pin));	//ADC input pin is analogue mode
	ADC_POT_port->MODER|=(3u<<(2*ADC_POT_pin));
	ADC_MIC_port->MODER|=(3u<<(2*ADC_MIC_pin));
	
	
	RCC->APB2ENR|=RCC_APB2ENR_ADC1EN;		//ADC clock enable
	// 3. Configure ADC
  ADC1->CR2 = 0;  // Disable ADC before configuration
  ADC1->SQR1 = 0; // Clear sequence register
	
	ADC1->SQR1 |= (2 << 20);						//set number of conversions per sequence to 3
	
	ADC1->SQR3 |= (ADC_POT_Channel << 0);  // First conversion: channel POT (ADC1_IN0)
  ADC1->SQR3 |= (ADC_LDR_Channel << 5);	 //Second conversion: channel LDR (ADC1_IN10)		
	ADC1->SQR3 |= (ADC_MIC_Channel << 10);
	
	
	ADC1->SQR3|=ADC_LDR_Channel;						//set channel
	
	ADC1->CR2|=ADC_CR2_ADON;						//enable ADC
	
}

void Control_LEDs(uint16_t LDR_value, uint16_t POT_value, uint16_t MIC_value) {
    // 12. Map ADC values to LED states
    // Turn on LED if ADC value is above a threshold (e.g., 2048 for 12-bit ADC)
                                    
   
   if (LDR_value > 2048) GPIOC->ODR^=(1u<<TRAF_YEL1_PIN);  // Turn on TRAF_YEL1 (PC3)
   else GPIOC->ODR&=~(1u<<TRAF_YEL1_PIN);                   // Turn off TRAF_YEL1 (PC3)

   if (POT_value > 2048) GPIOC->ODR^=(1u<<TRAF_RED1_PIN);  
   else GPIOC->ODR&=~(1u<<TRAF_RED1_PIN);                              
	  
	 if (MIC_value > 2048) GPIOC->ODR^=(1u<<TRAF_GRN1_PIN);  
   else GPIOC->ODR&=~(1u<<TRAF_GRN1_PIN); 
   
}


void CONTROL_LED_FUNCTION (void)
{
	while(1)
	{
		 TIM3->SR&=~TIM_SR_UIF;
		    uint16_t LDR_value = read_adc(0);
			  uint16_t POT_value = read_adc(10);
	    	uint16_t MIC_value = read_adc(3);
			  Control_LEDs(LDR_value, POT_value, MIC_value);
	}
}





uint16_t read_adc(uint16_t channel)
{
	
	TIM3->SR&=~TIM_SR_UIF;				
  // Start conversion
    ADC1->CR2 |= ADC_CR2_SWSTART;

    // Wait for conversion to complete
    while (!(ADC1->SR & ADC_SR_EOC));

    // Read the result
    return ADC1->DR;									//return converted value
}




	
	
		



