#include <stdint.h>
#include "ui_utils.h"
#include "ssd1306.h"
#include "bmfont.h"
#include "asciifont.h"

#define MAX_LINES 8

void uiu_text_area(bmf_BitmapFont *font, const uint8_t *text, uint32_t len, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t align, uint8_t color, uint8_t bg_color) {
    // calc line width
    uint16_t lws[MAX_LINES];
    uint16_t gws[MAX_LINES];
    uint16_t max_width = 0;
    uint8_t lines = 0;
    uint16_t p_off = 0;
    while ((p_off < len) && ((lines + 1) * font->char_height <= h) && (lines < MAX_LINES)) {
        uint16_t fitlen = bmf_get_text_offset(font, text + p_off, len - p_off, w, font->char_height);
        if (fitlen == 0) {
            break;
        }
        lws[lines] = fitlen;
        gws[lines] = bmf_get_text_width(font, text + p_off, fitlen);
        max_width = (gws[lines] > max_width) ? gws[lines] : max_width;
        lines ++;
        p_off += fitlen;
    }
    // draw lines
    uint8_t off_x = 0;
    uint8_t off_y = 0;
    if (align & uiu_ALIGN_VBOTTOM) {
        off_y = (h - (lines * font->char_height));
    }else if (align & uiu_ALIGN_VCENTER) {
        off_y = ((h - (lines * font->char_height)) / 2);
    }else {
        off_y = 0;
    }
    if (!SSD1306_alloc_area(x, y, w, h)) {
        return; // alloc failed, area too large
    }
    SSD1306_fill_rect(0, 0, w, h, bg_color);
    uint8_t cur_lines;
    p_off = 0;
    for (cur_lines = 0; cur_lines < lines; cur_lines ++) {
        if (align & uiu_ALIGN_HRIGHT) {
            off_x = (w - gws[cur_lines]);
        } else if (align & uiu_ALIGN_HCENTER) {
            off_x = ((w - gws[cur_lines]) / 2);
        } else {
            off_x = 0;
        }
        bmf_draw_text(font, text + p_off, lws[cur_lines], off_x, off_y, gws[cur_lines], font->char_height, color);
        off_y += font->char_height;
        p_off += lws[cur_lines];
    }
    SSD1306_write_area();
}

void uiu_title(const uint8_t *text, uint8_t len) {
    SSD1306_alloc_area(0, 0, SSD1306_WIDTH, UI_Y);
    uint8_t offset_y = (UI_Y - font_quan_8x8->char_height) / 2;
    uint16_t text_w = bmf_get_text_width(font_quan_8x8, text, len);
    uint8_t offset_x = (SSD1306_WIDTH - text_w) / 2;
    bmf_draw_text(font_quan_8x8, text, len, offset_x, offset_y, SSD1306_WIDTH, font_quan_8x8->char_height, SSD1306_COLOR_SET);
    SSD1306_write_area();
}
