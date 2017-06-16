/**
  **********************************************************************************
  * @file    VOICE_CHANGER/src/i2c.c
  * @author  Nrgagnon 
  * @version V1.3
  * @date    14-June-2017
  * @brief   I2C driver.
  *
  **********************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. H. Gagnon </center></h2>
  *
  **********************************************************************************
  */

/* Includes ----------------------------------------------------------------------*/
#include "../include/i2c.h"
#include <string.h>

/* Volatile variables ------------------------------------------------------------*/ 
static volatile int tc;

/* Function Implementations ------------------------------------------------------*/

/**
  * @brief Configure I2C1 in 7-bit addressing mode
  *	   with DMA and interrupts.
  * @param None
  * @retval None
  */
void i2c1_init(void)
{
	/* Configure PB6 and PB7 as I2C1_SCL and I2C1_SDA, respectively */
	i2c1_pins_init();	

	// Select the I2C1 clock source (HSI16)
	RCC->CCIPR &= ~RCC_CCIPR_I2C1SEL;
	RCC->CCIPR |= RCC_CCIPR_I2C1SEL_1;
	
	// Clock the I2C1 peripheral
	RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;	

	// Disable the I2C interface 
	I2C1->CR1 &= ~I2C_CR1_PE;
	while (I2C1->CR1 & I2C_CR1_PE);

	/* CS43L22 I2C signal requirements :

		f_SCL <= 100 kHz
		SCL_low >= 4.7 microseconds
		SCL_high >= 4.0 microseconds
		Hold time >= 0 microseconds
		Setup time >= 250 nanoseconds  

	   Programmed Values :
	
		f_SCL = 100 kHz
		SCL_low = 5 microseconds	
		SCL_high = 5 microseconds
		Hold time = 250 nanoseconds
		Setup time = 500 nanoseconds
						*/
	I2C1->TIMINGR |= I2C_CLK_FREQ_DIV_4;  // Divide input clk freq. by 4 (16 MHz / 4 = 4 MHz)
	I2C1->TIMINGR |= 0U << 24;  // Reserved bit field = 0b0000
	I2C1->TIMINGR |= 1U << 20; // Data setup time = (1 + 1) * (1 / 4M) = 500 ns > 250 ns
	I2C1->TIMINGR |= 1U << 16;  // Data hold time = 1 * (1 / 4M) = 250 ns > 0 ns
	I2C1->TIMINGR |= 0x13U << 8;  // SCL high time = (19 + 1) * (1 / 4M) = 5 microseconds
	I2C1->TIMINGR |= 0x13U << 0;  // SCL low time = (19 + 1) * (1 / 4M) = 5 microseconds
		
	// Use 7-bit addressing mode
	I2C1->CR2 &= ~I2C_CR2_ADD10;	

	// Allow DMA requests
	I2C1->CR1 |= I2C_CR1_TXDMAEN;

	// Enable interrupts
	I2C1->CR1 |= I2C_CR1_TCIE;
	NVIC_SetPriority(I2C1_EV_IRQn, 1);
	NVIC_EnableIRQ(I2C1_EV_IRQn);

	// Enable the I2C1 peripheral
	I2C1->CR1 |= I2C_CR1_PE;
}

void i2c1_pins_init(void)
{
        /*
         * Configure PB6 for I2C1 SCL and PB7 for I2C1 SDA 
         */

	// Clock port B if necessary
	if (RCC->AHB2ENR & RCC_AHB2ENR_GPIOBEN == 0) {
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	}

	// PB6:  Alt. func. 4 = I2C1_SCL
        GPIOB->MODER &= ~GPIO_MODER_MODER6;
        GPIOB->MODER |= GPIO_MODER_MODER6_1;
        GPIOB->AFR[0] &= ~GPIO_AFRL_AFRL6;
        GPIOB->AFR[0] |= 4 << (6 * 4); // alternative function 4 is SCL

	// Configure as open-drain
	GPIOB->OTYPER |= GPIO_OTYPER_OT_6;  // open-drain = 1

        // Configure as pull-up 
        GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR6;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR6_0;  // pull-up = 01

        // Configure for high output speed 
        GPIOB->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR6;
        GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6_1;  // high speed = 10


	// PB7: Alt. func. 4 = I2C1_SDA
	GPIOB->MODER &= ~GPIO_MODER_MODER7;
        GPIOB->MODER |= GPIO_MODER_MODER7_1;
        GPIOB->AFR[0] &= ~GPIO_AFRL_AFRL7;
        GPIOB->AFR[0] |= 4 << (7 * 4); // alternative function 4 is SCL

	// Configure as open-drain
	GPIOB->OTYPER |= GPIO_OTYPER_OT_7;  // open-drain = 1

        // Configure as pull-up 
        GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR7;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR7_0;  // pull-up = 01

        // Configure for high output speed 
        GPIOB->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR7;
        GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR7_1;  // high speed = 10
}

void i2c1_transmit(uint8_t nbytes, uint8_t slvaddr, uint8_t *payload)
{
	// set number of bytes to transmit	
	I2C1->CR2 &= ~I2C_CR2_NBYTES;	
	I2C1->CR2 |= nbytes << 16;  // + 1 for the starting_reg address

	// automatically send stop bit after transmission of nbytes
	I2C1->CR2 &= ~I2C_CR2_AUTOEND;
	I2C1->CR2 &= ~I2C_CR2_RELOAD;

	// configure slave address
	I2C1->CR2 &= ~I2C_CR2_SADD;
	I2C1->CR2 |= slvaddr << 1;

	// configure for write
	I2C1->CR2 &= ~I2C_CR2_RD_WRN;

	// Setup DMA
	DMA1_Channel6->CMAR = payload;
	DMA1_Channel6->CNDTR = nbytes;
	DMA1_Channel6->CCR |= DMA_CCR_EN;  // turn on DMA for loading I2C1_TXDR
	
	tc = 0;	  // transfer incomplete
	I2C1->CR2 |= I2C_CR2_START;  // send start bit
	while (tc == 0);  // wait for transfer to complete
	DMA1_Channel6->CCR &= ~DMA_CCR_EN;  // shut off the DMA
}

void I2C1_EV_IRQHandler(void)
{
	tc = 1;
	I2C1->CR2 |= I2C_CR2_STOP;
}
