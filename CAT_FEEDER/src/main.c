/* Includes ----------------------------------------------------------------------*/
#include <stm32l476xx.h>
#include "../include/servo.h"
#include "../include/delay.h"

/* Private defines ---------------------------------------------------------------*/
#define PA1_AF2_TIM5_CH2        ((uint32_t) 0x02 << (4 * 1))
#define PB3_AF1_TIM2_CH2	((uint32_t) 0x01 << (4 * 3))
#define CK_PSC_NODIV		((uint16_t) 0x00)

volatile uint8_t homed = 1;

/* Private function prototypes ---------------------------------------------------*/
static void sysclk_init(void);
static void pa1_pwm_config(void);
static void pb3_pwm_config(void);
static void tim5_ch2_config(void);
static void tim2_ch2_config(void);
static void button_pin_init(void);
/* Private functions -------------------------------------------------------------*/

/**
  * @brief Entry point (main program)
  * @param None
  * @retval None
  */
void main(void)
{	
	size_t i;
	sysclk_init();  // Setup SYSCLK (HSI16)
	pa1_pwm_config();  // Connect PA1 to TIM5_CH2 PWM output 
	pb3_pwm_config();  // Connect PB3 to TIM2_CH2 PWM output
	tim2_ch2_config();  // Setup and start TIM5_CH2
	tim5_ch2_config();  // Setup and start TIM2_CH4
	button_pin_init();  // Configure button
	systick_init(16000);
	
	while(1) {
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
  * @brief Connect PA1 to TIM5_CH2 for PWM output
  * @param None
  * @retval None
  */
static void pa1_pwm_config(void)
{
    // Clock I/O port A
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    
    // Configure PA1 for alternative function use (TIM5_CH2 : alt. func. 2)
    GPIOA->MODER &= ~GPIO_MODER_MODER1;
    GPIOA->MODER |= GPIO_MODER_MODER1_1;
    GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL1;
    GPIOA->AFR[0] |= PA1_AF2_TIM5_CH2;
    
    // No pull on PA1
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR1;
    
    // Configure PA1 for very high output speed
    GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR1;
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR1_0 | GPIO_OSPEEDER_OSPEEDR1_1;
}

/**
  * @brief Connect PB3 to TIM2_CH2 for PWM output
  * @param None
  * @retval None
  */
static void pb3_pwm_config(void)
{
    // Clock I/O port B
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    
    // Configure PB3 for alternative function use (TIM2_CH2 : alt. func. 1)
    GPIOB->MODER &= ~GPIO_MODER_MODER3;
    GPIOB->MODER |= GPIO_MODER_MODER3_1;
    GPIOB->AFR[0] &= ~GPIO_AFRL_AFRL3;
    GPIOB->AFR[0] |= PB3_AF1_TIM2_CH2;
    
    // No pull on PB3
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR3;
    
    // Configure PB3 for very high output speed
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
  * @brief Configures TIM2_CH2 in PWM Mode 1:
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
	TIM2->ARR = 800;  /* Data bit signal duration = (1.0 / 40 kHz) * 800 = 20 milliseconds : */
				 
	TIM2->CCR2 = 60;  // Set capture and compare register to 0
	
	// Configure the counter as an up-counter
	TIM2->CR1 &= ~TIM_CR1_DIR;
			
	/* Configure in PWM Mode 1:
	   PWM signal is high as long as TIM2_CNT < TIM2_CCR2.
	   It is low otherwise. */
	TIM2->CCMR1 &= ~TIM_CCMR1_OC2M;
	TIM2->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;  // OC1M bit field = 0b0110
	
	/* Enable output compare 2 preload :
		DMA write operations write to the preload register (NOT TIM2_CCR2).
		TIM1_CCR1 active register is written ONLY AT UPDATE EVENTS (i.e., counter overflow) */
	TIM2->CCMR1 |= TIM_CCMR1_OC2PE; 
	
	/* Enable complementary output signal OC2 :
		Set because PB3 is connected to TIM2_CH2. */
	TIM2->CCER |= TIM_CCER_CC2E;

	// Enable main output
	TIM2->BDTR |= TIM_BDTR_MOE;
	
	// Enable TIM2
	TIM2->CR1 |= TIM_CR1_CEN;
}

/**
  * @brief Configures TIM5_CH2 in PWM Mode 1:
  *        Channel 2 is active as long as TIM5_CNT < TIM5_CCR2;
  *        otherwise, it is inactive.
  * @param None
  * @retval None
  */
static void tim5_ch2_config(void)
{
	// Clock the TIM5 peripheral
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM5EN;
	
	// Setup input clock divider and auto-reload register
	TIM5->PSC = 400 - 1;  // CK_CNT freq. = (16 MHz / 400) = 40 kHz 
	TIM5->ARR = 800;  /* Data bit signal duration = (1.0 / 40 kHz) * 800 = 20 milliseconds */
				 
	TIM5->CCR2 = 60;  // Set capture and compare register to 0
	
	// Configure the counter as an up-counter
	TIM5->CR1 &= ~TIM_CR1_DIR;
			
	/* Configure in PWM Mode 1:
	   PWM signal is high as long as TIM5_CNT < TIM5_CCR2.
	   It is low otherwise. */
	TIM5->CCMR1 &= ~TIM_CCMR1_OC2M;
	TIM5->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;  // OC1M bit field = 0b0110
	
	/* Enable output compare 1 preload :
		DMA write operations write to the preload register (NOT TIM5_CCR2).
		TIM5_CCR2 active register is written ONLY AT UPDATE EVENTS (i.e., counter overflow) */
	TIM5->CCMR1 |= TIM_CCMR1_OC2PE; 
	
	/* Enable complementary output signal OC2 :
		Set because PA1 is connected to TIM5_CH2. */
	TIM5->CCER |= TIM_CCER_CC2E;

	// Enable main output
	TIM5->BDTR |= TIM_BDTR_MOE;
	
	// Enable TIM5
	TIM5->CR1 |= TIM_CR1_CEN;
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
  * @brief Handle interrupts generated by the button
  * @param None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{       
	size_t i;

        EXTI->PR1 |= EXTI_PR1_PIF0;
        
        // Wait for the button to be released   
        while (GPIOA->IDR & GPIO_IDR_IDR_0);
	
	if (homed == 1)	{
		servo_push();
		homed = 0;
	} else {
		home_servo2();
		home_servo1();
		homed = 1;
	}
	
}
/**** END OF FILE ****/
