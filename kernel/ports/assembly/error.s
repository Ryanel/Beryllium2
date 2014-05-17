;==============================================================================
; Error Handlers
;==============================================================================

;Requirements
[EXTERN vga_prints]
%macro Print 1
push esi
mov esi, %1
call vga_prints
pop esi
%endmacro

;------------------------------------------------------------------------------
; halt
; Halts the processor.
;------------------------------------------------------------------------------
[GLOBAL halt]
halt:
	Print haltmsg
	cli
	hlt
	jmp halt
;------------------------------------------------------------------------------
; multiboot_fail
; Called if initialising multiboot failed 
;------------------------------------------------------------------------------
[GLOBAL multiboot_fail]
multiboot_fail:
	cmp edx, 1
	je multiboot_fail_magic
	;cmp edx, 2 ;Cont...
	Print multiboot_unkn_failed_msg
	jmp halt
multiboot_fail_magic:
	Print multiboot_magic_failed_msg
	jmp halt
	
haltmsg: db "halt: Processor Halting...",0xA, 0
multiboot_magic_failed_msg: db "multiboot: Magic number was NOT 0x2BADB002. Halting immediately...", 0xA,0x0
multiboot_unkn_failed_msg: db "multiboot: An Unknown Error Occured", 0xA, 0x0