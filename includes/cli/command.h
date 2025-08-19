#pragma once

typedef struct
{
    const char *name;
    void (*fnc)(char *args);
} Command;

// List of available commands
extern Command cmd_list[];

void help_cmd(char *cmd_name);
void cls_cmd(char* unused);
void show_info_cmd(char *unused);
void baudrate_cmd(char *arg);
// handshaking in uart
void execute_cmd(const char* input);