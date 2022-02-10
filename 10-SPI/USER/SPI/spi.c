#include "spi.h"



static __IO uint32_t SPITimeout=SPI_LONG_TIMEOUT;//����Ϊ�ɶ���д

//static uint32_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);


void SPI_GPIO_Init(void){
	
	GPIO_InitTypeDef 	GPIO_InitStruct;
	
	SPI_CLKCmd(SPI_CLK,ENABLE);
	GPIO_CLKCmd(GPIO_CLK,ENABLE);
	
	//SCK �����츴�����
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

	
	//NSS ���������(�������)
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin=SPI_NSS_PIN;
	GPIO_Init(SPI_NSS_PORT,&GPIO_InitStruct);
	
	//ֹͣ�ź�NSS ���Ÿߵ�ƽ
	SPI_NSS_HIGH();
}
/*
	*@brief spiģʽ����
	*@param ��
	*@retval ��
	*/
void SPI_Mode_Config(void){
	
	SPI_InitTypeDef 	SPI_InitStruct;
	/*
	������,˫��ȫ˫��ģʽ������֡����Ϊ8λ
	ʹ��SPIģʽ3(CPOL=1,CPHA=1)
	NSS�������������,MSB����ģʽ
	ʱ��Ƶ��4��Ƶ
	*/
	SPI_InitStruct.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode=SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize=SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL=SPI_CPOL_High;
	SPI_InitStruct.SPI_CPHA=SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_NSS=SPI_NSS_Soft ;	//	���ģ��
	SPI_InitStruct.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_4;
	SPI_InitStruct.SPI_FirstBit=SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial=0;//������CRC�Ĺ���
	SPI_Init(FLASH_SPIx	,&SPI_InitStruct);
	//ʹ��SPI
	SPI_Cmd(FLASH_SPIx,ENABLE);
	
}
void FLASH_SPI_Init(void){
	SPI_GPIO_Init();
	SPI_Mode_Config();
	
}
//���Ͳ�����һ������
u8 SPI_FLASH_SendByte(u8 byte){
	
		SPITimeout	=	 SPI_FLAG_TIMEOUT;
	
	//�ȴ����ͻ�����λ�գ�TXEʱ��
	while(SPI_I2S_GetFlagStatus(FLASH_SPIx,SPI_I2S_FLAG_TXE) == RESET){
			if((SPITimeout--)==0)	//��ֹ��ѭ������ʱ�˳�
				return SPI_TIMEOUT_UserCallback(0);
	}
	//�������ѿգ���������
	SPI_I2S_SendData(FLASH_SPIx,byte);
	SPITimeout = SPI_FLAG_TIMEOUT;
	//��鲢�ȴ���RX������λ�ǿ�
	while(SPI_I2S_GetFlagStatus(FLASH_SPIx,SPI_I2S_FLAG_RXNE) == RESET){
		if((SPITimeout--)==0)	//��ֹ��ѭ������ʱ�˳�
				return SPI_TIMEOUT_UserCallback(1);
	}
	return SPI_I2S_ReceiveData(FLASH_SPIx);
}
/*
	*@brief ��ȡFLASH��ID,���ڲ��Գ������ȷ��
	*@param ��
	*@retval FLASH ID
	*/
uint32_t SPI_Read_ID(void){
	//FLASH ID �����ֽ���ɣ�ÿһ�ֽ�8λ����Ҫ����
	
	uint32_t flash_id=0;
	//��ʼͨѶ,sck���
	SPI_NSS_LOW();
	SPI_FLASH_SendByte(READ_JEDEC_ID);
	
	flash_id=SPI_FLASH_SendByte(Dummy_Byte);
	
	flash_id <<=8;
	
	flash_id |= SPI_FLASH_SendByte(Dummy_Byte);
	
	flash_id <<=8;
	
	flash_id |= SPI_FLASH_SendByte(Dummy_Byte);
	
	SPI_NSS_HIGH();	//����ͨѶ
	
	return flash_id;
}



u8 SPI_FLASH_ReadByte(void){
	return (SPI_FLASH_SendByte(Dummy_Byte));
}
/*
	*@brief Basic management of the timeout situation
	*@param errorCode:������룬���Զ�λ�Ǹ����ڳ���
	*@retval ����0����ʾSPI��ȡʧ��
	*/
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* �ȴ���ʱ��Ĵ���,���������Ϣ */
  FLASH_ERROR("SPI �ȴ���ʱ!errorCode = %d",errorCode);
  return 0;
}
/*
	*@brief ����FLASHָ������
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
	*@brief �ȴ��ڲ�ʱ��������
	*@param  ��
	*@retval ��
	*/
void SPI_WaitForWriteEnd(void){
	
	//״̬�Ĵ���
	uint8_t status_reg=0;
	
	SPI_NSS_LOW();
	
	(SPI_FLASH_SendByte(READ_STATUS));
	
	do{
		status_reg=SPI_FLASH_SendByte(Dummy_Byte);	
		
	}while((status_reg& 0x01)==1);
	
	SPI_NSS_HIGH();
	
}
/*
	*@brief ��ȡFLASH������
	*@param  ��
	*@retval ��
	*/
void SPI_Read_Data(uint32_t addr,uint8_t *readbuffer,uint32_t numByteToRead){
	
	SPI_NSS_LOW();
	
	SPI_FLASH_SendByte(READ_DATA);
	
	SPI_FLASH_SendByte((addr>>16)&0xff);
	
	SPI_FLASH_SendByte((addr>>8)&0xff);
	
	SPI_FLASH_SendByte(addr & 0xff);
	//���������ֽڵ�ַ
	while(numByteToRead--){
		
		*readbuffer=SPI_FLASH_SendByte(Dummy_Byte);
			readbuffer++;	
	}
	
	SPI_NSS_HIGH();
	
}


/*
	*@brief д��FLASH������
	*@param  ��
	*@retval ��
	*/
void SPI_Write_Data(uint32_t addr,uint8_t *writebuffer,uint32_t numByteToWrite){
	
	SPI_FLASH_WriteEnable();//д��ʹ�ܣ�Ҫ��FLASHд�����ݣ�����Ҫʹ�ô˺���
	
	SPI_NSS_LOW();//Ƭѡʹ��
	
	SPI_FLASH_SendByte(WRITE_DATA);
	
	SPI_FLASH_SendByte((addr>>16)&0xff);
	
	SPI_FLASH_SendByte((addr>>8)&0xff);
	
	SPI_FLASH_SendByte(addr & 0xff);
	//���������ֽڵ�ַ
	while(numByteToWrite--){
		
		SPI_FLASH_SendByte(*writebuffer);
			writebuffer++;	
	}
	
	SPI_NSS_HIGH();
	SPI_WaitForWriteEnd();
}

/*
	*@brief  ��FLASH����дʹ��
	*@param  ��
	*@retval ��
	*/
void SPI_FLASH_WriteEnable(void){
	
	//Ƭѡʹ��
	SPI_NSS_LOW();
	
	//����дʹ������
	SPI_FLASH_SendByte(Writeable);
	
	//����ͨѶ
	SPI_NSS_HIGH();
	
}

