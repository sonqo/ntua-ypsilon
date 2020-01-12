START:
	MVI A,0DH
	SIM					; interrupt flags initialization
	MVI D,00H 			; D <- 0 (Counter)
	MVI E,00H 			; E <- 0 (Interrupt counter)
	MVI L,00H 			; L <- 0 (flag: 1 == ignore interrupt)
	MVI A,00H	
	STA 0B01H
	MVI A,10H
	STA 0B02H
	STA 0B03H			; 7-segment initialization
	STA 0B04H
	STA 0B05H
	STA 0B06H
		
CHECK:		
	LXI B,0001H			; Delay parameter: 1 msec
	LDA 2000H
	ANI 01H      		; flag Z is modified
	JZ NO_INTER			; if LSB==0 ignore interrupts,
	EI					; else enable int/s and 
	MVI L,00H			; set L flag to 0
	MVI A,64H

LOOP1:		
	DI
	CALL DELB
	CALL PRINT			; loop 100 times with 1msec delay
	EI					; to avoid double int/s through DELB
	DCR A				; and keep a high 7-seg refresh rate
	JNZ LOOP1
	JMP COUNTER

NO_INTER:	
	DI					; disable int/s and
	MVI L,01H			; set L flag to 1 (ignore int/s)
	MVI H,C8H   		; H = 200

LOOP2:		
	CALL DELB
	CALL PRINT			; loop 200 times with 1 msec delay
	DCR H
	JNZ LOOP2

COUNTER:	
	MOV A,D
	RLC
	RLC
	RLC
	RLC
	CMA          		; inverse logic LED's 
	STA 3000H			; print counter to 4 MSB LEDs
	MOV A,D
	CPI 0FH
	JZ FIFTEEN			; if count == 15 reset,
	INR D				; else increase it
	JMP CHECK

FIFTEEN:		
	MVI D,00H    		; reset counter = 0
	JMP CHECK

INTR_ROUTINE:
	DI
	PUSH PSW
	MOV A,L      		; check L(flag)
	CPI 01H
	JZ INT_END	 		; if L(flag) == 1 ignore interrupt
	MVI L,01H    		; ingore all the next interrupts,
	INR E		 		; else increase interrupt counter
	MOV A,E     
	ANI 1EH				; divide by two
	RRC					; int/s = (int/s MOD 10)
	CPI 0AH     		; if int/s == 10, reset int/s
	JC AVOID_RESET
	MVI E,00H
	MOV A,E
	STA 0B01H

AVOID_RESET:
	STA 0B01H

INT_END:	
	POP PSW
	EI
	MVI L,00H   		; enable the other interrupts 
	RET

PRINT:		
	PUSH PSW
	PUSH D
	PUSH H
	LXI D,0B01H
	CALL STDM			; update 7-segment
	CALL DCD
	POP H
	POP D
	POP PSW
	RET
END