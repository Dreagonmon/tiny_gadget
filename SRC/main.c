#include "debug.h"
#include "log.h"
#include "sysclock.h"
#include "keypad.h"
#include "ssd1306.h"
#include "ui_menu.h"


static const StrItem items_test[] = {
    stritem("Hello Dragon~"),
    stritem("World Dragon!"),
    NULL_STR_ITEM,
};

void init() {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    USART_Printf_Init(115200);
    systick_init();
    kp_init();
    SSD1306_init();
    SSD1306_fill_screen(SSD1306_COLOR_CLEAR);
    SSD1306_display_on_off(1);
    log("==== Hello ==== \n");
    ui_list_select(stritem("Dragon"), items_test, 0);
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    init();
    while (1) {
        uint8_t event = kp_query();
        uint8_t event_type = kp_Type(event);
        uint8_t event_value = kp_Value(event);
        if (event_type != kp_NOP && event_type != kp_KEY_DOWN && event_type != kp_KEY_UP) {
            log("KeyEvent: ");
            log_unum(event_type);
            log(" ");
            log_unum(event_value);
            log_ln();
        }
    };
}
