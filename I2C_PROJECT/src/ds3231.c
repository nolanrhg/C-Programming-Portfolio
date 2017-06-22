/**********************************************************************************\
 * @file    I2C_PROJECT/src/ds3231.c                                              *
 * @author  Enki                                                                  *
 * @version V1.0                                                                  *
 * @date    21-June-2017                                                          *
 * @brief   DS3231 driver.                                                        *
 *                                                                                *
 **********************************************************************************
 * @attention                                                                     *
 *                                                                                *
 * <h2><center>&copy; COPYRIGHT(c) 2017 Enki </center></h2>                       *
 *                                                                                *
\**********************************************************************************/

#include "../include/i2c.h"
#include "../include/ds3231.h"
#include "../include/aux.h"

void RTC_set_date(Date_TypeDef *restrict d, DS3231_TypeDef *restrict rtc)
{
	uint8_t date_settings[8];
	uint8_t tens_digit;
	uint8_t ones_digit;
	uint8_t thousands_digit;
	
	RTC_struct_reset(rtc);
	
	/* First data byte will be written here */	
	date_settings[0] = DS3231_SECR_PTR;

	/* Convert second to BCD */
	tens_digit = get_digit(2, d->second);		
	ones_digit = get_digit(1, d->second);
	
	rtc->SECR = 0x00 | (tens_digit << 4) | (ones_digit << 0);	
	date_settings[1] = rtc->SECR;

	/* Convert minute to BCD */
	tens_digit = get_digit(2, d->minute);		
	ones_digit = get_digit(1, d->minute);
	
	rtc->MINR = 0x00 | (tens_digit << 4) | (ones_digit << 0);	
	date_settings[2] = rtc->MINR;

	/* Set hour register */
	tens_digit = get_digit(2, d->hour);
	ones_digit = get_digit(1, d->hour);

	rtc->HOURR |= DS3231_HOURR_MILTIME;	// Use military time

	switch (tens_digit) {
	case 0:
		rtc->HOURR |= 0 << 4;
		break;
	case 1:
		rtc->HOURR |= 1 << 4;
		break;
	case 2:
		rtc->HOURR |= DS3231_HOURR_20HR;
		break;
	default:
		break;
	}
	
	rtc->HOURR |= ones_digit << 0;
	date_settings[3] = rtc->HOURR;

	/* Set day register */
	rtc->DAYR |= d->day;		
	date_settings[4] = rtc->DAYR;

	/* Set date register */
	tens_digit = get_digit(2, d->date);
	ones_digit = get_digit(1, d->date);
	rtc->DATER |= tens_digit << 4;
	rtc->DATER |= ones_digit << 0;	
	date_settings[5] = rtc->DATER;

	/* Set month register */
	thousands_digit = get_digit(4, (uint32_t) d->year);

	if (thousands_digit == 1)
		rtc->MONTHR |= 0 << 7;
	else if (thousands_digit == 2)
		rtc->MONTHR |= 1 << 7;
	else
		rtc->MONTHR |= 1 << 7;

	tens_digit = get_digit(2, (uint32_t) d->month);
	ones_digit = get_digit(1, (uint32_t) d->month);
	
	rtc->MONTHR |= (tens_digit << 4) | (ones_digit << 0);
	date_settings[6] = rtc->MONTHR;

	/* Set year register */
	tens_digit = get_digit(2, (uint32_t) d->year);
	ones_digit = get_digit(1, (uint32_t) d->year);	
	
	rtc->YEARR |= (tens_digit << 4)	| (ones_digit << 0);
	date_settings[7] = rtc->YEARR;
	
	/* Transmit date settings to DS3231 via I2C bus */
	i2c1_transmit(8, DS3231_I2C_ADDR, date_settings);
}

void RTC_read_date(Date_TypeDef *restrict d, DS3231_TypeDef *restrict rtc)
{	
	uint8_t reg[1] = {DS3231_SECR_PTR};
	uint8_t *storage;

	storage = (uint8_t *) malloc(7 * sizeof(uint8_t));

	i2c1_read(7, DS3231_I2C_ADDR, reg, storage);
	
	d->second = rtc_reg_bcd_to_int(storage[0]);
	d->minute = rtc_reg_bcd_to_int(storage[1]);
	
	if (0x20 & storage[2]) 
		storage[2] = 0x20 | (storage[2] & 0x0F);
	else
		storage[2] = (0x10 & storage[2]) | (storage[2] & 0x0F);

	d->hour = rtc_reg_bcd_to_int(storage[2]);
	d->day = storage[3] & 0x0F;
	d->date = rtc_reg_bcd_to_int(storage[4]);
	d->month = rtc_reg_bcd_to_int((uint8_t) (0x7F & storage[5]));	

	if (0x80 & storage[5])
		d->year = 2000 + rtc_reg_bcd_to_int(storage[6]);
	else
		d->year = 1900 + rtc_reg_bcd_to_int(storage[6]);

	free(storage);
}

void RTC_struct_reset(DS3231_TypeDef *rtc)
{
	rtc->SECR = 0x00;
	rtc->MINR = 0x00;
	rtc->HOURR = 0x00;
	rtc->DAYR = 0x00;
	rtc->DATER = 0x00;
	rtc->MONTHR = 0x00;
	rtc->YEARR = 0x00;
	rtc->ALRM1SECR = 0x00;
	rtc->ALRM1MINR = 0x00;
	rtc->ALRM1HOURR = 0x00;
	rtc->ALRM1DAYR = 0x00;
	rtc->ALRM2MINR = 0x00;
	rtc->ALRM2HOURR = 0x00;
	rtc->ALRM2DAYR = 0x00;
	rtc->CTLR = 0x00;
	rtc->SR = 0x00;
	rtc->AGEOFFR = 0x00;
	rtc->TEMPMSBR = 0x00;
	rtc->TEMPLSBR = 0x00;
}
