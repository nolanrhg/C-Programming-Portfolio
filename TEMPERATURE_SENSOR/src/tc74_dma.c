/**
  **********************************************************************************
  * @file    RGB_SENSOR/src/dma.c
  * @author  Nrgagnon 
  * @version V1.6
  * @date    29-May-2017
  * @brief   DMA driver.
  *
  **********************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. H. Gagnon </center></h2>
  *
  **********************************************************************************
  */

/* Includes ----------------------------------------------------------------------*/
#include "../include/tc74_dma.h"
#include "../include/tc74_led.h"

/* Function Implementations ------------------------------------------------------*/

/**
  * @brief Sets up DMA channel between I2C1_RXDR and memory.
  * @param None
  * @retval None
  */
void dma_i2c_rx2mem_init(void)
{
	// Clock DMA controller 1
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	
	// Ensure DMA1_Channel7 is disabled before configuring
	DMA1_Channel7->CCR &= ~DMA_CCR_EN;
	
	// Setup data path (periph ---data---> mem)
	DMA1_CSELR->CSELR &= ~DMA_CSELR_C7S;  // Clear channel selection bit field
	DMA1_CSELR->CSELR |= DMA1_CH7_MAP_ON_I2C1_RX;  // Map DMA1 CH7 on I2C1_RX
	DMA1_Channel7->CCR &= ~DMA_CCR_DIR;  // DMA reads from peripheral
	DMA1_Channel7->CPAR = &(I2C1->RXDR);  // Send data from I2C1_RXDR

	// Xfer characteristics configuration
	DMA1_Channel7->CCR &= ~DMA_CCR_PINC;  // Do not increment periph. addr.
	DMA1_Channel7->CCR |= DMA_CCR_MINC;  // Increment mem. addr.
	DMA1_Channel7->CCR &= ~DMA_CCR_MSIZE; 
	DMA1_Channel7->CCR |= MEM_SIZE_8_BITS;  // Memory data size = 8 bits
	DMA1_Channel7->CCR &= ~DMA_CCR_PSIZE; 
	DMA1_Channel7->CCR |= PERIPH_SIZE_8_BITS;  // Peripheral data size = 8 bits
	DMA1_Channel7->CCR |= DMA_CCR_PL_0 | DMA_CCR_PL_1;  // very high priority
}

/**
  * @brief Sets up DMA channel between memory and I2C1_TXDR.
  * @param None
  * @retval None
  */
void dma_i2c_mem2tx_init(void)
{
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;  // clock DMA1
        DMA1_Channel6->CCR &= ~DMA_CCR_EN;  // disable DMA1_Channel6
        DMA1_Channel6->CPAR = &(I2C1->TXDR);  // send data to I2C1_TXDR
        DMA1_Channel6->CCR |= DMA_CCR_DIR;  // tx from mem to periph
        DMA1_Channel6->CCR &= ~DMA_CCR_PINC;  // do not increment periph addr
        DMA1_Channel6->CCR |= DMA_CCR_MINC;  // increment mem addr
        DMA1_Channel6->CCR &= ~DMA_CCR_MSIZE;  // size of mem = 8 bits
        DMA1_Channel6->CCR &= ~DMA_CCR_PSIZE;  // size of periph = 8 bits
        DMA1_Channel6->CCR |= DMA_CCR_PL_0 | DMA_CCR_PL_1;  // very high priority
        DMA1_CSELR->CSELR &= ~DMA_CSELR_C6S;
        DMA1_CSELR->CSELR |= 3 << 20;  // DMA map to I2C1_TXDR
}

/**
  * @brief Sets up DMA channel between memory and TIM1_CCR1.
  * @param dma_mem_ptr : Pointer to memory region that contains
  *	   the CCR values to be transferred to TIM1_CCR1.
  * @retval None
  */
void dma_mem2tim1_init(uint16_t *restrict dma_mem_ptr)
{
	// Clock DMA controller 1       
       	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

        // Ensure the DMA is disabled before configuring
        DMA1_Channel2->CCR &= ~DMA_CCR_EN;

        // Setup data path (mem ---data---> periph)
        DMA1_CSELR->CSELR &= ~DMA_CSELR_C2S;  // clear channel selection bit field
        DMA1_CSELR->CSELR |= DMA1_CH2_MAP_ON_TIM1CH1;  // Map DMA1 CH2 on TIM1_CH1
        DMA1_Channel2->CCR |= DMA_CCR_DIR;  // DMA reads from memory
        DMA1_Channel2->CPAR = &(TIM1->CCR1); // data Xfer TO TIM1_CCR1 preload reg.
        DMA1_Channel2->CMAR = dma_mem_ptr;  // data Xfer FROM memory region that starts at this addr. 

        // Xfer characteristics configuration
        DMA1_Channel2->CCR &= ~DMA_CCR_PINC;  // Do not increment the Xfer destination address
        DMA1_Channel2->CCR |= DMA_CCR_MINC;  // Do increment Xfer source address from supplied base address
        DMA1_Channel2->CCR |= DMA_CCR_CIRC;  // Continously perform Xfer seq. from supplied base address
        DMA1_Channel2->CCR |= PERIPH_SIZE_16_BITS; // peripheral data size = 16 bits (see ref. man., pg. 834)
        DMA1_Channel2->CCR |= MEM_SIZE_16_BITS;  //  memory data size = 16 bits
        DMA1_Channel2->CNDTR = NUM_PERIODS_FOR_RESET  + (NUM_LEDS * NUM_COLOR_BITS);  // Number of 16-bit data to Xfer
        DMA1_Channel2->CCR |= DMA_CCR_PL_0 | DMA_CCR_PL_1;  // Set channel priority level to VERY HIGH
}
