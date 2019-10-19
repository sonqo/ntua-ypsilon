.include "m16def.inc"

.org 0x0
rjmp reset
.org 0x4
rjmp isr1

reset:
	ldi r24, low(RAMEND)	; initializing stack pointer
	out SPL, r24
	ldi r24, high(RAMEND)
	out SPH, r24
	
	ldi r24, (1<<ISC11)|(1<<ISC10)		; rising edge of INT1 will cause an interrupt request
	out MCUCR, r24
	ldi r24, (1<<INT0)		; enabling INT1 interrupt
	out GICR, r24
	sei

	ser r24	
	out DDRA, r24			; initializing PORTA for output = counter of interrupts				
	out DDRB, r24			; initializing PORTB for output = timer
	clr r26					; initializing time counter
	clr r27					; initializing interrupts counter

main:
	out PORTB, r26			
	
	ldi r24, low(500)		; initializing delay to 1/2 a second
	ldi r25, high(500)
	rcall wait_msec
	
	inc r26
	rjmp main

isr1:
	ldi r24, (1<<INTF1)		; loading 1 to 7-bit of GIFR
	out GIFR, r24
	ldi r24, low(0005)		; 5ms dealy
	ldi r25, high(0005)
	rcall wait_msec
	in r24, GIFR
	ori r24, (1<<PB7)		; if 7-bit of GIFR is not zero, wait to avoid multiple interrupts
	cpi r24, 0x00			; else proceed with the interrupt code
	brne isr1
	
	in r23, SREG			; saving SREG to stack
	push r23
	push r24
	push r25
	inc r27
	out PORTA, r27
	pop r25
	pop r24
	pop r23					; retrieving SREG from stack
	out SREG, r23
	reti

wait_msec:					; delay routine, calls wait_usec
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