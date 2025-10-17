[BITS 32]
[EXTERN kernel_main]        ; Declare that kernel_main is external
[GLOBAL kernel_entry]       ; Make kernel_entry visible to linker

kernel_entry:
    call kernel_main        ; Call the C kernel function
    jmp $                   ; Hang if kernel returns
