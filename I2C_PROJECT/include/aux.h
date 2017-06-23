 /**********************************************************************************\
  * @file    I2C_PROJECT/include/aux.h                                             *
  * @author  Nolan R. Gagnon                                                       *
  * @version V1.0                                                                  *
  * @date    23-June-2017                                                          *
  * @brief   Auxiliary Functions.                                                  *
  *                                                                                *
  **********************************************************************************
  * @attention                                                                     *
  *                                                                                *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. Gagnon  </center></h2>           *
  *                                                                                *
 \**********************************************************************************/
#ifndef AUX_H
#define AUX_H

/***********************************************************************************\
 *                                                                                 *
 *                              FUNCTION PROTOTYPES                                *
 *                                                                                 *
\***********************************************************************************/

/**
  * @brief This function extracts a specified digit from an integer.
  * @param digit : The digit to be extracted.
  * @param num : The integer to extract the digit from.
  * @retval get_digit : The extracted digit.
  */
uint8_t get_digit(uint8_t digit, uint32_t num);

uint8_t rtc_reg_bcd_to_int(uint8_t num);

uint8_t digit_to_7seg(uint8_t digit);

void time_to_7seg(uint8_t hour, uint8_t min, uint8_t *segarr);

#endif
