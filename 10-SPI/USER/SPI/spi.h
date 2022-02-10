#ifndef _SPI_H_
#define _SPI_H_


#include "stm32f10x.h"
#include "stdio.h"

//等待超时时间
#define   SPI_FLAG_TIMEOUT  	((uint32_t)0x1000)					
#define   SPI_LONG_TIMEOUT 		((uint32_t)10*SPI_FLAG_TIMEOUT)

#define   SPI_CLK  						RCC_APB2Periph_SPI1
#define   SPI_CLKCmd  				RCC_APB2PeriphClockCmd

#define   GPIO_CLK  					RCC_APB2Periph_GPIOA
#define   GPIO_CLKCmd  				RCC_APB2PeriphClockCmd

#define    FLASH_SPIx						SPI1
//NSS:A4
#define  	SPI_NSS_PORT 						GPIOA 
#define   SPI_NSS_PIN  						GPIO_Pin_4
//SCK:A5
#define  	SPI_SCK_PORT 						GPIOA 
#define   SPI_SCK_PIN  						GPIO_Pin_5
//MISO:A6
#define  	SPI_MISO_PORT 						GPIOA 
#define   SPI_MISO_PIN  						GPIO_Pin_6
//MOSI:A7
#define  	SPI_MOSI_PORT 						GPIOA 
#define   SPI_MOSI_PIN  						GPIO_Pin_7

//设置cs引脚高低电平
#define 	SPI_NSS_HIGH()					GPIO_SetBits(GPIOA,SPI_NSS_PIN)
#define 	SPI_NSS_LOW()						GPIO_ResetBits(GPIOA,SPI_NSS_PIN)

//Flash芯片代码
#define 	READ_JEDEC_ID						0x9F
#define 	Dummy_Byte							0xFF
#define   	ERASE_SECTOR						0x20
#define  	READ_STATUS							0x05
#define  	READ_DATA							0x03
#define		WRITE_DATA							0x02
#define		Writeable							0x06
//信息输出
#define 	FLASH_DEBUG_ON							1

#define 	FLASH_INFO(fmt,arg...)			printf("<<-FLASH-INFO->>"fmt"\n",##arg)
#define 	FLASH_ERROR(fmt,arg...)			printf("<<-FLASH-ERROR->>"fmt"\n",##arg)
#define 	FLASH_DEBUG(fmt,arg...)			do{\
																			if(FLASH_DEBUG_ON)\
																			 printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                        }while(0)
typedef  uint8_t		u8;

void SPI_GPIO_Init(void);
void SPI_Mode_Config(void); 	
void FLASH_SPI_Init(void);

u8 SPI_FLASH_SendByte(u8 byte);
uint32_t SPI_Read_ID(void);
u8 SPI_FLASH_ReadByte(void);
static uint16_t SPI_TIMEOUT_UserCallback(u8 errorCode);

																				
void SPI_ERASE_SECTOR(uint32_t adddr);																				
void SPI_WaitForWriteEnd(void);
void SPI_Read_Data(uint32_t addr,uint8_t *readbuffer,uint32_t numByteToRead);	
void SPI_Write_Data(uint32_t addr,uint8_t *writebuffer,uint32_t numByteToWrite);
void SPI_FLASH_WriteEnable(void);										
										
#endif

