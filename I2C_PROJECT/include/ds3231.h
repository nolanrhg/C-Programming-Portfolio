 /**********************************************************************************\
  * @file    I2C_PROJECT/include/ds3231.h                                          *
  * @author  Enki                                                                  *
  * @version V1.0                                                                  *
  * @date    20-June-2017                                                          *
  * @brief   DS3231 Application Interface.                                         *
  *                                                                                *
  **********************************************************************************
  * @attention                                                                     *
  *                                                                                *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Enki  </center></h2>     	           *
  *                                                                                *
 \**********************************************************************************/
#ifndef DS3231_H
#define DS3231_H

#define __IO			volatile

/***********************************************************************************\
 *                                                                                 *
 *                              DS3231 ENUMS                                       *
 *                                                                                 *
\***********************************************************************************/
typedef enum
{
	PERSEC = 0,
	PERMIN = 1,
	PERHOUR = 2,
	PERDAY = 3,
	PERWEEK = 4,
	PERMONTH = 5
} ALARM_RATE_TypeDef;

typedef enum
{
	ALARM1 = 0,
	ALARM2 = 1
} DS3231_ALARM_TypeDef;

/***********************************************************************************\
 *                                                                                 *
 *                              DS3231 STRUCTS                                     *
 *                                                                                 *
\***********************************************************************************/

/* RTC Struct ---------------------------------------------------------------------*/
typedef struct
{
	__IO uint8_t SECR;		/*!< DS3231 Seconds Register */
	__IO uint8_t MINR;		/*!< DS3231 Minutes Register */
	__IO uint8_t HOURR;		/*!< DS3231 Hours Register */
	__IO uint8_t DAYR;		/*!< DS3231 Day Register */
	__IO uint8_t DATER;		/*!< DS3231 Date Register */
	__IO uint8_t MONTHR;		/*!< DS3231 Month/Century Register */
	__IO uint8_t YEARR;		/*!< DS3231 Year Register */
	__IO uint8_t ALRM1SECR;		/*!< DS3231 Alarm 1 Seconds Register */
	__IO uint8_t ALRM1MINR;		/*!< DS3231 Alarm 1 Minutes Register */
	__IO uint8_t ALRM1HOURR;	/*!< DS3231 Alarm 1 Hours Register */
	__IO uint8_t ALRM1DAYR;		/*!< DS3231 Alarm 1 Day Register */
	__IO uint8_t ALRM2MINR;		/*!< DS3231 Alarm 2 Minutes Register */
	__IO uint8_t ALRM2HOURR;	/*!< DS3231 Alarm 2 Hours Register */
	__IO uint8_t ALRM2DAYR;		/*!< DS3231 Alarm 2 Day Register */
	__IO uint8_t CTLR;		/*!< DS3231 Control Register */
	__IO uint8_t SR;		/*!< DS3231 Status Register */
	__IO uint8_t AGEOFFR;		/*!< DS3231 Aging Offset Register */
	__IO uint8_t TEMPMSBR;		/*!< DS3231 MSB of Temperature Register */
	__IO uint8_t TEMPLSBR;		/*!< DS3231 LSB of Temperature Register */
} DS3231_TypeDef;

/* Date Struct -------------------------------------------------------------------*/
typedef struct
{
	__IO uint8_t second;
	__IO uint8_t minute;
	__IO uint8_t hour;
	__IO uint8_t day;
	__IO uint8_t date;
	__IO uint8_t month;
	__IO uint16_t year;
} Date_TypeDef;

/* Alarm Struct ------------------------------------------------------------------*/
typedef struct
{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t date;
	DS3231_ALARM_TypeDef alrm_num;
	ALARM_RATE_TypeDef rate;
} Alarm_TypeDef;

/***********************************************************************************\
 *                                                                                 *
 *                              DS3231 I2C ADDRESS                                 *
 *                                                                                 *
\***********************************************************************************/
#define DS3231_I2C_ADDR			((uint8_t) 0x68)

