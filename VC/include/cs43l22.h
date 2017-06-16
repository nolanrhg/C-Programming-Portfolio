/**
  **********************************************************************************
  * @file    VOICE_CHANGER/include/cs43l22.h					   *
  * @author  Nolan R. Gagnon 	 						   *
  * @version V1.0								   *
  * @date    15-June-2017							   *
  * @brief   CS43L22 Application Interface.				 	   *
  *										   *
  **********************************************************************************
  * @attention									   *
  *										   *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. Gagnon  </center></h2>	   *
  *										   *
  **********************************************************************************
  */
#ifndef CS43L22_H
#define CS43L22_H

/***********************************************************************************
 *										   *
 *				AUDIO CODEC STRUCT		                   *
 *										   *
 ***********************************************************************************/
typedef struct
{
	uint8_t CMDR;		/*!< CS43L22 Command Register */
	uint8_t IDR;		/*!< CS43L22 Chip ID Register (HARDWARE REG. IS READ ONLY) */
	uint8_t PWRCTLR1;	/*!< CS43L22 Power Control 1 Register */
	uint8_t PWRCTLR2;	/*!< CS43L22 Power Control 2 Register */
	uint8_t CKCTLR;		/*!< CS43L22 Clocking Control Register */
	uint8_t IFCTLR1;	/*!< CS43L22 Interface Control 1 Register */
	uint8_t IFCTLR2;	/*!< CS43L22 Interface Control 2 Register */
	uint8_t PTHRUSELR1;	/*!< CS43L22 Passthrough A Select Register */
	uint8_t PTHRUSELR2;	/*!< CS43L22 Passthrough B Select Register */
	uint8_t ZCSRR;		/*!< CS43L22 Analog ZC and SR Settings Register */
	uint8_t GANGCTLR;	/*!< CS43L22 Passthrough Gang Control Register */
	uint8_t PBACKCTLR1;	/*!< CS43L22 Playback Control 1 Register */
	uint8_t MISCCTLR;	/*!< CS43L22 Misc. Control Register */
	uint8_t PBACKCTLR2;	/*!< CS43L22 Playback Control 2 Register */
	uint8_t PTHRUVOLR1;	/*!< CS43L22 Passthrough Volume A Register */
	uint8_t PTHRUVOLR2; 	/*!< CS43L22 Passthrough Volume B Register */
	uint8_t PCMVOLR1;	/*!< CS43L22 PCMA Volume Register */
	uint8_t PCMVOLR2;	/*!< CS43L22 PCMB Volume Register */
	uint8_t BFREQR;		/*!< CS43L22 BEEP Freq. and On-time Register */
	uint8_t BVOLR;		/*!< CS43L22 BEEP Vol. and Off-time Register */
	uint8_t BTONER;		/*!< CS43L22 BEEP Tone Configuration Register */
	uint8_t TONECTLR;	/*!< CS43L22 Tone Control Register */
	uint8_t MVOLR1;		/*!< CS43L22 Master A Volume Register */
	uint8_t MVOLR2;		/*!< CS43L22 Master B Volume Register */
	uint8_t HPVOLR1;	/*!< CS43L22 Headphone A Volume Register */
	uint8_t HPVOLR2;	/*!< CS43L22 Headphone B Volume Register */
	uint8_t SPKRVOLR1;	/*!< CS43L22 Speaker A Volume Register */
	uint8_t SPKRVOLR2;	/*!< CS43L22 Speaker B Volume Register */
	uint8_t CHMIXR;		/*!< CS43L22 Channel Mixer & Swap Register */
	uint8_t LIMCTLR1;	/*!< CS43L22 Limit Control 1 (Thresholds) Register */
	uint8_t LIMCTLR2;	/*!< CS43L22 Limit Control 2 (Release Rate) Register */
	uint8_t ATTKRR;		/*!< CS43L22 Limiter Attack Rate Register */
	uint8_t OFCKSTR;	/*!< CS43L22 Overflow and Clock Status Register (HARDWARE REG. IS READ ONLY) */
	uint8_t BATTCOMPR;	/*!< CS43L22 Battery Compensation Register */
	uint8_t VPBATTLR;	/*!< CS43L22 VP Battery Level Register (HARDWARE REG. IS READ ONLY) */
	uint8_t SPKRSTR;	/*!< CS43L22 Speaker Status Register (HARDWARE REG. IS READ ONLY)*/
	uint8_t CPUMPFR;	/*!< CS43L22 Charge Pump Frequency Register */
} Audio_Codec_TypeDef;

