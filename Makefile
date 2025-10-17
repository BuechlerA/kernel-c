# Kernel Build System
# ===================

# Directories
SRC_DIR = src
BUILD_DIR = build
BOOT_DIR = $(SRC_DIR)/boot
KERNEL_DIR = $(SRC_DIR)/kernel
DRIVERS_DIR = $(SRC_DIR)/drivers
LIB_DIR = $(SRC_DIR)/lib
INCLUDE_DIR = include

# Tools
CC = gcc
LD = ld
ASM = nasm

# Flags
CFLAGS = -m32 -ffreestanding -nostdlib -fno-pie -fno-stack-protector -c -I$(INCLUDE_DIR)
LDFLAGS = -m elf_i386 -T $(KERNEL_DIR)/linker.ld
ASMFLAGS = -f elf32

# Output files
BOOT_BIN = $(BUILD_DIR)/boot.bin
KERNEL_BIN = $(BUILD_DIR)/kernel.bin
OS_IMAGE = $(BUILD_DIR)/os-image.bin

# Object files
KERNEL_ENTRY_OBJ = $(BUILD_DIR)/kernel_entry.o
KERNEL_OBJ = $(BUILD_DIR)/kernel.o
IDT_ASM_OBJ = $(BUILD_DIR)/idt_asm.o
IDT_OBJ = $(BUILD_DIR)/idt.o
SCREEN_OBJ = $(BUILD_DIR)/screen.o
KEYBOARD_OBJ = $(BUILD_DIR)/keyboard.o
SHELL_OBJ = $(BUILD_DIR)/shell.o
STRING_OBJ = $(BUILD_DIR)/string.o

# All object files
OBJS = $(KERNEL_ENTRY_OBJ) $(KERNEL_OBJ) $(IDT_ASM_OBJ) $(IDT_OBJ) $(SCREEN_OBJ) $(KEYBOARD_OBJ) $(SHELL_OBJ) $(STRING_OBJ)

# Phony targets
.PHONY: all clean run debug dirs info help

# Default target
all: dirs $(OS_IMAGE)

# Create build directory
dirs:
	@mkdir -p $(BUILD_DIR)

