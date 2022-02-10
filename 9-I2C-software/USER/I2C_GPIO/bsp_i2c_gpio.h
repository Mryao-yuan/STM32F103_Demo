#ifndef _BSP_I2C_GPIO_H_
#define _BSP_I2C_GPIO_H_


#include "stm32f10x.h"
#include <stdio.h>


//I2C_GPIO宏定义
#define   I2C_OwnAddress  						0x5f
#define   I2C_GPIO_Cmd  						RCC_APB2PeriphClockCmd
#define   I2C_GPIO_CLK 							RCC_APB2Periph_GPIOA


#define   I2Cx														I2C1
#define   EEPROM_I2C_SDA_PORT							GPIOA
#define   EEPROM_I2C_SDA_PIN 							GPIO_Pin_3
#define   EEPROM_I2C_SCL_PORT							GPIOA
#define  	EEPROM_I2C_SCL_PIN 							GPIO_Pin_2

//I2C配置
//#define   I2Cx											I2C1
//#define   I2C_ClockCmd							RCC_APB1PeriphClockCmd
//#define   I2C_CLK 									RCC_APB1Periph_I2C1
//#define   I2C_Speed 								300000

//SDA 电平配置
#define  	I2C_SDA_HIGH()							GPIO_SetBits(EEPROM_I2C_SDA_PORT,EEPROM_I2C_SDA_PIN)
#define  	I2C_SDA_LOW()								GPIO_ResetBits(EEPROM_I2C_SDA_PORT,EEPROM_I2C_SDA_PIN)

//SCL 电平配置
#define  	I2C_SCL_HIGH()							GPIO_SetBits(EEPROM_I2C_SDA_PORT,EEPROM_I2C_SCL_PIN)
#define  	I2C_SCL_LOW()								GPIO_ResetBits(EEPROM_I2C_SDA_PORT,EEPROM_I2C_SCL_PIN)
/* 读 SDA 口线状态 */
#define 	I2C_SDA_READ()							GPIO_ReadInputDataBit(EEPROM_I2C_SDA_PORT,EEPROM_I2C_SDA_PIN)

void static Delay(void);

void I2C_GPIO_Configue(void);
void I2C_Mode_Configue(void);
void I2C_Configue(void);

void I2c_Start(void);
void I2c_Stop(void);
void I2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(void);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
#endif

