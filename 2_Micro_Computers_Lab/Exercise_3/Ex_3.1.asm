.include "m16def.inc"

main:
	ldi r24, low(RAMEND)	; initializing stack pointer
	out SPL, r24
	ldi r24, high(RAMEND)
	out SPH, r24			
	ser r24					; initializing PORTD for output
	out DDRD, r24
	clr r24
	rcall lcd_init

zerofy:
	ldi r20, 0x00
	ldi r21, 0x00
loop:
	inc r20
	cpi r20, 0x3B
	breq increment_minutes
	rjmp loop
increment_minutes:
	inc r21
	cpi r21, 0x3B
	breq zerofy
	rjmp loop

write_2_nibbles:
	push r24
	in r25, PIND 
	andi r25, 0x0F
	andi r24, 0xF0
	add r24, r25 
	out PORTD, r24 
	sbi PORTD, PD3		; enable pulse
	cbi PORTD, PD3 
	pop r24 
	swap r24 
	andi r24, 0xF0 
	add r24, r25
	out PORTD, r24
	sbi PORTD, PD3		; enable pulse
	cbi PORTD, PD3
	ret

lcd_data:				; data transfered located in register r24
	sbi PORTD, PD2 
	rcall write_2_nibbles
	ldi r24, 43			; 43ms delay
	ldi r25, 0
	rcall wait_usec
	ret

lcd_command:
	cbi PORTD, PD2 
	rcall write_2_nibbles
	ldi r24, 39			; 39ms delay
	ldi r25, 0 
	rcall wait_usec 
	ret

lcd_init:
	ldi r24, 40
	ldi r25, 0 
	rcall wait_msec		; initial delay of 40ms
	ldi r24, 0x30		; setting to 8-bit mode
	out PORTD, r24 
	sbi PORTD, PD3		; enable pulse
	cbi PORTD, PD3 
	ldi r24, 39
	ldi r25, 0 
	rcall wait_usec 
	ldi r24, 0x30		; re-setting to 8-bit mode
	out PORTD, r24
	sbi PORTD, PD3		; enable pulse
	cbi PORTD, PD3
	ldi r24, 39
	ldi r25, 0
	rcall wait_usec
	ldi r24, 0x20		; switch to 4-bit mode
	out PORTD, r24
	sbi PORTD, PD3		; enable pulse
	cbi PORTD, PD3
	ldi r24, 39
	ldi r25, 0
	rcall wait_usec
	ldi r24, 0x28		; two-line display in a 5x8 mannner
	rcall lcd_command 
	ldi r24, 0x0c		; hide cursor
	rcall lcd_command
	ldi r24, 0x01		; clear display
	rcall lcd_command
	ldi r24, low(1530)
	ldi r25, high(1530)
	rcall wait_usec
	ldi r24, 0x06		; disable screen shifting
	rcall lcd_command 
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
	cpi r26, 0x09
	breq end
end:
	ret

wait_msec:			; delay routine, calls wait_usec
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