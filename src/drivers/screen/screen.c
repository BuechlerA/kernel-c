#include "../../../include/drivers/screen.h"
#include "../../../include/lib/io.h"

// VGA text mode buffer
#define VIDEO_MEMORY 0xb8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define WHITE_ON_BLACK 0x0f

// Screen state
static int cursor_x = 0;
static int cursor_y = 0;
static unsigned short* video_memory = (unsigned short*)VIDEO_MEMORY;

// Move hardware cursor
void move_cursor(void) {
    unsigned short cursor_location = cursor_y * VGA_WIDTH + cursor_x;

    // Send location to VGA controller
    outb(0x3D4, 14);  // Send high byte command
    outb(0x3D5, cursor_location >> 8);
    outb(0x3D4, 15);  // Send low byte command
    outb(0x3D5, cursor_location);
}

// Scroll screen up by one line
void scroll(void) {
    // Move all lines up
    for (int y = 0; y < VGA_HEIGHT - 1; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            int src = (y + 1) * VGA_WIDTH + x;
            int dst = y * VGA_WIDTH + x;
            video_memory[dst] = video_memory[src];
        }
    }

    // Clear last line
    for (int x = 0; x < VGA_WIDTH; x++) {
        int pos = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
        video_memory[pos] = (WHITE_ON_BLACK << 8) | ' ';
    }

    cursor_y = VGA_HEIGHT - 1;
}

// Clear screen
void screen_clear(void) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        video_memory[i] = (WHITE_ON_BLACK << 8) | ' ';
    }
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

// Print a single character
void screen_putchar(char c) {
    if (c == '\n') {
        // Newline
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\b') {
        // Backspace
        if (cursor_x > 0) {
            cursor_x--;
            int pos = cursor_y * VGA_WIDTH + cursor_x;
            video_memory[pos] = (WHITE_ON_BLACK << 8) | ' ';
        }
    } else if (c == '\t') {
        // Tab (align to 4 spaces)
        cursor_x = (cursor_x + 4) & ~(4 - 1);
    } else if (c >= ' ') {
        // Printable character
        int pos = cursor_y * VGA_WIDTH + cursor_x;
        video_memory[pos] = (WHITE_ON_BLACK << 8) | c;
        cursor_x++;
    }

    // Check if we need to wrap to next line
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    // Scroll if needed
    if (cursor_y >= VGA_HEIGHT) {
        scroll();
    }

    // Update hardware cursor
    move_cursor();
}

// Print a string
void screen_print(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        screen_putchar(str[i]);
    }
}

// Print string at specific position
void screen_print_at(const char* str, int row, int col) {
    cursor_x = col;
    cursor_y = row;
    screen_print(str);
}

// Set cursor position
void screen_set_cursor(int x, int y) {
    cursor_x = x;
    cursor_y = y;
    move_cursor();
}

// Get cursor position
void screen_get_cursor(int* x, int* y) {
    *x = cursor_x;
    *y = cursor_y;
}
