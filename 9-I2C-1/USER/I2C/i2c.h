#ifndef _I2C_H_
#define _I2C_H_

#include "stm32f10x.h"
#include <stdio.h>

/*
	7位地址	1010000
	读地址		10100001
	写地址		10100000
*/
#define EEPROM_ADDR					0xA0



#define STM32_I2C_OWNER_ADDR					0x5f
//I2C
#define I2Cx							I2C1
#define EEPROM_I2C1_CLK 							RCC_APB1Periph_I2C1
#define EEPROM_I2C1_APBxClkCmd 				RCC_APB1PeriphClockCmd
#define EEPROM_I2C1_BAUDRATE					400000
//I2C GPIO 引脚宏定义
#define EEPROM_I2C1_SCL_GPIO_CLK 			RCC_APB2Periph_GPIOB
#define EEPROM_I2C1_SDA_GPIO_CLK 			RCC_APB2Periph_GPIOB

#define EEPROM_I2C1_GPIO_APBxClkCmd 	RCC_APB2PeriphClockCmd

#define EEPROM_I2C1_SCL_GPIO_PORT 		GPIOB
#define EEPROM_I2C1_SCL_GPIO_PIN 			GPIO_Pin_6
#define EEPROM_I2C1_SDA_GPIO_PORT 		GPIOB
#define EEPROM_I2C1_SDA_GPIO_PIN 			GPIO_Pin_7


void I2c_Configue(void);
void EEPROM_Byte_Write(uint8_t addr,uint8_t data);
void EEPROM_Byte_Read(uint8_t addr,uint8_t *data,uint8_t numByteToRead);
void EEPROM_WaitForEnd(void);
void EEPROM_Page_Write(uint8_t addr,uint8_t data,uint8_t numByteToWrite);
#endif
