#pragma once
#include <stdint.h>
#include <stdlib.h>
#include "bmfont.h"
#include "ssd1306.h"

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

typedef struct
{
    const uint8_t *text;
    const uint8_t len;
} StrItem;

typedef struct
{
    uint8_t index;
    uint8_t confirmed;
} IndexResult;


/** get string item in the list.
 * @param index item index
 * @return STR_ITEM, if STR_ITEM.len == 0 means item not exist.
 */
typedef StrItem (*uiu_GetStrItemFunc)(uint8_t index);

void uiu_title(const uint8_t *text, uint8_t len);

#define stritem(x) ((StrItem){.text = (const uint8_t *)(x), .len = (uint8_t)(sizeof(x))})
#define NULL_STR_ITEM ((StrItem){.text = NULL, .len = 0})
