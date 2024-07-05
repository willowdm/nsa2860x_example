#ifndef NSA2860_H_
#define NSA2860_H_

#include "stdint.h"

#define NSA_I2C_ADDR (0xDA)

typedef enum {NSA2860X_ERR_NO, NSA2860X_ERR_I2C_FUNC, NSA2860X_ERR_COMMUNICATION} nsa_errors_t;

typedef void (*i2c_read_func_t)(uint8_t ucSlaveAdress, uint8_t ucReg, uint8_t* buff,uint8_t len) ;
typedef void (*i2c_write_func_t)(unsigned char ucSlaveAdress, unsigned char ucReg, unsigned char ucValue);

void nsa2860x_i2c_init(i2c_read_func_t i2c_read_func, i2c_write_func_t i2c_write_func);

nsa_errors_t nsa2860x_init(void);

uint8_t nsa2860x_get_error_code(void);

void nsa2860x_dac_set(uint16_t dac_lvl);
uint32_t nsa2860x_adc_get(void);

#endif /* NSA2860_H_ */
