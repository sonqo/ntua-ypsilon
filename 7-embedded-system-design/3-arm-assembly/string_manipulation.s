.text
.align 4

.global strlen
.type strlen, %function
strlen:
    push {fp, lr}
    mov r5, #0 // counter
strlen_loop:
    ldrb r1, [r0], #1 // get first chatacter, increment pointer position by one
    addne r5, r5, #1
    bne strlen_loop
    mov r0, r5
    pop {fp, pc}
    bx lr

.global strcat
.type strcat, %function
strcat:
    push {fp, lr}
    mov r5, r0
strcat_traverse:
    ldrb r2, [r0], #1
    cmp r2, #0
    bne strcat_traverse
    sub r0, r0, #1 // ignore EOS character
strcat_loop:
    ldrb r2, [r1], #1
    strb r2, [r0], #1
    cmp r2, #0
    bne strcat_loop
    mov r0, r5
    pop {fp, pc}
    bx lr

.global strcpy
.type strcpy, %function
strcpy:
    push {fp, lr}
    mov r5, r0 // dst to r5
strcpy_loop:
    ldrb r2, [r1], #1 // get character from s2
    strb r2, [r0], #1 // copy to s1
    cmp r2, #0
    bne strcat_loop
    mov r0, r5
    pop {fp, pc}
    bx lr

.global strcmp
.type strcmp, %function
strcmp:
    push {fp, lr}
    ldrb r2, [r0], #1
    ldrb r3, [r1], #1
    cmp r2, r3
    movgt r0, #1
    movlt r0, #-1
    bne strcmp_finish // unequal character found
    cmp r2, #0
    beq strcmp_finish
    mov r0, #0
strcmp_finish:
    pop {fp, pc}
    bx lr
