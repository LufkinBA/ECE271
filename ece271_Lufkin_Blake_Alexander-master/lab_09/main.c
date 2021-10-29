//Blake Lufkin
//Lab 9 Timer Input Capture in C
//4/14/2020
#include "stm32l476xx.h"
#include "lcd.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//Sets the internal clock to 16Mhz
void System_Clock_Init(void);

void LED_Init(void);

void Toggle_RED_LED(void);

void TIM1_Init(void);

void TIM4_Init(void);

void Pulse_LED(void);

void Servo_control(void);

void Something_cool(void);

void TIM4_IRQHandler(void);

volatile uint32_t pulse_width = 0;
volatile uint32_t last_captured = 0;
volatile uint32_t signal_polarity = 0;
volatile uint32_t overflow_count = 0;
volatile int distance = 0;
int main(void){
	
	char buffer[3];

	System_Clock_Init();
	LED_Init();
	TIM4_Init();
	TIM1_Init();

	
	//Set alternate function as TIM4_CH1
	GPIOB->AFR[0] &= ~(0x0F000000);
	GPIOB->AFR[0] |= (0x2000000);
	GPIOB->AFR[1] &= ~(0xFFFFFFFF);
	GPIOB->AFR[1] |= (0x0);
		
	//Set alternate function of GPIOE PE11 to TIM1_CH2
	GPIOE->AFR[0] &= ~(0xFFFFFFFF);
	GPIOE->AFR[1] &= ~(0xFFFFFFFF);
	GPIOE->AFR[1] |= 0x1000;
	
	LCD_Initialization();
	LCD_Clear();
	
 	while(1) {
		//convert distance int to a string
		snprintf(buffer, 3, "%d", distance); 
		
		//buffer stores the string value for distance
		LCD_DisplayString((uint8_t*)buffer,6);
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
