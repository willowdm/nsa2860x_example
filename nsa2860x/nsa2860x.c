#include "nsa2860x.h"
#include "nsa2860x_reg.h"

static i2c_read_func_t i2c_read;
static i2c_write_func_t i2c_write;

static uint8_t nsa2860x_get_reg(uint8_t addr)
{
	uint8_t reg[4];

	i2c_read(NSA_I2C_ADDR, addr, reg, 1);

	return reg[0];
}

static void nsa2860x_set_reg(uint8_t addr, uint8_t val)
{

	i2c_write(NSA_I2C_ADDR, addr, val);

}

static void nsa2860x_set_bit(uint8_t addr, uint8_t val)
{
	uint8_t reg[4];

	i2c_read(NSA_I2C_ADDR, addr, reg, 1);

	i2c_write(NSA_I2C_ADDR, addr, val|reg[0]);

}

void nsa2860x_i2c_init(i2c_read_func_t i2c_read_func, i2c_write_func_t i2c_write_func)
{
	i2c_read = i2c_read_func;
	i2c_write = i2c_write_func;
}

nsa_errors_t nsa2860x_init(void)
{
	nsa_errors_t err = NSA2860X_ERR_NO;

	if ((i2c_read != ((void *)0)) && (i2c_write != ((void *)0))){

		//soft_reset
		//nsa2860x_set_reg(REG_IF_CTRL,IF_CTRL_SOFT_RESET);

		//wait eeprom loading end flag
		while (!(nsa2860x_get_reg(REG_STATUS)&STATUS_LOADING_END)){};

		//cmd mode
		nsa2860x_set_reg(REG_COMMAND, COMMAND_COMMAND_MODE);

		if (nsa2860x_get_reg(REG_COMMAND) == COMMAND_COMMAND_MODE){

			//update raw primary ADC data into 'PDATA' register afterconversion, and allow DAC to be set externally
			//DAC full scale reference - 11: AVDD (Ratiometric)
			nsa2860x_set_bit(REG_PCH_CONFIG2, PCH_CONFIG2_RAW_P|PCH_CONFIG2_DAC_REF_AVDD);

			//IEXC1 750uA, IEXC2 disabled
			nsa2860x_set_bit(REG_CURRENT_EXC, CURRENT_EXC_IEXC2_EXT_R);

			//active mode
			nsa2860x_set_reg(REG_COMMAND, COMMAND_ACTIVE_MODE);


		}else{
			err = NSA2860X_ERR_COMMUNICATION;
		}
	}else{
		err = NSA2860X_ERR_I2C_FUNC;
	}

	return err;

}

uint8_t nsa2860x_get_error_code(void)
{
	return (nsa2860x_get_reg(REG_STATUS)&(STATUS_ERROR_CODE));
}

void nsa2860x_dac_set(uint16_t dac_lvl)
{
	nsa2860x_set_reg(REG_DAC_BLANK, 0x01);

	nsa2860x_set_reg(REG_DAC_DATA_7_0, (uint8_t)(dac_lvl));
	nsa2860x_set_reg(REG_DAC_DATA_15_8, (uint8_t)(dac_lvl>>8));

	nsa2860x_set_reg(REG_DAC_BLANK, 0x00);
}

uint32_t nsa2860x_adc_get(void)
{

	uint32_t adc_value = 0;

	adc_value = (uint32_t)((nsa2860x_get_reg(REG_PDATA_23_16)<<16)|(nsa2860x_get_reg(REG_PDATA_15_8)<<8)|(nsa2860x_get_reg(REG_PDATA_7_0)));

	return adc_value;
}

