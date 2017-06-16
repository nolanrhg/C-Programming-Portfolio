/**
  **********************************************************************************
  * @file    VOICE_CHANGER/src/dma.c
  * @author  YSSKOSSK_JR.
  * @version V1.0
  * @date    14-June-2017
  * @brief   DMA driver.
  *
  **********************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 YSSKOSSK_JR. </center></h2>
  *
  **********************************************************************************
  */

/* Includes ----------------------------------------------------------------------*/
#include "../include/dma.h"

/* Function Implementations ------------------------------------------------------*/

/**
  * @brief Creates circular DMA channel between DFSDM0_RDATAR and memory.
  * @param buff : Pointer to memory region where 32-bit data from
  *	 	  DFSDM0_RDATAR will be transferred.
  * @param buff_size : Number of 32-bit values to transfer to memory in 
  *	  	       each DMA cycle.
  * @retval None
  */
void dfsdm_dma_init(int32_t *restrict buff, uint32_t buff_size)
{
        /* Clock DMA controller 1 */
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

        /* Disable DMA1_Channel4 before changing settings */
        DMA1_Channel4->CCR &= ~DMA_CCR_EN;

	/* Setup data path (periph ---data---> mem) */
        DMA1_CSELR->CSELR &= ~DMA_CSELR_C4S;  // Clear channel selection bit field
	DMA1_CSELR->CSELR |= DMA1_CH4_MAP_ON_DFSDM0;  // Map DMA1 CH4 on DFSDM0
        DMA1_Channel4->CCR &= ~DMA_CCR_DIR;  // DMA reads from peripheral
        DMA1_Channel4->CPAR = (uint32_t) &(DFSDM_Filter0->RDATAR);  // Send data from DFSDM_Filter0_RDATAR
        DMA1_Channel4->CMAR = (uint32_t) buff;  // Send data to buffer in memory

	/* Xfer characteristics */
        DMA1_Channel4->CCR &= ~DMA_CCR_PINC;  // Peripheral memory address not auto-incremented
        DMA1_Channel4->CCR |= DMA_CCR_MINC;  // Memory address auto-incremented
        DMA1_Channel4->CCR |= DMA_CCR_CIRC;  // Circular memory buffer
	DMA1_Channel4->CCR &= ~DMA_CCR_PSIZE;
	DMA1_Channel4->CCR |= DMA_CCR_PSIZE_1;  // Peripheral data size = 32 bits
	DMA1_Channel4->CCR &= ~DMA_CCR_MSIZE;
	DMA1_Channel4->CCR |= DMA_CCR_MSIZE_1;  // Memory data size = 32 bits
        DMA1_Channel4->CNDTR = buff_size;  // Number of 32-bit ints to transfer
        DMA1_Channel4->CCR &= ~DMA_CCR_PL;
        DMA1_Channel4->CCR |= DMA_CCR_PL_1;  // High transfer priority

        /* Enable DMA interrupts (XFER CPLT & HALF-XFER CPLT) */
        DMA1_Channel4->CCR |= DMA_CCR_TCIE;  // xfer-cplt interrupt
	DMA1_Channel4->CCR |= DMA_CCR_HTIE;  // 1/2 xfer-cplt interrupt
        NVIC_SetPriority(DMA1_Channel4_IRQn, 1);  // Register interrupt handler with NVIC
        NVIC_EnableIRQ(DMA1_Channel4_IRQn);

        /* Enable DMA1_Channel4 */
        DMA1_Channel4->CCR |= DMA_CCR_EN;
}

/**
  * @brief Creates circular DMA channel between memory and SAI1_A.
  * @param buff : Pointer to memory region that contains 16-bit data
  *	 	  to be loaded into SAI1_A FIFO. 
  * @param buff_size : Number of 16-bit values to transfer to SAI1_A FIFO in 
  *	  	       each DMA cycle.
  * @retval None
  */
void sai_dma_init(int16_t *restrict buff, uint32_t buff_size)
{
	/* Clock DMA controller 2 */
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

        /* Disable DMA1_Channel4 before changing settings */
        DMA2_Channel1->CCR &= ~DMA_CCR_EN;
	
	/* Setup data path (mem ---data---> periph) */
        DMA2_CSELR->CSELR &= ~DMA_CSELR_C1S;  // Clear channel selection bit field
        DMA2_CSELR->CSELR |= DMA2_CH1_MAP_ON_SAI1A;  // Map DMA2 CH1 on SAI1_A
        DMA2_Channel1->CCR |= DMA_CCR_DIR;  // DMA reads from memory
        DMA2_Channel1->CPAR = (uint32_t) &(SAI1_Block_A->DR);  // Send data to SAI1_Block_A_DR (loads FIFO if space avail.)
        DMA2_Channel1->CMAR = (uint32_t) buff;  // Send data from memory buffer
	
	/* Xfer characteristics  */
        DMA2_Channel1->CCR &= ~DMA_CCR_PINC;  // Peripheral memory address not auto-incremented
        DMA2_Channel1->CCR |= DMA_CCR_MINC;  // Memory address auto-incremented
        DMA2_Channel1->CCR |= DMA_CCR_CIRC;  // Circular memory buffer
        DMA2_Channel1->CCR &= ~DMA_CCR_PSIZE;  
        DMA2_Channel1->CCR |= DMA_CCR_PSIZE_0;  // Peripheral size = 16 bits
        DMA2_Channel1->CCR &= ~DMA_CCR_MSIZE;  
        DMA2_Channel1->CCR |= DMA_CCR_MSIZE_0;  // Memory size = 16 bits
        DMA2_Channel1->CNDTR = buff_size;  // Number of 16-bit ints to transfer
        DMA2_Channel1->CCR &= ~DMA_CCR_PL;  
        DMA2_Channel1->CCR |= DMA_CCR_PL_1 | DMA_CCR_PL_0;  // Very high transfer priority

        /* Enable DMA2_Channel1 */
        DMA2_Channel1->CCR |= DMA_CCR_EN;
}

/**
  * @brief Configures DMA channel for transfers between memory and I2C1_TXDR.
  * @param None
  * @retval None
  */
void i2c_tx_dma_init(void)
{
	/* Clock DMA controller 1 */
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	
	/* Disable DMA1_Channel6 before changing settings */
	DMA1_Channel6->CCR &= ~DMA_CCR_EN;
	
	/* Setup data path (mem ---data---> periph) */ 
	DMA1_CSELR->CSELR &= ~DMA_CSELR_C6S;  // Clear channel selection bit field
	DMA1_CSELR->CSELR |= DMA1_CH6_MAP_ON_I2C1_TXDR;  // Map DMA1 CH6 on I2C1_TXDR
	DMA1_Channel6->CCR |= DMA_CCR_DIR;  // DMA reads from memory
	DMA1_Channel6->CPAR = (uint32_t) &(I2C1->TXDR);  // Send data to I2C1_TXDR
	
	/* Xfer characteristics */
	DMA1_Channel6->CCR &= ~DMA_CCR_PINC;  // Peripheral memory address not auto-incremented
	DMA1_Channel6->CCR |= DMA_CCR_MINC;  // Memory address auto-incremented
	DMA1_Channel6->CCR &= ~DMA_CCR_MSIZE;  // Memory size = 8 bits
	DMA1_Channel6->CCR &= ~DMA_CCR_PSIZE;  // Peripheral size = 8 bits
	DMA1_Channel6->CCR |= DMA_CCR_PL_1 | DMA_CCR_PL_0;  // Very high transfer priority
}
