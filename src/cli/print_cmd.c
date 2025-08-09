#include "includes/utils/color.h"

void print_usage(char *command)
{
    print_color("Usage description for command: ", WHITE);
    print_color(command, YELLOW);
    uart_puts("\n");
}

void print_help_guide() {
    print_usage("help");
    print_color("help\n", YELLOW);
    print_color("Show brief information of all commands\n", WHITE);
    uart_puts("\n");
    print_color("help ", YELLOW);
    print_color("<command_name>\n", HI_GREEN);
    print_color("Show full information of a specific command\n", WHITE);
    uart_puts("\n");
}

void print_cls_guide() {
    print_usage("clear");
    print_color("clear\n", YELLOW);
    print_color("Clear screen\n", WHITE);
    uart_puts("\n");
}

void print_show_info_guide() {
    print_usage("showinfo");
    print_color("show_info\n", YELLOW);
    print_color("Show board revision (value and information) and board MAC address in correct format\n", WHITE);
    uart_puts("\n");
}

void print_baudrate_guide() {
    print_usage("baudrate");
    print_color("baudrate\n", YELLOW);
    print_color("Allow the user to change the baudrate of current UART being used\n"
                "Support for baudrate: 9600, 19200, 38400, 57600, and 115200\n", WHITE);
    uart_puts("\n");
}

void print_handshake_guide() {
    print_usage("handshake");
    print_color("handshake\n", YELLOW);
    print_color("Allow the user to turn on/off CTS/RTS handshaking on current UART if possible.\n", WHITE);
    uart_puts("\n");
}