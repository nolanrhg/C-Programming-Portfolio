/*!
 * @file
 *
 * @brief LED strip control library
 *
 * @author Nrgagnon
 *
 * @date May 07, 2017
 *
 */

#ifndef LED_H
#define LED_H

/* Number of LEDs in the physical LED strip */
#define NUM_LEDS 			 30
/* Number of RGB bits used to control color of LED */
#define NUM_COLOR_BITS			 24
/* Number of counter periods needed for the reset stage of the WS2812 protocol */
#define NUM_PERIODS_FOR_RESET    	 41 

/* Enumerated type to keep track of direction
   light streak is traveling. */
enum direction {
	up,
	down
};

/**
  * @brief Sets the LEDs in the LED strip to the specified color
  * @param start : Index of first LED in strip to be set to color
  * @param stop : Index of last LED in strip to be set to color
  * @param color : GRB color code
  * @retval None
  */
void set_color(uint16_t *restrict ccr_buff, size_t start, size_t stop, uint32_t color);

/**
  * @brief Converts RGB codes to GRB codes
  *	   Standard color hex codes are in the
  *	   order RGB, but the WS2812 LEDs expect
  *        data in the order GRB.  This function
  *	   converts from RGB to GRB.
  * @param rgb_code : Hexadecimal color code in RGB format
  * @retval rgb2grb : Hexadecimal color code in GRB format
  */
uint32_t rgb2grb(uint32_t rgb_code);

/**
  * @brief Cycles the LEDs in the LED strip through the colors
  * 	   of the rainbow, starting at red.
  * @param None
  * @retval None
  */
void color_transition_demo(uint16_t *restrict ccr_buff);

/**
  * @brief Red streaks of color "bounce" back and forth
  *        on each half of the LED strip.
  * @param None
  * @retval None
  */
void color_bounce_demo(uint16_t *restrict ccr_buff);

/**
  * @brief A streak of color travels up and down the LED strip,
  *        cycling through the colors of the rainbow as it goes.
  * @param None 
  * @retval None
  */
void travel_change_demo(uint16_t *restrict ccr_buff);

/**
  * @brief Updates the color of the strip 
  * @param ccr_buff : Pointer to memory location which is used
  *        to update the CCR value of the timer controlling the
  *	   PWM wave form being sent to the LED strip.
  * @retval None
  */
void color_update(uint16_t *restrict ccr_buff, uint32_t color);

/**
  * @brief Plays start-up sequence on LED strip 
  * @param ccr_buff : Pointer to memory location which is used
  *        to update the CCR value of the timer controlling the
  *	   PWM wave form being sent to the LED strip.
  * @retval None
  */
void start_sequence(uint16_t *restrict ccr_buff);

/**
  * @brief Half the LEDs transition from blue to red
  *        and half transition from red to blue.
  * @param ccr_buff : ptr to buffer of ccr values which control the duty cycle of the pwm
  * @retval None
  */
void interweave_demo(uint16_t *restrict ccr_buff);

#endif
