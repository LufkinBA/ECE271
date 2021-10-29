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
;Edit original program to perform dot product on two arrays
;Edited: 5/7/2020

	INCLUDE core_cm4_constants.s		; Load Constant Definitions
	INCLUDE stm32l476xx_constants.s      

	AREA    main, CODE, READONLY
	EXPORT	__main				; make __main visible to linker
	ENTRY			
				
__main	PROC
	
	; Initialialize the input arguments
	; ...
	LDR r0, =A_Array
	LDR r1, =B_Array
	LDR r3, =size
	LDR r2, [r3]
	; call the function
	BL dot_product
	LDR r1, =Product
	STR r0, [r1]  ; Save the result to the memory
  
stop 	B 		stop     		; dead loop & program hangs here

	ENDP
		
dot_product PROC
	EXPORT dot_product
	; Complete your implementation here.
	PUSH {r4, r5, r6}
	MOV r6, #0
	MOV r4, r2
	MOV r5, #0
loop	
	CMP r4, r5
	BEQ out
	LDR r2, [r0], #4 
	LDR r3, [r1], #4
	MUL r2, r2, r3
	ADD r6, r2, r6
	SUB r4, r4, #1
	B loop

out MOV r0, r6
	POP {r4,r5,r6}
	
	BX lr
	ENDP
					
	ALIGN			

		 AREA myData, DATA
size     DCD  8
A_Array  DCD  1,2,3,4,5,6,7,8
B_Array  DCD  9,10,11,12,13,14,15,16
Product  DCD  0
	END
