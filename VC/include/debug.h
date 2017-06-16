/**
  **********************************************************************************
  * @file    VOICE_CHANGER/include/debug.h                                         *
  * @author  Nolan R. Gagnon                                                       *
  * @version V1.0                                                                  *
  * @date    15-June-2017                                                          *
  * @brief   LED Debug Interface.	                                           *
  *                                                                                *
  **********************************************************************************
  * @attention                                                                     *
  *                                                                                *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. Gagnon  </center></h2>           *
  *                                                                                *
  **********************************************************************************
  */
#ifndef DEBUG_H
#define DEBUG_H

/***********************************************************************************
 *                                                                                 *
 *                              LED DEBUG FUNCTIONS                                *
 *                                                                                 *
 ***********************************************************************************/

/* Configures PB2 as digital output */
void led_init(void);

/* LD4 on if on == 1, else LD4 off */
void led_debug(unsigned char on);

#endif
