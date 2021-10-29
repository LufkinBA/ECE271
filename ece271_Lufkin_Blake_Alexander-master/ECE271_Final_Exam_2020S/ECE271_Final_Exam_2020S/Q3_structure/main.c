#include "stm32l476xx.h"
#include "SysClock.h"
#include "LED.h"

struct Student_T {
 char c1;
 char c2;
 int score;
 char c3;
} students[10];

extern int find_avg_score(struct Student_T *p, int num_of_students);

void init_structure(){
	uint32_t i;
	for (i = 0; i < 10; i++){
		students[i].score = i * 10;
	}
}

volatile int avg;
	
int main(void){

	System_Clock_Init(); // Switch System Clock = 80 MHz
	LED_Init();
	
	init_structure();
	
	avg = find_avg_score(&students[0], 10);

	while(1);
}

