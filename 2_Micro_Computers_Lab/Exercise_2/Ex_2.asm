.include "m16def.inc"

.org 0x0
	rjmp reset
.org 0x2					; INT0 adress
	rjmp isr0

reset:
	ldi r24, low(RAMEND)	; initializing stack pointer
	out SPL, r24
	ldi r24, high(RAMEND)
	out SPH, r24
	
	ldi r24, (1<<ISC01)|(1<<ISC00)		; rising edge of INT0 will cause an interrupt request
	out MCUCR, r24
	ldi r24, (1<<INT0)		; enabling INT0 interrupt
	out GICR, r24

	clr r24
	out DDRA, r24			; initializing PORTA as input
	ser r24	
	out DDRC, r24			; initializing PORTC for output = sum of interrupts				
	out DDRB, r24			; initializing PORTB for output = timer
	clr r26					; initializing time counter
	sei						; enabling global interrupts

main:
	out PORTB, r26			
	
	ldi r24, low(500)		; initializing delay to 1/2 a second
	ldi r25, high(500)
	rcall wait_msec
	
	inc r26
	rjmp main

isr0:
	ldi r23, (1<<INTF0)		; button debouncing handling
	out GIFR, r23			; loading 1 to 7-bit of GIFR for INT1 interrupt
	ldi r24, low(5)			; 5ms dealy
	ldi r25, high(5)
	rcall wait_msec
	in r24, GIFR
	andi r24, 0x40			; if 6-bit of GIFR is not zero, wait to avoid multiple interrupts
	cpi r24, 0x00			; else proceed with the interrupt code
	brne isr0
	
	in r23, SREG			; saving SREG to stack
	push r23

	clr r27					; number of LEDs counter
	ldi r22, 0x08			; loop flag for calculating hamming distance
	in r23, PINA			; read PINA input
loop:		
	lsr r23
	brcc zero
	lsl r27					; if 1 is met, shift left
	inc r27					; and increment by one
zero:
	dec r22
	cpi r22, 0x00
	brne loop

	out PORTC, r27

	pop r23
	out SREG, r23			; retrieving SREG from stack
	ldi r24, low(500)		; reloading correct delay values before return
	ldi r25, high(500)
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