/***********************************************************************************\
 *                                                                                 *
 *                              DS3231 REGISTER ADDRESSES                          *
 *                                                                                 *
\***********************************************************************************/
#define DS3231_SECR_PTR			((uint8_t) 0x00)
#define DS3231_MINR_PTR			((uint8_t) 0x01)
#define DS3231_HOURR_PTR		((uint8_t) 0x02)
#define DS3231_DAYR_PTR			((uint8_t) 0x03)
#define DS3231_DATER_PTR		((uint8_t) 0x04)	
#define DS3231_MONTHR_PTR 		((uint8_t) 0x05)
#define	DS3231_YEARR_PTR		((uint8_t) 0x06)
#define DS3231_ALRM1SECR_PTR		((uint8_t) 0x07)
#define DS3231_ALRM1MINR_PTR		((uint8_t) 0x08)
#define DS3231_ALRM1HOURR_PTR  		((uint8_t) 0x09)	
#define DS3231_ALRM1DAYR_PTR		((uint8_t) 0x0A)	
#define DS3231_ALRM2MINR_PTR   		((uint8_t) 0x0B)	
#define DS3231_ALRM2HOURR_PTR		((uint8_t) 0x0C)
#define DS3231_ALRM2DAYR_PTR		((uint8_t) 0x0D)
#define DS3231_CTLR_PTR			((uint8_t) 0x0E)
#define DS3231_SR_PTR			((uint8_t) 0x0F)
#define DS3231_AGEOFFR_PTR		((uint8_t) 0x10)
#define DS3231_TEMPMSBR_PTR		((uint8_t) 0x11)
#define DS3231_TEMPLSBR_PTR		((uint8_t) 0x12)	

/***********************************************************************************\
 *                                                                                 *
 *                              DS3231 REGISTER BIT DEFS                           *
 *                                                                                 *
\***********************************************************************************/

/* SECR ---------------------------------------------------------------------------*/

// Bit 7 is reserved:  must be 0.
// Bits 6 - 4 are the ten's digit of the current second.
// Bits 3 - 0 are the one's digit of the current second.

/* MINR ---------------------------------------------------------------------------*/

// Bit 7 is reserved:  must be 0.
// Bits 6 - 4 are the ten's digit of the current minute.
// Bits 3 - 0 are the one's digit of the current minute.

/* HOURR --------------------------------------------------------------------------*/

// Bit 7 is reserved:  must be 0.
#define DS3231_HOURR_NONMILTIME		((uint8_t) 0x40)	/*!< Time format is not military time */
#define DS3231_HOURR_PM			((uint8_t) 0x20)	/*!< Current time is PM */
#define DS3231_HOURR_20HR		((uint8_t) 0x20)	/*!< Current hour is between 20 and 23 */
// Bit 4 is the ten's digit of the current hour.
// Bits 3 - 0 are the one's digit of the current hour.

/* DAYR ---------------------------------------------------------------------------*/

// Bits 7 - 3 are reserved:  must be 0.
#define DS3231_DAYR_MON			((uint8_t) 0x01)	/*!< Current day is Monday */
#define DS3231_DAYR_TUE			((uint8_t) 0x02)	/*!< Current day is Tuesday */
#define DS3231_DAYR_WED			((uint8_t) 0x03)	/*!< Current day is Wednesday */
#define DS3231_DAYR_THUR		((uint8_t) 0x04)	/*!< Current day is Thursday */
#define DS3231_DAYR_FRI			((uint8_t) 0x05)	/*!< Current day is Friday */
#define DS3231_DAYR_SAT			((uint8_t) 0x06)	/*!< Current day is Saturday */
#define DS3231_DAYR_SUN			((uint8_t) 0x07)	/*!< Current day is Sunday */

/* DATER ---------------------------------------------------------------------------*/
// Bits 7 - 6 are reserved:  must be 0;
// Bits 5 - 4 are the ten's digit of the current date.
// Bits 3 - 0 are the one's digit of the current date.

/* MONTHR --------------------------------------------------------------------------*/

