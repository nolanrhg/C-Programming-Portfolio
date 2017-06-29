/**********************************************************************************\
 * @file    SMART_WATCH/include/ili9340c.h                                        *
 * @author  Nolan R. H. Gagnon                                                    *
 * @version V1.0                                                                  *
 * @date    28-June-2017                                                          *
 * @brief   ILI9340C Firmware-level Interface.                                    *
 *                                                                                *
 **********************************************************************************
 * @attention                                                                     *
 *                                                                                *
 * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. H. Gagnon  </center></h2>        *
 *                                                                                *
\**********************************************************************************/
#ifndef ILI9340C_H
#define ILI9340C_H

/**********************************************************************************\
 *                                                                                *
 *                              ILI9340C COMMANDS                                 *
 *                                                                                *
\**********************************************************************************/
#define ILI9340C_NOP			((uint8_t) 0x00)	/*!< No operation */
#define ILI9340C_SWRST			((uint8_t) 0x01)	/*!< Software reset */
#define ILI9340C_GET_INFO		((uint8_t) 0x04)	/*!< Read ID Information */
#define ILI9340C_GET_STAT		((uint8_t) 0x09)	/*!< Read display status */
#define ILI9340C_GET_PWRMOD		((uint8_t) 0x0A)	/*!< Read power mode */
#define ILI9340C_GET_MADCTL		((uint8_t) 0x0B)	/*!< Read Memory address control */
#define ILI9340C_GET_PIXFMT		((uint8_t) 0x0C)	/*!< Read pixel format */
#define ILI9340C_GET_IMGFMT		((uint8_t) 0x0D)	/*!< Read image format */
#define ILI9340C_GET_SIGMOD		((uint8_t) 0x0E)	/*!< Read signal mode */
#define ILI9340C_GET_DIAGRSLT		((uint8_t) 0x0F)	/*!< Read self-diagnostic result */
#define ILI9340C_SLEEP			((uint8_t) 0x10)	/*!< Enter sleep mode */
#define ILI9340C_WAKEUP			((uint8_t) 0x11)	/*!< Exit sleep mode */
#define ILI9340C_PMOD_ON		((uint8_t) 0x12)	/*!< Turn partial mode on */
#define ILI9340C_NMOD_ON		((uint8_t) 0x13)	/*!< Turn normal mode one */
#define ILI9340C_INV_OFF		((uint8_t) 0x20)	/*!< Turn display inversion off */
#define ILI9340C_INV_ON			((uint8_t) 0x21)	/*!< Turn display inversion on */
#define ILI9340C_SET_GAMMA		((uint8_t) 0x26)	/*!< Set gamma correction curve */
#define ILI9340C_DISP_OFF		((uint8_t) 0x28)	/*!< Turn display off */
#define ILI9340C_DISP_ON		((uint8_t) 0x29)	/*!< Turn display on */
#define ILI9340C_SET_CADDR		((uint8_t) 0x2A)	/*!< Set column address */
#define ILI9340C_SET_PADDR		((uint8_t) 0x2B)	/*!< Set page address */
#define ILI9340C_RAMWRITE		((uint8_t) 0x2C)	/*!< Write data into RAM */
#define ILI9340C_SET_COLOR		((uint8_t) 0x2C)	/*!< Define LUT for 16-bit to 18-bit color depth conversion */
#define ILI9340C_RAMREAD		((uint8_t) 0x2E)	/*!< Read data from RAM */
#define ILI9340C_SET_PAREA		((uint8_t) 0x30)	/*!< Set partial mode display area */
#define ILI9340C_SET_VSAREA		((uint8_t) 0x33)	/*!< Set vertical scrolling area of display */
#define ILI9340C_TEAR_OFF		((uint8_t) 0x34)	/*!< Turn tearing effect line off */
#define ILI9340C_TEAR_ON		((uint8_t) 0x35)	/*!< Turn tearing effect line on */
#define ILI9340C_SET_MADCTL		((uint8_t) 0x36)	/*!< Define read/write scanning direction of frame memory */
#define ILI9340C_SET_VSSADDR		((uint8_t) 0x37)	/*!< Set vertical scrolling start address */
#define ILI9340C_IDLE_OFF		((uint8_t) 0x38)	/*!< Turn idle mode off */
#define ILI9340C_IDLE_ON		((uint8_t) 0x39)	/*!< Turn idle mode on */
#define ILI9340C_SET_PIXFMT		((uint8_t) 0x3A)	/*!< Set pixel format */
#define ILI9340C_CONT_MEMWR		((uint8_t) 0x3C)	/*!< Continue writing to memory */
#define ILI9340C_CONT_MEMRD		((uint8_t) 0x3E)	/*!< Continue reading from memory */
#define ILI9340C_SET_TEARLN		((uint8_t) 0x44)	/*!< Set tear scanline */
#define ILI9340C_GET_SCANLN		((uint8_t) 0x45)	/*!< Get current scanline */
#define ILI9340C_SET_BRTNSS		((uint8_t) 0x51)	/*!< Set display brightness */
#define ILI9340C_GET_BRTNSS		((uint8_t) 0x52)	/*!< Read display brightness */
#define ILI9340C_SET_CTLDISP		((uint8_t) 0x53)	/*!< Write control display */ 
#define ILI9340C_GET_CTLDISP		((uint8_t) 0x54)	/*!< Read control display */
#define ILI9340C_SET_CABC		((uint8_t) 0x55)	/*!< Write content adaptive brightness control (CABC) */
#define ILI9340C_GET_CABC		((uint8_t) 0x56)	/*!< Read CABC */
#define ILI9340C_SET_MINBRTNSS		((uint8_t) 0x5E)	/*!< Write CABC minimum brightness */
#define ILI9340C_GET_MINBRTNSS		((uint8_t) 0x5F)	/*!< Read CABC minimum brightness */
#define ILI9340C_GET_ID1		((uint8_t) 0xDA)	/*!< Read LCD module's manufacturer ID */
#define ILI9340C_GET_ID2		((uint8_t) 0xDB)	/*!< Read LCD module/driver version */
#define ILI9340C_GET_ID3		((uint8_t) 0xDC)	/*!< Read LCD module/driver */
#define ILI9340C_SET_IFMOD		((uint8_t) 0xB0)	/*!< Set operation status of display interface */
#define ILI9340C_SET_FRMRTN		((uint8_t) 0xB1)	/*!< Set frame rate in normal mode */
#define ILI9340C_SET_FRMRTI		((uint8_t) 0xB2)	/*!< Set frame rate in idle mode */
#define ILI9340C_SET_FRMRTP		((uint8_t) 0xB3)	/*!< Set frame rate in partial mode */
#define ILI9340C_SET_INVMOD		((uint8_t) 0xB4) 	/*!< Set display inversion mode */
#define ILI9340C_SET_BLNKPRCH		((uint8_t) 0xB5)	/*!< Set porch line numbers */
#define ILI9340C_SET_DISPFUNC		((uint8_t) 0xB6)	/*!< Set display functions */
#define ILI9340C_SET_ENTMOD		((uint8_t) 0xB7)	/*!< Set entry mode */
#define ILI9340C_SET_BLCTL1		((uint8_t) 0xB8)	/*!< Set backlight control settings (1) */
#define ILI9340C_SET_BLCTL2		((uint8_t) 0xB9)	/*!< Set backlight control settings (2) */
#define ILI9340C_SET_BLCTL3		((uint8_t) 0xBA)	/*!< Set backlight control settings (3) */
#define ILI9340C_SET_BLCTL4		((uint8_t) 0xBB)	/*!< Set backlight control settings (4) */
#define ILI9340C_SET_BLCTL5 		((uint8_t) 0xBC)	/*!< Set backlight control settings (5) */
#define ILI9340C_SET_BLCTL7		((uint8_t) 0xBE)	/*!< Set backlight control settings (7) */
#define ILI9340C_SET_BLCTL8		((uint8_t) 0xBF)	/*!< Set backlight control settings (8) */
#define ILI9340C_SET_PWRCTL1		((uint8_t) 0xC0)	/*!< Set power control settings (1) */
#define ILI9340C_SET_PWRCTL2		((uint8_t) 0xC1)	/*!< Set power control settings (2) */
#define ILI9340C_SET_PWRCTL3		((uint8_t) 0xC2)	/*!< Set power control settings (3) */
#define ILI9340C_SET_PWRCTL4		((uint8_t) 0xC3)	/*!< Set power control settings (4) */
#define ILI9340C_SET_PWRCTL5		((uint8_t) 0xC4)	/*!< Set power control settings (5) */
#define ILI9340C_SET_VCOMCTL1		((uint8_t) 0xC5)	/*!< Set VCOM control settings (1) */
#define ILI9340C_SET_VCOMCTL2		((uint8_t) 0xC7)	/*!< Set VCOM control settings (2) */
#define ILI9340C_NVMEMWR		((uint8_t) 0xD0)	/*!< Write to non-volatile memory */
#define ILI9340C_NVMEMKEY		((uint8_t) 0xD1)	/*!< Write non-volatile memory key */
#define ILI9340C_GET_NVMEMSTAT		((uint8_t) 0xD2)	/*!< Read non-volatile memory status */
#define ILI9340C_GET_ID4		((uint8_t) 0xD3)	/*!< Read IC device code */
#define ILI9340C_SET_POSGAMCORR		((uint8_t) 0xE0)	/*!< Set positive gamma correction */
#define ILI9340C_SET_NEGGAMCORR		((uint8_t) 0xE1)	/*!< Set negative gamma correction */
#define ILI9340C_SET_DIGAMCTL1		((uint8_t) 0xE2)	/*!< Set digital gamma control settings (1) */
#define ILI9340C_SET_DIGAMCTL2		((uint8_t) 0xE3)	/*!< Set digital gamma control settings (2) */
#define ILI9340C_SET_IFCTL		((uint8_t) 0xF6)	/*!< Set interface control settings */

#endif
