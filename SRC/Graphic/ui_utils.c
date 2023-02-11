#include <stdint.h>
#include "ui_utils.h"
#include "ssd1306.h"
#include "bmfont.h"
#include "asciifont.h"

#define MAX_LINES 8
#define TITLE_COLOR_BG SSD1306_COLOR_CLEAR

void uiu_title(const uint8_t *text, uint8_t len) {
    SSD1306_alloc_area(0, 0, SSD1306_WIDTH, UI_Y);
    uint8_t offset_y = (UI_Y - font_quan_8x8->char_height) / 2;
    uint16_t text_w = bmf_get_text_width(font_quan_8x8, text, len);
    uint8_t offset_x = (SSD1306_WIDTH - text_w) / 2;
    bmf_draw_text(font_quan_8x8, text, len, offset_x, offset_y, SSD1306_WIDTH, font_quan_8x8->char_height, SSD1306_COLOR_SET);
    SSD1306_write_area();
}
