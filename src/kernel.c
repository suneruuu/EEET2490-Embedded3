#include "includes/utils/color.h"
#include "includes/peripheral/uart1.h"
#include "includes/peripheral/uart0.h"
#include "includes/cli/mode.h"
#include "includes/cli/printcmd.h"
#include "..\includes\mbox.h"
#include "..\includes\framebf.h"
#include "..\includes\color.h"

void main()
{

    // enter CLI
    chmod_to_cli_mode();
    handle_cli_mode();
}