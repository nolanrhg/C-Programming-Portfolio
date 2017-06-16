/**
  **********************************************************************************
  * @file    VOICE_CHANGER/include/dfsdm.h                                         *
  * @author  Nolan R. Gagnon                                                       *
  * @version V1.0                                                                  *
  * @date    15-June-2017                                                          *
  * @brief   DFSDM Interface.		                                           *
  *                                                                                *
  **********************************************************************************
  * @attention                                                                     *
  *                                                                                *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. Gagnon  </center></h2>           *
  *                                                                                *
  **********************************************************************************
  */
#ifndef DFSDM_H
#define DFSDM_H

/***********************************************************************************
 *                                                                                 *
 *                              DFSDM Functions                                    *
 *                                                                                 *
 ***********************************************************************************/

/* Configures the digital filter for sigma-delta modulators
   for audio recording (MEMS microphone is the sigma-delta modulator) */
void dfsdm_init(void);

/* Configures GPIO pins needed for DFSDM clock output and 
   data input */
static void dfsdm_pins_init(void);

#endif
