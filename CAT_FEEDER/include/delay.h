#ifndef DELAY_H
#define DELAY_H

#define SysTick_CTRL_ENABLE             ((uint32_t)0x00000001)
#define SysTick_CTRL_TICKINT            ((uint32_t)0x00000002)
#define SysTick_CTRL_CLKSOURCE          ((uint32_t)0x00000004)
#define SysTick_CTRL_COUNTFLAG          ((uint32_t)0x00010000)

void systick_init(uint32_t ticks);

void delay(uint32_t time_ms);

#endif
