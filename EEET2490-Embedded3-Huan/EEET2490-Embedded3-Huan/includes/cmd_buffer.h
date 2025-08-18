#pragma once

// Command arity so we can call the correct handler safely.
typedef enum { CMD_ARITY_0 = 0, CMD_ARITY_1 = 1 } cmd_arity_t;

typedef struct {
    const char   *name;
    cmd_arity_t   arity;
    union {
        void (*fn0)(void);
        void (*fn1)(char *arg);
    } h;
} Command;

// Exposed by cmd.c
extern Command cmd_list[];
extern const int CMD_COUNT;

// Your handlers (from your snippet)
void help_cmd(char *cmd_name);     // help [cmd]
void cls_cmd(void);                // clear
void show_info_cmd(void);          // showinfo
void baudrate_cmd(char *cnum);     // baudrate <num>

// Call this with a full line (without the prompt).
void execute_command(char *line);
