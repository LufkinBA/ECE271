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
	const int halfs = 4096;
	const int fulls = 2048;
	unsigned char key;
	char str[50];
	char halfstr[6];
	char fullstr[6];
	unsigned char len=0;
	int j, i, k; 
	unsigned char FullStep[4] = {0x9, 0xA, 0x6, 0x5};
	unsigned char HalfStep[8] = {0x9, 0x8, 0xa, 0x2, 0x6, 0x4, 0x5, 0x1};
	double angle_per_half, angle_per_full, temp; 
	
	System_Clock_Init();
	LCD_Initialization();
	LCD_Clear();

	  // Enable the clock to GPIO Port B	
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;   

	// Enable the clock to GPIO Port A	
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

	GPIOA->MODER &= ~(0xCFF); //clear bits 11,10,7,6,5,4,3,2,1,0

	GPIOA->PUPDR &=~(0xCFF);	//no pull up/pull down mask
	GPIOA->PUPDR |=(0x8AA);		//no pull up/pull down value
	
	GPIOA->IDR |= 0x4F; 	//Initialize joystick input data register
		
	// MODE: 00: Input mode,              01: General purpose output mode
  //       10: Alternate function mode, 11: Analog mode (reset state)
  GPIOB->MODER &= ~(0x0000F0F0) ;   // Clear bit 2, 3, 6, 7
  GPIOB->MODER |= (0x00005050);
		
	GPIOB->OTYPER &= ~(0xCC);				//push-pull
	GPIOB->PUPDR &= ~(0xF0F0);
	GPIOB->ODR |= GPIO_ODR_ODR_2;
	
	angle_per_half = 360./halfs; 
	angle_per_full = 360./fulls; 
	
	while (1) {
			if (GPIOA->IDR & GPIO_IDR_ID0) {
				for (j=0;j<512;j++) {
					for (i=0;i<4;i++) 	{
						GPIOB->ODR &= ~(GPIO_ODR_ODR_2);
						GPIOB->ODR &= ~(GPIO_ODR_ODR_3);
						GPIOB->ODR &= ~(GPIO_ODR_ODR_6);
						GPIOB->ODR &= ~(GPIO_ODR_ODR_7);
						//For A shifting to PB2
						GPIOB->ODR |= ((FullStep[i] & 0x8) >> 3) << 2; 
						//For A! shifting to PB3
						GPIOB->ODR |= ((FullStep[i] & 0x4) >> 2) << 3; 
						//For B shifting to PB6
						GPIOB->ODR |= ((FullStep[i] & 0x2) >> 1) << 6; 
						//For B! shifting to PB7
						GPIOB->ODR |= ((FullStep[i] & 0x1)) << 7;
						for (k=0;k<6000;k++);
				}
					//Outputs the current angle to the LCD
				temp = (j+1) * 4 * angle_per_full;
				snprintf(fullstr, 6, "%f", temp);
				LCD_DisplayString(fullstr, 6);
			}
		}
			if (GPIOA->IDR & GPIO_IDR_ID1) {
				for (j=0;j<512;j++) {
					for (i=0;i<8;i++)	{
						GPIOB->ODR &= ~(GPIO_ODR_ODR_2);
						GPIOB->ODR &= ~(GPIO_ODR_ODR_3);
						GPIOB->ODR &= ~(GPIO_ODR_ODR_6);
						GPIOB->ODR &= ~(GPIO_ODR_ODR_7);
						//For A shifting to PB2
						GPIOB->ODR |= ((HalfStep[i] & 0x8) >> 3) << 2; 
						//For A! shifting to PB3
						GPIOB->ODR |= ((HalfStep[i] & 0x4) >> 2) << 3; 
						//For B shifting to PB6
						GPIOB->ODR |= ((HalfStep[i] & 0x2) >> 1) << 6; 
						//For B! shifting to PB7
						GPIOB->ODR |= ((HalfStep[i] & 0x1)) << 7;
						for (k=0;k<3100;k++);
			}
					//Outputs the current angle to the LCD
				temp = (j+1) * 8 * angle_per_half;
				snprintf(halfstr, 6, "%f", temp);
				LCD_DisplayString(halfstr, 6);
		}
	}
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

