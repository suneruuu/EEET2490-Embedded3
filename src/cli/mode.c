#include "includes/cli/mode.h"

static Mode current_mode = MODE_CLI;

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
    // TODO: implement CLI mode loop / logic
}

void handle_vid_mode(void) {
    // TODO: implement Video mode loop / logic
}

void handle_text_mode(void) {
    // TODO: implement Text mode loop / logic
}

void handle_game_mode(void) {
    // TODO: implement Game mode loop / logic
}

void handle_img_mode(void) {
    // TODO: implement Image mode loop / logic
}

/* ---- Mode changing ---- */
void chmod_to_cli_mode(void) {
    current_mode = MODE_CLI;
}

void chmod_to_vid_mode(void) {
    current_mode = MODE_VID;
}

void chmod_to_text_mode(void) {
    current_mode = MODE_TEXT;
}

void chmod_to_game_mode(void) {
    current_mode = MODE_GAME;
}

void chmod_to_img_mode(void) {
    current_mode = MODE_IMG;
}
