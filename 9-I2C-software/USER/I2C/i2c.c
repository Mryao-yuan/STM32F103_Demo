#include "i2c.h"



//I2C EEPROM �����ù�����������
void I2c_Configue(void){
	
	I2C_InitTypeDef 		I2C_InitStruct;
	GPIO_InitTypeDef		GPIO_InitStructure;
	
	
	//��I2Cʱ��
	EEPROM_I2C1_APBxClkCmd(EEPROM_I2C1_CLK ,ENABLE);
	EEPROM_I2C1_GPIO_APBxClkCmd(EEPROM_I2C1_SCL_GPIO_CLK|EEPROM_I2C1_SDA_GPIO_CLK,ENABLE);
	
	//��I2C SCL����Ϊ��©����ģʽ
	GPIO_InitStructure.GPIO_Pin=EEPROM_I2C1_SCL_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(EEPROM_I2C1_SCL_GPIO_PORT, &GPIO_InitStructure);
	
	//��I2C SDA����Ϊ��©����ģʽ(�����ģʽ���ٶ��������һ�£�����Ҫ�޸Ŀ���ʡ��)
	GPIO_InitStructure.GPIO_Pin=EEPROM_I2C1_SDA_GPIO_PIN;
	GPIO_Init(EEPROM_I2C1_SDA_GPIO_PORT, &GPIO_InitStructure);
	
	//���� I2C �Ĺ�������
	I2C_InitStruct.I2C_Ack=I2C_Ack_Enable ;//ʹ��Ӧ��
	I2C_InitStruct.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit ;//Ӧ���ַ
	I2C_InitStruct.I2C_ClockSpeed=EEPROM_I2C1_BAUDRATE;//���ò�����
	I2C_InitStruct.I2C_DutyCycle=I2C_DutyCycle_2;//ռ�ձ�
	I2C_InitStruct.I2C_Mode=I2C_Mode_I2C;
	I2C_InitStruct.I2C_OwnAddress1=STM32_I2C_OWNER_ADDR;//������ַ(STM32 I2C�ϵĵ�ַ��ֻҪ��Ψһ����)

	I2C_Init(I2Cx, &I2C_InitStruct);
	//ʹ��I2C
	I2C_Cmd(I2Cx,ENABLE);
}

//��EEPROMд��һ���ֽ�
void EEPROM_Byte_Write(uint8_t addr,uint8_t data){
	//������ʼ�ź�
	I2C_GenerateSTART(I2Cx,ENABLE);
	//���EV5�¼�
	while(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)==ERROR);
	
	//�����豸��ַ
	I2C_Send7bitAddress(I2Cx, addr,I2C_Direction_Transmitter);
	//���EV6�¼�
	while(I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);
	
	//��������(���ǻᱻ�豸��Ϊ��Ҫд����ڴ浥Ԫ�ĵ�ַ)
	I2C_SendData(I2Cx,addr);
	//EV8
	while(I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTING )==ERROR);
	
	//����Ҫ�洢������
	I2C_SendData(I2Cx,data);
	//EV8-����
	while(I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED )==ERROR);
	
	//�����ź�
	I2C_GenerateSTOP(I2Cx, ENABLE);
}

//��EEPROM��ȡһ���ֽ�(���ݲ���ָ��)
void EEPROM_Byte_Read(uint8_t addr,uint8_t *data,uint8_t numByteToRead){
	
	//������ʼ�ź�
	I2C_GenerateSTART(I2Cx,ENABLE);
	//���EV5�¼�
	while(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)==ERROR);
	
	//�����豸��ַ
	I2C_Send7bitAddress(I2Cx, addr,I2C_Direction_Transmitter);
	//���EV6�¼�
	while(I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);
	
	//��ȡ����(���ǻᱻ�豸��Ϊ��Ҫд����ڴ浥Ԫ�ĵ�ַ)
	I2C_SendData(I2Cx,addr);
	//EV8
	while(I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTING )==ERROR);
	
	/*
			�ڶ�����ʼ�ź�(��)
	*/
	
	//������ʼ�ź�
	I2C_GenerateSTART(I2Cx,ENABLE);
	//���EV5�¼�
	while(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)==ERROR);
	//�����豸��ַ
	I2C_Send7bitAddress(I2Cx, addr,I2C_Direction_Receiver);
	//���EV6�¼�
	while(I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)==ERROR);
	
	
	
		
	while(numByteToRead){
	
	//���һ���ֽ�(��Ӧ��
	if(numByteToRead==1){
	I2C_AcknowledgeConfig(I2Cx,DISABLE);
	*data=I2C_ReceiveData(I2Cx);
	}
		
	//EV7�¼�����⵽���Ĵ������µ�����
	while(I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED )==ERROR);
	//��ȡҪ�洢������
	*data=I2C_ReceiveData(I2Cx);

	numByteToRead--;
	data++;
	}	
	//EV8-����
	while(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED )==ERROR);
	
	//�����ź�
	I2C_GenerateSTOP(I2Cx, ENABLE);
	
	//��������ACKʹ�ܣ��Ա����´�ͨѶ
	I2C_AcknowledgeConfig(I2Cx,ENABLE);
}


//EEPROM�ڲ�ʱ�����
void EEEPROM_WaitForEnd(void){
	//��ͣ�Ĳ�����ʼ�ź�
	do{
		
		I2C_GenerateSTART(I2Cx,ENABLE);
	
		while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_SB)==	RESET);
	
		
	//�����豸��ַ
	I2C_Send7bitAddress(I2Cx, STM32_I2C_OWNER_ADDR,I2C_Direction_Transmitter);
	}while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_ADDR)==RESET);

	//�ڲ�ʱ�����
	I2C_GenerateSTOP(I2Cx, ENABLE);
	
}




//��EEPROMд�����ֽ�(ҳд��),ÿ��д�벻�ܳ���8�ֽ�
void EEPROM_Page_Write(uint8_t addr,uint8_t data,uint8_t numByteToWrite){
	//������ʼ�ź�
	I2C_GenerateSTART(I2Cx,ENABLE);
	//���EV5�¼�
	while(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)==ERROR);
	
	//�����豸��ַ
	I2C_Send7bitAddress(I2Cx, addr,I2C_Direction_Transmitter);
	//���EV6�¼�
	while(I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);
	
	
	//��������(���ǻᱻ�豸��Ϊ��Ҫд����ڴ浥Ԫ�ĵ�ַ)
	I2C_SendData(I2Cx,addr);
	//EV8
	while(I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTING )==ERROR);
	
	
	while(numByteToWrite){
		
		//����Ҫ�洢������
		I2C_SendData(I2Cx,data);
		//EV8-����
		while(I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED )==ERROR);
		numByteToWrite--;
		data++;
	}
	//�����ź�
	I2C_GenerateSTOP(I2Cx, ENABLE);
}



