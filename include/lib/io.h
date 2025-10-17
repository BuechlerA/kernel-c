#ifndef IO_H
#define IO_H

// Port I/O functions

// Write byte to port
static inline void outb(unsigned short port, unsigned char val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Read byte from port
static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Write word to port
static inline void outw(unsigned short port, unsigned short val) {
    asm volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

// Read word from port
static inline unsigned short inw(unsigned short port) {
    unsigned short ret;
    asm volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// I/O wait (small delay)
static inline void io_wait(void) {
    outb(0x80, 0);
}

#endif
