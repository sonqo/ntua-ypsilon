.include "m16def.inc"

main:
	rcall save_string2RAM 
	rcall usart_init			

	clr XL				
	clr XH 

send2UART:
	ld r24, X+
	cpi r24, '\0'
	breq end_of_string ; if '\0' is met, the string is over
	rcall usart_transmit ; transmit through the USART
	rjmp send2UART
end_of_string:
	ret

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
	sts 0x0000, r16
	ldi r16, 'e'
	sts 0x0001, r16
	ldi r16, 'a'
	sts 0x0002, r16
	ldi r16, 'm'
	sts 0x0003, r16
	ldi r16, ' '
	sts 0x0004, r16
	ldi r16, '0'
	sts 0x0005, r16
	ldi r16, '5'
	sts 0x0006, r16
	ldi r16, '\0'
	sts 0x0007, r16
ret