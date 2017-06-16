/*!
 * @file
 *
 * @brief Implementation of LED strip control library
 *
 * @author Nrgagnon
 *
 * @date June 09, 2017
 *
 */

#include "../include/tc74_led.h"

uint32_t rgbw2grbw(uint32_t rgbw_code)
{
    uint32_t grbw;  // green-red-blue-white color code
	
	        // BLUE - WHITE                   GREEN                             RED
    grbw = (rgbw_code & 0x0000FFFF) | ((rgbw_code & 0x00FF0000) << 8) | ((rgbw_code & 0xFF000000) >> 8);

    return grbw;
}

void set_color(uint16_t *restrict ccr_buff, size_t start, size_t stop, uint32_t color)
{   
    size_t i;  // outer loop idx 
    size_t j;  // inner loop idx
    uint32_t cmask = 0x01 << (NUM_COLOR_BITS - 1);  // basic mask for examining color hex code
    
    for (i = start - 1; i < stop; i++) {
        for (j = 0; j < NUM_COLOR_BITS; j++) {  // check for 1s in the color hex code
            if (color & (cmask >> j)) 
                ccr_buff[(NUM_COLOR_BITS * i) + j] = 10; // PWM pulse-on time = 0.63 microseconds (data bit = 1)
            else
                ccr_buff[(NUM_COLOR_BITS * i) + j] = 5; // PWM pulse-on time = 0.31 microseconds (data bit = 0)
        }
    }
}

void set_color_with_temp(uint16_t *restrict ccr_buff, int8_t temp)
{
	size_t i;
	uint8_t blue_mask = 0x00;
	uint8_t red_mask = 0x00;
	uint32_t color;

	DMA1_Channel2->CCR |= DMA_CCR_EN;

	if (temp > 20) {
		if (0x11 + (temp - 20) * 5 > 255)
			red_mask = 0xFF;
		else
			red_mask = 0x11 + (temp - 20) * 5;	
	} else if (temp < 20){
		if (0x11 + (20 - temp) * 5 > 255)
			blue_mask = 0xFF;
		else
			blue_mask = 0x11 + (20 - temp) * 5;
	} else {
		blue_mask = 0x00;
		red_mask = 0x00;
	}

	
	color = (red_mask << 24) | (0x00 << 16) | (blue_mask << 8) | (0x01 << 0);
	
	set_color(ccr_buff, 1, NUM_LEDS, rgbw2grbw(color));

	for (i = 0; i < 40; i++) {
		while (DMA1_Channel2->CNDTR < NUM_PERIODS_FOR_RESET + (NUM_LEDS * NUM_COLOR_BITS));
	}

	DMA1_Channel2->CCR &= ~DMA_CCR_EN;
}

void tc74_startup(uint16_t *restrict ccr_buff)
{
	uint32_t moving_colr = 0x0000FF00;
	uint8_t pos;
	size_t i;
	size_t j;

	DMA1_Channel2->CCR |= DMA_CCR_EN;

	for (pos = 1; pos < 5; pos++) {
		set_color(ccr_buff, 1, 4, rgbw2grbw(0x00000000));
		set_color(ccr_buff, pos, pos, rgbw2grbw(moving_colr));
	        for (i = 0; i < 2000; i++) {
            		while (DMA1_Channel2->CNDTR < NUM_PERIODS_FOR_RESET + (NUM_LEDS * NUM_COLOR_BITS));
		}
        }			
	
	set_color(ccr_buff, 1, 4, 0x00000000);
	
	for (i = 0; i < 2000; i++) {
		while (DMA1_Channel2->CNDTR < NUM_PERIODS_FOR_RESET + (NUM_LEDS * NUM_COLOR_BITS));
	}

	set_color(ccr_buff, 1, 4, rgbw2grbw(moving_colr));

	for (i = 0; i < 2000; i++) {
        	while (DMA1_Channel2->CNDTR < NUM_PERIODS_FOR_RESET + (NUM_LEDS * NUM_COLOR_BITS));
	}

	DMA1_Channel2->CCR &= ~DMA_CCR_EN;
}

