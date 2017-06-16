#include "lcd.h"
#include "stm32l476xx.h"
#include <stdint.h>
#include <stdlib.h>



/*-----------------Data structures-----------------*/




/* Constant table for cap characters 'A' --> 'Z' */
const uint16_t CapLetterMap[26] = {
        /* A      B      C      D      E      F      G      H      I  */
        0xFE00,0x6714,0x1d00,0x4714,0x9d00,0x9c00,0x3f00,0xfa00,0x0014,
        /* J      K      L      M      N      O      P      Q      R  */
        0x5300,0x9841,0x1900,0x5a48,0x5a09,0x5f00,0xFC00,0x5F01,0xFC01,
        /* S      T      U      V      W      X      Y      Z  */
        0xAF00,0x0414,0x5b00,0x18c0,0x5a81,0x00c9,0x0058,0x05c0
};

/* Constant table for number '0' --> '9' */
const uint16_t NumberMap[10] = {
        /* 0      1      2      3      4      5      6      7      8      9  */
        0x5F00,0x4200,0xF500,0x6700,0xEa00,0xAF00,0xBF00,0x04600,0xFF00,0xEF00
};



/*----------------------Macros----------------------*/



uint8_t t_bar[2] = {0x00,0x00};

#define BAR0_ON  t_bar[1] |= 8
#define BAR0_OFF t_bar[1] &= ~8
#define BAR1_ON  t_bar[0] |= 8
#define BAR1_OFF t_bar[0] &= ~8
#define BAR2_ON  t_bar[1] |= 2
#define BAR2_OFF t_bar[1] &= ~2
#define BAR3_ON  t_bar[0] |= 2 
#define BAR3_OFF t_bar[0] &= ~2 

#define DOT                   ((uint16_t) 0x8000 ) /* for add decimal point in string */
#define DOUBLE_DOT            ((uint16_t) 0x4000) /* for add decimal point in string */

/* code for '(' character */
#define C_OPENPARMAP          ((uint16_t) 0x0028)

/* code for ')' character */
#define C_CLOSEPARMAP         ((uint16_t) 0x0011)

/* code for 'd' character */
#define C_DMAP                ((uint16_t) 0xf300)

/* code for 'm' character */
#define C_MMAP                ((uint16_t) 0xb210)

/* code for 'n' character */
#define C_NMAP                ((uint16_t) 0x2210)

/* code for 'µ' character */
#define C_UMAP                ((uint16_t) 0x6084)

/* constant code for '*' character */
#define C_STAR                ((uint16_t) 0xA0DD)

/* constant code for '-' character */
#define C_MINUS               ((uint16_t) 0xA000)

/* constant code for '+' character */
#define C_PLUS                ((uint16_t) 0xA014)

/* constant code for '/' */
#define C_SLATCH              ((uint16_t) 0x00c0)

/* constant code for ° */
#define C_PERCENT_1           ((uint16_t) 0xec00)

/* constant code for small o */
#define C_PERCENT_2           ((uint16_t) 0xb300)

#define C_FULL                ((uint16_t) 0xffdd)



/*-------------------Functions--------------------*/



void LCD_PIN_Init(void)
{

	/*--------------------------------Enable port clocks-----------------------------*/
	
	
	
	// Port A
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	// Port B
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	
	// Port C
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	
	// Port D
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
	
	
	
	/*----------------------Configure ports A-D as alt function----------------------*/
	
	
	// Port A, Pins 6,7,8,9,10,15
	GPIOA->MODER &= ~(0xC03FF000);
	GPIOA->MODER |= (0x802AA000);
	
	// Port B, Pins 0,1,5,9,12,13,14,15
	GPIOB->MODER &= ~(0xFF0C0F0F);
	GPIOB->MODER |= (0xAA080A0A);
	
	// Port C, Pins 3,4,5,6,7,8
	GPIOC->MODER &= ~(0x0003FFC0);
	GPIOC->MODER |= (0x0002AA80);
	
	// Port D, Pins 8,9,10,11,12,13,14,15
	GPIOD->MODER &= ~(0xFFFF0000);
	GPIOD->MODER |= (0xAAAA0000);
	
	
	
	/*-------------------Configure ports A-D as alt function 11 (LCD)-----------------*/
	
	
	// Port A, Pins 6,7,8,9,10,15
	GPIOA->AFR[0] &= ~(0xFF000000);
	GPIOA->AFR[0] |= (0xBB000000);
	GPIOA->AFR[1] &= ~(0xF0000FFF);
	GPIOA->AFR[1] |= 0xB0000BBB;
	
	
	// Port B, Pins 0,1,4,5,9,12,13,14,15
	GPIOB->AFR[0] &= ~(0x00FF00FF);
	GPIOB->AFR[0] |= (0x00BB00BB);
	GPIOB->AFR[1] &= ~(0xFFFF00F0);
	GPIOB->AFR[1] |= (0xBBBB00B0);
	
	// Port C, Pins 3,4,5,6,7,8
	GPIOC->AFR[0] &= ~(0xFFFFFF00);
	GPIOC->AFR[0] |= (0xBBBBB000);
	GPIOC->AFR[1] &= ~(0xF);
	GPIOC->AFR[1] |= (0xB);
	
	// Port D, Pins 8,9,10,11,12,13,14,15
	GPIOD->AFR[0] &= ~(0x0);
	GPIOD->AFR[0] |= (0x0);
	GPIOD->AFR[1] &= ~(0xFFFFFFFF);
	GPIOD->AFR[1] |= (0xBBBBBBBB);
	
}

