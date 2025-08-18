#pragma once

#define HISTORY_SIZE 10
#define CMD_BUFFER_LEN 128

void add_history(const char *cmd);
const char* get_history_up(void);
const char* get_history_down(void);