/***********************************************************************************
 *										   *
 *				CS43L22 I2C ADDRESS		                   *
 *										   *
 ***********************************************************************************/
#define CS43L22_I2C_ADDR		((uint8_t) 0x4A)


/***********************************************************************************
 *										   *
 *				CS43L22 REGISTER ADDRESSES		           *
 *										   *
 ***********************************************************************************/
#define CS43L22_CMDR_PTR		((uint8_t) 0x00)	
#define CS43L22_IDR_PTR			((uint8_t) 0x01)
#define CS43L22_PWRCTLR1_PTR 		((uint8_t) 0x02)
#define CS43L22_PWRCTLR2_PTR		((uint8_t) 0x04)
#define CS43L22_CKCTLR_PTR		((uint8_t) 0x05)
#define CS43L22_IFCTLR1_PTR		((uint8_t) 0x06)
#define CS43L22_IFCTLR2_PTR		((uint8_t) 0x07)
#define CS43L22_PTHRUSELR1_PTR		((uint8_t) 0x08)
#define CS43L22_PTHRUSELR2_PTR		((uint8_t) 0x09)
#define CS43L22_ZCSRR_PTR		((uint8_t) 0x0A)
#define CS43L22_GANGCTLR_PTR		((uint8_t) 0x0C)
#define CS43L22_PBACKCTLR1_PTR		((uint8_t) 0x0D)
#define CS43L22_MISCCTLR_PTR		((uint8_t) 0x0E)
#define CS43L22_PBACKCTLR2_PTR		((uint8_t) 0x0F)
#define CS43L22_PTHRUVOLR1_PTR		((uint8_t) 0x14)
#define CS43L22_PTHRUVOLR2_PTR		((uint8_t) 0x15)
#define CS43L22_PCMVOLR1_PTR		((uint8_t) 0x1A)
#define CS43L22_PCMVOLR2_PTR		((uint8_t) 0x1B)
#define CS43L22_BFREQR_PTR		((uint8_t) 0x1C)
#define CS43L22_BVOLR_PTR		((uint8_t) 0x1D)
#define CS43L22_BTONER_PTR		((uint8_t) 0x1E)
#define CS43L22_TONECTLR_PTR		((uint8_t) 0x1F)
#define CS43L22_MVOLR1_PTR		((uint8_t) 0x20)
#define CS43L22_MVOLR2_PTR		((uint8_t) 0x21)
#define CS43L22_HPVOLR1_PTR		((uint8_t) 0x22)
#define CS43L22_HPVOLR2_PTR		((uint8_t) 0x23)
#define CS43L22_SPKRVOLR1_PTR		((uint8_t) 0x24)
#define CS43L22_SPKRVOLR2_PTR		((uint8_t) 0x25)
#define CS43L22_CHMIXR_PTR		((uint8_t) 0x26)
#define CS43L22_LIMCTLR1_PTR		((uint8_t) 0x27)
#define CS43L22_LIMCTLR2_PTR		((uint8_t) 0x28)
#define CS43L22_ATTKRR_PTR		((uint8_t) 0x29)
#define CS43L22_OFCKSTR_PTR		((uint8_t) 0x2E)
#define CS43L22_BATTCOMPR_PTR		((uint8_t) 0x2F)
#define CS43L22_VPBATTLR_PTR		((uint8_t) 0x30)
#define CS43L22_SPKRSTR_PTR		((uint8_t) 0x31)
#define CS43L22_CPUMPFR_PTR		((uint8_t) 0x34)

/***********************************************************************************
 *										   *
 *				CS43L22 REGISTER BIT DEFS		           *
 *										   *
 ***********************************************************************************/

