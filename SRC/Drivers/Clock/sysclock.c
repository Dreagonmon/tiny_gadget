#include "sysclock.h"
#include "ch32v00x.h"

volatile static uint32_t sec = 0;
volatile static uint32_t msec = 0;
volatile static uint32_t usec = 0;

void systick_init() {
    NVIC_EnableIRQ(SysTicK_IRQn);
    SysTick->SR = 0;
    SysTick->CMP = SystemCoreClock - 1;
    SysTick->CNT = 0;
    /* bit3: 1: 向上计数，到达比较值后重装0
       bit2: 1: 时钟HCLK 0: HCLK/8
       bit1: 1: 使能计数器中断
       bit0: 1: 启动系统计数器 */
    SysTick->CTLR = 0b00001111u;
}

uint32_t us_since_last_s() {
    uint32_t cnt = SysTick->CNT;
    return (uint32_t)(((uint64_t)cnt * (uint64_t)1000000) / (uint64_t)SystemCoreClock);
}

void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void SysTick_Handler(void)
{
    SysTick->SR = 0;
    SysTick->CNT = 0;
    sec  += 1;
    msec += 1000;
    usec += 1000000;
}

uint32_t time() {
    return sec;
}

int32_t ticks_s() {
    return sec & INT32_MAX;
}

int32_t ticks_ms() {
    uint32_t msec_s = msec;
    uint32_t us_offset = us_since_last_s();
    if (msec != msec_s) return ticks_ms(); // msec changed, try again.
    return (msec_s + (us_offset / 1000)) & INT32_MAX;
}

int32_t ticks_us() {
    uint32_t usec_s = usec;
    uint32_t us_offset = us_since_last_s();
    if (usec != usec_s) return ticks_ms(); // usec changed, try again.
    return (usec_s + us_offset) & INT32_MAX;
}

int32_t ticks_add(int32_t t1, int32_t delta) {
    return (t1 + delta) & INT32_MAX;
}

int32_t ticks_diff(int32_t t1, int32_t t2) {
    int32_t half = (INT32_MAX / 2) + 1;
    return ((t1 - t2 + half) & INT32_MAX) - half;
}
