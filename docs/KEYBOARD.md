# Keyboard Input Implementation

This document describes the keyboard input implementation in the kernel.

## Overview

The kernel now supports keyboard input through hardware interrupts. When you type on the keyboard, the characters appear on screen in real-time.

## Components

### 1. Interrupt Descriptor Table (IDT)

**Files:**
- `src/kernel/idt.asm` - Assembly interrupt handlers
- `src/kernel/idt.c` - IDT initialization and management
- `include/kernel/idt.h` - IDT interface

**Features:**
- 256-entry IDT covering all CPU exceptions and hardware interrupts
- ISR (Interrupt Service Routine) handlers for CPU exceptions (0-31)
- IRQ (Interrupt Request) handlers for hardware (32-47)
- PIC (Programmable Interrupt Controller) remapping
- Keyboard interrupt (IRQ1) enabled

### 2. Keyboard Driver

**Files:**
- `src/drivers/keyboard/keyboard.c` - Keyboard interrupt handler
- `include/drivers/keyboard.h` - Keyboard interface

**Features:**
- US QWERTY scancode to ASCII mapping
- Support for shifted characters (uppercase, symbols)
- Caps Lock support
- Special key handling (Backspace, Enter, Tab)
- Shift key state tracking

**Supported Keys:**
- All alphanumeric keys (a-z, A-Z, 0-9)
- Special characters (symbols, punctuation)
- Backspace (removes previous character)
- Enter (newline)
- Tab (4-space alignment)
- Space
- Shift (for uppercase and symbols)
- Caps Lock

### 3. Screen Driver

**Files:**
- `src/drivers/screen/screen.c` - VGA text mode driver
- `include/drivers/screen.h` - Screen interface

**Features:**
- Hardware cursor positioning
- Automatic scrolling when screen is full
- Character output with cursor advancement
- Newline and backspace support
- Tab alignment
- Screen clearing

**Functions:**
- `screen_clear()` - Clear entire screen
- `screen_putchar(char c)` - Print single character with cursor
- `screen_print(const char* str)` - Print string
- `screen_print_at(str, row, col)` - Print at specific position
- `screen_set_cursor(x, y)` - Set cursor position
- `screen_get_cursor(&x, &y)` - Get cursor position

### 4. I/O Port Operations

**Files:**
- `include/lib/io.h` - Port I/O functions

**Functions:**
- `outb(port, value)` - Write byte to I/O port
- `inb(port)` - Read byte from I/O port
- `outw(port, value)` - Write word to I/O port
- `inw(port)` - Read word from I/O port

## How It Works

### Interrupt Flow

1. **User presses key** → Keyboard controller generates IRQ1
2. **CPU receives interrupt** → Looks up handler in IDT entry 33 (IRQ1)
3. **IDT calls `irq1`** → Assembly stub in `idt.asm`
4. **Assembly stub saves state** → Pushes all registers to stack
5. **Calls `irq_handler()`** → C function in `idt.c`
6. **`irq_handler()` calls `keyboard_handler()`** → Driver function
7. **`keyboard_handler()` reads scancode** → From port 0x60
8. **Converts scancode to ASCII** → Using lookup table
9. **Calls `screen_putchar()`** → Display character
10. **Sends EOI to PIC** → End of Interrupt signal
11. **Restores state and returns** → Resume kernel execution

### Memory Map

| Address | Description |
|---------|-------------|
| 0x1000 | Kernel code start |
| 0xB8000 | VGA text mode buffer (80x25 characters) |

### I/O Ports Used

| Port | Description |
|------|-------------|
| 0x20, 0x21 | Master PIC (command and data) |
| 0xA0, 0xA1 | Slave PIC (command and data) |
| 0x60 | Keyboard data port |
| 0x64 | Keyboard status port |
| 0x3D4, 0x3D5 | VGA cursor position |

## Building and Running

```bash
# Build the kernel
make clean && make

# Run in QEMU (graphical mode)
./run.sh

# Or run headless (WSL2)
make run-headless
```

## Testing

When the kernel boots, you'll see:
```
Minimal x86 Kernel
==================

Keyboard driver initialized!
Type something: _
```

The cursor (blinking underscore) indicates the kernel is ready for input. Type on your keyboard and the characters will appear on screen.

## Keyboard Scancode Reference

### Normal Keys
- `0x02-0x0B`: 1-9, 0
- `0x10-0x19`: Q-P
- `0x1E-0x26`: A-L
- `0x2C-0x32`: Z-M
- `0x39`: Space
- `0x1C`: Enter
- `0x0E`: Backspace

### Modifier Keys
- `0x2A`: Left Shift
- `0x36`: Right Shift
- `0x3A`: Caps Lock
- `0x1D`: Left Control
- `0x38`: Left Alt

## Limitations

Current implementation limitations:
- No support for extended scancodes (E0/E1 prefixes)
- No support for Control/Alt key combinations
- No function keys (F1-F12)
- No arrow keys or navigation keys
- No numeric keypad
- Only US QWERTY layout supported
- No key repeat handling
- No keyboard LED control

## Future Enhancements

Potential improvements:
- [ ] Support for arrow keys and navigation
- [ ] Function key support
- [ ] Control/Alt key combinations
- [ ] Multiple keyboard layouts
- [ ] Keyboard LED control (Caps/Num/Scroll Lock)
- [ ] Key repeat delay/rate configuration
- [ ] Input buffering for line editing
- [ ] Command line interface / shell
- [ ] Ctrl+C interrupt handling

## Technical Details

### PIC Remapping

The 8259 PIC is remapped to avoid conflicts with CPU exceptions:
- Master PIC: IRQ 0-7 → interrupts 32-39
- Slave PIC: IRQ 8-15 → interrupts 40-47

### Scancode Sets

The keyboard uses PS/2 scancode set 1 (default):
- Make codes: Sent when key is pressed
- Break codes: Make code | 0x80 (sent when key is released)

### VGA Text Mode

- Resolution: 80 columns × 25 rows
- Character format: 2 bytes per character
  - Byte 0: ASCII character
  - Byte 1: Attribute (color)
- Default color: White on black (0x0F)

## Troubleshooting

**No keyboard response:**
- Ensure interrupts are enabled (`sti` instruction)
- Check PIC is properly remapped
- Verify IDT is loaded correctly
- Check QEMU is in correct mode (not `-nographic` for testing)

**Wrong characters:**
- Verify scancode table matches your keyboard layout
- Check shift state is being tracked correctly
- Ensure Caps Lock logic is correct

**Cursor not visible:**
- Check VGA cursor commands are sent correctly
- Verify cursor position is within bounds (0-79, 0-24)
