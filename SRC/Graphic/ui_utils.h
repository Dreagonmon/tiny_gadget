#pragma once
#include <stdint.h>
#include <stdlib.h>
#include "bmfont.h"
#include "ssd1306.h"
#include "u8str.h"

#define uiu_ALIGN_HLEFT                0b00000001
#define uiu_ALIGN_HCENTER              0b00000010
#define uiu_ALIGN_HRIGHT               0b00000100
#define uiu_ALIGN_VTOP                 0b00010000
#define uiu_ALIGN_VCENTER              0b00100000
#define uiu_ALIGN_VBOTTOM              0b01000000

#define UI_X 0
#define UI_Y 16
#define UI_W (SSD1306_WIDTH)
#define UI_H (SSD1306_HEIGHT - UI_Y)

void uiu_text_area(bmf_BitmapFont *font, U8String text, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t align, uint8_t color, uint8_t bg_color);
void uiu_title(U8String text);
