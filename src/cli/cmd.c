#include "includes/cli/cmd.h"

Command cmd_list[] = {
    {"help", help_cmd},
    {"clear", cls_cmd}, 
    {"showinfo", show_info_cmd},
    {"baudrate", baudrate_cmd},
    // {"handshake", handshake_cmd}, // in uart
};

void help_cmd(char *name) { // <blank> | <cmd_name>

}

void cls_cmd() { // clear scr

}

void show_info_cmd() {

}

void baudrate_cmd(char *cnum) {

}