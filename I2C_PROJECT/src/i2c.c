 /**********************************************************************************\
  * @file    I2C_PROJECT/src/i2c.c						   *
  * @author  Enki 								   *
  * @version V1.0								   *
  * @date    21-June-2017	    				 		   *
  * @brief   I2C driver.	 						   *
  *										   *
  **********************************************************************************
  * @attention								 	   *
  *										   *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Enki </center></h2>	 	  	   *
  *										   *
 \**********************************************************************************/

/* Includes ----------------------------------------------------------------------*/
#include "../include/i2c.h"

volatile uint32_t tc = 1;  // transfer complete
volatile uint32_t rc = 1;  // read complete
volatile uint32_t restart_req = 0; // restart request
/* Function Implementations ------------------------------------------------------*/

/**
  * @brief Configure I2C1 in 7-bit addressing mode
  *	   with DMA and interrupts.
  * @param None
  * @retval None
  */	
void i2c1_init(void)
{
	/* Configure PB6 and PB7 as I2C1 SCL and SDA, respectively. */
	i2c1_pins_init();	

	// Select the I2C1 clock source (HSI16)
	RCC->CCIPR &= ~RCC_CCIPR_I2C1SEL;
	RCC->CCIPR |= RCC_CCIPR_I2C1SEL_1;
	
	// Clock the I2C1 peripheral
	RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;	

	// Disable the I2C interface 
	I2C1->CR1 &= ~I2C_CR1_PE;
	while (I2C1->CR1 & I2C_CR1_PE);
	
	// Configure I2C timing settings for TCS34725: f_SCL = 400 KHz = (1 / (2.500 microseconds)) [max. allowed by snsr]
	I2C1->TIMINGR |= I2C_CLK_FREQ_DIV_2;  // Divide input clk freq. by 2 (16 MHz / 2 = 8 MHz)
	I2C1->TIMINGR |= 0U << 24;  // Reserved bit field = 0b0000
	I2C1->TIMINGR |= 3U << 20;  // Data setup time = (3 + 1) * (1 / 8M) = 500 ns > 100 ns min. req. by snsr
	I2C1->TIMINGR |= 3U << 16;  // Data hold time = 3 * (1 / 8M) = 0.25 microseconds
	I2C1->TIMINGR |= 0x07U << 8;  // SCL high period = (7 + 1) * (1 / 8M) = 1.0 microseconds 
	I2C1->TIMINGR |= 0x0BU << 0;  // SCL low period = (11 + 1) * (1 / 8M) = 1.5 microseconds
		
	// Use 7-bit addressing mode
	I2C1->CR2 &= ~I2C_CR2_ADD10;	

	// Allow DMA requests
	I2C1->CR1 |= I2C_CR1_TXDMAEN;
	I2C1->CR1 |= I2C_CR1_RXDMAEN;

	// Enable interrupts
	I2C1->CR1 |= I2C_CR1_TCIE;  // Unmask xfer cplt. interrupts
	NVIC_SetPriority(I2C1_EV_IRQn, 1);  // Register I2C1_EV_IRQn with NVIC
	NVIC_EnableIRQ(I2C1_EV_IRQn);

	// Enable the I2C1 peripheral
	I2C1->CR1 |= I2C_CR1_PE;
}

/**
  * @brief Configure PB6 and PB7 for I2C1 SCL and SDA, respectively.
  * @param None
  * @retval None
  */
static void i2c1_pins_init(void)
{
	// Clock port B if necessary
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

	// PB6:  Alt. func. 4 = I2C1_SCL
        GPIOB->MODER &= ~GPIO_MODER_MODER6;
        GPIOB->MODER |= GPIO_MODER_MODER6_1;
        GPIOB->AFR[0] &= ~GPIO_AFRL_AFRL6;
        GPIOB->AFR[0] |= 4U << (6 * 4); // alternative function 4 is SCL

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
        GPIOB->AFR[0] |= 4U << (7 * 4); // alternative function 4 is SDA

	// Configure as open-drain
	GPIOB->OTYPER |= GPIO_OTYPER_OT_7;  // open-drain = 1

        // Configure as pull-up 
        GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR7;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR7_0;  // pull-up = 01

        // Configure for high output speed 
        GPIOB->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR7;
        GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR7_1;  // high speed = 10
}

