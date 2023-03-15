.data

bufferInput: .space 32
bufferOutput: .space 40

inputFormat: .asciz "%s"
outputFormat: .asciz "Result: %s\n"

exitMsg: .asciz "Exiting...\n"
inputMsg: .asciz "Input a string of up to 32 characters long: "

.text
.global main

.extern scanf
.extern printf

main:
	push {ip, lr}

start:
	ldr r0, =inputMsg // print input message
	bl printf
	ldr r0, addrInputFormat // read input string
	ldr r1, addrBuffer
	bl scanf

	ldr r8, addrOutputFormat // setting up processed output
	ldr r9, addrResult

	ldr r1, addrBuffer // check 'Q'
	ldrb r2, [r1]
	cmp r2, #'Q'
	beq check_exit
	cmp r2, #'q' // check 'q'
	beq check_exit
	b traverse_string

check_exit:
	ldrb r2, [r1, #1] // exit only if input string is a length of 1
	cmp r2, #0
	beq exit

traverse_string:
	ldrb r5, [r1], #1
	cmp r5, #0 // if EOS, print and re-loop
	beq finish

	bl modifier // else transform character
	b traverse_string // and keep traversing

modifier:
	cmp r5, #'0'
	blt modified
	cmp r5, #'9'
	ble number
	cmp r5, #'A'
	blt modified
	cmp r5, #'Z'
	addle r5, #32 // upper character
	ble modified
	cmp r5, #'a'
	blt modified
	cmp r5, #'z'
	suble r5, #32 // lower character
	ble modified
number:	cmp r5, #'4'
	addle r5, #5
	subgt r5, #5
modified:
	strb r5, [r9], #1
	bx lr

finish:
	mov r5, #0 // before printing the result, update buffer
	strb r5, [r9] // with EOS character
	ldr r0, addrOutputFormat
	ldr r1, addrResult
	bl printf
	b start

exit:
	ldr r0, =exitMsg
	bl printf

	pop {ip, pc} // return

addrBuffer: .word bufferInput
addrResult: .word bufferOutput
addrInputFormat: .word inputFormat
addrOutputFormat: .word outputFormat
