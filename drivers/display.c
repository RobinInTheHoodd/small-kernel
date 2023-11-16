#include "display.h"
#include "ports.h"
#include "../kernel/util.h"
#include <string.h> 
#include <stdint.h>

#define NUM_COLORS 16

uint8_t COLOR_DISPLAY = 0x0f;

typedef struct {
    const char* name;
    uint8_t value;
} ColorMapping;

static const ColorMapping color_mappings[NUM_COLORS] = {
    {"BLACK", 0x0},
    {"BLUE", 0x1},
    {"GREEN", 0x2},
    {"CYAN", 0x3},
    {"RED", 0x4},
    {"MAGENTA", 0x5},
    {"BROWN", 0x6},
    {"LIGHT-GRAY", 0x7},
    {"DARK-GRAY", 0x8},
    {"LIGHT-BLUE", 0x9},
    {"LIGHT-GREEN", 0xA},
    {"LIGHT-CYAN", 0xB},
    {"LIGHT-RED", 0xC},
    {"LIGHT-MAGENTA", 0xD},
    {"LIGHT-BROWN", 0xE},
    {"WHITE", 0xF}
};

void set_cursor(int offset) {
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset & 0xff));
}

int get_cursor() {
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8; 
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2;
}

int get_offset(int col, int row) {
    return 2 * (row * MAX_COLS + col);
}

int get_row_from_offset(int offset) {
    return offset / (2 * MAX_COLS);
}

int move_offset_to_new_line(int offset) {
    return get_offset(0, get_row_from_offset(offset) + 1);
}

void set_char_at_video_memory(char character, int offset) {
    uint8_t *vidmem = (uint8_t *) VIDEO_ADDRESS;
    vidmem[offset] = character;
    vidmem[offset + 1] = COLOR_DISPLAY;
}

int scroll_ln(int offset) {
    memory_copy(
            (uint8_t * )(get_offset(0, 1) + VIDEO_ADDRESS),
            (uint8_t * )(get_offset(0, 0) + VIDEO_ADDRESS),
            MAX_COLS * (MAX_ROWS - 1) * 2
    );

    for (int col = 0; col < MAX_COLS; col++) {
        set_char_at_video_memory(' ', get_offset(col, MAX_ROWS - 1));
    }

    return offset - 2 * MAX_COLS;
}


void print_string(char *string) {
    int offset = get_cursor();
    int i = 0;
    while (string[i] != 0) {
        if (offset >= MAX_ROWS * MAX_COLS * 2) {
            offset = scroll_ln(offset);
        }
        if (string[i] == '\n') {
            offset = move_offset_to_new_line(offset);
        } else {
            set_char_at_video_memory(string[i], offset);
            offset += 2;
        }
        i++;
    }
    set_cursor(offset);
}

void print_nl() {
    int newOffset = move_offset_to_new_line(get_cursor());
    if (newOffset >= MAX_ROWS * MAX_COLS * 2) {
        newOffset = scroll_ln(newOffset);
    }
    set_cursor(newOffset);
}

void clear_screen() {
    int screen_size = MAX_COLS * MAX_ROWS;
    for (int i = 0; i < screen_size; ++i) {
        set_char_at_video_memory(' ', i * 2);
    }
    set_cursor(get_offset(0, 0));
}

void print_backspace() {
    set_cursor(get_cursor() - 2);
    print_string(" ");
    set_cursor(get_cursor() - 2);
}


void change_text_color_from_name(const char* color_name) {
    uint8_t color = get_color_value_from_name(color_name);
    COLOR_DISPLAY = (COLOR_DISPLAY & 0xF0) | (color & 0x0F);
    
}

void change_bg_color_from_name(const char* color_name) {
    uint8_t color = get_color_value_from_name(color_name);
    COLOR_DISPLAY = (color << 4) | (COLOR_DISPLAY & 0x0F); 
    
}

uint8_t get_color_value_from_name(const char* color_name) {
    for (size_t i = 0; i < NUM_COLORS; i++) {
        if (strcmp(color_name, color_mappings[i].name) == 0) {
            return color_mappings[i].value;
        }
    }
    return 0x0F;
}

void set_cursor_color() {
    int cursor_offset = get_cursor();
    uint8_t *vidmem = (uint8_t *) VIDEO_ADDRESS;
    vidmem[cursor_offset + 1] = COLOR_DISPLAY;
}

void update_screen_colors() {
    uint8_t *vidmem = (uint8_t *) VIDEO_ADDRESS;
    int screen_size = MAX_COLS * MAX_ROWS;
    for (int i = 0; i < screen_size; ++i) {
        vidmem[i * 2 + 1] = COLOR_DISPLAY;
    }
}

void change_colors(const char* text_color_name, const char* bg_color_name) {
    change_text_color_from_name(text_color_name);
    change_bg_color_from_name(bg_color_name);
    update_screen_colors();
}

