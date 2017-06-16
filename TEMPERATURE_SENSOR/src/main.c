/* Includes ----------------------------------------------------------------------*/
#include <stm32l476xx.h>
#include "../include/tc74_led.h"
#include "../include/tc74_dma.h"
#include "../include/tc74_i2c.h"
#include "../include/tc74_lcd.h"
#include "../include/tc74_funcs.h"
#include "../include/servo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* Private defines ---------------------------------------------------------------*/
#define PE8_AF1_TIM1_CH1N       ((uint32_t) 0x01 << (4 * 0))
#define PB3_AF1_TIM2_CH2	((uint32_t) 0x01 << (4 * 3))
#define CK_PSC_NODIV		((uint16_t) 0x00)

/* Private function prototypes ---------------------------------------------------*/
static void sysclk_init(void);
static void pe8_pwm_config(void);
static void pb3_pwm_config(void);
static void tim1_ch1n_config(void);
static void tim2_ch2_config(void);

/* Private functions -------------------------------------------------------------*/

/**
  * @brief Entry point (main program)
  * @param None
  * @retval None
  */
void main(void)
{	
	uint16_t *ccr_buff_ptr;
	int8_t temperature;
	char t_string[6];
	size_t i;

	ccr_buff_ptr = (uint16_t *) calloc(NUM_PERIODS_FOR_RESET + 
					  (NUM_LEDS * NUM_COLOR_BITS), sizeof(uint16_t));
	
	if (ccr_buff_ptr == NULL) {
		perror("calloc");
		exit(1);
	}

	sysclk_init();  // Setup SYSCLK (HSI16)
	pe8_pwm_config();  // Connect PE8 to TIM1_CH1N PWM output 
	pb3_pwm_config();  // Connect PB3 to TIM2_CH2
	tim1_ch1n_config();  // Setup and start TIM1_CH1N
	tim2_ch2_config();  // Setup and start TIM2_CH4
	i2c1_init();
	LCD_Initialization();
	LCD_Clear();
	
	/* Setup DMA channels */
	dma_i2c_rx2mem_init();
	dma_i2c_mem2tx_init();
	dma_mem2tim1_init(ccr_buff_ptr);
	
	tc74_startup(ccr_buff_ptr);	

	while(1) {
		temperature = read_tc74_temp();	
		set_color_with_temp(ccr_buff_ptr, temperature);
		if (temperature > 26) {
			rotate_servo_right();
		} else {
			stop_servo();
		}

		sprintf(t_string, "%d C", temperature);	
		LCD_DisplayString((uint8_t *)t_string);

		for (i = 0; i < 5000000; i++);
		LCD_Clear();
	}
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
  * @brief Connect PE8 to TIM1_CH1N for PWM output
  * @param None
  * @retval None
  */
static void pe8_pwm_config(void)
{
    // Clock I/O port E
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
    
    // Configure pin E8 for alternative function use (TIM1_CH1N : alt. func. 1)
    GPIOE->MODER &= ~GPIO_MODER_MODER8;
    GPIOE->MODER |= GPIO_MODER_MODER8_1;
    GPIOE->AFR[1] &= ~GPIO_AFRH_AFRH0;
    GPIOE->AFR[1] |= PE8_AF1_TIM1_CH1N;
    
    // No pull on PE8
    GPIOE->PUPDR &= ~GPIO_PUPDR_PUPDR8;
    
    // Configure PE8 for very high output speed
    GPIOE->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR8;
    GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR8_0 | GPIO_OSPEEDER_OSPEEDR8_1;
}

/**
  * @brief Connect PE10 to TIM1_CH2N for PWM output
  * @param None
  * @retval None
  */
static void pb3_pwm_config(void)
{
    // Clock I/O port E
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    
    // Configure PE10 for alternative function use (TIM1_CH2N : alt. func. 1)
    GPIOB->MODER &= ~GPIO_MODER_MODER3;
    GPIOB->MODER |= GPIO_MODER_MODER3_1;
    GPIOB->AFR[0] &= ~GPIO_AFRL_AFRL3;
    GPIOB->AFR[0] |= PB3_AF1_TIM2_CH2;
    
    // No pull on PE10
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR3;
    
    // Configure PE10 for very high output speed
    GPIOB->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR3;
    GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3_0 | GPIO_OSPEEDER_OSPEEDR3_1;
}

/**
  * @brief Configures TIM1_CH1N in PWM Mode 1:
  *        Channel 1 is active as long as TIM1_CNT < TIM1_CCR1;
  *        otherwise, it is inactive.
  * @param None
  * @retval None
  */
static void tim1_ch1n_config(void)
{
	// Clock the TIM1 peripheral
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; 
	
	// Setup input clock divider and auto-reload register
	TIM1->PSC = CK_PSC_NODIV;  // CK_CNT freq. = SYSCLK freq. (no clock division) 
	TIM1->ARR = 20;  /* Data bit signal duration = (1.0 / 16E6) * 20 = 1.25 micro-seconds :
			    Required by the WS2812 LEDs (refer to pg. 4 of datasheet) */
				 
	TIM1->CCR1 = 0;  // Set capture and compare register to 0
	
	// Configure the counter as an up-counter
	TIM1->CR1 &= ~TIM_CR1_DIR;
			
	/* Configure in PWM Mode 1:
	   PWM signal is high as long as TIM1_CNT < TIM1_CCR1.
	   It is low otherwise. */
	TIM1->CCMR1 &= ~TIM_CCMR1_OC1M;
	TIM1->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;  // OC1M bit field = 0b0110
	
	/* Enable output compare 1 preload :
		DMA write operations write to the preload register (NOT TIM1_CCR1).
		TIM1_CCR1 active register is written ONLY AT UPDATE EVENTS (i.e., counter overflow) */
	TIM1->CCMR1 |= TIM_CCMR1_OC1PE; 
	
	/* Enable complementary output signal OC1N :
		Set because PE8 is connected to TIM1_CH1N. */
	TIM1->CCER |= TIM_CCER_CC1NE;

	// Allow TIM1 to make DMA requests when CC1 event occurs
	TIM1->DIER |= TIM_DIER_CC1DE;  // Enable DMA req.
	TIM1->CR2 &= ~TIM_CR2_CCDS;  // Make req. when CC1 event occurs

	// Enable main output
	TIM1->BDTR |= TIM_BDTR_MOE;
	
	// Enable TIM1
	TIM1->CR1 |= TIM_CR1_CEN;
}

/**
  * @brief Configures TIM2_CH4 in PWM Mode 1:
  *        Channel 1 is active as long as TIM2_CNT < TIM2_CCR1;
  *        otherwise, it is inactive.
  * @param None
  * @retval None
  */
static void tim2_ch2_config(void)
{
	// Clock the TIM2 peripheral
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN; 
	
	// Setup input clock divider and auto-reload register
	TIM2->PSC = 400 - 1;  // CK_CNT freq. = (16 MHz / 400) = 40 kHz 
	TIM2->ARR = 800;  /* Data bit signal duration = (1.0 / 40 kHz) * 800 = 20 milliseconds :
			    Required by the WS2812 LEDs (refer to pg. 4 of datasheet) */
				 
	TIM2->CCR2 = 60;  // Set capture and compare register to 0
	
	// Configure the counter as an up-counter
	TIM2->CR1 &= ~TIM_CR1_DIR;
			
	/* Configure in PWM Mode 1:
	   PWM signal is high as long as TIM1_CNT < TIM1_CCR1.
	   It is low otherwise. */
	TIM2->CCMR1 &= ~TIM_CCMR1_OC2M;
	TIM2->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;  // OC1M bit field = 0b0110
	
	/* Enable output compare 1 preload :
		DMA write operations write to the preload register (NOT TIM1_CCR1).
		TIM1_CCR1 active register is written ONLY AT UPDATE EVENTS (i.e., counter overflow) */
	TIM2->CCMR1 |= TIM_CCMR1_OC2PE; 
	
	/* Enable complementary output signal OC1N :
		Set because PE8 is connected to TIM1_CH1N. */
	TIM2->CCER |= TIM_CCER_CC2E;

	// Enable main output
	TIM2->BDTR |= TIM_BDTR_MOE;
	
	// Enable TIM1
	TIM2->CR1 |= TIM_CR1_CEN;
}
/**** END OF FILE ****/
