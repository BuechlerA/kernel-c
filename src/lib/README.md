# Libraries Directory

This directory contains utility libraries used by the kernel.

## Planned Libraries

### String Operations (`string/`)
Standard string manipulation functions:
- `strlen()` - Calculate string length
- `strcmp()` - Compare strings
- `strcpy()` - Copy strings
- `strcat()` - Concatenate strings
- `strchr()` - Find character in string
- `memset()` - Fill memory with constant byte
- `memcpy()` - Copy memory area
- `memcmp()` - Compare memory areas

### Memory Management (`memory/`)
Dynamic memory allocation and management:
- `malloc()` - Allocate memory
- `free()` - Free allocated memory
- `realloc()` - Resize allocated memory
- `calloc()` - Allocate and zero memory
- Heap management
- Memory pool allocator
- Virtual memory management

### I/O Operations (`io/`)
Low-level I/O utilities:
- `outb()`, `outw()`, `outl()` - Output to I/O ports
- `inb()`, `inw()`, `inl()` - Input from I/O ports
- `io_wait()` - I/O delay
- Port mapping utilities

### Data Structures (`ds/`)
Common data structures:
- Linked lists
- Queues
- Stacks
- Hash tables
- Trees

### Math Operations (`math/`)
Basic mathematical functions:
- Integer arithmetic
- Division and modulo
- Power and square root
- Random number generation

## Directory Structure

```
lib/
├── string/
│   ├── string.c
│   └── string.h
├── memory/
│   ├── memory.c
│   └── memory.h
├── io/
│   ├── io.c
│   └── io.h
├── ds/
│   ├── list.c
│   ├── list.h
│   ├── queue.c
│   └── queue.h
└── math/
    ├── math.c
    └── math.h
```

## Implementation Guidelines

1. Keep functions small and focused
2. Avoid standard library dependencies
3. Optimize for kernel use (no floating point unless necessary)
4. Document all public APIs
5. Include comprehensive error checking
6. Write unit tests where possible
7. Consider memory constraints