// Bit 7 determines century
// Bits 6 - 5 are reserved:  must be 0.
#define DS3231_MONTHR_JAN		((uint8_t) 0x01)	/*!< Current month is January */
#define DS3231_MONTHR_FEB		((uint8_t) 0x02)	/*!< Current month is February */
#define DS3231_MONTHR_MAR		((uint8_t) 0x03)	/*!< Current month is March */
#define DS3231_MONTHR_APR		((uint8_t) 0x04)	/*!< Current month is April */
#define DS3231_MONTHR_MAY		((uint8_t) 0x05)	/*!< Current month is May */
#define DS3231_MONTHR_JUN		((uint8_t) 0x06)	/*!< Current month is June */
#define DS3231_MONTHR_JUL		((uint8_t) 0x07)	/*!< Current month is July */
#define DS3231_MONTHR_AUG		((uint8_t) 0x08)	/*!< Current month is August */
#define DS3231_MONTHR_SEP		((uint8_t) 0x09)	/*!< Current month is September */
#define DS3231_MONTHR_OCT		((uint8_t) 0x0A)	/*!< Current month is October */
#define DS3231_MONTHR_NOV		((uint8_t) 0x0B)	/*!< Current month is November */
#define DS3231_MONTHR_DEC		((uint8_t) 0x0C)	/*!< Current month is December */

/* YEARR ----------------------------------------------------------------------------*/

// Bits 7 - 4 are the ten's digit of the current year.
// Bits 3 - 0 are the one's digit of the current year.

/* ALRM1SECR ------------------------------------------------------------------------*/

#define DS3231_ALRM1SECR_MASK1		((uint8_t) 0x80)
// Bits 6 - 4 are the ten's digit of the alarm1 second.
// Bits 3 - 0 are the one's digit of the alarm1 second.

/* ALRM1MINR ------------------------------------------------------------------------*/

#define DS3231_ALRM1MINR_MASK2		((uint8_t) 0x80)
// Bits 6 - 4 are the ten's digit of the alarm1 minute.
// Bits 3 - 0 are the one's digit of the alarm1 minute.

/* ALRM1HOURR -----------------------------------------------------------------------*/

#define DS3231_ALRM1HOURR_MASK3		((uint8_t) 0x80)
#define DS3231_ALRM1HOURR_NONMILTIME	((uint8_t) 0x40)
#define DS3231_ALRM1HOURR_PM		((uint8_t) 0x20)
#define DS3231_ALRM1HOURR_20HR		((uint8_t) 0x20)
// Bit 4 is the ten's digit of the alarm1 hour.
// Bits 3 - 0 are the one's digit of the alarm1 hour.

/* ALRM1DAYR -------------------------------------------------------------------------*/

#define DS3231_ALRM1DAYR_MASK4		((uint8_t) 0x80)
#define DS3231_ALRM1DAYR_DAY		((uint8_t) 0x40)
// Bits 5 - 4 are the ten's digit of the current date.
// Bits 3 - 0 are either the one's digit of the alarm1 date or the alarm1 day (depends on the DAY bit defined above)

/* ALRM2MINR --------------------------------------------------------------------------*/

#define DS3231_ALRM2MINR_MASK2		((uint8_t) 0x80)
// Bits 6 - 4 are the ten's digit of the alarm2 minute.
// Bits 3 - 0 are the one's digit of the alarm2 minute.

/* ALRM2HOURR -------------------------------------------------------------------------*/

#define DS3231_ALRM2HOURR_MASK3		((uint8_t) 0x80)
#define DS3231_ALRM2HOURR_NONMILTIME	((uint8_t) 0x40)
#define DS3231_ALRM2HOURR_PM		((uint8_t) 0x20)
// Bit 4 is the ten's digit of the alarm2 hour.
// Bits 3 - 0 are the one's digit of the alarm2 hour.

/* ALRM2DAYR --------------------------------------------------------------------------*/

#define DS3231_ALRM2DAYR_MASK4		((uint8_t) 0x80)
#define DS3231_ALRM2DAYR_DAY		((uint8_t) 0x40)
// Bits 5 - 4 are the ten's digit of the alarm2 date.
// Bits 3 - 0 are either the one's digit of the alarm2 date or the alarm2 day (depends on DAY bit defined above)

