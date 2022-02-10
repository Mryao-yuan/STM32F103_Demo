#include "stm32f10x.h"
#include "LED.h"
#include "dma.h"
#include "usart.h"
#include "bsp_i2c_gpio.h"
#include "bsp_i2c_ee.h"


/*Function：
**			利用I2C读取EEPROM
**			1.初始化I2C相关的GPIO
**			2.配置I2C外设的工作模式
**			3.编写I2C写入EEPROM的Byte_write函数
**			4.编写I2C读取EEPROM的RANDOM_Read函数
**			5.使用read函数和write函数进行读写校验
**
**/
int main(){
	
	Usart_Configue();
	I2C_Configue();
	led_init();
	
	printf("EEPROM 软件模拟 i2c:\n");
	if(ee_test()){
		LED_ON;
	}
	else{
		LED_CHANGE;

	}
	while(1);
}
