#include <string.h>
#include "includes/cli/cmd_buffer.h"

static char history[HISTORY_SIZE][CMD_BUFFER_LEN];
static int history_count = 0;
static int history_index = 0;
//Cursor: initial index [0]
//history_index == history_count -> position in blank new line
//press Up to decrease (old entry) and Down to increase (new entry)

void add_history(const char* cmd){
    if(strlen(cmd == 0)){
        return;
    }
    strncpy(history[history_count % HISTORY_SIZE], cmd, CMD_BUFFER_LEN);
    history[history_count % HISTORY_SIZE][CMD_BUFFER_LEN - 1] = '\0';
    history_count++;
    history_index = history_count; //reset the history to the newest (new line)
}

const char* get_history_down(void){
    if(history_count == 0){
        return NULL;
    }
    if(history_index < history_count - 1){
        history_index++;
    }
    return history[history_index % HISTORY_SIZE];
}

const char*get_history_up(void){
    if(history_count == 0){
        return NULL;
    }
    if(history_index > 0){
        history_index--;
    }
    return history[history_index % HISTORY_SIZE];
}

void reset_history_cursor(void){
    history_index = history_count;
}