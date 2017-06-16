/*!
 *
 * @file
 *
 * @brief DMA library
 *
 * @author Nrgagnon
 *
 * @date May 11, 2017
 *
 */

#ifndef TC74_DMA_H
#define TC74_DMA_H

#define DMA1_CH7_MAP_ON_I2C1_RX                 ((uint32_t) 0x03 << 24)
#define DMA1_CH2_MAP_ON_TIM1CH1                 ((uint32_t) 0x07 << 4)
#define PERIPH_SIZE_8_BITS                      ((uint32_t) 0x00 << 8)
#define PERIPH_SIZE_16_BITS                     ((uint32_t) 0x01 << 8)
#define MEM_SIZE_8_BITS                         ((uint32_t) 0x00 << 10)
#define MEM_SIZE_16_BITS                        ((uint32_t) 0x01 << 10)

void dma_i2c_rx2mem_init(void);

void dma_i2c_mem2tx_init(void);

void dma_mem2tim1_init(uint16_t *restrict dma_mem_ptr);

#endif
