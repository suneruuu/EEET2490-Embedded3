#include "includes/utils/color.h"
#include "includes/peripheral/uart1.h"
#include "includes/peripheral/uart0.h"
#include "includes/cli/mode.h"
#include "includes/cli/printcmd.h"
#include "includes/library/mbox.h"
#include "includes/framebf.h"
#include "includes/color.h"

void main(void)
{
    // Initialize framebuffer
    framebf_init();
    // Enter CLI on UART
    print_welcome_msg();
    chmod_to_cli_mode();
    handle_cli_mode();
}