/* CTLR -------------------------------------------------------------------------------*/

#define DS3231_CTLR_OSCDIS		((uint8_t) 0x80)	/*!< Disables the oscillator */
#define DS3231_CTLR_SQWEN		((uint8_t) 0x40)	/*!< Enables square wave output */
#define DS3231_CTLR_CONV		((uint8_t) 0x20)	/*!< Convert temperature */
#define DS3231_CTLR_SQW1HZ		((uint8_t) 0x00)	/*!< Square wave output has freq. = 1 Hz */
#define DS3231_CTLR_SQW1024HZ		((uint8_t) 0x08)	/*!< Square wave output has freq. = 1.024 kHz */
#define DS3231_CTLR_SQW4096HZ		((uint8_t) 0x10)	/*!< Square wave output has freq. = 4.096 kHz */
#define DS3231_CTLR_SQW8192HZ		((uint8_t) 0x18)	/*!< Square wave output has freq. = 8.192 kHz */
#define DS3231_CTLR_INTEN		((uint8_t) 0x04)	/*!< Enable interrupts */
#define DS3231_CTLR_A2IE		((uint8_t) 0x02)	/*!< Allow alarm2 to generate interrupts */
#define DS3231_CTLR_A1IE		((uint8_t) 0x01)	/*!< Allow alarm1 to generate interrupts */

/* SR ---------------------------------------------------------------------------------*/

// Bit 7 is the oscillator stop flag.
// Bits 6 - 4 are reserved:  must be 0.
#define DS3231_SR_32KEN			((uint8_t) 0x80)	/*!< Enables 32.768 kHz square wave output */
// Bit 2 is the busy flag.
#define DS3231_SR_A2F			((uint8_t) 0x02)
#define DS3231_SR_A1F			((uint8_t) 0x01)

/* AGEOFFR -----------------------------------------------------------------------------*/

#define DS3231_AGEOFFR_NEG		((uint8_t) 0x80)	/*!< Makes aging offset value negative */
// Bits 6 - 0 are the aging offset value.

/* TEMPMSBR ----------------------------------------------------------------------------*/

// Bit 7 is the temperature sign bit.
// Bits 6 - 0 are the MSBits of the RTC temperature.

/* TEMPLSBR ----------------------------------------------------------------------------*/

// Bits 7 - 6 are the LSBits of the RTC temperature.
// Bits 5 - 0 are reserved:  must be 0.


/***********************************************************************************\
 *                                                                                 *
 *                              RTC FUNCTIONS                                      *
 *                                                                                 *
\***********************************************************************************/

/**
  * @brief
  *
  *
  * @param : date  
  * @param : rtc
  * @retval None
  */
void RTC_set_date(Date_TypeDef *restrict date, DS3231_TypeDef *restrict rtc);

/**
  * @brief
  *
  *
  * @param : time_arr 
  * @retval None
  */
void RTC_read_date(Date_TypeDef *restrict d, DS3231_TypeDef *restrict rtc);

/**
  * @brief
  *
  *
  * @param : alarm 
  * @retval None
  */
void RTC_set_alarm(Alarm_TypeDef *restrict alarm);

/**
  * @brief
  *
  *
  * @param : alarm 
  * @retval None
  */
void RTC_read_alarm(Alarm_TypeDef *restrict alarm);

/**
  * @brief
  * 
  *
  * @param alrm_num :
  * @param alrm_rate :
  * @retval None
  */
void RTC_enable_interrupts(DS3231_TypeDef *restrict rtc, Alarm_TypeDef *restrict alarm);

/**
  * @brief
  * 
  *
  * @param alrm_num :
  * @param alrm_rate :
  * @retval None
  */
void RTC_clear_interrupt_flag(DS3231_TypeDef *restrict rtc, DS3231_ALARM_TypeDef alrm);

/**
  * @brief
  * 
  * @param rtc : 
  * @retval None
  */
void RTC_struct_reset(DS3231_TypeDef *rtc);

#endif
