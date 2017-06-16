#include <stm32l476xx.h>

void led_init(void);
void led_debug(void);

void sysclk_init_pll(int in_mul, int in_div, int out_div);
void sysclk_init_hsi_16MHz(void);
void sysclk_init_msi(unsigned char range);
void sysclk_init_hse(void);

void pa0_init(void);
void tim2_init(void);

int main(void)
{
	/*
	 *  Use this program to measure the system clock frequency with an oscilloscope.
	 *  Determine the period of the output waveform and divide by 5000.  Then take
	 *  the reciprocal to find the system clock frequency. 
	 */
	unsigned char msi_range;
	msi_range = 9;

	led_init();
//	sysclk_init_msi(msi_range); 
//	sysclk_init_hsi_16MHz();
//	sysclk_init_hse();
	sysclk_init_pll(10, 4, 2);		
	pa0_init();
	tim2_init();

	while (1);
}


void led_init(void)
{
	/*
	 * Configure PB2 for output so we can turn on red led
	 */
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	GPIOB->MODER &= ~GPIO_MODER_MODER2;
	GPIOB->MODER |= GPIO_MODER_MODER2_0;
}

void led_debug(void)
{
	GPIOB->ODR |= GPIO_ODR_ODR_2;
}

void sysclk_init_hse(void)
{
	// Enable HSE clock
	RCC->CR |= RCC_CR_HSEON;	

	// Wait until HSE is ready
	while (RCC->CR & RCC_CR_HSERDY == 0);

	// Select HSE as the system clock
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_HSE;

	// Wait until HSI16 is used as the system clock
	while (RCC->CFGR & RCC_CFGR_SWS == 0);
}

void sysclk_init_pll(int in_mul, int in_div, int out_div)
{
	/*
	   Notes: (1) PLL input frequency must be between 4 and 16 MHz.
	 	  (2) Select input clock and multiplication factor before enabling PLL.
		  (3) PLLM divides clock source by number between 1 and 8.
	 */

	
	/*
         * Setup MSI clock 
	 */
	int range = 6;

	// Set MSI frequency
	RCC->CR &= ~RCC_CR_MSIRANGE;
	RCC->CR |= range << 4;
	
	// Indicate that MSIRANGE value in RCC_CR will be used
	RCC->CR |= RCC_CR_MSIRGSEL;
	
	// Enable MSI clock
	RCC->CR |= RCC_CR_MSION;	

	// Wait until MSI is ready	
	while (RCC->CR & RCC_CR_MSIRDY == 0);


	/*
	 * Setup PLL
	 */
	// Disable the PLL
	RCC->CR &= ~RCC_CR_PLLON;

	// Wait until PLLRDY bit is cleared
	while (RCC->CR & RCC_CR_PLLRDY);

	// Select PLL input clock source (only MSI for now)
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_MSI;
	
	// Set VCO multiplier
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN;
	RCC->PLLCFGR |= in_mul << 8;

	// Set VCO divider
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM;
	RCC->PLLCFGR |= (in_div - 1) << 4;	
			
	// Set main PLL output division factor
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLR;
	RCC->PLLCFGR |= (out_div / 2 - 1) << 25;

	// Enable PLL
	RCC->CR |= RCC_CR_PLLON;
	
	// Wait until PLL is ready
	while (RCC->CR & RCC_CR_PLLRDY == 0);

	// Enable main PLL output
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;	

	// Select PLL as system clock
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;

	// Wait until PLL is used as the system clock
	while (RCC->CFGR & RCC_CFGR_SWS == 0);	
}

void sysclk_init_hsi_16MHz(void)
{
	// Enable HSI16 clock (16 MHz)
	RCC->CR |= RCC_CR_HSION;

	// Wait until HSI16 is ready
	while (RCC->CR & RCC_CR_HSIRDY == 0);
	
	// Select HSI16 as the system clock
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_HSI;

	// Wait until HSI16 is used as the system clock
	while (RCC->CFGR & RCC_CFGR_SWS == 0);
}

void sysclk_init_msi(unsigned char range)
{
	// Ensure valid range used
	if (range > 11 || range < 0) {
		exit(1);	
	}
	
	// Set MSI frequency
	RCC->CR &= ~RCC_CR_MSIRANGE;
	RCC->CR |= range << 4;
	
	// Indicate that MSIRANGE value in RCC_CR will be used
	RCC->CR |= RCC_CR_MSIRGSEL;
	
	// Enable MSI clock
	RCC->CR |= RCC_CR_MSION;	

	// Wait until MSI is ready	
	while (RCC->CR & RCC_CR_MSIRDY == 0);
	
	// Select MSI as the system clock
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_MSI;

	// Wait until MSI is used as the system clock
	while (RCC->CFGR & RCC_CFGR_SWS != 0);

	led_debug();
}

void pa0_init(void)
{
	// Enable IO port A clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	// Configure pa0 for alternative function 1 (tim2_ch1)	
	GPIOA->MODER &= ~GPIO_MODER_MODER0;	
	GPIOA->MODER |= GPIO_MODER_MODER0_1;	
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL0;
	GPIOA->AFR[0] |= (0x01) << (0 * 4);  

	// Configure pa0 as no pull-up, no pull-down
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR0;

	// Configure pa0 for high output speed
	GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR0;
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0_0 | GPIO_OSPEEDER_OSPEEDR0_1;

	// Configure pa0 as push-pull
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT0;
}

void tim2_init(void)
{
	float dc = 1.0 / 2.0; // duty cycle

	// Enable TIM2 clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	
	// Set prescaler to zero so system clock is not divided	
	TIM2->PSC = 0;  
	
	// Configure counter to count to 5000
	TIM2->ARR = 5000;

	// Configure for PWM mode 2
	// Output high as long as TIM2_CNT > CCR1
	TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;
	TIM2->CCMR1 |= TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	
	// Duty cycle will be dc: (1 - CCR1 / ARR) * 100% for PWM Mode 2
	TIM2->CCR1 = (1 - dc) * TIM2->ARR;

	// Enable capture/compare 1
	TIM2->CCER |= TIM_CCER_CC1E;
	
	// Enable the counter
	TIM2->CR1 |= TIM_CR1_CEN;
}
