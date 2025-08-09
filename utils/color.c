#include "includes/utils/color.h"
#include "includes/peripheral/uart0.h"
#include "includes/peripheral/uart1.h"

void print_color(const char *str, const char *color_code)
{
    uart_puts(color_code);
    uart_puts(str);
    uart_puts(COLOR_RESET);
}
