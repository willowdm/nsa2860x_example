#ifndef NSA2860X_REG_H_
#define NSA2860X_REG_H_

/*
 The register map of theNSA2860X includes two parts, normal registers and EEPROM registers.
 The normal registers include dataregisters and some control registers,
 while the EEPROM registers are mainly configuration registers and calibration coefficients. All
 EEPROM registers should be written by external interface on command mode (register ¡®CMD¡¯ = ¡®0x00¡¯).

 novosens~nsa2860x-dqnr.pdf

*/



//NORMAL REGISTERS

#define REG_IF_CTRL 			(0x00)
#define IF_CTRL_SDO_ACTIVE_SPI3 	(0x00)
#define IF_CTRL_SDO_ACTIVE_SPI4 	(0x81)
#define IF_CTRL_LSB_FIRST_MSB 		(0x00)
#define IF_CTRL_LSB_FIRST_LSB 		(0x42)
#define IF_CTRL_SOFT_RESET 			(0x24)

#define REG_STATUS 				(0x02)
#define STATUS_DRDY 				(1<<0)
#define STATUS_LOADING_END			(1<<1)
#define STATUS_CRC_ERR		    	(1<<2)
#define STATUS_ERROR_CODE		    (0x1F<<3)
#define STATUS_ERROR_CODE_VIN2GND   (1<<3)
#define STATUS_ERROR_CODE_VIN2VREF  (1<<4)
#define STATUS_ERROR_CODE_VIP2GND   (1<<5)
#define STATUS_ERROR_CODE_VIP2VREF  (1<<6)

/*
 Signed£¬2¡¯s complement:
 When¡¯RAW_P¡¯= 1, stores the ADC output of primary channel,
 When¡¯RAW_P¡¯=0, stores the calibrated primary channel data
*/
#define REG_PDATA_23_16			(0x06)
#define REG_PDATA_15_8			(0x07)
#define REG_PDATA_7_0			(0x08)


/*
Signed£¬2¡¯s complement:
 When ¡¯RAW_T¡¯ = 1, stores the ADC output of temperature channel,
 When ¡¯RAW_T¡¯= 0, stores the calibrated temperature data, LSB=1/2^16¡æ.
 Real Temperature =TDATA/2^16+25 ¡æ
*/
#define REG_TDATA_23_16			(0x09)
#define REG_TDATA_15_8			(0x0A)
#define REG_TDATA_7_0			(0x0B)

/*
 DAC input data, unsigned;
 When ¡®RAW_P¡¯ = 0, set by the internal calibration logic,
 readonly, When ¡®RAW_P¡¯= 1, set externally through serial interface.
*/
#define REG_DAC_DATA_15_8		(0x12)
#define REG_DAC_DATA_7_0		(0x13)

/*
 Blank DAC input update when ¡®RAW_P¡¯=1,
 should be set beforewritingDAC_MSB and DAC_LSB and cleared */
#define REG_DAC_BLANK			(0x14)

/*
 0x00: command mode, all EEPROM can be written onlyincommand mode.
 0x33: Enter EEPROM Program Mode
*/
#define REG_COMMAND				(0x30)
#define COMMAND_COMMAND_MODE		(0x00)
#define COMMAND_ACTIVE_MODE			(0x03)
#define COMMAND_EEPROM_PGM_MODE		(0x33)

/*
 Write¡¯0x5D¡¯ to this register to quit OWI communication
 If ¡®QUIT_OWI_CNT¡¯ =0x00, quit OWI communication
 permanently. If ¡®QUIT_OWI_CNT¡¯ is not 0x00,
 quit OWI modetemporary with a certain time and then get back to OWI mode.
*/
#define REG_QUIT_OWI			(0x61)
#define QUIT_OWI					(0x5D)

/*
 Time for temporary quit OWI communication Mode.
 0x00£ºQuit forever£¬0x01£º50ms£¬0x02£º100ms ¡­0xFF£º12.8s
*/
#define REG_QUIT_OWI_CNT		(0x62)

/*
 Write ¡®1E¡¯ to this register to start EEPROMProgramming.
 Automatically cleared to ¡®0x00¡¯ after programming finished.
*/
#define REG_EE_PROG				(0x6A)
#define EE_PROG_START				(0x1E)

/*
 Write ¡®1¡¯ to force VDD/2 as the input of temperature ADC
*/
#define REG_VDD_CHECK			(0x70)



//EEPROM REGISTERS

