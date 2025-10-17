#ifndef SCREEN_H
#define SCREEN_H

// Screen driver functions
void screen_clear(void);
void screen_putchar(char c);
void screen_print(const char* str);
void screen_print_at(const char* str, int row, int col);
void screen_set_cursor(int x, int y);
void screen_get_cursor(int* x, int* y);
void move_cursor(void);
void scroll(void);

#endif
