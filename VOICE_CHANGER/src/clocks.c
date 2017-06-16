/**
  **********************************************************************************
  * @file    VOICE_CHANGER/src/clocks.c                                            *
  * @author  Nolan R. Gagnon                                                       *
  * @version V1.0                                                                  *
  * @date    15-June-2017                                                          *
  * @brief   Implementation system clock functions.                                *
  *                                                                                *
  **********************************************************************************
  * @attention								 	   *
  *									  	   *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. Gagnon </center></h2>	           *
  *										   *
  **********************************************************************************
  */

/* Includes -----------------------------------------------------------------------*/
#include "../include/clocks.h" 

/* Functions ----------------------------------------------------------------------*/

/**
  * @brief Turns on the HSI16 16 MHz internal oscillator.
  * @param None
  * @retval None
  */
void hsi16_init(void)
{
	/* Enable HSI16 clock (16 MHz) */
	RCC->CR |= RCC_CR_HSION; 

	/* Wait until HSI16 is ready */
	while (RCC->CR & RCC_CR_HSIRDY == 0);
}

/**
  * @brief Turns on the MSI internal oscillator.
  * @param range : Determines MSI frequency (0 <= range <= 11)
  *	           range 0 = 100 kHz
  *		   range 1 = 200 kHz
  *		   range 2 = 400 kHz
  *		   range 3 = 800 kHz
  *		   range 4 = 1 MHz
  *		   range 5 = 2 MHz
  *		   range 6 = 4 MHz
  *		   range 7 = 8 MHz
  *		   range 8 = 16 MHz
  *	    	   range 9 = 24 MHz
  *		   range 10 = 32 MHz
  *		   range 11 = 48 MHz
  * @retval None
  */
void msi_init(unsigned char range) 
{ 
	/* Ensure valid msi range used */
	if (range < 0 || range > 11) { 
		exit(1); 
	} 

	/* Set MSI clock speed */
	RCC->CR &= ~RCC_CR_MSIRANGE; 
	RCC->CR |= range << 4; 

	/* Indicate that MSIRANGE value in RCC_CR will be used */
	RCC->CR |= RCC_CR_MSIRGSEL;

	/* Enable MSI clock */
	RCC->CR |= RCC_CR_MSION;

	/* Wait until MSI is ready */
	while (RCC->CR & RCC_CR_MSIRDY == 0);
}

/**
  * @brief Configures and turns on the main PLL (phase-locked loop)
  *        with MSI as the input clock source.  Final PLL output is used 
  *	   as SYSCLK.
  * @param in_mul : Multiplier for the MSI input clock frequency.
  * @param in_div : Divider for the MSI input clock frequency.
  * @param out_div : Divider for the output of the main PLL.
  * @retval None
  */
void sysclk_init_pll(int in_mul, int in_div, int out_div)
{
	/* Disable PLL before changing the settings */
	RCC->CR &= ~RCC_CR_PLLON;
	while (RCC->CR & RCC_CR_PLLRDY);

	/* Select PLL input clock source (MSI) */
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_MSI;
	
	/* Set main PLL output frequency :
	       Final PLL output freq. = (f_MSI) * (in_mul / in_div) * (1 / out_div) */
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN;
	RCC->PLLCFGR |= in_mul << 8;  // Mutlipies MSI frequency
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM;
	RCC->PLLCFGR |= (in_div - 1) << 4;  // Divides MSI frequency
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLR;
	RCC->PLLCFGR |= (out_div / 2 - 1) << 25;  // Divides PLL output frequency

	/* Set PLLSAI3CLK output division factor to 7 (bit = 0) */
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP;
	
	/* Set PLL48M1CLK output division factor to 4 (bits = 01) */
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLQ;
	RCC->PLLCFGR |= 1 << 21;

	/* Enable PLL */
	RCC->CR |= RCC_CR_PLLON;

	/* Wait until PLL is ready */
	while (RCC->CR & RCC_CR_PLLRDY == 0);

	/* Enable main PLL output */
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;

	/* Enable PLLSAI3CLK */
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLPEN;

	/* Enable PLL48M1CLK */
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLQEN;

	/* Select PLL as system clock (SYSCLK) */
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while (RCC->CFGR & RCC_CFGR_SWS == 0);
}

/**
  * @brief Configures and turns on the PLLSAI1 clock.
  * @param in_mul : Input clock multiplier.
  * @param out_div : Divides the output of the PLLSAI1 clock.
  * @retval None
  */
void pllsai1_init(int in_mul, int out_div)
{
	/* Disable PLLSAI1 so it can be configured */
	RCC->CR &= ~RCC_CR_PLLSAI1ON;
	while (RCC->CR & RCC_CR_PLLSAI1RDY);
	
	/* Set PLLSAI1 output frequency :
		Final output freq. = (f_MSI) * (in_mul) * (1 / out_div) */
	RCC->PLLSAI1CFGR &= ~RCC_PLLSAI1CFGR_PLLSAI1N; 
	RCC->PLLSAI1CFGR |= in_mul << 8;  // Input clock multiplier
	RCC->PLLSAI1CFGR &= ~RCC_PLLSAI1CFGR_PLLSAI1P;
	RCC->PLLSAI1CFGR |= (out_div == 7 ? 0 : 1) << 17;  // Output clock divider

	/* Enable PLLSAI1 */
	RCC->CR |= RCC_CR_PLLSAI1ON;
	while (RCC->CR & RCC_CR_PLLSAI1RDY == 0);

	/* Enable PLLSAI1CLK */
	RCC->PLLSAI1CFGR |= RCC_PLLSAI1CFGR_PLLSAI1PEN;
}

/**
  * @brief Causes a system delay with SysTick.
  * @param time_ms : Number of milliseconds to delay.
  * @retval None
  */
void delay(uint32_t time_ms)
{       
	/* Initialize global volatile variable TimingDelay.
		This variable is decremented in the SysTick ISR */
        TimingDelay = time_ms;
	
	/* Wait for SysTick ISR to decrement TimingDelay to 0 */
        while(TimingDelay != 0);
}

/**
  * @brief Initializes SysTick down-counter.
  * @param ticks : Number of clock pulses needed for down-counter to reach 0.
  * @retval None
  */
void systick_init(uint32_t ticks)
{       
        /* Disable SysTick IRQ and SysTick Counter */
        SysTick->CTRL = 0;
        
        /* Set reload register */
        SysTick->LOAD = ticks - 1;
        
        /* Set priority */
        NVIC_SetPriority(SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
        
        /* Reset the SysTick counter value */
        SysTick->VAL = 0;
        
        /* Select SysTick clock source :
		Processor clock used */
         SysTick->CTRL |= SysTick_CTRL_CLKSOURCE;
        
        /* Enable SysTick IRQ and SysTick timer */
        SysTick->CTRL |= SysTick_CTRL_ENABLE;
        
        /* Enable SysTick exception request :
                 1 = counting down to zero asserts the SysTick exception request
        	 0 = counting down to zero does not assert the SysTick exception request */
        SysTick->CTRL |= SysTick_CTRL_TICKINT;
}

/**
  * @brief SysTick interrupt handler. Makes delay() function work.
  * @param None
  * @retval None
  */
void SysTick_Handler(void)
{
	/* Decrease TimingDelay to 0; then exit */
        if (TimingDelay > 0)
                TimingDelay--;
}
