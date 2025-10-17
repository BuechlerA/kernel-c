#ifndef IDT_H
#define IDT_H

// Registers structure passed to ISR/IRQ handlers
typedef struct {
    unsigned int ds;                          // Data segment selector
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  // Pushed by pusha
    unsigned int int_no, err_code;           // Interrupt number and error code
    unsigned int eip, cs, eflags, useresp, ss;  // Pushed by processor automatically
} registers_t;

// Function prototypes
void idt_init(void);
void idt_load(void);
void irq_remap(void);
void isr_handler(registers_t regs);
void irq_handler(registers_t regs);

// External keyboard handler
extern void keyboard_handler(void);

#endif
