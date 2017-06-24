 /**********************************************************************************\
  * @file    I2C_PROJECT/src/main.c                                                *
  * @author  Nolan R. H. Gagnon                                                    *
  * @version V1.0                                                                  *
  * @date    23-June-2017                                                          *
  * @brief   Digital Clock.                                                        *
  *                                                                                *
  **********************************************************************************
  * @attention                                                                     *
  *                                                                                *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. H. Gagnon  </center></h2>        *
  *                                                                                *
 \**********************************************************************************/

/***********************************************************************************\
 *                                                                                 *
 *                                  INCLUDES                                       *
 *                                                                                 *
\***********************************************************************************/
#include "../include/i2c.h"
#include "../include/dma.h"
#include "../include/ds3231.h"
#include "../include/lcd.h"
#include "../include/ht16k33.h"
#include "../include/adc.h"
#include "../include/delay.h"
#include <stdio.h>

/***********************************************************************************\
 *                                                                                 *
 *                         PRIVATE FUNCTION PROTOTYPES                             *
 *                                                                                 *
\***********************************************************************************/ 
static void sysclk_init(void);
static void exti_pin_init(void);
static void button_pin_init(void);

/***********************************************************************************\
 *                                                                                 *
 *                                  ENUMS                                          *
 *                                                                                 *
\***********************************************************************************/ 
typedef enum
{
	time = 0,
	day = 1,
	month = 2,
	year = 3
} Display_TypeDef;

