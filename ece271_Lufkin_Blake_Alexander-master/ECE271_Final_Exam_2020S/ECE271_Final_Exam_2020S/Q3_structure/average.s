;Blake Lufkin
;ECE271
;Edit original program to perform average score from a passed struct and number of students
;Edited: 5/7/2020

	AREA    fun, CODE, READONLY, ALIGN=3
		
find_avg_score PROC
	EXPORT find_avg_score
	IMPORT students
	;r1 = number of students
	LDR r0, =students
	PUSH {r4}
	MOV r4, r1
	LDR r2, [r0,#4]! ;score in student[0]
	MOV r3, #0
	ADD r3, r3, r2
	
loop	CMP r1, #0
		BEQ finish
		SUB r1, r1, #1
		LDR r2, [r0,#12]!
		ADD r3, r3, r2
		B loop
		
finish	UDIV r3, r3, r4
		MOV r0, r3
		POP {r4}
		
	BX  lr	
	ENDP
	END