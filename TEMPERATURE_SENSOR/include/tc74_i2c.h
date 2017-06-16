/*!
 * @file
 *
 * @brief I2C peripheral control library
 *
 * @author Nrgagnon
 *
 * @date May 11, 2017
 *
 */

#ifndef TC74_I2C_H
#define TC74_I2C_H

#define AUTO_INCR			((uint8_t) 0x80)

/* TIMINGR Constants */
#define I2C_CLK_FREQ_DIV_2		((uint32_t) 1U << 28)
#define I2C_CLK_FREQ_DIV_4		((uint32_t) 3U << 28)

enum xfer_dir {
	m2s = 0,
	s2m
};

// initialize i2c1 peripheral
void i2c1_init(void);

// configure pins used for i2c1
static void i2c1_pins_init(void);

void i2c1_read(size_t nbytes, uint8_t slvaddr, uint8_t *reg, int8_t *storage_ptr);

// transmit data on the i2c1 bus
void i2c1_transmit(size_t nbytes, uint8_t slvaddr, uint8_t *payload_ptr);

#endif
