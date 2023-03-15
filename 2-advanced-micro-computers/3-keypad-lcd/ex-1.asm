.include "m16def.inc"

.DSEG
	_tmp_: .byte 2

.CSEG

reset:
	ldi r24, 0xF0 ; initialize PORTC for keypad scanning
	out DDRC, r24
	ser r24 ; initialize PORTB for input
	out DDRB, r24
	ldi r24, low(RAMEND) ; initialize stack pointer
	out SPL, r24
	ldi r24, high(RAMEND)
	out SPH, r24

start:
	clr r24 ; initialize _tmp_ = 0
	ldi XH, high(_tmp_)
	ldi XL, low(_tmp_)
	st X+, r24
	st X, r24

scan_first:
	ldi r24,low(20) ; 20ms estimated bouncing time
	rcall scan_keypad_rising_edge ; read input of first digit
	rcall keypad_to_ascii
	cpi r24, 0x00
	breq scan_first
	push r24

scan_second:
	ldi r24, low(20)
	rcall scan_keypad_rising_edge
	rcall keypad_to_ascii
	cpi r24, 0x00
	breq scan_second
	pop r25
	cpi r25, '0' ; check for PASSWORD: 05
	brne blink
	cpi r24, '5'
	brne blink

light_all: ; light up all PORTB LEDs, in case PASSWORD is correct
	ldi r22, 0xFF
	out PORTB, r22
	ldi r24, low(4000) ; for 4 seconds
	ldi r25, high(4000)
	rcall wait_msec
	ldi r22, 0x00
	out PORTB, r22
	rjmp start

blink:
	ldi r23, 0x10 ; blink all PORTB LEDs for 4 secs, in case PASSWORD is incorrect
again:
	out PORTB, r22
	com r22 ; compliment LED output for the next loop
	ldi r24, 250 ; wait 250ms
	clr r25
	rcall wait_msec
	dec r23             
	brne again ; loop 16 times 
    rjmp start

scan_row:
	ldi r25, 0x08 
back_: 
	lsl r25 
	dec r24 
	brne back_
	out PORTC, r25 
	nop
	nop 
	in r24, PINC 
	andi r24, 0x0F
	ret 

scan_keypad:
	ldi r24, 0x01
	rcall scan_row
	swap r24 
	mov r27, r24 
	ldi r24, 0x02 
	rcall scan_row
	add r27, r24 
	ldi r24, 0x03 
	rcall scan_row
	swap r24 
	mov r26, r24 
	ldi r24, 0x04 
	rcall scan_row
	add r26, r24 
	movw r24, r26 
	ret

scan_keypad_rising_edge:
	mov r22, r24 
	rcall scan_keypad 
	push r24 
	push r25
	mov r24, r22 
	ldi r25, 0 
	rcall wait_msec
	rcall scan_keypad 
	pop r23 
	pop r22
	and r24, r22
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
	movw r26, r24 
	ldi r24, '*'
	sbrc r26, 0
	ret
	ldi r24, '0'
	sbrc r26, 1
	ret
	ldi r24, '#'
	sbrc r26, 2
	ret
	ldi r24, 'D'
	sbrc r26, 3 
	ret 
	ldi r24, '7'
	sbrc r26, 4
	ret
	ldi r24, '8'
	sbrc r26, 5
	ret
	ldi r24, '9'
	sbrc r26, 6
	ret
	ldi r24, 'C'
	sbrc r26, 7
	ret
	ldi r24, '4'
	sbrc r27, 0 
	ret
	ldi r24, '5'
	sbrc r27, 1
	ret
	ldi r24, '6'
	sbrc r27, 2
	ret
	ldi r24, 'B'
	sbrc r27, 3
	ret
	ldi r24, '1'
	sbrc r27, 4
	ret
	ldi r24, '2'
	sbrc r27, 5
	ret
	ldi r24, '3'
	sbrc r27, 6
	ret
	ldi r24, 'A'
	sbrc r27, 7
	ret
	clr r24
	ret

wait_msec: ; delay routine, calls wait_usec
	push r24
	push r25
	ldi r24, low(998)
	ldi r25, high(998)
	rcall wait_usec
	pop r25
	pop r24
	sbiw r24,1
	brne wait_msec
	ret

wait_usec:
	sbiw r24,1
	nop
	nop
	nop
	nop
	brne wait_usec
	ret