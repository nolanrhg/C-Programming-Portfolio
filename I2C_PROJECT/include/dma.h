/**********************************************************************************\
 * @file    I2C_PROJECT/include/dma.h                                             *
 * @author  Nolan R. H. Gagnon                                                    *
 * @version V2.0                                                                  *
 * @date    26-June-2017                                                          *
 * @brief   DMA Interface.	                                                  *
 *                                                                                *
 **********************************************************************************
 * @attention                                                                     *
 *                                                                                *
 * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. H. Gagnon  </center></h2>        *
 *                                                                                *
\**********************************************************************************/
#ifndef DMA_H
#define DMA_H

/**********************************************************************************\
 *                                                                                *
 *                              DMA CONSTANTS                                     *
 *                                                                                *
\**********************************************************************************/
#define DMA1_CH7_MAP_ON_I2C1_RX                 ((uint32_t) 0x3 << 24)
#define DMA1_CH6_MAP_ON_I2C1_TX			((uint32_t) 0x3 << 20)
#define DMA1_CH2_MAP_ON_TIM1CH1                 ((uint32_t) 0x7 << 4)
#define PERIPH_SIZE_8_BITS                      ((uint32_t) 0x0 << 8)
#define PERIPH_SIZE_16_BITS                     ((uint32_t) 0x1 << 8)
#define MEM_SIZE_8_BITS                         ((uint32_t) 0x0 << 10)
#define MEM_SIZE_16_BITS 			((uint32_t) 0x1 << 10)

/**********************************************************************************\
 *                                                                                *
 *                              FUNCTION PROTOTYPES                               *
 *                                                                                *
\**********************************************************************************/

/**
  * @brief Creates DMA channel from memory to I2C_TXDR. 
  * @param None
  * @retval None
  */
void dma_i2c_tx_init(void);

/**
  * @brief Creates DMA channel from I2C_RXDR to memory.
  * @param None
  * @retval None
  */
void dma_i2c_rx_init(void);

#endif
