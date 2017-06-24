 /**********************************************************************************\
  * @file    I2C_PROJECT/include/adc.h                                             *
  * @author  Nolan R. Gagnon                                                       *
  * @version V1.0                                                                  *
  * @date    24-June-2017                                                          *
  * @brief   A/D converter Interface.                                              *
  *                                                                                *
  **********************************************************************************
  * @attention                                                                     *
  *                                                                                *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. Gagnon  </center></h2>           *
  *                                                                                *
 \**********************************************************************************/
#ifndef ADC_H
#define ADC_H


/***********************************************************************************\
 *                                                                                 *
 *                              ADC FUNCTION PROTOTYPES                            *
 *                                                                                 *
\***********************************************************************************/
static void adc_pin_init(void);

static void adc_resurrect(void);

static void adc_configure(void);

static void adc_enable(void);

void adc_init(void);

#endif
