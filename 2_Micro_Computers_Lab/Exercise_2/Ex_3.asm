.include "m16def.inc"

.org 0x0
	rjmp reset
.org 0x4					; INT1 address
	rjmp isr1

reset:
	ldi r24, low(RAMEND)	; initializing stack pointer
	out SPL, r24
	ldi r24, high(RAMEND)
	out SPH, r24
	
	ldi r24, (1<<ISC11)|(1<<ISC10)		; rising edge of INT1 will cause an interrupt request
	out MCUCR, r24
	ldi r24, (1<<INT1)		; enabling INT1 interrupt
	out GICR, r24

	clr r24
	out DDRA, r24			; initialization PORTA for input
	ser r24	
	out DDRB, r24			; initializing PORTB for output = LED
	sei						; enable global interrupts

main:
	in r23, PINA
	andi r23, (1<<PB7)
	cpi r23, 0x00
	breq main
	rjmp isr1

isr1:
	