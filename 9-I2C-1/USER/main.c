#include "stm32f10x.h"
#include "LED.h"
#include "dma.h"
#include "i2c.h"
#include "usart.h"

extern uint8_t SendBuffer[ SendBuffer_Size];

void Delay(uint32_t count){
	for(;count!=0;count--);
	
}

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

	uint8_t i=0;
	uint8_t data[10],writeDate[10];
	Usart_Configue();
	I2c_Configue();
	printf("这个是I2C通讯实验！\n");
	//写数据
	EEPROM_Byte_Write(11,0xbb);
	//等待写入操作完成
	EEPROM_WaitForEnd();
	//读取数据
	EEPROM_Byte_Read(11,data,1);
	
	printf("读取到的数据为%d\n",data[0]);
//	EEPROM_Page_Write(11,14,6);
	//addr%8==0 即地址对齐
	//EEPROM_Page_Write(8,writeDate,8);
	while(1){
		i++;
	printf("接收到的数据为0x%x\n",data[0]);
		if(i==10)
			break;
	}
}
