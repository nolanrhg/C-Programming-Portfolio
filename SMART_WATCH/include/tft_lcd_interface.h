/**********************************************************************************\
 * @file    SMART_WATCH/include/tft_lcd.h                                         *
 * @author  Nolan R. H. Gagnon                                                    *
 * @version V1.0                                                                  *
 * @date    29-June-2017                                                          *
 * @brief   Color LCD Application-level Interface.                                *
 *                                                                                *
 **********************************************************************************
 * @attention                                                                     *
 *                                                                                *
 * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. H. Gagnon  </center></h2>        *
 *                                                                                *
\**********************************************************************************/
#ifndef TFT_LCD_H
#define TFT_LCD_H

#include "./shape.h"

/**********************************************************************************\
 *                                                                                *
 *                                  LCD STRUCT                                    *
 *                                                                                *
\**********************************************************************************/
typedef struct _LCD_screen LCD_screen;


/**********************************************************************************\
 *                                                                                *
 *                            LCD FUNCTION PROTOTYPES                             *
 *                                                                                *
\**********************************************************************************/

/**
  * @brief Returns a pointer to an initialized LCD_screen data structure.
  * @param None
  * @retval get_LCD_instance : Pointer to initialized LCD_screen struct.
  */
LCD_screen *get_LCD_instance(void);

/**
  * @brief Deallocates memory for an LCD_screen structure.
  * @param lcd : Pointer to LCD_screen structure whose memory is to be deallocated.
  * @retval None
  */
void destroy_LCD_instance(LCD_screen *lcd);

/**
  * @brief Turn on the specified LCD screen.
  * @param lcd : The LCD screen to turn on.
  * @retval None
  */
void LCD_turn_on(LCD_screen *lcd);

/**
  * @brief Turn off the specified LCD screen.
  * @param lcd : The LCD screen to turn off.
  * @retval None
  */
void LCD_turn_off(LCD_screen *lcd);

/**
  * @brief Draw a shape on the LCD screen at position (x,y) in Cartesian coordinate system.
  * @param lcd : The LCD screen to draw on.
  * @param shape : Pointer to structure representing shape to be drawn on screen.
  * @param x : Horizontal location of shape's center.
  * @param y : Vertical location of shape's center.
  * @retval None
  */
void LCD_draw_shape(LCD_screen *lcd, SHAPE_TypeDef *shape, int x, int y);

/**
  * @brief Sets the background color of an LCD screen.
  * @param lcd : The LCD whose background color is to be changed.
  * @param color : The color to set the background of the LCD screen to.
  * @retval None
  */
void LCD_set_background(LCD_screen *lcd, uint32_t color);

/**
  * @brief Adjust the brightness of the LCD screen.
  * @param lcd : The LCD whose brightness is to be adjusted.
  * @param brtnss : Brightness level to set the LCD screen to.
  * @retval None
  */
void LCD_set_brightness(LCD_screen *lcd, uint8_t brtnss);

/**********************************************************************************\
 *                                                                                *
 *                                LCD CONSTANTS                                   *
 *                                                                                *
\**********************************************************************************/
#define LCD_MIN_BRIGHTNESS                      ((uint8_t) 0x00)
#define LCD_MAX_BRIGHTNESS                      ((uint8_t) 0xFF)

#endif
