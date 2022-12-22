.include "m16def.inc"

reset:
ldi r24, low(RAMEND) ;initialization of stack pointer
out SPL, r24
ldi r24, high(RAMEND)
out SPH, r24

clr r24 ;initialization PORTA as input (0x01)
out DDRA, r24

out DDRB, r24 ;initialization of PORT B as output

ldi r26, 0b00000001 ;initialization counter to 1

main1:
out PORTB, r26 ;output r26 to PORTB

ldi r24, low(500) ;half a second delay
ldi r25, high(500) 
rcall wait_msec

in r16, PINA ;read input data from PORTA, store it to r16
cpi r16, 0b00000000 ;if switch is off do nothing, wait
breq main1

lsl r26 ;if switch is on, shift r26 1bit left
cpi r26, 128 ;if r26 lower than 128, continue
brlo main1 
rjmp main2 ;else jump to shifting right

main2:
out PORTB, r26 ;output r26 to PORTB

ldi r24, low(500) ;half a second delay
ldi r25, high(500) 
rcall wait_msec

in r16, PINA ;read input data from PORTA, store it to r16
cpi r16, 0b00000000 ;if switch is off do nothing, wait
breq main2

lsr r26 ;if switch is on, shift r26 1bit right
cpi r26, 1 ;if r26 higher than 1, continue
brhi main2 
rjmp main1 ;else jump to shifting left

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