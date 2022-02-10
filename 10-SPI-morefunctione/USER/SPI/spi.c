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
	*@brief д��FLASH������(һ�����д��256���ֽ�)
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

/*
	*@brief  ��FLASHд�����ݣ���д������֮ǰҪ�Ȳ�������(д�����ֽ�)
	*@param  pbuffer��Ҫд�������
	*@param  WriteAddr��Ҫд��ĵ�ַ		
	*@param  NumByteToWrite��Ҫд�����ݵĳ���
	*@retval ��
	*/
void SPI_FLASH_BufferWrite(uint8_t *pbuffer,uint32_t WriteAddr,uint16_t NumByteToWrite){
	uint8_t NumOfPage = 0,NumOfSingle = 0, Addr = 0,count = 0,temp=0;
	
	//Ƭѡʹ��
	//SPI_NSS_LOW();
	//mod����ȡ�࣬�� WriteAddr ������ SPI_FLASH_PageSize ��������Addr=0
	Addr = WriteAddr % SPI_FLASH_PageSize;
	//�����Ҫд��������ҳ
	NumOfPage = NumByteToWrite / SPI_FLASH_PageSize;
	//mod �������࣬���������һҳ������
	NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
	
	//Addr=0,��WriteAddr�պð�ҳ���� 
	if(Addr == 0){
		
		if(NumOfPage == 0)
		{
			SPI_Write_Data(WriteAddr,pbuffer,NumByteToWrite);
		}
		else{	
			/*NumByteToWrite > SPI_FLASH_PageSize(256)*/
			//�Ȱ���ҳ��д��
			while(NumOfPage--){
				SPI_Write_Data(WriteAddr,pbuffer,SPI_FLASH_PageSize);
					WriteAddr += SPI_FLASH_PageSize;
					pbuffer += SPI_FLASH_PageSize;
		}
			//���ж���Ĳ���һҳ�����ݣ�����д��
			SPI_Write_Data(WriteAddr,pbuffer,NumOfSingle);
	}}
	//����ַ�� SPI_FLASH_PageSize ������
	else{
		/* NumByteToWrite < SPI_FLASH_PageSize */
		if(NumOfPage == 0)
		{
			/* ��ǰҳʣ���countλ�ñ�NumOfSingle С��һҳд���� */
			if(NumOfSingle > count){
				
				temp= NumOfSingle - count;
				
				/* ��д����ǰҳ */
				SPI_Write_Data(WriteAddr,pbuffer,count);
				
				WriteAddr += count;
				pbuffer += count;
				
				/* ��дʣ������� */
				SPI_Write_Data(WriteAddr,pbuffer,temp);
			}
			else{/*��ǰҳʣ���count��λ����д��NumOfSingle������*/
				SPI_Write_Data(WriteAddr,pbuffer,NumByteToWrite);
			}
		}
		/* NumByteToWrite > SPI_FLASH_PageSize */
		else{
			/* ��ַ����������count�ֿ���������������� */
			
			NumByteToWrite -= count;
			NumOfPage = NumByteToWrite / SPI_FLASH_PageSize;
			NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
			
			/* ��д��count�����ݣ�Ϊ������һ��д������ݵ�ַ���� */
			SPI_Write_Data(WriteAddr,pbuffer,count);
			
			/* ���������ظ���ַ�������� */
      WriteAddr +=  count;
      pbuffer += count;
			/*������ҳ��д��*/
      while (NumOfPage--)
      {
        SPI_Write_Data(WriteAddr,pbuffer,SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pbuffer += SPI_FLASH_PageSize;
      }
			
			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
      if (NumOfSingle != 0)
      {
        SPI_Write_Data(WriteAddr,pbuffer,NumOfSingle);
      }
		}
	}
}

/*
	*@brief  ��FLASH����дʹ��
	*@param  ��
	*@retval ��
	*tips: Ҫ�ӵ���ģʽ���ѣ�ʹ��readDevice����
	*/
void SPI_FLASH_PowerDown(void){
	
	SPI_NSS_LOW();
	
	//����дʹ������
	SPI_FLASH_SendByte(Power_Down);
	
	//����ͨѶ
	SPI_NSS_HIGH();
}
