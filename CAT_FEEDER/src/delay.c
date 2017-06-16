/**
  **********************************************************************************
  * @file    RGB_SENSOR/src/delay.c
  * @author  Nrgagnon 
  * @version V1.6
  * @date    29-May-2017
  * @brief   Library to provide accurate delays using SysTick.
  *
  **********************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. H. Gagnon </center></h2>
  *
  **********************************************************************************
  */

/* Includes ----------------------------------------------------------------------*/
#include "../include/delay.h"

/* Private volatile variables ----------------------------------------------------*/
static volatile uint32_t timing_delay;

/**
  * @brief Delays the system for a number of milliseconds.
  * @param time_ms : Number of milliseconds to delay for.
  * @retval None
  */
void delay(uint32_t time_ms)
{
        timing_delay = time_ms;
        while (timing_delay != 0);
}

/**
  * @brief Initialize SysTick for providing delays.
  * @param ticks : Number of clock pulses needed for 
  *		   the counter to reach zero.
  * @retval None
  */
void systick_init(uint32_t ticks)
{
        // Disable SysTick IRQ and SysTick Counter
        SysTick->CTRL = 0;

        // Set reload register
        SysTick->LOAD = ticks - 1;

        // Set priority
        NVIC_SetPriority(SysTick_IRQn, 2);

        // Reset the SysTick counter value
        SysTick->VAL = 0;

        // Select processor clock
        // 1 = processor clock; 0 = external clock
         SysTick->CTRL |= SysTick_CTRL_CLKSOURCE;

        // Enable SysTick IRQ and SysTick timer
        SysTick->CTRL |= SysTick_CTRL_ENABLE;

        //Enable SysTick exception request
        // 1 = counting down to zero asserts the SysTick exception request
        // 0 = counting down to zero does not assert the SysTick exception request
        SysTick->CTRL |= SysTick_CTRL_TICKINT;
}

/**
  * @brief Decreases the delay counter every time the SysTick counter
  *	   reaches zero.
  * @param None
  * @retval None
  */
void SysTick_Handler(void)
{
        if (timing_delay > 0)
                timing_delay--;
}
