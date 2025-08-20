#include "includes/utils/color.h"
#include "includes/cli/printcmd.h"
#include "includes/peripheral/uart0.h"
#include "includes/framebf.h"
#include "assets/images/meme.c"

#define name_msg1 "Ngo Minh Hieu - s3940891"
#define name_msg2 "Nguyen Ngoc Huan - s3979896"
#define name_msg3 "Nguyen Le Thuc Quynh - s3924993"
#define name_msg4 "Ngo Duc Minh Quan - s3938194"

void print_prompt(void) {
    uart_puts("MyOS> ");
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
    print_color("cli|text|img\n", HI_GREEN);
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
    drawString(450, 760, "Hello!", 0xFFFFFFFF,3);
    drawString(450, 790, name_msg1, 0xFFFFFFFF, 2);
    drawString(450, 820, name_msg2, 0xFFFFFFFF, 2);
    drawString(450, 850, name_msg3, 0xFFFFFFFF, 2);
    drawString(450, 880, name_msg4, 0xFFFFFFFF, 2);
}

void print_image(void) {
    framebf_init();
    drawImage(meme, 400, 200, 612, 612);
}

void print_welcome_msg(void) {
    uart_puts(
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
        "Developed by Ngo Minh Hieu - s3940891\n"
        "             Nguyen Ngoc Huan - s3979896\n"
        "             Nguyen Le Thuc Quynh - s3924993\n"
        "             Ngo Duc Minh Quan - s3938194\n"
    );
}