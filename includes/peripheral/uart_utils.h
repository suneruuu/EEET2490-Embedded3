#pragma once

int uart_read_ready(void); // 1 if available to read, 0 if not 

/* Tries to get one byte. Returns 1 if read and stores it in *out, else 0 */
int uart_try_getc(unsigned char *out);


