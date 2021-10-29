	;Blake Lufkin
	;3/11/2020
	;Lab 6
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
	
	BL Stepper_Pin_Init
	

	; Checks constantly for buttons to be pressed for LED's
stop		BL greenled
			BL Full_Stepping
			BL greenled
			BL Half_Stepping
			B 	stop     	; dead loop & program hangs here

	ENDP
; Turns on Red light by accessing PB2
redled	PROC
		PUSH {r0, r1, LR}
		LDR r0, =GPIOB_BASE
		LDR r1, [r0, #GPIO_ODR]
		EOR r1, r1, #(1<<2)
		STR r1, [r0, #GPIO_ODR]
		POP {r0, r1, PC}
		ENDP
	
		; Turns on Green Light by accessing PE8
greenled PROC
		PUSH {r4, r5, LR}
		LDR r4, =GPIOE_BASE
		LDR r5, [r4, #GPIO_ODR]
		EOR r5, r5, #(1<<8)
		STR r5, [r4, #GPIO_ODR]
		POP {r4, r5, PC}
		ENDP

Stepper_Pin_Init PROC	;Initializes needed pins to control stepper motor
		; MODE: 00: Input mode,              01: General purpose output mode
    ;       10: Alternate function mode, 11: Analog mode (reset state)
	LDR r0, =GPIOB_BASE				;SETTING PB2, 3, 6, 7 to output
	LDR r1, [r0, #GPIO_MODER]
	BIC r1, r1, #GPIO_MODER_MODER2
	BIC r1, r1, #GPIO_MODER_MODER3
	BIC r1, r1, #GPIO_MODER_MODER6
	BIC r1, r1, #GPIO_MODER_MODER7
	ORR r1, r1, #GPIO_MODER_MODER2_0
	ORR r1, r1, #GPIO_MODER_MODER3_0
	ORR r1, r1, #GPIO_MODER_MODER6_0
	ORR r1, r1, #GPIO_MODER_MODER7_0
	STR r1, [r0, #GPIO_MODER]
	BX LR
	ENDP

;Fullstepping function
Full_Stepping PROC
		PUSH {LR}
		LDR r0, =GPIOB_BASE
		LDR r1, [r0, #GPIO_ODR] ; Load gpio B ODR from r0, =GPIOB_BASE
		MOV r2, #0	; Counter for outside for loop. 512 iterations
		
Ocheck	MOV r7, #0x201
		CMP r2, r7
		BEQ endfull
		
		ADD r2, r2, #1
		MOV r3, #0	;r3=inside for loop counter
		LDR r4, =Fullarray
		LDR r6, [r4], #4

Fcheck	CMP r3, #4
		BEQ Ocheck

		LDR r1, [r0, #GPIO_ODR]
		BIC r1, r1, #GPIO_ODR_ODR_2
		BIC r1, r1, #GPIO_ODR_ODR_3
		BIC r1, r1, #GPIO_ODR_ODR_6
		BIC r1, r1, #GPIO_ODR_ODR_7
		STR r1, [r0, #GPIO_ODR]
		
		;ALL 4 Steps
		LDR r1, [r0, #GPIO_ODR]	;Find value of 4th bit
		AND r5, r6, #0x8
		MOV r5, r5, LSR #3 
		MOV r5, r5, LSL #2
		ADD r1, r1, r5	;Store in r1
		
		;Find value of 3rd bit
		AND r5, r6, #0x4
		MOV r5, r5, LSR #2
		MOV r5, r5, LSL #3
		ADD r1, r5, r1		;Add to r1
		
		;Find value of 2nd bit
		AND r5, r6, #0x2
		MOV r5, r5, LSR #1
		MOV r5, r5, LSL #6
		ADD r1, r5, r1		;Add to r1
		
		;Find Value of 1st bit
		AND r5, r6, #0x1
		MOV r5, r5, LSL #7
		ADD r1, r5, r1		;Add to r1
		
		STR r1, [r0, #GPIO_ODR];Store value of r10 in GPIO ODR
		
		PUSH {r0}
		MOV r0, #2500
		BL delay
		POP {r0}
		
		ADD r3, r3, #1		;x++
		LDR r6, [r4], #4 	;Next iteration in the array
		B Fcheck
endfull POP {PC}	;back to dead loop
		ENDP

;Halfstepping Function
Half_Stepping PROC	
		PUSH {LR}
		
		LDR r0, =GPIOB_BASE
		LDR r1, [r0, #GPIO_ODR] ; Load gpio B ODR from r0, =GPIOB_BASE
		MOV r2, #0	; Counter for outside for loop. 512 iterations
HOcheck	CMP r2, r7
		BEQ endhalf
		ADD r2, r2, #1
		MOV r3, #0	;r3=inside for loop counter
		LDR r4, =Halfarray
		LDR r6, [r4], #4
		
Hcheck	CMP r3, #8
		BEQ HOcheck
		
		LDR r1, [r0, #GPIO_ODR]
		BIC r1, r1, #GPIO_ODR_ODR_2
		BIC r1, r1, #GPIO_ODR_ODR_3
		BIC r1, r1, #GPIO_ODR_ODR_6
		BIC r1, r1, #GPIO_ODR_ODR_7
		STR r1, [r0, #GPIO_ODR] 
		;ALL 4 Steps
			;Find value of 4th bit
		LDR r1, [r0, #GPIO_ODR]
		AND r5, r6, #0x8
		MOV r5, r5, LSR #3 
		MOV r5, r5, LSL #2
		ADD r1, r5, r1		;Store in r10
		
			;Find value of 3rd bit
		AND r5, r6, #0x4
		MOV r5, r5, LSR #2
		MOV r5, r5, LSL #3
		ADD r1, r5, r1		;Add to r10
		
			;Find value of 2nd bit
		AND r5, r6, #0x2
		MOV r5, r5, LSR #1
		MOV r5, r5, LSL #6
		ADD r1, r5, r1		;Add to r10
		
			;Find Value of 1st bit
		AND r5, r6, #0x1
		MOV r5, r5, LSL #7
		ADD r1, r5, r1		;Add to r11
		STR r1, [r0, #GPIO_ODR];Store value of r10 in GPIO ODR
		
		PUSH {r0}
		MOV r0, #2500
		BL delay
		POP {r0}
		
		ADD r3, r3, #1
		LDR r6, [r4], #4 ;Next iteration in the array
		
		B Hcheck
endhalf POP {PC}	;back to dead loop
		ENDP
			
delay PROC
		PUSH {r1, LR}
		MOV r1, #0

slp		CMP r0, r1
		BEQ endslp
		ADD r1, r1, #1
		B slp
		
endslp	POP {r1, PC}
		ENDP
			

	ALIGN			

	AREA    myData, DATA, READWRITE
	ALIGN
Halfarray	DCD		0x9, 0x8, 0xA, 0x2, 0x6, 0x4, 0x5, 0x1 
Fullarray	DCD		0x9, 0xA, 0x6, 0x5
	END
