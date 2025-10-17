#include "../../include/kernel/idt.h"
#include "../../include/lib/io.h"

// IDT entry structure
struct idt_entry {
    unsigned short base_low;
    unsigned short selector;
    unsigned char zero;
    unsigned char flags;
    unsigned short base_high;
} __attribute__((packed));

// IDT pointer structure
struct idt_ptr {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

// Declare IDT and pointer
struct idt_entry idt[256];
struct idt_ptr idtp;

// External ISR declarations from idt.asm
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

// IRQ declarations
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

// Set an IDT gate
void idt_set_gate(unsigned char num, unsigned int base, unsigned short selector, unsigned char flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector = selector;
    idt[num].zero = 0;
    idt[num].flags = flags;
}

// Remap PIC (Programmable Interrupt Controller)
void irq_remap(void) {
    // ICW1 - Begin initialization
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // ICW2 - Remap offset address of IDT
    outb(0x21, 0x20);  // Master PIC offset to 32
    outb(0xA1, 0x28);  // Slave PIC offset to 40

    // ICW3 - Setup cascading
    outb(0x21, 0x04);  // Tell master PIC there's a slave at IRQ2
    outb(0xA1, 0x02);  // Tell slave PIC its cascade identity

    // ICW4 - Environment info
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // Mask all interrupts except keyboard (IRQ1)
    outb(0x21, 0xFD);  // 11111101 - Enable IRQ1 (keyboard)
    outb(0xA1, 0xFF);  // 11111111 - Disable all on slave
}

// Initialize IDT
void idt_init(void) {
    // Set up IDT pointer
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base = (unsigned int)&idt;

    // Clear IDT
    for (int i = 0; i < 256; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    // Remap IRQs
    irq_remap();

    // Install ISRs for CPU exceptions (0-31)
    idt_set_gate(0, (unsigned int)isr0, 0x08, 0x8E);
    idt_set_gate(1, (unsigned int)isr1, 0x08, 0x8E);
    idt_set_gate(2, (unsigned int)isr2, 0x08, 0x8E);
    idt_set_gate(3, (unsigned int)isr3, 0x08, 0x8E);
    idt_set_gate(4, (unsigned int)isr4, 0x08, 0x8E);
    idt_set_gate(5, (unsigned int)isr5, 0x08, 0x8E);
    idt_set_gate(6, (unsigned int)isr6, 0x08, 0x8E);
    idt_set_gate(7, (unsigned int)isr7, 0x08, 0x8E);
    idt_set_gate(8, (unsigned int)isr8, 0x08, 0x8E);
    idt_set_gate(9, (unsigned int)isr9, 0x08, 0x8E);
    idt_set_gate(10, (unsigned int)isr10, 0x08, 0x8E);
    idt_set_gate(11, (unsigned int)isr11, 0x08, 0x8E);
    idt_set_gate(12, (unsigned int)isr12, 0x08, 0x8E);
    idt_set_gate(13, (unsigned int)isr13, 0x08, 0x8E);
    idt_set_gate(14, (unsigned int)isr14, 0x08, 0x8E);
    idt_set_gate(15, (unsigned int)isr15, 0x08, 0x8E);
    idt_set_gate(16, (unsigned int)isr16, 0x08, 0x8E);
    idt_set_gate(17, (unsigned int)isr17, 0x08, 0x8E);
    idt_set_gate(18, (unsigned int)isr18, 0x08, 0x8E);
    idt_set_gate(19, (unsigned int)isr19, 0x08, 0x8E);
    idt_set_gate(20, (unsigned int)isr20, 0x08, 0x8E);
    idt_set_gate(21, (unsigned int)isr21, 0x08, 0x8E);
    idt_set_gate(22, (unsigned int)isr22, 0x08, 0x8E);
    idt_set_gate(23, (unsigned int)isr23, 0x08, 0x8E);
    idt_set_gate(24, (unsigned int)isr24, 0x08, 0x8E);
    idt_set_gate(25, (unsigned int)isr25, 0x08, 0x8E);
    idt_set_gate(26, (unsigned int)isr26, 0x08, 0x8E);
    idt_set_gate(27, (unsigned int)isr27, 0x08, 0x8E);
    idt_set_gate(28, (unsigned int)isr28, 0x08, 0x8E);
    idt_set_gate(29, (unsigned int)isr29, 0x08, 0x8E);
    idt_set_gate(30, (unsigned int)isr30, 0x08, 0x8E);
    idt_set_gate(31, (unsigned int)isr31, 0x08, 0x8E);

    // Install IRQs (32-47)
    idt_set_gate(32, (unsigned int)irq0, 0x08, 0x8E);
    idt_set_gate(33, (unsigned int)irq1, 0x08, 0x8E);
    idt_set_gate(34, (unsigned int)irq2, 0x08, 0x8E);
    idt_set_gate(35, (unsigned int)irq3, 0x08, 0x8E);
    idt_set_gate(36, (unsigned int)irq4, 0x08, 0x8E);
    idt_set_gate(37, (unsigned int)irq5, 0x08, 0x8E);
    idt_set_gate(38, (unsigned int)irq6, 0x08, 0x8E);
    idt_set_gate(39, (unsigned int)irq7, 0x08, 0x8E);
    idt_set_gate(40, (unsigned int)irq8, 0x08, 0x8E);
    idt_set_gate(41, (unsigned int)irq9, 0x08, 0x8E);
    idt_set_gate(42, (unsigned int)irq10, 0x08, 0x8E);
    idt_set_gate(43, (unsigned int)irq11, 0x08, 0x8E);
    idt_set_gate(44, (unsigned int)irq12, 0x08, 0x8E);
    idt_set_gate(45, (unsigned int)irq13, 0x08, 0x8E);
    idt_set_gate(46, (unsigned int)irq14, 0x08, 0x8E);
    idt_set_gate(47, (unsigned int)irq15, 0x08, 0x8E);

    // Load IDT
    idt_load();
}

// Load IDT (inline assembly)
void idt_load(void) {
    asm volatile ("lidt (%0)" : : "r"(&idtp));
}

// ISR handler
void isr_handler(registers_t regs) {
    // For now, just ignore CPU exceptions
    (void)regs;
}

// IRQ handler
void irq_handler(registers_t regs) {
    // Send EOI (End of Interrupt) to PICs
    if (regs.int_no >= 40) {
        outb(0xA0, 0x20);  // Send EOI to slave
    }
    outb(0x20, 0x20);      // Send EOI to master

    // Call keyboard handler for IRQ1
    if (regs.int_no == 33) {
        keyboard_handler();
    }
}
