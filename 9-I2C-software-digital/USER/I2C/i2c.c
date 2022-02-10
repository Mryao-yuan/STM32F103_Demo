#include "i2c.h"



//I2C EEPROM 的配置工作参数配置
void I2c_Configue(void){
	
	I2C_InitTypeDef 		I2C_InitStruct;
	GPIO_InitTypeDef		GPIO_InitStructure;
	
	
	//开I2C时钟
	EEPROM_I2C1_APBxClkCmd(EEPROM_I2C1_CLK ,ENABLE);
	EEPROM_I2C1_GPIO_APBxClkCmd(EEPROM_I2C1_SCL_GPIO_CLK|EEPROM_I2C1_SDA_GPIO_CLK,ENABLE);
	
	//将I2C SCL设置为开漏复用模式
	GPIO_InitStructure.GPIO_Pin=EEPROM_I2C1_SCL_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(EEPROM_I2C1_SCL_GPIO_PORT, &GPIO_InitStructure);
	
	//将I2C SDA设置为开漏复用模式(下面的模式和速度与上面的一致，不需要修改可以省略)
	GPIO_InitStructure.GPIO_Pin=EEPROM_I2C1_SDA_GPIO_PIN;
	GPIO_Init(EEPROM_I2C1_SDA_GPIO_PORT, &GPIO_InitStructure);
	
	//配置 I2C 的工作参数
	I2C_InitStruct.I2C_Ack=I2C_Ack_Enable ;//使能应答
	I2C_InitStruct.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit ;//应答地址
	I2C_InitStruct.I2C_ClockSpeed=EEPROM_I2C1_BAUDRATE;//配置波特率
	I2C_InitStruct.I2C_DutyCycle=I2C_DutyCycle_2;//占空比
	I2C_InitStruct.I2C_Mode=I2C_Mode_I2C;
	I2C_InitStruct.I2C_OwnAddress1=STM32_I2C_OWNER_ADDR;//主机地址(STM32 I2C上的地址，只要是唯一即可)

	I2C_Init(I2Cx, &I2C_InitStruct);
	//使能I2C
	I2C_Cmd(I2Cx,ENABLE);
}

//向EEPROM写入一个字节
void EEPROM_Byte_Write(uint8_t addr,uint8_t data){
	//产生起始信号
	I2C_GenerateSTART(I2Cx,ENABLE);
	//检测EV5事件
	while(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)==ERROR);
	
	//发送设备地址
	I2C_Send7bitAddress(I2Cx, addr,I2C_Direction_Transmitter);
	//检测EV6事件
	while(I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);
	
	//发送数据(但是会被设备认为是要写入的内存单元的地址)
	I2C_SendData(I2Cx,addr);
	//EV8
	while(I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTING )==ERROR);
	
	//发送要存储的数据
	I2C_SendData(I2Cx,data);
	//EV8-结束
	while(I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED )==ERROR);
	
	//结束信号
	I2C_GenerateSTOP(I2Cx, ENABLE);
}

//向EEPROM读取一个字节(数据采用指针)
void EEPROM_Byte_Read(uint8_t addr,uint8_t *data,uint8_t numByteToRead){
	
	//产生起始信号
	I2C_GenerateSTART(I2Cx,ENABLE);
	//检测EV5事件
	while(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)==ERROR);
	
	//发送设备地址
	I2C_Send7bitAddress(I2Cx, addr,I2C_Direction_Transmitter);
	//检测EV6事件
	while(I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);
	
	//读取数据(但是会被设备认为是要写入的内存单元的地址)
	I2C_SendData(I2Cx,addr);
	//EV8
	while(I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTING )==ERROR);
	
	/*
			第二次起始信号(读)
	*/
	
	//产生起始信号
	I2C_GenerateSTART(I2Cx,ENABLE);
	//检测EV5事件
	while(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)==ERROR);
	//发送设备地址
	I2C_Send7bitAddress(I2Cx, addr,I2C_Direction_Receiver);
	//检测EV6事件
	while(I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)==ERROR);
	
	
	
		
	while(numByteToRead){
	
	//最后一个字节(不应答）
	if(numByteToRead==1){
	I2C_AcknowledgeConfig(I2Cx,DISABLE);
	*data=I2C_ReceiveData(I2Cx);
	}
		
	//EV7事件被检测到，寄存器有新的数据
	while(I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED )==ERROR);
	//读取要存储的数据
	*data=I2C_ReceiveData(I2Cx);

	numByteToRead--;
	data++;
	}	
	//EV8-结束
	while(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED )==ERROR);
	
	//结束信号
	I2C_GenerateSTOP(I2Cx, ENABLE);
	
	//重新配置ACK使能，以便于下次通讯
	I2C_AcknowledgeConfig(I2Cx,ENABLE);
}


//EEPROM内部时序完成
void EEEPROM_WaitForEnd(void){
	//不停的产生起始信号
	do{
		
		I2C_GenerateSTART(I2Cx,ENABLE);
	
		while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_SB)==	RESET);
	
		
	//发送设备地址
	I2C_Send7bitAddress(I2Cx, STM32_I2C_OWNER_ADDR,I2C_Direction_Transmitter);
	}while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_ADDR)==RESET);

	//内部时序完成
	I2C_GenerateSTOP(I2Cx, ENABLE);
	
}




//向EEPROM写入多个字节(页写入),每次写入不能超过8字节
void EEPROM_Page_Write(uint8_t addr,uint8_t data,uint8_t numByteToWrite){
	//产生起始信号
	I2C_GenerateSTART(I2Cx,ENABLE);
	//检测EV5事件
	while(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)==ERROR);
	
	//发送设备地址
	I2C_Send7bitAddress(I2Cx, addr,I2C_Direction_Transmitter);
	//检测EV6事件
	while(I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);
	
	
	//发送数据(但是会被设备认为是要写入的内存单元的地址)
	I2C_SendData(I2Cx,addr);
	//EV8
	while(I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTING )==ERROR);
	
	
	while(numByteToWrite){
		
		//发送要存储的数据
		I2C_SendData(I2Cx,data);
		//EV8-结束
		while(I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED )==ERROR);
		numByteToWrite--;
		data++;
	}
	//结束信号
	I2C_GenerateSTOP(I2Cx, ENABLE);
}



