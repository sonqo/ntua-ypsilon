START:	
	MVI A,80H			; initialization of LEDs to 0
	MVI B,05H			; used in DELB function
	MVI C,80H
	MVI D,81H			; current state of leds
	MVI E,81H			; previous state of leds
	MVI L,01H			; limits for direction shifting
	MVI H,80H
	CMA 				; inverse logic LEDs
	STA 3000H
		
LOOP1:
	MOV E,D
	LDA 2000H
	RAR
	JNC LOOP1			; if MSB if OFF, do nothing
	RAL
	MOV D,A
	CMP E				; compare current state of LEDs with previous
	JNZ LOOP2			; if they are different, switch shifting
	MOV A,C
	RRC
	MOV C,A
	CALL DELB
	CMA
	STA 3000H
	MOV A,C
	CMP L				; compare with lower limit
	JZ LOOP2
	JNZ LOOP1

LOOP2:
	MOV E,D
	LDA 2000H
	RAR
	JNC LOOP2			; if MSB is OFF, do nothing
	RAL
	MOV D,A
	CMP E				; compare current state of LEDs with previous
	JNZ LOOP1			; if they are different, switch shifting
	MOV A,C
	RLC
	MOV C,A
	CALL DELB
	CMA
	STA 3000H
	MOV A,C
	CMP H				; compare with upper limit
	JZ LOOP1
	JNZ LOOP2

END