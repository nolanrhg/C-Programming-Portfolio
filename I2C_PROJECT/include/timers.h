/**********************************************************************************\
 * @file    I2C_PROJECT/include/timers.h                                          *
 * @author  Nolan R. H. Gagnon                                                    *
 * @version V1.0                                                                  *
 * @date    26-June-2017                                                          *
 * @brief   Interface for various on-chip timers.                                 *
 *                                                                                *
 **********************************************************************************
 * @attention                                                                     *
 *                                                                                *
 * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. H. Gagnon </center></h2>         *
 *                                                                                *
\**********************************************************************************/
#ifndef TIMERS_H
#define TIMERS_H

/***********************************************************************************\
 *                                                                                 *
 *                              TIMER CONSTANTS                                    *
 *                                                                                 *
\***********************************************************************************/
#define PE11_AF_TIM1CH2			((uint32_t) 1 << (4 * 3))

/***********************************************************************************\
 *                                                                                 *
 *                         TIMER FUNCTION PROTOTYPES                               *
 *                                                                                 *
\***********************************************************************************/

/**
  * @brief Configures TIM1_CH2 in PWM mode 2.
  * @param None
  * @retval None
  */
void tim1_ch2_init(void);


/**
  * @brief Configures PE11 for alternate function 1.
  * @param None
  * @retval None
  */
void tim1_ch2_pin_init(void);

#endif
