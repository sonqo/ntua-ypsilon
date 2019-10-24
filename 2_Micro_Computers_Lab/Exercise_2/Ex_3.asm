.include "m16def.inc"

.org 0x0
	rjmp reset
.org 0x4					; INT1 address
	rjmp isr1
.org 0x10
	rjmp timer1

reset:
	ldi r24, low(RAMEND)	; initializing stack pointer
	out SPL, r24
	ldi r24, high(RAMEND)
	out SPH, r24
	
	ldi r24, (1<<ISC11)|(1<<ISC10)		; rising edge of INT1 will cause an interrupt request
	out MCUCR, r24
	ldi r24, (1<<INT1)		; enabling INT1 interrupt
	out GICR, r24

	clr r21					; initialization of flag = 0
	clr r24
	out DDRA, r24			; initialization PORTA for input
	ser r24	
	out DDRB, r24			; initializing PORTB for output = LED
	
	ldi r24, (1<<TOIE1)		; initialization of TCNT1 interrupt for TIMER1
	out TIMSK, r24
	ldi r24, (1<<CS12)|(0<<CS11)|(1<<CS10)		;1/1024
	out TCCR1B, r24
	sei						; enable global interrupts

main:
	in r23, PINA
	andi r23, (1<<PB7)		; if MSB(A) is not 1, do nothing
	cpi r23, 0x80
	brne main
	
	ldi r23, 0x85			; initialize TIMER1 for 4 seconds interrupt
	out TCNT1H, r23			
	ldi r23, 0xEE
	out TCNT1L, r23
	
	cpi r21, 0x00			; check if LEDs are already open
	breq first_time

	ser r23					; if LSB(B) is 1, open all the other LEDs as well
	out PORTB, r23
	ldi r24, low(500)		; 1/2 a second delay for all LEDs
	ldi r25, high(500)
	rcall wait_msec
	
first_time:					; if LEDs are close, open just the LSB(B)
	ldi r21, 0x01			; set flag to 1
	ldi r23, 0x01
	out PORTB, r23
	rjmp main				
	
isr1:
	ldi r23, (1<<INTF0)		; button debouncing handling
	out GIFR, r23			; loading 1 to 7-bit of GIFR for INT1 interrupt
	ldi r24, low(5)			; 5ms dealy
	ldi r25, high(5)
	rcall wait_msec
	in r24, GIFR
	andi r24, (1<<PB6)		; if 7-bit of GIFR is not zero, wait to avoid multiple interrupts
	cpi r24, 0x00			; else proceed with the interrupt code
	brne isr1

	in r23, SREG			; saving SREG to stack
	push r23

	ldi r23, 0x85			; initialize TIMER1 for 4 seconds interrupt
	out TCNT1H, r23			
	ldi r23, 0xEE
	out TCNT1L, r23
	
	cpi r21, 0x00			; check if LEDs are already open
	breq first_time_int

	ser r23					; if LSB(B) is 1, open all the other LEDs as well
	out PORTB, r23
	ldi r24, low(500)
	ldi r25, high(500)
	rcall wait_msec
	
first_time_int:
	ldi r21, 0x01			; set flag to 1
	ldi r23, 0x01
	out PORTB, r23			 

	pop r23
	out SREG, r23			; retrieving SREG from stack
	ldi r24, low(500)		; reloading correct delay values before return
	ldi r25, high(500)
	reti

timer1:
	in r20, SREG			; save SREG to stack
	push r20
	ldi r21, 0x00			; set flag back to 0, since all LED's will turn off
	clr r23
	out PORTB, r23
	pop r20					; retrieve SREG from stack
	out SREG, r20
	reti

wait_usec:
	sbiw r24, 1 
	nop 
	nop 
	nop 
	nop 
	brne wait_usec 
	ret 

wait_msec:
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