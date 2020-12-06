.data

buffer: .space 32
inputFormat: .asciz "%s"
outputFormat: .asciz "Result: %s\n"

exitMsg: .asciz "Exiting...\n"
inputMsg: .asciz "Input a string of up to 32 characters long:"

.text
.global main

.extern scanf
.extern printf

main:
	push {ip, lr}

	ldr r0, =inputMsg // print input message
	bl printf
	ldr r0, addrInputFormat // read input string
	ldr r1, addrBuffer
	bl scanf

	ldr r1, addrBuffer // check 'Q'
	ldrb r2, [r1]
	cmp r2, #'Q'
	beq exit
	cmp r2, #'q' // check 'q'
	beq exit

	ldr r1, addrBuffer
	ldr r0, addrOutputFormat
	bl printf

	pop {ip, pc} // return

exit:
	ldr r0, =exitMsg
	bl printf

	pop {ip, pc} // return

addrBuffer: .word buffer
addrInputFormat: .word inputFormat
addrOutputFormat: .word outputFormat

