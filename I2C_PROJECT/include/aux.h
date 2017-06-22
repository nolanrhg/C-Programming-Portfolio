#ifndef AUX_H
#define AUX_H

/**
  * @brief This function extracts a certain digit from an integer.
  * @param digit : The digit to be extracted.
  * @param num : The integer to extract the digit from.
  * @retval get_digit : The extracted digit.
  */
uint8_t get_digit(uint8_t digit, uint32_t num);

uint8_t rtc_reg_bcd_to_int(uint8_t num);

#endif
