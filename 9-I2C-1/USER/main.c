#include "stm32f10x.h"
#include "LED.h"
#include "dma.h"
#include "i2c.h"
#include "usart.h"

extern uint8_t SendBuffer[ SendBuffer_Size];

void Delay(uint32_t count){
	for(;count!=0;count--);
	
}

/*Function��
**			����I2C��ȡEEPROM
**			1.��ʼ��I2C��ص�GPIO
**			2.����I2C����Ĺ���ģʽ
**			3.��дI2Cд��EEPROM��Byte_write����
**			4.��дI2C��ȡEEPROM��RANDOM_Read����
**			5.ʹ��read������write�������ж�дУ��
**	
**/
int main(){

	uint8_t i=0;
	uint8_t data[10],writeDate[10];
	Usart_Configue();
	I2c_Configue();
	printf("�����I2CͨѶʵ�飡\n");
	//д����
	EEPROM_Byte_Write(11,0xbb);
	//�ȴ�д��������
	EEPROM_WaitForEnd();
	//��ȡ����
	EEPROM_Byte_Read(11,data,1);
	
	printf("��ȡ��������Ϊ%d\n",data[0]);
//	EEPROM_Page_Write(11,14,6);
	//addr%8==0 ����ַ����
	//EEPROM_Page_Write(8,writeDate,8);
	while(1){
		i++;
	printf("���յ�������Ϊ0x%x\n",data[0]);
		if(i==10)
			break;
	}
}
