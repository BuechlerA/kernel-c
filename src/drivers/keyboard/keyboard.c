#include "../../../include/drivers/keyboard.h"
#include "../../../include/lib/io.h"

// US QWERTY keyboard scancode to ASCII mapping
static unsigned char scancode_to_ascii[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, /* Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, /* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    0, /* Right shift */
    '*',
    0,  /* Alt */
    ' ',  /* Space */
    0,  /* Caps lock */
    0,  /* F1 */
    0, 0, 0, 0, 0, 0, 0, 0, 0,  /* F2-F10 */
    0,  /* Num lock */
    0,  /* Scroll Lock */
    0,  /* Home */
    0,  /* Up Arrow */
    0,  /* Page Up */
    '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
    '+',
    0,  /* End */
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert */
    0,  /* Delete */
    0, 0, 0,
    0,  /* F11 */
    0,  /* F12 */
    0,  /* All other keys are undefined */
};

// Shifted characters
static unsigned char scancode_to_ascii_shift[128] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, /* Control */
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0, /* Left shift */
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',
    0, /* Right shift */
    '*',
    0,  /* Alt */
    ' ',  /* Space */
    0,  /* Caps lock */
    0,  /* F1 */
    0, 0, 0, 0, 0, 0, 0, 0, 0,  /* F2-F10 */
    0,  /* Num lock */
    0,  /* Scroll Lock */
    0,  /* Home */
    0,  /* Up Arrow */
    0,  /* Page Up */
    '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
    '+',
    0,  /* End */
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert */
    0,  /* Delete */
    0, 0, 0,
    0,  /* F11 */
    0,  /* F12 */
    0,  /* All other keys are undefined */
};

// Keyboard state
static int shift_pressed = 0;
static int caps_lock = 0;

// External shell function
extern void shell_handle_input(char c);

// Keyboard handler - called by IRQ1
void keyboard_handler(void) {
    unsigned char scancode;

    // Read scancode from keyboard data port
    scancode = inb(0x60);

    // Check if key released (bit 7 set)
    if (scancode & 0x80) {
        // Key released
        scancode &= 0x7F;  // Clear bit 7

        // Check for shift release
        if (scancode == 0x2A || scancode == 0x36) {
            shift_pressed = 0;
        }
    } else {
        // Key pressed

        // Check for shift press
        if (scancode == 0x2A || scancode == 0x36) {
            shift_pressed = 1;
            return;
        }

        // Check for caps lock
        if (scancode == 0x3A) {
            caps_lock = !caps_lock;
            return;
        }

        // Convert scancode to ASCII
        char ascii = 0;
        if (shift_pressed) {
            ascii = scancode_to_ascii_shift[scancode];
        } else {
            ascii = scancode_to_ascii[scancode];
        }

        // Apply caps lock to letters
        if (caps_lock && ascii >= 'a' && ascii <= 'z') {
            ascii -= 32;  // Convert to uppercase
        } else if (caps_lock && ascii >= 'A' && ascii <= 'Z' && shift_pressed) {
            ascii += 32;  // Convert to lowercase
        }

        // Send character to shell if valid
        if (ascii != 0) {
            shell_handle_input(ascii);
        }
    }
}
