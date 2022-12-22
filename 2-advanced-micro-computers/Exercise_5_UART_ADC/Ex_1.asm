.include "m16def.inc"

.dseg
	_tmp_: .byte 0x10

.cseg

main:
	ldi r24, low(RAMEND) ; initialize stack pointer
	out SPL, r24
	ldi r24, high(RAMEND)
	out SPH, r24

	ldi XL, low(_tmp_)
	ldi XH, high(_tmp_)
	rcall save_string2RAM 
	rcall usart_init			

	ldi XL, low(_tmp_)
	ldi XH, high(_tmp_)

send2UART:
	ld r24, X+
	cpi r24, '\0'
	breq end_of_string ; if '\0' is met, the string is over
	rcall usart_transmit ; transmit through the USART
	rjmp send2UART

end_of_string:
	rjmp end_of_string

usart_init:
	clr r24	; initialize UCSRA to zero
	out UCSRA, r24
	ldi r24, (1<<RXEN) | (1<<TXEN) ; activate transmitter/receiver
	out UCSRB, r24
	ldi r24, 0 ; baud rate = 9600
	out UBRRH, r24
	ldi r24, 51
	out UBRRL, r24
	ldi r24, (1<<URSEL)|(3<<UCSZ0) ; 8-bit character size,
	out UCSRC, r24 ; 1 stop bit
	ret

usart_transmit:
	sbis UCSRA, UDRE ; check if usart is ready to transmit
	rjmp usart_transmit ; if no check again, else transmit
	out UDR, r24 ; content of r24
	ret

save_string2RAM:
	ldi r16, 'T'
	st X+, r16	
	ldi r16, 'e'
	st X+, r16
	ldi r16, 'a'
	st X+, r16
	ldi r16, 'm'
	st X+, r16
	ldi r16, ' '
	st X+, r16
	ldi r16, '0'
	st X+, r16
	ldi r16, '5'
	st X+, r16
	ldi r16, '\0'
	st X+, r16
	ret
