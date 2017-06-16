/**
  **********************************************************************************
  * @file    RGB_SENSOR/src/led.c
  * @author  Nrgagnon 
  * @version V1.6
  * @date    29-May-2017
  * @brief   Driver for controlling RGB LEDs.
  *
  **********************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. H. Gagnon </center></h2>
  *
  **********************************************************************************
  */

/* Includes ----------------------------------------------------------------------*/
#include "../include/led.h"

/* Function Implementations ------------------------------------------------------*/


/**
  * @brief Converts an RGB hexadecimal code to a GRB hexadecimal code.
  * @param rgb_code : RGB hexadecimal code to be converted.
  * @retval rgb2grb : GRB hexadecimal code.
  */
uint32_t rgb2grb(uint32_t rgb_code)
{
    uint32_t grb;  // green-red-blue color code
    grb = (rgb_code & 0x0000FF) | ((rgb_code >> 8) & 0x00FF00) | ((rgb_code << 8) & 0xFF0000);
    return grb;
}

/**
  * @brief Sets all LEDs from <start> to <stop> to <color>.
  * @param ccr_buff : Pointer to memory region which contains
  *		      CCR values for the TIMER controlling the
  *		      PWM waveform being sent to the LEDs.
  * @param start : Index of first LED to set to <color>.
  * @param stop : Index of last LED to set to <color>.
  * @param color : GRB hexadecimal code corresponding to desired color.
  * @retval None
  */
void set_color(uint16_t *restrict ccr_buff, size_t start, size_t stop, uint32_t color)
{   
    size_t i;  // outer loop idx 
    size_t j;  // inner loop idx
    uint32_t cmask = 0x01U << (NUM_COLOR_BITS - 1);  // basic mask for examining color hex code
    
    for (i = start - 1; i < stop; i++) {
        for (j = 0; j < NUM_COLOR_BITS; j++) {  // check for 1s in the color hex code
            if (color & (cmask >> j)) 
                ccr_buff[(NUM_COLOR_BITS * i) + j] = 12U; // PWM pulse-on time = 0.75 microseconds (data bit = 1)
					       // Meets spec. given in WS2812 datasheet (pg. 4)
            else
                ccr_buff[(NUM_COLOR_BITS * i) + j] = 6U; // PWM pulse-on time = 0.375 microseconds (data bit = 0)
					      // Meets spec. given in WS2812 datasheet (pg. 4)
        }
    }
}

/**
  * @brief Cycles the LEDs in the LED strip through the colors
  *        of the rainbow, starting at red.
  * @param None
  * @retval None
  */
void color_transition_demo(uint16_t *restrict ccr_buff)
{
    size_t i;  // loop idx
    static uint8_t red_mask = 0xFF;   // bits 16-23 of the RGB hex code 
    static uint8_t green_mask = 0x00;  // bits 8-15 of the RGB hex code 
    static uint8_t blue_mask = 0x00;  // bits 0-7 of the RGB hex code 
    uint32_t bg_color = (red_mask << 16) | (green_mask << 8) | (blue_mask << 0);  // color of the entire strip
	
    // Set RGB masks depending on which part of rainbow is being displayed at the moment
    if (red_mask == 0xFF && green_mask == 0x00 && blue_mask != 0xFF)
        blue_mask++;
    else if (red_mask != 0x00 && green_mask == 0x00 && blue_mask == 0xFF)
        red_mask--;
    else if (red_mask == 0x00 && blue_mask == 0xFF && green_mask != 0xFF)
        green_mask++;
    else if (red_mask == 0x00 && green_mask == 0xFF && blue_mask != 0x00)
        blue_mask--;
    else if (red_mask != 0xFF && green_mask == 0xFF && blue_mask == 0x00)
        red_mask++;
    else if (red_mask == 0xFF && green_mask != 0x00 && blue_mask == 0x00)
        green_mask--;

    bg_color &= ~0xFFFFFF;  // Clear the background color
    bg_color |= (red_mask << 16) | (green_mask << 8) | (blue_mask << 0);  // Set background color using masks

    set_color(ccr_buff, 1, NUM_LEDS, rgb2grb(bg_color));  /* Load buffer with CCR values that will create
							     the specified background color */
	
    /* Pause before updating buffer again :
		Determines LED transition speed */
    for(i = 0; i < 50; i++){
            while(DMA1_Channel2->CNDTR < NUM_PERIODS_FOR_RESET + (NUM_LEDS * NUM_COLOR_BITS));
    }		
}

/**
  * @brief Red streaks of color "bounce" back and forth
  *        on each half of the LED strip.
  * @param None
  * @retval None
  */
