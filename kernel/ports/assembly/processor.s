;==============================================================================
; Processor Initialisation and Management
;==============================================================================

struc gdt_entry_struct

        limit_low:   resb 2
        base_low:    resb 2
        base_middle: resb 1
        access:      resb 1
        granularity: resb 1
        base_high:   resb 1

endstruc
