#include "includes/color.h"
#include "includes/uart0.h"
#include "includes/uart1.h"

void print_color(const char *str, const char *color_code)
{
    uart_puts(color_code);
    uart_puts(str);
    uart_puts(COLOR_RESET);
}
