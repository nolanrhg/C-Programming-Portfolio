#include "../include/timers.h"

void tim1_ch2_init(void)
{
        float dc = 1.0 / 2.0; // duty cycle

        // Enable TIM1 clock
        RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

        // Set prescaler to zero so system clock is not divided 
        TIM1->PSC = 1599;

        // Configure counter to count to 5000
        TIM1->ARR = 1000;

        // Configure for PWM mode 2
        // Output high as long as TIM2_CNT > CCR1
        TIM1->CCMR1 &= ~TIM_CCMR1_OC2M;
        TIM1->CCMR1 |= TIM_CCMR1_OC2M_0 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;

        // Duty cycle will be dc: (1 - CCR1 / ARR) * 100% for PWM Mode 2
        TIM1->CCR2 = 500;

        // Enable capture/compare 1
        TIM1->CCER |= TIM_CCER_CC2E;

	TIM1->BDTR |= TIM_BDTR_MOE;

        // Enable the counter
        TIM1->CR1 |= TIM_CR1_CEN;
}

void tim1_ch2_pin_init(void)
{
	/* Clock I/O port E */
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;

	/* Configure PE11 for alternate function 1 (TIM1_CH2) */
        GPIOE->MODER &= ~GPIO_MODER_MODER11;
        GPIOE->MODER |= GPIO_MODER_MODER11_1;
        GPIOE->AFR[1] &= ~GPIO_AFRH_AFRH3;
        GPIOE->AFR[1] |= PE11_AF_TIM1CH2;
}
