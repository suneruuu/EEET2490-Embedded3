#pragma once

#ifndef NULL
#define NULL ((void *)0)
#endif

typedef unsigned long size_t; // 64-bit ARM

int strcmp(const char *a, const char *b);
size_t strlen(const char *s);
void *memcpy(void *dst, const void *src, size_t n);
char *strncpy(char *dst, const char *src, size_t n);

int atoi(const char *s); // ASCII to Integer

/* Parses first token and the rest of line. Returns 0=no token, 1=only token, 2=token+args. */
int split_first_token(const char *input, char *cmd, size_t cmd_size, char *args, size_t args_size);