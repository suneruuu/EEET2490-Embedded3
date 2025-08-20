#pragma once

void print_usage(char *command);
void print_prompt(void);

/* Help sections */
void print_help_guide(void);
void print_cls_guide(void);
void print_show_info_guide(void);
void print_baudrate_guide(void);
void print_handshake_guide(void);
void print_cmd_not_found(void);

/* Extras */
void print_name(void);
void print_image(void);
void print_welcome_msg(void);