#pragma once

typedef struct
{
    const char *name;
    void (*fnc)(char *args);
} Command;

// List of available commands
extern Command cmd_list[];

void get_command_table(const Command **table, unsigned *count); // for TAB auto-complete
void help_cmd(char *cmd_name);
void cls_cmd(char* unused);
void show_info_cmd(char *unused);
void baudrate_cmd(char *arg);
void handshake_cmd(char *arg);
void execute_cmd(const char* input);
void mode_cmd(char *arg);
