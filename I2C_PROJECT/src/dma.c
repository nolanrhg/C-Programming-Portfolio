 /**********************************************************************************\
  * @file    I2C_PROJECT/src/dma.c						   *
  * @author  Nolan R. Gagnon 		            				   *
  * @version V1.0								   *
  * @date    23-June-2017							   *
  * @brief   DMA driver.							   *
  *										   *
  **********************************************************************************
  * @attention							  		   *
  *										   *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. Gagnon </center></h2>		   *
  *										   *
 \**********************************************************************************/
  

/* Includes ----------------------------------------------------------------------*/
#include "../include/dma.h"

/* Function Implementations ------------------------------------------------------*/

/**
  * @brief Sets up DMA channel between I2C1_RXDR and memory.
  * @param None
  * @retval None
  */
void dma_i2c_rx_init(void)
{
        // Clock DMA controller 1
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

        // Ensure DMA1_Channel7 is disabled before configuring
        DMA1_Channel7->CCR &= ~DMA_CCR_EN;

        // Setup data path (periph ---data---> mem)
        DMA1_CSELR->CSELR &= ~DMA_CSELR_C7S;  // Clear channel selection bit field
        DMA1_CSELR->CSELR |= DMA1_CH7_MAP_ON_I2C1_RX;  // Map DMA1 CH7 on I2C1_RX
        DMA1_Channel7->CCR &= ~DMA_CCR_DIR;  // DMA reads from peripheral
        DMA1_Channel7->CPAR = (uint32_t) &(I2C1->RXDR);  // Send data from I2C1_RXDR

        // Transfer characteristics configuration
        DMA1_Channel7->CCR &= ~DMA_CCR_PINC;  // Do not increment periph. addr.
        DMA1_Channel7->CCR |= DMA_CCR_MINC;  // Increment mem. addr.
        DMA1_Channel7->CCR &= ~DMA_CCR_MSIZE;
        DMA1_Channel7->CCR |= MEM_SIZE_8_BITS;  // Memory data size = 8 bits
        DMA1_Channel7->CCR &= ~DMA_CCR_PSIZE;
        DMA1_Channel7->CCR |= PERIPH_SIZE_8_BITS;  // Peripheral data size = 8 bits
        DMA1_Channel7->CCR |= DMA_CCR_PL_0 | DMA_CCR_PL_1;  // very high priority
}

/**
  * @brief Configures DMA channel for transfers between memory and I2C1_TXDR.
  * @param None
  * @retval None
  */
void dma_i2c_tx_init(void)
{
	/* Clock DMA controller 1 */
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	
	/* Disable DMA1_Channel6 before changing settings */
	DMA1_Channel6->CCR &= ~DMA_CCR_EN;
	
	/* Setup data path (mem ---data---> periph) */ 
	DMA1_CSELR->CSELR &= ~DMA_CSELR_C6S;  // Clear channel selection bit field
	DMA1_CSELR->CSELR |= DMA1_CH6_MAP_ON_I2C1_TX;  // Map DMA1 CH6 on I2C1_TXDR
	DMA1_Channel6->CCR |= DMA_CCR_DIR;  // DMA reads from memory
	DMA1_Channel6->CPAR = (uint32_t) &(I2C1->TXDR);  // Send data to I2C1_TXDR
	
	/* Xfer characteristics */
	DMA1_Channel6->CCR &= ~DMA_CCR_PINC;  // Peripheral memory address not auto-incremented
	DMA1_Channel6->CCR |= DMA_CCR_MINC;  // Memory address auto-incremented
	DMA1_Channel6->CCR &= ~DMA_CCR_MSIZE;  // Memory size = 8 bits
	DMA1_Channel6->CCR &= ~DMA_CCR_PSIZE;  // Peripheral size = 8 bits
	DMA1_Channel6->CCR |= DMA_CCR_PL_1 | DMA_CCR_PL_0;  // Very high transfer priority
}
