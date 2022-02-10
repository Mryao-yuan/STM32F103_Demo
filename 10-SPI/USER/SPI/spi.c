#include "spi.h"



static __IO uint32_t SPITimeout=SPI_LONG_TIMEOUT;//定义为可读可写

//static uint32_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);


void SPI_GPIO_Init(void){
	
	GPIO_InitTypeDef 	GPIO_InitStruct;
	
	SPI_CLKCmd(SPI_CLK,ENABLE);
	GPIO_CLKCmd(GPIO_CLK,ENABLE);
	
	//SCK ：推挽复用输出
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin=SPI_SCK_PIN;
	GPIO_Init(SPI_SCK_PORT,&GPIO_InitStruct);
	//MOSI
	GPIO_InitStruct.GPIO_Pin=SPI_MOSI_PIN;
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(SPI_MOSI_PORT,&GPIO_InitStruct);
	//MISO
	GPIO_InitStruct.GPIO_Pin=SPI_MISO_PIN;
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(SPI_MISO_PORT,&GPIO_InitStruct);

	
	//NSS ：推挽输出(软件控制)
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin=SPI_NSS_PIN;
	GPIO_Init(SPI_NSS_PORT,&GPIO_InitStruct);
	
	//停止信号NSS 引脚高电平
	SPI_NSS_HIGH();
}
/*
	*@brief spi模式配置
	*@param 无
	*@retval 无
	*/
void SPI_Mode_Config(void){
	
	SPI_InitTypeDef 	SPI_InitStruct;
	/*
	主机端,双线全双工模式，数据帧长度为8位
	使用SPI模式3(CPOL=1,CPHA=1)
	NSS引脚由软件控制,MSB先行模式
	时钟频率4分频
	*/
	SPI_InitStruct.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode=SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize=SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL=SPI_CPOL_High;
	SPI_InitStruct.SPI_CPHA=SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_NSS=SPI_NSS_Soft ;	//	软件模拟
	SPI_InitStruct.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_4;
	SPI_InitStruct.SPI_FirstBit=SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial=0;//不适用CRC的功能
	SPI_Init(FLASH_SPIx	,&SPI_InitStruct);
	//使能SPI
	SPI_Cmd(FLASH_SPIx,ENABLE);
	
}
void FLASH_SPI_Init(void){
	SPI_GPIO_Init();
	SPI_Mode_Config();
	
}
//发送并接收一个数据
u8 SPI_FLASH_SendByte(u8 byte){
	
		SPITimeout	=	 SPI_FLAG_TIMEOUT;
	
	//等待发送缓冲区位空，TXE时间
	while(SPI_I2S_GetFlagStatus(FLASH_SPIx,SPI_I2S_FLAG_TXE) == RESET){
			if((SPITimeout--)==0)	//防止死循环，超时退出
				return SPI_TIMEOUT_UserCallback(0);
	}
	//缓冲区已空，发送数据
	SPI_I2S_SendData(FLASH_SPIx,byte);
	SPITimeout = SPI_FLAG_TIMEOUT;
	//检查并等待至RX缓冲区位非空
	while(SPI_I2S_GetFlagStatus(FLASH_SPIx,SPI_I2S_FLAG_RXNE) == RESET){
		if((SPITimeout--)==0)	//防止死循环，超时退出
				return SPI_TIMEOUT_UserCallback(1);
	}
	return SPI_I2S_ReceiveData(FLASH_SPIx);
}
/*
	*@brief 读取FLASH的ID,用于测试程序的正确性
	*@param 无
	*@retval FLASH ID
	*/
