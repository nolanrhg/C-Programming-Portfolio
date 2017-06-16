/**
  **********************************************************************************
  * @file    VOICE_CHANGER/include/dma.h                                           *
  * @author  Nolan R. Gagnon                                                       *
  * @version V1.0                                                                  *
  * @date    15-June-2017                                                          *
  * @brief   DMA Interface.	                                                   *
  *                                                                                *
  **********************************************************************************
  * @attention                                                                     *
  *                                                                                *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. Gagnon  </center></h2>           *
  *                                                                                *
  **********************************************************************************
  */
#ifndef DMA_H
#define DMA_H

/***********************************************************************************
 *                                                                                 *
 *                              DMA CONSTANTS                                      *
 *                                                                                 *
 ***********************************************************************************/
#define DMA1_CH4_MAP_ON_DFSDM0			((uint32_t) 0x0 << 12)
#define DMA1_CH6_MAP_ON_I2C1_TXDR		((uint32_t) 0x3 << 20)
#define DMA2_CH1_MAP_ON_SAI1A			((uint32_t) 0x1 << 0)


/***********************************************************************************
 *                                                                                 *
 *                              DMA FUNCTIONS                                      *
 *                                                                                 *
 ***********************************************************************************/
void dfsdm_dma_init(int32_t *restrict buff, uint32_t buff_size);

void sai_dma_init(int16_t *restrict buff, uint32_t buff_size);

void i2c_tx_dma_init(void);

#endif
