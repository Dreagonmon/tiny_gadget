#include "log.h"
#include "debug.h"

const static char HEX_CHAR[16] = {
    '0', '1', '2', '3',
    '4', '5', '6', '7',
    '8', '9', 'A', 'B',
    'C', 'D', 'E', 'F',
};

void _log(const char *text) {
    char ch = *text;
    while (ch != '\0') {
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        USART_SendData(USART1, ch);
        ch = *(++text);
    }
}

void log_ln() {
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    USART_SendData(USART1, '\n');
}

void log_unum(uint64_t num) {
    uint8_t offset = 64 - 4;
    uint8_t following = 0;
    _log((const char *)("0x"));
    while (1)
    {
        uint8_t val = (num >> offset) & 0b1111;
        if (following || val > 0) {
            following = 1;
            while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
            USART_SendData(USART1, HEX_CHAR[val]);
        }
        if (offset == 0 && val == 0 && !following) {
            while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
            USART_SendData(USART1, HEX_CHAR[0]);
        }
        if (offset == 0) break;
        offset -= 4;
    }
    
}

void log_num(int64_t num) {
    if (num < 0) {
        _log((const char *)("-"));
        log_unum(0 - num);
    } else {
        log_unum(num);
    }
}
