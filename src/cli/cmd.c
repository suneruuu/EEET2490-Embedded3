#include "includes/cli/cmd.h"
#include "includes/utils/color.h"
#include "includes/cli/cmd.h"
#include "includes/cli/print_cmd.h"
#include <string.h>
#include <stdio.h>

#define CMD_LIST_SIZE (sizeof(cmd_list) / sizeof(Command))

Command cmd_list[] = {
    {"help", help_cmd},
    {"clear", cls_cmd}, 
    {"showinfo", show_info_cmd},
    {"baudrate", baudrate_cmd},
    // {"handshake", handshake_cmd}, // in uart
};

void help_cmd(char *cmd_name) { // <blank> | <cmd_name>
    uart_puts("\n");

    // list all
    if (strcmp(cmd_name, "") == 0) { // <blank> case
        print_help_guide();
        print_cls_guide();
        print_show_info_guide();
        print_baudrate_guide();
        print_handshake_guide();
    } else { // <cmd_name> case
        if (strcmp(cmd_name, "help") == 0)
            print_help_guide();
        else if (strcmp(cmd_name, "clear") == 0)
            print_cls_guide();
        else if (strcmp(cmd_name, "showinfo") == 0) 
            print_show_info_guide();
        else if (strcmp(cmd_name, "baudrate") == 0)
            print_baudrate_guide();
        else if (strcmp(cmd_name, "handshake") == 0)
            print_handshake_guide();
        else 
            print_error_not_found();
    }
}

void cls_cmd(char* unused) { // clear scr

}

void show_info_cmd(char *unused) {

}

void baudrate_cmd(char *cnum) {

}

void execute_cmd(const char* input){
    char cmd[64];
    char args[128];
    int match = sscanf(input, "%63s %127[^\n]", cmd, args); //%127[^\n] reads the remainder including spaces until newline.
    if (match <= 0){
        return;
    }

    for(size_t i = 0; i < CMD_LIST_SIZE; i++){
        if(strcmp(cmd, cmd_list[i].name) == 0){
            if(match == 1){
                cmd_list[i].fnc("");
            } else{
                cmd_list[i].fnc(args);
            }
            return;
        }
    }
    uart_puts("Unknow command. Please type 'help' for the list.\n");
}