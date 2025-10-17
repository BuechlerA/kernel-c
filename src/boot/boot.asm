[BITS 16]
[ORG 0x7c00]

KERNEL_OFFSET equ 0x1000

start:
    cli                         ; Disable interrupts during setup
    xor ax, ax                  ; Clear ax
    mov ds, ax                  ; Set data segment to 0
    mov es, ax                  ; Set extra segment to 0
    mov ss, ax                  ; Set stack segment to 0
    mov [BOOT_DRIVE], dl        ; BIOS stores boot drive in DL

    mov bp, 0x9000
    mov sp, bp                  ; Set up stack
    sti                         ; Re-enable interrupts

    mov bx, MSG_REAL_MODE
    call print_string

    call load_kernel            ; Load kernel from disk
    call switch_to_pm           ; Switch to protected mode

    jmp $                       ; Should never reach here

%include "print.asm"
%include "disk.asm"
%include "gdt.asm"
%include "switch_pm.asm"

[BITS 16]
load_kernel:
    mov bx, MSG_LOAD_KERNEL
    call print_string

    mov bx, KERNEL_OFFSET       ; Read from disk and store in 0x1000
    mov dh, 20                  ; Read 20 sectors (10240 bytes, enough for kernel with shell)
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret

[BITS 32]
BEGIN_PM:
    mov ebx, MSG_PROT_MODE
    call print_string_pm        ; Use 32-bit print
    call KERNEL_OFFSET          ; Jump to kernel
    jmp $

BOOT_DRIVE db 0
MSG_REAL_MODE db "Started in 16-bit Real Mode", 0
MSG_PROT_MODE db "Loaded 32-bit Protected Mode", 0
MSG_LOAD_KERNEL db "Loading kernel into memory", 0

; Padding and boot signature
times 510-($-$$) db 0
dw 0xaa55
