#include "includes/utils/color.h"
#include "includes/peripheral/uart1.h"
#include "includes/cli/mode.h"
#include "includes/cli/printcmd.h"

void main()
{
    // set up serial console
    uart_init();
    // say hello

    print_welcome_msg();

    // enter CLI
    chmod_to_cli_mode();
    handle_cli_mode();
}