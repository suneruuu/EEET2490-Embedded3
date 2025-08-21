#include "includes/cli/mode.h"
#include "includes/cli/command.h"
#include "includes/cli/commandbf.h"
#include "includes/cli/printcmd.h"
#include "includes/utils/keystroke.h"
#include "includes/peripheral/uart0.h"
#include "includes/utils/color.h"
#include "includes/framebf.h"

static Mode current_mode = MODE_CLI;

/* ===== tiny utils ===== */
static void echo_bs(void) { uart_sendc('\b'); uart_sendc(' '); uart_sendc('\b'); }

static void clear_line(char *buf, int *cur) {
    while (*cur > 0) { (*cur)--; buf[*cur] = '\0'; echo_bs(); }
}

static void write_line(const char *src, char *buf, int *cur) {
    clear_line(buf, cur);
    int i = 0;
    while (src[i] && i < CMD_BUFFER_LEN - 1) { buf[i] = src[i]; i++; }
    buf[i] = '\0';
    *cur = i;
    uart_puts(buf);
}

/* print_prompt now provided by printcmd.c */

static void complete_buffer(char *buf, int *cur) {
    /* Completion for:
       1) First token (no spaces)
       2) Second token if first token is "help" and only a single arg is present */
    const Command *tbl = NULL; unsigned n = 0;
    get_command_table(&tbl, &n);

    /* detect first space */
    int sp = -1; for (int i = 0; i < *cur; i++) { if (buf[i] == ' ' || buf[i] == '\t') { sp = i; break; } }

    const char *prefix = buf; int prefix_len = *cur; /* default: whole buffer */
    if (sp >= 0) {
        /* only complete arg if first token is exactly "help" and no extra spaces */
        int i = 0; const char *HELP = "help"; int ok = 1;
        while (i < sp) { if (buf[i] != HELP[i]) { ok = 0; break; } i++; }
        if (!(ok && sp == 4)) { uart_sendc('\a'); return; }
        /* start of arg (skip spaces) */
        int a = sp + 1; while (a < *cur && (buf[a] == ' ' || buf[a] == '\t')) a++;
        /* disallow more spaces after arg start */
        for (int j = a; j < *cur; j++) { if (buf[j] == ' ' || buf[j] == '\t') { uart_sendc('\a'); ok = 0; break; } }
        if (!ok) return;
        prefix = &buf[a]; prefix_len = *cur - a;
    }

    /* gather matches by prefix against command names */
    unsigned matches = 0; int first = -1;
    for (unsigned i = 0; i < n; i++) {
        const char *name = tbl[i].name;
        int k = 0, ok = 1;
        while (k < prefix_len) { if (name[k] != prefix[k]) { ok = 0; break; } k++; }
        if (ok) { if (first < 0) first = (int)i; matches++; }
    }
    if (matches == 0) { uart_sendc('\a'); return; }

    /* extend common prefix relative to prefix */
    int p = prefix_len;
    for (;;) {
        char c = 0; int init = 0, allsame = 1;
        for (unsigned i = 0; i < n; i++) {
            const char *name = tbl[i].name;
            int k = 0, ok = 1;
            while (k < prefix_len) { if (name[k] != prefix[k]) { ok = 0; break; } k++; }
            if (!ok) continue;
            if (!name[p]) { allsame = 0; break; }
            if (!init) { c = name[p]; init = 1; }
            else if (name[p] != c) { allsame = 0; break; }
        }
        if (!allsame) break;
        buf[*cur] = c; (*cur)++; buf[*cur] = '\0'; uart_sendc(c);
        p++;
    }
}

/* ---- Mode checks ---- */
int is_cli_mode(void) {
    return current_mode == MODE_CLI ? 1 : 0;
}

int is_vid_mode(void) {
    return current_mode == MODE_VID ? 1 : 0;
}

int is_text_mode(void) {
return current_mode == MODE_TEXT ? 1 : 0;
}

int is_game_mode(void) {
return current_mode == MODE_GAME ? 1 : 0;
}

int is_img_mode(void) {
return current_mode == MODE_IMG ? 1 : 0;
}

