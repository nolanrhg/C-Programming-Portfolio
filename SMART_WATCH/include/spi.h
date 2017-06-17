#ifndef SPI_H
#define SPI_H

/***********************************************************************************
 *                                                                                 *
 *                              SPI CONSTANTS                                      *
 *                                                                                 *
 ***********************************************************************************/

#define PE12_AF5_SPI1_NSS		((uint32_t) 5U << (4 * 4)) 
#define PE13_AF5_SPI1_SCK		((uint32_t) 5U << (4 * 5))
#define PE14_AF5_SPI1_MISO		((uint32_t) 5U << (4 * 6))
#define PE15_AF5_SPI1_MOSI		((uint32_t) 5U << (4 * 7))

/* Public functions ---------------------------------------------------------------*/
void spi_init(SPI_TypeDef *SPIx);

/* Private functions --------------------------------------------------------------*/
static void spi_pins_init(SPI_TypeDef *SPIx);

#endif
