#pragma once

typedef enum {
    MODE_CLI,
    MODE_VID,
    MODE_TEXT,
    MODE_GAME,
    MODE_IMG,
} Mode;

/* ---- Mode checks ---- */
int is_cli_mode(void);
int is_vid_mode(void);
int is_text_mode(void);
int is_game_mode(void);
int is_img_mode(void);

/* ---- Mode handlers ---- */
void handle_cli_mode(void);
void handle_vid_mode(void);
void handle_text_mode(void);
void handle_game_mode(void);
void handle_img_mode(void);

/* ---- Mode changing ---- */
void chmod_to_cli_mode(void);
void chmod_to_vid_mode(void);
void chmod_to_text_mode(void);
void chmod_to_game_mode(void);
void chmod_to_img_mode(void);