void LCD_DisplayString(uint8_t* ptr)
{

	/*
	 *  Displays a string of 6 characters or less
	 */
	
	int index = 0;
	uint8_t* addressOfNextChar;
	uint8_t* addressOfCurrentChar;
	
	
	
	while(*ptr != '\0'){  // As long as the end of the string hasn't been reached, continue processing chars
		addressOfNextChar = ptr+1;  // store address of the next character, just in case the next char is a '.' or ':'
		addressOfCurrentChar = ptr;
		if (*addressOfCurrentChar == '.' || *addressOfCurrentChar == ':'){
				ptr++;
			continue;  /* If the current char is a '.' or a ':', then skip this iteration of the loop
			            * the '.' or ':' was already included at the end of the previously displayed char
									*/
		}
		if (*addressOfNextChar == '.' && index < 4){  /* Index < 4 because only the first four positions on the LCD can display
																									 * '.' and ':'
																									 */
			LCD_WriteChar(addressOfCurrentChar, 1, 0, index);  /* If the next char in the input string is '.'
																													* then include it as part of the current char
																													*/
		} else if (*addressOfNextChar == ':' && index < 4) {
			LCD_WriteChar(addressOfCurrentChar, 0, 1, index);	  /* If the next char in the input string is ':'
																													* then include it as part of the current char
																													*/
		} else {
			LCD_WriteChar(addressOfCurrentChar, 0, 0, index);  // No '.' or ':' to include
		}
		index++;  
		ptr++;  // Go to address of next char in the string
	}
}


void LCD_DisplayScrollingString(uint8_t* ptr)
{

	 /*
		* Displays a string of any length using scrolling text
		*/
	
	
	// index variables for loops used to implement this function
	int i;
	int k;
	int s;
	int index = 0;
	
	// Dynamically allocate an array to store 6 chars
	static uint8_t *nextSixChars;            
	nextSixChars = malloc(6*sizeof(uint8_t));
	
	// Initialize the array with the first 6 characters of the input string
	for(i = 0; i < 6; i++){
		nextSixChars[i] = *(ptr+i);
	}

	while(*ptr != '\0'){  // Until we reach the end of the string
		
		ptr++;
		LCD_Clear();
		LCD_DisplayString(nextSixChars);  // Display the chars currently in the array
		for(k = 0; k <6; k++){
			if (*(ptr+k) == '\0'){  // Never access memory addresses that you do not own
				for(i = k; i < 6; i++){
					nextSixChars[k] = ' ';  // Add blank spaces to the end of the string for better scrolling effect
				}
				break;
			} else {
				nextSixChars[k] = *(ptr+k);  // Shift first char out of the array and shift each subseq. char down
																		 // then append next char of string at the of the array
			}
		}
		
		for(s = 0; s < 7000000000; s++);  // A delay that makes it possible for a user to read the scrolling text
		
	}
	
	// Keep the screen blank for a few seconds
	for(i = 0; i < 6; i ++){
		nextSixChars[i] = ' ';
	}
	LCD_DisplayString(nextSixChars);
	for(s = 0; s < 4000000; s++);
	LCD_DisplayString(nextSixChars);
	for(s = 0; s < 4000000; s++);
	LCD_DisplayString(nextSixChars);
	for(s = 0; s < 4000000; s++);
	
	// Free the memory allocated for the array
	free(nextSixChars);
	
}

