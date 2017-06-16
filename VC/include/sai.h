/**
  **********************************************************************************
  * @file    VOICE_CHANGER/include/sai.h                                           *
  * @author  Nolan R. Gagnon                                                       *
  * @version V1.0                                                                  *
  * @date    15-June-2017                                                          *
  * @brief   SAI Interface.                                                        *
  *                                                                                *
  **********************************************************************************
  * @attention                                                                     *
  *                                                                                *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. Gagnon </center></h2>            *
  *                                                                                *
  **********************************************************************************
  */
#ifndef SAI_H
#define SAI_H

/***********************************************************************************
 *                                                                                 *
 *                              SAI FUNCTIONS                                      *
 *                                                                                 *
 ***********************************************************************************/

/* Configures the Serial Audio Interface (SAI1) for controlling the CS43L22 audio codec */
void sai1_init(void);

/* Configures GPIO pins needed for the SAI communication protocol */
static void sai1_pins_init(void);

#endif
