#include "bsp_i2c_ee.h"



uint8_t ee_CheckDevice(uint8_t _Address){
	uint8_t ucAck;
	
	//���������ź�
	I2c_Start();
	
	//�����豸��ַ+��д����λ
	I2c_SendByte(_Address|EEPROM_I2C_WR);
	ucAck=i2c_WaitAck();
	
	
	I2c_Stop();	/* ����ֹͣ�ź� */
	i2c_NAck();	/*��������Ƕ���ַ����Ҫ������Ӧ���ź�*/
	return ucAck;
}

/*
*	���ܣ��ȴ�EEPROM��׼��״̬����д�����ݺ󣬱�����ñ�����
*				д�����ʱ��ʹ�� I2C�����ݴ��䵽EEPROM��
*				EEPROM�����ڲ��ռ�д��������Ҫһ����ʱ�䣬
*				��EEPEOMд����ɺ󣬻��I2C�豸Ѱַ����Ӧ
*				
*				���ñ������ɵȴ���EEPROM�ڲ�ʱ��д�����
*/
u8	ee_WaitStandby(void){
	u32	wait_count=0;
	
	while(ee_CheckDevice(EEPROM_DEV_ADDR))
	{
		//����ⳬ���������˳�ѭ��(������ѭ����	
		if(wait_count ++ >0xFFFF)
			//�ȴ���ʱ	
			return 1;
	}
	return 0;
}
/*
*	���ܣ�����EEPROMָ����ַд���������ݣ�����ҳ�����������д��Ч��
*				
*	�βΣ�	_usAddress:��ʼ��ַ		
*				_usSize:���ݳ��ȣ���λΪ�ֽ�
*				_pWriteBuf:��Ŷ��������ݵĻ�����ָ��
*/
u8	ee_WriteBytes(u8 *_pWriteBuf,u16 	_usAddress,u16 _usSize){
	u16 i,m,usAddr;
	/*
	д����EEPROM���������һ������������ȡ�ܶ��ֽڣ�ÿ�β�����ֻ��д��ͬһ��page
	��Ӧ24xx02оƬ��page size =8;
	�򵥵Ĵ���ʽΪ�������ֽ�д����ģʽ��ÿдһ���ֽڣ������͵�ַ
	���Ч�ʲ��� page write
	
	*/
	usAddr = _usAddress;
	for(i=0;i<_usSize;i++){
		
		//�����͵�һ���ֽڻ�����ҳ���׵�ַʱ����Ҫ���·��������źź͵�ַ
		if((i==0)||(usAddr&(EEPROM_PAGE_SIZE-1))==0){
			//��0��������ֹͣ�źţ�������һҳ��ͨѶ��׼����һ��ͨѶ
			I2c_Stop();
			/*	ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ���ɣ�һ��С��10ms
			CLKƵ��ΪKHzʱ,��ѯ����Ϊ30������
			ԭ����ee_WaitStandby()���������Ǹú��������ɺ�����ֹͣ�źţ�
			�������ڴ˴�
			*/
			for(m=0;m < 1000; m++){
			//��һ��������I2C�����ź�
				
				I2c_Start();
				
			//�ڶ�������������ֽڣ���7bit�ǵ�ַ��bit0Ϊ��д����λ��0��д��1����
				
				I2c_SendByte(EEPROM_DEV_ADDR|EEPROM_I2C_WR);
				
			//������:����һ��ʱ�ӣ��ж������Ƿ���ȷӦ��
					if(i2c_WaitAck() == 0)
							break;
				}
				if(m == 1000)
						goto cmd_fail;//EEPROM����д��ʱ
				
				//���Ĳ�:�����ֽڵ�ַ��24c02ֻ��256���ֽڣ����1���ֽھ͹��ˣ�����������ͺţ�\
				Ҫ���������ַ
				I2c_SendByte((u8)usAddr);
				//���岽:�ȴ�ACK
				if(i2c_WaitAck() != 0)
				{
						goto cmd_fail;//EEPROM������Ӧ��
				}
			}
				//������:��ʼд������
				I2c_SendByte(_pWriteBuf[i]);
				
				//���߲�:����ACK
				if(i2c_WaitAck() != 0)
						goto cmd_fail;//EEPROM������Ӧ��
				
				usAddr++;		//��ַ��1
			}
			//����ִ�гɹ�������I2C����ֹͣ�ź�
			I2c_Stop();
		
			//�ȴ����һ�� EEPROM�ڲ�д�����
				if(ee_WaitStandby() == 1)
						goto cmd_fail;			
			return 1;
	
cmd_fail:	/* 	����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸*/
		I2c_Stop();
		return 0;
}


