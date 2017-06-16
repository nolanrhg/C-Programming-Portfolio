/**
  **********************************************************************************
  * @file    RGB_SENSOR/src/main.c
  * @author  Nrgagnon 
  * @version V1.6
  * @date    28-May-2017
  * @brief   
  *
  **********************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. H. Gagnon </center></h2>
  *
  **********************************************************************************
  */

/* Includes ----------------------------------------------------------------------*/
#include <stm32l476xx.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/dma.h"
#include "../include/i2c.h"
#include "../include/led.h"
#include "../include/tcs34725.h"
#include "../include/lcd.h"
#include "../include/delay.h"
#include "../include/color_processing.h"

/* Private defines --------------------------------------------------------------*/
#define PE8_AF1_TIM1_CH1N       ((uint32_t) 0x01)
#define CK_PSC_NODIV            ((uint16_t) 0x00)
/* Private function prototypes --------------------------------------------------*/
static void exti_pin_init(void);
static void sysclk_init(void);
static void pwm_pin_config(void);
static void tim1_config(void);
static void led_ctrl_pin_init(void);
static void button_pin_init(void);
static void snsr_pwr_pin_init(void);
static void snsr_pwr_on(void);
/* Global variables -------------------------------------------------------------*/
volatile uint32_t strip_color = 0x000000;

volatile uint8_t color_reg_base[1] = {TCS_CMDR_PTR | TCS_CMDR_AUTOINC_PROT | TCS_CDATA_LBR_PTR};

uint16_t *ccr_buff_ptr;
volatile RGB_Sensor_TypeDef *tcs34725;
volatile uint8_t button_pressed = 0;

/* Private functions ------------------------------------------------------------*/

/**
  * @brief Entry point (main program)
  * @param None
  * @retval None
  */
