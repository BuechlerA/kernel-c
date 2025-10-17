[BITS 16]

; Print string in real mode
; bx = address of string
print_string:
    pusha
    mov ah, 0x0e            ; BIOS teletype output
.loop:
    mov al, [bx]
    cmp al, 0
    je .done
    int 0x10                ; BIOS interrupt
    add bx, 1
    jmp .loop
.done:
    mov al, 0x0a            ; newline
    int 0x10
    mov al, 0x0d            ; carriage return
    int 0x10
    popa
    ret

[BITS 32]
VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

; Print string in protected mode
print_string_pm:
    pusha
    mov edx, VIDEO_MEMORY
.loop:
    mov al, [ebx]
    mov ah, WHITE_ON_BLACK
    cmp al, 0
    je .done
    mov [edx], ax
    add ebx, 1
    add edx, 2
    jmp .loop
.done:
    popa
    ret
