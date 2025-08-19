#include "../../includes/peripheral/uart0.h"
#include "../../includes/peripheral/uart1.h"

/* Use UART0 flags by default; switchable later if needed */

int uart_read_ready(void)
{
#if 1 /* UART0 */
	return (UART0_FR & UART0_FR_RXFE) ? 0 : 1;
#else /* UART1 */
	return (AUX_MU_LSR & 0x01) ? 1 : 0;
#endif
}

int uart_try_getc(unsigned char *out)
{
	if (!out) return 0;
	if (!uart_read_ready()) return 0;
	*out = uart_getc();
	return 1;
}


