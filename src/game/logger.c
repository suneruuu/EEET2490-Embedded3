#include "../../includes/game/logger.h"
#include "../../includes/peripheral/uart1.h"   // or whatever provides uart_puts/uart_putc

static int commandCount = 0;

// Simple int to string converter
static void intToStr(int value, char *buffer) {
    char temp[12];
    int i = 0, j = 0;

    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    while (value > 0) {
        temp[i++] = (value % 10) + '0';
        value /= 10;
    }

    while (i > 0) {
        buffer[j++] = temp[--i];
    }
    buffer[j] = '\0';
}

void initLogger() {
    commandCount = 0;
    uart_puts("\033[32m[LOG]\033[0m Logger initialized.\n");
}

void logCommand(char command) {
    commandCount++;

    uart_puts("\033[32m[LOG]\033[0m Command ");
    char numBuf[12];
    intToStr(commandCount, numBuf);
    uart_puts(numBuf);

    uart_puts(" received: '");
    uart_sendc(command);
    uart_puts("'\n");
}

void printLogSummary() {
    uart_puts("=========== GAME LOG SUMMARY ===========\n");
    uart_puts("Total commands received: ");

    char numBuf[12];
    intToStr(commandCount, numBuf);
    uart_puts(numBuf);

    uart_puts("\n========================================\n");
}
