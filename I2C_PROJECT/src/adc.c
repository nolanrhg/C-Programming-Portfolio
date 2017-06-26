/**********************************************************************************\
 * @file    I2C_PROJECT/include/adc.c                                             *
 * @author  Nolan R. H. Gagnon                                                    *
 * @version V2.0                                                                  *
 * @date    26-June-2017                                                          *
 * @brief   A/D converter driver.                                                 *
 *                                                                                *
 **********************************************************************************
 * @attention                                                                     *
 *                                                                                *
 * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. H. Gagnon  </center></h2>        *
 *                                                                                *
\**********************************************************************************/

/**********************************************************************************\
 *                                                                                *
 *                                  INCLUDES                                      *
 *                                                                                *
\**********************************************************************************/
#include "../include/adc.h"
#include "../include/delay.h"
#include "../include/ht16k33.h"
#include "../include/timers.h"

/**********************************************************************************\
 *                                                                                *
 *                                  GLOBAL VARS                                   *
 *                                                                                *
\**********************************************************************************/
static volatile uint32_t prev_adconvs[MAX_REPEAT_ADCONV] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/**********************************************************************************\
 *                                                                                *
 *                              ADC FUNCTIONS                                     *
 *                                                                                *
\**********************************************************************************/
static void adc1_pin_init(void)
{
	/* Configure PA1 as analog input */
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;	
	GPIOA->MODER &= ~GPIO_MODER_MODER1;	
	GPIOA->MODER |= GPIO_MODER_MODER1;  // 0b11

	/* Connect PA1 to ADC input */
	GPIOA->ASCR |= GPIO_ASCR_EN_1;	
}

static void adc1_resurrect(void)
{		
	/* Exit deep-power-down mode */
	ADC1->CR &= ~ADC_CR_DEEPPWD;
	
	/* Enable internal voltage regulator */		
	ADC1->CR |= ADC_CR_ADVREGEN;	

	/* Wait for the voltage regulator to start up */
	delay(1);  // 1 ms delay is enough : max. startup time is 20 microseconds
}

static void adc1_configure(void)
{
	/* Clock the ADC peripheral */
        RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;

        /* Ensure ADC1 is disabled before configuring */
        ADC1->CR &= ~ADC_CR_ADEN;      
	
	/* Setup ADC input clock */
        ADC123_COMMON->CCR &= ~ADC_CCR_PRESC;  // Clock input to ADC not divided (f_ADC = f_SYSCLK)
        ADC123_COMMON->CCR &= ~ADC_CCR_CKMODE;
        ADC123_COMMON->CCR |= ADC_CCR_CKMODE_0;  // Synchronous clock mode
        ADC123_COMMON->CCR &= ~ADC_CCR_DUAL;	// All ADCs act independently

 	/* Bring ADC1 out of deep-power-down mode */
        adc1_resurrect();

        /* Use 12-bit resolution for conversions */
        ADC1->CFGR &= ~ADC_CFGR_RES;

        /* Data right-aligned in data register */ 
        ADC1->CFGR &= ~ADC_CFGR_ALIGN;
	
	/* Convert only channel 6 input */	
        ADC1->DIFSEL &= ~ADC_DIFSEL_DIFSEL;  // Use single-ended mode
        ADC1->SQR1 &= ~ADC_SQR1_L;  // Only ONE conversion in the regular sequence (i.e., channel 6)
        ADC1->SQR1 &= ~ADC_SQR1_SQ1;
        ADC1->SQR1 |= ADC_SQR1_SQ1_1 | ADC_SQR1_SQ1_2;  // Make channel 6 the only converted channel in the sequence
        ADC1->SMPR1 |= ADC_SMPR1_SMP6;  // Sample time = 640.5 ADC clock cycles

        /* Generate interrupt at the end of conversion */ 
        ADC1->IER |= ADC_IER_EOC;
        NVIC_SetPriority(ADC1_2_IRQn, 10);  // Low priority
        NVIC_EnableIRQ(ADC1_2_IRQn);
	
        /* Trigger A/D conversions on the rising edge of TIM1_CH2 PWM waveform */
        ADC1->CFGR |= ADCONV_TRIG_RISEDGE;
	ADC1->CFGR |= ADCONV_TRIG_TIM1CH2;
}

