#ifndef TC74_FUNCS_H
#define TC74_FUNCS_H

/* TC74 CONSTANTS */
#define TC74_I2C_ADDR		((uint8_t) 0x48)
#define TC74_TEMPR_PTR		((uint8_t) 0x00)
#define TC74_CFGR_PTR		((uint8_t) 0x01)

/* TC74 COMMANDS */
int8_t read_tc74_temp(void);
float cels2fahr(int8_t cels);
	
#endif
