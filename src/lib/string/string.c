#include "../../../include/lib/string.h"

// Calculate string length
int strlen(const char* str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

// Compare two strings
int strcmp(const char* str1, const char* str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return str1[i] - str2[i];
        }
        i++;
    }
    return str1[i] - str2[i];
}

// Compare first n characters of two strings
int strncmp(const char* str1, const char* str2, int n) {
    for (int i = 0; i < n; i++) {
        if (str1[i] != str2[i]) {
            return str1[i] - str2[i];
        }
        if (str1[i] == '\0') {
            return 0;
        }
    }
    return 0;
}

// Copy string
char* strcpy(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return dest;
}

// Copy first n characters
char* strncpy(char* dest, const char* src, int n) {
    int i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    return dest;
}

// Concatenate strings
char* strcat(char* dest, const char* src) {
    int dest_len = strlen(dest);
    int i = 0;
    while (src[i] != '\0') {
        dest[dest_len + i] = src[i];
        i++;
    }
    dest[dest_len + i] = '\0';
    return dest;
}

// Find character in string
char* strchr(const char* str, int c) {
    while (*str != '\0') {
        if (*str == c) {
            return (char*)str;
        }
        str++;
    }
    return 0;
}

// Set memory to value
void* memset(void* ptr, int value, int num) {
    unsigned char* p = (unsigned char*)ptr;
    for (int i = 0; i < num; i++) {
        p[i] = (unsigned char)value;
    }
    return ptr;
}

// Copy memory
void* memcpy(void* dest, const void* src, int num) {
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;
    for (int i = 0; i < num; i++) {
        d[i] = s[i];
    }
    return dest;
}

// Compare memory
int memcmp(const void* ptr1, const void* ptr2, int num) {
    const unsigned char* p1 = (const unsigned char*)ptr1;
    const unsigned char* p2 = (const unsigned char*)ptr2;
    for (int i = 0; i < num; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] - p2[i];
        }
    }
    return 0;
}

// Convert integer to string
void itoa(int value, char* str, int base) {
    char* ptr = str;
    char* ptr1 = str;
    char tmp_char;
    int tmp_value;

    // Handle negative numbers for base 10
    if (value < 0 && base == 10) {
        *ptr++ = '-';
        ptr1++;
        value = -value;
    }

    // Convert to string (reversed)
    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "0123456789abcdef"[tmp_value - value * base];
    } while (value);

    *ptr-- = '\0';

    // Reverse string
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}

// Convert string to integer
int atoi(const char* str) {
    int result = 0;
    int sign = 1;
    int i = 0;

    // Skip whitespace
    while (str[i] == ' ' || str[i] == '\t') {
        i++;
    }

    // Handle sign
    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }

    // Convert digits
    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
        i++;
    }

    return sign * result;
}
