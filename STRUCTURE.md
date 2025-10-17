# Project Structure Overview

This document provides a comprehensive overview of the kernel project's directory structure and organization.

## Directory Tree

```
kernel-c/
├── src/                          # All source code
│   ├── boot/                     # Bootloader code
│   │   ├── boot.asm             # Main boot sector (512 bytes)
│   │   ├── print.asm            # 16-bit and 32-bit printing
│   │   ├── disk.asm             # BIOS disk loading
│   │   ├── gdt.asm              # Global Descriptor Table
│   │   └── switch_pm.asm        # Protected mode switching
│   │
│   ├── kernel/                   # Kernel core
│   │   ├── kernel_entry.asm     # Assembly entry point
│   │   ├── kernel.c             # Main kernel C code
│   │   ├── idt.asm              # IDT assembly handlers
│   │   ├── idt.c                # IDT initialization
│   │   ├── shell.c              # Shell implementation
│   │   └── linker.ld            # Linker script
│   │
│   ├── drivers/                  # Device drivers
│   │   ├── README.md            # Driver documentation
│   │   ├── keyboard/            # Keyboard driver (implemented)
│   │   │   └── keyboard.c       # PS/2 keyboard with scancode mapping
│   │   ├── screen/              # VGA screen driver (implemented)
│   │   │   └── screen.c         # VGA text mode with cursor
│   │   ├── disk/                # Disk driver (future: ATA/IDE)
│   │   ├── timer/               # PIT timer driver (future)
│   │   └── serial/              # Serial port driver (future)
│   │
│   └── lib/                      # Utility libraries
│       ├── README.md            # Library documentation
│       ├── string/              # String operations (implemented)
│       │   └── string.c         # strlen, strcmp, strcpy, memset, etc.
│       ├── memory/              # Memory management (future: malloc, free)
│       ├── io/                  # I/O port operations (in headers)
│       ├── ds/                  # Data structures (future)
│       └── math/                # Math functions (future)
│
├── include/                      # Header files
│   ├── README.md                # Header documentation
│   ├── kernel/                  # Kernel headers
│   │   ├── idt.h               # IDT definitions (implemented)
│   │   └── shell.h             # Shell interface (implemented)
│   ├── drivers/                 # Driver headers
│   │   ├── keyboard.h          # Keyboard interface (implemented)
│   │   └── screen.h            # Screen interface (implemented)
│   └── lib/                     # Library headers
│       ├── io.h                # Port I/O functions (implemented)
│       └── string.h            # String library (implemented)
│
├── build/                        # Build output (generated, not in git)
│   ├── boot.bin                 # Compiled boot sector
│   ├── kernel.bin               # Compiled kernel
│   ├── kernel_entry.o           # Kernel entry object
│   ├── kernel.o                 # Kernel object file
│   └── os-image.bin             # Final bootable image
│
├── scripts/                      # Build and utility scripts
│   ├── run.sh                   # Build and run in QEMU
│   └── test.sh                  # Quick test script
│
├── docs/                         # Documentation
│   ├── README.md                # Main documentation
│   ├── KEYBOARD.md              # Keyboard driver documentation
│   └── SHELL.md                 # Shell documentation
│
├── Makefile                      # Build configuration
├── STRUCTURE.md                  # This file (project structure)
├── README.md                     # Main readme
├── QUICKSTART.md                 # Quick start guide
├── run.sh                        # Convenience build & run script
├── test.sh                       # Test script
└── .gitignore                    # Git ignore rules
```

## Key Files

### Boot Sector (`src/boot/`)
- **boot.asm**: Main bootloader that loads the kernel and switches to protected mode
- **print.asm**: Printing routines for both real and protected modes
- **disk.asm**: BIOS disk reading with retry logic
- **gdt.asm**: Global Descriptor Table for memory segmentation
- **switch_pm.asm**: Code to switch from real mode to protected mode

### Kernel (`src/kernel/`)
- **kernel_entry.asm**: Assembly stub that calls the C kernel
- **kernel.c**: Main kernel initialization and entry point
- **idt.asm**: IDT assembly interrupt handlers (ISRs and IRQs)
- **idt.c**: IDT initialization and PIC remapping
- **shell.c**: Shell implementation with command parsing
- **linker.ld**: Linker script defining memory layout (kernel at 0x1000)

