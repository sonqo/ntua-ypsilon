.include "m16def.inc"

.DSEG
	_tmp_: .byte 2

.CSEG

main:
	ldi r24, low(RAMEND) ; initializing stack pointer
	out SPL, r24
	ldi r24, high(RAMEND)
	out SPH, r24
	ldi r24, 0xF0 ; initializing PORTC for keypad scanning
	out DDRC, r24		
	ser r24 ; initializing PORTD for output
	out DDRD, r24
	
	clr r24
	rcall lcd_init

start:
	clr r24 ; initialize _tmp_ = 0
	ldi XH, high(_tmp_)
	ldi XL, low(_tmp_)
	st X+,r24
	st X,r24

scan_first:
	ldi r18, 0x00 ; E = 0
	ldi r19, 0x00 ; D = 0
	ldi r20, 0x00 ; M = 0

	ldi r24, low(20)
	rcall scan_keypad_rising_edge
	rcall keypad_to_hex
	cpi r24, 0xFF
	breq scan_first
	push r24

scan_second:
	ldi r24, low(20)
	rcall scan_keypad_rising_edge
	rcall keypad_to_hex
	cpi r24, 0xFF
	breq scan_second
	pop r25

	lsl r25 ; move first digit to higher nibble
	lsl r25
	lsl r25
	lsl r25
	add r24, r25
	rol r24 ; check sign of number
	brcs minus
	ldi r17, '+'
	ror r24
	rjmp digits

minus:
	ldi r17, '-'
	ror r24
	neg r24

digits:
	cpi r24, 0x64 ; check if number is greater than 100
	brlt no_hunderds ; if true, increment hundreds
	inc r18
	subi r24, 0x64

no_hunderds:
	cpi r24, 0x0A
	brlt no_tens ; increment tens, as long as the number is greater than 10
	inc r19
	subi r24, 0x0A
	rjmp no_hunderds

no_tens:
	mov r20, r24 ; ones is equal to the result of all above functions
	rcall display
	rjmp scan_first

display:
	mov r24, r17
	rcall lcd_data
	mov r24, r18
	rcall hex_to_ascii
	rcall lcd_data
	mov r24, r19
	rcall hex_to_ascii
	rcall lcd_data
	mov r24, r20
	rcall hex_to_ascii
	rcall lcd_data
	ldi r24, 0x02 ; cursor home command
	rcall lcd_command
	ret

write_2_nibbles:
	push r24
	in r25, PIND 
	andi r25, 0x0F
	andi r24, 0xF0
	add r24, r25 
	out PORTD, r24 
	sbi PORTD, PD3 ; enable pulse
	cbi PORTD, PD3 
	pop r24 
	swap r24 
	andi r24, 0xF0 
	add r24, r25
	out PORTD, r24
	sbi PORTD, PD3 ; enable pulse
	cbi PORTD, PD3
	ret

lcd_data: ; data transfered located in register r24
	sbi PORTD, PD2 
	rcall write_2_nibbles
	ldi r24, 43 ; 43us delay
	ldi r25, 0
	rcall wait_usec
	ret

lcd_command:
	cbi PORTD, PD2 
	rcall write_2_nibbles
	ldi r24, 39 ; 39us delay
	ldi r25, 0 
	rcall wait_usec 
	ret

lcd_init:
	ldi r24, 40
	ldi r25, 0 
	rcall wait_msec ; initial delay of 40ms
	ldi r24, 0x30 ; setting to 8-bit mode
	out PORTD, r24 
	sbi PORTD, PD3 ; enable pulse
	cbi PORTD, PD3 
	ldi r24, 39
	ldi r25, 0 
	rcall wait_usec 
	ldi r24, 0x30 ; re-setting to 8-bit mode
	out PORTD, r24
	sbi PORTD, PD3 ; enable pulse
	cbi PORTD, PD3
	ldi r24, 39
	ldi r25, 0
	rcall wait_usec
	ldi r24, 0x20 ; switch to 4-bit mode
	out PORTD, r24
	sbi PORTD, PD3 ; enable pulse
	cbi PORTD, PD3
	ldi r24, 39
	ldi r25, 0
	rcall wait_usec
	ldi r24, 0x28 ; two-line display in a 5x8 mannner
	rcall lcd_command 
	ldi r24, 0x0c ; hide cursor
	rcall lcd_command
	ldi r24, 0x01 ; clear display
	rcall lcd_command
	ldi r24, low(1530)
	ldi r25, high(1530)
	rcall wait_usec
	ldi r24, 0x06 ; disable screen shifting
	rcall lcd_command 
	ret

