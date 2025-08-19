#pragma once

#include "includes/utils/string.h"

typedef enum KeyType {
	KEY_UNKNOWN = 0,
	KEY_CHAR,
	KEY_ENTER,
	KEY_BACKSPACE,
	KEY_TAB,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_CTRL_U,
	KEY_CTRL_W,
	KEY_CTRL_C
} KeyType;

typedef struct KeyEvent {
	KeyType type;
	char character;
} KeyEvent;

int read_key_event(KeyEvent *event);


