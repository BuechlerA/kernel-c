/* Entry point of the kernel */
ENTRY(kernel_entry)

/* Memory layout */
SECTIONS
{
    /* Kernel starts at 0x1000 (as loaded by bootloader) */
    . = 0x1000;

    /* Code section */
    .text : {
        *(.text)
    }

    /* Read-only data */
    .rodata : {
        *(.rodata)
    }

    /* Initialized data */
    .data : {
        *(.data)
    }

    /* Uninitialized data (BSS) */
    .bss : {
        *(COMMON)
        *(.bss)
    }
}