/*
*	���ܣ��Ӵ���EEPROMָ����ַ��ȡ��������
*				
*	�βΣ�	_usAddress:��ʼ��ַ		
*				_usSize:���ݳ��ȣ���λΪ�ֽ�
*				_pReadBuf:��Ŷ��������ݵĻ�����ָ��
*/
u8	ee_ReadBytes(u8 *_pReadBuf,u16	_usAddress,u16 _usSize){
	u16 i;
	
	//��һ��������I2C���������ź�
	I2c_Start();
	
	//�ڶ���:��������ֽڣ�����λ�ǵ�ַ�����һλ�Ƕ�дλ ��0��д��1����
	I2c_SendByte(EEPROM_DEV_ADDR|EEPROM_I2C_WR);//д����д��ַ
	
	//������:�ȴ�ACK
	if(i2c_WaitAck() != 0)
			goto cmd_fail;		//EEPROM������Ӧ��
	
	//���Ĳ�:�����ֽڵ�ַ��24c02ֻ��256���ֽڣ����һ���ֽھ͹���
	I2c_SendByte((u8)_usAddress);
	
	//���岽���ȴ�ACK
	if(i2c_WaitAck() != 0)
			goto cmd_fail;		//EEPROM������Ӧ��
	
	//������:��������I2C����
	//ǰ���Ŀ����δ����EEPROM���͵�ַ�����濪ʼ��ȡ����
	I2c_Start();
	
	//���߲�:���Ϳ����ֽ�
	I2c_SendByte(EEPROM_DEV_ADDR|EEPROM_I2C_RD);
	
	//�ڰ˲�:����ACK
	if(i2c_WaitAck() != 0)
			goto cmd_fail;		//EEPROM������Ӧ��
	
	//�ھŲ�:ѭ����ȡ����
	for(i = 0; i < _usSize; i++){
		
		_pReadBuf[i]=i2c_ReadByte();//��һ���ֽ�
		
		//ÿ�ζ���һ���ֽں���Ҫ����ACK�����һ���ֽڲ���Ҫ����ACK,����NACK
		if(i != _usSize -1)
				i2c_Ack();
		else
			i2c_NAck();
	}
	
	//����ֹͣ�ź�
	I2c_Stop();	
	return 1;	//ִ�гɹ�
cmd_fail:	//����ִ��ʧ�ܣ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����ϵ������豸
	//I2C����ֹͣ�ź�
	I2c_Stop();
	return 0;
}


//EPROM��д����	
u8 ee_test(void){
	u16 i;
	u8 writebuf[EEPROM_SIZE];
	u8 readbuf[EEPROM_SIZE];
	
	if(ee_CheckDevice(EEPROM_DEV_ADDR)==1){
		printf("û�м�⵽����EEPROM\r\n");
		return 0;
	}
	//��仺����
	for(i = 0;i < EEPROM_SIZE; i++)
		writebuf[i]=i;
	
	//д
	if(ee_WriteBytes(writebuf,0,EEPROM_SIZE) == 0){
			printf("дEEPROM����\r\n");
			return 0;
	}else
	printf("дEEPROM�ɹ�\r\n");
	//��
		if(ee_ReadBytes(readbuf,0,EEPROM_SIZE) == 0){
			printf("��EEPROM����\r\n");
			return 0;
	}else
	printf("��EEPROM�ɹ�\r\n");
	
	//�������������(��һ��)
	printf("readbuf:\n");
	for(i=0;i<EEPROM_SIZE ; i++){
		if(readbuf[i] != writebuf[i]){
			printf("0x%02X ", readbuf[i]);
			printf("����EEPROM������д������ݲ�һ��\r\n");
			return 0;
		}
		if((i % 16)== 0&&(i != 0))
			printf("\n");
		printf(" %02X", readbuf[i]);
		
	}
	printf("\nEEPROM ��д���Գɹ�\r\n");
	return 1;
}
