/**
  **********************************************************************************
  * @file    VOICE_CHANGER/include/i2c.h                                           *
  * @author  Nolan R. Gagnon                                                       *
  * @version V1.0                                                                  *
  * @date    15-June-2017                                                          *
  * @brief   I2C Interface.                                                        *
  *                                                                                *
  **********************************************************************************
  * @attention                                                                     *
  *                                                                                *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. Gagnon </center></h2>            *
  *                                                                                *
  **********************************************************************************
  */
#ifndef I2C_H
#define I2C_H

/***********************************************************************************
 *                                                                                 *
 *                              I2C CONSTANTS                                      *
 *                                                                                 *
 ***********************************************************************************/
#define I2C_CLK_FREQ_DIV_4		((uint32_t) 3U << 28)

/***********************************************************************************
 *                                                                                 *
 *                              I2C FUNCTIONS                                      *
 *                                                                                 *
 ***********************************************************************************/

/* initialize i2c1 peripheral */
void i2c1_init(void);

/* configure pins used for i2c1 */
void i2c1_pins_init(void);

/* transmit data on the i2c1 bus */
void i2c1_transmit(uint8_t nbytes, uint8_t slvaddr, uint8_t *payload);

#endif
