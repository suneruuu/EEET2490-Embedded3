#include "includes/cli/mode.h"

static Mode current_mode = MODE_CLI;

/* ---- Mode checks ---- */
int is_cli_mode(void) {
    // TODO: return 1 if current mode is CLI, else 0
    return 0;
}

int is_vid_mode(void) {
    // TODO: return 1 if current mode is Video, else 0
    return 0;
}

int is_text_mode(void) {
    // TODO: return 1 if current mode is Text, else 0
    return 0;
}

int is_game_mode(void) {
    // TODO: return 1 if current mode is Game, else 0
    return 0;
}

int is_img_mode(void) {
    // TODO: return 1 if current mode is Image, else 0
    return 0;
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
    // TODO: set current mode to CLI
}

void chmod_to_vid_mode(void) {
    // TODO: set current mode to Video
}

void chmod_to_text_mode(void) {
    // TODO: set current mode to Text
}

void chmod_to_game_mode(void) {
    // TODO: set current mode to Game
}

void chmod_to_img_mode(void) {
    // TODO: set current mode to Image
}
