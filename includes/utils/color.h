#pragma once

/* ------------- ANSI TEXT COLOR CODE ------------- */ 

// Normal intensity
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

// High-intensity colors
#define HI_BLACK "\033[90m"
#define HI_RED "\033[91m"
#define HI_GREEN "\033[92m"
#define HI_YELLOW "\033[93m"
#define HI_BLUE "\033[94m"
#define HI_MAGENTA "\033[95m"
#define HI_CYAN "\033[96m"
#define HI_WHITE "\033[97m"

// Common alias for grey/gray (bright black)
#define GREY HI_BLACK
#define GRAY HI_BLACK

// Reset all attributes
#define COLOR_RESET "\033[0m"

void print_color(const char *str, const char *color);
