#pragma once

#include <stdint.h>

#define u8str(x) ((uint8_t *)x)

typedef struct bmf_BitmapFont bmf_BitmapFont;
typedef uint8_t (*bmf_FunctionGetCharImage)(bmf_BitmapFont *font, uint32_t unicode, uint8_t *buffer); // MONO_HMSB format
typedef struct bmf_BitmapFont {
    bmf_FunctionGetCharImage get_char_image;
    const uint8_t *font_date;
    const uint8_t char_width;
    const uint8_t char_height;
    const uint8_t *ascii_width; // size [94]
} bmf_BitmapFont;

uint16_t bmf_get_text_width(bmf_BitmapFont *font, const uint8_t *text, uint32_t bytes_len);
uint32_t bmf_draw_text(bmf_BitmapFont *font, const uint8_t *text, uint32_t bytes_len, uint16_t x, uint16_t y, uint16_t width_limit, uint16_t height_limit, uint16_t color);
uint32_t bmf_get_text_offset(bmf_BitmapFont *font, const uint8_t *text, uint32_t bytes_len, uint16_t width_limit, uint16_t height_limit);