/* ---- Mode handlers ---- */
void handle_cli_mode(void) {
    uart_puts("\r\nWelcome to ChaOS!\r\n");
    print_color("[HELP] ", YELLOW);
    uart_puts("Start by typing ");
    print_color("help", YELLOW);
    uart_puts(" to see available commands.\r\n");
    for (;;) {
        char buf[CMD_BUFFER_LEN];
        int cur = 0;
        buf[0] = '\0';
        reset_history_cursor();
        print_prompt();

        int new_prompt = 0;
        int history_active = 0;
        for (;;) {
            KeyEvent ev; read_key_event(&ev);
            switch (ev.type) {
                case KEY_ENTER:
                    buf[cur] = '\0';
                    uart_puts("\r\n");
                    if (cur > 0) { add_history(buf); execute_cmd(buf); }
                    new_prompt = 1;
                    break;
                case KEY_BACKSPACE:
                    if (cur > 0) { cur--; buf[cur] = '\0'; echo_bs(); }
                    history_active = 0;
                    break;
                case KEY_TAB:
                    complete_buffer(buf, &cur);
                    history_active = 0;
                    break;
                case KEY_CTRL_U:
                    clear_line(buf, &cur);
                    history_active = 0;
                    break;
                case KEY_CTRL_W:
                    /* Trim trailing spaces (if any), then delete back to previous space */
                    while (cur>0 && (buf[cur-1]==' ' || buf[cur-1]=='\t')) { cur--; buf[cur]='\0'; echo_bs(); }
                    while (cur>0 && (buf[cur-1]!=' ' && buf[cur-1]!='\t')) { cur--; buf[cur]='\0'; echo_bs(); }
                    history_active = 0;
                    break;
                case KEY_CTRL_C:
                    uart_puts("^C\r\n");
                    cur = 0; buf[0] = '\0';
                    new_prompt = 1;
                    history_active = 0;
                    break;
                case KEY_UP: {
                    const char *h = get_history_up();
                    if (h) { write_line(h, buf, &cur); history_active = 1; } else uart_sendc('\a');
                    break;
                }
                case KEY_DOWN: {
                    const char *h = get_history_down();
                    if (h) { write_line(h, buf, &cur); history_active = 1; } else uart_sendc('\a');
                    break;
                }
                case KEY_CHAR:
                    /* Map '_' (Shift + -) and '+' (Shift + =) to history when line is empty or currently showing history */
                    if ((cur == 0 || history_active) && ev.character == '_') {
                        const char *h = get_history_up();
                        if (h) { write_line(h, buf, &cur); history_active = 1; } else uart_sendc('\a');
                        break;
                    }
                    if ((cur == 0 || history_active) && ev.character == '+') {
                        const char *h = get_history_down();
                        if (h) { write_line(h, buf, &cur); history_active = 1; } else uart_sendc('\a');
                        break;
                    }
                    if (cur < CMD_BUFFER_LEN-1) { buf[cur++] = ev.character; buf[cur]='\0'; uart_sendc(ev.character); history_active = 0; }
                    else { uart_sendc('\a'); }
                    break;
                default:
                    break;
            }
            if (new_prompt) break;
        }
    }
}

void handle_vid_mode(void) {
    // TODO: implement Video mode loop / logic
}

void handle_text_mode(void) {
    framebf_init();
    print_name();
    print_image();
}

void handle_game_mode(void) {
    // TODO: implement Game mode loop / logic
}

void handle_img_mode(void) {
    framebf_init();
    print_image();
}

/* ---- Mode changing ---- */
void chmod_to_cli_mode(void) {
    framebf_clear();
    current_mode = MODE_CLI;
}

void chmod_to_vid_mode(void) {
    framebf_clear();
    current_mode = MODE_VID;
}

void chmod_to_text_mode(void) {
    framebf_clear();
    current_mode = MODE_TEXT;
}

void chmod_to_game_mode(void) {
    framebf_clear();
    current_mode = MODE_GAME;
}

void chmod_to_img_mode(void) {
    framebf_clear();
    current_mode = MODE_IMG;
}
