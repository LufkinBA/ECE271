//Blake Lufkin
//Lab 7 System timer in C
//3/19/2020
#include "stm32l476xx.h"

//Sets the internal clock to 8Mhz
void System_Clock_Init(void);

//Sets the internal clock to 16Mhz
void System_Clock_InitHSI(void);

//initializes the SysTick counter
void SysTick_Initialize(int ticks);

//Interrupt handler 
void SysTick_Handler(void);

//Delay function that takes in nanoseconds
void Delay(uint32_t nTime);

void LED_Init(void);

void Toggle_RED_LED(void);

//Declaring a global TimeDelay variable 
int TimeDelay; 

int main(void){
	System_Clock_Init();
//	System_Clock_InitHSI();
	LED_Init();
//	SysTick_Initialize(15999);
  SysTick_Initialize(7999);
	while(1) {
		Delay(1000);
		Toggle_RED_LED();
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
}

void Toggle_RED_LED(void)
{
	GPIOB->ODR ^= GPIO_ODR_ODR_2;
}

void System_Clock_Init(void)
{
	// Disable the MSION so the clock frequency can be changed
  RCC->CR &= ~((uint32_t)RCC_CR_MSION);
	
	//Clear the MSI range then set range to 0x7 which enables the 8Mhz clock
	RCC->CR &= ~((uint32_t)RCC_CR_MSIRANGE);
	RCC->CR |= ((uint32_t)RCC_CR_MSIRANGE_7);
	
	//Tell MSI clock to use this new value
	RCC->CR |= ((uint32_t)RCC_CR_MSIRGSEL); 
	
	//re-enable the clock
	RCC->CR |= ((uint32_t)RCC_CR_MSION);
	
  // wait until MSI is ready
  while ( (RCC->CR & (uint32_t) RCC_CR_MSIRDY) == 0 ) {;}
	
}

void System_Clock_InitHSI(void)
{
	// Enable High Speed Internal Clock (HSI = 16 MHz)
  RCC->CR |= ((uint32_t)RCC_CR_HSION);
	
  // wait until HSI is ready
  while ( (RCC->CR & (uint32_t) RCC_CR_HSIRDY) == 0 ) {;}
	
  // Select HSI as system clock source 
  RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
  RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSI;     // 01: HSI16 oscillator used as system clock

  // Wait till HSI is used as system clock source 
  while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) == 0 ) {;}
}

void SysTick_Initialize(int ticks)
{
	//Disable the systick counter
	SysTick->CTRL = 0;
	
	//Set the value of the reload register to ticks - 1
	SysTick->LOAD = ticks - 1;
	
	//Set interrupt priority of SysTick
	//Make SysTick least urgent (i.e., highest priority number)
	//__NVIC_PRIO_BITS: number of bits for priority levels, defined in CMSIS
	NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
	
	//Reset the count 
	SysTick->VAL = 0;
	
	//Set CTRL to use the internal processor clock rather than the external clock 
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	
	//Enables SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	//Enable SysTick timer
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void)
{
	//decrements TimeDelay if its greater than zero
	if (TimeDelay > 0) {
		TimeDelay--; 
	}
}

void Delay(uint32_t nTime)
{
	TimeDelay = nTime;
	while(TimeDelay != 0);
}
