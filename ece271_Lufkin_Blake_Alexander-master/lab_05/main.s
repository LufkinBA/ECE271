;Blake Lufkin
;Lab 5
;2/26/2020
	INCLUDE core_cm4_constants.s		; Load Cortex-M4 Definitions
	INCLUDE stm32l476xx_constants.s     ; Load STM32L4 Definitions 

	AREA    main, CODE, READONLY
	EXPORT	__main				; make __main visible to linker
	ENTRY			
				
__main	PROC
	
    ; Enable the clock to GPIO Port B	
	LDR r0, =RCC_BASE
	LDR r1, [r0, #RCC_AHB2ENR]
	ORR r1, r1, #RCC_AHB2ENR_GPIOBEN
	STR r1, [r0, #RCC_AHB2ENR]
	
	; Enable the clock for GPIO Port A
	LDR r2, =RCC_BASE
	LDR r3, [r2, #RCC_AHB2ENR]
	ORR r3, r3, #RCC_AHB2ENR_GPIOAEN
	STR r3, [r2, #RCC_AHB2ENR]
	
	; Enable the clock for GPIO Port E
	LDR r2, =RCC_BASE
	LDR r3, [r2, #RCC_AHB2ENR]
	ORR r3, r3, #RCC_AHB2ENR_GPIOEEN
	STR r3, [r2, #RCC_AHB2ENR]

	; MODE: 00: Input mode,              01: General purpose output mode
    ;       10: Alternate function mode, 11: Analog mode (reset state)
	LDR r0, =GPIOB_BASE
	LDR r1, [r0, #GPIO_MODER]
	EOR r1, r1, #GPIO_MODER_MODER2
	ORR r1, r1, #(1<<4)
	STR r1, [r0, #GPIO_MODER]
	
	; MODE: 00: Input mode,              01: General purpose output mode
    ;       10: Alternate function mode, 11: Analog mode (reset state)
	LDR r2, =GPIOA_BASE
	LDR r3, [r2, #GPIO_MODER]
	EOR r3, r3, #GPIO_MODER_MODER0
	EOR r3, r3, #GPIO_MODER_MODER1
	EOR r3, r3, #GPIO_MODER_MODER2
	EOR r3, r3, #GPIO_MODER_MODER3
	EOR r3, r3, #GPIO_MODER_MODER5
	STR r3, [r2, #GPIO_MODER]
	
	; MODE: 00: Input mode,              01: General purpose output mode
    ;       10: Alternate function mode, 11: Analog mode (reset state)
	LDR r4, =GPIOE_BASE
	LDR r5, [r4, #GPIO_MODER]
	EOR r5, r5, #GPIO_MODER_MODER8
	ORR r5, r5, #(1<<16)
	STR r5, [r4, #GPIO_MODER]
	
	; Select push pull for gpio B
	LDR r0, =GPIOB_BASE
	LDR r1, [r0, #GPIO_OTYPER]
	BIC r1, r1, #GPIO_OTYPER_OT_2
	STR r1, [r0, #GPIO_OTYPER]
	
	; Select push pull for gpio E
	LDR r4, =GPIOE_BASE
	LDR r5, [r4, #GPIO_OTYPER]
	BIC r5, r5, #GPIO_OTYPER_OT_8
	STR r5, [r4, #GPIO_OTYPER]
	
	; Sets PA0, 1, 2, 3 and 5 to no pullup/pulldown
	LDR r2, =GPIOA_BASE
	LDR r3, [r2, #GPIO_PUPDR]
	BIC r3, r3, #GPIO_PUPDR_PUPDR0
	BIC r3, r3, #GPIO_PUPDR_PUPDR1
	BIC r3, r3, #GPIO_PUPDR_PUPDR2
	BIC r3, r3, #GPIO_PUPDR_PUPDR3
	BIC r3, r3, #GPIO_PUPDR_PUPDR5
	ORR r3, r3, #GPIO_PUPDR_PUPDR0_1
	ORR r3, r3, #GPIO_PUPDR_PUPDR1_1
	ORR r3, r3, #GPIO_PUPDR_PUPDR2_1
	ORR r3, r3, #GPIO_PUPDR_PUPDR3_1
	ORR r3, r3, #GPIO_PUPDR_PUPDR5_1
	STR r3, [r2, #GPIO_PUPDR]
	
	
	

	
stop LDR r3, [r2, #GPIO_IDR]
	 AND r7, r3, #1
	 CMP r7, #1
	 BEQ red
	 
	 LDR r3, [r2, #GPIO_IDR]
	 AND r7, r3, #GPIO_IDR_IDR_3
	 CMP r7, #GPIO_IDR_IDR_3
	 BEQ gre

	
	B 		stop     	; dead loop & program hangs here

; Turns on Red light by accessing PB2
red		LDR r1, [r0, #GPIO_ODR]
		EOR r1, r1, #(1<<2)
		STR r1, [r0, #GPIO_ODR]
loop	LDR r3, [r2, #GPIO_IDR]	;DEBOUNCING
		AND r7, r3, #1
		CMP r7, #1
		BNE endloop
		B	loop
endloop	B 		stop
	
		; Turns on Green Light by accessing PE8
gre		LDR r5, [r4, #GPIO_ODR]
		EOR r5, r5, #(1<<8)
		STR r5, [r4, #GPIO_ODR]
loop2	LDR r3, [r2, #GPIO_IDR]	;DEBOUNCING
		AND r7, r3, #GPIO_IDR_IDR_3
		CMP r7, #GPIO_IDR_IDR_3
		BNE endloo2
		B	loop2
endloo2 B		stop

	
					
	ALIGN			

	AREA    myData, DATA, READWRITE
	ALIGN
array	DCD   1, 2, 3, 4
	END