/* CMDR ---------------------------------------------------------------------------*/
#define CS43L22_CMDR_AUTOINC		((uint8_t) 0x80)

/* PWRCTLR1 -----------------------------------------------------------------------*/
#define CS43L22_PWRCTLR1_PUP		((uint8_t) 0x9E)
#define CS43L22_PWRCTLR1_PDWN		((uint8_t) 0x01)

/* PWRCTLR2 -----------------------------------------------------------------------*/
#define CS43L22_PWRCTLR2_HPBON 		((uint8_t) 0x80)
#define CS43L22_PWRCTLR2_HPAON		((uint8_t) 0x20)
#define CS43L22_PWRCTLR2_SPKRBON	((uint8_t) 0x08)
#define CS43L22_PWRCTLR2_SPKRAON	((uint8_t) 0x02)

/* CKCTLR -------------------------------------------------------------------------*/
#define CS43L22_CKCTLR_AUTOEN		((uint8_t) 0x80)
#define CS43L22_CKCTLR_SPEED0		((uint8_t) 0x00)
#define CS43L22_CKCTLR_SPEED1		((uint8_t) 0x20)
#define CS43L22_CKCTLR_SPEED2		((uint8_t) 0x40)
#define CS43L22_CKCTLR_SPEED3		((uint8_t) 0x60)
#define CS43L22_CKCTLR_32KGR		((uint8_t) 0x10)
#define CS43L22_CKCTLR_VIDCK		((uint8_t) 0x08)
#define CS43L22_CKCTLR_RATIO0		((uint8_t) 0x00)
#define CS43L22_CKCTLR_RATIO1		((uint8_t) 0x02)
#define CS43L22_CKCTLR_RATIO2		((uint8_t) 0x04)
#define CS43L22_CKCTLR_RATIO3		((uint8_t) 0x06)
#define CS43L22_CKCTLR_MCKDIV2		((uint8_t) 0x01)

/* IFCTLR1 ------------------------------------------------------------------------*/
#define CS43L22_IFCTLR1_MSTR		((uint8_t) 0x80)
#define CS43L22_IFCTLR1_SCKPOLINV	((uint8_t) 0x40)
#define CS43L22_IFCTLR1_DSP		((uint8_t) 0x10)
#define CS43L22_IFCTLR1_DACIF0		((uint8_t) 0x00)
#define CS43L22_IFCTLR1_DACIF1		((uint8_t) 0x04)
#define CS43L22_IFCTLR1_DACIF2		((uint8_t) 0x08)
#define CS43L22_IFCTLR1_DACIF3		((uint8_t) 0x0C)
#define CS43L22_IFCTLR1_AWL0		((uint8_t) 0x00)
#define CS43L22_IFCTLR1_AWL1		((uint8_t) 0x01)
#define CS43L22_IFCTLR1_AWL2		((uint8_t) 0x02)
#define CS43L22_IFCTLR1_AWL3		((uint8_t) 0x03)

/* IFCTLR2 -------------------------------------------------------------------------*/
#define CS43L22_IFCTLR2_SCKEQMCK	((uint8_t) 0x40)	/* Only valid for MCLK = 12 MHz */
#define CS43L22_IFCTLR2_SWINV		((uint8_t) 0x08)

/* PTHRUSELR1 ----------------------------------------------------------------------*/
// Use only the least significant 4 bits of this register

/* PTHRUSELR2 ----------------------------------------------------------------------*/
// Use only the least significant 4 bits of this register

/* ZCSRR ---------------------------------------------------------------------------*/
#define CS43L22_ZCSRR_SRB		((uint8_t) 0x08)
#define CS43L22_ZCSRR_ZCB		((uint8_t) 0x04)
#define CS43L22_ZCSRR_SRA		((uint8_t) 0x02)
#define CS43L22_ZCSRR_ZCA		((uint8_t) 0x01)

/* GANGCTLR ------------------------------------------------------------------------*/
#define CS43L22_GANGCTLR_GANGEN		((uint8_t) 0x80)

