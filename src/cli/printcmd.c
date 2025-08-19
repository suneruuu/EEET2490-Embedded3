#include "includes/utils/color.h"
#include "includes/cli/printcmd.h"
#include "includes/peripheral/uart0.h"

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
}

void print_cls_guide(void) {
    print_usage("clear");
    print_color("clear\n", YELLOW);
    print_color("Clear screen\n", WHITE);
    uart_puts("\n");
}

void print_show_info_guide(void) {
    print_usage("showinfo");
    print_color("show_info\n", YELLOW);
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