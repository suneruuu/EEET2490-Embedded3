#include "includes/cli/cmd.h"
#include "includes/utils/color.h"
#include "includes/cli/cmd.h"
#include "includes/cli/print_cmd.h"

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
    if (strcmp(cmd_name, "") == 0) {
        print_help_guide();
        print_cls_guide();
        print_show_info_guide();
        print_baudrate_guide();
        print_handshake_guide();
    } else {
        /* TODO*/
    }
}

void cls_cmd() { // clear scr

}

void show_info_cmd() {

}

void baudrate_cmd(char *cnum) {

}