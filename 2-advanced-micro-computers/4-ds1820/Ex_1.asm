.include "m16def.inc"

reset:
	ldi r24, low(RAMEND) ; initializing stack pointer
	out SPL, r24
	ldi r24, high(RAMEND)
	out SPH, r24		
	ser r24
	out DDRB, r24 ; initializing PORTB for output

main:
	rcall ds1820_routine
	out PORTB, r24
	rjmp main

ds1820_routine:
	rcall one_wire_reset
	sbrs r24, 0 ; if no device is connected, return 0x8000
	rjmp no_device
	ldi r24, 0xCC ; skip device selection
	rcall one_wire_transmit_byte
	ldi r24, 0x44 ; start temperature measurement
	rcall one_wire_transmit_byte
loop:	
	rcall one_wire_receive_bit
	sbrs r24, 0 ; wait until measurement is completed
	rjmp loop
	rcall one_wire_reset
	sbrs r24, 0
	rjmp no_device
	ldi r24, 0xCC
	rcall one_wire_transmit_byte
	ldi r24, 0xBE
	rcall one_wire_transmit_byte
	rcall one_wire_receive_byte
	mov r23, r24 ; LSB of temperature in r23
	rcall one_wire_receive_byte
	mov r25, r24 ; MSB of temperature in r25
	mov r24, r23 ; LSB of temperature in r24
	lsr r24 ; right shift for rounding down temperature	
	rjmp end
no_device:
	ldi r24, 0x00 ; no device code
	ldi r25, 0x80
end:
	ret

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