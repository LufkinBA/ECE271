//Blake Lufkin
//Lab 10
//4/23/2020
#include "stm32l476xx.h"
#include "lcd.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//Sets the internal clock to 4Mhz
void System_Clock_Init(void);

void LED_Init(void);

void Toggle_RED_LED(void);

void TIM1_Init(void);

void TIM4_Init(void);

void Pulse_LED(void);

void Servo_control(void);

void Something_cool(void);

void TIM4_IRQHandler(void);

void ADC1_Wakeup(void);

void ADC_init(void);

volatile uint32_t pulse_width = 0;
volatile uint32_t last_captured = 0;
volatile uint32_t signal_polarity = 0;
volatile uint32_t overflow_count = 0;
volatile int distance = 0;
volatile uint32_t converted_value = 0; 
int main(void){
	
	char buffer[6];
	float voltage;
	LCD_Initialization();
	LCD_Clear();
	System_Clock_Init();
	ADC_init();

	//Enable GPIOA clock
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;  
	
	//Enable GPIOB clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;  
	
	//Set pin 1 to be analog 0b11
	GPIOA->MODER &= ~0xC;
	GPIOA->MODER |= 0xC;
	
	//Hook up analog pin to ADC
	GPIOA->ASCR &= ~0x2;
	GPIOA->ASCR = 0x2;
	
	// MODE: 00: Input mode,              01: General purpose output mode
  //       10: Alternate function mode, 11: Analog mode (reset state)
 // GPIOB->MODER &= ~(0x03<<(2*2)) ;   // Clear bit 13 and bit 12
 // GPIOB->MODER |= (1<<4);
	
		//push pull for PB2
	//GPIOB->OTYPER &= ~(0x2);
	//GPIOB->OTYPER |= 0x0;
	LCD_Initialization();
	LCD_Clear();
	System_Clock_Init();
	ADC_init();

	
	while (1) {
		ADC1->CR |= ADC_CR_ADSTART;
		
		while ((ADC123_COMMON->CSR & ADC_CSR_EOC_MST) == 0);
		
		converted_value = ADC1->DR;
		
		voltage = (converted_value/4096.) * 3;
		
				//convert distance int to a string
		snprintf(buffer, 6, "%g", voltage); 
		
		//buffer stores the string value for distance
		LCD_DisplayString((uint8_t*)buffer,6);
	}

}
void ADC_init(void) {
	
	//Enable ADC clock bit
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;
	
	//Disable ADC1
	ADC1->CR |= ADC_CR_ADEN;

	//Enable I/O analog switches voltage booster
	SYSCFG->CFGR1 |= SYSCFG_CFGR1_BOOSTEN;
	
	//Enable conversion of internal channels
	ADC123_COMMON->CCR |= ADC_CCR_VREFEN;
	
	//Configure adc prescaler not divided
	ADC123_COMMON->CCR &= ~ADC_CCR_PRESC;
	
	//Select synchronous clock mode 0b01
	ADC123_COMMON->CCR |= ADC_CCR_CKMODE_0;
	
	//configure all ADCs as independent by clearing ADC_CCR_DUAL
	ADC123_COMMON->CCR &= ~ADC_CCR_DUAL;
	
	//Wake up adc
	ADC1_Wakeup();
	
	//Configure ADC to have 12-bit resolution 0b00
	ADC1->CFGR &= ~ADC_CFGR_RES;
	
	//set right-alignment of the 12 bit result
	ADC1->CFGR &= ~ADC_CFGR_ALIGN;
	
	//select 1 conversion in the regular channel conversion sequence
	ADC1->SQR1 &= ~ADC_SQR1_L;
	
	//specify the channel number of the 1st conversion in regular sequence
	ADC1->SQR1 &= ~ADC_SQR1_SQ1;
	ADC1->SQR1 |= (6U << 6);
	ADC1->DIFSEL &= ~ADC_DIFSEL_DIFSEL_6;
	
	//Select ADC sampling time??? set to 1
	ADC1->SMPR1 |= ADC_SMPR1_SMP6_0;
	
	//select adc to disontinuous mode by clearing ADC_CFGR_CONT
	ADC1->CFGR &= ~ADC_CFGR_CONT;
	
	//Select software trigger by clearing ADC_CFGR_EXTEN
	ADC1->CFGR &= ~ADC_CFGR_EXTEN;
	
	//enable ADC1 by setting ADC_CR_ADEN 
	ADC1->CR |= ADC_CR_ADEN;
	
	while ((ADC1->ISR & ADC_ISR_ADRDY) == 0);
}

