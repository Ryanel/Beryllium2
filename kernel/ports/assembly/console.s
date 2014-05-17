;==============================================================================
; VGA Console Functions
;==============================================================================

;------------------------------------------------------------------------------
; Includes
;------------------------------------------------------------------------------

CHAR_NEWLINE 		equ 0xA
VGA_CRTC_ADDR 		equ 0x3D4
VGA_WIDTH 			equ 80
VGA_HEIGHT			equ 25

DISP_ATTRIBUTE		equ 0x1F

; Global Variables
console_x db 0
console_y db 0

hexstr db '0123456789ABCDEF'
outstr32 db '0x00000000', 0
reg32 dd 0               
;------------------------------------------------------------------------------
; vga_clearscreen
; Clears the screen
;------------------------------------------------------------------------------
[GLOBAL vga_clearscreen]
vga_clearscreen:
	pusha
	cld
	mov	edi, 0xB8000
	mov	ecx, 2000
	mov	ah, DISP_ATTRIBUTE
	mov	al, ' '	
	mov	byte [console_x], 0
	mov	byte [console_y], 0
	rep	stosw
	popa
	ret

;------------------------------------------------------------------------------
; vga_forcenewline
; Forces a newline
;------------------------------------------------------------------------------
vga_forcenewline:
	push eax
	mov eax, VGA_HEIGHT
	cmp eax, console_y
	je vga_forcenewline_scroll
    add byte [console_y], 1
    mov byte [console_x], 0
	pop eax
	ret
vga_forcenewline_scroll:
	pop eax
	mov byte [console_x], 0 ;TODO: Scrolling
	ret
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
	cmp al, CHAR_NEWLINE
	je vga_prints_newline
    cmp al, 0
    jne dochar
	jmp vga_prints_end         
vga_prints_newline:    
    call vga_forcenewline
vga_prints_end:
	call vga_move_cursor
    ret
 
cprint:
    mov ah, DISP_ATTRIBUTE ;Attr
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
 
vga_linestring: db "--------------------------------------------------------------------------------", 0
vga_printline:
	pusha
	mov esi, vga_linestring
	call vga_prints
	call vga_forcenewline
	popa
	ret
;------------------------------------------------------------------------------
; vga_test
; Adds a Blue and White "Hi" to the screen. Used as a early boot test
;------------------------------------------------------------------------------
vga_test_eax_msg: db " eax:", 0
vga_test_ebx_msg: db " ebx:", 0
vga_test_ecx_msg: db " ecx:", 0
vga_test_edx_msg: db " edx:", 0
vga_test_ebp_msg: db " ebp:", 0
vga_test_esp_msg: db " esp:", 0
vga_test_esi_msg: db " esi:", 0
vga_test_edi_msg: db " edi:", 0
%macro PrintReg 2
mov esi, %2
call vga_prints
mov [reg32], %1
call vga_printreg32
;call vga_forcenewline
%endmacro
[GLOBAL vga_printregs]
vga_printregs:
	call vga_printline
	PrintReg eax, vga_test_eax_msg
	PrintReg ebx, vga_test_ebx_msg
	PrintReg ecx, vga_test_ecx_msg
	PrintReg edx, vga_test_edx_msg
	call vga_forcenewline
	PrintReg esp, vga_test_esp_msg
	PrintReg ebp, vga_test_ebp_msg
	PrintReg esi, vga_test_esi_msg
	PrintReg edi, vga_test_edi_msg
	call vga_forcenewline
	call vga_printline
	ret

;------------------------------------------------------------------------------
; vga_move_cursor
; This assumes 80x25 text mode
;------------------------------------------------------------------------------
[GLOBAL vga_move_cursor]
vga_move_cursor:
	pusha	
	xor	eax, eax
	mov	ecx, 80
	mov	al, [console_y]
	mul	ecx	
	add	al, [console_x]
	mov	ebx, eax
	
	mov	al, 0x0f
	mov	dx, 0x03D4
	out	dx, al
        	
	mov	al, bl	
	mov	dx, 0x03D5
	out	dx, al	
        	
	xor	eax, eax
        	
	mov	al, 0x0e
	mov	dx, 0x03D4
	out	dx, al
        	
	mov	al, bh	
	mov	dx, 0x03D5
	out	dx, al	
        	
	popa
	ret