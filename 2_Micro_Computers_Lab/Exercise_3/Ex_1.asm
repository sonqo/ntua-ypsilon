.DSEG
	_tmp_: .byte 2

.CSEG
		ldi r24, (1<<PC7)|(1<<PC6)|(1<<PC5)|(1<<PC4)		; initializing PORTC for keypad scanning
		out DDRC, r24

scan_row:						; scanning row number r24 for pressed keys, returns respective status in r24
		ldi r25, 0x08
back:	lsl r25
		dec r24
		brne back
		out PORTC, r25
		nop						; waitng for status switching to take place
		nop
		in r24, PINC
		andi r24, 0x0F			; status isolated in LSBs of r24
		ret

scan_keypad:					; returns in register pair r25:r24 the keypad status of all 16 keys - calls scan row
		ldi r24, 0x01
		rcall scan_row			; checking 1st line
		swap r24
		mov r27, r24
		ldi r24, 0x02
		rcall scan_row			; checking 2nd line
		add r27, r24
		ldi r24, 0x03
		rcall scan_row			; checking 3rd line
		swap r24
		mov r26, r24
		ldi r24, 0x04
		rcall scan_row			; checking 4th line
		add r26, r24
		movw r24, r26
		ret

scan_keypad_rising_edge:		
		mov r22, r24			; save delay timer(r24) before first read
		rcall scan_keypad
		push r24				; save status to stack
		push r25
		mov r24, r22	
		ldi r25, 0x00
		rcall wait_msec			; delay before rechecking
		rcall scan_keypad
		pop r23
		pop r22
		and r24, r22			; discard
		and r25, r23
		ldi r26, low(_tmp_)
		ldi r27, high(_tmp_)
		ld r23, X+
		ld r22, X
		st X, r24 
		st -X, r25 
		com r23
		com r22 
		and r24, r22
		and r25, r23
		ret

wait_msec:						; delay routine, calls wait_usec
		push r24 
		push r25 
		ldi r24 , low(998) 
		ldi r25 , high(998) 
		rcall wait_usec
		pop r25 
		pop r24 
		sbiw r24 , 1 
		brne wait_msec 
		ret

wait_usec:
		sbiw r24 ,1 
		nop 
		nop 
		nop 
		nop 
		brne wait_usec 
		ret 