void ADC1_Wakeup(void) {
	int wait_time;
	
	//Set ADC to not in deep power down
	if ((ADC1->CR & ADC_CR_DEEPPWD) == ADC_CR_DEEPPWD) {
		ADC1->CR &= ~ADC_CR_DEEPPWD;
	}
	
	//Enable ADC internal voltage regulator
	ADC1->CR |= ADC_CR_ADVREGEN;
	
	//wait for adc voltage regulator start-up time
	wait_time = 20 * (80000000/1000000);
	while (wait_time != 0) {
		wait_time--;
	}
}

void Something_cool(void)
{
	int i;
	int brightness = 1; 
	int bs = 50;
	// Enable the clock to GPIO Port A	
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;   
	
	GPIOA->MODER &=~(0xCFF); //clear bits 11,10,7,6,5,4,3,2,1,0
	GPIOA->MODER |= 0x0;
	
	GPIOA->PUPDR &=~(0xCFF);	//no pull up/pull down mask
	GPIOA->PUPDR |=(0x8AA);		//no pull up/pull down value
	
	while (1) {
		
		if ((GPIOA->IDR & GPIO_IDR_IDR_3)!=0)	{
			if (brightness<= 949)	{
				brightness= brightness+bs;
			}	else {
				brightness = 999;
			}
			for(i=0;i<50000;i++);
		}
		
		if ((GPIOA->IDR & GPIO_IDR_IDR_5)!=0)	{
			if (brightness >= 51){
				brightness = brightness-bs;
			}	else	{
				brightness = 1; 
			}
			for(i=0;i<50000;i++);
		}
		
		TIM1->CCR1 = brightness;
		
	}
}

void Servo_control(void)
{
	int i;
	while (1) {
	TIM1->CCR1 = 150;
	
	for (i=0;i<1000000;i++);
		
	TIM1->CCR1 = 50;
	
	for (i=0;i<1000000;i++);
	
	TIM1->CCR1 = 250;

	for (i=0;i<1000000;i++);
		
	TIM1->CCR1 = 150;
	
	for (i=0;i<1000000;i++);
	}
}
void Pulse_LED(void)
{
	int i;
	int brightness = 1;
	int stepSize = 1;
	while(1) {
		if ((brightness >= 1999) || (brightness <=0))
			stepSize = -stepSize;
		
		brightness += stepSize;
		
		TIM1->CCR1 = brightness;
		
		for(i=0; i<1000; i++);
	}
}
void LED_Init (void)
{
	// Enable the clock to GPIO Port B	
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;   

	
	//Enable the clock to GPIO Port E
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
	
	//Set Pb 6 as alternate funtion
	GPIOB->MODER &= ~(0x3000);
	GPIOB->MODER |= 0x2000;
	
	//No Pullup pulldown
	GPIOB->PUPDR &= ~(0x3000);
	GPIOB->PUPDR |= 0x0;
	
	//push pull
	GPIOB->OTYPER &= ~(0x20);
	GPIOB->OTYPER |= 0x0;
	
	//Set PE 11 as alternate funtion
	GPIOE->MODER &= ~(0xC00000);
	GPIOE->MODER |= 0x800000; 
	
}

void Toggle_RED_LED(void)
{
	GPIOB->ODR ^= GPIO_ODR_ODR_2;
}

void System_Clock_Init(void)
{
	//enable the clock
	RCC->CR |= ((uint32_t)RCC_CR_HSION);
	
  // wait until HSI is ready
  while ( (RCC->CR & (uint32_t) RCC_CR_HSIRDY) == 0 ) {;}
	
	//Select HSI as system clock source by setting the SW field in RCC->CFGR
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_HSI;
}

