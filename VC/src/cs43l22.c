/**
  **********************************************************************************
  * @file    VOICE_CHANGER/src/cs43l22.c					   *
  * @author  Nolan R. Gagnon 							   *
  * @version V1.0							           *
  * @date    15-June-2017							   *
  * @brief   Implementation of CS43L22 functions. 				   *
  *									   	   *
  **********************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. Gagnon </center></h2>
  *
  **********************************************************************************
  */

/* Includes ----------------------------------------------------------------------*/
#include "../include/cs43l22.h"
#include "../include/i2c.h"
#include "../include/clocks.h"
#include "../include/sai.h"

/* Functions ---------------------------------------------------------------------*/

/**
  * @brief Powers on and configures the CS43L22 audio codec.
  * @param codec : Pointer to Audio_Codec_Typedef structure.
  * @retval None
  */
void codec_init(Audio_Codec_TypeDef *restrict codec)
{
	uint8_t reg_settings[38]; // Container for audio codec register settings
	
	// Zero the members of the codec struct	
	codec_struct_reset(codec);

	audio_reset_pin_init();
	/* Codec power-up sequence from page 31 of the datasheet */
	
	// 1. Hold not(RESET) low until power supplies are stable
	GPIOE->ODR &= ~GPIO_ODR_ODR_3;  // RESET == 1
	delay(100);
	
	// 2. Bring not(RESET) high
	GPIOE->ODR |= GPIO_ODR_ODR_3;  // RESET == 0

	// 3. Load desired register settings while keeping PWR_CTRL1 set to 0x01
	
	// CMDR
	codec->CMDR |= CS43L22_CMDR_PTR;
	codec->CMDR |= CS43L22_CMDR_AUTOINC;	
	codec->CMDR |= CS43L22_PWRCTLR2_PTR;
	reg_settings[0] = codec->CMDR;

	// PWRCTLR2
	codec->PWRCTLR2 |= CS43L22_PWRCTLR2_HPBON;
	codec->PWRCTLR2 |= CS43L22_PWRCTLR2_HPAON;
	reg_settings[1] = codec->PWRCTLR2;

	/* CKCTLR :
		    MCLK = 11.2896 MHz (determined by SAI)
	            Fs = 44.1 kHz (Determined by settings below) */
	codec->CKCTLR |= CS43L22_CKCTLR_SPEED1;	
	reg_settings[2] = codec->CKCTLR;

	// IFCTLR1
	codec->IFCTLR1 |= CS43L22_IFCTLR1_DACIF1;
	codec->IFCTLR1 |= CS43L22_IFCTLR1_AWL3;
	reg_settings[3] = codec->IFCTLR1;

	// IFCTLR2
	codec->IFCTLR2 |= CS43L22_IFCTLR2_SWINV;
	reg_settings[4] = codec->IFCTLR2;
	
	// PTHRUSELR1
	codec->PTHRUSELR1 = 0x80;
	reg_settings[5] = codec->PTHRUSELR1;

	// PTHRUSELR2
	codec->PTHRUSELR2 = 0x80;
	reg_settings[6] = codec->PTHRUSELR2;
	
	// ZCSRR
	reg_settings[7] = codec->ZCSRR;

	// RESERVED (0x0B)
	reg_settings[8] = 0x00;  // refer to pg. 35 of ds

	// GANGCTLR
	reg_settings[9] = codec->GANGCTLR;

	// PBACKCTLR1	
	codec->PBACKCTLR1 |= 0x10;
	codec->PBACKCTLR1 |= CS43L22_PBACKCTLR1_PCMAINV;
	codec->PBACKCTLR1 |= CS43L22_PBACKCTLR1_PCMBINV;
	reg_settings[10] = codec->PBACKCTLR1;

	// MISCCTLR
	codec->MISCCTLR |= CS43L22_MISCCTLR_PMUTEA;
	codec->MISCCTLR |= CS43L22_MISCCTLR_PMUTEB;
	reg_settings[11] = codec->MISCCTLR;

	// PBACKCTLR2
	reg_settings[12] = codec->PBACKCTLR2;	
	
	// RESERVED (0x10 --- 0x13)
	reg_settings[13] = 0x00;  // refer to pg. 35 of ds
	reg_settings[14] = 0x00;
	reg_settings[15] = 0x00;
	reg_settings[16] = 0x00;
	
	// PTHRUVOLR1
	codec->PTHRUVOLR1 = 0x7F;
	reg_settings[17] = codec->PTHRUVOLR1;

	// PTHRUVOLR2
	codec->PTHRUVOLR2 = 0x7F;
	reg_settings[18] = codec->PTHRUVOLR2;

	// RESERVED (0x16 --- 0x17)
	reg_settings[19] = 0x00;  // refer to pg. 35 of ds
	reg_settings[20] = 0x00;

	// RESERVED (0x18 --- 0x19)
	reg_settings[21] = 0x80;  // refer to pg. 35 of ds
	reg_settings[22] = 0x80;  

	// PCMVOLR1
	codec->PCMVOLR1 = 0x39;
	reg_settings[23] = codec->PCMVOLR1;

	// PCMVOLR2
	codec->PCMVOLR2 = 0x39;
	reg_settings[24] = codec->PCMVOLR2;
	
	// BFREQR
	codec->BFREQR = 0x50;	
	reg_settings[25] = codec->BFREQR;

	// BVOLR
	codec->BVOLR = 0x07;
	reg_settings[26] = codec->BVOLR;

	// BTONER
	codec->BTONER |= CS43L22_BTONER_OFF;
	reg_settings[27] = codec->BTONER;

	// TONECTLR
	codec->TONECTLR = 0x11;
	reg_settings[28] = codec->TONECTLR;	
	
	// MVOLR1
	codec->MVOLR1 = 0x18;
	reg_settings[29] = codec->MVOLR1;
	
	// MVOLR2
	codec->MVOLR2 = 0x18;
	reg_settings[30] =  codec->MVOLR2;
	
	// HPVOLR1
	codec->HPVOLR1 = 0x00;
	reg_settings[31] = codec->HPVOLR1;

	// HPVOLR2
	codec->HPVOLR2 = 0x00;
	reg_settings[32] = codec->HPVOLR2;

	// SPKRVOLR1
	codec->SPKRVOLR1 = 0x00;
	reg_settings[33] = codec->SPKRVOLR1;

	// SPKRVOLR2
	codec->SPKRVOLR2 = 0x00;
	reg_settings[34] = codec->SPKRVOLR2;

	// CHMIXR
	codec->CHMIXR = 0x00; // Default value
	reg_settings[35] = codec->CHMIXR;

	// LIMCTLR1
	codec->LIMCTLR1 = 0x00; // Default value
	reg_settings[36] = codec->LIMCTLR1;

	// LIMCTLR2
	codec->LIMCTLR2 = 0xFF; // Default value
	reg_settings[37] = codec->LIMCTLR2;

	// ATTKRR
	codec->ATTKRR = 0x00; // Default value
	reg_settings[38] = codec->ATTKRR;
	
	// Transmit these settings via the I2C bus
	i2c1_transmit(39, CS43L22_I2C_ADDR, reg_settings);
	
	// CMDR & BATTCOMPR
	codec->CMDR &= ~0xFF;
	codec->CMDR |= CS43L22_CMDR_PTR;
	codec->CMDR |= CS43L22_BATTCOMPR_PTR;
	reg_settings[0] = codec->CMDR;

	codec->BATTCOMPR = 0x00; // Default value
	reg_settings[1] = codec->BATTCOMPR;	
	
	i2c1_transmit(2, CS43L22_I2C_ADDR, reg_settings);

	// CMDR & CPUMPFR
	codec->CMDR &= ~0xFF;
	codec->CMDR |= CS43L22_CMDR_PTR;
	codec->CMDR |= CS43L22_CPUMPFR_PTR;
	reg_settings[0] = codec->CMDR;

	codec->CPUMPFR = 0x5F;  // Default value
	reg_settings[1] = codec->CPUMPFR;	

	i2c1_transmit(2, CS43L22_I2C_ADDR, reg_settings);

	// 4. Load required init. settings
	reg_settings[0] = 0x00;
	reg_settings[1] = 0x99;
	i2c1_transmit(2, CS43L22_I2C_ADDR, reg_settings);
	
	reg_settings[0] = 0x47;
	reg_settings[1] = 0x80;
	i2c1_transmit(2, CS43L22_I2C_ADDR, reg_settings);	

	reg_settings[0] = 0x32;
	reg_settings[1] = 0x80;
	i2c1_transmit(2, CS43L22_I2C_ADDR, reg_settings);

	reg_settings[0] = 0x32;
	reg_settings[1] = 0x00;	
	i2c1_transmit(2, CS43L22_I2C_ADDR, reg_settings);

	reg_settings[0] = 0x00;
	reg_settings[1] = 0x00;
	i2c1_transmit(2, CS43L22_I2C_ADDR, reg_settings);
	
	// 5. Set PWR_CTRL1 reg to 0x9E (power on)
	reg_settings[0] = 0x02;
	reg_settings[1] = 0x9E;
	i2c1_transmit(2, CS43L22_I2C_ADDR, reg_settings);

	// 6. Apply MCLK
	sai1_init();
}