/* PBACKCTLR1 ----------------------------------------------------------------------*/
// Headphone gain set with bits 7 - 5
#define CS43L22_PBACKCTLR1_BEQA		((uint8_t) 0x10)
#define CS43L22_PBACKCTLR1_PCMBINV	((uint8_t) 0x08)
#define CS43L22_PBACKCTLR1_PCMAINV	((uint8_t) 0x04)
#define CS43L22_PBACKCTLR1_MBMUTE	((uint8_t) 0x02)
#define CS43L22_PBACKCTLR1_MAMUTE	((uint8_t) 0x01)

/* MISCCTLR ------------------------------------------------------------------------*/
#define CS43L22_MISCCTLR_PTHRUB		((uint8_t) 0x80)
#define CS43L22_MISCCTLR_PTHRUA		((uint8_t) 0x40)
#define CS43L22_MISCCTLR_PMUTEB		((uint8_t) 0x20)
#define CS43L22_MISCCTLR_PMUTEA		((uint8_t) 0x10)
#define CS43L22_MISCCTLR_FREEZE		((uint8_t) 0x08)
#define CS43L22_MISCCTLR_DEEMPH		((uint8_t) 0x04)
#define CS43L22_MISCCTLR_DIGSFT		((uint8_t) 0x02)
#define CS43L22_MISCCTLR_DIGZC		((uint8_t) 0x01)

/* PBACKCTLR2 ----------------------------------------------------------------------*/
#define CS43L22_PBACKCTLR2_HPBMUTE	((uint8_t) 0x80)
#define CS43L22_PBACKCTLR2_HPAMUTE	((uint8_t) 0x40)
#define CS43L22_PBACKCTLR2_SPKBMUTE	((uint8_t) 0x20)
#define CS43L22_PBACKCTLR2_SPKAMUTE	((uint8_t) 0x10)
#define CS43L22_PBACKCTLR2_SPKBEQA	((uint8_t) 0x08)
#define CS43L22_PBACKCTLR2_SPKSWAP	((uint8_t) 0x04)
#define CS43L22_PBACKCTLR2_SPKMONO	((uint8_t) 0x02)
#define CS43L22_PBACKCTLR2_MUTE5050	((uint8_t) 0x01)

/* PTHRUVOLR1 ----------------------------------------------------------------------*/
// All 8 bits available for determining gain

/* PTHRUVOLR2 ----------------------------------------------------------------------*/
// All 8 bits available for determining gain

/* PCMVOLR1 ------------------------------------------------------------------------*/
#define CS43L22_PCMVOLR1_PCM1MUTE       ((uint8_t) 0x80)
// Remaining 7 bits used to determine gain

/* PCMVOLR2 ------------------------------------------------------------------------*/
#define CS43L22_PCMVOLR2_PCM2MUTE	((uint8_t) 0x80)
// Remaining 7 bits used to determine gain

/* BFREQR --------------------------------------------------------------------------*/
// Bits 7 - 4 determine beep frequency (Range:  260.87 Hz [0b0000] --- 2181.82 Hz [0b1111])
// Bits 3 - 0 determine beep on-time (Range: 86 ms [0b0000] --- 5.20 s [0b1111])

/* BVOLR ---------------------------------------------------------------------------*/
// Bits 7 - 5 determine beep off-time (Range:  1.23 s [0b000] --- 10.80 s [0b111])
// Bits 4 - 0 determine beep volume (Range:  -56 dB [0b00111] --- 6.0 dB [0b00110])

/* BTONER --------------------------------------------------------------------------*/
#define CS43L22_BTONER_OFF		((uint8_t) 0x00)
#define CS43L22_BTONER_SINGLE		((uint8_t) 0x40)
#define CS43L22_BTONER_MULTI		((uint8_t) 0x80)
#define CS43L22_BTONER_CONT		((uint8_t) 0xC0)
#define CS43L22_BTONER_BMIXDIS		((uint8_t) 0x20)
#define CS43L22_BTONER_TCF5K		((uint8_t) 0x00)
#define CS43L22_BTONER_TCF7K		((uint8_t) 0x08)
#define CS43L22_BTONER_TCF10K		((uint8_t) 0x10)
#define CS43L22_BTONER_TCF15K		((uint8_t) 0x18)
#define CS43L22_BTONER_BCF50		((uint8_t) 0x00)
#define CS43L22_BTONER_BCF100		((uint8_t) 0x02)
#define CS43L22_BTONER_BCF200		((uint8_t) 0x04)
#define CS43L22_BTONER_BCF250		((uint8_t) 0x06)
#define CS43L22_BTONER_TCEN		((uint8_t) 0x01)

