#include "stm32l476xx.h"  // gives you access to all the masks (like GPIO_ODR_ODR_8) and structures (like RCC)

void pe8_config(void);   // function prototype:  lets main -+ know that this function exists
// Everything happens inside main          		    |
void main(void)// <-----------------------------------------+ 
{
	int i;  // This variable will be used in the for loop below
	pe8_config();// set pe8 to output mode             |
		     //                                    |
	while(1) {   // keeps the programming running      |
		GPIOE->ODR ^= GPIO_ODR_ODR_8;// blinks led |	
		for (i = 0; i < 1000000; i++);// <---------+
	}
}

// When called, this function turns on the clock for IO port E, which is necessary for making the port work
// After that, it sets the mode of pin 8 on port E to output, so it is possible to turn the green LED on
void pe8_config(void)
{
	// Enable clock for IO port E
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN; // RCC is a data structure that contains the "registers" of the Reset and Clock
					     // Control (RCC) peripheral of the microcontroller

	// Set the mode of pin 8 on port E to output	
	GPIOE->MODER &= ~GPIO_MODER_MODER8;
	GPIOE->MODER |= GPIO_MODER_MODER8_0;
}
