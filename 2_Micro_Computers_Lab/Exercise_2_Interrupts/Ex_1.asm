.include "m16def.inc"

.org 0x0
	rjmp reset
.org 0x4 ; INT1 address
	rjmp isr1

reset:
	ldi r24, low(RAMEND) ; initializing stack pointer
	out SPL, r24
	ldi r24, high(RAMEND)
	out SPH, r24
	
	ldi r24, (1<<ISC11)|(1<<ISC10) ; initializing INT1
	out MCUCR, r24 ; rising edge of INT1 will cause an interrupt request
	ldi r24, (1<<INT1) ; enabling INT1 interrupt
	out GICR, r24

	clr r24
	out DDRD, r24 ; initialization PORTD for input
	ser r24	
	out DDRA, r24 ; initializing PORTA for output = counter of interrupts				
	out DDRB, r24 ; initializing PORTB for output = timer
	clr r26	; initializing time counter
	clr r27	; initializing interrupts counter

	sei	; enable global interrupts

main:
	out PORTB, r26			
	
	ldi r24, low(500) ; initializing delay to 1/2 a second
	ldi r25, high(500)
	rcall wait_msec
	
	inc r26
	rjmp main

isr1:
	ldi r23, (1<<INTF1) ; button debouncing handling
	out GIFR, r23 ; loading 1 to 7-bit of GIFR for INT1 interrupt
	ldi r24, low(5)	; 5ms dealy
	ldi r25, high(5)
	rcall wait_msec
	in r24, GIFR
	andi r24, (1<<PB7) ; if 7-bit of GIFR is not zero, wait to avoid multiple interrupts
	cpi r24, 0x00 ; else proceed with the interrupt code
	brne isr1
	
	in r23, SREG ; saving SREG to stack
	push r23

	in r23, PIND ; check PIND(7) status
	andi r23, (1<<PB7)
	cpi r23, 0x00			
	breq no_display	; if PIND(7) == 0 don't update PORTA LEDs
	inc r27	; increase interrupts counter by one
	out PORTA, r27 ; else update them

no_display:
	ldi r24, low(500)
	ldi r25, high(500)	
	pop r23 ; retrieving SREG from stack
	out SREG, r23
	reti

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