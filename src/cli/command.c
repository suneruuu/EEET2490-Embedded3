#include "includes/cli/command.h"
#include "includes/utils/color.h"
#include "includes/cli/printcmd.h"
#include "includes/utils/string.h"
#include "includes/peripheral/uart0.h"
#include "includes/peripheral/uart1.h"

#define CMD_LIST_SIZE (sizeof(cmd_list) / sizeof(Command))

Command cmd_list[] = {
    {"help", help_cmd},
    {"clear", cls_cmd}, 
    {"showinfo", show_info_cmd},
    {"baudrate", baudrate_cmd},
    {"handshake", handshake_cmd},
};

void get_command_table(const Command **table, unsigned *count) {
    if (table) *table = cmd_list;
    if (count) *count = (unsigned)CMD_LIST_SIZE;
}

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
            print_cmd_not_found();
    }
}

void cls_cmd(char* unused) { // clear scr
    uart_puts("\033[2J\033[H"); //use ANSI escape sequence: \033[2J -> erase screen, \033[H -> move cursor to top-left
    uart_puts("MyOS> "); //reprint prompt
}

void show_info_cmd(char *unused) {
    uart_puts("Board Revision: check this later\n");
    uart_puts("MAC Address: check this later\n");
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
            uart_puts(cnum);
            uart_puts("\n");
            break;
        default:
            uart_puts("Only support baudrate 9600, 19200, 38400, 57600, or 11520. Try again.\n");
            break;
    }
}

void handshake_cmd(char *arg) {
    // Syntax: handshake on | off
    if (strcmp(arg, "on") == 0) {
        uart_set_handshake(1);
        uart_puts("Handshake enabled (CTS/RTS)\n");
    } else if (strcmp(arg, "off") == 0) {
        uart_set_handshake(0);
        uart_puts("Handshake disabled\n");
    } else {
        uart_puts("Usage: handshake on|off\n");
    }
}

void execute_cmd(const char* input) {
    char cmd[64];
    char args[128];
    int match = split_first_token(input, cmd, sizeof(cmd), args, sizeof(args));
    if (match <= 0) return;

    for(size_t i = 0; i < CMD_LIST_SIZE; i++) {
        if (strcmp(cmd, cmd_list[i].name) == 0) {
            if (match == 1){
                cmd_list[i].fnc("");
            } else{
                cmd_list[i].fnc(args);
            }
            return;
        }
    }
    print_cmd_not_found();
}