#include "includes/cmd_buffer.h"
#include "includes/color.h"
#include "includes/print_cmd.h"

// --- UART HAL you provide elsewhere ---
extern void uart_putc(char c);
extern void uart_puts(const char *s);
extern unsigned int uart_get_baudrate(void);           // return 0 if unknown
extern int  uart_set_baudrate(unsigned int baud);      // return 0 on success

// ---- small freestanding helpers ----
static int kstrcmp(const char *a, const char *b){
    while (*a && *b && *a == *b){ a++; b++; }
    return ((unsigned char)*a) - ((unsigned char)*b);
}
static int kisspace(char c){ return (c==' '||c=='\t'||c=='\r'||c=='\n'); }
static int katoi(const char *s){
    int n=0, neg=0; if (!s) return 0;
    while (kisspace(*s)) s++;
    if (*s=='+' || *s=='-'){ neg = (*s=='-'); s++; }
    while (*s>='0' && *s<='9'){ n = n*10 + (*s - '0'); s++; }
    return neg ? -n : n;
}
static void kprint_u32(unsigned int x){
    char buf[16]; int i=0; unsigned int d=1, t=x;
    while (t>=10){ t/=10; d*=10; }
    do { buf[i++] = '0' + (x/d)%10; d/=10; } while (d);
    buf[i] = '\0'; uart_puts(buf);
}

// --- Forward declarations for usage/help printing (you already have these) ---
// print_help_guide(), print_cls_guide(), print_show_info_guide(),
// print_baudrate_guide(), print_handshake_guide(), print_error_not_found()

// ===================== Command list =====================
Command cmd_list[] = {
    { "help",     CMD_ARITY_1, .h.fn1 = help_cmd },
    { "clear",    CMD_ARITY_0, .h.fn0 = cls_cmd },
    { "showinfo", CMD_ARITY_0, .h.fn0 = show_info_cmd },
    { "baudrate", CMD_ARITY_1, .h.fn1 = baudrate_cmd },
    // {"handshake", CMD_ARITY_0, .h.fn0 = handshake_cmd },
};
const int CMD_COUNT = (int)(sizeof(cmd_list)/sizeof(cmd_list[0]));

// ===================== Implementations =====================

void help_cmd(char *cmd_name) { // "" | <cmd_name>
    uart_puts("\r\n");
    if (!cmd_name || kstrcmp(cmd_name, "") == 0) {
        print_help_guide();
        print_cls_guide();
        print_show_info_guide();
        print_baudrate_guide();
        print_handshake_guide();
        return;
    }
    if (kstrcmp(cmd_name, "help") == 0)          print_help_guide();
    else if (kstrcmp(cmd_name, "clear") == 0)    print_cls_guide();
    else if (kstrcmp(cmd_name, "showinfo") == 0) print_show_info_guide();
    else if (kstrcmp(cmd_name, "baudrate") == 0) print_baudrate_guide();
    else if (kstrcmp(cmd_name, "handshake") == 0) print_handshake_guide();
    else                                         print_error_not_found();
}

void cls_cmd(void) {
    // ANSI clear screen + cursor home (works with minicom/screen/tty)
    uart_puts("\x1b[2J\x1b[H");
}

void show_info_cmd(void) {
    uart_puts("BareOS on Raspberry Pi 3\r\n");
    uart_puts("Build: " __DATE__ " " __TIME__ "\r\n");
    unsigned int br = uart_get_baudrate();
    if (br) {
        uart_puts("UART baudrate: ");
        kprint_u32(br);
        uart_puts("\r\n");
    }
}

// Usage:
//   baudrate            -> show current and usage
//   baudrate <number>   -> set UART baudrate (e.g., 115200)
void baudrate_cmd(char *cnum) {
    if (!cnum || kstrcmp(cnum,"")==0) {
        uart_puts("Usage: baudrate <number>\r\n");
        unsigned int cur = uart_get_baudrate();
        if (cur) { uart_puts("Current: "); kprint_u32(cur); uart_puts("\r\n"); }
        return;
    }

    int v = katoi(cnum);
    if (v <= 0) {
        uart_puts("Invalid baudrate.\r\n");
        return;
    }

    // (Optional) restrict to common rates for the lab
    static const int allowed[] = {9600,19200,38400,57600,115200,230400,460800,921600};
    int ok = 0;
    for (unsigned i=0;i<sizeof(allowed)/sizeof(allowed[0]);i++) if (v==allowed[i]) { ok=1; break; }
    if (!ok) {
        uart_puts("Unsupported baud. Try: 9600..921600\r\n");
        return;
    }

    if (uart_set_baudrate((unsigned)v) == 0) {
        uart_puts("Baudrate set to "); kprint_u32((unsigned)v); uart_puts("\r\n");
    } else {
        uart_puts("Failed to set baudrate.\r\n");
    }
}