#define REG_SYS_CONFIG1			(0xA1)
#define REG_SYS_CONFIG2			(0xA2)

#define REG_CURRENT_EXC			(0xA3)
#define CURRENT_EXC_IEXC1_750uA 	(0xF0)
#define CURRENT_EXC_IEXC2_EXT_R 	(0x0F)

#define REG_PCH_CONFIG1			(0xA4)

#define REG_PCH_CONFIG2			(0xA5)
#define PCH_CONFIG2_RAW_P			(1<<0)
#define PCH_CONFIG2_INPUT_SWAP		(1<<1)
#define PCH_CONFIG2_SYS_CHOP_EN		(1<<2)
#define PCH_CONFIG2_DAC_REF_5V		(0x00)
#define PCH_CONFIG2_DAC_REF_3_3V	(0x40)
#define PCH_CONFIG2_DAC_REF_1_2V	(0x80)
#define PCH_CONFIG2_DAC_REF_AVDD	(0xC0)

#define REG_TCH_CONFIG			(0xA6)
#define REG_CLAMP_H				(0xA7)
#define REG_CLAMP_L				(0xA8)

#define REG_OFFSET0_15_8		(0xA9)
#define REG_OFFSET0_7_8	    	(0xAA)

#define REG_CTC1_15_8			(0xAB)
#define REG_CTC1_7_0			(0xAC)

#define REG_CTC2_15_8			(0xAD)
#define REG_CTC2_7_0			(0xAE)

#define REG_S0_15_8				(0xAF)
#define REG_S0_7_0				(0xB0)

#define REG_STC1_15_8			(0xB1)
#define REG_STC1_7_0			(0xB2)

#define REG_STC2_15_8			(0xB3)
#define REG_STC2_7_0			(0xB4)

#define REG_KS_15_8				(0xB5)
#define REG_KS_7_0				(0xB6)

#define REG_KSS_15_8			(0xB7)
#define REG_KSS_7_0				(0xB8)

#define REG_SCALE_OFF_23_16		(0xB9)
#define REG_SCALE_OFF_15_8		(0xBA)
#define REG_SCALE_OFF_7_0		(0xBB)

#define REG_SCALE_S_23_16		(0xBC)
#define REG_SCALE_S_15_8		(0xBD)
#define REG_SCALE_S_7_0		    (0xBE)

#define REG_T0					(0xBF)

#define REG_KTS					(0xC0)

#define REG_MTO_15_8			(0xC1)
#define REG_MTO_7_0				(0xC2)

#define REG_KT_15_8				(0xC3)
#define REG_KT_7_0				(0xC4)

#define REG_DAC_OFF_15_8		(0xC5)
#define REG_DAC_OFF_7_0			(0xC6)

#define REG_DAC_GAIN_15_8		(0xC7)
#define REG_DAC_GAIN_7_0		(0xC8)

#define REG_PADC_OFF_23_16		(0xC9)
#define REG_PADC_OFF_15_8		(0xCA)
#define REG_PADC_OFF_7_0		(0xCB)

#define REG_PADC_GAIN_15_8		(0xCC)
#define REG_PADC_GAIN_7_0		(0xCD)

#define REG_P0					(0xCE)

#define REG_SPARE1				(0xCF)
#define REG_SPARE2				(0xD0)
#define REG_SPARE3				(0xD1)
#define REG_SPARE4				(0xD2)
#define REG_SPARE5				(0xD3)
#define REG_SPARE6				(0xD4)
#define REG_SPARE7				(0xD5)
#define REG_SPARE8				(0xD6)

#define REG_PDM_FREQ			(0xD7)
#define PDM_FREQ_DIG_GAIN_1X		(0x00)
#define PDM_FREQ_DIG_GAIN_2X		(0x40)
#define PDM_FREQ_DIG_GAIN_4X		(0x80)
#define PDM_FREQ_DIG_GAIN_8X		(0xC0)
#define PDM_FREQ_19_2				(0x00)
#define PDM_FREQ_38_4				(0x10)
#define PDM_FREQ_76_8				(0x20)
#define PDM_FREQ_153_6				(0x30)

/*
 1: EEPROM lock£¬set 1 and then EEPROMcan¡¯t be
 programmed. (Won¡¯t be effective until next power
 onreset orsoft reset after EEPROM is programmed)
*/
#define REG_EEPROM_LOCK			(0xD9)
#define EEPROM_LOCK					(1<<7)

#endif /* NSA2860X_REG_H_ */
