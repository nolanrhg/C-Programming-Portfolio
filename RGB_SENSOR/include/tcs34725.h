#ifndef TCS34725_H
#define TCS34725_H

typedef struct
{
	uint8_t CMDR;		/*!< TCS34725 Command Register */
	uint8_t ENR;		/*!< TCS34725 Enable Register */
	uint8_t ATIMR;		/*!< TCS34725 Conversion Time Register */
	uint8_t WTIMR;		/*!< TCS34725 Wait Time Register */
	uint8_t LTLBR;		/*!< TCS34725 Clear Interrupt Low Threshold Low Byte Register */
	uint8_t LTHBR;	 	/*!< TCS34725 Clear Interrupt Low Threshold High Byte Register */
	uint8_t HTLBR;		/*!< TCS34725 Clear Interrupt High Threshold Low Byte Register */
	uint8_t HTHBR;		/*!< TCS34725 Clear Interrupt High Threshold High Byte Register */
	uint8_t PERSR;          /*!< TCS34725 Interrupt Persistence Filter Register */
	uint8_t CFGR;		/*!< TCS34725 Configuration Register */
	uint8_t CR;		/*!< TCS34725 Control Register */
	uint8_t IDR;		/*!< TCS34725 ID Register */
	uint8_t SR;		/*!< TCS34725 Status Register */
	uint8_t *COLRDATA;

} RGB_Sensor_TypeDef;

/* TCS34725 I2C Address */
#define TCS_I2C_ADDR		((uint8_t) 0x29)

/* TCS34725 Settings Registers */
#define TCS_CMDR_PTR		((uint8_t) 0x80)  /* Command Register */
#define TCS_ENR_PTR		((uint8_t) 0x00)  /* Enable Register */
#define TCS_ATIMR_PTR		((uint8_t) 0x01)  /* RGBC Timing Register */
#define TCS_WTIMR_PTR  		((uint8_t) 0x03)  /* Wait Time Register */
#define TCS_LTLBR_PTR		((uint8_t) 0x04)  /* Low Threshold Low Byte Register */
#define TCS_LTHBR_PTR		((uint8_t) 0x05)  /* Low Threshold High Byte Register */
#define TCS_HTLBR_PTR		((uint8_t) 0x06)  /* High Threshold Low Byte Register */
#define TCS_HTHBR_PTR		((uint8_t) 0x07)  /* High Threshold High Byte Register */
#define TCS_PERSR_PTR		((uint8_t) 0x0C)  /* Persistence Register */
#define TCS_CFGR_PTR		((uint8_t) 0x0D)  /* Configuration Register */
#define TCS_CR_PTR		((uint8_t) 0x0F)  /* Control Register */
#define TCS_IDR_PTR		((uint8_t) 0x12)  /* ID Register */
#define TCS_SR_PTR		((uint8_t) 0x13)  /* Status Register */

/* TCS34725 Data Registers */
#define TCS_CDATA_LBR_PTR	((uint8_t) 0x14)  /* Clear Data Low Byte Register */
#define TCS_CDATA_HBR_PTR	((uint8_t) 0x15)  /* Clear Data High Byte Register */
#define TCS_RDATA_LBR_PTR	((uint8_t) 0x16)  /* Red Data Low Byte Register */
#define TCS_RDATA_HBR_PTR	((uint8_t) 0x17)  /* Red Data High Byte Register */
#define TCS_GDATA_LBR_PTR	((uint8_t) 0x18)  /* Green Data Low Byte Register */
#define TCS_GDATA_HBR_PTR	((uint8_t) 0x19)  /* Green Data High Byte Register */
#define TCS_BDATA_LBR_PTR	((uint8_t) 0x1A)  /* Blue Data Low Byte Register */
#define TCS_BDATA_HBR_PTR	((uint8_t) 0x1B)  /* Blue Data High Byte Register */

/* Bit Definitions for COMMAND REGISTER */
#define TCS_CMDR_RBYTE_PROT	((uint8_t) 0x00) /* Repeated Byte Protocol Transaction */
#define TCS_CMDR_AUTOINC_PROT	((uint8_t) 0x20) /* Auto-increment Protocol Transaction */
#define TCS_CMDR_SF		((uint8_t) 0x60) /* Special Function */
#define TCS_CMDR_CLRINT		((uint8_t) 0x06) /* Clears TCS34725 Interrupts */

/* Bit Definitions for ENABLE REGISTER */
#define TCS_ENR_INTEN		((uint8_t) 0x10) /* Interrupt Enable */
#define TCS_ENR_WEN		((uint8_t) 0x08) /* Wait Enable */
#define TCS_ENR_ADCEN		((uint8_t) 0x02) /* Enable RGBC ADC */
#define TCS_ENR_PON		((uint8_t) 0x01) /* Power On The Sensor */

/* Bit Definitions For PERSISTENCE REGISTER */
#define TCS_PERSR_EVERY		((uint8_t) 0x00) /* Interrupt Generated Every RGBC Cycle */
#define TCS_PERSR_1		((uint8_t) 0x01) /* 1 Clear Value Outside Threshold Gen. Int. */
#define TCS_PERSR_2		((uint8_t) 0x02)
#define TCS_PERSR_3		((uint8_t) 0x03)
#define TCS_PERSR_5		((uint8_t) 0x04)
#define TCS_PERSR_10		((uint8_t) 0x05)
#define TCS_PERSR_15		((uint8_t) 0x06)
#define TCS_PERSR_20		((uint8_t) 0x07)
#define TCS_PERSR_25		((uint8_t) 0x08)
#define TCS_PERSR_30		((uint8_t) 0x09)
#define TCS_PERSR_35		((uint8_t) 0x0A)
#define TCS_PERSR_40		((uint8_t) 0x0B)
#define TCS_PERSR_45		((uint8_t) 0x0C)
#define TCS_PERSR_50		((uint8_t) 0x0D)
#define TCS_PERSR_55		((uint8_t) 0x0E)
#define TCS_PERSR_60		((uint8_t) 0x0F)

/* Bit Definitions For CONFIGURATION REGISTER */
#define TCS_CFGR_WLONG		((uint8_t) 0x02) /* Wait Cycles Increased by Factor of 12 */

/* Bit Definitions For CONTROL REGISTER */
#define TCS_CR_1XGAIN		((uint8_t) 0x00) /* RGBC Gain = 1 */
#define TCS_CR_4XGAIN		((uint8_t) 0x01) /* RGBC Gain = 4 */
#define TCS_CR_16XGAIN		((uint8_t) 0x02) /* RGBC Gain = 16 */
#define TCS_CR_60XGAIN		((uint8_t) 0x03) /* RGBC Gain = 60 */

/* Bit Definitions For STATUS REGISTER */
#define TCS_SR_AINT		((uint8_t) 0x10) /* RGBC Clear Channel Interrupt Flag Set */
#define TCS_SR_AVALID		((uint8_t) 0x01) /* RGBC Data Valid */

void rgb_struct_reset(RGB_Sensor_TypeDef *RGB_SNSR);

void rgb_sensor_init(RGB_Sensor_TypeDef *RGB_SNSR);

void rgb_sensor_threshold_calibration(RGB_Sensor_TypeDef *RGB_SNSR);

void tcs34725_interrupt_clr(void);

#endif
