#include <stdint.h>
#include "keypad.h"
#include "ch32v00x.h"
#include "sysclock.h"

static uint8_t key_status = 0;
static const GPIO_TypeDef *ports[] = {
    GPIOD,
    GPIOC,
    GPIOC,
    GPIOC,
    GPIOD
};
static const uint16_t pins[] = {
    GPIO_Pin_4,
    GPIO_Pin_3,
    GPIO_Pin_0,
    GPIO_Pin_4,
    GPIO_Pin_3
};
static const uint8_t keys[] = {
    kp_KUP,
    kp_KDOWN,
    kp_KLEFT,
    kp_KRIGHT,
    kp_KA
};
static int32_t pressed_at[] = {
    -1,
    -1,
    -1,
    -1,
    -1
};

void kp_init() {
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    uint8_t i;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    for (i = 0; i < sizeof(keys); i++) {
        GPIO_InitStructure.GPIO_Pin = pins[i];
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init((GPIO_TypeDef *)ports[i], &GPIO_InitStructure);
    }
}

#define K_GET(x) (((0b1 << x) & key_status) >> x)
#define K_SET0(x) (key_status = (~(0b1 << x)) & key_status)
#define K_SET1(x) (key_status = (0b1 << x) | key_status)
#define K_HGET(x) (GPIO_ReadInputDataBit((GPIO_TypeDef *)ports[x], pins[x]) == Bit_RESET)
#define K_EVENT(event, value) ((value << 4) | event)

uint8_t kp_query() {
    // Scan keypad
    int32_t now = ticks_ms();
    uint8_t state, mstate, key_n;
    for (key_n = 0; key_n < sizeof(keys); key_n ++) {
        state = K_HGET(key_n);
        mstate = K_GET(key_n);
        if (state && (!mstate)) {
            K_SET1(key_n);
            pressed_at[key_n] = now;
            return K_EVENT(kp_KEY_DOWN, keys[key_n]);
        } else if ((!state) && mstate) {
            if (pressed_at[key_n] >= 0) {
                pressed_at[key_n] = -1;
                if (ticks_diff(now, pressed_at[key_n]) > kp_CONFIRM_CLICK_TIMEOUT_MS) {
                    return K_EVENT(kp_SHORT_CLICK, keys[key_n]);
                }
            }
            K_SET0(key_n);
            return K_EVENT(kp_KEY_UP, keys[key_n]);
        }
        // ticks pressed time
        if (pressed_at[key_n] >= 0 && state) {
            if (ticks_diff(now, pressed_at[key_n]) > kp_LONG_PRESS_TIMEOUT_MS) {
                pressed_at[key_n] = -1;
                return K_EVENT(kp_LONG_PRESS, keys[key_n]);
            }
        }
    }
    return K_EVENT(kp_NOP, 0);
}

void discare_kp_events() {
    uint8_t event = kp_query();
    while (kp_Type(event) != kp_NOP) {
        event = kp_query();
    }
}
