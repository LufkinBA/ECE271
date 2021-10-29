;Blake Lufkin
;ECE271
;Edit original program to count leading zeros of a 64bit number
;Edited: 5/7/2020

	AREA    fun, CODE, READONLY, ALIGN=3
		
count_leading_zeros PROC
	EXPORT count_leading_zeros
	CLZ r2, r1
	CMP r2, #32
	BEQ upper
	B easyend
	
upper CLZ r3, r0
	ADD r2, r2, r3
	
easyend MOV r0, r2
	BX  lr	
	ENDP
	END