# Compile boot sector
$(BOOT_BIN): $(BOOT_DIR)/boot.asm $(BOOT_DIR)/*.asm
	@echo "Assembling boot sector..."
	$(ASM) -f bin -i $(BOOT_DIR)/ $(BOOT_DIR)/boot.asm -o $(BOOT_BIN)
	@echo "Boot sector: $(BOOT_BIN) created"

# Compile kernel entry
$(KERNEL_ENTRY_OBJ): $(KERNEL_DIR)/kernel_entry.asm
	@echo "Assembling kernel entry..."
	$(ASM) $(ASMFLAGS) $(KERNEL_DIR)/kernel_entry.asm -o $(KERNEL_ENTRY_OBJ)

# Compile kernel C code
$(KERNEL_OBJ): $(KERNEL_DIR)/kernel.c
	@echo "Compiling kernel..."
	$(CC) $(CFLAGS) $(KERNEL_DIR)/kernel.c -o $(KERNEL_OBJ)

# Assemble IDT assembly
$(IDT_ASM_OBJ): $(KERNEL_DIR)/idt.asm
	@echo "Assembling IDT..."
	$(ASM) $(ASMFLAGS) $(KERNEL_DIR)/idt.asm -o $(IDT_ASM_OBJ)

# Compile IDT C code
$(IDT_OBJ): $(KERNEL_DIR)/idt.c
	@echo "Compiling IDT..."
	$(CC) $(CFLAGS) $(KERNEL_DIR)/idt.c -o $(IDT_OBJ)

# Compile screen driver
$(SCREEN_OBJ): $(DRIVERS_DIR)/screen/screen.c
	@echo "Compiling screen driver..."
	$(CC) $(CFLAGS) $(DRIVERS_DIR)/screen/screen.c -o $(SCREEN_OBJ)

# Compile keyboard driver
$(KEYBOARD_OBJ): $(DRIVERS_DIR)/keyboard/keyboard.c
	@echo "Compiling keyboard driver..."
	$(CC) $(CFLAGS) $(DRIVERS_DIR)/keyboard/keyboard.c -o $(KEYBOARD_OBJ)

# Compile shell
$(SHELL_OBJ): $(KERNEL_DIR)/shell.c
	@echo "Compiling shell..."
	$(CC) $(CFLAGS) $(KERNEL_DIR)/shell.c -o $(SHELL_OBJ)

# Compile string library
$(STRING_OBJ): $(LIB_DIR)/string/string.c
	@echo "Compiling string library..."
	$(CC) $(CFLAGS) $(LIB_DIR)/string/string.c -o $(STRING_OBJ)

# Link kernel
$(KERNEL_BIN): $(OBJS)
	@echo "Linking kernel..."
	$(LD) $(LDFLAGS) -o $(KERNEL_BIN) $(OBJS) --oformat binary
	@echo "Kernel: $(KERNEL_BIN) created"

# Create OS image (boot sector + kernel)
$(OS_IMAGE): $(BOOT_BIN) $(KERNEL_BIN)
	@echo "Creating OS image..."
	cat $(BOOT_BIN) $(KERNEL_BIN) > $(OS_IMAGE)
	@echo ""
	@echo "Build complete!"
	@echo "==============="
	@echo "Boot sector size: $$(stat -c%s $(BOOT_BIN)) bytes"
	@echo "Kernel size: $$(stat -c%s $(KERNEL_BIN)) bytes"
	@echo "OS image size: $$(stat -c%s $(OS_IMAGE)) bytes"
	@echo "Output: $(OS_IMAGE)"

# Run in QEMU
run: $(OS_IMAGE)
	@echo "Starting QEMU..."
	qemu-system-i386 -drive format=raw,file=$(OS_IMAGE)

# Run in QEMU with debugging
debug: $(OS_IMAGE)
	@echo "Starting QEMU in debug mode..."
	@echo "Connect GDB with: gdb -ex 'target remote localhost:1234'"
	qemu-system-i386 -drive format=raw,file=$(OS_IMAGE) -s -S

# Run in QEMU headless (for WSL2)
run-headless: $(OS_IMAGE)
	@echo "Starting QEMU in headless mode..."
	qemu-system-i386 -drive format=raw,file=$(OS_IMAGE) -nographic

# Clean build files
clean:
	@echo "Cleaning build files..."
	rm -rf $(BUILD_DIR)
	@echo "Clean complete!"

# Show build information
info:
	@echo "Kernel Build System Information"
	@echo "================================"
	@echo "Source directory: $(SRC_DIR)"
	@echo "Build directory: $(BUILD_DIR)"
	@echo "Boot directory: $(BOOT_DIR)"
	@echo "Kernel directory: $(KERNEL_DIR)"
	@echo "Drivers directory: $(DRIVERS_DIR)"
	@echo "Library directory: $(LIB_DIR)"
	@echo "Include directory: $(INCLUDE_DIR)"
	@echo ""
	@echo "Tools:"
	@echo "  Assembler: $(ASM)"
	@echo "  C Compiler: $(CC)"
	@echo "  Linker: $(LD)"
	@echo ""
	@echo "Flags:"
	@echo "  CFLAGS: $(CFLAGS)"
	@echo "  LDFLAGS: $(LDFLAGS)"
	@echo "  ASMFLAGS: $(ASMFLAGS)"

# Show help
help:
	@echo "Kernel Build System - Available Targets"
	@echo "========================================"
	@echo ""
	@echo "  make              - Build the OS image"
	@echo "  make all          - Same as 'make'"
	@echo "  make clean        - Remove all build files"
	@echo "  make run          - Build and run in QEMU"
	@echo "  make run-headless - Build and run in QEMU (no graphics)"
	@echo "  make debug        - Build and run in QEMU with GDB stub"
	@echo "  make info         - Show build configuration"
	@echo "  make help         - Show this help message"
	@echo ""
	@echo "QEMU Controls:"
	@echo "  Ctrl+A then X     - Exit QEMU"
	@echo "  Ctrl+A then C     - Switch to QEMU console"
