#ifndef __STM32L476G_DISCOVERY_LCD_H
#define __STM32L476G_DISCOVERY_LCD_H

#include <stdint.h>

void LCD_Initialization(void);
void LCD_bar(void);
void LCD_Clock_Init(void);
void LCD_PIN_Init(void);
void LCD_Configure(void);
void LCD_Clear(void);
void LCD_DisplayString(uint8_t* ptr);
void LCD_WriteChar(uint8_t* ch, char point, char colon, uint8_t position);
static void LCD_Conv_Char_Seg(uint8_t* c,char point,char colon, uint8_t* digit);
void LCD_Display_Name(void);
void LCD_DisplayScrollingString(uint8_t* ptr);

#endif /* __STM32L476G_DISCOVERY_LCD_H */
