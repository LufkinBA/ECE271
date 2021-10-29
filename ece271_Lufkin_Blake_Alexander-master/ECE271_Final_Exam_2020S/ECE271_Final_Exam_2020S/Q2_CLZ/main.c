#include "stm32l476xx.h"
#include "SysClock.h"
#include "LED.h"


extern uint32_t count_leading_zeros(uint64_t x);

int main(void){

	volatile  uint32_t n;
	uint64_t x;
	
	x = 0ULL;   // ULL = Unsigned Long Long
	n = count_leading_zeros(x);  // n should be 64
	
	x = 3ULL;
	n = count_leading_zeros(x);  // n should be 62
	
	x = 0x12345678ULL;
	n = count_leading_zeros(x);  // n should be 35
	
	x = 0x876543210000ULL;
	n = count_leading_zeros(x);  // n should be 16

	x = 0x1234567812345678ULL;
	n = count_leading_zeros(x);  // n should be 3
	
	while(1);
}

