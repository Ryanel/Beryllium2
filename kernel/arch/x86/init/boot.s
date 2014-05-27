;
; boot.s
; The entry point of the kernel
; By (as in edited by) Corwin Mcknight -- Credit to James Molloy (jamesmolloy.co.uk)
;

MBOOT_PAGE_ALIGN    equ 1<<0    ; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 1<<1    ; Provide your kernel with memory info
MBOOT_VIDEO_MODE    equ 1<<0    ; Provide your kernel with memory info
MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; Multiboot Magic value
; NOTE: We do not use MBOOT_AOUT_KLUDGE. It means that GRUB does not
; pass us a symbol table.
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO | MBOOT_VIDEO_MODE
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)


[BITS 32]                       ; All instructions should be 32-bit.

[GLOBAL mboot]                  ; Make 'mboot' accessible from C.
[EXTERN code]                   ; Start of the '.text' section.
[EXTERN bss]                    ; Start of the .bss section.
[EXTERN end]                    ; End of the last loadable section.
section .__mbHeader
align 4
mboot:
	dd  MBOOT_HEADER_MAGIC        ; GRUB will search for this value on each
								; 4-byte boundary in your kernel file
	dd  MBOOT_HEADER_FLAGS        ; How GRUB should load your file / settings
	dd  MBOOT_CHECKSUM            ; To ensure that the above values are correct

	dd  mboot                     ; Location of this descriptor
	dd  code                      ; Start of kernel '.text' (code) section.
	dd  bss                       ; End of kernel '.data' section.
	dd  end                       ; End of kernel.
	dd  start                     ; Kernel entry point (initial EIP).
	section .bs_stack
align 4
stack_bottom:
times 16384 db 0
stack_top:

section .text
[GLOBAL start]
[EXTERN x86_early]
start:
	mov esp, stack_top
	mov ebp, 0
	push ebx
	push eax
	
	;Disable VGA Blinking
	mov dx, 0x3DA
	in al, dx
	mov dx, 0x3C0
	mov al, 0x30
	out dx, al
	inc dx
	in al, dx
	and al, 0xF7
	dec dx
	out dx, al 
	
	cli
	call x86_early
	pop eax
	pop ebx
	jmp $
