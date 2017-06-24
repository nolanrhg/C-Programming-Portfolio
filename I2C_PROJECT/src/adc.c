 /**********************************************************************************\
  * @file    I2C_PROJECT/include/adc.c                                             *
  * @author  Nolan R. H. Gagnon                                                    *
  * @version V1.0                                                                  *
  * @date    24-June-2017                                                          *
  * @brief   A/D converter driver.                                                 *
  *                                                                                *
  **********************************************************************************
  * @attention                                                                     *
  *                                                                                *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. H. Gagnon  </center></h2>        *
  *                                                                                *
 \**********************************************************************************/
#include "../include/adc.h"
#include "../include/delay.h"
#include "../include/ht16k33.h"

/***********************************************************************************\
 *                                                                                 *
 *                              ADC FUNCTIONS                                      *
 *                                                                                 *
\***********************************************************************************/
static void adc_pin_init(void)
{
	/* Configure PA1 as analog input */
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;	
	GPIOA->MODER &= ~GPIO_MODER_MODER1;	
	GPIOA->MODER |= GPIO_MODER_MODER1;  // 0b11

	/* Connect PA1 to ADC input */
	GPIOA->ASCR |= GPIO_ASCR_EN_1;	
}

static void adc_resurrect(void)
{		
	/* Exit deep-power-down mode */
	ADC1->CR &= ~ADC_CR_DEEPPWD;
	
	/* Enable internal voltage regulator */		
	ADC1->CR |= ADC_CR_ADVREGEN;	

	/* Wait for the voltage regulator to start up */
	delay(1);  // 1 ms delay is enough : max. startup time is 20 microseconds
}

static void adc_configure(void)
{
	/* Turn on ADC clock */	
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;

	/* Ensure the ADC is disabled before configuring it */	
	ADC1->CR &= ~ADC_CR_ADEN;
	
	ADC123_COMMON->CCR |= ADC_CCR_VREFEN;
	ADC123_COMMON->CCR &= ~ADC_CCR_PRESC;
	ADC123_COMMON->CCR &= ~ADC_CCR_CKMODE;
	ADC123_COMMON->CCR |= ADC_CCR_CKMODE_0;
	ADC123_COMMON->CCR &= ~ADC_CCR_DUAL;

	/* Wake up ADC1 */
	adc_resurrect();	

	ADC1->CFGR &= ~ADC_CFGR_RES;  // 12-bit resolution
	ADC1->CFGR &= ~ADC_CFGR_ALIGN;	// right-alignment

	ADC1->SQR1 &= ~ADC_SQR1_L;	
	
        ADC1->SQR1 &= ~ADC_SQR1_SQ1;
        ADC1->SQR1 |= ADC_SQR1_SQ1_1 | ADC_SQR1_SQ1_2;

	ADC1->DIFSEL &= ~ADC_DIFSEL_DIFSEL;

	ADC1->SMPR1 &= ~ADC_SMPR1_SMP6;
        ADC1->SMPR1 |= ADC_SMPR1_SMP6;	
	
        // Enable interrupts
        ADC1->IER |= ADC_IER_EOC;
        NVIC_SetPriority(ADC1_2_IRQn, 1);
        NVIC_EnableIRQ(ADC1_2_IRQn);
	ADC1->CFGR |= ADC_CFGR_CONT;
	
        //Select software trigger
        ADC1->CFGR &= ~ADC_CFGR_EXTEN;
}

static void adc_enable(void)
{
	/* Enable the ADC : Refer to page 440 of the ref. manual */
	ADC1->ISR |= ADC_ISR_ADRDY;  // ADRDY bit is cleared by writing 1
	ADC1->CR |= ADC_CR_ADEN;  // Enable the ADC
	while (ADC1->ISR & ADC_ISR_ADRDY == 0);  // Wait until the ADC is ready
	ADC1->ISR |= ADC_ISR_ADRDY;  // ADRDY bit is cleared by writing 1
}

void adc_init(void)
{
	adc_pin_init();
	adc_configure();
	adc_enable();
}

void ADC1_2_IRQHandler(void)
{
         if(ADC1->DR > 2590){
		DISPLAY_set_blink_freq(HT16K33_DSETUP_BLK1HZ);
         } else {
		DISPLAY_set_blink_freq(HT16K33_DSETUP_BLKOFF);
         }
}
