//Blake Lufkin
//ECE 271 Lab 1(toggling led using center joystick button)
//Clock speed: 16MHz
//Ports Utilized:GPIOA,GPIOB,GPIOE 
//Last updated 1/29/2020
#include "stm32l476xx.h"

int main(void){
		
	uint32_t input, input2, input3;

	// Enable High Speed Internal Clock (HSI = 16 MHz)
  RCC->CR |= ((uint32_t)RCC_CR_HSION);
	
  // wait until HSI is ready
  while ( (RCC->CR & (uint32_t) RCC_CR_HSIRDY) == 0 ) {;}
	
  // Select HSI as system clock source 
  RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
  RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSI;     // 01: HSI16 oscillator used as system clock

  // Wait till HSI is used as system clock source 
  while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) == 0 ) {;}
  
  // Enable the clock to GPIO Port B	
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;   
		
		// Enable the clock to GPIO Port A	
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;   
		
		// Enable the clock to GPIO Port E
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;   
	
	// MODE: 00: Input mode,              01: General purpose output mode
  //       10: Alternate function mode, 11: Analog mode (reset state)
  GPIOB->MODER &= ~(0x30) ;   // Clear bit 4 and bit 5
  GPIOB->MODER |= (1<<4);
		
	GPIOB->OTYPER &= ~(1UL<<2);	//Select push pull output
		
	GPIOE->MODER &= ~(0x30000); //Clear bit 16 and 17
	GPIOE->MODER |= (1<<16);
		
	GPIOE->OTYPER &= ~(1UL<<8);	//Select push pull output
		
	GPIOA->MODER &=~(0xCFF); //clear bits 11,10,7,6,5,4,3,2,1,0
	
	GPIOA->PUPDR &=~(0xCFF);	//no pull up/pull down mask
	GPIOA->PUPDR |=(0x8AA);		//no pull up/pull down value
	

	while (1){
		input = (GPIOA->IDR & 1UL);
		input2 = (GPIOA->IDR & 1UL<<5); //Red light
		input3 = (GPIOA->IDR & 1UL<<3);	//Green Light
	
				if (input != 0) {
				
					GPIOB->ODR ^= GPIO_ODR_ODR_2;
		
					GPIOE->ODR ^= GPIO_ODR_ODR_8;
					while(input==1)
					{
						input = (GPIOA->IDR & 1UL);
					//do nothing
					}
					}
				if (input2 != 0){
					GPIOB->ODR ^= GPIO_ODR_ODR_2;
					while((GPIOA->IDR & 1UL<<5)==1)
					{
					//do nothing
					}
				}
				if (input3 != 0){
					GPIOE->ODR ^= GPIO_ODR_ODR_8;
					while(input3==1)
					{
						input3 = (GPIOA->IDR & 1UL<<3);
					//do nothing
					}
				}
	}
	
	
}
