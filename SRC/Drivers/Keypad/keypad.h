#pragma once

#define kp_NOP           0x0
#define kp_KEY_DOWN      0x1
#define kp_KEY_UP        0x2
#define kp_SHORT_CLICK   0x3
#define kp_LONG_PRESS    0x4

#define kp_KUP           0x0
#define kp_KDOWN         0x1
#define kp_KLEFT         0x2
#define kp_KRIGHT        0x3
#define kp_KA            0x4

#define kp_Type(x) (x & 0b1111)
#define kp_Value(x) (x >> 4)

#define kp_CONFIRM_CLICK_TIMEOUT_MS 50
#define kp_LONG_PRESS_TIMEOUT_MS 500

// query and return keypad event
void kp_init(void);
uint8_t kp_query();
void discare_kp_events(void);
