/**********************************************************************************\
 * @file    I2C_PROJECT/include/delay.h                                           *
 * @author  Nolan R. H. Gagnon                                                    *
 * @version V2.0                                                                  *
 * @date    26-June-2017                                                          *
 * @brief   Provides accurate delays using system timer.                          *
 *                                                                                *
 **********************************************************************************
 * @attention                                                                     *
 *                                                                                *
 * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. H. Gagnon  </center></h2>        *
 *                                                                                *
\**********************************************************************************/
#ifndef DELAY_H
#define DELAY_H

/**********************************************************************************\
 *                                                                                *
 *                               SYSTICK CONSTANTS                                *
 *                                                                                *
\**********************************************************************************/
#define SysTick_CTRL_ENABLE             ((uint32_t) 0x00000001)
#define SysTick_CTRL_TICKINT            ((uint32_t) 0x00000002)
#define SysTick_CTRL_CLKSOURCE          ((uint32_t) 0x00000004)
#define SysTick_CTRL_COUNTFLAG          ((uint32_t) 0x00010000)

/**********************************************************************************\
 *                                                                                *
 *                           DELAY FUNCTION PROTOTYPES                            *
 *                                                                                *
\**********************************************************************************/

/**
  * @brief Initializes the SysTick system timer. 
  * @param ticks : Number of clock pulses required for SysTick counter to reach 0. 
  * @retval None
  */
void systick_init(uint32_t ticks);

/**
  * @brief Turn of the SysTick counter. 
  * @param None
  * @retval None
  */
void systick_deinit(void);

/**
  * @brief Delay the processor for a number of milliseconds.
  * @param time_ms : Number of milliseconds to delay. 
  * @retval None
  */
void delay(uint32_t time_ms);

#endif
