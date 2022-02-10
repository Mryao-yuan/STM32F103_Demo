#ifndef _BSP_I2C_EE_H_
#define _BSP_I2C_EE_H_

//10100000
#define EEPROM_DEV_ADDR   0xA0
//写/读控制位
#define EEPROM_I2C_WR   	0
#define EEPROM_I2C_RD   	1
//页大小
#define EEPROM_PAGE_SIZE  8
#define EEPROM_SIZE				128

#include <stm32f10x.h>
#include "bsp_i2c_gpio.h"

#define		u8		uint8_t
#define		u16		uint16_t
#define		u32		uint32_t

u8	ee_WaitStandby(void);

uint8_t ee_CheckDevice(uint8_t _Address);

u8	ee_WriteBytes(u8 *_pWriteBuf,u16 	_usAddress,u16 _usSize);

u8	ee_ReadBytes(u8 *_pReadBuf,u16	_usAddress,u16 _usSize);

u8 ee_test(void);










#endif 
