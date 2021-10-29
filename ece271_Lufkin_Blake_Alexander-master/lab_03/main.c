//Blake Lufkin 
//2/13/2020
//Lab 3
//Getting Keypad to display character pressed

#include "stm32l476xx.h"
#include "lcd.h"

void System_Clock_Init(void);
void Keypad_Pin_Init(void);
unsigned char keypad_scan(void);

int main(void){
	unsigned char key;
	char str[50];
	unsigned char len=0;
	
	System_Clock_Init();
	LCD_Initialization();
	LCD_Clear();
	Keypad_Pin_Init();
	
	while (1)
	{
		key=keypad_scan();
		
		switch (key) {
			case '*':
				len=0; 
				LCD_Clear();
				while (key!=0xFF)
				{
					key=keypad_scan();
				}
				break;
			case '#':
				len--;
				LCD_Clear();
				while (key!=0xFF)
				{
					key=keypad_scan();
				}
				break;
			case 0xFF:
				break;
			default:
				str[len]=key;
				str[len+1]=0;
				len++;
				if(len>=48) len=0;
				while (key!=0xFF)
				{
					key=keypad_scan();
				}
		}
			LCD_DisplayString((uint8_t*)str, len);
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
void Keypad_Pin_Init(void) {
// Enable the clock to GPIO Port A
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;   
		
		// Enable the clock to GPIO Port E	
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;

	GPIOA->MODER &= ~(0xCFC) ;   // Clear bit 4 and bit 5
  GPIOA->MODER |= (0x000);	
	
	GPIOE->MODER &= ~(0x0FF00000) ;   // Clear bit 4 and bit 5
  GPIOE->MODER |= (0x05500000);	
}

unsigned char keypad_scan(void){

	unsigned char row, col, ColumnPressed;
	unsigned char key = 0xFF;
	int colpins[4]={1,2,3,5};
	int rowpins[4]={10,11,12,13};
	int pin;
	int i;
	unsigned char Column[4]={'W','X','Y','Z'};
	unsigned char key_map[4][4]={
	{'1','2','3','A'},
	{'4','5','6','B'},
  {'7','8','9','C'},
	{'*','0','#','D'},
};
	
	
	//CHeck wether any key has been pressed
	//1. Output zeros on all row pins
	//2. Delay shortly, and read inputs of column pins
	//3. If inputs are 1 for all columns, then no key has been pressed
	GPIOE->ODR &= ~GPIO_ODR_ODR_10;
	GPIOE->ODR &= ~GPIO_ODR_ODR_11;
	GPIOE->ODR &= ~GPIO_ODR_ODR_12;
	GPIOE->ODR &= ~GPIO_ODR_ODR_13;
	
	for (i=0;i<500;i++);
	
	if ((GPIOA->IDR & ((1UL<<1)|(1UL<<2)|(1UL<<3)|(1UL<<5)))==((1UL<<1)|(1UL<<2)|(1UL<<3)|(1UL<<5))){
		return 0xFF;
	}
	
	for (col=0;col<4;col++)
	{
		if ((GPIOA->IDR & 1UL<<colpins[col]) == 0)
		{
			ColumnPressed=col;
		}
	}
	
	for (row=0;row<4;row++)
	{
		GPIOE->ODR |= (1UL<<(rowpins[row]-1));
		
		for (i=0;i<500;i++);
		
		for (col=0;col<4;col++)
		{
			if ((GPIOA->IDR & 1UL<<colpins[col])==0)
			{
				key=key_map[row][ColumnPressed];
			}
		}
	}
return key;
}