### Drivers (`src/drivers/`)
- **keyboard/keyboard.c**: PS/2 keyboard driver with scancode translation
- **screen/screen.c**: VGA text mode driver with cursor support

### Libraries (`src/lib/`)
- **string/string.c**: String manipulation functions (13 functions)

### Build System
- **Makefile**: Comprehensive build system with multiple targets
- **scripts/run.sh**: Convenient script to build and run with QEMU
- **scripts/test.sh**: Quick test with timeout

## Build Targets

The Makefile provides several useful targets:

```bash
make              # Build the OS image
make clean        # Remove all build files
make run          # Build and run in QEMU (graphical)
make run-headless # Build and run in QEMU (no graphics, for WSL2)
make debug        # Build and run with GDB debugging enabled
make info         # Show build configuration
make help         # Show all available targets
```

## Memory Map

| Address | Description |
|---------|-------------|
| 0x0000 - 0x03FF | Real Mode IVT (Interrupt Vector Table) |
| 0x0400 - 0x04FF | BIOS Data Area |
| 0x0500 - 0x7BFF | Free memory (can be used by OS) |
| 0x7C00 - 0x7DFF | Boot sector loaded here by BIOS |
| 0x7E00 - 0x7FFFF | Free memory (stack area) |
| 0x1000 - ... | Kernel loaded here |
| 0xB8000 - 0xBFFFF | VGA text mode video memory |

## Expansion Guidelines

### Adding a New Driver

1. Create directory: `src/drivers/newdriver/`
2. Add source file: `src/drivers/newdriver/newdriver.c`
3. Add header: `include/drivers/newdriver.h`
4. Update Makefile to compile the driver
5. Document in `src/drivers/README.md`

### Adding a Library Function

1. Choose appropriate subdirectory in `src/lib/`
2. Add implementation in `.c` file
3. Add header in `include/lib/`
4. Update Makefile if needed
5. Document in `src/lib/README.md`

### Adding Kernel Features

1. Add source in `src/kernel/`
2. Add headers in `include/kernel/`
3. Update linker script if memory layout changes
4. Update Makefile compilation rules
5. Document in main README

## Coding Standards

### Assembly Files
- Use meaningful labels and comments
- Follow NASM syntax
- Use include guards for shared files
- Document register usage in comments

### C Files
- Use `-ffreestanding` (no standard library)
- Fixed-width types (uint8_t, uint16_t, etc.)
- Avoid floating point operations
- Comment all non-obvious code
- Follow kernel coding style (e.g., Linux style)

### Naming Conventions
- Functions: `lowercase_with_underscores`
- Constants: `UPPERCASE_WITH_UNDERSCORES`
- Types: `typedef struct { ... } name_t;`
- Macros: `UPPERCASE_WITH_UNDERSCORES`

## Git Workflow

The `.gitignore` file excludes:
- `build/` directory (generated files)
- Binary files (`*.bin`, `*.o`, `*.img`, `*.iso`)
- Backup files (`*.old`, `*~`)
- Editor files (`.vscode/`, `.idea/`)

Only commit source files and documentation.

## Resources for Extension

- **OSDev Wiki**: https://wiki.osdev.org/
- **Intel Manuals**: https://software.intel.com/content/www/us/en/develop/articles/intel-sdm.html
- **NASM Docs**: https://www.nasm.us/docs.php
- **GCC Options**: https://gcc.gnu.org/onlinedocs/gcc/

## Future Roadmap

Phase 1 (Completed):
- ✓ Basic bootloader
- ✓ Protected mode
- ✓ VGA text output

Phase 2 (Completed):
- ✓ Interrupt handling (IDT)
- ✓ Keyboard input
- ✓ Basic shell
- ✓ String library
- ✓ Command parser

Phase 3 (Next):
- [ ] Memory management (heap, malloc/free)
- [ ] Process scheduler
- [ ] System calls
- [ ] Timer driver

Phase 4:
- [ ] File system (FAT12/16 or custom)
- [ ] User programs
- [ ] Advanced shell features (history, tab completion)
- [ ] More drivers (serial, disk)