void color_transition_demo(uint16_t *restrict ccr_buff)
{
    size_t i;
    static uint8_t red_mask = 0xFF;   // bits 24-31 of the RGBW hex code 
    static uint8_t green_mask = 0x00;  // bits 16-23 of the RGBW hex code 
    static uint8_t blue_mask = 0x00;  // bits 8-15 of the RGBw hex code 
    static uint8_t white_mask = 0x00;  // bits 0-7 of the RGBW hex code
    uint32_t bg_color = (red_mask << 24) | (green_mask << 16) | (blue_mask << 8) | (white_mask << 0);  // color of entire strip

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

    bg_color &= ~0xFFFFFFFF;
    bg_color |= (red_mask << 24) | (green_mask << 16) | (blue_mask << 8) | (white_mask << 0);

    set_color(ccr_buff, 1, NUM_LEDS, rgbw2grbw(bg_color));

    for (i = 0; i < 50; i++) {
            while (DMA1_Channel2->CNDTR < NUM_PERIODS_FOR_RESET + (NUM_LEDS * NUM_COLOR_BITS));
        }		
}

void color_bounce_demo(uint16_t *restrict ccr_buff, uint32_t rcolor, uint32_t lcolor)
{
	static uint32_t bg_color = 0xFFFFFFFF;
        uint32_t left_color = lcolor;
        uint32_t right_color = rcolor;

        static uint8_t count_left = 1;
        static uint8_t count_right = NUM_LEDS;

        static char step_left = 1;
        static char step_right = -1;

        static enum direction dir_left = up;
        static enum direction dir_right = down;

	int k;

       	if (dir_left == up)
          	set_color(ccr_buff, count_left, count_left, rgbw2grbw(left_color));
       	else
            	set_color(ccr_buff, count_left, count_left, rgbw2grbw(bg_color));

       	if (dir_right == down)
           	set_color(ccr_buff, count_right, count_right, rgbw2grbw(right_color));
       	else
            	set_color(ccr_buff, count_right, count_right, rgbw2grbw(bg_color));

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

       	if (count_right <= NUM_LEDS / 2) {
            	step_right = -step_right;
            	count_right += step_right;
            	dir_right = up;
       	} else if (count_right >= NUM_LEDS + 1) {
            	step_right = -step_right;
            	count_right += step_right;
            	dir_right = down;
       	}

       	for (k = 0; k < 300; k++) {
            	while(DMA1_Channel2->CNDTR < NUM_PERIODS_FOR_RESET + (NUM_LEDS * NUM_COLOR_BITS));
       	}
}

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

        for (i = 0; i < 50; i++) {
            while(DMA1_Channel2->CNDTR < NUM_PERIODS_FOR_RESET + (NUM_LEDS * NUM_COLOR_BITS));
        }
}

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

    for (i = 0; i < 25; i++) {
            while (DMA1_Channel2->CNDTR < NUM_PERIODS_FOR_RESET + (NUM_LEDS * NUM_COLOR_BITS));
        }		
}

void strike_demo(uint16_t *restrict ccr_buff, uint32_t stk_colr, uint32_t bg_colr)
{
	size_t i;  // loop idx
	static uint8_t height = 0;
	uint32_t strike_color = stk_colr;
	uint32_t bg_color = bg_colr;
	static enum direction strike_dir = up;
	
	switch (height) {
	case NUM_LEDS:
		strike_dir = down;
		break;
	case 0:
		strike_dir = up;
		break;
	default:
		break;
	}	
	
	// If STRIKE up, speed = FAST		
	if (strike_dir == up) {
		height++;
		if (height < NUM_LEDS) 
			set_color(ccr_buff, 1, NUM_LEDS - height - 1, rgb2grb(bg_color));

		set_color(ccr_buff, NUM_LEDS - height, NUM_LEDS, rgb2grb(strike_color));

		for (i = 0; i < 50; i++) {
			while (DMA1_Channel2->CNDTR < NUM_PERIODS_FOR_RESET + (NUM_LEDS * NUM_COLOR_BITS));
		}

	} else {  // If STRIKE down, speed = SLOW
		height--;
		set_color(ccr_buff, 1, NUM_LEDS - height, rgb2grb(bg_color));

		for (i = 0; i < 300; i++) {
			while (DMA1_Channel2->CNDTR < NUM_PERIODS_FOR_RESET + (NUM_LEDS * NUM_COLOR_BITS));		
		}

	}
}