void LCD_Display_Name(void)
{
	
	/*
	 *  Displays my last name ("GAGNON") on the LCD screen
	 */
		
	int i;
	
	LCD_Clear();
	
	// Check to see if the data in LCD_RAM is currently protected
	// If it is, then wait until it is unprotected
	while(LCD->SR & LCD_SR_UDR);
	
	// Update the bits in LCD_RAM
	LCD->RAM[0] |= (0xA182B270);
	LCD->RAM[1] |= (0x9);
	LCD->RAM[2] |= (0x77C0F138);
	LCD->RAM[3] |= (0xE);
	LCD->RAM[4] |= (0x0);
	LCD->RAM[5] |= (0x0);
	LCD->RAM[6] |= (0xC4020000);
	LCD->RAM[7] |= (0x0);
	
	// After writing to the LCD_RAM, set the UDR bit to protect previously written data and
	// to request an update to the LCD screen
	LCD->SR |= LCD_SR_UDR;
	
	// Wait for the screen update to complete
	while(!(LCD->SR & LCD_SR_UDD));  
	
}


void LCD_Configure(void)
{
	
	LCD->CR &= ~(LCD_CR_BIAS);  // Clear the BIAS bits
	LCD->CR |= (LCD_CR_BIAS_1);  // Set BIAS to 1/3
	
	LCD->CR &= ~(LCD_CR_DUTY);  // Clear the DUTY RATIO bits
	LCD->CR |= (0x03<<2);  // Set DUTY RATIO to 1/4
	
	LCD->FCR &= ~(LCD_FCR_CC); //Clear the contrast bits
	LCD->FCR |= (LCD_FCR_CC); // Set contrast to max value
	
	LCD->FCR &= ~(LCD_FCR_PON);  //Clear the pulse-on period bits
	LCD->FCR |= (LCD_FCR_PON); // Set the PULSE ON PERIOD to 7/ck_ps
	
	LCD->CR &= ~(LCD_CR_MUX_SEG);  //Clear the mux-seg bits
	//LCD->CR |= LCD_CR_MUX_SEG; // Enable the MUX segment of the LCD_CR
	
	LCD->CR &= ~(LCD_CR_VSEL);  // Clear the voltage source selector bits
	LCD->CR |= 0x00000000; // Select internal voltage source
	
	while(!(LCD->SR & LCD_SR_FCRSR)); // wait until flag is set
	
	LCD->CR &= ~(LCD_CR_LCDEN);
	LCD->CR |= LCD_CR_LCDEN; 

	while(!(LCD->SR & LCD_SR_ENS));
	while(!(LCD->SR & LCD_SR_RDY));
	
}




/*---------------Professor Zhu's Code-----------------*/



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not change the code below
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LCD_Initialization(void){
	LCD_PIN_Init();
	LCD_Clock_Init();	
	LCD_Configure();
	LCD_Clear();
}


void LCD_Clock_Init(void){
	// Enable write access to Backup domain
	if ( (RCC->APB1ENR1 & RCC_APB1ENR1_PWREN) == 0)
		RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;	// Power interface clock enable
	(void) RCC->APB1ENR1;  // Delay after an RCC peripheral clock enabling
	
	// Select LSE as RTC clock soucre 
	if ( (PWR->CR1 & PWR_CR1_DBP) == 0) {
		PWR->CR1  |= PWR_CR1_DBP;				  			// Enable write access to Backup domain
		while((PWR->CR1 & PWR_CR1_DBP) == 0);  	// Wait for Backup domain Write protection disable
	}
	
	// Reset LSEON and LSEBYP bits before configuring the LSE
	RCC->BDCR &= ~(RCC_BDCR_LSEON | RCC_BDCR_LSEBYP);

	// RTC Clock selection can be changed only if the Backup Domain is reset
	RCC->BDCR |=  RCC_BDCR_BDRST;
	RCC->BDCR &= ~RCC_BDCR_BDRST;
	
	// Note from STM32L4 Reference Manual: 	
  // RTC/LCD Clock:  (1) LSE is in the Backup domain. (2) HSE and LSI are not.	
	while((RCC->BDCR & RCC_BDCR_LSERDY) == 0){  // Wait until LSE clock ready
		RCC->BDCR |= RCC_BDCR_LSEON;
	}
	
	// Select LSE as RTC clock source
	// BDCR = Backup Domain Control Register 
	RCC->BDCR	&= ~RCC_BDCR_RTCSEL;	  // RTCSEL[1:0]: 00 = No Clock, 01 = LSE, 10 = LSI, 11 = HSE
	RCC->BDCR	|= RCC_BDCR_RTCSEL_0;   // Select LSE as RTC clock	
	
	RCC->APB1ENR1 &= ~RCC_APB1ENR1_PWREN;	// Power interface clock disable
	
	// Wait for the external capacitor Cext which is connected to the VLCD pin is charged (approximately 2ms for Cext=1uF) 
	
	// Enable LCD peripheral Clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_LCDEN;
	
	// Enable SYSCFG 
	// RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
}
	

