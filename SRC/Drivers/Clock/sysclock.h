#pragma once

#include <stdint.h>

void systick_init(void);
uint32_t time();
int32_t ticks_s();
int32_t ticks_ms();
int32_t ticks_us();
int32_t ticks_add(int32_t t1, int32_t delta);
int32_t ticks_diff(int32_t t1, int32_t t2);
