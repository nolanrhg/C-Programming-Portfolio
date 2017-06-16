/**
  **********************************************************************************
  * @file    RGB_SENSOR/src/color_processing.c
  * @author  Nrgagnon 
  * @version V1.6
  * @date    29-May-2017
  * @brief   Library of functions for processing color data returned by the TCS34725 
  *	     RGB sensor.
  *
  **********************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 Nolan R. H. Gagnon </center></h2>
  *
  **********************************************************************************
  */

/* Includes ----------------------------------------------------------------------*/
#include "../include/color_processing.h"
#include "../include/lcd.h"
#include <stdlib.h>
#include <stdio.h>

/**
  * @brief Brightens color returned by the TCS34725 RGB sensor.
  * @param raw_colr_dat : Ptr to an array of uint8_t with 8 members.
  *	   Data MUST be ordered according to the diagram below:
  *
  *	   -------------------------------------------------------
  *       |      |      |      |      |      |      |      |      |
  *	  | C_LB | C_HB | R_LB | R_HB | G_LB | G_HB | B_LB | B_HB | 
  *       |      |      |      |      |      |      |      |      |
  *	   -------------------------------------------------------
  *          0      1      2      3      4       5      6     7
  *
  *	   This is the raw color data produced by the TCS34725.
  *
  * @retval enh_colr_dat : Ptr to array of uint8_t with 8 members.
  *	    The array contains color data suitable for transfer to
  *	    LED units.   
  */
uint8_t *enhance_color(uint8_t *raw_colr_dat)
{
	int diff;
	char buff[6];
	uint8_t max_enh_val = 0xFF;
	uint8_t mid_enh_val = 0x40;
	uint8_t min_enh_val = 0x00;
	uint32_t *sorted_idx;
	uint8_t byte_rgb[3];

	uint8_t *ordered_byte_rgb;
	
	uint16_t *comb_colr_dat;

	uint16_t ovrflw_test;

	uint16_t red_comb = (uint16_t)raw_colr_dat[2] | ((uint16_t)raw_colr_dat[3] << 8);
	uint16_t green_comb = (uint16_t)raw_colr_dat[4] | ((uint16_t)raw_colr_dat[5] << 8);
	uint16_t blue_comb = (uint16_t)raw_colr_dat[6] | ((uint16_t)raw_colr_dat[7] << 8);

	ordered_byte_rgb = malloc(3 * sizeof(uint8_t));
	
	if (ordered_byte_rgb == NULL) {
		exit(1);
	}

	comb_colr_dat = malloc(3 * sizeof(uint16_t));

	if (comb_colr_dat == NULL) {
		exit(1);
	}

	comb_colr_dat[0] = red_comb;
	comb_colr_dat[1] = green_comb;
	comb_colr_dat[2] = blue_comb;
	
	sorted_idx = sort_arr(comb_colr_dat, 3);	
	
	diff = comb_colr_dat[2] - comb_colr_dat[1]; 
	sprintf(buff, "%d", diff);
	LCD_Clear();
	LCD_DisplayString((uint8_t *)buff);
		
	// Enhance colors	
	ovrflw_test = ((comb_colr_dat[2] >> 8) & 0x00FF) + max_enh_val;
	if (ovrflw_test > 0xFF) 
		byte_rgb[2] = 0xFF;
	else
		byte_rgb[2] = (uint8_t)((comb_colr_dat[2] >> 8) & 0x00FF) + max_enh_val;

	ovrflw_test = ((comb_colr_dat[1] >> 8) & 0x00FF) + mid_enh_val;
	if (ovrflw_test > 0xFF) 
		byte_rgb[1] = 0xFF;
	else
		byte_rgb[1] = 0x00; //(uint8_t)((comb_colr_dat[1] >> 8) & 0x00FF) - mid_enh_val;


	ovrflw_test = ((comb_colr_dat[0] >> 8) & 0x00FF) + min_enh_val;
	if (ovrflw_test > 0xFF) 
		byte_rgb[0] = 0xFF;
	else
		byte_rgb[0] = 0x00; //(uint8_t)((comb_colr_dat[0] >> 8) & 0x00FF) - min_enh_val;


	// Put colors in correct order for LED functions
	ordered_byte_rgb[0] = byte_rgb[sorted_idx[0]];  // R
	ordered_byte_rgb[1] = byte_rgb[sorted_idx[1]];  // G
	ordered_byte_rgb[2] = byte_rgb[sorted_idx[2]];  // B
	
	free(sorted_idx);	
	free(comb_colr_dat);
		
	return ordered_byte_rgb;
}

/**
  * @brief Sorts an array of RGB color values and returns pointer 
  *	   to array containing locations of RGB values in the 
  *	   sorted array.
  * @param arr : Pointer to array to be sorted.
  * @param size : Size of the array to be sorted.
  * @retval sort_arr : Pointer to array containing locations
  *	    of the RGB values in the sorted array :
  *	    {i_R, i_G, i_B}.
  */
static uint32_t *sort_arr(uint16_t *arr, size_t size)
{
	size_t i;			
	size_t j;
	uint32_t *sorted_idxs;
	uint16_t *arr_cpy;

	arr_cpy = (uint16_t *) malloc(size * sizeof(uint16_t));

	if (arr_cpy == NULL) {
		perror("malloc");
		exit(1);
	}
	
	// make copy of array to be sorted	
	for (i = 0; i < size; i++) {
		arr_cpy[i] = arr[i];
	}

	sorted_idxs = (uint32_t *) malloc(size * sizeof(uint32_t));  // Caller MUST free
	
	if (sorted_idxs == NULL) {
		exit(1);		
	}
	
	for (i = size - 1; i > 0; i--) {
		for (j = 1; j <= i; j++) {
			if (arr[j - 1] > arr[j] ) {
				swap_16bit(arr, j - 1, j);	
				swap_32bit(sorted_idxs, j - 1, j);
			}
		}
	}
	
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			if (arr_cpy[i] != arr[j]) {
				continue;
			}
			sorted_idxs[i] = j;	
		}
	}

	return sorted_idxs;
}

/**
  * @brief Swaps two 16-bit numbers of an array.
  * @param arr : Pointer to array that contains the values
  *	   to be swapped.
  * @param i : Index of first value to be swapped.
  * @param j : Index of second value to be swapped.
  * @retval None
  */
static void swap_16bit(uint16_t *arr, size_t i, size_t j)
{
	uint16_t t;  // temp var.

	t = arr[i];
	arr[i] = arr[j];		
	arr[j] = t;	
}

/**
  * @brief Swaps two 32-bit numbers of an array.
  * @param arr : Pointer to array that contains the values
  *	   to be swapped.
  * @param i : Index of first value to be swapped.
  * @param j : Index of second value to be swapped.
  * @retval None
  */
static void swap_32bit(uint32_t *arr, size_t i, size_t j)
{
	uint32_t t;  // temp var.
	
	t = arr[i];
	arr[i] = arr[j];
	arr[j] = t;
}