void color_bounce_demo(uint16_t *restrict ccr_buff)
{
	static unsigned int bg_color = 0xFFFFFF;
        static unsigned int left_color = 0x0000FF;
        static unsigned int right_color = 0xFF0000;

        static unsigned char count_left = 1;
        static unsigned char count_right = NUM_LEDS;

        static char step_left = 1;
        static char step_right = -1;

        static enum direction dir_left = up;
        static enum direction dir_right = down;

	size_t k;

       	if (dir_left == up)
          	set_color(ccr_buff, count_left, count_left, rgb2grb(left_color));
       	else
            	set_color(ccr_buff, count_left, count_left, rgb2grb(bg_color));

       	if (dir_right == down)
           	set_color(ccr_buff, count_right, count_right, rgb2grb(right_color));
       	else
            	set_color(ccr_buff, count_right, count_right, rgb2grb(bg_color));

        count_left += step_left;
      	count_right += step_right;
	
	if (count_left >= NUM_LEDS / 2 + 1) {
            	step_left = -step_left;
            	count_left += step_left;
            	dir_left = down;
       	} else if (count_left <= 0) {
            	step_left = -step_left;
            	count_left += step_left;
            	dir_left = up;
       	}

       	if (count_right <= NUM_LEDS / 2){
            	step_right = -step_right;
            	count_right += step_right;
            	dir_right = up;
       	} else if (count_right >= NUM_LEDS + 1){
            	step_right = -step_right;
            	count_right += step_right;
            	dir_right = down;
       	}

       	for(k = 0; k < 40; k++){
            	while (DMA1_Channel2->CNDTR < NUM_PERIODS_FOR_RESET + (NUM_LEDS * NUM_COLOR_BITS));
       	}
}

/**
  * @brief A streak of color travels up and down the LED strip,
  *        cycling through the colors of the rainbow as it goes.
  * @param None 
  * @retval None
  */
void travel_change_demo(uint16_t *restrict ccr_buff)
{
        size_t i;  // loop idx 
        static uint8_t red_mask = 0xFF;   // bits 16-23 of the RGB number
        static uint8_t green_mask = 0x00;  // bits 8-15 of the RGB number
        static uint8_t blue_mask = 0x00;  // bits 0-7 of the RGB number
        uint32_t traveling_color = (red_mask << 16) | (green_mask << 8) | (blue_mask << 0);
        static uint32_t bg_color = 0x000000;
        static uint8_t step = 1;
        static uint8_t position = 1;

    if (red_mask == 0xFF && green_mask == 0x00 && blue_mask != 0xFF)
        blue_mask++;
    else if (red_mask != 0x00 && green_mask == 0x00 && blue_mask == 0xFF)
        red_mask--;
    else if (red_mask == 0x00 && blue_mask == 0xFF && green_mask != 0xFF)
        green_mask++;
    else if (red_mask == 0x00 && green_mask == 0xFF && blue_mask != 0x00)
        blue_mask--;
    else if (red_mask != 0xFF && green_mask == 0xFF && blue_mask == 0x00)
        red_mask++;
    else if (red_mask == 0xFF && green_mask != 0x00 && blue_mask == 0x00)
        green_mask--;

        traveling_color = (red_mask << 16) | (green_mask << 8) | (blue_mask << 0);

        for (i = 1; i <= NUM_LEDS; i++) {
                if ((i < position) || (i > position + 5))
                	set_color(ccr_buff, i, i, rgb2grb(bg_color));
        }

        set_color(ccr_buff, position, position + 5, rgb2grb(traveling_color));

        if (position + 5 >= NUM_LEDS || position < 1)
        	step = -step;

        position += step;

        for(i = 0; i < 50; i++){
            while(DMA1_Channel2->CNDTR < NUM_PERIODS_FOR_RESET + (NUM_LEDS * NUM_COLOR_BITS));
        }
}

/**
  * @brief Updates the color of the RGB strip using data from the
  *	   TCS34725 RGB sensor.
  * @param ccr_buff : Pointer to region of memory that contains
  *	   the CCR values for the TIMER that is controlling the
  *	   PWM waveform being sent to the LED strip.
  * @param color : RGB color detected by the TCS34725 RGB sensor.
  * @retval None
  */
