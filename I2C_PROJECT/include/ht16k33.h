 /**********************************************************************************\
  * @file    I2C_PROJECT/include/ht16k33.h                                         *
  * @author  Nolan R. Gagnon                                                       *
  * @version V1.0                                                                  *
  * @date    23-June-2017                                                          *
  * @brief   HT16K33 Application Interface.                                        *
  *                                                                                *
  **********************************************************************************
  * @attention                                                                     *
  *                                                                                *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. Gagnon  </center></h2>           *
  *                                                                                *
 \**********************************************************************************/
#ifndef HT16K33_H
#define HT16K33_H

/***********************************************************************************\
 *                                                                                 *
 *                              HT16K33 I2C ADDRESS                                *
 *                                                                                 *
\***********************************************************************************/
#define HT16K33_I2C_ADDR			((uint8_t) 0x70)

/***********************************************************************************\
 *                                                                                 *
 *                              HT16K33 REGISTERS                                  *
 *                                                                                 *
\***********************************************************************************/
#define HT16K33_DISPDATR_PTR			((uint8_t) 0x00)
#define HT16K33_KEYDATR_PTR			((uint8_t) 0x40)
#define HT16K33_INTFLGR_PTR			((uint8_t) 0x60)

/***********************************************************************************\
 *                                                                                 *
 *                              HT16K33 COMMANDS                                   *
 *                                                                                 *
\***********************************************************************************/
#define HT16K33_SSETUP				((uint8_t) 0x20)
#define HT16K33_DSETUP				((uint8_t) 0x80)
#define HT16K33_DIMSETUP			((uint8_t) 0xE0)

/***********************************************************************************\
 *                                                                                 *
 *                              HT16K33 COMMAND PARAMS                             *
 *                                                                                 *
\***********************************************************************************/

/* SSETUP **************************************************************************/
#define HT16K33_SSETUP_OSCON			((uint8_t) 0x01)

/* DSETUP **************************************************************************/
#define HT16K33_DSETUP_DISPON			((uint8_t) 0x01)
#define HT16K33_DSETUP_BLKOFF			((uint8_t) 0x00)
#define HT16K33_DSETUP_BLK2HZ			((uint8_t) 0x02)
#define HT16K33_DSETUP_BLK1HZ			((uint8_t) 0x04)
#define HT16K33_DSETUP_BLKHFHZ			((uint8_t) 0x06)

/* DIMSETUP ************************************************************************/
#define HT16K33_DIMSETUP_DUTY1			((uint8_t) 0x00)
#define HT16K33_DIMSETUP_DUTY2			((uint8_t) 0x01)
#define HT16K33_DIMSETUP_DUTY3                  ((uint8_t) 0x02)
#define HT16K33_DIMSETUP_DUTY4                  ((uint8_t) 0x03)
#define HT16K33_DIMSETUP_DUTY5                  ((uint8_t) 0x04)
#define HT16K33_DIMSETUP_DUTY6                  ((uint8_t) 0x05)
#define HT16K33_DIMSETUP_DUTY7                  ((uint8_t) 0x06)
#define HT16K33_DIMSETUP_DUTY8                  ((uint8_t) 0x07)
#define HT16K33_DIMSETUP_DUTY9                  ((uint8_t) 0x08)
#define HT16K33_DIMSETUP_DUTY10                 ((uint8_t) 0x09)
#define HT16K33_DIMSETUP_DUTY11                 ((uint8_t) 0x0A)
#define HT16K33_DIMSETUP_DUTY12                 ((uint8_t) 0x0B)
#define HT16K33_DIMSETUP_DUTY13                 ((uint8_t) 0x0C)
#define HT16K33_DIMSETUP_DUTY14                 ((uint8_t) 0x0D)
#define HT16K33_DIMSETUP_DUTY15                 ((uint8_t) 0x0E)
#define HT16K33_DIMSETUP_DUTY16                 ((uint8_t) 0x0F)

/***********************************************************************************\
 *                                                                                 *
 *                              HT16K33 FUNCTIONS PROTOTYPES                       *
 *                                                                                 *
\***********************************************************************************/

void DISPLAY_set_brightness(uint8_t duty);

void DISPLAY_power_on(void);

void DISPLAY_write_time(uint8_t *tdata, uint8_t data_size);

void DISPLAY_set_blink_freq(uint8_t freq);

#endif
