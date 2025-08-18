#pragma once

/* ---- Minimal integer aliases (no stdlib) ---- */
typedef unsigned       u32;
typedef unsigned short u16;
typedef unsigned char  u8;

/* ---- UART HAL (you provide these) ---- */
void uart_init(void);
void uart_putc(char c);
char uart_getc(void);
void uart_puts(const char *s);

/* ---- Forward-declare Command type (definition lives in includes/cmd.h) ---- */
struct Command;  /* not used directly here */

/* ---- Only declare what this module owns ---- */
void execute_command(char *line);   /* your dispatcher provided by cmd.c */

/* ---- CLI configuration ---- */
#define OS_PROMPT  "MyOS> "
#define CLI_MAX    128
#define HIST_MAX   10

/* ---- Entry point ---- */
void cli_run(void);
