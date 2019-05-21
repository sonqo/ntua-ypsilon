.include "m16def.inc"

reset:
ldi r24, low(RAMEND) ;initialization of stack pointer
out SPL, r24
ldi r24, high(RAMEND)
out SPH, r24

clr r24 ;initialization PORTA as input (0x01)
out DDRA, r24
ser r24 ;activating PORTA pull-ups
out PORTA, r24

out DDRB, r24 ;initialization of PORT B as output

ldi r26, 0b00000001 ;initialization counter to 1

main:
out PORTB, r26 ;output r26 to PORTB

ldi r24, low(1000) ;1 second delay
ldi r25, high(1000) 
rcall wait_msec

in r16, PINA ;read input data from PORTA, store it to r16
cpi r16, 0b00000000 ;if switch is off, jump to main
breq main

lsr r26 ;if switch is on, shift r26 1bit right
cpi r26, 128 ;if r26 lower than 128, continue
brlo main
ldi r26, 1 ;else, reload 1 and continue
rjmp main

wait_msec: ;1msecond delay
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

wait_usec: ;1000msecond delay
sbiw r24 ,1 
nop 
nop 
nop 
nop 
brne wait_usec 
ret 