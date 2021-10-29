//Blake Lufkin
//ECE 271
//Displaying to the LCD
#include "stm32l476xx.h"
#include "lcd.h"

void System_Clock_Init(void);

int main(void){
	LCD_Initialization();
	LCD_Clear();
	while (1){

		if ((GPIOA->IDR & 1UL)!=0){
			LCD_Clear();
			LCD_DisplayString((uint8_t*)"ECE271");
		}
			else
			{
				LCD_Clear();
				LCD_Display_Name();
			}
		if ((GPIOA->IDR & 1UL<<5)!=0)
		{
			LCD_Clear();
			LCD_DisplayStringlong((uint8_t*)"   ZACH IS A WIENER   ");
		}
	LCD_bar();
	}
}

void System_Clock_Init(void){
	
	// Enable High Speed Internal Clock (HSI = 16 MHz)
  RCC->CR |= ((uint32_t)RCC_CR_HSION);
	
  // wait until HSI is ready
  while ( (RCC->CR & (uint32_t) RCC_CR_HSIRDY) == 0 );
	
  // Select HSI as system clock source 
  RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
  RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSI;  //01: HSI16 oscillator used as system clock

  // Wait till HSI is used as system clock source 
  while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) == 0 );
		
}

