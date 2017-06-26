/**********************************************************************************\
 * @file    I2C_PROJECT/include/adc.h                                             *
 * @author  Nolan R. H. Gagnon                                                    *
 * @version V2.0                                                                  *
 * @date    26-June-2017                                                          *
 * @brief   A/D converter interface.                                              *
 *                                                                                *
 **********************************************************************************
 * @attention                                                                     *
 *                                                                                *
 * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. H. Gagnon  </center></h2>        *
 *                                                                                *
\**********************************************************************************/
#ifndef ADC_H
#define ADC_H

/**********************************************************************************\
 *                                                                                *
 *                                  ADC CONSTANTS                                 *
 *                                                                                *
\**********************************************************************************/
#define ADCONV_TRIG_RISEDGE		((uint32_t) 0x01 << 10)
#define ADCONV_TRIG_TIM1CH2		((uint32_t) 0x01 << 6)
#define NODE_VOLTAGE			ADC1->DR
#define MAX_REPEAT_ADCONV		10

/**********************************************************************************\
 *                                                                                *
 *                              ADC FUNCTION PROTOTYPES                           *
 *                                                                                *
\**********************************************************************************/

/**
  * @brief Configures PA1 for analog input.
  * @param None
  * @retval None
  */ 
static void adc1_pin_init(void);

/**
  * @brief Brings the ADC out of deep-power-down mode.
  * @param None
  * @retval None
  */
static void adc1_resurrect(void);

/**
  * @brief Configures ADC1 with the following characteristics :
  *
  *	   (*) Conversions triggered externally by TIM1_CH2 
  * 	       at a rate of 1 conversion every 10 ms.
  *
  *        (*) 12-bit resolution.
  *
  *	   (*) Conversions right-aligned in the data register.
  *
  *	   (*) Single-ended mode used.
  *
  *	   (*) Only one conversion int he regular sequence (conversion of channel 6)
  *
  * @param None
  * @retval None
  */
static void adc1_configure(void);

/**
  * @brief Turns on ADC1.
  * @param None
  * @retval None
  */
static void adc1_enable(void);

/**
  * @brief Configures and enables ADC1.
  * @param None
  * @retval None
  */
void adc1_init(void);

#endif
