#include <stdint.h>
#include "keypad.h"
#include "ssd1306.h"
#include "asciifont.h"
#include "ui_menu.h"
#include "ui_utils.h"
#include "log.h"

#define LINE_H 8
#define LIST_PAGE_SIZE (UI_H / LINE_H)
#define FGC SSD1306_COLOR_SET
#define BGC SSD1306_COLOR_CLEAR
#define UI_LIST_OFFSET_Y(p) (UI_Y + (p * LINE_H))

#define U8_RESET 255

static uint8_t get_item_count(const StrItem *items) {
    uint8_t len = 0;
    while (items[len].len > 0) {
        len ++;
    }
    return len;
}

uint8_t ui_list_select(const StrItem title, const StrItem *items, uint8_t init_index) {
    uint8_t items_count = get_item_count(items);
    uint8_t pointer = init_index % items_count;
    uint8_t tmp_pointer = U8_RESET;
    uint8_t last_pointer = U8_RESET;
    uint8_t page_start = (pointer / LIST_PAGE_SIZE) * LIST_PAGE_SIZE;
    uiu_title(title.text, title.len);
    while (1) {
        uint8_t event = kp_query();
        uint8_t et = kp_Type(event);
        uint8_t ev = kp_Value(event);
        // process event
        if (et == kp_SHORT_CLICK) {
            if (ev == kp_KA) {
                discare_kp_events();
                return pointer;
            } else if (ev == kp_KUP || ev == kp_KDOWN) {
                if (ev == kp_KDOWN) {
                    pointer += 1;
                } else {
                    pointer += (0x7F / items_count) * items_count;
                    pointer -= 1;
                }
                pointer %= items_count;
                if (((pointer / LIST_PAGE_SIZE) * LIST_PAGE_SIZE) != page_start) {
                    last_pointer = U8_RESET;
                    page_start = ((pointer / LIST_PAGE_SIZE) * LIST_PAGE_SIZE);
                }
                tmp_pointer = U8_RESET;
            }
        } else if (et != kp_NOP) {
            log("Unhandled event: ");
            log_unum(et);
            log(" ");
            log_unum(ev);
            log_ln();
        }
        // render items
        if (tmp_pointer == U8_RESET) {
            // need render
            if (last_pointer == U8_RESET) {
                // render all
                for (tmp_pointer = 0; tmp_pointer < LIST_PAGE_SIZE; tmp_pointer ++) {
                    uint16_t current = page_start + tmp_pointer;
                    if (current >= items_count) {
                        uiu_text_area(font_quan_8x8, u8str(" "), 1,
                            UI_X, UI_LIST_OFFSET_Y(tmp_pointer),
                            UI_W, LINE_H,
                            uiu_ALIGN_HCENTER | uiu_ALIGN_VCENTER, FGC, BGC
                        );
                        continue;
                    }
                    StrItem item = items[current];
                    if (current == pointer) {
                        uiu_text_area(font_quan_8x8, item.text, item.len,
                            UI_X, UI_LIST_OFFSET_Y(tmp_pointer),
                            UI_W, LINE_H,
                            uiu_ALIGN_HCENTER | uiu_ALIGN_VCENTER, BGC, FGC
                        );
                        last_pointer = tmp_pointer;
                    } else {
                        uiu_text_area(font_quan_8x8, item.text, item.len,
                            UI_X, UI_LIST_OFFSET_Y(tmp_pointer),
                            UI_W, LINE_H,
                            uiu_ALIGN_HCENTER | uiu_ALIGN_VCENTER, FGC, BGC
                        );
                    }
                }
            } else {
                StrItem item = items[page_start + last_pointer];
                uiu_text_area(font_quan_8x8, item.text, item.len,
                    UI_X, UI_LIST_OFFSET_Y(last_pointer),
                    UI_W, LINE_H,
                    uiu_ALIGN_HCENTER | uiu_ALIGN_VCENTER, FGC, BGC
                );
                last_pointer = pointer - page_start;
                StrItem item_curr = items[pointer];
                uiu_text_area(font_quan_8x8, item_curr.text, item_curr.len,
                    UI_X, UI_LIST_OFFSET_Y(last_pointer),
                    UI_W, LINE_H,
                    uiu_ALIGN_HCENTER | uiu_ALIGN_VCENTER, BGC, FGC
                );
            }
            tmp_pointer = 0;
        }
    }
}
