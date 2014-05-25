.globl _start
.extern arm_raspberry_pi_early
_start:
    mov sp,#0x8000
	
	mov r1,#1
	lsl r1,#18
	str r1,[r0,#4]
	
	mov r1,#1
	lsl r1,#16
	str r1,[r0,#40]
	
	mov r5, #0xFF
	b hang
    bl arm_raspberry_pi_early
hang: b hang

.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

.globl dummy
dummy:
    bx lr
