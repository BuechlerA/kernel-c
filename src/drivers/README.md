# Drivers Directory

This directory contains device drivers for the kernel.

## Planned Drivers

### Keyboard Driver
- Handle keyboard interrupts (IRQ1)
- Scancode translation
- Input buffering
- Support for different keyboard layouts

### Screen/VGA Driver
- Advanced VGA text mode operations
- Scrolling support
- Color management
- Cursor positioning and visibility

### Disk Driver
- ATA/IDE disk operations
- Read/write sectors
- Disk detection and initialization
- Support for primary and secondary channels

### Timer Driver
- PIT (Programmable Interval Timer) configuration
- System tick counter
- Sleep/delay functions
- Preemptive multitasking support

### Serial Driver
- COM port communication (RS-232)
- Debug output via serial
- Kernel logging
- Boot parameter input

## Directory Structure

```
drivers/
├── keyboard/
│   ├── keyboard.c
│   └── keyboard.h
├── screen/
│   ├── screen.c
│   └── screen.h
├── disk/
│   ├── ata.c
│   └── ata.h
├── timer/
│   ├── timer.c
│   └── timer.h
└── serial/
    ├── serial.c
    └── serial.h
```

## Implementation Guidelines

1. Each driver should be self-contained in its own directory
2. Provide initialization and cleanup functions
3. Use consistent naming conventions
4. Document hardware registers and protocols
5. Handle errors gracefully
6. Include driver-specific header files with API definitions
