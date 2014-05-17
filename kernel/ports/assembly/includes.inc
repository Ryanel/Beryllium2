;==============================================================================
; VGA Console Includes
;==============================================================================

[EXTERN vga_move_cursor]
[EXTERN vga_printregs]
[EXTERN vga_prints]
[EXTERN vga_printreg32]
[EXTERN vga_clearscreen]
%macro Print 1
push esi
mov esi, %1
call vga_prints
pop esi
%endmacro

;==============================================================================
; Error
;==============================================================================
[EXTERN halt]
[EXTERN multiboot_fail]
