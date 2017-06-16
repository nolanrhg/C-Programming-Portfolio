#include "../include/servo.h"

void rotate_servo_right(void)
{
	TIM2->CCR2 = 40;
}

void rotate_servo_left(void)
{
	TIM2->CCR2 = 80;
}

void stop_servo(void)
{
	TIM2->CCR2 = 0;
}