void LCD_Clear(void){
  uint8_t counter = 0;

  // Wait until LCD ready */  
	while ((LCD->SR & LCD_SR_UDR) != 0); // Wait for Update Display Request Bit
  
  for (counter = 0; counter <= 15; counter++) {
    LCD->RAM[counter] = 0;
  }

  /* Update the LCD display */
	LCD->SR |= LCD_SR_UDR; 
}


// Setting bar on LCD, writes bar value in LCD frame buffer 
void LCD_bar(void) {

	// TO wait LCD Ready *
  while ((LCD->SR & LCD_SR_UDR) != 0); // Wait for Update Display Request Bit
	// Bar 0: COM3, LCD_SEG11 -> MCU_LCD_SEG8
	// Bar 1: COM2, LCD_SEG11 -> MCU_LCD_SEG8
	// Bar 2: COM3, LCD_SEG9 -> MCU_LCD_SEG25
	// Bar 3: COM2, LCD_SEG9 -> MCU_LCD_SEG25
	
  LCD->RAM[4] &= ~(1U << 8 | 1U << 25);
  LCD->RAM[6] &= ~(1U << 8 | 1U << 25);
	
  /* bar1 bar3 */
  if (BAR0_ON)
		LCD->RAM[6] |= 1U << 8;
  
  if (BAR1_ON)
		LCD->RAM[4] |= 1U << 8;
 
	if (BAR2_ON)
		LCD->RAM[6] |= 1U << 25;
  
  if (BAR1_ON)
		LCD->RAM[4] |= 1U << 25;
	
	LCD->SR |= LCD_SR_UDR; 
}

/**
  * @brief  Converts an ascii char to the a LCD digit.
  * @param  c: a char to display.
  * @param  point: a point to add in front of char
  *         This parameter can be: POINT_OFF or POINT_ON
  * @param  colon : flag indicating if a colon has to be add in front
  *         of displayed character.
  *         This parameter can be: colon_OFF or colon_ON.
	* @param 	digit array with segment 
  * @retval None
  */
static void LCD_Conv_Char_Seg(uint8_t* c, char point, char colon, uint8_t* digit) {
  uint16_t ch = 0 ;
  uint8_t loop = 0, index = 0;
  
  switch (*c)
    {
    case ' ' :
      ch = 0x00;
      break;

    case '*':
      ch = C_STAR;
      break;

    case '(' :
      ch = C_OPENPARMAP;
      break;

    case ')' :
      ch = C_CLOSEPARMAP;
      break;
      
    case 'd' :
      ch = C_DMAP;
      break;
    
    case 'm' :
      ch = C_MMAP;
      break;
    
    case 'n' :
      ch = C_NMAP;
      break;

    case 'µ' :
      ch = C_UMAP;
      break;

    case '-' :
      ch = C_MINUS;
      break;

    case '+' :
      ch = C_PLUS;
      break;

    case '/' :
      ch = C_SLATCH;
      break;  
      
    case '°' :
      ch = C_PERCENT_1;
      break;  
		
    case '%' :
      ch = C_PERCENT_2; 
      break;
		
    case 255 :
      ch = C_FULL;
      break ;
    
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':			
      ch = NumberMap[*c-0x30];		
      break;
          
    default:
      /* The character c is one letter in upper case*/
      if ( (*c < 0x5b) && (*c > 0x40) )
      {
        ch = CapLetterMap[*c-'A'];
      }
      /* The character c is one letter in lower case*/
      if ( (*c <0x7b) && ( *c> 0x60) )
      {
        ch = CapLetterMap[*c-'a'];
      }
      break;
  }
       
  /* Set the digital point can be displayed if the point is on */
  if (point)
  {
    ch |= 0x0002;
  }

  /* Set the "COL" segment in the character that can be displayed if the colon is on */
  if (colon)
  {
    ch |= 0x0020;
  }		

	for (loop = 12,index=0 ;index < 4; loop -= 4,index++)
  {
    digit[index] = (ch >> loop) & 0x0f; /*To isolate the less significant digit */
  }
	
}


