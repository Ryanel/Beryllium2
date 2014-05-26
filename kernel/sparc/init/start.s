.section .text
.align 16
.global _start
 
_start:
halt_sparc:
	ba halt_sparc ! Loop forever
		nop
