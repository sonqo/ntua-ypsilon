START:
	IN 10H 
	CALL DISP_INIT ; clearing screen

READ_1ST_NUM:
	CALL KIND ; read FIRST_NUM
	MOV C,A
	CALL KIND ; read SECOND_NUM
	MOV B,A
	MOV A,C
	CMP B
	JC NEGATIVE	; result is a negative number
	JNC NON_NEGATIVE ; result is a positive number

NEGATIVE:
	MOV C,A	
	MVI A,1CH ; loading minus sign code
	STA 0902H ; in the third last digit
	MOV A,B ; switch x,y numbers
	MOV B,C
	SUB B
	CPI 09H ; if result is lower than 9, the first digit is 0,
	JC ZERO	; else the first digit is 1
	JZ ZERO
	JNC ONE

NON_NEGATIVE:
	MOV D,A
	MVI A,10H ; clearing any possible minus sign
	STA 0902H ; in the third last digit
	MOV A,D
	SUB B
	CPI 09H	; if result is lower than 9, the first digit is 0,
	JC ZERO	; else the first digit is 1
	JZ ZERO
	JNC ONE

ZERO:
	MOV B,A
	MVI A,00H
	STA 0901H
	JMP FINAL

ONE:
	MOV B,A
	MVI A,01H
	STA 0901H
	MOV A,B
	MVI B,FFH
L:	DCR A ; count how many times we should substract 1 from result to reach 10
	INR B ; result kept in B register
	CPI 0AH
	JNC L
	JMP FINAL

FINAL: ; final loading of result
	MOV A,B
	STA 0900H
	LXI D,0900H 
	CALL STDM
	CALL DCD
	JMP READ_1ST_NUM

DISP_INIT: ; clearing 7-segment screen
	MVI A,10H ; loading blank
	STA 0900H		
	STA 0901H
	STA 0902H
	STA 0903H
	STA 0904H
	STA 0905H
	LXI D,0900H
	CALL STDM
	CALL DCD
	RET

END