void audio_reset_pin_init(void)
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
	/* Configure PE3 (pin that is connected to ~RESET pin of codec internally) */
	GPIOE->MODER &= ~GPIO_MODER_MODER3;
	GPIOE->MODER |= GPIO_MODER_MODER3_0; // digital output

	GPIOE->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR3;
	GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3_1;  // high output speed
	
	GPIOE->PUPDR &= ~GPIO_PUPDR_PUPDR3;  // no pull

	GPIOE->OTYPER &= ~GPIO_OTYPER_OT_3;  // push-pull
}

void codec_struct_reset(Audio_Codec_TypeDef *restrict codec)
{
	// Reset all members of the codec struct
	codec->CMDR = 0x00;	
	codec->IDR = 0x00;
	codec->PWRCTLR1 = 0x00;
	codec->PWRCTLR2 = 0x00;
	codec->CKCTLR = 0x00;
	codec->IFCTLR1 = 0x00;
	codec->IFCTLR2 = 0x00;
	codec->PTHRUSELR1 = 0x00;
	codec->PTHRUSELR2 = 0x00;
	codec->ZCSRR = 0x00;
	codec->GANGCTLR = 0x00;
	codec->PBACKCTLR1 = 0x00;
	codec->MISCCTLR = 0x00;
	codec->PBACKCTLR2 = 0x00;
	codec->PTHRUVOLR1 = 0x00;
	codec->PTHRUVOLR2 = 0x00;
	codec->PCMVOLR1 = 0x00;
	codec->PCMVOLR2 = 0x00;
	codec->BFREQR = 0x00;
	codec->BVOLR = 0x00;
	codec->BTONER = 0x00;
	codec->TONECTLR = 0x00;
	codec->MVOLR1 = 0x00;
	codec->MVOLR2 = 0x00;
	codec->HPVOLR1 = 0x00;
	codec->HPVOLR2 = 0x00;
	codec->SPKRVOLR1 = 0x00;
	codec->SPKRVOLR2 = 0x00;
	codec->CHMIXR = 0x00;
	codec->LIMCTLR1 = 0x00;
	codec->LIMCTLR2 = 0x00;
	codec->ATTKRR = 0x00;
	codec->OFCKSTR = 0x00;
	codec->BATTCOMPR = 0x00;
	codec->VPBATTLR = 0x00;
	codec->SPKRSTR = 0x00;
	codec->CPUMPFR = 0x00;
}
