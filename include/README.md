# Include Directory

This directory contains header files for the kernel.

## Structure

```
include/
├── kernel/          # Kernel core headers
│   ├── types.h     # Type definitions
│   ├── kernel.h    # Kernel main functions
│   ├── multiboot.h # Multiboot specification
│   └── idt.h       # Interrupt Descriptor Table
├── drivers/         # Driver headers
│   ├── keyboard.h
│   ├── screen.h
│   ├── disk.h
│   ├── timer.h
│   └── serial.h
└── lib/            # Library headers
    ├── string.h
    ├── memory.h
    ├── io.h
    └── math.h
```

## Header File Guidelines

1. **Include Guards**: Always use include guards or `#pragma once`
   ```c
   #ifndef KERNEL_H
   #define KERNEL_H
   // content
   #endif
   ```

2. **Type Definitions**: Define common types in `types.h`
   ```c
   typedef unsigned char uint8_t;
   typedef unsigned short uint16_t;
   typedef unsigned int uint32_t;
   ```

3. **Function Prototypes**: Declare all public functions
   ```c
   void kernel_main(void);
   void panic(const char* message);
   ```

4. **Constants**: Define hardware constants and magic numbers
   ```c
   #define VGA_WIDTH 80
   #define VGA_HEIGHT 25
   #define VIDEO_MEMORY 0xb8000
   ```

5. **Documentation**: Use comments to document APIs
   ```c
   /**
    * Prints a string to the screen
    * @param str The null-terminated string to print
    */
   void print(const char* str);
   ```

## Common Headers to Implement

### `kernel/types.h`
- Fixed-width integer types
- Boolean type
- Size types
- NULL definition

### `kernel/kernel.h`
- Kernel initialization
- Panic and error handling
- System information

### `lib/io.h`
- Port I/O functions
- Memory-mapped I/O helpers

### `lib/string.h`
- String manipulation functions
- Memory operations
