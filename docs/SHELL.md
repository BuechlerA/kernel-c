# Shell Implementation

This document describes the basic shell implementation in the kernel.

## Overview

The kernel now includes a simple command-line shell that allows users to interact with the system through text commands. The shell supports command parsing, argument handling, and multiple built-in commands.

## Features

- **Command prompt** with `$` indicator
- **Command parsing** with argument support
- **Line editing** with backspace support
- **Built-in commands** (help, clear, echo, about)
- **Command execution** with error handling
- **Extensible architecture** for adding new commands

## Components

### 1. String Library

**Files:**
- `src/lib/string/string.c` - String manipulation functions
- `include/lib/string.h` - String library interface

**Functions:**
- `strlen()` - Calculate string length
- `strcmp()` - Compare two strings
- `strncmp()` - Compare first n characters
- `strcpy()` - Copy string
- `strncpy()` - Copy n characters
- `strcat()` - Concatenate strings
- `strchr()` - Find character in string
- `memset()` - Set memory to value
- `memcpy()` - Copy memory
- `memcmp()` - Compare memory
- `itoa()` - Convert integer to string
- `atoi()` - Convert string to integer

### 2. Shell Core

**Files:**
- `src/kernel/shell.c` - Shell implementation
- `include/kernel/shell.h` - Shell interface

**Key Functions:**
- `shell_init()` - Initialize shell and display welcome message
- `shell_prompt()` - Display command prompt
- `shell_handle_input(char c)` - Process keyboard input
- `shell_parse_command()` - Parse command line into command and arguments
- `shell_execute_command()` - Execute parsed command

## Architecture

### Input Flow

```
Keyboard → keyboard_handler() → shell_handle_input() → shell_execute_command()
```

1. User types on keyboard
2. `keyboard_handler()` converts scancode to ASCII
3. `shell_handle_input()` buffers characters and handles special keys
4. On Enter, `shell_execute_command()` processes the command
5. Command is parsed and appropriate handler is called
6. Prompt is displayed for next command

### Command Structure

```c
typedef struct {
    const char* name;
    void (*handler)(int argc, char* args[]);
    const char* description;
} shell_command_t;
```

Each command has:
- **name**: Command string (e.g., "help")
- **handler**: Function pointer to command implementation
- **description**: Help text shown in `help` command

### Command Buffer

- Maximum command length: 256 characters
- Maximum arguments: 16
- Simple history: Stores previous command

## Built-in Commands

### `help`
Display list of available commands with descriptions.

**Usage:**
```
$ help
```

**Output:**
```
Available commands:
  help - Display this help message
  clear - Clear the screen
  echo - Print arguments to screen
  about - Display kernel information
```

### `clear`
Clear the screen and position cursor at top-left.

**Usage:**
```
$ clear
```

### `echo`
Print arguments to the screen.

**Usage:**
```
$ echo Hello World
Hello World

$ echo This is a test
This is a test
```

### `about`
Display kernel information including version, architecture, and features.

**Usage:**
```
$ about
```

**Output:**
```
MiniOS - Minimal x86 Kernel
============================
Version: 0.1
Architecture: x86 (32-bit protected mode)
Features:
  - Custom bootloader
  - Interrupt handling (IDT)
  - Keyboard driver
  - VGA text mode driver
  - Basic shell

Written in C and x86 assembly
```

## Adding New Commands

To add a new command to the shell:

1. **Define the command handler function:**
```c
void cmd_mycommand(int argc, char* args[]) {
    screen_print("My command executed!\n");

    // Process arguments
    for (int i = 0; i < argc; i++) {
        screen_print(args[i]);
        screen_print("\n");
    }
}
```

2. **Add to command table in `shell.c`:**
```c
static shell_command_t commands[] = {
    {"help", cmd_help, "Display this help message"},
    {"clear", cmd_clear, "Clear the screen"},
    {"echo", cmd_echo, "Print arguments to screen"},
    {"about", cmd_about, "Display kernel information"},
    {"mycommand", cmd_mycommand, "My new command"},  // Add here
    {0, 0, 0}  // Sentinel
};
```

3. **Rebuild and test:**
```bash
make clean && make
./run.sh
```

## Command Parsing

The shell uses space-separated argument parsing:

**Example:** `echo hello world`
- Command: `echo`
- argc: 2
- args[0]: `hello`
- args[1]: `world`

**Example:** `help`
- Command: `help`
- argc: 0
- args: (none)

## Special Key Handling

- **Enter**: Execute command
- **Backspace**: Delete previous character
- **Printable characters**: Add to command buffer
- **Tab, Shift, Caps Lock**: Handled by keyboard driver

## Limitations

Current limitations:
- No command history navigation (up/down arrows)
- No tab completion
- No pipe or redirection operators
- No quotes for arguments with spaces
- No escape sequences
- No environment variables
- Maximum 256 characters per command
- Maximum 16 arguments

## Future Enhancements

Potential improvements:
- [ ] Command history with arrow key navigation
- [ ] Tab completion for commands
- [ ] File system commands (ls, cat, cd, etc.)
- [ ] Process management commands
- [ ] System information commands (ps, free, uptime)
- [ ] Configuration commands
- [ ] Script support
- [ ] Pipe and redirection operators
- [ ] Environment variables
- [ ] Aliases

## Technical Details

### Memory Layout

- Command buffer: 256 bytes
- Previous command buffer: 256 bytes
- Parse buffer: 256 bytes
- Total memory: ~768 bytes

### Screen Integration

The shell uses the screen driver for all output:
- `screen_print()` - For command output
- `screen_putchar()` - For character echo
- `screen_clear()` - For clear command

### Error Handling

- Unknown commands display error message
- No crash on empty commands
- Graceful handling of buffer overflow

## Example Session

```
MiniOS Shell v0.1
=================
Type 'help' for available commands.

$ help
Available commands:
  help - Display this help message
  clear - Clear the screen
  echo - Print arguments to screen
  about - Display kernel information

$ echo Hello from MiniOS!
Hello from MiniOS!

$ about

MiniOS - Minimal x86 Kernel
============================
Version: 0.1
Architecture: x86 (32-bit protected mode)
Features:
  - Custom bootloader
  - Interrupt handling (IDT)
  - Keyboard driver
  - VGA text mode driver
  - Basic shell

Written in C and x86 assembly

$ clear
(screen clears)

MiniOS Shell v0.1
=================
Type 'help' for available commands.

$
```

## Code Organization

```
src/kernel/shell.c       - Shell implementation
include/kernel/shell.h   - Shell interface
src/lib/string/string.c  - String library
include/lib/string.h     - String library interface
```

## Testing

To test the shell:

1. Build and run: `./run.sh`
2. Type commands at the `$` prompt
3. Test each built-in command
4. Test command with arguments: `echo test 123`
5. Test unknown command: `foo` (should show error)
6. Test backspace editing
7. Test screen scrolling with multiple commands

## Troubleshooting

**No prompt appears:**
- Check `shell_init()` is called in `kernel_main()`
- Verify interrupts are enabled (`sti`)
- Check screen driver is working

**Commands not executing:**
- Verify keyboard driver is calling `shell_handle_input()`
- Check command table in `shell.c`
- Ensure string comparison is working

**Characters not appearing:**
- Check keyboard to shell integration
- Verify `screen_putchar()` is working
- Check command buffer isn't full
