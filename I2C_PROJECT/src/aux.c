 /**********************************************************************************\
  * @file    I2C_PROJECT/include/aux.c                                             *
  * @author  Nolan R. Gagnon                                                       *
  * @version V1.0                                                                  *
  * @date    23-June-2017                                                          *
  * @brief   Auxiliary functions                                                   *
  *                                                                                *
  **********************************************************************************
  * @attention                                                                     *
  *                                                                                *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. Gagnon  </center></h2>           *
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

uint8_t digit_to_7seg(uint8_t digit)
{
	switch (digit) {	
	case 0:
		return 0x3FU;
	case 1:
		return 0x06U;		
	case 2:
		return 0x5BU;
	case 3:
		return 0x4FU;
	case 4:
		return 0x66U;
	case 5:
		return 0x6DU;
	case 6:
		return 0x7DU;
	case 7:
		return 0x07U;
	case 8:
		return 0x7FU;
	case 9:
		return 0x6FU;
	default:
		return 0xFFU;
	}
}

void time_to_7seg(uint8_t hour, uint8_t min, uint8_t *segarr)
{
	uint8_t tens_digit;
	uint8_t ones_digit;	
	
	/* hour */	
	tens_digit = get_digit(2, hour);
	ones_digit = get_digit(1, hour);	
	segarr[0] = digit_to_7seg(tens_digit);
	segarr[1] = (uint8_t) 0x00;
	segarr[2] = digit_to_7seg(ones_digit);
	segarr[3] = (uint8_t) 0x00;
	segarr[4] = (uint8_t) 0x02;  // Colon
	segarr[5] = (uint8_t) 0x00;
	
	/* minute */
	tens_digit = get_digit(2, min);
	ones_digit = get_digit(1, min);
	segarr[6] = digit_to_7seg(tens_digit);
	segarr[7] = (uint8_t) 0x00;
	segarr[8] = digit_to_7seg(ones_digit);	
	segarr[9] = (uint8_t) 0x00;	
}
