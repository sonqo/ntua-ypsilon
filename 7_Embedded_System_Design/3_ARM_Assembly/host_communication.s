.data

openFlag: .word 0x902
filePort: .asciz "/dev/ttyAMA0"

bufferInput: .space 64
counterAccumulator: .space 128

inputFormat: .asciz "%s"
outputFormatv1: .ascii "The most frequent character is %c and it appears %d times.\n"

.text
.global main

.extern printf

main:
	push {ip, lr}
	ldr r0, =filePort // load file location
	ldr r1, =openFlag // load open flags
	ldr r2, =0777 // permissios
	mov r7, #5 // open system call
	swi 0

	mov r5, r0 // save file descriptor on r5

read:
	mov r0, r5 // file descriptor
	ldr r1, addrBuffer // input buffer
	mov r2, #64 // BUFFER_SIZE
	mov r7, #3 // read system call
	swi 0
	cmp r0, #0 // check if n_read equals zero
	beq read // if yes, nothing was read

	mov r6, r0 // n_read
	sub r6, #2 // exlude EOS character, zero indexing

	mov r7, #0 // counter pointer
	ldr r1, addrBuffer // input string
	ldr r8, addrCounter // counter

traverse:
	cmp r7, r6 // if all characters are visited, calculate max
	beq max_init
	ldrb r9, [r1], #1 // current character in r9, increment pointer
	cmp r9, #32 // ignore space character
	addeq r7, r7, #1
	beq traverse
	ldrb r10, [r8, r9] // get character position on counter array
	add r10, r10, #1 // increase it's count by one
	strb r10, [r8, r9] // update it's count
	add r7, r7, #1
	b traverse // traverse the rest of the input

max_init:
	mov r7, #0 // counter pointer
	ldr r8, addrCounter // counter array
	mov r10, #0 // initialize max to zero
	mov r11, #0 // initialize character to zero
max_loop:
	cmp r7, #128
	beq finish
	ldrb r12, [r8], #1 // get current character and increment by one
	cmp r10, r12
	beq max_equal
	movlt r10, r12 // save max
	movlt r11, r7 // save relative position
	add r7, r7, #1
	b max_loop
max_equal:
	cmp r11, r7
	movgt r10, r12 // keep character with the smaller ASCII code
	movgt r11, r7
	add r7, r7, #1
	b max_loop

finish:
	ldr r0, addrOutputFormatv1
	mov r1, r11
	mov r2, r10
	bl printf

	pop {ip, pc}

addrBuffer: .word bufferInput
addrInputFormat: .word inputFormat
addrCounter: .word counterAccumulator
addrOutputFormatv1: .word outputFormatv1
