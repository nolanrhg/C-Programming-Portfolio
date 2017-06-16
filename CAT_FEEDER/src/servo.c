#include <stm32l476xx.h>
#include "../include/servo.h"
#include "../include/delay.h"

void servo_push(void)
{
	TIM5->CCR2 = 20;
	TIM2->CCR2 = 100;
}

void rotate_servo1(uint16_t ccr)
{
	TIM2->CCR2 = ccr;
}

void rotate_servo2(uint16_t ccr)
{
	TIM5->CCR2 = ccr;
}

void home_servo1(void)
{
	TIM2->CCR2 = 60;
}

void home_servo2(void)
{
	TIM5->CCR2 = 60;
}

void servo1_signal_off(void)
{
	TIM2->CR1 &= ~TIM_CR1_CEN;	
}

void servo2_signal_off(void)
{
	TIM5->CR1 &= ~TIM_CR1_CEN;
}

void servo1_signal_on(void)
{
	TIM2->CR1 |= TIM_CR1_CEN;
}

void servo2_signal_on(void)
{
	TIM5->CR1 |= TIM_CR1_CEN;
}