void TIM1_Init(void) 
{
	//Enable TIMER 1 clock
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	
	//Counting direction 0=upcount 1=downcount
	TIM1->CR1 &= ~TIM_CR1_DIR;	//select upcount
	
	//Prescaler, slow down the input clock by a factor of (1+prescaler)
	TIM1->PSC = 15;
	
	//Auto-reload
	TIM1->ARR = 0xFFFF; //PWM period = (1999 + 1) * 1/100kHz = 0.02s
	
	//clear output compare mode bits for channel 2
	TIM1->CCMR1 &= ~TIM_CCMR1_OC2M;
	
	//Select PWM mode 1 output on channel 2 (OC3M = 110)
	TIM1->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;
	
	//Output 1 preload enable 
	TIM1->CCMR1 |= TIM_CCMR1_OC2PE;
	
	//Select output polarity: 0 = Active high, 1 = active low
	TIM1->CCER &= ~TIM_CCER_CC2P;
	
	//enable complementary output of channel 2(CH2)
	TIM1->CCER |= TIM_CCER_CC2E;
	
	//Main output enable (MOE): 0 = Disable, 1 = Enable
	TIM1->BDTR |= TIM_BDTR_MOE;
	
	//Output compare register for channel 2
	TIM1->CCR2 = 10; //for 10us pulse width
	
	//Enable counter
	TIM1->CR1 |= TIM_CR1_CEN;
}

void TIM4_Init(void)
{
	//Enable clock to Timer 4
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;
	
	//Prescaler set to divide the 16MHz clock to 1MHz
	TIM4->PSC = 15;
	
	//Set Auto-reload value to maximum value
	TIM4->ARR = 0xFFFF;
	
	//Set the direction of channel 1 as input, and select active input
	TIM4->CCMR1 &= ~TIM_CCMR1_CC1S;
	TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;
	
	//Program the input filer duration: Disable digital filering by clearing
	//IC1F[3:0] bits because we want to capture every event
	TIM4->CCMR1 &= ~TIM_CCMR1_IC1F;
	
	//Set the active transistion as both rising and falling edges
	//CC1NP:CC1P bits:	00=rising edge		01=falling edge
	//									10=reserved 			11=both edges
	TIM4->CCER |=TIM_CCER_CC1P|TIM_CCER_CC1NP;
	
	//Program the input prescaler: Clear prescaler to capture each transition
	TIM4->CCMR1 &= ~(TIM_CCMR1_IC1PSC);
	
	//Enable capture for channel 1
	TIM4->CCER |= TIM_CCER_CC1E;
	
	//Allow channel 1 of timer 4 to generate interrupts
	TIM4->DIER |= TIM_DIER_CC1IE;
	
	//Enable the overflow interrupt
	TIM4->DIER |= TIM_DIER_UIE;
	
	//enable the timer counter
	TIM4->CR1 |= TIM_CR1_CEN;
	
	//Set priority of timer 4 interrupt to 0
	NVIC_SetPriority(TIM4_IRQn, 0);
	
	//Enable timer 4 interrupt in the interrupt controller (NVIC)
	NVIC_EnableIRQ(TIM4_IRQn);
}

void TIM4_IRQHandler(void)
{
	uint32_t current_captured;
	
	if ((TIM4->SR & TIM_SR_CC1IF) != 0){
		
		//Reading CCR1 clars CC1IF interrupt flag
		current_captured = TIM4->CCR1;
		
		//Toggle the polarity flag
		signal_polarity= 1 - signal_polarity;
		
		if(signal_polarity == 0){
			pulse_width = (current_captured - last_captured) + (overflow_count * 0xFFFF);
		}
		last_captured = current_captured;
	}
	
	if ((TIM4->SR & TIM_SR_UIF) !=0) {
		TIM4->SR &= ~TIM_SR_UIF;
		overflow_count++;
	}
	if (signal_polarity == 1)
	{
		overflow_count=0;
	}
	distance = pulse_width/58;
}