void color_update(uint16_t *restrict ccr_buff, uint32_t color)
{
	size_t i;
	size_t j;
	size_t k;
	
	// Turn on the DMA	
	DMA1_Channel2->CCR |= DMA_CCR_EN;
		
	i = 15;
	j = 15;			

	// Phase 1
	set_color(ccr_buff, i, i, 0x000000);
	
	for (k = 0; k < 40; k++) {
		while (DMA1_Channel2->CNDTR < NUM_PERIODS_FOR_RESET + (NUM_LEDS * NUM_COLOR_BITS));
	}

	while (j > 0 && i < 29) {
		i++;
		j--;
		set_color(ccr_buff, i, i, 0x000000);
		set_color(ccr_buff, j, j, 0x000000);
	
   	        for (k = 0; k < 40; k++) {
            		while (DMA1_Channel2->CNDTR < NUM_PERIODS_FOR_RESET + (NUM_LEDS * NUM_COLOR_BITS));
      		}		
	}

	for (k = 0; k < 200; k++) {
		while (DMA1_Channel2->CNDTR < NUM_PERIODS_FOR_RESET + (NUM_LEDS * NUM_COLOR_BITS));
	}
	
	i = 1;

	set_color(ccr_buff, i, i, rgb2grb(color));

	for (k = 0; k < 10; k++) {
		while (DMA1_Channel2->CNDTR < NUM_PERIODS_FOR_RESET + (NUM_LEDS * NUM_COLOR_BITS));
	}

	while (i < NUM_LEDS) {
		i++;
		set_color(ccr_buff, i, i, rgb2grb(color));
	
   	        for (k = 0; k < 10; k++) {
            		while (DMA1_Channel2->CNDTR < NUM_PERIODS_FOR_RESET + (NUM_LEDS * NUM_COLOR_BITS));
      		}		
	}
	
	// Shut off the DMA to conserve power
	DMA1_Channel2->CCR &= ~DMA_CCR_EN;
}

/**
  * @brief Light show displayed at system start-up
  * @param ccr_buff : Pointer to memory region that
  *	   contains the CCR values for the TIMER
  *	   that controls the PWM waveform being
  *	   sent to the LEDs.
  * @retval None
  */
void start_sequence(uint16_t *restrict ccr_buff)
{
	size_t i = 0;
	size_t k;
	size_t j;
	uint8_t red_mask = 0xFF;
	uint8_t green_mask = 0xFF;
	uint8_t blue_mask = 0xFF;
	uint32_t center_color = red_mask << 16 | green_mask << 8 | blue_mask << 0;
	uint32_t left_color = red_mask << 16 | green_mask << 8 | blue_mask << 0;
	uint32_t right_color = red_mask << 16 | green_mask << 8 | blue_mask << 0;

	size_t r_i = 4;
	int r_dir = -1;

	size_t gl_i = 13;
	int gl_dir = 1;

	size_t gr_i = 17;
	int gr_dir = -1;
	
	size_t b_i = 26;
	int b_dir = 1;

	// Turn on the DMA
	DMA1_Channel2->CCR |= DMA_CCR_EN;

	set_color(ccr_buff, 1, NUM_LEDS, rgb2grb(0x000000));

	for(k = 0; k < 500; k++){
       	     while(DMA1_Channel2->CNDTR < NUM_PERIODS_FOR_RESET + (NUM_LEDS * NUM_COLOR_BITS));
    	}		
	
	while (i++ < 17) {
		left_color = red_mask << 16 | (green_mask - i * 15) << 8 | (blue_mask - i * 15) << 0;
		center_color = (red_mask - i * 15) << 16 | green_mask << 8 | (blue_mask - i * 15) << 0;
		right_color = (red_mask - i * 15) << 16 | (green_mask - i * 15) << 8 | blue_mask << 0;
		set_color(ccr_buff, 1, 3, rgb2grb(left_color));
		set_color(ccr_buff, 4, 13, rgb2grb(0x000000));
		set_color(ccr_buff, 14, 16, rgb2grb(center_color));
		set_color(ccr_buff, 17, 26, rgb2grb(0x000000));
		set_color(ccr_buff, 27, 29, rgb2grb(right_color));
		
   		for(k = 0; k < 30; k++){
       		     while(DMA1_Channel2->CNDTR < NUM_PERIODS_FOR_RESET + (NUM_LEDS * NUM_COLOR_BITS));
    		}		

		if (i == 17) {
			for (j = 0; j < 20; j++) {
				set_color(ccr_buff, 4, 13, rgb2grb(0x000000));
				set_color(ccr_buff, 17, 26, rgb2grb(0x000000));
				set_color(ccr_buff, r_i, r_i, rgb2grb(0xFF0000));			
				set_color(ccr_buff, gl_i, gl_i, rgb2grb(0x00FF00));
				set_color(ccr_buff, gr_i, gr_i, rgb2grb(0x00FF00));
				set_color(ccr_buff, b_i, b_i, rgb2grb(0x0000FF));
				if (r_i == 8 || r_i == 4) {
					r_dir = -r_dir;
				}
				if (gl_i == 9 || gl_i == 13) {
					gl_dir = -gl_dir;
				}
				if (gr_i == 21 || gr_i == 17) {
					gr_dir = -gr_dir;
				}
				if (b_i == 22 || b_i == 26) {
					b_dir = -b_dir;
				}
				r_i += r_dir;
				gl_i += gl_dir;
				gr_i += gr_dir;
				b_i += b_dir;	
		   		for(k = 0; k < 100; k++){
       				     while(DMA1_Channel2->CNDTR < NUM_PERIODS_FOR_RESET + (NUM_LEDS * NUM_COLOR_BITS));
    				}		
	
			}			
		}			
	}
	
	// Shut off the DMA to conserve power
	DMA1_Channel2->CCR &= ~DMA_CCR_EN;
}

