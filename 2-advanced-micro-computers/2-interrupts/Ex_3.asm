.include "m16def.inc"

.org 0x0
	rjmp reset
.org 0x4 ; INT1 address
	rjmp isr1
.org 0x10 ; TIMER1 address
	rjmp timer1

reset:
	ldi r24, low(RAMEND) ; initializing stack pointer
	out SPL, r24
	ldi r24, high(RAMEND)
	out SPH, r24
	
	ldi r24, (1<<ISC11)|(1<<ISC10) ; initializing INT1
	out MCUCR, r24 ; rising edge of INT1 will cause an interrupt request
	ldi r24, (1<<INT1) ; enabling INT1 interrupt
	out GICR, r24

	clr r21	; initialization of binary flag(0,1)
	clr r24
	out DDRA, r24 ; initialization PORTA for input
	ser r24	
	out DDRB, r24 ; initializing PORTB for output = LED
	
	ldi r24, (1<<TOIE1)	; initialization of TCNT1 interrupt for TIMER1
	out TIMSK, r24
	ldi r24, (1<<CS12)|(0<<CS11)|(1<<CS10) ; 1/1024 for 8MHz AVR frequency
	out TCCR1B, r24

	sei	; enable global interrupts

main:
	in r23, PINA
	andi r23, 0x80 ; if MSB(A) is not 1, do nothing
	cpi r23, 0x80
	brne main
	
loop:
	in r20, PINA
	andi r20, 0x80
	cpi r20, 0x00
	brne loop

	ldi r23, 0x85 ; else initialize TIMER1 for 4 seconds interrupt
	out TCNT1H, r23			
	ldi r23, 0xEE
	out TCNT1L, r23
	
	cpi r21, 0x00 ; check flag in case LEDs are already open
	breq first_time

	ser r23	; if flag is 1, open all the other LEDs as well
	out PORTB, r23
	ldi r24, low(500) ; 1/2 a second delay for all LEDs
	ldi r25, high(500)
	rcall wait_msec
	
first_time: ; if LEDs are close, open just the LSB(B)
	inc r21	; set flag to 1
	ldi r23, 0x01
	out PORTB, r23
	rjmp main				
	
isr1:
	ldi r23, (1<<INTF1)	; button debouncing handling
	out GIFR, r23 ; loading 1 to 7-bit of GIFR for INT1 interrupt
	ldi r24, low(5)	; 5ms dealy
	ldi r25, high(5)
	rcall wait_msec
	in r24, GIFR
	andi r24, (1<<PB6) ; if 7-bit of GIFR is not zero, wait to avoid multiple interrupts
	cpi r24, 0x00 ; else proceed with the interrupt code
	brne isr1

	ldi r23, 0x85 ; initialize TIMER1 for 4 seconds interrupt
	out TCNT1H, r23			
	ldi r23, 0xEE
	out TCNT1L, r23
	
	cpi r21, 0x00 ; check if LEDs are already open
	breq first_time_int

	ser r23	; else open all the other LEDs as well
	out PORTB, r23
	ldi r24, low(500) ; 1/2 a second delay
	ldi r25, high(500)
	rcall wait_msec

first_time_int:
	inc r21	; set flag to 1
	ldi r23, 0x01
	out PORTB, r23 ; turn just the MSB(B) on

	ldi r24, low(500) ; reloading correct delay values before return
	ldi r25, high(500)
	reti

timer1:	; TIMER1 rotuine
	clr r21	; set flag back to 0, since all LED's will be turned off
	clr r23
	out PORTB, r23
	reti

wait_msec: ; interrupt routine, calls wait_usec
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