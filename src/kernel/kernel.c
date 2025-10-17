#include "../../include/kernel/idt.h"
#include "../../include/kernel/shell.h"
#include "../../include/drivers/screen.h"
#include "../../include/drivers/keyboard.h"

// Main kernel entry point
void kernel_main() {
    // Initialize IDT and enable interrupts
    idt_init();
    asm volatile ("sti");  // Enable interrupts

    // Initialize and start shell
    shell_init();

    // Infinite loop - interrupts will handle keyboard
    while (1) {
        asm volatile ("hlt");  // Halt CPU until next interrupt
    }
}
