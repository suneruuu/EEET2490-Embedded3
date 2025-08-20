#include "includes/utils/string.h"

int strcmp(const char *a, const char *b) {
    while (*a && (*a == *b)) { a++; b++; }
    return (unsigned char)*a - (unsigned char)*b;
}

size_t strlen(const char *s) {
    const char *p = s;
    while (*p) p++;
    return (size_t)(p - s);
}

void *memcpy(void *dst, const void *src, size_t n) {
    unsigned char *d = (unsigned char*)dst;
    const unsigned char *s = (const unsigned char*)src;
    while (n--) *d++ = *s++;
    return dst;
}

char *strncpy(char *dst, const char *src, size_t n) {
    size_t i = 0;
    for (; i < n && src[i]; ++i) dst[i] = src[i];
    for (; i < n; ++i) dst[i] = '\0';
    return dst;
}

int atoi(const char *s) {
    if (!s) return 0;
    int sign = 1;
    long result = 0;
    while (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r' || *s == '\v' || *s == '\f') s++;
    if (*s == '+' || *s == '-') {
        if (*s == '-') sign = -1;
        s++;
    }
    while (*s >= '0' && *s <= '9') {
        result = result * 10 + (*s - '0');
        s++;
    }
    return (int)(result * sign);
}

int split_first_token(const char *input, char *cmd, size_t cmd_size, char *args, size_t args_size) {
    if (!input || !cmd || cmd_size == 0) return 0;
    cmd[0] = '\0';
    if (args && args_size) args[0] = '\0';

    // skip leading spaces
    while (*input == ' ' || *input == '\t') input++;
    if (*input == '\0') return 0;

    // copy cmd
    size_t i = 0;
    while (*input && *input != ' ' && *input != '\t' && i + 1 < cmd_size) {
        cmd[i++] = *input++;
    }
    cmd[i] = '\0';

    // skip spaces before args
    while (*input == ' ' || *input == '\t') input++;
    if (*input == '\0') return 1;

    // copy rest as args
    if (args && args_size) {
        size_t j = 0;
        while (*input && j + 1 < args_size) {
            args[j++] = *input++;
        }
        args[j] = '\0';
    }
    return 2;
}