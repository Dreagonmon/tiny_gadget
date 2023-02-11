#include <stdint.h>
#include "keypad.h"
#include "ssd1306.h"
#include "asciifont.h"
#include "ui_menu.h"
#include "ui_utils.h"

#define U8_RESET 255

static uint8_t get_item_count(const StrItem *items) {
    uint8_t len = 0;
    while (items[len].len > 0) {
        len ++;
    }
    return len;
}

uint8_t ui_list_select(const StrItem title, const StrItem *items, uint8_t init_index) {
    uiu_title(title.text, title.len);
}