/**
  * @brief Transmits data to a slave on I2C1 bus.
  * @param nbytes : Number of bytes to transmit.
  * @param slvaddr : Address of the slave that will receive the data.
  * @param payload_ptr : Pointer to memory region that contains the
  *	   data to be transmitted on the I2C1 bus.
  * @retval None
  */
void i2c1_transmit(uint8_t nbytes, uint8_t slvaddr, uint8_t *payload_ptr)
{
	// Configure I2C1 for writing
	I2C1->CR2 &= ~I2C_CR2_RD_WRN;

	// Set the address of the slave to be written to
	I2C1->CR2 &= ~I2C_CR2_SADD;
	I2C1->CR2 |= slvaddr << 1;

	// Set number of bytes to transmit to the slave
	I2C1->CR2 &= ~I2C_CR2_NBYTES;	
	I2C1->CR2 |= nbytes << 16;

	// Do not automatically send stop bit after transmission of nbytes
	I2C1->CR2 &= ~I2C_CR2_AUTOEND;

	/* Configure DMA1_Channel6 to transfer nbytes from 
		the memory region pointed to by payload_ptr */
	DMA1_Channel6->CMAR = payload_ptr;
	DMA1_Channel6->CNDTR = nbytes;

	// Enable DMA1_Channel6
	DMA1_Channel6->CCR |= DMA_CCR_EN;
	
	tc = 0;
	I2C1->CR2 |= I2C_CR2_START;  // send start bit
	while (tc == 0);
}

/**
  * @brief Reads data from a slave on the I2C1 bus.
  * @param nbytes : Number of bytes to read from the slave.
  * @param slvaddr : Address of the slave to read from.
  * @param reg : Slave register to read from.
  * @param storage_ptr : Pointer to memory region where the 
  *	   received data will be stored (in the master).
  * @retval None
  */
void i2c1_read(uint8_t nbytes, uint8_t slvaddr, uint8_t *reg, uint8_t *storage_ptr)
{
	// Set number of bytes to transmit	
	I2C1->CR2 &= ~I2C_CR2_NBYTES;	
	I2C1->CR2 |= 1 << 16;

	// Do not automatically send stop bit after transmission of nbytes
	I2C1->CR2 &= ~I2C_CR2_AUTOEND;
	I2C1->CR2 &= ~I2C_CR2_RELOAD;

	// configure slave address
	I2C1->CR2 &= ~I2C_CR2_SADD;
	I2C1->CR2 |= slvaddr << 1;

	// configure for write
	I2C1->CR2 &= ~I2C_CR2_RD_WRN;

	// Setup DMA
	DMA1_Channel6->CMAR = reg;
	DMA1_Channel6->CNDTR = 1;
	DMA1_Channel6->CCR |= DMA_CCR_EN;  // turn on DMA for loading I2C1_TXDR
	
	restart_req = 1;	

	tc = 0;
	I2C1->CR2 |= I2C_CR2_START;  // send start bit
	while (tc == 0);  // wait for transfer to complete
	DMA1_Channel6->CCR &= ~DMA_CCR_EN;  // shut off the DMA

	// Set number of bytes to receive	
	I2C1->CR2 &= ~I2C_CR2_NBYTES;
	I2C1->CR2 |= nbytes << 16; 

	I2C1->CR2 |= I2C_CR2_RD_WRN;

	// Setup DMA	
	DMA1_Channel7->CMAR = storage_ptr;
	DMA1_Channel7->CNDTR = nbytes;
	DMA1_Channel7->CCR |= DMA_CCR_EN;
	
	rc = 0;  // read incomplete
	I2C1->CR2 |= I2C_CR2_START;
	while (rc == 0);
}

/**
  * @brief Handles interrupts generated by the I2C1 peripheral.
  * @param None
  * @retval None
  */
void I2C1_EV_IRQHandler(void)
{
	if (I2C1->ISR & I2C_ISR_TC) {
		if (restart_req == 1) {
			tc = 1;
			restart_req = 0;
		} else {
			if (I2C1->CR2 & I2C_CR2_RD_WRN) {
				rc = 1;	
				DMA1_Channel7->CCR &= ~DMA_CCR_EN;
			} else {
				tc = 1;
				DMA1_Channel6->CCR &= ~DMA_CCR_EN;	
			}
			I2C1->CR2 |= I2C_CR2_STOP;  // Send stop bit
		}
	}
}
