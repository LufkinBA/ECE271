#include "stm32l476xx.h"
#include "SysClock.h"
#include "LED.h"

extern float max(float a, float b, float c);
volatile float d;


int main(void){

	System_Clock_Init(); // Switch System Clock = 80 MHz
	LED_Init();
	
  d = max(1.11, 2.22, -3.33); 
  d = max(1.11, -3.33, 2.22);
  d = max(-3.33, 2.22, 1.11);
  d = max(-3.33, 1.11, 2.22);
  d = max(2.22, -3.33, 1.11);
	d = max(2.22, 1.11, -3.33);

	while(1);
}

