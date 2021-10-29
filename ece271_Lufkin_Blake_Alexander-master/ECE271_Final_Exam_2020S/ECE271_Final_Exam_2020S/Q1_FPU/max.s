;Blake Lufkin
;ECE271
;Edit original program to determine the greatest number out of 3 passed floating point numbers
;Edited: 5/7/2020

	AREA    fun, CODE, READONLY, ALIGN=3

		
max PROC
	EXPORT max
	VCMPE.F32 s0, s1 ;r0=x r1=y r2=z
	VMRS APSR_nzcv, FPSCR
	BGT zchx
	B zchy

zchx	VCMPE.F32 s0, s2
		VMRS APSR_nzcv, FPSCR
		BGT endf ;r0=x greatest
		VMOV.F32 s0, s2
		B endf ;r3=z greatest

zchy	VCMPE.F32 s1, s2
		VMRS APSR_nzcv, FPSCR
		BGT sety	;y= greatest
		VMOV.F32 s0, s2	;z= greatest
		B endf 

sety	VMOV.F32 s0, s1 ;return y=max
		B endf
		
endf	BX  lr	
		ENDP
		END