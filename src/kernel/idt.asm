[BITS 32]

; External ISR handlers
[EXTERN isr_handler]
[EXTERN irq_handler]

; Macro to create ISR stubs without error codes
%macro ISR_NOERRCODE 1
[GLOBAL isr%1]
isr%1:
    cli
    push byte 0         ; Push dummy error code
    push byte %1        ; Push interrupt number
    jmp isr_common_stub
%endmacro

; Macro to create ISR stubs with error codes
%macro ISR_ERRCODE 1
[GLOBAL isr%1]
isr%1:
    cli
    push byte %1        ; Push interrupt number
    jmp isr_common_stub
%endmacro

; Macro to create IRQ stubs
%macro IRQ 2
[GLOBAL irq%1]
irq%1:
    cli
    push byte 0         ; Push dummy error code
    push byte %2        ; Push IRQ number
    jmp irq_common_stub
%endmacro

; CPU Exception ISRs (0-31)
ISR_NOERRCODE 0     ; Divide by zero
ISR_NOERRCODE 1     ; Debug
ISR_NOERRCODE 2     ; NMI
ISR_NOERRCODE 3     ; Breakpoint
ISR_NOERRCODE 4     ; Overflow
ISR_NOERRCODE 5     ; Bound Range Exceeded
ISR_NOERRCODE 6     ; Invalid Opcode
ISR_NOERRCODE 7     ; Device Not Available
ISR_ERRCODE 8       ; Double Fault
ISR_NOERRCODE 9     ; Coprocessor Segment Overrun
ISR_ERRCODE 10      ; Invalid TSS
ISR_ERRCODE 11      ; Segment Not Present
ISR_ERRCODE 12      ; Stack-Segment Fault
ISR_ERRCODE 13      ; General Protection Fault
ISR_ERRCODE 14      ; Page Fault
ISR_NOERRCODE 15    ; Reserved
ISR_NOERRCODE 16    ; x87 Floating-Point Exception
ISR_ERRCODE 17      ; Alignment Check
ISR_NOERRCODE 18    ; Machine Check
ISR_NOERRCODE 19    ; SIMD Floating-Point Exception
ISR_NOERRCODE 20    ; Virtualization Exception
ISR_NOERRCODE 21    ; Reserved
ISR_NOERRCODE 22    ; Reserved
ISR_NOERRCODE 23    ; Reserved
ISR_NOERRCODE 24    ; Reserved
ISR_NOERRCODE 25    ; Reserved
ISR_NOERRCODE 26    ; Reserved
ISR_NOERRCODE 27    ; Reserved
ISR_NOERRCODE 28    ; Reserved
ISR_NOERRCODE 29    ; Reserved
ISR_ERRCODE 30      ; Security Exception
ISR_NOERRCODE 31    ; Reserved

; IRQs (32-47) - Hardware interrupts
IRQ 0, 32           ; System timer
IRQ 1, 33           ; Keyboard
IRQ 2, 34           ; Cascade
IRQ 3, 35           ; COM2
IRQ 4, 36           ; COM1
IRQ 5, 37           ; LPT2
IRQ 6, 38           ; Floppy
IRQ 7, 39           ; LPT1
IRQ 8, 40           ; CMOS RTC
IRQ 9, 41           ; Free
IRQ 10, 42          ; Free
IRQ 11, 43          ; Free
IRQ 12, 44          ; PS2 Mouse
IRQ 13, 45          ; FPU
IRQ 14, 46          ; Primary ATA
IRQ 15, 47          ; Secondary ATA

; Common ISR stub - saves state and calls C handler
isr_common_stub:
    pusha               ; Push all general purpose registers

    mov ax, ds
    push eax            ; Save data segment

    mov ax, 0x10        ; Load kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call isr_handler    ; Call C handler

    pop eax             ; Restore data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa                ; Restore registers
    add esp, 8          ; Clean up error code and ISR number
    sti
    iret                ; Return from interrupt

; Common IRQ stub - saves state and calls C handler
irq_common_stub:
    pusha               ; Push all general purpose registers

    mov ax, ds
    push eax            ; Save data segment

    mov ax, 0x10        ; Load kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call irq_handler    ; Call C handler

    pop eax             ; Restore data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa                ; Restore registers
    add esp, 8          ; Clean up error code and IRQ number
    sti
    iret                ; Return from interrupt
