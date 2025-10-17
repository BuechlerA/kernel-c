[BITS 16]

; Load sectors from disk
; dh = number of sectors to read
; dl = drive number
; bx = memory address to load to
disk_load:
    push dx
    push ax
    push bx
    push cx

    mov ah, 0x02            ; BIOS read sector function
    mov al, dh              ; Number of sectors to read (from dh parameter)
    mov ch, 0x00            ; Cylinder 0
    mov cl, 0x02            ; Start reading from sector 2 (after boot sector)
    push dx                 ; Save original dx
    mov dh, 0x00            ; Head 0
    ; dl still contains drive number from parameter
    ; es:bx already set to buffer address

    ; Try up to 3 times
    mov di, 3               ; Retry counter
.retry:
    int 0x13                ; BIOS disk interrupt
    jnc .success            ; Jump if no error (carry flag clear)

    ; Reset disk system on error
    xor ah, ah
    int 0x13

    dec di
    jnz .retry              ; Retry if counter not zero
    jmp disk_error          ; Failed after retries

.success:
    pop dx                  ; Restore original dx
    pop cx
    pop bx
    pop ax
    pop dx
    ret

disk_error:
    mov bx, DISK_ERROR_MSG
    call print_string
    jmp $

DISK_ERROR_MSG db "Disk read error!", 0
