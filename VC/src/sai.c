/*!
 *
 * @file
 *
 * @brief Implementation of SAI library
 *
 * @author Nrgagnon
 *
 * @date April 20, 2017
 *
 */

#include "../include/sai.h"

void sai1_init(void)
{
	sai1_pins_init();

        // Select SAI1 clock (source = PLLSAI1CLK)
        RCC->CCIPR &= ~RCC_CCIPR_SAI1SEL;

        // Enable SAI1 clock
        RCC->APB2ENR |= RCC_APB2ENR_SAI1EN;

	// Disable interrupts
	SAI1_Block_A->IMR &= ~SAI_xIMR_FREQIE;

        // Configure audio block A in master transmitter mode 
        SAI1_Block_A->CR1 &= ~SAI_xCR1_MODE;

        // Configure audio block A in asynchronous mode
        SAI1_Block_A->CR1 &= ~SAI_xCR1_SYNCEN;

        // Enable master divider
        SAI1_Block_A->CR1 &= ~SAI_xCR1_NODIV;

        // Do not divide master clock
        SAI1_Block_A->CR1 &= ~SAI_xCR1_MCKDIV;

        // Configure for stereo mode
        SAI1_Block_A->CR1 &= ~SAI_xCR1_MONO;

        // Free protocol used
        SAI1_Block_A->CR1 &= ~SAI_xCR1_PRTCFG;

        // Data-size = 16 bits
        SAI1_Block_A->CR1 &= ~SAI_xCR1_DS;
        SAI1_Block_A->CR1 |= 4 << 5;

	// MSB transferred first
        SAI1_Block_A->CR1 &= ~SAI_xCR1_LSBFIRST;

        // Clock strobing on falling edge
        SAI1_Block_A->CR1 &= ~SAI_xCR1_CKSTR;

        // Enable DMA
        SAI1_Block_A->CR1 |= SAI_xCR1_DMAEN;

        // No companding
        SAI1_Block_A->CR2 &= ~SAI_xCR2_COMP;

        // SD output line not released
        SAI1_Block_A->CR2 &= ~SAI_xCR2_TRIS;

        // FIFO threshold 1/4 FIFO
        SAI1_Block_A->CR2 &= ~SAI_xCR2_FTH;
        SAI1_Block_A->CR2 |= 1 << 0;

        // FSOffset 1 bit before
        SAI1_Block_A->FRCR |= SAI_xFRCR_FSOFF;

        // Polarity: active low
        SAI1_Block_A->FRCR &= ~SAI_xFRCR_FSPO;

        // Synchro def
        SAI1_Block_A->FRCR |= SAI_xFRCR_FSDEF;

	// Frame length = 32; must be a power of 2 since NODIV bit was cleared
        SAI1_Block_A->FRCR &= ~SAI_xFRCR_FRL;
        SAI1_Block_A->FRCR |= (32 - 1) << 0;

        // Active frame length = 16
        SAI1_Block_A->FRCR &= ~SAI_xFRCR_FSALL;
        SAI1_Block_A->FRCR |= (16 - 1) << 8;

        // First bit offset = 0 
        SAI1_Block_A->SLOTR &= ~SAI_xSLOTR_FBOFF;

        // slot size equivalent to data size
        SAI1_Block_A->SLOTR &= ~SAI_xSLOTR_SLOTSZ;

        // number of slots = 2
        SAI1_Block_A->SLOTR &= ~SAI_xSLOTR_NBSLOT;
        SAI1_Block_A->SLOTR |= (2 + 1) << 8;

        // make slots active
        SAI1_Block_A->SLOTR &= ~SAI_xSLOTR_SLOTEN;
        SAI1_Block_A->SLOTR |= 3 << 16;

        // Enable output drive
        SAI1_Block_A->CR1 |= SAI_xCR1_OUTDRIV;

        // Enable SAI1 block A
        SAI1_Block_A->CR1 |= SAI_xCR1_SAIEN;
}

static void sai1_pins_init(void)
{
        /*
         * Configure PE2 for SAI1_MCLK_A (AF 13)
         */
        GPIOE->MODER &= ~GPIO_MODER_MODER2;
        GPIOE->MODER |= GPIO_MODER_MODER2_1;
        GPIOE->AFR[0] &= ~GPIO_AFRL_AFRL2;
        GPIOE->AFR[0] |= 13 << (2 * 4);

        // Configure as no pull-up, no pull-down 
        GPIOE->PUPDR &= ~GPIO_PUPDR_PUPDR2;

        // Configure for very high output frequency
        GPIOE->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR2;
        GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2_0 | GPIO_OSPEEDER_OSPEEDR2_1;

        /*
         * Configure PE4 for SAI1_FS_A (AF 13)
         */
        GPIOE->MODER &= ~GPIO_MODER_MODER4;
        GPIOE->MODER |= GPIO_MODER_MODER4_1;
        GPIOE->AFR[0] &= ~GPIO_AFRL_AFRL4;
        GPIOE->AFR[0] |= 13 << (4 * 4);

	// Configure as no pull-up, no pull-down 
        GPIOE->PUPDR &= ~GPIO_PUPDR_PUPDR4;

        // Configure for very high output frequency
        GPIOE->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR4;
        GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR4_0 | GPIO_OSPEEDER_OSPEEDR4_1;

        /*
         * Configure PE5 for SAI1_SCK_A (AF 13)
         */
        GPIOE->MODER &= ~GPIO_MODER_MODER5;
        GPIOE->MODER |= GPIO_MODER_MODER5_1;
        GPIOE->AFR[0] &= ~GPIO_AFRL_AFRL5;
        GPIOE->AFR[0] |= 13 << (5 * 4);

        // Configure as no pull-up, no pull-down 
        GPIOE->PUPDR &= ~GPIO_PUPDR_PUPDR5;

        // Configure for very high output frequency
        GPIOE->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR5;
        GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5_0 | GPIO_OSPEEDER_OSPEEDR5_1;

        /*
         * Configure PE6 for SAI1_SD_A (AF 13)
         */
        GPIOE->MODER &= ~GPIO_MODER_MODER6;
        GPIOE->MODER |= GPIO_MODER_MODER6_1;
        GPIOE->AFR[0] &= ~GPIO_AFRL_AFRL6;
        GPIOE->AFR[0] |= 13 << (6 * 4);

        // Configure as no pull-up, no pull-down 
        GPIOE->PUPDR &= ~GPIO_PUPDR_PUPDR6;

        // Configure for very high output frequency
        GPIOE->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR6;
        GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6_0 | GPIO_OSPEEDER_OSPEEDR6_1; 
}