int main(void)
{
	size_t i;
	sysclk_init();  // SYSCLK_freq = 16 MHz
	led_ctrl_pin_init();  // Initialize pin to control LED on TCS34725
	button_pin_init();  // Initialize user button
	pwm_pin_config();  // Initialize PE8
	i2c1_init();  // Initialize I2C1 peripheral
	tim1_config(); // Initialize TIM1 peripheral
	LCD_Initialization();
	LCD_Clear();

	// Allocate memory for TIM1_CCR1 values	
	ccr_buff_ptr = (uint16_t *) calloc(NUM_PERIODS_FOR_RESET +
					  (NUM_LEDS * NUM_COLOR_BITS), sizeof(uint16_t));
	if (ccr_buff_ptr == NULL) {  // Error checking
		perror("calloc");	
		exit(1);
	}
	
	// Allocate memory for a struct that represents the TCS34725	
	tcs34725 = (RGB_Sensor_TypeDef *) malloc(sizeof(RGB_Sensor_TypeDef));
	if (tcs34725 == NULL) {  // Error checking
		perror("malloc");
		exit(1);
	}
	
	// Allocate memory to store the color data returned by the TCS34725
	tcs34725->COLRDATA = (uint8_t *) calloc(8, sizeof(uint8_t));
	if (tcs34725->COLRDATA == NULL) {  // Error checking
		perror("malloc");
		exit(1);
	}
	
	/* Setup DMA channels */
	dma_i2c_rx2mem_init();  // Channel between I2C1_RXDR and memory
	dma_i2c_mem2tx_init();  // Channel between memory and I2C1_TXDR
	dma_mem2tim1_init(ccr_buff_ptr);  // Channel between memory and TIM1_CCR1
	
	/* Setup the TCS34725 RGB sensor */
	snsr_pwr_pin_init();
	snsr_pwr_on();
	rgb_sensor_init(tcs34725);  // Power on and configure sensor
	rgb_sensor_threshold_calibration(tcs34725);  // Calibrate sensor
	
	/* Play system start-up light show */
	start_sequence(ccr_buff_ptr);

	/* Configure PA1 to trigger interrupts when rgb data from 
	   the sensor is ready */
	exti_pin_init();
	
	/* Run the program forever, waiting for the sensor
	   to provide new color data */
	while (1) {
	}
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
  * @brief Configure PA2 for controlling the
  *        TCS34725 LED.
  * @param None
  * @retval None
  */
void led_ctrl_pin_init(void)
{
	// Enable IO Port A
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

	// Configure PA2 as digital output		
	GPIOA->MODER &= ~GPIO_MODER_MODER2;
	GPIOA->MODER |= GPIO_MODER_MODER2_0;

	// Configure PA2 as open-drain
	GPIOA->OTYPER |= GPIO_OTYPER_OT_2;

	// Turn LED off 
	GPIOA->ODR &= ~GPIO_ODR_ODR_2;
}

/**
  * @brief Configure PB3 for controlling
  *	   power to the TCS34725.
  * @param None
  * @retval None
  */
void snsr_pwr_pin_init(void)
{
	// Enable IO Port B
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

	// Configure PB3 as digital output
	GPIOB->MODER &= ~GPIO_MODER_MODER3;
	GPIOB->MODER |= GPIO_MODER_MODER3_0;
}

/**
  * @brief Reset the registers of the TCS34725
  * @param None
  * @retval None
  */
void snsr_pwr_on(void)
{
	GPIOB->ODR |= GPIO_ODR_ODR_3;
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
  * @brief Configures PD0 for receiving interrupts
  *	   generated by the TCS34725 RGB sensor.
  * @param None
  * @retval None
  */
static void exti_pin_init(void)
{
	// Enable Port D IO Clock	
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

	// Configure PD0 as a digital input pin
	GPIOA->MODER &= ~GPIO_MODER_MODER1;  // bit field = 0b00

	// Enable the SYSCFG clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// Connect EXTI0 to PD0
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI1;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PA;
	
	// Unmask interrupt request from line 1
	EXTI->IMR1 |= EXTI_IMR1_IM1;

	// Disable rising edge trigger for input line 1
	EXTI->RTSR1 &= ~EXTI_RTSR1_RT1;

	// Enable falling edge trigger for input line 1
	EXTI->FTSR1 |= EXTI_FTSR1_FT1;
	
	// Register EXTI0 interrupt handler with NVIC
	NVIC_SetPriority(EXTI1_IRQn, 0x03);
	NVIC_EnableIRQ(EXTI1_IRQn);
}

/**
  * @brief Connect PE8 to TIM1_CH1N for PWM output.
  * @param None
  * @retval None
  */
static void pwm_pin_config(void)
{   
    // Clock I/O port E
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
    
    // Configure PE8 for alternative function use (TIM1_CH1N : alt. func. 1)
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
  * @brief Configure TIM1 for generating a PWM
  *        waveform on PE8.
  * @param None
  * @retval None
  */
static void tim1_config(void)
{
        // Clock the TIM1 peripheral
        RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

        // Setup input clock divider and auto-reload register
        TIM1->PSC = CK_PSC_NODIV;  // f_CKCNT = f_SYSCLK (no clock division) 
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
  * @brief Handle interrupts generated by the button
  * @param None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
	EXTI->PR1 |= EXTI_PR1_PIF0;

	// Wait for the button to be released	
	while (GPIOA->IDR & GPIO_IDR_IDR_0);	

	// Turn the LED on
	GPIOA->ODR |= GPIO_ODR_ODR_2;
	
	button_pressed = 1;
}

/**
  * @brief Handle interrupts generated by the TCS34725 
  *	   RGB sensor.
  * @param None
  * @retval None
  */
void EXTI1_IRQHandler(void)
{
	uint8_t *enh_colrs;

	EXTI->PR1 |= EXTI_PR1_PIF1;	
	tcs34725_interrupt_clr();
	
	/* If the user button was pressed, read the 
 		sensor data and update the LEDs*/	
	if (button_pressed) {
		// Turn off the LED	
		GPIOA->ODR &= ~GPIO_ODR_ODR_2;
	
		// Read color data from the rgb sensor
		i2c1_read(8, TCS_I2C_ADDR, color_reg_base, tcs34725->COLRDATA);  // Get clear data
		
		// Enhance the raw color data returned by the sensor	
		enh_colrs = enhance_color(tcs34725->COLRDATA);	
	
		// Update the LEDs	
		strip_color = (enh_colrs[0] << 16) | (enh_colrs[1] << 8) | (enh_colrs[2] << 0);
		color_update(ccr_buff_ptr, strip_color);
		
		free(enh_colrs);
	
		button_pressed = 0;

	/* Otherwise, the sensor threshold levels
		must be adjusted */
	} else {
	}
}
