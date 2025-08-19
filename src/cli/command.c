#include "includes/cli/cmd.h"
#include "includes/utils/color.h"
#include "includes/cli/print_cmd.h"
#include "includes/utils/string.h"
#include "includes/peripheral/uart0.h"

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
            print_cmd_not_found();
    }
}

void cls_cmd() { // clear scr

}

void show_info_cmd() {

}

void baudrate_cmd(char *cnum) {

}