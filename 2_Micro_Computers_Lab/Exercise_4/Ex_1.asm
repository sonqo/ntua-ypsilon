.include "m16def.inc"

one_wire_reset: ; transmits a reset pulse across the wire, returns 1 to r24 if a device is connected, else 0
	sbi DDRA, PA4 ; PA4 configured for output
	cbi PORTA, PA4 ; set PA4 to 0
	ldi r24, low(480)
	ldi r25, high(480)
	rcall wait_usec ; 480usec reset pulse
	cbi DDRA, PA4 ; PA4 configured for input
	cbi PORTA, PA4
	ldi r24, 100
	ldi r25, 0
	rcall wait_usec ; waiting for devices to transmit presence pulse
	in r24, PINA ; sample the line
	push r24
	ldi r24, low(380)
	ldi r25, high(380)
	rcall wait_usec
	pop r25
	clr r24 ; if no device was detected return 0
	sbrs r25, PA4 ; else return 1
	ldi r24, 0x01
	ret

one_wire_transmit_bit: ; transmits bit located in r24 across the wire
	push r24 ; save r24
	sbi DDRA, PA4 ; PA4 configured for output
	cbi PORTA, PA4 ; set PA4 to 0
	ldi r24, 0x02
	ldi r25, 0x00
	rcall wait_usec
	pop r24 ; output bit
	sbrc r24, 0
	sbi PORTA, PA4 ; set PA4 to 1
	sbrs r24, 0
	cbi PORTA, PA4 ; or 0 accordingly
	ldi r24, 58 
	ldi r25, 0
	rcall wait_usec ; wait 58usec for the device to sample the line
	cbi DDRA, PA4 ; PA4 configured for input
	cbi PORTA, PA4
	ldi r24, 0x01
	ldi r25, 0x00
	rcall wait_usec
	ret

one_wire_transmit_byte: ; transmits byte located in r24 across the wire
	mov r26, r24
	ldi r27, 8 ; counter for total shifting of 8 times
_one_more_:
	clr r24
	sbrc r26, 0
	ldi r24, 0x01
	rcall one_wire_transmit_bit
	lsr r26
	dec r27
	brne _one_more_
	ret

one_wire_receive_bit: ; reads bit from wire into LSB of r24
	sbi DDRA, PA4
	cbi PORTA, PA4 ; generate time slot
	ldi r24, 0x02
	ldi r25, 0x00
	rcall wait_usec
	cbi DDRA, PA4 ; release the line
	cbi PORTA, PA4
	ldi r24, 10
	ldi r25, 0
	rcall wait_usec
	clr r24 ; sample the line
	sbic PINA, PA4
	ldi r24, 1
	push r24
	ldi r24, 49 
	ldi r25, 0
	rcall wait_usec
	pop r24
	ret

one_wire_receive_byte: ; reads byte from wire into r24
	ldi r27, 8
	clr r26
loop_:
	rcall one_wire_receive_bit
	lsr r26
	sbrc r24, 0
	ldi r24, 0x80
	or r26, r24
	dec r27
	brne loop_
	mov r24, r26
	ret

wait_msec: ; delay routine, calls wait_usec
	push r24
	push r25
	ldi r24, low(998)
	ldi r25, high(998)
	rcall wait_usec
	pop r25
	pop r24
	sbiw r24,1
	brne wait_msec
	ret

wait_usec:
	sbiw r24,1
	nop
	nop
	nop
	nop
	brne wait_usec
	ret