/**
  * @brief Write a character in the LCD frame buffer
  * @param  ch: the character to display.
  * @param  Point: a point to add in front of char
  * @param  Colon: flag indicating if a colon character has to be added in front
  *         of displayed character.         
  * @param  Position: position in the LCD of the character to write [1:6]
	*/

void LCD_WriteChar(uint8_t* ch, char point, char colon, uint8_t position){
  uint8_t digit[4];     /* Digit frame buffer */
   
  // Convert displayed character in segment in array digit 
  LCD_Conv_Char_Seg(ch, point, colon, digit);

  // TO wait LCD Ready *
  while ((LCD->SR & LCD_SR_UDR) != 0); // Wait for Update Display Request Bit
  
  switch (position) {
		
    /* Position 1 on LCD (digit1)*/
    case 0:
	
			LCD->RAM[0] &= ~( 1U << 4 | 1U << 23 | 1U << 22 | 1U << 3 );
      LCD->RAM[2] &= ~( 1U << 4 | 1U << 23 | 1U << 22 | 1U << 3 );
      LCD->RAM[4] &= ~( 1U << 4 | 1U << 23 | 1U << 22 | 1U << 3 );
      LCD->RAM[6] &= ~( 1U << 4 | 1U << 23 | 1U << 22 | 1U << 3 );
			/* 1G 1B 1M 1E */
      LCD->RAM[0] |= ((digit[0] & 0x1) << 4) | (((digit[0] & 0x2) >> 1) << 23) | (((digit[0] & 0x4) >> 2) << 22) | (((digit[0] & 0x8) >> 3) << 3);
      /* 1F 1A 1C 1D  */
      LCD->RAM[2] |= ((digit[1] & 0x1) << 4) | (((digit[1] & 0x2) >> 1) << 23) | (((digit[1] & 0x4) >> 2) << 22) | (((digit[1] & 0x8) >> 3) << 3);
      /* 1Q 1K 1Col 1P  */
      LCD->RAM[4] |= ((digit[2] & 0x1) << 4) | (((digit[2] & 0x2) >> 1) << 23) | (((digit[2] & 0x4) >> 2) << 22) | (((digit[2] & 0x8) >> 3) << 3);
      /* 1H 1J 1DP 1N  */
      LCD->RAM[6] |= ((digit[3] & 0x1) << 4) | (((digit[3] & 0x2) >> 1) << 23) | (((digit[3] & 0x4) >> 2) << 22) | (((digit[3] & 0x8) >> 3) << 3);

			break;

    /* Position 2 on LCD (digit2)*/
    case 1:
			
			LCD->RAM[0] &= ~( 1U << 6 | 1U << 13 | 1U << 12 | 1U << 5 );
      LCD->RAM[2] &= ~( 1U << 6 | 1U << 13 | 1U << 12 | 1U << 5 );
      LCD->RAM[4] &= ~( 1U << 6 | 1U << 13 | 1U << 12 | 1U << 5 );
      LCD->RAM[6] &= ~( 1U << 6 | 1U << 13 | 1U << 12 | 1U << 5 );
			/* 2G 2B 2M 2E */
      LCD->RAM[0] |= ((digit[0] & 0x1) << 6) | (((digit[0] & 0x2) >> 1) << 13) | (((digit[0] & 0x4) >> 2) << 12) | (((digit[0] & 0x8) >> 3) << 5);
      /* 2F 2A 2C 2D  */
      LCD->RAM[2] |= ((digit[1] & 0x1) << 6) | (((digit[1] & 0x2) >> 1) << 13) | (((digit[1] & 0x4) >> 2) << 12) | (((digit[1] & 0x8) >> 3) << 5);
      /* 2Q 2K 2Col 2P  */
      LCD->RAM[4] |= ((digit[2] & 0x1) << 6) | (((digit[2] & 0x2) >> 1) << 13) | (((digit[2] & 0x4) >> 2) << 12) | (((digit[2] & 0x8) >> 3) << 5);
      /* 2H 2J 2DP 2N  */
      LCD->RAM[6] |= ((digit[3] & 0x1) << 6) | (((digit[3] & 0x2) >> 1) << 13) | (((digit[3] & 0x4) >> 2) << 12) | (((digit[3] & 0x8) >> 3) << 5);

			break;
    
    /* Position 3 on LCD (digit3)*/
    case 2:
			
			LCD->RAM[0] &= ~( 1U << 15 | 1U << 29 | 1U << 28 | 1U << 14 );
      LCD->RAM[2] &= ~( 1U << 15 | 1U << 29 | 1U << 28 | 1U << 14 );
      LCD->RAM[4] &= ~( 1U << 15 | 1U << 29 | 1U << 28 | 1U << 14 );
      LCD->RAM[6] &= ~( 1U << 15 | 1U << 29 | 1U << 28 | 1U << 14 );
			/* 3G 3B 3M 3E */
      LCD->RAM[0] |= ((digit[0] & 0x1) << 15) | (((digit[0] & 0x2) >> 1) << 29) | (((digit[0] & 0x4) >> 2) << 28) | (((digit[0] & 0x8) >> 3) << 14);
      /* 3F 3A 3C 3D */
      LCD->RAM[2] |= ((digit[1] & 0x1) << 15) | (((digit[1] & 0x2) >> 1) << 29) | (((digit[1] & 0x4) >> 2) << 28) | (((digit[1] & 0x8) >> 3) << 14);
      /* 3Q 3K 3Col 3P  */
      LCD->RAM[4] |= ((digit[2] & 0x1) << 15) | (((digit[2] & 0x2) >> 1) << 29) | (((digit[2] & 0x4) >> 2) << 28) | (((digit[2] & 0x8) >> 3) << 14);
      /* 3H 3J 3DP  3N  */
      LCD->RAM[6] |= ((digit[3] & 0x1) << 15) | (((digit[3] & 0x2) >> 1) << 29) | (((digit[3] & 0x4) >> 2) << 28) | (((digit[3] & 0x8) >> 3) << 14);

			break;
    
    /* Position 4 on LCD (digit4)*/
    case 3:
			
			LCD->RAM[0] &= ~( 1U << 31 | 1U << 30);
			LCD->RAM[1] &= ~( 1U << 1 | 1U << 0 );
      LCD->RAM[2] &= ~( 1U << 31 | 1U << 30);
			LCD->RAM[3] &= ~( 1U << 1 | 1U << 0 );
      LCD->RAM[4] &= ~( 1U << 31 | 1U << 30);
			LCD->RAM[5] &= ~( 1U << 1 | 1U << 0 );
      LCD->RAM[6] &= ~( 1U << 31 | 1U << 30);
			LCD->RAM[7] &= ~( 1U << 1 | 1U << 0 );
			/* 4G 4B 4M 4E */
      LCD->RAM[0] |= ((digit[0] & 0x1) << 31) | (((digit[0] & 0x8) >> 3) << 30);
			LCD->RAM[1] |= (((digit[0] & 0x2) >> 1) << 1) | (((digit[0] & 0x4) >> 2) << 0);
      /* 4F 4A 4C 4D */
      LCD->RAM[2] |= ((digit[1] & 0x1) << 31) | (((digit[1] & 0x8) >> 3) << 30);
			LCD->RAM[3] |= (((digit[1] & 0x2) >> 1) << 1) | (((digit[1] & 0x4) >> 2) << 0);
      /* 4Q 4K 4Col 4P  */
      LCD->RAM[4] |= ((digit[2] & 0x1) << 31) | (((digit[2] & 0x8) >> 3) << 30);
			LCD->RAM[5] |= (((digit[2] & 0x2) >> 1) << 1) | (((digit[2] & 0x4) >> 2) << 0);
      /* 4H 4J 4DP  4N  */
      LCD->RAM[6] |= ((digit[3] & 0x1) << 31) | (((digit[3] & 0x8) >> 3) << 30);
			LCD->RAM[7] |= (((digit[3] & 0x2) >> 1) << 1) | (((digit[3] & 0x4) >> 2) << 0);

			break;
    
    /* Position 5 on LCD (digit5)*/
    case 4:
			
			LCD->RAM[0] &= ~( 1U << 25 | 1U << 24);
			LCD->RAM[1] &= ~( 1U << 3 | 1U << 2 );
      LCD->RAM[2] &= ~( 1U << 25 | 1U << 24);
			LCD->RAM[3] &= ~( 1U << 3 | 1U << 2 );
      LCD->RAM[4] &= ~( 1U << 25 | 1U << 24 );
			LCD->RAM[5] &= ~( 1U << 3 | 1U << 2 );
      LCD->RAM[6] &= ~( 1U << 25 | 1U << 24 );
			LCD->RAM[7] &= ~( 1U << 3 | 1U << 2 );
			/* 5G 5B 5M 5E */
      LCD->RAM[0] |= (((digit[0] & 0x2) >> 1) << 25) | (((digit[0] & 0x4) >> 2) << 24);
			LCD->RAM[1] |= ((digit[0] & 0x1) << 3) | (((digit[0] & 0x8) >> 3) << 2);
      /* 5F 5A 5C 5D */
      LCD->RAM[2] |= (((digit[1] & 0x2) >> 1) << 25) | (((digit[1] & 0x4) >> 2) << 24);
			LCD->RAM[3] |= ((digit[1] & 0x1) << 3) | (((digit[1] & 0x8) >> 3) << 2);
      /* 5Q 5K 5Col 5P  */
      LCD->RAM[4] |= (((digit[2] & 0x2) >> 1) << 25) | (((digit[2] & 0x4) >> 2) << 24);
			LCD->RAM[5] |= ((digit[2] & 0x1) << 3) | (((digit[2] & 0x8) >> 3) << 2);
      /* 5H 5J 5DP  5N  */
      LCD->RAM[6] |= (((digit[3] & 0x2) >> 1) << 25) | (((digit[3] & 0x4) >> 2) << 24);
			LCD->RAM[7] |= ((digit[3] & 0x1) << 3) | (((digit[3] & 0x8) >> 3) << 2);

			break;
    
    /* Position 6 on LCD (digit6)*/
    case 5:
			
			LCD->RAM[0] &= ~( 1U << 17 | 1U << 8 | 1U << 9 | 1U << 26 );
      LCD->RAM[2] &= ~( 1U << 17 | 1U << 8 | 1U << 9 | 1U << 26 );
      LCD->RAM[4] &= ~( 1U << 17 | 1U << 8 | 1U << 9 | 1U << 26 );
      LCD->RAM[6] &= ~( 1U << 17 | 1U << 8 | 1U << 9 | 1U << 26 );
			/* 6G 6B 6M 6E */
      LCD->RAM[0] |= ((digit[0] & 0x1) << 17) | (((digit[0] & 0x2) >> 1) << 8) | (((digit[0] & 0x4) >> 2) << 9) | (((digit[0] & 0x8) >> 3) << 26);
      /* 6F 6A 6C 6D */
      LCD->RAM[2] |= ((digit[1] & 0x1) << 17) | (((digit[1] & 0x2) >> 1) << 8) | (((digit[1] & 0x4) >> 2) << 9) | (((digit[1] & 0x8) >> 3) << 26);
      /* 6Q 6K 6Col 6P  */
      LCD->RAM[4] |= ((digit[2] & 0x1) << 17) | (((digit[2] & 0x2) >> 1) << 8) | (((digit[2] & 0x4) >> 2) << 9) | (((digit[2] & 0x8) >> 3) << 26);
      /* 6H 6J 6DP  6N  */
      LCD->RAM[6] |= ((digit[3] & 0x1) << 17) | (((digit[3] & 0x2) >> 1) << 8) | (((digit[3] & 0x4) >> 2) << 9) | (((digit[3] & 0x8) >> 3) << 26);

			break;
    
     default:
      break;
  }

  /* Refresh LCD  bar */
  //LCD_bar();

  // Update the LCD display 
	// Set the Update Display Request.
	//
	// Each time software modifies the LCD_RAM, it must set the UDR bit to transfer the updated
	// data to the second level buffer. The UDR bit stays set until the end of the update and during
	// this time the LCD_RAM is write protected.
	//
	// When the display is enabled, the update is performed only for locations for which
	// commons are active (depending on DUTY). For example if DUTY = 1/2, only the
	// LCD_DISPLAY of COM0 and COM1 will be updated.
	LCD->SR |= LCD_SR_UDR; 								// Update display request. Cleared by hardware
	//while ((LCD->SR & LCD_SR_UDD) == 0);	// Wait Until the LCD display is done
	//LCD->CLR &= ~LCD_CLR_UDDC;            // Clear UDD flag
  
}
