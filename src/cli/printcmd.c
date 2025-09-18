#include "includes/utils/color.h"
#include "includes/cli/printcmd.h"
#include "includes/peripheral/uart0.h"
#include "includes/framebf.h"
#include "assets/images/meme.c"
#include "assets/images/biome.c"
#include "assets/images/meme2.c"
#include "assets/images/hello.c"
#include "assets/images/cinema.c"
#include "includes\timer.h"


#define name_msg1 "Ngo Minh Hieu - s3940891"
#define name_msg2 "Nguyen Ngoc Huan - s3979896"
#define name_msg3 "Nguyen Le Thuc Quynh - s3924993"
#define name_msg4 "Ngo Duc Minh Quan - s3938194"
#define Welcome_msg \
"\n"\
":::::::::: :::::::::: :::::::::: ::::::::::: ::::::::   :::     ::::::::   :::::::\n"\
":+:        :+:        :+:            :+:    :+:    :+: :+:     :+:    :+: :+:   :+:\n"\
"+:+        +:+        +:+            +:+          +:+ +:+ +:+  +:+    +:+ +:+  :+:+\n"\
"+#++:++#   +#++:++#   +#++:++#       +#+        +#+  +#+  +:+   +#++:++#+ +#+ + +:+\n"\
"+#+        +#+        +#+            +#+      +#+   +#+#+#+#+#+       +#+ +#+#  +#+\n"\
"#+#        #+#        #+#            #+#     #+#          #+#  #+#    #+# #+#   #+#\n"\
"########## ########## ##########     ###    ##########    ###   ########   #######\n"\
"\n"\
" ::::::::  :::    :::     :::      ::::::::   ::::::::\n"\
":+:    :+: :+:    :+:   :+: :+:   :+:    :+: :+:    :+:\n"\
"+:+        +:+    +:+  +:+   +:+  +:+    +:+ +:+\n"\
"+#+        +#++:++#++ +#++:++#++: +#+    +:+ +#++:++#++\n"\
"+#+        +#+    +#+ +#+     +#+ +#+    +#+        +#+\n"\
"#+#    #+# #+#    #+# #+#     #+# #+#    #+# #+#    #+#\n"\
" ########  ###    ### ###     ###  ########   ########\n"\
"\n"\
"Developed by Ngo Minh Hieu - s3940891"\
"\n"\
"             Nguyen Ngoc Huan - s3979896"\
"\n"\
"             Nguyen Le Thuc Quynh - s3924993"\
"\n"\
"             Ngo Duc Minh Quan - s3938194\n"\
        
        
void print_prompt(void) {
    uart_puts("ChaOS> ");
}

void print_usage(char *command)
{
    print_color("Usage description for command: ", WHITE);
    print_color(command, YELLOW);
    uart_puts("\n");
}

void print_help_guide(void) {
    print_usage("help");
    print_color("help\n", YELLOW);
    print_color("Show brief information of all commands\n", WHITE);
    uart_puts("\n");
    print_color("help ", YELLOW);
    print_color("<command_name>\n", HI_GREEN);
    print_color("Show full information of a specific command\n", WHITE);
    uart_puts("\n");
    print_usage("mode");
    print_color("mode ", YELLOW);
    print_color("cli|text|img|vid|game\n", HI_GREEN);
    print_color("Switch runtime mode\n", WHITE);
    uart_puts("\n");
}

void print_cls_guide(void) {
    print_usage("clear");
    print_color("clear\n", YELLOW);
    print_color("Clear screen\n", WHITE);
    uart_puts("\n");
}

void print_show_info_guide(void) {
    print_usage("showinfo");
    print_color("showinfo\n", YELLOW);
    print_color("Show board revision (value and information) and board MAC address in correct format\n", WHITE);
    uart_puts("\n");
}

void print_baudrate_guide(void) {
    print_usage("baudrate");
    print_color("baudrate\n", YELLOW);
    print_color("Allow the user to change the baudrate of current UART being used\n"
                "Support for baudrate: 9600, 19200, 38400, 57600, and 115200\n", WHITE);
    uart_puts("\n");
}

void print_handshake_guide(void) {
    print_usage("handshake");
    print_color("handshake\n", YELLOW);
    print_color("Allow the user to turn on/off CTS/RTS handshaking on current UART if possible.\n", WHITE);
    uart_puts("\n");
}


void print_cmd_not_found(void) {
    print_color("COMMAND NOT FOUND!\n", RED);
    print_color("There are a list of available command\nType ", WHITE);
    print_color("help ", YELLOW);
    print_color("for an overview of the system", WHITE);
    uart_puts("\n");
}

void print_name(void) {
    framebf_init();
    print_welcome_msg();
}

void print_image(void) {
    framebf_init();
    drawImage(hello, 0, 0, 800, 600);
    wait_msec(300);
    framebf_clear();
    drawImage(meme, 0, 0, 720, 540);
    drawString(40, 790, "Huan", 0xFFFFFFFF, 2);
    drawString(315, 780, "Hieu", 0xFFFFFFFF, 2);
    drawString(1190, 200, "Quynh", 0xFFFFFFFF, 2);
    drawString(1275, 160, "BUGS", 0xFFFFFFFF, 2);
    drawString(1260, 140, "LOT OF", 0xFFFFFFFF, 2);
    wait_msec(300);
    framebf_clear();
    drawImage(biome, 0, 0, 720, 540);
    wait_msec(300);
    framebf_clear();
    drawImage(meme2, 0, 0, 720, 540);
    wait_msec(300);
    framebf_clear();
    // 
    // drawString(830, 50, "The", 0xFFFFFFFF, 3);
    // drawString(820, 80, "Game", 0xFFFFFFFF, 3);
    // drawString(500, 350, "Deadline", 0x00000000, 3);
    // drawString(110, 450, "Group 4", 0xFFFFFFFF, 4);
    // drawString(600, 830, "Quan", 0xFFFFFFFF, 2);
    // drawString(50, 570, "Who cause the most bugs?", 0x000000000, 3);
    // wait_msec(300);
    // framebf_clear();
    // drawImage(cinema, 0, 0, 800, 600);
    // drawString(300, 100, "Team 4", 0x0000BB00, 2);
    // drawString(150, 130, name_msg1, 0x00AA0000, 2);
    // drawString(150, 160, name_msg2, 0x000000CC, 2);
    // drawString(150, 190, name_msg3, 0x00FF8C00, 2);
    // drawString(150, 220, name_msg4, 0x00F7A189, 2);
}

void print_name_img(){
    framebf_init();
    framebf_clear();
    drawImage(cinema, 0, 0, 800, 600);
    drawString(300, 100, "Team 4", 0x0000BB00, 2);
    drawString(150, 130, name_msg1, 0x00AA0000, 2);
    drawString(150, 160, name_msg2, 0x000000CC, 2);
    drawString(150, 190, name_msg3, 0x00FF8C00, 2);
    drawString(150, 220, name_msg4, 0x00F7A189, 2);
}

void print_welcome_msg(void) {
    uart_puts(Welcome_msg);
}