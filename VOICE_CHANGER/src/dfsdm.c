/*!
 *
 * @file
 *
 * @brief Implementation of dfsdm library
 *
 * @author Nrgagnon
 *
 * @date June 14, 2017
 *
 */

#include "../include/dfsdm.h"

void dfsdm_init(void)
{
	// Configure pins used to clock and get data from the MP34DT01 microphone	
	dfsdm_pins_init();

	/*
 	 * Configure DFSDM (Channel 2, Filter 0)
	 */

	// Enable DFSDM clock
	RCC->APB2ENR |= RCC_APB2ENR_DFSDMEN;

	// Disable DFSDM interface in order to configure it
	DFSDM_Channel0->CHCFGR1 &= ~DFSDM_CHCFGR1_DFSDMEN;

	// Use audio clock as the output clock source (SAI1 clock = PLLSAI1CLK)
	DFSDM_Channel0->CHCFGR1 |= DFSDM_CHCFGR1_CKOUTSRC;

	// Divide audio clock output by 4 (output clock will be 2.82 MHz)
	DFSDM_Channel0->CHCFGR1 &= ~DFSDM_CHCFGR1_CKOUTDIV;
	DFSDM_Channel0->CHCFGR1 |= (4 - 1) << 16;
	
	// Disable Filter 0	
	DFSDM_Filter0->CR1 &= ~DFSDM_CR1_DFEN;

	// Disable Channel 2
	DFSDM_Channel2->CHCFGR1 &= ~DFSDM_CHCFGR1_CHEN;

        // Select continous conversion mode
	DFSDM_Filter0->CR1 |= DFSDM_CR1_RCONT;


	/* CONFIGURE CHANNEL 2 */
	
	// Config. input data multiplexer (ext. serial inputs)
	DFSDM_Channel2->CHCFGR1 &= ~DFSDM_CHCFGR1_DATMPX;	
	// Choose standard data packing mode (data in INDAT0[15:0]
	DFSDM_Channel2->CHCFGR1 &= ~DFSDM_CHCFGR1_DATPACK;

	// Config. channel input selection (inputs taken from pins of same channel)
	DFSDM_Channel2->CHCFGR1 &= ~DFSDM_CHCFGR1_CHINSEL;

	// Strobe data on rising edge using SPI
	DFSDM_Channel2->CHCFGR1 &= ~DFSDM_CHCFGR1_SITP;

	// Set SPI clock as internal DFSDM_CKOUT
	DFSDM_Channel2->CHCFGR1 &= ~DFSDM_CHCFGR1_SPICKSEL;
	DFSDM_Channel2->CHCFGR1 |= DFSDM_CHCFGR1_SPICKSEL_1;

	// Set channel offset to zero
	DFSDM_Channel2->CHCFGR2 &= ~DFSDM_CHCFGR2_OFFSET;
	
	// Set channel right bit-shift to 2
	DFSDM_Channel2->CHCFGR2 &= ~DFSDM_CHCFGR2_DTRBS;
	DFSDM_Channel2->CHCFGR2 |= 2 << 3;

	
	/* CONFIGURE FILTER 0 */
	
	// Enable fast mode
	DFSDM_Filter0->CR1 |= DFSDM_CR1_FAST;

	// Enable DMA mode
	DFSDM_Filter0->CR1 |= DFSDM_CR1_RDMAEN;

	// Set channel 2 as the regular channel
	DFSDM_Filter0->CR1 &= ~DFSDM_CR1_RCH;
	DFSDM_Filter0->CR1 |= 2 << 24;

	// Select filter order (sinc3)
	DFSDM_Filter0->FCR &= ~DFSDM_FCR_FORD;
	DFSDM_Filter0->FCR |= 3 << 29;  // sinc 3 filter type
	
	// Select oversampling ratio (Fs = 44.1 kHz)
	DFSDM_Filter0->FCR &= ~DFSDM_FCR_FOSR;
	DFSDM_Filter0->FCR |= (64 - 1) << 16;  // 11.294 / (4 * 64) = 44.1 k

	// Set integrator oversampling ratio to 1
	DFSDM_Filter0->FCR &= ~DFSDM_FCR_IOSR;
	DFSDM_Filter0->FCR |= (1 - 1) << 0;

	// Enable filter 0
	DFSDM_Filter0->CR1 |= DFSDM_CR1_DFEN;

	// Enable channel 2
	DFSDM_Channel2->CHCFGR1 |= DFSDM_CHCFGR1_CHEN;

	// Enable DFSDM interface
	DFSDM_Channel0->CHCFGR1 |= DFSDM_CHCFGR1_DFSDMEN;
}

static void dfsdm_pins_init(void)
{
        // Clock IO port E
       	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;

        /*
         * PE9
         */
        // Configure pe9 for alternative function use (AUDIO_CLK, alt. func. 6)
        GPIOE->MODER &= ~GPIO_MODER_MODER9;
        GPIOE->MODER |= GPIO_MODER_MODER9_1;
        GPIOE->AFR[1] &= ~GPIO_AFRH_AFRH1;
        GPIOE->AFR[1] |= 6 << (1 * 4);

        // Configure as pull-down
        GPIOE->PUPDR &= ~GPIO_PUPDR_PUPDR9;
        GPIOE->PUPDR |= GPIO_PUPDR_PUPDR9_1;

        // Configure for very high output frequency
        GPIOE->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR9;
        GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9_0 | GPIO_OSPEEDER_OSPEEDR9_1;

 	/*
         * PE7
         */
        // Configure pe7 for alternative function use (AUDIO_DIN, alt. func. 6)
        GPIOE->MODER &= ~GPIO_MODER_MODER7;
        GPIOE->MODER |= GPIO_MODER_MODER7_1;
        GPIOE->AFR[0] &= ~GPIO_AFRL_AFRL7;
        GPIOE->AFR[0] |= 6 << (7 * 4);

        // Configure as pull-down
        GPIOE->PUPDR &= ~GPIO_PUPDR_PUPDR7;
        GPIOE->PUPDR |= GPIO_PUPDR_PUPDR7_1;

        // Configure for very high output frequency
        GPIOE->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR7;
        GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR7_0 | GPIO_OSPEEDER_OSPEEDR7_1;
}

