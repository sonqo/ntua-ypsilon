START:
	MVI A,00H ; initialization of LEDs to 0
	MVI B,10H ; used in DELB function
	MVI C,00H
	MVI D,00H		
	MVI H,00H
	CMA ; inverse logic LEDs
	STA 3000H

UPDATE:	
	MVI E,00H ; flag for INR(0) or DCR(1)
	LDA 2000H
	ANI F0H ; keeping the 4 MSB of PORT-2000H
	RRC
	RRC
	RRC
	RRC
	MOV D,A	; counter upper limit

CHECK:
	LDA 2000H
	RAR
	JNC CHECK ; if LSB = 0, do nothing
	RAL
	MVI A,00H
	CMP D
	JZ UPDATE ; if initial limit is 0, do nothing until update
	CMP E
	JZ PLUS
	JNZ MINUS

PLUS:	
	MOV A,C
	INR A
	MOV C,A
	CALL DELB
	CMA
	STA 3000H
	MOV A,C
	CMP D
	JZ ALTER ; if A = UpperLimit, count backwards
	JNZ CHECK

MINUS:
	MOV A,C
	DCR A
	MOV C,A
	CALL DELB
	CMA
	STA 3000H
	MOV A,C
	CMP H
	JZ UPDATE ; if A = 0, update upper limit
	JNZ CHECK

ALTER:
	MVI E,01H ; alter flag for backward counting
	JMP CHECK	

END
