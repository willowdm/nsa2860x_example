/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/*
 *@Note
 USART Print debugging routine:
 USART1_Tx(PA9).
 This example demonstrates using USART1(PA9) as a print debug port output.

*/

#include "debug.h"
#include "nsa2860x.h"

/* Global typedef */

/* Global define */

/* Global Variable */

static nsa_errors_t nsa2860x_status;
static uint16_t nsa2860x_dac_lvl = 0;
static float nsa2860x_adc_voltage = 0;

#define I2C_MASTER_BASE (I2C2)

static void NSA_I2CRegRead(uint8_t ucSlaveAdress, uint8_t ucReg, uint8_t* buff,uint8_t len)
{

    uint8_t reg_cnt = 0;

    I2C_GenerateSTART(I2C_MASTER_BASE, ENABLE);
        while(!I2C_GetFlagStatus(I2C_MASTER_BASE, I2C_FLAG_SB));

    I2C_Send7bitAddress(I2C_MASTER_BASE, ucSlaveAdress, I2C_Direction_Transmitter);
        while(!I2C_CheckEvent(I2C_MASTER_BASE, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C_MASTER_BASE, ucReg);
        while(!I2C_GetFlagStatus(I2C_MASTER_BASE,I2C_FLAG_TXE));

    I2C_GenerateSTOP(I2C_MASTER_BASE, ENABLE);
        while(I2C_GetFlagStatus(I2C_MASTER_BASE, I2C_FLAG_BUSY));

    I2C_GenerateSTART(I2C_MASTER_BASE, ENABLE);
        while(!I2C_CheckEvent(I2C_MASTER_BASE, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_AcknowledgeConfig(I2C_MASTER_BASE, ENABLE);

    I2C_Send7bitAddress(I2C_MASTER_BASE, ucSlaveAdress, I2C_Direction_Receiver);
        while(!I2C_CheckEvent(I2C_MASTER_BASE,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    while (len){
        while(!I2C_CheckEvent(I2C_MASTER_BASE,I2C_EVENT_SLAVE_BYTE_RECEIVED));
        buff[reg_cnt++] = I2C_ReceiveData(I2C_MASTER_BASE);
    	len--;
    }

    I2C_GenerateSTOP(I2C_MASTER_BASE, ENABLE);
        while(I2C_GetFlagStatus(I2C_MASTER_BASE, I2C_FLAG_BUSY));

}

static void NSA_I2CCmdWrite(unsigned char ucSlaveAdress, unsigned char ucReg, unsigned char ucValue)
{

    /* Send I2C1 START condition */
    I2C_GenerateSTART(I2C_MASTER_BASE, ENABLE);
        while(!I2C_GetFlagStatus(I2C_MASTER_BASE, I2C_FLAG_SB));

    I2C_Send7bitAddress(I2C_MASTER_BASE, ucSlaveAdress, I2C_Direction_Transmitter);
        while(!I2C_CheckEvent(I2C_MASTER_BASE, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C_MASTER_BASE, ucReg);
        while(!I2C_GetFlagStatus(I2C_MASTER_BASE,I2C_FLAG_TXE));

    I2C_SendData(I2C_MASTER_BASE, ucValue);
        while(!I2C_GetFlagStatus(I2C_MASTER_BASE,I2C_FLAG_TXE));

    I2C_GenerateSTOP(I2C_MASTER_BASE, ENABLE);
        while(I2C_GetFlagStatus(I2C_MASTER_BASE, I2C_FLAG_BUSY));

}

void i2c_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    I2C_InitTypeDef  I2C_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_StructInit(&GPIO_InitStructure);

    //GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_DeInit(I2C2);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 =  1;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 400000;

    I2C_Init(I2C2, &I2C_InitStructure);
    I2C_Cmd(I2C2, ENABLE);
}


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemCoreClockUpdate();
	Delay_Init();
	USART_Printf_Init(115200);	
	printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
	printf("This is printf example\r\n");


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

	i2c_init();
	Delay_Ms(100);

	nsa2860x_i2c_init(NSA_I2CRegRead, NSA_I2CCmdWrite);

	nsa2860x_status = nsa2860x_init();

	if ((nsa2860x_status == NSA2860X_ERR_NO) && (!nsa2860x_get_error_code())){
		printf("nsa2860x_init done \r\n");

		Delay_Ms(100);

		for(;;)
	    {
			nsa2860x_adc_voltage = ((nsa2860x_adc_get()*3.3)/8388608.0);
			//printf( "adc_voltage = %f (V)\r\n", nsa2860x_adc_voltage);
			//Delay_Ms(1000);

			nsa2860x_dac_set(nsa2860x_dac_lvl);
			nsa2860x_dac_lvl+= 1024;

		}
	}else{
		printf("nsa2860x_init error code = %d\r\n", nsa2860x_status);
	}

	while(1){

	}

}

