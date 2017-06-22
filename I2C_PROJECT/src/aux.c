 /**********************************************************************************\
  * @file    I2C_PROJECT/include/aux.c                                             *
  * @author  Enki                                                                  *
  * @version V1.0                                                                  *
  * @date    21-June-2017                                                          *
  * @brief   Auxiliary functions                                                   *
  *                                                                                *
  **********************************************************************************
  * @attention                                                                     *
  *                                                                                *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Enki  </center></h2>                      *
  *                                                                                *
 \**********************************************************************************/
#include <math.h>
#include "../include/aux.h"

/***********************************************************************************\
 *                                                                                 *
 *                              AUX FUNCTIONS                                      *
 *                                                                                 *
\***********************************************************************************/

uint8_t get_digit(uint8_t digit, uint32_t num)
{
	uint8_t num_digits = floor(log10(num) + 1);

	if (digit > num_digits)
		return 0;
	else
		return (uint8_t)(((uint32_t)(num / (pow(10, digit - 1)))) % 10);
}

uint8_t rtc_reg_bcd_to_int(uint8_t num)
{
	return (((num & 0xF0) >> 4) * 10) + (num & 0x0F);
}
