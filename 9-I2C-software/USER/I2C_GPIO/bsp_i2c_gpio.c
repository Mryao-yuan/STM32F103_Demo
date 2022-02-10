#include "bsp_i2c_gpio.h"



//I2C_GPIO ����
void I2C_GPIO_Configue(void){
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	//��I2C_GPIO��ʱ��
	I2C_GPIO_Cmd(I2C_GPIO_CLK,ENABLE);
	
	//SDA ��©���
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin=EEPROM_I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(EEPROM_I2C_SDA_PORT, &GPIO_InitStructure);
	
	//SCL
	GPIO_InitStructure.GPIO_Pin=EEPROM_I2C_SCL_PIN;
	GPIO_Init(EEPROM_I2C_SCL_PORT, &GPIO_InitStructure);
	
}
//I2C ģʽ����
//void I2C_Mode_Configue(void){
//	I2C_InitTypeDef	I2C_InitStructure;
//	
//	I2C_ClockCmd(I2C_CLK,ENABLE);
//	
//	//ģʽ
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


//��ʱ
static void  Delay(void){
	uint8_t i;
	for(i=0;i <10;i++);
}
	


/*	
	*��ʼ�ź�
	*SCL�ߵ�ƽ,SDA�͵�ƽ	
*/

void I2c_Start(void)
{
	/* �� SCL �ߵ�ƽʱ��SDA ����һ��������
	��ʾ I2C ���������ź� */
	I2C_SCL_HIGH();
	I2C_SDA_HIGH();
	Delay();
	I2C_SDA_LOW();
	Delay();
	I2C_SCL_LOW();
	Delay();
}

/*	
	*�����ź�
	*SCL�ߵ�ƽ,SDA�ߵ�ƽ	
*/

void I2c_Stop(void)
{
	/* �� SCL �ߵ�ƽʱ��
	SDA ����һ�������ر�ʾ
	I2C ����ֹͣ�ź� */
	I2C_SDA_LOW();
	I2C_SCL_HIGH();
	Delay();
	I2C_SDA_HIGH();
}

//��I2C�����豸���� 8bit ����
void I2c_SendByte(uint8_t _ucByte){
	uint8_t i;
	
	//�߷����ֽڸ�λ bit7
	for(i=0; i<8;i++){
		//�ж������λ���߼�ֵ��Ϊ 1 ʱ���� SDA ����ߵ�ƽ��Ϊ 0 ����� SDA ����͵�ƽ
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
				I2C_SDA_HIGH();	//�ͷ�����	,�ȴ����շ���Ӧ���ź�			
			}
			_ucByte <<= 1; 	//����һ��bit��������һ��ѭ��������һλ����
			Delay();
	}
}
//CPU ��I2C�����豸��ȡ8bit
uint8_t i2c_ReadByte(void){
	uint8_t i,value;
	
	//��ȡ��һ��bitΪ����bit7
	value = 0;
	for(i=0;i<8;i++){
		value <<= 1;	//���ж�
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


//CPU����һ��ʱ�ӣ�����ȡ����ACKӦ���ź�
uint8_t i2c_WaitAck(void){
	uint8_t re;
	
	I2C_SDA_HIGH();//SDA���ź����������̬���ͷ�����SDA�Ŀ���Ȩ,CPU �ͷ� SDA ����
	Delay();
	I2C_SCL_HIGH();//CPU �ͷ� SCL ���� , ��ʱ�����᷵��ACKӦ��
	
	Delay();
	if(I2C_SDA_READ())
		re=1;
	else re=0;
	I2C_SCL_LOW();
	Delay();
	return re;
}
 
//CPU����һ�� ACK Ӧ���ź�
void i2c_Ack(void){
	I2C_SDA_LOW();	//CPU ���� SDA = 0
	Delay();
	I2C_SCL_HIGH();//CPU ����һ�� ʱ��
	Delay();
	I2C_SCL_LOW();
	Delay();
	I2C_SDA_HIGH();	//CPU�ͷ� SDA ����
}

//CPU����һ�� NACK Ӧ���ź�
//SCL�ź����л��ߵ͵�ƽ������һ��ʱ���ź�
void i2c_NAck(void){
	I2C_SDA_HIGH();	//CPU ���� SDA = 1
	Delay();
	I2C_SCL_HIGH();//CPU ����һ�� ʱ��
	Delay();
	I2C_SCL_LOW();
	Delay();
}

