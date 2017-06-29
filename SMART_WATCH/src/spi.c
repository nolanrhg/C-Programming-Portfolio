/*********************************************************************************\
 * @file    SMART_WATCH/src/spi.c                                                *
 * @author  Nolan R. H. Gagnon                                                   *
 * @version V1.0                                                                 *
 * @date    26-June-2017                                                         *
 * @brief   SPI driver.		     	                                         *
 *                                                                               *
 *********************************************************************************
 * @attention									 *
 *										 *
 * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. H. Gagnon </center></h2>        *
 *										 *
\*********************************************************************************/

/* Includes ----------------------------------------------------------------------*/
#include "../include/spi.h"

/* Public functions --------------------------------------------------------------*/
void spi_init(SPI_TypeDef *SPIx)
{
	/* Configure pins for controlling SPIx */
	spi_pins_init(SPIx);	

	/* Clock the SPIx peripheral */	
	if (SPIx == SPI1) {
		RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;	
	} else if (SPIx == SPI2) {
		RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN;	
	} else if (SPIx == SPI3) {
		RCC->APB1ENR1 |= RCC_APB1ENR1_SPI3EN;	
	}
	
	/* Disable the peripheral before configuration */	
	SPIx->CR1 &= ~SPI_CR1_SPE;	
	
	/* Set baud rate */
	SPIx->CR1 &= ~SPI_CR1_BR;	
	SPIx->CR1 |= 1U << 3;	// f_baud = 16 MHz / 4 = 4 MHz

	/* Set CPOL, CPHA combination */
	SPIx->CR1 &= ~SPI_CR1_CPOL;  // Idle state is low voltage
	SPIx->CR1 &= ~SPI_CR1_CPHA;  // Sample on the rising edge

	/* Use full-duplex communications */
	SPIx->CR1 &= ~SPI_CR1_RXONLY;
	SPIx->CR1 &= ~SPI_CR1_BIDIOE;
	
	/* Transmit MSB first */	
	SPIx->CR1 &= ~SPI_CR1_LSBFIRST;	

	/* Use software slave management */		
	SPIx->CR1 &= ~SPI_CR1_SSI;  // 0 is forced on the slave-select pin
	SPIx->CR1 |= SPI_CR1_SSM;  // Software slave management enabled

	/* Configure as master */
	SPIx->CR1 |= SPI_CR1_MSTR;	
}

/* Private functions -------------------------------------------------------------*/

/**
  * @brief Configures the pins used for the SPIx peripheral :
  *
  *	   SPI1 :
  *	   (*) PE12 = SPI1_NSS (chip select)
  *	   (*) PE13 = SPI1_SCK
  *	   (*) PE14 = SPI1_MISO
  *	   (*) PE15 = SPI1_MOSI
  *
  *	   SPI2 :
  *
  *	   SPI3 :
  *
  * @param None
  * @retval None
  */
static void spi_pins_init(SPI_TypeDef *SPIx)
{
	if (SPIx == SPI1) {
		/* Clock GPIO IO port E */
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;	
	
		/* Configure PE12 for SPI1_NSS */	
		GPIOE->MODER &= ~GPIO_MODER_MODER12;
		GPIOE->MODER |= GPIO_MODER_MODER12_1;  // Alternative function mode
		GPIOE->AFR[1] &= ~GPIO_AFRH_AFRH4;
		GPIOE->AFR[1] |= PE12_AF5_SPI1_NSS;  // Alternative function 5 = SPI1_NSS

		/* Configure PE13 for SPI1_SCK */	
		GPIOE->MODER &= ~GPIO_MODER_MODER13;
		GPIOE->MODER |= GPIO_MODER_MODER13_1;  // Alternative function mode
		GPIOE->AFR[1] &= ~GPIO_AFRH_AFRH5;
		GPIOE->AFR[1] |= PE13_AF5_SPI1_SCK;  // Alternative function 5 = SPI1_SCK

		/* Configure PE14 for SPI1_MISO */	
		GPIOE->MODER &= ~GPIO_MODER_MODER14;
		GPIOE->MODER |= GPIO_MODER_MODER14_1;  // Alternative function mode
		GPIOE->AFR[1] &= ~GPIO_AFRH_AFRH6;
		GPIOE->AFR[1] |= PE14_AF5_SPI1_MISO;  // Alternative function 5 = SPI1_MISO

		/* Configure PE15 for SPI1_MOSI */	
		GPIOE->MODER &= ~GPIO_MODER_MODER15;
		GPIOE->MODER |= GPIO_MODER_MODER15_1;  // Alternative function mode
		GPIOE->AFR[1] &= ~GPIO_AFRH_AFRH7;
		GPIOE->AFR[1] |= PE15_AF5_SPI1_MOSI;  // Alternative function 5 = SPI1_MOSI
	} else if (SPIx == SPI2) {
		/* Not needed right now */	
	} else if (SPIx == SPI3) {
		/* Not needed right now */	
	}
}
