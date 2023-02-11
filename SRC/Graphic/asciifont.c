#include <stdint.h>
#include "asciifont.h"
#include "bmfont.h"
#include "Fonts/quan8x8.h"

#define ASCII_CHAR_START 0x21
#define ASCII_CHAR_END 0x7E

uint8_t af_get_char_data(bmf_BitmapFont *font, uint32_t unicode, uint8_t *buffer) {
    if (unicode < ASCII_CHAR_START || unicode > ASCII_CHAR_END) {
        return 0;
    }
    uint8_t ch_w = font->char_width;
    uint8_t ch_h = font->char_height;
    uint8_t wp = ch_w / 8;
    wp += (ch_w % 8) ? 1 : 0;
    uint16_t buffer_size = wp * ch_h;
    uint32_t start, curr;
    start = (unicode - ASCII_CHAR_START) * buffer_size;
    for (curr = 0; curr < buffer_size; curr ++) {
        buffer[curr] = font->font_date[start + curr];
    }
    return 1;
}

bmf_BitmapFont font_quan_8x8_obj = {
    af_get_char_data,
    FONT_QUAN_8_8,
    8,
    8,
    FONT_QUAN_ASCII_WIDTH_8_8
};

bmf_BitmapFont *font_quan_8x8 = &font_quan_8x8_obj;
