.include "m16def.inc"

.DSEG
		_tmp_: .byte 2

.CSEG

main:
		ldi r24, (1<<PC7)|(1<<PC6)|(1<<PC5)|(1<<PC4)	; initializing PORTC for keypad scanning
		out DDRC, r24
		ser r24				; initializing PORTB for output
		out DDRB, r24
		ldi r24 , low(RAMEND)	; initializing stack pointer
		out SPL , r24		
		ldi r24 , high(RAMEND)
		out SPH , r24
		clr r24				; initialize _tmp_ = 0
		ldi XH, 0x00
		ldi XL, 0x00
		st X+,r24
		st X,r24

		ldi r24, low(20)	; initializing debouncing delay
		rcall scan_keypad_rising_edge
		rcall keypad_to_ascii
		push r24			; save first number typed to stack
		rcall scan_keypad_rising_edge
		rcall keypad_to_ascii
		pop r25

		ldi r20, 0x10		; initializing counter in case of blink
		ldi r21, 0x00		; initializing status of LEDs
		cpi r25, '1'
		brne blink
		cpi r24, '1'
		brne blink

light_all:
		ldi r21, 0xFF		; light all LEDs
		out PORTB, r21
		ldi r24, low(4000)	; for 4 seconds
		ldi r25, high(4000)
		rcall wait_msec
		clr r21
		out PORTB, r21
		rjmp main

blink:
		ldi r21, 0xFF
		out PORTB, r21
		ldi r24, low(250)
		ldi r25, high(250)
		rcall wait_msec
		dec r20
		cpi r20, 0x00
		brne blink
		rjmp main


scan_row:					; scanning row number r24 for pressed keys, returns respective status in r24
		ldi r25, 0x08
back:	
		lsl r25
		dec r24
		brne back
		out PORTC, r25
		nop					; waitng for status switching to take place
		nop
		in r24, PINC
		andi r24, 0x0F		; status isolated in LSBs of r24
		ret

scan_keypad:				; returns in register pair r25:r24 the keypad status of all 16 keys - calls scan row
		ldi r24, 0x01
		rcall scan_row		; checking 1st line
		swap r24
		mov r27, r24
		ldi r24, 0x02
		rcall scan_row		; checking 2nd line
		add r27, r24
		ldi r24, 0x03
		rcall scan_row		; checking 3rd line
		swap r24
		mov r26, r24
		ldi r24, 0x04
		rcall scan_row		; checking 4th line
		add r26, r24
		movw r24, r26
		ret

scan_keypad_rising_edge:	; return result in r25:r24
		mov r22, r24		; save delay timer(r24) before first read
		rcall scan_keypad
		push r24			; save status to stack
		push r25
		mov r24, r22	
		ldi r25, 0x00
		rcall wait_msec		; delay before rechecking
		rcall scan_keypad
		pop r23
		pop r22
		and r24, r22		; discard
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

keypad_to_ascii: 
		movw r26 ,r24 
		ldi r24 ,'*'
		sbrc r26 ,0
		ret
		ldi r24 ,'0'
		sbrc r26 ,1
		ret
		ldi r24 ,'#'
		sbrc r26 ,2
		ret
		ldi r24 ,'D'
		sbrc r26 ,3 
		ret 
		ldi r24 ,'7'
		sbrc r26 ,4
		ret
		ldi r24 ,'8'
		sbrc r26 ,5
		ret
		ldi r24 ,'9'
		sbrc r26 ,6
		ret
		ldi r24 ,'C'
		sbrc r26 ,7
		ret
		ldi r24 ,'4'
		sbrc r27 ,0 
		ret
		ldi r24 ,'5'
		sbrc r27 ,1
		ret
		ldi r24 ,'6'
		sbrc r27 ,2
		ret
		ldi r24 ,'B'
		sbrc r27 ,3
		ret
		ldi r24 ,'1'
		sbrc r27 ,4
		ret
		ldi r24 ,'2'
		sbrc r27 ,5
		ret
		ldi r24 ,'3'
		sbrc r27 ,6
		ret
		ldi r24 ,'A'
		sbrc r27 ,7
		ret
		clr r24
		ret

wait_msec:					; delay routine, calls wait_usec
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