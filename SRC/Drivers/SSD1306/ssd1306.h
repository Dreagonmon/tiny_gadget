#pragma once

#ifndef __SSD1306_H__
#define __SSD1306_H__

#include <stdint.h>

/* Screen */
#define SSD1306_WIDTH    128
#define SSD1306_HEIGHT    64

/* Commands */
#define SSD1306_SET_MUX_RATIO     0xA8
#define SSD1306_DISPLAY_OFFSET    0xD3
#define SSD1306_DISPLAY_ON        0xAF
#define SSD1306_DISPLAY_OFF       0xAE
#define SSD1306_DIS_ENT_DISP_ON   0xA4
#define SSD1306_DIS_IGNORE_RAM    0xA5
#define SSD1306_DIS_NORMAL        0xA6
#define SSD1306_DIS_INVERSE       0xA7
#define SSD1306_DEACT_SCROLL      0x2E
#define SSD1306_ACTIVE_SCROLL     0x2F
#define SSD1306_SET_START_LINE    0x40
#define SSD1306_MEMORY_ADDR_MODE  0x20
#define SSD1306_SET_COLUMN_ADDR   0x21
#define SSD1306_SET_PAGE_ADDR     0x22
#define SSD1306_SEG_REMAP         0xA0
#define SSD1306_SEG_REMAP_OP      0xA1
#define SSD1306_COM_SCAN_DIR      0xC0
#define SSD1306_COM_SCAN_DIR_OP   0xC8
#define SSD1306_COM_PIN_CONF      0xDA
#define SSD1306_SET_CONTRAST      0x81
#define SSD1306_SET_OSC_FREQ      0xD5
#define SSD1306_SET_CHAR_REG      0x8D
#define SSD1306_SET_PRECHARGE     0xD9
#define SSD1306_VCOM_DESELECT     0xDB

/* Colors */
#define SSD1306_COLOR_SET 1
#define SSD1306_COLOR_CLEAR 0

/* Macros */
#define SSD1306_CALC_PAGE(h) ((h / 8) + (h % 8 > 0))
#define SSD1306_CALC_BUF_SIZE(w, h) (w * SSD1306_CALC_PAGE(h))

void SSD1306_init(void);
void SSD1306_invert(uint8_t invert);
void SSD1306_set_contrast(uint8_t value);
void SSD1306_display_on_off(uint8_t on_off);
void SSD1306_set_data_window(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void SSD1306_fill_screen(uint8_t color);
uint8_t SSD1306_alloc_area(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void SSD1306_write_area();
void SSD1306_write_image_data(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t *img);
void SSD1306_pixel(uint8_t x, uint8_t y, uint8_t c);
void SSD1306_fill_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t c);

#endif
