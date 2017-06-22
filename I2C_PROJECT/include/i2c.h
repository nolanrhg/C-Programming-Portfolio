/**
  **********************************************************************************
  * @file    I2C_PROJECT/include/i2c.h                                             *
  * @author  Enki                                                                  *
  * @version V1.0                                                                  *
  * @date    21-June-2017                                                          *
  * @brief   I2C Interface.                                                        *
  *                                                                                *
  **********************************************************************************
  * @attention                                                                     *
  *                                                                                *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Enki </center></h2>                       *
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
#define I2C_CLK_FREQ_DIV_2	((uint32_t) 1U << 28)

/***********************************************************************************
 *                                                                                 *
 *                              I2C FUNCTIONS                                      *
 *                                                                                 *
 ***********************************************************************************/

/* initialize i2c1 peripheral */
void i2c1_init(void);

/* configure pins used for i2c1 */
static void i2c1_pins_init(void);

/* transmit data on the i2c1 bus */
void i2c1_transmit(uint8_t nbytes, uint8_t slvaddr, uint8_t *payload);

/* read data from a slave on the i2c1 bus */
void i2c1_read(uint8_t nbytes, uint8_t slvaddr, uint8_t *reg, uint8_t *storage_ptr);

#endif
