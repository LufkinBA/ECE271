//Blake Lufkin
//Lab 8 System timer in C
//3/19/2020
#include "stm32l476xx.h"

//Sets the internal clock to 4Mhz
void System_Clock_Init(void);

void LED_Init(void);

void Toggle_RED_LED(void);

void TIM1_Init(void);

void Pulse_LED(void);

void Servo_control(void);

void Something_cool(void);

int main(void){
	
	System_Clock_Init();
	LED_Init();
	TIM1_Init();
	
	//Set alternate function as TIM1_CH1N
	GPIOE->AFR[0] &= ~(0x0);
	GPIOE->AFR[0] |= (0x0);
	GPIOE->AFR[1] &= ~(0x1);
	GPIOE->AFR[1] |= (0x1);
		
	//Pulse_LED();
	Something_cool();
	//Servo_control();	//if uncommenting be sure to change AAR to 1999
	
	
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

	// MODE: 00: Input mode,              01: General purpose output mode
  //       10: Alternate function mode, 11: Analog mode (reset state)
  GPIOB->MODER &= ~(0x03<<(2*2)) ;   // Clear bit 13 and bit 12
  GPIOB->MODER |= (1<<4);
	
	//Enable the clock to GPIO Port E
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
	
	//Set PE 8 as alternate funtion
	GPIOE->MODER &= ~(0x30000);
	GPIOE->MODER |= 0x20000;
	
	//No Pullup pulldown
	GPIOE->PUPDR &= ~(0x30000);
	GPIOE->PUPDR |= 0x0;
	
	//push pull
	GPIOE->OTYPER &= ~(0x100);
	GPIOE->OTYPER |= 0x0;
	
	
	
}

void Toggle_RED_LED(void)
{
	GPIOB->ODR ^= GPIO_ODR_ODR_2;
}

void System_Clock_Init(void)
{
	// Disable the MSION so the clock frequency can be changed
  RCC->CR &= ~((uint32_t)RCC_CR_MSION);
	
	//Clear the MSI range then set range to 0x6 which enables the 4Mhz clock
	RCC->CR &= ~((uint32_t)RCC_CR_MSIRANGE);
	RCC->CR |= ((uint32_t)RCC_CR_MSIRANGE_6);
	
	//Tell MSI clock to use this new value
	RCC->CR |= ((uint32_t)RCC_CR_MSIRGSEL); 
	
	//re-enable the clock
	RCC->CR |= ((uint32_t)RCC_CR_MSION);
	
  // wait until MSI is ready
  while ( (RCC->CR & (uint32_t) RCC_CR_MSIRDY) == 0 ) {;}
}

void TIM1_Init(void) 
{
	//Enable TIMER 1 clock
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	
	//Counting direction 0=upcount 1=downcount
	TIM1->CR1 &= ~TIM_CR1_DIR;	//select upcount
	
	//Prescaler, slow down the input clock by a factor of (1+prescaler)
	TIM1->PSC = 39;
	
	//Auto-reload
	//TIM1->ARR = 999; 
	TIM1->ARR = 1999; //PWM period = (1999 + 1) * 1/100kHz = 0.02s
	
	//clear output compare mode bits for channel 1
	TIM1->CCMR1 &= ~TIM_CCMR1_OC1M;
	
	//Select PWM mode 1 output on channel 1 (OC1M = 110)
	TIM1->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	
	//Output 1 preload enable 
	TIM1->CCMR1 |= TIM_CCMR1_OC1PE;
	
	//Select output polarity: 0 = Active high, 1 = active low
	TIM1->CCER &= ~TIM_CCER_CC1NP;
	
	//enable complementary output of channel 1(CH1N)
	TIM1->CCER |= TIM_CCER_CC1NE;
	
	//Main output enable (MOE): 0 = Disable, 1 = Enable
	TIM1->BDTR |= TIM_BDTR_MOE;
	
	//Output compare register for channel 1
	TIM1->CCR1 = 500; //initial duty cycle of 50%
	
	//Enable counter
	TIM1->CR1 |= TIM_CR1_CEN;
}
