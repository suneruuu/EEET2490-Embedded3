#pragma once

#define HISTORY_SIZE 10 //Keep 10 cmds
#define CMD_BUFFER_LEN 128 //Max input length per cmd

void add_history(const char *cmd); // Add a cmd line to the history
const char* get_history_up(void);   //Move the history cursor up. Return null if no history.
const char* get_history_down(void); //Move the history cursor down. Return null if no history
void reset_history_cursor(void); //reset the browser to the newest position (call after add_history if needed)