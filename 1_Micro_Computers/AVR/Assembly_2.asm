.include "m16def.inc"

.DEF A = r16 ;declaration of variables A-D
.DEF B = r17
.DEF C = r18
.DEF nC = r19
.DEF D = r20
.DEF Temp = r21

.DEF TempA = r1
.DEF TempB = r2
.DEF TempC = r3
.DEF TempnC = r4
.DEF TempD = r5

reset:
ldi r24, low(RAMEND) ;initialization of stack pointer
out SPL, r24
ldi r24, high(RAMEND)
out SPH, r24

clr r24 ;initialization PORTA for input
out DDRA, r24
ser r24 ;activating PORTA pull-ups
out PORTA, r24

out DDRB, r24 ;initialization PORTB for output

in Temp, PINA ;Reading A
mov A, Temp
andi A, 0x01

lsr Temp ;Reading B
mov B, Temp
andi B, 0x01

lsr Temp ;Reading C
mov C, Temp
andi C, 0x01

mov nC, C ;Getting complement of C
com nC
andi nC, 0x01

lsr Temp ;Reading D
mov D, Temp
andi D, 0x01

mov TempA, A
mov TempB, B
mov TempC, C
mov TempnC, nC
mov TempD, D

F1:
and A, B ;A = ABC
and A, C
and nC, D ;nC = C'D
or A, nC ;A = ABC + C'D
com A ;A = F1
andi A, 01H
mov r22, A

mov A, TempA ;re-initializing registers
mov B, TempB
mov C, TempC
mov nC, TempnC
mov D, TempD

F2:
or A, B ;A = A + B
or C, D ;C = C + D
and A, C ;A = F2
lsl A
or r22, A

out PORTB, r22