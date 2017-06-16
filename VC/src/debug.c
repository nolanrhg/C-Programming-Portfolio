/*!
 *
 * @file
 *
 * @brief Implementation of LED debugging functions
 *
 * @author Nrgagnon
 *
 * @date April 15, 2017
 *
 */

#include "../include/debug.h"

void led_init(void)
{
	/*
	 * Configure PB2 for digital output
         */

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	GPIOB->MODER &= ~GPIO_MODER_MODER2;
	GPIOB->MODER |= GPIO_MODER_MODER2_0;
}

void led_debug(unsigned char on)
{
	if (on == 1)
		GPIOB->ODR |= GPIO_ODR_ODR_2;
	else 
		GPIOB->ODR &= ~GPIO_ODR_ODR_2;
}