/* TONECTLR -----------------------------------------------------------------------*/
// Bits 7 - 4 determine treble gain (Range: -10.5 dB [0b1111] --- 12.0 dB [0b0000])
// Bits 3 - 0 determine bass gain (Range: -10.5 dB [0b1111] --- 12.0 dB [0b0000])

/* MVOLR1 -------------------------------------------------------------------------*/
// All 8 bits determine A master volume (Range: -102 dB [0b00011001] --- 12.0 dB [0b00011000])

/* MVOLR2 -------------------------------------------------------------------------*/
// All 8 bits determine B master volume (Range:  -102 dB [0b00011001] --- 12.0 dB [0b00011000])

/* HPVOLR1 ------------------------------------------------------------------------*/
// All 8 bits determine headphone A volume (Range: Muted [0b00000001] --- 0 dB [0b00000000])

/* HPVOLR2 ------------------------------------------------------------------------*/
// All 8 bits determine headphone B volume (Range: Muted [0b00000001] --- 0 dB [0b00000000])

/* SPKRVOLR1 ----------------------------------------------------------------------*/
// All 8 bits determine speaker A volume (Range:  Muted [0b00000001] --- 0 dB [0b00000000])

/* SPKRVOLR2 ----------------------------------------------------------------------*/
// All 8 bits determine speaker B volume (Range: Muted [0b00000001] --- 0 dB [0b00000000])

/* CHMIXR -------------------------------------------------------------------------*/
// TODO:  Figure out what the hell this is for (pg. 52 of datasheet).

/* LIMCTLR1 -----------------------------------------------------------------------*/
// Bits 7 - 5 determine threshold gain (Range: -30 dB [0b111] --- 0 dB [0b000])
// Bits 4 - 2 determine cushion threshold gain (Range: -30 dB [0b111] --- 0 dB [0b000])
#define CS43L22_LIMCTLR1_SRDIS		((uint8_t) 0x02)
#define CS43L22_LIMCTLR1_ZCDIS		((uint8_t) 0x01)

/* LIMCTLR2 -----------------------------------------------------------------------*/
#define CS43L22_LIMCTLR2_LIMIT		((uint8_t) 0x80)
#define CS43L22_LIMCTLR2_LIMALL		((uint8_t) 0x40)
// Bits 5 - 0 determine limiter release rate (0b000000 = fastest release rate, 0b111111 = slowest release rate)

/* ATTKRR -------------------------------------------------------------------------*/
// Bits 5 - 0 determine the attack rate (0b000000 = fastest attack, 0b111111 = slowest attack)

/* BATTCOMPR ----------------------------------------------------------------------*/
#define CS43L22_BATTCOMPR_BATTCMPEN	((uint8_t) 0x80)
#define CS43L22_BATTCOMPR_VPMONEN	((uint8_t) 0x40)
// Bits 3 - 0 determine VP reference voltage level (Range: 1.5 V [0b0000] --- 5.0 V [0b1111])

/* CPUMPFR ------------------------------------------------------------------------*/
// Bits 7 - 4 determine charge pump frequency (Range:  (64*Fs)*2 [0b0000] --- (64*Fs)*17 [0b1111])

/***********************************************************************************
 *										   *
 *				AUDIO CODEC FUNCTIONS		                   *
 *										   *
 ***********************************************************************************/

/* Powers up the audio codec */
void codec_init(Audio_Codec_TypeDef *restrict codec);

/* Configures PE3 as digital output */
void audio_reset_pin_init(void);

/* Resets codec struct members */
void codec_struct_reset(Audio_Codec_TypeDef *restrict codec);
#endif
