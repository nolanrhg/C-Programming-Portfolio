/**
  **********************************************************************************
  * @file    VOICE_CHANGER/include/clocks.h                                       *
  * @author  Nolan R. Gagnon                                                       *
  * @version V1.0                                                                  *
  * @date    15-June-2017                                                          *
  * @brief   System Clock Control Interface.                                       *
  *                                                                                *
  **********************************************************************************
  * @attention                                                                     *
  *                                                                                *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. Gagnon  </center></h2>           *
  *                                                                                *
  **********************************************************************************
  */
#ifndef CLOCKS_H
#define CLOCKS_H

/***********************************************************************************
 *                                                                                 *
 *                              CLOCK CONTROL CONSTANTS                            *
 *                                                                                 *
 ***********************************************************************************/

#define SysTick_CTRL_ENABLE		((uint32_t) 0x00000001)
#define SysTick_CTRL_TICKINT		((uint32_t) 0x00000002)
#define SysTick_CTRL_CLKSOURCE		((uint32_t) 0x00000004)
#define SysTick_CTRL_COUNTFLAG	        ((uint32_t) 0x00010000)

volatile uint32_t TimingDelay;

/***********************************************************************************
 *                                                                                 *
 *                              CLOCK CONTROL FUNCTIONS                            *
 *                                                                                 *
 ***********************************************************************************/

/* Configure the HSI 16 MHz clock */
void hsi16_init(void);

/* Configures mid-speed internal clock
   0 <= range <= 11 */
void msi_init(unsigned char range);

/* Configures PLL and sets it as the SYSCLK */
void sysclk_init_pll(int in_mul, int in_div, int out_div);

/* Configures the PLLSAI1CLK, which can be used for SAI1 or SAI2 */
void pllsai1_init(int in_mul, int out_div);

/* Tie up proc. for number of ms */
void delay(uint32_t time_ms);

/* Initialize SysTick */
void systick_init(uint32_t ticks);

#endif
