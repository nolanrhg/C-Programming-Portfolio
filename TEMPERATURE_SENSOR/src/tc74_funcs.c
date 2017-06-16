#include "../include/tc74_funcs.h"
#include "../include/tc74_i2c.h"

int8_t read_tc74_temp(void)
{
	int8_t cfgr_storage[1];
	uint8_t config_reg[1] = {TC74_CFGR_PTR};
	uint8_t temp_reg[1] = {TC74_TEMPR_PTR};
	int8_t temp_data[1];
	uint8_t data_rdy = 0;
	
	// Check if temperature data is ready	
//	while (data_rdy == 0) {
//	i2c1_read(1, TC74_I2C_ADDR, config_reg, cfgr_storage);
//		data_rdy = cfgr_storage[0] & 0x40;
//	}	
	
	// Read temperature now that it is ready	
	i2c1_read(1, TC74_I2C_ADDR, temp_reg, temp_data);	
		
	return temp_data[0];
}

float cels2fahr(int8_t cels)
{
	return ((9.0/5.0) * cels + 32);
}
