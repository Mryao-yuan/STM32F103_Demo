#include "stm32f10x.h"
#include "LED.h"
#include "dma.h"
#include "usart.h"
#include "bsp_i2c_gpio.h"
#include "bsp_i2c_ee.h"


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
	
	Usart_Configue();
	I2C_Configue();
	led_init();
	
	printf("EEPROM ���ģ�� i2c:\n");
	if(ee_test()){
		LED_ON;
	}
	else{
		LED_CHANGE;

	}
	while(1);
}
