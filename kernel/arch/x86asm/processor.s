;==============================================================================
; Processor Initialisation and Management
;==============================================================================

;Taken shamelessly from the brokenthorn tutorials
gdt_data: 
 
; Null descriptor (Offset: 0x0)--Remember each descriptor is 8 bytes!
	dd 0 				; null descriptor
	dd 0 
 
; Kernel Space code (Offset: 0x8 bytes)
	dw 0FFFFh 			; limit low
	dw 0 				; base low
	db 0 				; base middle
	db 10011010b 			; access - Notice that bits 5 and 6 (privilege level) are 0 for Ring 0
	db 11001111b 			; granularity
	db 0 				; base high
 
; Kernel Space data (Offset: 16 (0x10) bytes
	dw 0FFFFh 			; limit low (Same as code)10:56 AM 7/8/2007
	dw 0 				; base low
	db 0 				; base middle
	db 10010010b 			; access - Notice that bits 5 and 6 (privilege level) are 0 for Ring 0
	db 11001111b 			; granularity
	db 0				; base high
 
; User Space code (Offset: 24 (0x18) bytes)
	dw 0FFFFh 			; limit low
	dw 0 				; base low
	db 0 				; base middle
	db 11111010b 			; access - Notice that bits 5 and 6 (privilege level) are 11b for Ring 3
	db 11001111b 			; granularity
	db 0 				; base high
 
; User Space data (Offset: 32 (0x20) bytes
	dw 0FFFFh 			; limit low (Same as code)10:56 AM 7/8/2007
	dw 0 				; base low
	db 0 				; base middle
	db 11110010b 			; access - Notice that bits 5 and 6 (privilege level) are 11b for Ring 3
	db 11001111b 			; granularity
	db 0				; base high
	