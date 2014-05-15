;==============================================================================
; VGA Console Functions
;==============================================================================

VGA_CRTC_ADDR 		equ 0x3D4
VGA_WIDTH 			equ 80
VGA_HEIGHT			equ 25

; Global Variables
console_x db 0
console_y db 0

hexstr db '0123456789ABCDEF'
outstr32 db '0x00000000', 0
reg32 dd 0               

;------------------------------------------------------------------------------
; vga_prints
; Prints a string to vga console
;------------------------------------------------------------------------------
[GLOBAL vga_prints]
dochar:
    call cprint            
vga_prints:
    mov eax, [esi]         
    lea esi, [esi+1]
	cmp al, 13
	je vga_prints_newline
    cmp al, 0
    jne dochar
	jmp vga_prints_end         
vga_prints_newline:    
    add byte [console_y], 1
    mov byte [console_x], 0
vga_prints_end:
    ret
 
cprint:
    mov ah, 0x0F ;Attr
    mov ecx, eax
    movzx eax, byte [console_y]
    mov edx, 160
    mul edx
    movzx ebx, byte [console_x]
    shl ebx, 1            
 
    mov edi, 0xb8000      
    add edi, eax          
    add edi, ebx          
 
    mov eax, ecx          
    mov word [ds:edi], ax
    add byte [console_x], 1    
 
    ret

;------------------------------------------------------------------------------
; vga_printreg32
; Prints a 32 bit register number
;------------------------------------------------------------------------------
vga_printreg32:
    mov edi, outstr32
	inc edi
	inc edi
    mov eax, [reg32]
    mov esi, hexstr
    mov ecx, 8               ; eight nibbles
 
hexloop:
    rol eax, 4               ; leftmost will
    mov ebx, eax             ; become rightmost
    and ebx, 0x0f ;
    mov bl, [esi + ebx]      ; index into hexstr
    mov [edi], bl
    inc edi
    dec ecx
    jnz hexloop
    mov esi, outstr32
    call vga_prints

	ret
 
;------------------------------------------------------------------------------
; vga_test
; Adds a Blue and White "Hi" to the screen. Used as a early boot test
;------------------------------------------------------------------------------
vga_test_stack_msg: db "Stack is at:", 0
[GLOBAL vga_test]
vga_test:
	mov esi, vga_test_stack_msg
	call vga_prints
	mov [reg32], esp
	call vga_printreg32
	ret

;------------------------------------------------------------------------------
; vga_move_cursor
; This assumes 80x25 text mode
;------------------------------------------------------------------------------
[GLOBAL vga_move_cursor]
vga_move_cursor:
	push ecx
	push eax
	;Generate offset for VGA Firmware
	mov eax, console_y
	imul ecx, eax, VGA_WIDTH
	pop eax
	add ecx, console_x
	pop ecx
	;TODO: Set VGA Firmware Cursor Position
	ret