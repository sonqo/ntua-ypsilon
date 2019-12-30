.include "m16def.inc"

.org 0x1C ; ADC address
	rjmp ADC_routine

reset:
	rcall usart_init
	rcall ADC_init
	ser r24	
	out DDRB, r24 ; initializing PORTB for output
	ldi r20, 0x00 ; initializing counter
	sei

main:
	ldi r24, (1<<ADSC) ; initiate conversion
	out ADCSRA, r24
	out PORTB, 20
	ldi r24, low(500)
	ldi r25, high(500)
	rcall wait_msec
	rjmp main

 ADC_init:
	 ldi r24, (1<<REFS0) ; Vref: Vcc
	 out ADMUX, r24
	 ldi r24, (1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)
	 out ADCSRA, r24
	 ret

ADC_routine:
	/* INSERT CODE HERE */
	inc r20
	reti

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

wait_msec: ; delay routine, calls wait_usec
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