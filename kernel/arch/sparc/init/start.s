.section .text
.align 16
.global _start
.extern sparc_early

.macro PrintString message length
call 1f                      ! put PC on the stack.
mov %o7, %l0               ! use the delay slot to grab it
1: mov 0x60, %l3
add %l3, %o0, %l3
ld [%l3], %l5
mov \message - _start, %l4    ! offset to text to print
add %l4, %l0, %l4            ! absolute address of text to print
mov %l4, %o0                 ! first argument is pointer
mov \length, %o1                ! second argument is number of characters
call %l5                     ! call function
	nop                      ! waste the delay slot for simplicity
.endm

_start:
	PrintString systeminit_s 20
	PrintString systeminit_s 19
halt_sparc:
	ba halt_sparc ! Loop forever
		nop

systeminit_s:
       .asciz "*** System Init ***\n"