/**
  * @brief Half the LEDs transition from blue to red
  *        and half transition from red to blue.
  * @param ccr_buff : ptr to buffer of ccr values which control the duty cycle of the pwm
  * @retval None
  */
void interweave_demo(uint16_t *restrict ccr_buff)
{
    size_t i;
    static uint8_t red_mask_odd = 0xFF;   // bits 16-23 of the RGB hex code 
    static uint8_t green_mask_odd = 0x00;  // bits 8-15 of the RGB hex code 
    static uint8_t blue_mask_odd = 0x00;  // bits 0-7 of the RGB hex code 
    static uint8_t red_mask_even = 0xFF;
    static uint8_t green_mask_even = 0x00;
    static uint8_t blue_mask_even = 0x00;
    uint32_t bg_color_odd = (red_mask_odd << 16) | (green_mask_odd << 8) | (blue_mask_odd << 0);  // color of odd LEDs
    uint32_t bg_color_even = (red_mask_even << 16) | (green_mask_even << 8) | (blue_mask_odd << 0);  // color of even LEDs

    // odd LEDs
    if (red_mask_odd == 0xFF && green_mask_odd == 0x00 && blue_mask_odd != 0xFF)
        blue_mask_odd++;
    else if (red_mask_odd != 0x00 && green_mask_odd == 0x00 && blue_mask_odd == 0xFF)
        red_mask_odd--;
    else if (red_mask_odd == 0x00 && blue_mask_odd == 0xFF && green_mask_odd != 0xFF)
        green_mask_odd++;
    else if (red_mask_odd == 0x00 && green_mask_odd == 0xFF && blue_mask_odd != 0x00)
        blue_mask_odd--;
    else if (red_mask_odd != 0xFF && green_mask_odd == 0xFF && blue_mask_odd == 0x00)
        red_mask_odd++;
    else if (red_mask_odd == 0xFF && green_mask_odd != 0x00 && blue_mask_odd == 0x00)
        green_mask_odd--;
	
    // even LEDs	
    if (red_mask_even == 0xFF && green_mask_even != 0xFF && blue_mask_even == 0x00)
        green_mask_even++;
    else if (red_mask_even != 0x00 && green_mask_even == 0xFF && blue_mask_even == 0x00)
        red_mask_even--;
    else if (red_mask_even == 0x00 && blue_mask_even != 0xFF && green_mask_even == 0xFF)
        blue_mask_even++;
    else if (red_mask_even == 0x00 && green_mask_even != 0x00 && blue_mask_even == 0xFF)
        green_mask_even--;
    else if (red_mask_even != 0xFF && green_mask_even == 0x00 && blue_mask_even == 0xFF)
        red_mask_even++;
    else if (red_mask_even == 0xFF && green_mask_even == 0x00 && blue_mask_even != 0x00)
        blue_mask_even--;

    bg_color_odd &= ~0xFFFFFF;
    bg_color_odd |= (red_mask_odd << 16) | (green_mask_odd << 8) | (blue_mask_odd << 0);

    bg_color_even &= ~0xFFFFFF;
    bg_color_even |= (red_mask_even << 16) | (green_mask_even << 8) | (blue_mask_even << 0);

    for (i = 1; i <= NUM_LEDS; i++) {
	if (i % 2 == 0) 
		set_color(ccr_buff, i, i, rgb2grb(bg_color_even));
	else
        	set_color(ccr_buff, i, i, rgb2grb(bg_color_odd));
    } 

    for(i = 0; i < 25; i++){
            while(DMA1_Channel2->CNDTR < NUM_PERIODS_FOR_RESET + (NUM_LEDS * NUM_COLOR_BITS));
    }		
}
