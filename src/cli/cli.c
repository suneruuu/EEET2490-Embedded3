#include "includes/cli.h"
#include "includes/cmd_buffer.h"   /* for the Command struct definition (names) */

/* ===== tiny utils ===== */
static int kisprint(char c){ return (c>=32 && c<127); }
static void echo_bs(void){ uart_putc('\b'); uart_putc(' '); uart_putc('\b'); }
static void bell(void){ uart_putc('\a'); }

/* ===== history (ring) ===== */
static char hist[HIST_MAX][CLI_MAX];
static int  hist_count = 0;
static int  hist_head  = 0;
static int  hist_index = -1;

static void hist_add(const char *line){
    if (!line || !*line) return;
    {
        int prev = (hist_head + HIST_MAX - 1) % HIST_MAX;
        int same = 1, i = 0;
        while (line[i] || hist[prev][i]){ if (line[i] != hist[prev][i]){ same = 0; break; } i++; }
        if (same && hist_count > 0) return;
    }
    {
        int i = 0;
        for (; i < CLI_MAX-1 && line[i]; i++) hist[hist_head][i] = line[i];
        hist[hist_head][i] = '\0';
    }
    hist_head = (hist_head + 1) % HIST_MAX;
    if (hist_count < HIST_MAX) hist_count++;
}
static void hist_reset(void){ hist_index = -1; }
static int hist_get(int dir, char *out){ /* -1 older ('_'), +1 newer ('+') */
    if (hist_count == 0) return 0;
    if (hist_index < 0){
        if (dir < 0) hist_index = (hist_head + HIST_MAX - 1) % HIST_MAX;
        else return 0;
    } else {
        if (dir < 0){
            int oldest = (hist_head + HIST_MAX - hist_count) % HIST_MAX;
            if (hist_index == oldest) return 0;
            hist_index = (hist_index + HIST_MAX - 1) % HIST_MAX;
        } else {
            if (hist_index == (hist_head + HIST_MAX - 1) % HIST_MAX){
                hist_index = -1; out[0] = '\0'; return 1; /* leave history */
            }
            hist_index = (hist_index + 1) % HIST_MAX;
        }
    }
    {
        int i = 0; while (i < CLI_MAX-1 && hist[hist_index][i]){ out[i] = hist[hist_index][i]; i++; }
        out[i] = '\0';
    }
    return 1;
}

/* ===== small helpers (no stdlib) ===== */
static unsigned kstrlen(const char *s){ unsigned n=0; while (s && s[n]) n++; return n; }
static int starts_with(const char *s, const char *p){
    while (*p){ if (*s++ != *p++) return 0; } return 1;
}

/* ===== TAB completion (command names only) ===== */
static void complete_once(char *buf, int *cur){
    unsigned ncmds = 0;
    const Command *tbl = cmd_table(&ncmds);

    unsigned matches = 0; /* count matching command names */
    unsigned i;
    for (i = 0; i < ncmds; i++){
        if (starts_with(tbl[i].name, buf)) matches++;
    }
    if (matches == 0){ bell(); return; }

    /* extend to longest common prefix across matches */
    unsigned p = kstrlen(buf);
    for (;;){
        char c = 0; int init = 0, allsame = 1;
        for (i = 0; i < ncmds; i++){
            if (!starts_with(tbl[i].name, buf)) continue;
            if (!tbl[i].name[p]){ allsame = 0; break; }
            if (!init){ c = tbl[i].name[p]; init = 1; }
            else if (tbl[i].name[p] != c){ allsame = 0; break; }
        }
        if (!allsame) break;
        buf[*cur] = c; (*cur)++; buf[*cur] = '\0'; uart_putc(c);
        p++;
    }
}

/* ===== main read/execute loop ===== */
void cli_run(void){
    uart_init();
    uart_puts("\r\nWelcome to MyOS!\r\n");

    char buf[CLI_MAX]; int cur;

    for(;;){
        uart_puts(OS_PROMPT);
        cur = 0; buf[0] = '\0'; hist_reset();

        for(;;){
            char c = uart_getc();

            /* ENTER */
            if (c == '\r' || c == '\n'){
                buf[cur] = '\0'; uart_puts("\r\n");
                if (cur > 0){ hist_add(buf); execute_command(buf); }
                break; /* new prompt */
            }

            /* BACKSPACE / DEL */
            if (c == 0x08 || c == 0x7F){
                if (cur > 0){ cur--; buf[cur] = '\0'; echo_bs(); }
                continue;
            }

            /* TAB completion (command name) */
            if (c == 0x09){ complete_once(buf, &cur); continue; }

            /* '_' (UP) older history */
            if (c == 0x5F){
                char tmp[CLI_MAX];
                if (hist_get(-1, tmp)){
                    while (cur > 0){ cur--; buf[cur] = '\0'; echo_bs(); }
                    { int i=0; while (tmp[i] && i<CLI_MAX-1){ buf[i]=tmp[i]; i++; } buf[i]='\0'; cur=i; }
                    uart_puts(buf);
                } else bell();
                continue;
            }

            /* '+' (DOWN) newer history */
            if (c == 0x2B){
                char tmp[CLI_MAX];
                if (hist_get(+1, tmp)){
                    while (cur > 0){ cur--; buf[cur] = '\0'; echo_bs(); }
                    { int i=0; while (tmp[i] && i<CLI_MAX-1){ buf[i]=tmp[i]; i++; } buf[i]='\0'; cur=i; }
                    uart_puts(buf);
                } else bell();
                continue;
            }

            /* Ctrl-U clear current line */
            if (c == 0x15){ while (cur > 0){ cur--; buf[cur]='\0'; echo_bs(); } continue; }

            /* Ctrl-W erase previous word */
            if (c == 0x17){
                while (cur>0 && (buf[cur-1]==' ' || buf[cur-1]=='\t')){ cur--; buf[cur]='\0'; echo_bs(); }
                while (cur>0 && (buf[cur-1]!=' ' && buf[cur-1]!='\t')){ cur--; buf[cur]='\0'; echo_bs(); }
                continue;
            }

            /* Ctrl-C cancel line */
            if (c == 0x03){ uart_puts("^C\r\n"); cur = 0; buf[0] = '\0'; break; }

            /* printable */
            if (kisprint(c)){
                if (cur < CLI_MAX-1){ buf[cur++] = c; buf[cur] = '\0'; uart_putc(c); }
                else bell();
                continue;
            }

            /* ignore others */
        }
    }
}
