/**
  **********************************************************************************
  * @file    VOICE_CHANGER/src/main.c 						   *
  * @author  Nolan R. Gagnon 							   *
  * @version V1.0								   *
  * @date    15-June-2017							   *
  * @brief   Software package that uses DFSDM, SAI1, and  DMA to create data path  *
  *	     between the on-board MEMS microphone and the CS43L22 audio codec.     *
  *	     Audio output is filtered using adjustable biquad filter to modify     *
  *	     a human's voice.							   *
  **********************************************************************************
  * @attention							  		   *
  *										   *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. Gagnon </center></h2>	           *
  *										   *
  **********************************************************************************
  */

/* Includes ----------------------------------------------------------------------*/
#include <stm32l476xx.h>
#include <stdlib.h>
#include "../include/cs43l22.h"
#include "../include/debug.h"
#include "../include/clocks.h"
#include "../include/sai.h"
#include "../include/dfsdm.h"
#include "../include/i2c.h"
#include "../include/dma.h"

/* Defines -----------------------------------------------------------------------*/
#define PLAY_BUFF_SIZE		4096
#define RECORD_BUFF_SIZE	2048
#define SaturaLH(N, L, H) (((N)<(L))?(L):(((N)>(H))?(H):(N)))

/* Global variables --------------------------------------------------------------*/
volatile int32_t record_buff[RECORD_BUFF_SIZE];
volatile int16_t play_buff[PLAY_BUFF_SIZE];

Audio_Codec_TypeDef *cs43l22;  // Struct representing the audio codec

/**
  * @brief Entry point (main program)
  * @param None
  * @retval None
  */
int main(void)
{
	unsigned char msi_range = 6;  // 4 MHz
	int pll_n = 40; // Input multiplier for PLL
	int pll_m = 1;  // Input divider for PLLs (PLL, PLLSAI1, PLLSAI2)
	int pll_r = 4;  // Output divider for PLL
	int i, j;

	// Allocate memory for audio codec struct
	cs43l22 = (Audio_Codec_TypeDef *) malloc(sizeof(Audio_Codec_TypeDef));
	
	/* Configure SysTick */
	systick_init(4000);

	led_init();
	
	// Setup clocks
	msi_init(msi_range);  // 4 MHz
	sysclk_init_pll(pll_n, pll_m, pll_r);	// SYSCLK = (4M * 40) / 4 = 40 MHz
	pllsai1_init(48, 17);  // PLLSAI1CLK = (4M * 48) / 17 = 11.294 MHz ~ 11.2896 MHz
	hsi16_init();

	// Setup DFSDM
	dfsdm_dma_init(record_buff, RECORD_BUFF_SIZE);
	dfsdm_init();

	led_debug(1);

	// Setup I2C
	i2c_tx_dma_init();
	i2c1_init();
	
	// Setup SAI
	sai_dma_init(play_buff, PLAY_BUFF_SIZE);
	codec_init(cs43l22);
	
	/* Begin regular conversions of MEMS-microphone data */
	DFSDM_Filter0->CR1 |= DFSDM_CR1_RSWSTART;

	while (1);
}

void DMA1_Channel4_IRQHandler(void)
{
        uint16_t i;

        if (DMA1->ISR & DMA_ISR_HTIF4) {
                for (i = 0; i < 1024; i++) {
                        play_buff[2 * i] = SaturaLH((record_buff[i] >> 8), -32768, 32767);
                        play_buff[2 * i + 1] = play_buff[2 * i];
                }
                DMA1->IFCR |= DMA_IFCR_CHTIF4;
        }

        if (DMA1->ISR & DMA_ISR_TCIF4) {
                for (i = 1024; i < 2048; i++) {
                        play_buff[2 * i] = SaturaLH((record_buff[i] >> 8), -32768, 32767);
                        play_buff[2 * i + 1] = play_buff[2 * i];
                }
                DMA1->IFCR |= DMA_IFCR_CTCIF4;
        }
}
                       
