#include "includes/cli/cmd.h"
#include "includes/utils/color.h"
#include "includes/cli/cmd.h"
#include "includes/cli/print_cmd.h"
#include <string.h>
#include <stdio.h>
#include "includes/library/mbox.h"

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
    uart_puts("\033[2J\033[H"); //use ANSI escape sequence: \033[2J -> erase screen, \033[H -> move cursor to top-left
    uart_puts("MyOS> "); //reprint prompt
}

void show_info_cmd(char *unused) {
    get_mac_address();
    get_board_revision();
}

void baudrate_cmd(char *cnum) {
    int baud = atoi(cnum); //change this string to number
    switch(baud){
        case 9600:
        case 19200:
        case 38400:
        case 57600:
        case 115200:
            uart_set_baudrate(baud);
            uart_puts("Baudrate changed to ");
            char buffer[16];
            sprintf(buffer, "%d\n", baud);
            uart_puts(buffer);
            break;
        default:
            uart_puts("Please use 9600, 19200, 38400, 57600, or 11520 Baudrate.\n");
            break;
    }
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