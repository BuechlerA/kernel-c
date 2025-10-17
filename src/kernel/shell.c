#include "../../include/kernel/shell.h"
#include "../../include/drivers/screen.h"
#include "../../include/lib/string.h"

#define MAX_COMMAND_LENGTH 256
#define MAX_ARGS 16

// Command buffer
static char command_buffer[MAX_COMMAND_LENGTH];
static int command_index = 0;

// Command history (simple - just one previous command)
static char prev_command[MAX_COMMAND_LENGTH];

// Forward declarations
void shell_execute_command(const char* command);
void shell_parse_command(const char* command, char* cmd, char* args[], int* argc);

// Built-in commands
void cmd_help(int argc, char* args[]);
void cmd_clear(int argc, char* args[]);
void cmd_echo(int argc, char* args[]);
void cmd_about(int argc, char* args[]);

// Command structure
typedef struct {
    const char* name;
    void (*handler)(int argc, char* args[]);
    const char* description;
} shell_command_t;

// Command table
static shell_command_t commands[] = {
    {"help", cmd_help, "Display this help message"},
    {"clear", cmd_clear, "Clear the screen"},
    {"echo", cmd_echo, "Print arguments to screen"},
    {"about", cmd_about, "Display kernel information"},
    {0, 0, 0}  // Sentinel
};

// Initialize shell
void shell_init(void) {
    memset(command_buffer, 0, MAX_COMMAND_LENGTH);
    memset(prev_command, 0, MAX_COMMAND_LENGTH);
    command_index = 0;

    screen_clear();
    screen_print("MiniOS Shell v0.1\n");
    screen_print("=================\n");
    screen_print("Type 'help' for available commands.\n\n");
    shell_prompt();
}

// Display prompt
void shell_prompt(void) {
    screen_print("$ ");
}

// Handle keyboard input for shell
void shell_handle_input(char c) {
    if (c == '\n') {
        // Enter pressed - execute command
        screen_putchar('\n');
        command_buffer[command_index] = '\0';

        if (command_index > 0) {
            // Save to history
            strcpy(prev_command, command_buffer);

            // Execute command
            shell_execute_command(command_buffer);
        }

        // Reset buffer
        command_index = 0;
        memset(command_buffer, 0, MAX_COMMAND_LENGTH);

        // Show prompt
        shell_prompt();
    } else if (c == '\b') {
        // Backspace
        if (command_index > 0) {
            command_index--;
            command_buffer[command_index] = '\0';
            screen_putchar('\b');
        }
    } else if (c >= ' ' && c <= '~') {
        // Printable character
        if (command_index < MAX_COMMAND_LENGTH - 1) {
            command_buffer[command_index++] = c;
            screen_putchar(c);
        }
    }
}

// Parse command into command name and arguments
void shell_parse_command(const char* command, char* cmd, char* args[], int* argc) {
    static char parse_buffer[MAX_COMMAND_LENGTH];
    strcpy(parse_buffer, command);

    *argc = 0;
    int i = 0;
    int cmd_start = 0;
    int in_word = 0;

    // Skip leading spaces
    while (parse_buffer[i] == ' ' || parse_buffer[i] == '\t') {
        i++;
    }

    // Extract command name
    cmd_start = i;
    while (parse_buffer[i] != '\0' && parse_buffer[i] != ' ' && parse_buffer[i] != '\t') {
        i++;
    }

    // Copy command name
    int cmd_len = i - cmd_start;
    strncpy(cmd, &parse_buffer[cmd_start], cmd_len);
    cmd[cmd_len] = '\0';

    // Parse arguments
    while (parse_buffer[i] != '\0' && *argc < MAX_ARGS) {
        // Skip spaces
        while (parse_buffer[i] == ' ' || parse_buffer[i] == '\t') {
            i++;
        }

        if (parse_buffer[i] == '\0') {
            break;
        }

        // Mark argument start
        args[*argc] = &parse_buffer[i];
        (*argc)++;

        // Find argument end
        while (parse_buffer[i] != '\0' && parse_buffer[i] != ' ' && parse_buffer[i] != '\t') {
            i++;
        }

        // Null-terminate argument
        if (parse_buffer[i] != '\0') {
            parse_buffer[i] = '\0';
            i++;
        }
    }
}

// Execute command
void shell_execute_command(const char* command) {
    char cmd[64];
    char* args[MAX_ARGS];
    int argc;

    shell_parse_command(command, cmd, args, &argc);

    if (cmd[0] == '\0') {
        return;  // Empty command
    }

    // Find and execute command
    for (int i = 0; commands[i].name != 0; i++) {
        if (strcmp(cmd, commands[i].name) == 0) {
            commands[i].handler(argc, args);
            return;
        }
    }

    // Command not found
    screen_print("Unknown command: ");
    screen_print(cmd);
    screen_print("\nType 'help' for available commands.\n");
}

// Built-in command: help
void cmd_help(int argc, char* args[]) {
    (void)argc;
    (void)args;

    screen_print("Available commands:\n");
    for (int i = 0; commands[i].name != 0; i++) {
        screen_print("  ");
        screen_print(commands[i].name);
        screen_print(" - ");
        screen_print(commands[i].description);
        screen_print("\n");
    }
}

// Built-in command: clear
void cmd_clear(int argc, char* args[]) {
    (void)argc;
    (void)args;
    screen_clear();
}

// Built-in command: echo
void cmd_echo(int argc, char* args[]) {
    for (int i = 0; i < argc; i++) {
        screen_print(args[i]);
        if (i < argc - 1) {
            screen_print(" ");
        }
    }
    screen_print("\n");
}

// Built-in command: about
void cmd_about(int argc, char* args[]) {
    (void)argc;
    (void)args;

    screen_print("\n");
    screen_print("MiniOS - Minimal x86 Kernel\n");
    screen_print("============================\n");
    screen_print("Version: 0.1\n");
    screen_print("Architecture: x86 (32-bit protected mode)\n");
    screen_print("Features:\n");
    screen_print("  - Custom bootloader\n");
    screen_print("  - Interrupt handling (IDT)\n");
    screen_print("  - Keyboard driver\n");
    screen_print("  - VGA text mode driver\n");
    screen_print("  - Basic shell\n");
    screen_print("\n");
    screen_print("Written in C and x86 assembly\n");
    screen_print("\n");
}
