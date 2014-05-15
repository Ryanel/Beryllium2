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

%include "kernel/ports/assembly/console.inc"

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
; halt
; Halts the processor.
; TODO: Move to error.s
;------------------------------------------------------------------------------
halt:
	cli
	hlt
	jmp halt
;------------------------------------------------------------------------------
; multiboot_fail
; Called if initialising multiboot failed 
; TODO: Move to error.s
;------------------------------------------------------------------------------
multiboot_fail:
	cmp edx, 1
	je multiboot_fail_magic
	;cmp edx, 2 ;Cont...
multiboot_fail_magic:
	;mov edx, {NAME OF STRING}
	jmp halt
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
	mov esi, bootmsg
	call vga_prints
	call vga_test
	call halt
	
bootmsg: db "Beryllium2 Kernel Version 0.0.0.1(0.0.0.5)-[port:asm]",13, 0
