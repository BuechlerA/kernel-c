# Minimal x86 Kernel in C

A minimal operating system kernel written in C for the x86 architecture, designed to run in QEMU.

## Features

- Bootloader written in x86 assembly
- 32-bit protected mode
- VGA text mode output with hardware cursor
- **Keyboard input support** - Type characters in real-time!
- **Interactive shell** - Command-line interface with built-in commands
- Interrupt handling (IDT with CPU exceptions and hardware IRQs)
- Screen driver with scrolling and cursor positioning
- Keyboard driver with scancode translation
- String library (strcmp, strlen, strcpy, memset, etc.)
- Command parser with argument support
- Modular and extensible directory structure

## Prerequisites

### Linux (Debian/Ubuntu/WSL2)

```bash
sudo apt update
sudo apt install build-essential nasm qemu-system-x86
```

### Linux (Fedora/RHEL/CentOS)

```bash
sudo dnf groupinstall "Development Tools"
sudo dnf install nasm qemu-system-x86
```

### Linux (Arch/Manjaro)

```bash
sudo pacman -S base-devel nasm qemu-system-x86
```

### macOS

Install Homebrew if you haven't already:
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

Then install the required tools:
```bash
brew install nasm qemu
```

**Note for macOS**: You may need to install the i386-elf cross-compiler toolchain for proper linking:
```bash
brew tap nativeos/i386-elf-toolchain
brew install i386-elf-binutils i386-elf-gcc
```

Alternatively, use the system toolchain with appropriate flags (already configured in the Makefile).

## Project Structure

```
kernel-c/
├── src/                    # Source code
│   ├── boot/              # Bootloader and boot utilities
│   │   ├── boot.asm       # Main boot sector
│   │   ├── print.asm      # Real/protected mode printing
│   │   ├── disk.asm       # Disk loading routines
│   │   ├── gdt.asm        # Global Descriptor Table
│   │   └── switch_pm.asm  # Protected mode switching
│   ├── kernel/            # Kernel core
│   │   ├── kernel_entry.asm  # Kernel entry point
│   │   ├── kernel.c       # Main kernel code
│   │   └── linker.ld      # Linker script
│   ├── drivers/           # Device drivers (future)
│   │   ├── keyboard/      # Keyboard driver
│   │   ├── screen/        # Screen/VGA driver
│   │   └── disk/          # Disk driver
│   └── lib/               # Utility libraries (future)
│       ├── string/        # String operations
│       ├── memory/        # Memory management
│       └── io/            # I/O operations
├── include/               # Header files (future)
│   ├── kernel/            # Kernel headers
│   ├── drivers/           # Driver headers
│   └── lib/               # Library headers
├── build/                 # Build output directory
│   ├── boot.bin          # Compiled boot sector
│   ├── kernel.bin        # Compiled kernel
│   └── os-image.bin      # Final OS image
├── scripts/               # Build and utility scripts
│   ├── run.sh            # Build and run script
│   └── test.sh           # Test script
├── docs/                  # Documentation
│   └── README.md         # This file
├── Makefile              # Build configuration
└── .gitignore           # Git ignore file
```

## Building

### Option 1: Use the run script
```bash
chmod +x scripts/run.sh
./scripts/run.sh
```

### Option 2: Use make commands
```bash
# Build the OS image
make

# Build and run in QEMU
make run

# Clean build files
make clean
```

### Option 3: Manual build
```bash
# Assemble boot sector
nasm -f bin src/boot/boot.asm -o build/boot.bin

# Assemble kernel entry
nasm -f elf32 src/kernel/kernel_entry.asm -o build/kernel_entry.o

# Compile kernel
gcc -m32 -ffreestanding -nostdlib -fno-pie -fno-stack-protector -c src/kernel/kernel.c -o build/kernel.o

# Link kernel
ld -m elf_i386 -T src/kernel/linker.ld -o build/kernel.bin build/kernel_entry.o build/kernel.o --oformat binary

# Create OS image
cat build/boot.bin build/kernel.bin > build/os-image.bin

# Run in QEMU
qemu-system-i386 -drive format=raw,file=build/os-image.bin
```

