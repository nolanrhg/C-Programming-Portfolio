#ifndef COLOR_PROCESSING_H
#define COLOR_PROCESSING_H

uint8_t *enhance_color(uint8_t *raw_colr_dat);

static uint32_t *sort_arr(uint16_t *arr, size_t size);

static void swap_16bit(uint16_t *arr, size_t i, size_t j);

static void swap_32bit(uint32_t *arr, size_t i, size_t j);

#endif
