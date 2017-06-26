/**********************************************************************************\
 * @file    I2C_PROJECT/include/aux.h                                             *
 * @author  Nolan R. H. Gagnon                                                    *
 * @version V2.0                                                                  *
 * @date    26-June-2017                                                          *
 * @brief   Auxiliary Functions.                                                  *
 *                                                                                *
 **********************************************************************************
 * @attention                                                                     *
 *                                                                                *
 * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. H. Gagnon  </center></h2>        *
 *                                                                                *
\**********************************************************************************/
#ifndef AUX_H
#define AUX_H

/**********************************************************************************\
 *                                                                                *
 *                              FUNCTION PROTOTYPES                               *
 *                                                                                *
\**********************************************************************************/

/**
  * @brief This function extracts a specified digit from an integer.
  * @param digit : The digit to be extracted.
  * @param num : The integer to extract the digit from.
  * @retval get_digit : The extracted digit.
  */
uint8_t get_digit(uint8_t digit, uint32_t num);

/**
  * @brief Converts BCD value from RTC register to an 8-bit integer.
  * @param num : BCD value from RTC register.
  * @retval rtc_reg_bcd_to_int : The BCD value converted to an 8-bit integer.
  */
uint8_t rtc_reg_bcd_to_int(uint8_t num);

/**
  * @brief Converts a digit to 7-segment LED display hexadecimal code. 
  * @param digit : Digit to be converted to 7-segment code.
  * @retval digit_to_7seg : The 7-segment hexadecimal code.
  */
uint8_t digit_to_7seg(uint8_t digit);

/**
  * @brief Converts current hour and minute to an array of 7-segment hex. codes. 
  * @param hour : The current hour.
  * @param min : The current minute.
  * @param segarr : Pointer to the array that holds the 7-segment hex. codes.
  * @retval None 
  */
void time_to_7seg(uint8_t hour, uint8_t min, uint8_t *segarr);

#endif
