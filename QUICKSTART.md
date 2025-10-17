# Quick Start Guide

## Building and Running

```bash
# Build the kernel
make clean && make

# Run in QEMU
./run.sh

# Or use make
make run-headless
```

## Using the Shell

When the kernel boots, you'll see:

```
MiniOS Shell v0.1
=================
Type 'help' for available commands.

$
```

### Available Commands

| Command | Description | Example |
|---------|-------------|---------|
| `help` | Show all commands | `$ help` |
| `clear` | Clear screen | `$ clear` |
| `echo` | Print text | `$ echo Hello World` |
| `about` | Kernel info | `$ about` |

### Keyboard Controls

- **Type**: Regular typing works
- **Enter**: Execute command
- **Backspace**: Delete character
- **Shift**: Uppercase letters and symbols
- **Caps Lock**: Toggle caps lock

### QEMU Controls

- **Ctrl+A then X**: Exit QEMU
- **Ctrl+A then C**: QEMU console

## Project Structure

```
kernel-c/
├── src/
│   ├── boot/        # Bootloader
│   ├── kernel/      # Kernel core + shell
│   ├── drivers/     # Keyboard, screen
│   └── lib/         # String functions
├── include/         # Headers
├── build/           # Compiled files
├── scripts/         # Helper scripts
└── docs/            # Documentation
```

## Quick Development

### Add a New Shell Command

1. Edit `src/kernel/shell.c`
2. Add your command handler:

```c
void cmd_mycommand(int argc, char* args[]) {
    screen_print("Hello from my command!\n");
}
```

3. Add to command table:

```c
static shell_command_t commands[] = {
    // ... existing commands ...
    {"mycommand", cmd_mycommand, "My new command"},
    {0, 0, 0}  // Keep sentinel at end
};
```

4. Rebuild: `make clean && make`

### Modify Screen Output

Edit `src/drivers/screen/screen.c`:
- `screen_print()` - Print string
- `screen_putchar()` - Print character
- `screen_clear()` - Clear screen

### Modify Keyboard Handling

Edit `src/drivers/keyboard/keyboard.c`:
- Scancode tables for key mapping
- Special key handling

## Common Tasks

### Change Boot Message

Edit `src/kernel/shell.c` → `shell_init()`

### Change Prompt

Edit `src/kernel/shell.c` → `shell_prompt()`

### Add More Sectors

If kernel grows too large, edit `src/boot/boot.asm`:
```asm
mov dh, 30  ; Increase sector count
```

### Debug Build Issues

```bash
# Check file sizes
ls -lh build/

# View build info
make info

# Clean rebuild
make clean && make
```

## File Sizes

- Boot sector: 512 bytes (fixed)
- Kernel: ~8KB (current)
- Total: ~9KB

## Documentation

- `README.md` - Main documentation
- `docs/KEYBOARD.md` - Keyboard driver details
- `docs/SHELL.md` - Shell implementation
- `STRUCTURE.md` - Project organization

## Troubleshooting

### Black screen
```bash
# Use headless mode
make run-headless
```

### No keyboard input
- Check interrupts are enabled
- Verify IDT is loaded

### Build errors
```bash
# Install dependencies
sudo apt install build-essential nasm qemu-system-x86

# Clean rebuild
make clean && make
```

## Next Steps

1. Try all shell commands
2. Add your own command
3. Modify the shell prompt
4. Explore the source code
5. Read the documentation in `docs/`

## Getting Help

- Check `docs/` for detailed documentation
- Review source code comments
- Look at existing command implementations
- Read `STRUCTURE.md` for architecture

## Example Session

```
$ help
Available commands:
  help - Display this help message
  clear - Clear the screen
  echo - Print arguments to screen
  about - Display kernel information

$ echo Testing the shell!
Testing the shell!

$ about

MiniOS - Minimal x86 Kernel
============================
Version: 0.1
Architecture: x86 (32-bit protected mode)
...

$ clear
(screen clears)

$
```

Happy hacking! 🚀