scan_row: ; scanning row number r24 for pressed keys, returns respective status in r24
	ldi r25, 0x08
back_:	
	lsl r25
	dec r24
	brne back_
	out PORTC, r25
	nop ; waitng for status switching to take place
	nop
	in r24, PINC
	andi r24, 0x0F ; status isolated in LSBs of r24
	ret

scan_keypad: ; returns in register pair r25:r24 the keypad status of all 16 keys - calls scan row
	ldi r24, 0x01
	rcall scan_row ; checking 1st line
	swap r24
	mov r27, r24
	ldi r24, 0x02
	rcall scan_row ; checking 2nd line
	add r27, r24
	ldi r24, 0x03
	rcall scan_row ; checking 3rd line
	swap r24
	mov r26, r24
	ldi r24, 0x04
	rcall scan_row	; checking 4th line
	add r26, r24
	movw r24, r26
	ret

scan_keypad_rising_edge:	
	mov r22, r24 ; save delay timer(r24) before first read
	rcall scan_keypad
	push r24 ; save status to stack
	push r25
	mov r24, r22	
	ldi r25, 0x00
	rcall wait_msec	; delay before rechecking
	rcall scan_keypad
	pop r23
	pop r22
	and r24, r22 ; discard
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

hex_to_ascii: 
	movw r26, r24 
	ldi r24, '0'
	cpi r26, 0x00
	breq end
	ldi r24, '1'
	cpi r26, 0x01
	breq end
	ldi r24, '2'
	cpi r26, 0x02
	breq end
	ldi r24, '3'
	cpi r26, 0x03
	breq end
	ldi r24, '4'
	cpi r26, 0x04
	breq end
	ldi r24, '5'
	cpi r26, 0x05
	breq end
	ldi r24, '6'
	cpi r26, 0x06
	breq end
	ldi r24, '7'
	cpi r26, 0x07
	breq end
	ldi r24, '8'
	cpi r26, 0x08
	breq end
	ldi r24, '9'
end:
	ret

keypad_to_hex: 
	movw r26, r24 
	ldi r24, 0x0E
	sbrc r26, 0
	ret
	ldi r24, 0x00
	sbrc r26, 1
	ret
	ldi r24, 0x0F
	sbrc r26, 2
	ret
	ldi r24, 0x0D
	sbrc r26, 3 
	ret 
	ldi r24, 0x07
	sbrc r26, 4
	ret
	ldi r24, 0x08
	sbrc r26, 5
	ret
	ldi r24, 0x09
	sbrc r26, 6
	ret
	ldi r24, 0x0C
	sbrc r26, 7
	ret
	ldi r24, 0x04
	sbrc r27, 0 
	ret
	ldi r24, 0x05
	sbrc r27, 1
	ret
	ldi r24, 0x06
	sbrc r27, 2
	ret
	ldi r24, 0x0B
	sbrc r27, 3
	ret
	ldi r24, 0x01
	sbrc r27, 4
	ret
	ldi r24, 0x02
	sbrc r27, 5
	ret
	ldi r24, 0x03
	sbrc r27, 6
	ret
	ldi r24, 0x0A
	sbrc r27, 7
	ret
	ldi r24, 0xFF ; error handling
	ret

wait_msec: ; delay routine, calls wait_usec
	push r24 
	push r25 
	ldi r24, low(998) 
	ldi r25, high(998) 
	rcall wait_usec
	pop r25 
	pop r24 
	sbiw r24, 1 
	brne wait_msec 
	ret

wait_usec:
	sbiw r24, 1 
	nop 
	nop 
	nop 
	nop 
	brne wait_usec 
	ret 