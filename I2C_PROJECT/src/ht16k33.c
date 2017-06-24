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

/***********************************************************************************\
 *                                                                                 *
 *                                  INCLUDES                                       *
 *                                                                                 *
\***********************************************************************************/
#include "../include/ht16k33.h"
#include "../include/i2c.h"

/***********************************************************************************\
 *                                                                                 *
 *                              HT16K33 FUNCTIONS                                  *
 *                                                                                 *
\***********************************************************************************/
void DISPLAY_set_brightness(uint8_t duty)
{
	uint8_t payload[1];
	uint8_t parameterized_cmd;	
	
	parameterized_cmd = HT16K33_DIMSETUP | duty;
	payload[0] = parameterized_cmd;
	
	i2c1_transmit(1, HT16K33_I2C_ADDR, payload);	
}

void DISPLAY_power_on(void)
{
	uint8_t payload[1];
	uint8_t parameterized_cmd;		

	/* Turn oscillator on */	
	parameterized_cmd = HT16K33_SSETUP | HT16K33_SSETUP_OSCON;	
	payload[0] = parameterized_cmd;	
	i2c1_transmit(1, HT16K33_I2C_ADDR, payload);

	/* Turn display on with no blinking */
	parameterized_cmd = HT16K33_DSETUP | HT16K33_DSETUP_DISPON | HT16K33_DSETUP_BLKOFF;
	payload[0] = parameterized_cmd;	
	i2c1_transmit(1, HT16K33_I2C_ADDR, payload);
}

void DISPLAY_write_time(uint8_t *tdata, uint8_t data_size)
{
	uint8_t i;
	uint8_t *payload;		
	
	payload = (uint8_t *) malloc((data_size + 1) * sizeof(uint8_t)); 

	payload[0] = HT16K33_DISPDATR_PTR;
	
	for (i = 1; i < data_size + 1; i++) {
		payload[i] = tdata[i - 1];	
	}

	i2c1_transmit(data_size + 1, HT16K33_I2C_ADDR, payload);	

	free(payload);
}

void DISPLAY_set_blink_freq(uint8_t freq)
{
	uint8_t payload[1];
	uint8_t parameterized_cmd;	
	
	parameterized_cmd = HT16K33_DSETUP | freq | HT16K33_DSETUP_DISPON;
	payload[0] = parameterized_cmd;	
	
	i2c1_transmit(1, HT16K33_I2C_ADDR, payload);		
}