static void adc1_enable(void)
{
	/* Enable the ADC : Refer to page 440 of the ref. manual */
	ADC1->ISR |= ADC_ISR_ADRDY;  // ADRDY bit is cleared by writing 1
	ADC1->CR |= ADC_CR_ADEN;  // Enable the ADC
	while (ADC1->ISR & ADC_ISR_ADRDY == 0);  // Wait until the ADC is ready
	ADC1->ISR |= ADC_ISR_ADRDY;  // ADRDY bit is cleared by writing 1
}

void adc1_init(void)
{
	/* Configure PA1 for analog input */
	adc1_pin_init();
	
	/* Configure ADC1 settings */
	adc1_configure();

	/* Turn on ADC1 */
	adc1_enable();
	
	/* Turn on the timer driving the conversions */
	tim1_ch2_init();
}

void ADC1_2_IRQHandler(void)
{
	if (NODE_VOLTAGE > 0x000U && NODE_VOLTAGE < 0x0FFU) {

		DISPLAY_set_brightness(HT16K33_DIMSETUP_DUTY1);

	} else if (NODE_VOLTAGE > 0x0FFU && NODE_VOLTAGE < 0x1FEU) {

		DISPLAY_set_brightness(HT16K33_DIMSETUP_DUTY2);

	} else if (NODE_VOLTAGE > 0x1FEU && NODE_VOLTAGE < 0x2FDU) {

		DISPLAY_set_brightness(HT16K33_DIMSETUP_DUTY3);

	} else if (NODE_VOLTAGE > 0x2FDU && NODE_VOLTAGE < 0x3FCU) {

		DISPLAY_set_brightness(HT16K33_DIMSETUP_DUTY4);

	} else if (NODE_VOLTAGE > 0x3FCU && NODE_VOLTAGE < 0x4FBU) {

		DISPLAY_set_brightness(HT16K33_DIMSETUP_DUTY5);

	} else if (NODE_VOLTAGE > 0x4FBU && NODE_VOLTAGE < 0x5FAU) {

		DISPLAY_set_brightness(HT16K33_DIMSETUP_DUTY6);

	} else if (NODE_VOLTAGE > 0x5FAU && NODE_VOLTAGE < 0x6F9U) {

		DISPLAY_set_brightness(HT16K33_DIMSETUP_DUTY7);

	} else if (NODE_VOLTAGE > 0x6F9U && NODE_VOLTAGE < 0x7F8U) {

		DISPLAY_set_brightness(HT16K33_DIMSETUP_DUTY8);

	} else if (NODE_VOLTAGE > 0x7F8U && NODE_VOLTAGE < 0x8F7U) {

		DISPLAY_set_brightness(HT16K33_DIMSETUP_DUTY9);

	} else if (NODE_VOLTAGE > 0x8F7U && NODE_VOLTAGE < 0x9F6U) {

		DISPLAY_set_brightness(HT16K33_DIMSETUP_DUTY10);

	} else if (NODE_VOLTAGE > 0x9F6U && NODE_VOLTAGE < 0xAF5U) {

		DISPLAY_set_brightness(HT16K33_DIMSETUP_DUTY11);

	} else if (NODE_VOLTAGE > 0xAF5U && NODE_VOLTAGE < 0xBF4U) {

		DISPLAY_set_brightness(HT16K33_DIMSETUP_DUTY12);

	} else if (NODE_VOLTAGE > 0xBF4U && NODE_VOLTAGE < 0xCF3U) {

		DISPLAY_set_brightness(HT16K33_DIMSETUP_DUTY13);

	} else if (NODE_VOLTAGE > 0xCF3U && NODE_VOLTAGE < 0xDF2U) {

		DISPLAY_set_brightness(HT16K33_DIMSETUP_DUTY14);

	} else if (NODE_VOLTAGE > 0xDF2U && NODE_VOLTAGE < 0xEF1U) {

		DISPLAY_set_brightness(HT16K33_DIMSETUP_DUTY15);

	} else if (NODE_VOLTAGE > 0xEF1U && NODE_VOLTAGE < 0xFFFU) {

		DISPLAY_set_brightness(HT16K33_DIMSETUP_DUTY16);

	}

	ADC1->ISR |= ADC_ISR_EOC;  // Clear flag
}
