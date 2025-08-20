#include "includes/utils/color.h"
#include "includes/peripheral/uart0.h"
#include "includes/peripheral/uart1.h"

void print_color(const char *str, const char *color_code)
{
    uart_puts((char *)color_code);
    uart_puts((char *)str);
    uart_puts((char *)COLOR_RESET);
}
