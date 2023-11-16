#pragma once
#include <stdint.h>
#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80


/* Screen i/o ports */
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5


/* Public kernel API */
extern uint8_t COLOR_DISPLAY;
void print_string(char* string);
void print_nl();
void clear_screen();
int scroll_ln(int offset);
void print_backspace();
void change_text_color_from_name(const char* color_name);
void change_bg_color_from_name(const char* color_name);
uint8_t get_color_value_from_name(const char* color_name);
void change_colors(const char* text_color_name, const char* bg_color_name);
void set_cursor_color();
void update_screen_colors();