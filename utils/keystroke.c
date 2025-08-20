#include "includes/utils/keystroke.h"
#include "includes/peripheral/uart0.h"

static int is_printable(char c){ return (c >= 32 && c < 127); }

static KeyType decode_simple_key(char c, char *outChar)
{
	if (c == '\r' || c == '\n') return KEY_ENTER;
	if (c == 0x08 || c == 0x7F)  return KEY_BACKSPACE;  // BS / DEL
	if (c == 0x09)               return KEY_TAB;        // TAB
	if (c == 0x15)               return KEY_CTRL_U;     // Ctrl-U
	if (c == 0x17)               return KEY_CTRL_W;     // Ctrl-W
	if (c == 0x03)               return KEY_CTRL_C;     // Ctrl-C
	if (is_printable(c)) { if (outChar) *outChar = c; return KEY_CHAR; }
	return (c == 0x1B) ? KEY_UNKNOWN : KEY_UNKNOWN;     // ESC handled elsewhere
}

static KeyType parse_escape_sequence(void)
{
	char c1 = uart_getc();
	if (c1 != '[') return KEY_UNKNOWN;
	char c2 = uart_getc();
	if (c2 == 'A') return KEY_UP;
	if (c2 == 'B') return KEY_DOWN;
	if (c2 == 'C') return KEY_RIGHT;
	if (c2 == 'D') return KEY_LEFT;
	return KEY_UNKNOWN;
}

int read_key_event(KeyEvent *event) {
	if (!event) return 0;
	static int has_pending = 0;
	static char pending = 0;
	static int caret_waiting = 0;
	if (has_pending) {
		has_pending = 0;
		event->type = KEY_CHAR;
		event->character = pending;
		return 1;
	}
	char c = uart_getc();
	if (caret_waiting) {
		caret_waiting = 0;
		if (c == 'U' || c == 'u') { event->type = KEY_CTRL_U; event->character = 0; return 1; }
		if (c == 'W' || c == 'w') { event->type = KEY_CTRL_W; event->character = 0; return 1; }
		if (c == 'C' || c == 'c') { event->type = KEY_CTRL_C; event->character = 0; return 1; }
		/* not a control mnemonic: emit '^' then queue this char */
		event->type = KEY_CHAR; event->character = '^';
		has_pending = 1; pending = c;
		return 1;
	}
	char out = 0;
	KeyType t = decode_simple_key(c, &out);
	/* Fallback for consoles that send caret sequences like '^U', '^W', '^C' */
	if (c == '^') { caret_waiting = 1; event->type = KEY_UNKNOWN; event->character = 0; return 1; }
	if (c == 0x1B) {
		// ESC-prefixed sequence (e.g., arrows). Fallback: emit '[' then queue next if unknown
		char c1 = uart_getc();
		if (c1 == '[') {
			char c2 = uart_getc();
			if (c2 == 'A') t = KEY_UP;
			else if (c2 == 'B') t = KEY_DOWN;
			else if (c2 == 'C') t = KEY_RIGHT;
			else if (c2 == 'D') t = KEY_LEFT;
			else {
				// unknown ESC-[x : emit '[' and queue x
				event->type = KEY_CHAR; event->character = '[';
				has_pending = 1; pending = c2;
				return 1;
			}
		} else {
			// Not '[' after ESC: emit ESC literally and queue c1
			event->type = KEY_CHAR; event->character = 0x1B;
			has_pending = 1; pending = c1;
			return 1;
		}
	}
	event->type = t;
	event->character = (t == KEY_CHAR) ? out : 0;
	return 1;
}


