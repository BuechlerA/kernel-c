[BITS 16]

; Switch to protected mode
switch_to_pm:
    cli                         ; Disable interrupts
    lgdt [gdt_descriptor]       ; Load GDT descriptor

    mov eax, cr0
    or eax, 0x1                 ; Set PE (Protection Enable) bit in CR0
    mov cr0, eax

    jmp CODE_SEG:init_pm        ; Far jump to 32-bit code

[BITS 32]
init_pm:
    ; Update segment registers
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Set up stack
    mov ebp, 0x90000
    mov esp, ebp

    call BEGIN_PM               ; Call the label in boot.asm
