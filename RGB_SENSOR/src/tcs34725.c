/**
  **********************************************************************************
  * @file    RGB_SENSOR/src/tcs34725.c
  * @author  Nrgagnon 
  * @version V1.6
  * @date    29-May-2017
  * @brief   Driver for the TCS34725 RGB sensor.
  *
  **********************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. H. Gagnon </center></h2>
  *
  **********************************************************************************
  */

/* Includes ----------------------------------------------------------------------*/
#include "../include/tcs34725.h"
#include "../include/i2c.h"

/* Function Implementations ------------------------------------------------------*/

/**
  * @brief Powers up and configures the 
  *	   TCS34725 RGB sensor.
  * @param RGB_SNSR : Pointer to RGB_Sensor_TypeDef Struct
  * @retval None 
  */
void rgb_sensor_init(RGB_Sensor_TypeDef *RGB_SNSR)
{
	/* Container for the hexadecimal values
  		which indicate the settings of the TCS34725 */
	uint8_t reg_settings[17];
	
	// Zero the members of the RGB_Sensor struct	
	rgb_struct_reset(RGB_SNSR);

	/* Initialize the TCS34725 Sensor */
	RGB_SNSR->CMDR |= TCS_CMDR_PTR | TCS_CMDR_AUTOINC_PROT | TCS_ENR_PTR; /* COMMAND REG :
							Second I2C frame to reg. 0x00.
							Auto-increment protocol used so that
							subsequent I2C frames go to
							subsequent registers. */
	reg_settings[0] = RGB_SNSR->CMDR;
	
	// ENABLE REGISTER (0x00) Settings
	RGB_SNSR->ENR |= TCS_ENR_INTEN;  // Enable Interrupts
	RGB_SNSR->ENR |= TCS_ENR_ADCEN;  // Turn on the ADC
	RGB_SNSR->ENR |= TCS_ENR_WEN;  // Enable state machine wait states 
	RGB_SNSR->ENR |= TCS_ENR_PON;  // Turn on the sensor
	reg_settings[1] = RGB_SNSR->ENR;
	
	// RGBC TIMING REGISTER (0x01) Settings 
	RGB_SNSR->ATIMR |= 0x25; // Integration time = 410.4 ms
	reg_settings[2] = RGB_SNSR->ATIMR;
	
	// RESERVED REGISTER (0x02)
	reg_settings[3] = 0x00;
		
	// WAIT TIME REGISTER (0x03) Settings
	RGB_SNSR->WTIMR |= 0xAB; // Wait Time = 204 ms
	reg_settings[4] = RGB_SNSR->WTIMR;

	/* RGBC INTERRUPT THRESHOLD REGISTERS (0x04 - 0x07) Settings :
	 	When CLEAR light values exceed the high
		threshold, the TCS34725 will generate an interrupt */
	RGB_SNSR->LTLBR |= 0x00;  // Low threshold low byte
	reg_settings[5] = RGB_SNSR->LTLBR;
	 
	RGB_SNSR->LTHBR |= 0x00;  // Low threshold high byte
	reg_settings[6] = RGB_SNSR->LTHBR;
		
	RGB_SNSR->HTLBR |= 0x11;  // High theshold low byte
	reg_settings[7] = RGB_SNSR->HTLBR;
		
	RGB_SNSR->HTHBR |= 0x0F;  // High threshold high byte
	reg_settings[8] = RGB_SNSR->HTHBR;
	
	// RESERVED REGISTERS (0x08 - 0x0B)
	reg_settings[9] = 0x00;  
	reg_settings[10] = 0x00; 
	reg_settings[11] = 0x00; 
	reg_settings[12] = 0x00;
	
	// PERSISTENCE REGISTER (0x0C) Settings 
	RGB_SNSR->PERSR |= TCS_PERSR_1;  /* If the high threshold is exceeded
				            once, generate an interrupt */	
	reg_settings[13] = RGB_SNSR->PERSR;
	
	// RESERVED REGISTER (0x0D)
	reg_settings[14] = 0x00; 

	// CONFIGURATION REGISTER (0x0E) Settings	
	RGB_SNSR->CFGR |= 0x00;  // Do not multiply WAIT time by 12
	reg_settings[15] = RGB_SNSR->CFGR;
	
	// CONTROL REGISTER (0x0F) Settings 
	RGB_SNSR->CR |= TCS_CR_4XGAIN;	// Gain = 4X
	reg_settings[16] = RGB_SNSR->CR;
	
	/* Transmit the settings chosen above to the 
		TCS34725 via I2C bus */	
	i2c1_transmit(17, TCS_I2C_ADDR, reg_settings);
	
	while (I2C1->ISR & I2C_ISR_BUSY);
	
	// Clear the TCS34725's interrupt flag for good measure 
	tcs34725_interrupt_clr();
}

/**
  * @brief Clears TCS34725 interrupts.
  * @param None
  * @retval None
  */
void tcs34725_interrupt_clr(void)
{
	// Command to clear TCS34725 interrupts
	uint8_t clrint[1] = {TCS_CMDR_PTR | TCS_CMDR_SF | TCS_CMDR_CLRINT};
	
	// Issue the command
	i2c1_transmit(1, TCS_I2C_ADDR, clrint);
}

/**
  * @brief Calibrates the TCS34725 for the amount of 
  *	   ambient light present at system startup.
  * @param RGB_SNSR : Pointer to struct that represents the TCS34725.
  * @retval None
  */
void rgb_sensor_threshold_calibration(RGB_Sensor_TypeDef *RGB_SNSR)
{

}

/**
  * @brief Zeros all members of the passed RGB_SNSR struct.
  * @param RGB_SNSR : Pointer to struct that reperesents the TCS34725.
  * @retval None
  */
void rgb_struct_reset(RGB_Sensor_TypeDef *RGB_SNSR)
{
	// Reset all members of the RGB_SNSR struct
	RGB_SNSR->CMDR = 0x00;
	RGB_SNSR->ENR = 0x00;
	RGB_SNSR->ATIMR = 0x00;
	RGB_SNSR->WTIMR = 0x00;
	RGB_SNSR->LTLBR = 0x00;
	RGB_SNSR->LTHBR = 0x00;
	RGB_SNSR->HTLBR = 0x00;
	RGB_SNSR->HTHBR = 0x00;
	RGB_SNSR->PERSR = 0x00;
	RGB_SNSR->CFGR = 0x00;
	RGB_SNSR->CR = 0x00;
	RGB_SNSR->IDR = 0x00;
	RGB_SNSR->SR = 0x00;
}