/***********************************************************************************\
 *                                                                                 *
 *                                  GLOBAL VARS                                    *
 *                                                                                 *
\***********************************************************************************/ 
static volatile uint8_t btn_presses = 0;
static volatile Display_TypeDef display_mode = time;
char days[7][4] = {"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};	
char months[12][4] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

static volatile DS3231_TypeDef rtc;
static volatile Date_TypeDef d;
static Alarm_TypeDef alrm;

static volatile char sev_seg_arr[10];

/***********************************************************************************\
 *                                                                                 *
 *                                  FUNCTIONS                                      *
 *                                                                                 *
\***********************************************************************************/ 

void main(void)
{
	char buff[6];

	sysclk_init();	
	button_pin_init();
	LCD_Initialization();
	LCD_Clear();
	i2c1_init();

	systick_init(16000);
	
	adc_init();
	
	dma_i2c_rx_init();
	dma_i2c_tx_init();
	
	d.second = (uint8_t) 0U;
	d.minute = (uint8_t) 25U;
	d.hour = (uint8_t) 20U;
	d.day = DS3231_DAYR_THUR;
	d.date = (uint8_t) 22U;
	d.month = DS3231_MONTHR_JUN;
	d.year = (uint16_t) 2017U;
	
	alrm.second = (uint8_t) 0U;
	alrm.minute = (uint8_t) 0U;
	alrm.hour = (uint8_t) 12U;
	alrm.day = DS3231_DAYR_FRI;
	alrm.date = (uint8_t) 13U;
	alrm.alrm_num = ALARM1;
	alrm.rate = PERMIN;
	
	DISPLAY_power_on();
	DISPLAY_set_brightness(HT16K33_DIMSETUP_DUTY13);
		
	//RTC_set_date(&d, &rtc);
	
	RTC_clear_interrupt_flag(&rtc, ALARM1);
	
	exti_pin_init();
	
	RTC_enable_interrupts(&rtc, &alrm);	

	RTC_read_date(&d, &rtc);	
	
	LCD_Clear();
	
	if (d.minute < 10) {
		if (d.hour < 10) 
			sprintf(buff, "0%d:0%d", d.hour, d.minute);	
		else
			sprintf(buff, "%d:0%d", d.hour, d.minute);
	} else {
		if (d.hour < 10) 
			sprintf(buff, "0%d:%d", d.hour, d.minute);
		else
			sprintf(buff, "%d:%d", d.hour, d.minute);
	}

	LCD_DisplayString((uint8_t *) buff);
	
	time_to_7seg(d.hour, d.minute, sev_seg_arr);
	
	DISPLAY_write_time(sev_seg_arr, 10);	
		
	ADC1->CR |= ADC_CR_ADSTART;		
	while(1);
}

/**
  * @brief Set SYSCLK to HSI16 (16 MHz clock)
  * @param None
  * @retval None
  */
static void sysclk_init(void)
{
    // Enable HSI16 clock (16 MHz)
    RCC->CR |= RCC_CR_HSION;

    // Wait until HSI16 is ready
    while (RCC->CR & RCC_CR_HSIRDY == 0);

    // Select HSI16 as SYSCLK 
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_HSI;

    // Wait until HSI16 is used as the system clock
    while (RCC->CFGR & RCC_CFGR_SWS == 0);
}

/**
  * @brief Configure the user button to generate interrupts
  *        when pressed.
  * @param None
  * @retval None
  */
void button_pin_init(void)
{
        // Enable Port A I/O Clock      
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

        // Configure PD0 as a digital input pin
        GPIOA->MODER &= ~GPIO_MODER_MODER0;  // bit field = 0b00

        // Enable the SYSCFG clock
        RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

        // Connect EXTI0 to PA0
        SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;
        SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;

        // Unmask interrupt request from line 0
        EXTI->IMR1 |= EXTI_IMR1_IM0;

        // Enable rising edge trigger for input line 0
        EXTI->RTSR1 |= EXTI_RTSR1_RT0;

        // Disable falling edge trigger for input line 0
        EXTI->FTSR1 &= ~EXTI_FTSR1_FT0;

        // Register EXTI0 interrupt handler with NVIC
        NVIC_SetPriority(EXTI0_IRQn, 0x03);
        NVIC_EnableIRQ(EXTI0_IRQn);
}

/**
  * @brief Configures PD0 for receiving interrupts
  *        generated by the TCS34725 RGB sensor.
  * @param None
  * @retval None
  */
static void exti_pin_init(void)
{
        // Enable Port A IO Clock       
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

        // Configure PA2 as a digital input pin
        GPIOA->MODER &= ~GPIO_MODER_MODER2;  // bit field = 0b00

        // Enable the SYSCFG clock
        RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

        // Connect EXTI0 to PD0
        SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI2;
        SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PA;

        // Unmask interrupt request from line 2
        EXTI->IMR1 |= EXTI_IMR1_IM2;

        // Disable rising edge trigger for input line 2
        EXTI->RTSR1 &= ~EXTI_RTSR1_RT2;

        // Enable falling edge trigger for input line 2 
        EXTI->FTSR1 |= EXTI_FTSR1_FT2;

        // Register EXTI2 interrupt handler with NVIC
        NVIC_SetPriority(EXTI2_IRQn, 0x03);
        NVIC_EnableIRQ(EXTI2_IRQn);
}

/**
  * @brief Handle interrupts generated by the button
  * @param None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
	char buff[6];

        EXTI->PR1 |= EXTI_PR1_PIF0;
	
	btn_presses = (btn_presses + 1) % 4;
	
        // Wait for the button to be released   
        while (GPIOA->IDR & GPIO_IDR_IDR_0);
	
	LCD_Clear();
		
	switch (btn_presses) {
	case 0:
		if (d.minute < 10) {
			if (d.hour < 10)
				sprintf(buff, "0%d:0%d", d.hour, d.minute);
			else
				sprintf(buff, "%d:0%d", d.hour, d.minute);
		} else {
			if (d.hour < 10)
				sprintf(buff, "0%d:%d", d.hour, d.minute);
			else
				sprintf(buff, "%d:%d", d.hour, d.minute);
		}

		break;
	case 1:	
		sprintf(buff, "%s", *(days + (d.day - 1)));
		break;
	case 2:
		sprintf(buff, "%s %d", *(months + (d.month - 1)), d.date);	
		break;
	case 3:
		sprintf(buff, "%d", d.year);
		break;
	default:
		sprintf(buff, "ERROR");
		break;
	
	}	
	
        LCD_DisplayString((uint8_t *) buff);
}

/**
  * @brief Handle interrupts generated by the TCS34725 
  *        RGB sensor.
  * @param None
  * @retval None
  */
void EXTI2_IRQHandler(void)
{
	char buff[6];

        EXTI->PR1 |= EXTI_PR1_PIF2;
	RTC_clear_interrupt_flag(&rtc, ALARM1);

	RTC_read_date(&d, &rtc);	
	
	LCD_Clear();

	if (d.minute < 10) {
		if (d.hour < 10) 
			sprintf(buff, "0%d:0%d", d.hour, d.minute);	
		else
			sprintf(buff, "%d:0%d", d.hour, d.minute);
	} else {
		if (d.hour < 10) 
			sprintf(buff, "0%d:%d", d.hour, d.minute);
		else
			sprintf(buff, "%d:%d", d.hour, d.minute);
	}

	LCD_DisplayString((uint8_t *) buff);

	time_to_7seg(d.hour, d.minute, sev_seg_arr);
	DISPLAY_write_time(sev_seg_arr, 10);
}
