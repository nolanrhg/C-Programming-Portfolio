/**********************************************************************************\
 * @file    I2C_PROJECT/src/ds3231.c                                              *
 * @author  Nolan R. H. Gagnon                                                    *
 * @version V1.0                                                                  *
 * @date    26-June-2017                                                          *
 * @brief   DS3231 driver.                                                        *
 *                                                                                *
 **********************************************************************************
 * @attention                                                                     *
 *                                                                                *
 * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. H. Gagnon </center></h2>         *
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

	rtc->HOURR &= ~DS3231_HOURR_NONMILTIME;	// Use military time

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

void RTC_enable_interrupts(DS3231_TypeDef *restrict rtc, Alarm_TypeDef *restrict alarm)
{
	uint8_t tens_digit;
	uint8_t ones_digit;
	uint8_t i2c_payload[5];
	
	RTC_struct_reset(rtc);
		
	if (alarm->alrm_num == ALARM1) {
		i2c_payload[0] = DS3231_ALRM1SECR_PTR;

		switch (alarm->rate) {

		case PERSEC:
			break;

		case PERMIN:
			
			/* Set alarm masks */
			rtc->ALRM1SECR &= ~DS3231_ALRM1SECR_MASK1;
			rtc->ALRM1MINR |= DS3231_ALRM1MINR_MASK2;
			rtc->ALRM1HOURR |= DS3231_ALRM1HOURR_MASK3;
			rtc->ALRM1DAYR |= DS3231_ALRM1DAYR_MASK4;
			
			/* Set alarm second */
			tens_digit = get_digit(2, alarm->second);	
			ones_digit = get_digit(1, alarm->second);
			rtc->ALRM1SECR |= (0x7F) & ((tens_digit << 4) | (ones_digit << 0));
			i2c_payload[1] = rtc->ALRM1SECR;
			
			/* Set alarm minute */
			tens_digit = get_digit(2, alarm->minute);
			ones_digit = get_digit(1, alarm->minute);
			rtc->ALRM1MINR |= (0x7F) & ((tens_digit << 4) | (ones_digit << 0));
			i2c_payload[2] = rtc->ALRM1MINR;

			/* Set alarm hour */
			tens_digit = get_digit(2, alarm->hour);
			ones_digit = get_digit(1, alarm->hour);

			if (alarm->hour >= 20)
				rtc->ALRM1HOURR |= DS3231_ALRM1HOURR_20HR;

			rtc->ALRM1HOURR |= (0x1F) & ((tens_digit << 4) | (ones_digit << 0));
			i2c_payload[3] = rtc->ALRM1HOURR;
				
			/* Set alarm day */	
			rtc->ALRM1DAYR |= DS3231_ALRM1DAYR_DAY;
			rtc->ALRM1DAYR |= (0x0F) & (alarm->day);
			i2c_payload[4] = rtc->ALRM1DAYR;
			
			break;

		default:
			break;		

		}	
		
		rtc->CTLR |= DS3231_CTLR_INTEN;
		rtc->CTLR |= DS3231_CTLR_A1IE;
		
	} else {
		i2c_payload[0] = DS3231_ALRM2MINR_PTR;	

		switch (alarm->rate) {
		case PERSEC:
			break;
		case PERMIN:
			break;
		default:
			break;		
		}
	}

	i2c1_transmit(5, DS3231_I2C_ADDR, i2c_payload);
		
	i2c_payload[0] = DS3231_CTLR_PTR;
	i2c_payload[1] = rtc->CTLR;
	
	i2c1_transmit(2, DS3231_I2C_ADDR, i2c_payload);
}

void RTC_clear_interrupt_flag(DS3231_TypeDef *restrict rtc, DS3231_ALARM_TypeDef alrm)
{
	uint8_t payload[2];
	
	payload[0] = DS3231_SR_PTR;
	
	if (alrm == ALARM1)	
		rtc->SR &= ~DS3231_SR_A1F;
	else
		rtc->SR &= ~DS3231_SR_A2F;
	
	payload[1] = rtc->SR;
	
	i2c1_transmit(2, DS3231_I2C_ADDR, payload);
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
