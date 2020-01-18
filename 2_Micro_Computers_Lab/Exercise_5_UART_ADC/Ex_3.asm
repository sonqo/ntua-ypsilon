.include "m16def.inc"

rjmp reset

.org 0x1C ; ADC address
	rjmp ADC_routine

reset:
	ldi r24, low(RAMEND) ; initializing stack pointer
	out SPL, r24
	ldi r24, high(RAMEND)
	out SPH, r24	

	rcall ADC_init
	rcall usart_init
	
	ser r24	
	out DDRB, r24 ; initializing PORTB for output
	ldi r20, 0x00 ; initializing counter
	out PORTB, r20
	
	sei

main:
	inc r20
	out PORTB, r20 ; increment LED
	
	ldi r24, (1<<ADSC) ; initiate conversion
	out ADCSRA, r24
	
	ldi r24, low(200)
	ldi r25, high(200)
	rcall wait_msec
	rjmp main

ADC_init:
	 ldi r24, (1<<REFS0) ; Vref: Vcc
	 out ADMUX, r24
	 ldi r24, (1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)
	 out ADCSRA, r24
	 ret

ADC_routine:
	in r16, ADCL ;low
	in r17, ADCH ;high
	andi r17, 0x03 ; 10-bit number
	
	/* Multiplication with 5 */
	mov r18, r16
	mov r19, r17
	clc
	rol r18 ; multiply with 2
	rol r19
	clc
	rol r18 ; multiply with 4
	rol r19
	add r18, r16 ; multiply with 5
	adc r19, r17

	/* Deviation by 1024 */
	clc
	ror r19 ; shift word r19-r18 2 bits right
	ror r18
	clc
	ror r19	; integer digits
	ror r18 ; decimal digits

	mov r24, r19 ; move interger digit to UART
	rcall hex_to_ascii
	rcall usart_transmit

	ldi r24, '.' ; move dot symbol '.' to UART 
	rcall usart_transmit 

	ldi r21, 0x00
	rol r18
	brcc 2nd
	ldi r22, 50
	add r21, r22
2nd:
	rol r18
	brcc 3rd
	ldi r22, 25
	add r21, r22
3rd:
	rol r18
	brcc 4th
	ldi r22, 12
	add r21, r22
4th:
	rol r18
	brcc 5th
	ldi r22, 12
	add r21, r22
5th:
	rol r18
	brcc 6th
	ldi r22, 6
	add r21, r22
6th:
	rol r18
	brcc end
	ldi r22, 1
	add r21, r22

end:
	mov r24, r21 ; move 2nd decimal digit to UART
	rcall hex_to_ascii
	rcall usart_transmit
	
	ldi r24, 0x0A ; move new line character to UART  
	rcall usart_transmit

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