uint32_t SPI_Read_ID(void){
	//FLASH ID 由三字节组成，每一字节8位，故要左移
	
	uint32_t flash_id=0;
	//开始通讯,sck变低
	SPI_NSS_LOW();
	SPI_FLASH_SendByte(READ_JEDEC_ID);
	
	flash_id=SPI_FLASH_SendByte(Dummy_Byte);
	
	flash_id <<=8;
	
	flash_id |= SPI_FLASH_SendByte(Dummy_Byte);
	
	flash_id <<=8;
	
	flash_id |= SPI_FLASH_SendByte(Dummy_Byte);
	
	SPI_NSS_HIGH();	//结束通讯
	
	return flash_id;
}



u8 SPI_FLASH_ReadByte(void){
	return (SPI_FLASH_SendByte(Dummy_Byte));
}
/*
	*@brief Basic management of the timeout situation
	*@param errorCode:错误代码，可以定位那个环节出错
	*@retval 返回0，表示SPI读取失败
	*/
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* 等待超时后的处理,输出错误信息 */
  FLASH_ERROR("SPI 等待超时!errorCode = %d",errorCode);
  return 0;
}
/*
	*@brief 擦除FLASH指定扇区
	*@param 
	*@retval 
	*/
void SPI_ERASE_SECTOR(uint32_t addr){
	SPI_FLASH_WriteEnable();
	SPI_NSS_LOW();
	
	SPI_FLASH_SendByte(ERASE_SECTOR);
	
	SPI_FLASH_SendByte((addr>>16)&0xFF);
	
	SPI_FLASH_SendByte((addr>>8)&0xFF);
	
	SPI_FLASH_SendByte(addr&0xFF);
	SPI_NSS_HIGH();
}
	
/*
	*@brief 等待内部时序操作完成
	*@param  无
	*@retval 无
	*/
void SPI_WaitForWriteEnd(void){
	
	//状态寄存器
	uint8_t status_reg=0;
	
	SPI_NSS_LOW();
	
	(SPI_FLASH_SendByte(READ_STATUS));
	
	do{
		status_reg=SPI_FLASH_SendByte(Dummy_Byte);	
		
	}while((status_reg& 0x01)==1);
	
	SPI_NSS_HIGH();
	
}
/*
	*@brief 读取FLASH的数据
	*@param  无
	*@retval 无
	*/
void SPI_Read_Data(uint32_t addr,uint8_t *readbuffer,uint32_t numByteToRead){
	
	SPI_NSS_LOW();
	
	SPI_FLASH_SendByte(READ_DATA);
	
	SPI_FLASH_SendByte((addr>>16)&0xff);
	
	SPI_FLASH_SendByte((addr>>8)&0xff);
	
	SPI_FLASH_SendByte(addr & 0xff);
	//发送三个字节地址
	while(numByteToRead--){
		
		*readbuffer=SPI_FLASH_SendByte(Dummy_Byte);
			readbuffer++;	
	}
	
	SPI_NSS_HIGH();
	
}


/*
	*@brief 写入FLASH的数据
	*@param  无
	*@retval 无
	*/
void SPI_Write_Data(uint32_t addr,uint8_t *writebuffer,uint32_t numByteToWrite){
	
	SPI_FLASH_WriteEnable();//写入使能，要对FLASH写入数据，必须要使用此函数
	
	SPI_NSS_LOW();//片选使能
	
	SPI_FLASH_SendByte(WRITE_DATA);
	
	SPI_FLASH_SendByte((addr>>16)&0xff);
	
	SPI_FLASH_SendByte((addr>>8)&0xff);
	
	SPI_FLASH_SendByte(addr & 0xff);
	//发送三个字节地址
	while(numByteToWrite--){
		
		SPI_FLASH_SendByte(*writebuffer);
			writebuffer++;	
	}
	
	SPI_NSS_HIGH();
	SPI_WaitForWriteEnd();
}

/*
	*@brief  向FLASH发送写使能
	*@param  无
	*@retval 无
	*/
void SPI_FLASH_WriteEnable(void){
	
	//片选使能
	SPI_NSS_LOW();
	
	//发送写使能命令
	SPI_FLASH_SendByte(Writeable);
	
	//结束通讯
	SPI_NSS_HIGH();
	
}

