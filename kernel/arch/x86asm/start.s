;==============================================================================
; Start (Kernel Entry)
;==============================================================================
[BITS 32]   
[GLOBAL mboot_header]
[EXTERN code] 
[EXTERN bss]  
[EXTERN end]  
[GLOBAL start]

;------------------------------------------------------------------------------
; Includes
;------------------------------------------------------------------------------

%include "kernel/ports/assembly/includes.inc"

;------------------------------------------------------------------------------
; Multiboot Header
;------------------------------------------------------------------------------
MBOOT_PAGE_ALIGN    equ 1<<0
MBOOT_MEM_INFO      equ 1<<1
MBOOT_HEADER_MAGIC  equ 0x1BADB002
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

section .__mbHeader
align 4
mboot_header:
	dd  MBOOT_HEADER_MAGIC
	dd  MBOOT_HEADER_FLAGS 
	dd  MBOOT_CHECKSUM     
	dd  mboot_header       
	dd  code               
	dd  bss                
	dd  end                
	dd  start              
	section .bs_stack
align 4

;------------------------------------------------------------------------------
; Stack
;------------------------------------------------------------------------------
section .bootstrap_stack
align 4
stack_bottom:
times 8192 db 0 ;4kb stack
stack_top:

section .text
;------------------------------------------------------------------------------
; start
; The entry point of the kernel
;------------------------------------------------------------------------------
start: ; Entry Point
	mov esp, stack_top ; Setup stack
	
	;Check if the multiboot header is correct
	
	mov edx, 1 ;Boot status is in EDX
	cmp eax,0x2BADB002
	jne multiboot_fail
	
	;We passed, lets print a welcome message
	call vga_clearscreen
	Print bootmsg
	call vga_printregs
	call halt
	
bootmsg: db "Beryllium2 Kernel Version 0.0.0.1(0.0.0.5)-[port:asm]",0xA, 0x0