;******************** (C) Yifeng ZHU *******************************************
; @file    main.s
; @author  Yifeng Zhu
; @date    May-17-2015
; @note
;           This code is for the book "Embedded Systems with ARM Cortex-M 
;           Microcontrollers in Assembly Language and C, Yifeng Zhu, 
;           ISBN-13: 978-0982692639, ISBN-10: 0982692633
; @attension
;           This code is provided for education purpose. The author shall not be 
;           held liable for any direct, indirect or consequential damages, for any 
;           reason whatever. More information can be found from book website: 
;           http:;www.eece.maine.edu/~zhu/book
;*******************************************************************************
;Blake Lufkin
;ECE271
;Edit original program to Toggle pb6 and pb7
;Edited: 5/7/2020

	INCLUDE core_cm4_constants.s		; Load Constant Definitions
	INCLUDE stm32l476xx_constants.s      

	AREA    main, CODE, READONLY
	EXPORT	__main				; make __main visible to linker
	ENTRY			
				
__main	PROC
	
    ; Enable the clock to GPIO Port B	
	LDR r0, =RCC_BASE
	LDR r1, [r0, #RCC_AHB2ENR]
	ORR r1, r1, #RCC_AHB2ENR_GPIOBEN
	STR r1, [r0, #RCC_AHB2ENR]

	; MODE: 00: Input mode, 01: General purpose output mode
    ;       10: Alternate function mode, 11: Analog mode (reset state)
	LDR r0, =GPIOB_BASE
	LDR r1, [r0, #GPIO_MODER]
	EOR r1, r1, #GPIO_MODER_MODER6
	EOR r1, r1, #GPIO_MODER_MODER7
	ORR r1, r1, #(5<<12)
	STR r1, [r0, #GPIO_MODER]


;Blake Lufkin BSRR code
stop
	LDR r1, [r0, #GPIO_BSRR]
	LDR r1, =GPIO_BSRR_BS_6
	STR r1, [r0, #GPIO_BSRR]
	
	MOV r2, #0x5000
	MOV r3, #0
de1	SUB r2, r2, #1
	CMP r2, r3
	BNE de1
	
	LDR r1, [r0, #GPIO_BSRR]
	LDR r1, =GPIO_BSRR_BS_7
	STR r1, [r0, #GPIO_BSRR]
	
	MOV r2, #0x5000
	MOV r3, #0
de2	SUB r2, r2, #1
	CMP r2, r3
	BNE de2
	
	LDR r1, [r0, #GPIO_BSRR]
	LDR r1, =GPIO_BSRR_BR_6
	STR r1, [r0, #GPIO_BSRR]
	
	MOV r2, #0x5000
	MOV r3, #0
de3	SUB r2, r2, #1
	CMP r2, r3
	BNE de3
	
	LDR r1, [r0, #GPIO_BSRR]
	LDR r1, =GPIO_BSRR_BR_7
	STR r1, [r0, #GPIO_BSRR]
	
	MOV r2, #0x5000
	MOV r3, #0
de4	SUB r2, r2, #1
	CMP r2, r3
	BNE de4
  
 	B 		stop     		; dead loop & program hangs here

	ENDP
					
	ALIGN			

	AREA    myData, DATA, READWRITE
	ALIGN
array	DCD   1, 2, 3, 4
	END
