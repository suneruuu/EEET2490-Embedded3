#pragma once

typedef struct
{
    const char *name;
    void (*fnc)(char *args);
} Command;

// List of available commands
extern Command cmd_list[];

void help_cmd(char *cmd);
void cls_cmd();
void show_info_cmd();
void baudrate_cmd(char *arg);
// handshaking in uart