## Running

After building, run the OS in QEMU:

```bash
qemu-system-i386 -drive format=raw,file=build/os-image.bin
```

Press `Ctrl+A` then `X` to exit QEMU.

## QEMU Options

- **Basic run**: `qemu-system-i386 -drive format=raw,file=build/os-image.bin`
- **Headless mode (WSL2)**: `qemu-system-i386 -drive format=raw,file=build/os-image.bin -nographic`
- **With debugging**: `qemu-system-i386 -drive format=raw,file=build/os-image.bin -s -S`
- **With serial output**: `qemu-system-i386 -drive format=raw,file=build/os-image.bin -serial stdio`
- **Curses mode (text only)**: `qemu-system-i386 -drive format=raw,file=build/os-image.bin -curses`

**Note for WSL2**: If you don't have an X server running, use the `-nographic` option or run `./scripts/run.sh` which will auto-detect this.

## How It Works

1. **Boot Sector** (`src/boot/boot.asm`):
   - BIOS loads the first 512 bytes (boot sector) into memory at 0x7c00
   - Initializes segment registers and sets up stack
   - Boot sector loads the kernel from disk into memory at 0x1000
   - Switches from 16-bit real mode to 32-bit protected mode
   - Jumps to the kernel entry point

2. **Kernel Entry** (`src/kernel/kernel_entry.asm`):
   - Calls the C function `kernel_main()`

3. **Kernel** (`src/kernel/kernel.c`):
   - Clears the screen
   - Prints welcome messages using VGA text mode
   - Enters an infinite loop with HLT instruction

## Memory Map

- `0x7c00`: Boot sector loaded here by BIOS
- `0x1000`: Kernel loaded here by bootloader
- `0xb8000`: VGA text mode video memory

## Troubleshooting

### Build errors:
- Ensure `nasm`, `gcc`, and `ld` are installed
- Check that you're using a 32-bit capable toolchain

### QEMU won't start:
- Install QEMU: `sudo apt install qemu-system-x86`
- On WSL2, make sure X11 forwarding is configured or use `-nographic` mode

### Black screen:
- Try using `-nographic` mode: `qemu-system-i386 -drive format=raw,file=build/os-image.bin -nographic`
- Check that the boot signature (0xaa55) is present in boot.bin

### Disk read error:
- Ensure the kernel binary is being created correctly
- Check that the OS image includes both boot sector and kernel
- The bootloader reads 3 sectors (1536 bytes) from disk

## Future Enhancements

The directory structure is designed to support future additions:

### Drivers (`src/drivers/`)
- **Keyboard driver**: Handle keyboard input and interrupts
- **Screen driver**: Advanced VGA/text mode operations
- **Disk driver**: ATA/IDE disk operations
- **Timer driver**: PIT (Programmable Interval Timer)
- **Serial driver**: COM port communication

### Libraries (`src/lib/`)
- **String operations**: strlen, strcmp, strcpy, etc.
- **Memory management**: malloc, free, memcpy, etc.
- **I/O operations**: Port I/O wrappers and utilities
- **Math functions**: Basic arithmetic operations

### Kernel Features (`src/kernel/`)
- **Interrupt Descriptor Table (IDT)**: Handle CPU interrupts
- **Memory management**: Paging and virtual memory
- **Process scheduler**: Multi-tasking support
- **System calls**: User-space to kernel communication
- **File system**: Basic FAT12/16 or custom FS

### Headers (`include/`)
- Type definitions and constants
- Function prototypes
- Structures and macros
- API documentation

## Resources

- [OSDev Wiki](https://wiki.osdev.org/)
- [Writing a Simple Operating System](https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf)
- [Intel x86 Architecture Manual](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html)
- [NASM Documentation](https://www.nasm.us/xdoc/2.15.05/html/nasmdoc0.html)
