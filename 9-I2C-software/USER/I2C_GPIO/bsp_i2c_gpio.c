#include "bsp_i2c_gpio.h"



//I2C_GPIO 配置
void I2C_GPIO_Configue(void){
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	//开I2C_GPIO的时钟
	I2C_GPIO_Cmd(I2C_GPIO_CLK,ENABLE);
	
	//SDA 开漏输出
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin=EEPROM_I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(EEPROM_I2C_SDA_PORT, &GPIO_InitStructure);
	
	//SCL
	GPIO_InitStructure.GPIO_Pin=EEPROM_I2C_SCL_PIN;
	GPIO_Init(EEPROM_I2C_SCL_PORT, &GPIO_InitStructure);
	
}
//I2C 模式配置
//void I2C_Mode_Configue(void){
//	I2C_InitTypeDef	I2C_InitStructure;
//	
//	I2C_ClockCmd(I2C_CLK,ENABLE);
//	
//	//模式
//	I2C_InitStructure.I2C_Ack=I2C_Ack_Enable;
//	I2C_InitStructure.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;
//	I2C_InitStructure.I2C_ClockSpeed=I2C_Speed ;
//	I2C_InitStructure.I2C_DutyCycle=I2C_DutyCycle_2 ;
//	I2C_InitStructure.I2C_Mode=I2C_Mode_I2C ;
//	I2C_InitStructure.I2C_OwnAddress1=I2C_OwnAddress ;
//	
//	I2C_Init(I2Cx,&I2C_InitStructure);



//  I2C_Cmd(I2Cx,ENABLE);
//}


void I2C_Configue(void){
	I2C_GPIO_Configue();
	
	//I2C_Mode_Configue();
}


//延时
static void  Delay(void){
	uint8_t i;
	for(i=0;i <10;i++);
}
	


/*	
	*起始信号
	*SCL高电平,SDA低电平	
*/

void I2c_Start(void)
{
	/* 当 SCL 高电平时，SDA 出现一个下跳沿
	表示 I2C 总线启动信号 */
	I2C_SCL_HIGH();
	I2C_SDA_HIGH();
	Delay();
	I2C_SDA_LOW();
	Delay();
	I2C_SCL_LOW();
	Delay();
}

/*	
	*结束信号
	*SCL高电平,SDA高电平	
*/

void I2c_Stop(void)
{
	/* 当 SCL 高电平时，
	SDA 出现一个上跳沿表示
	I2C 总线停止信号 */
	I2C_SDA_LOW();
	I2C_SCL_HIGH();
	Delay();
	I2C_SDA_HIGH();
}

//向I2C总线设备发送 8bit 数据
void I2c_SendByte(uint8_t _ucByte){
	uint8_t i;
	
	//线发送字节高位 bit7
	for(i=0; i<8;i++){
		//判断其最高位的逻辑值，为 1 时控制 SDA 输出高电平，为 0 则控制 SDA 输出低电平
			if(_ucByte &0x80){
						I2C_SDA_HIGH();
				
			}else{
						I2C_SDA_LOW();
			}
			Delay();
			I2C_SCL_HIGH();
			Delay();
			I2C_SCL_LOW();
			if(i == 7){
				I2C_SDA_HIGH();	//释放总线	,等待接收方的应答信号			
			}
			_ucByte <<= 1; 	//左移一个bit，方便下一次循环发送下一位数据
			Delay();
	}
}
//CPU 从I2C总线设备读取8bit
uint8_t i2c_ReadByte(void){
	uint8_t i,value;
	
	//读取第一个bit为数据bit7
	value = 0;
	for(i=0;i<8;i++){
		value <<= 1;	//串行读
		I2C_SCL_HIGH();
		Delay();
		if(I2C_SDA_READ()){
			value++;
		}
			I2C_SCL_LOW();
		Delay();
	}
	return value;
}


//CPU产生一个时钟，并读取器件ACK应答信号
uint8_t i2c_WaitAck(void){
	uint8_t re;
	
	I2C_SDA_HIGH();//SDA的信号线输出高阻态，释放他对SDA的控制权,CPU 释放 SDA 总线
	Delay();
	I2C_SCL_HIGH();//CPU 释放 SCL 总线 , 此时器件会返回ACK应答
	
	Delay();
	if(I2C_SDA_READ())
		re=1;
	else re=0;
	I2C_SCL_LOW();
	Delay();
	return re;
}
 
//CPU产生一个 ACK 应答信号
void i2c_Ack(void){
	I2C_SDA_LOW();	//CPU 驱动 SDA = 0
	Delay();
	I2C_SCL_HIGH();//CPU 产生一个 时钟
	Delay();
	I2C_SCL_LOW();
	Delay();
	I2C_SDA_HIGH();	//CPU释放 SDA 总线
}

//CPU产生一个 NACK 应答信号
//SCL信号线切换高低电平，产生一个时钟信号
void i2c_NAck(void){
	I2C_SDA_HIGH();	//CPU 驱动 SDA = 1
	Delay();
	I2C_SCL_HIGH();//CPU 产生一个 时钟
	Delay();
	I2C_SCL_LOW();
	Delay();
}

