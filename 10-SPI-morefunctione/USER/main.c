#include "stm32f10x.h"
#include "led.h"
#include "./USART/usart.h"
#include "./SPI/spi.h"

/*
	*@function 先测试读取FLASH的ID 验证程序的正确性
	*@		写入数据要先写入使能
	*@		页写入一次只能操作256个数据
	*/

uint8_t writebuff[]="你好,这是SPI关于FLASH的测试!\r\n";
uint8_t String[]="你好,这是SPI关于FLASH的测试!\r\n";
//#define RxBuffSize  (sizeof(writebuff)/sizeof(writebuff[0]))

#define RxBuffSize  (sizeof(String)/sizeof(String[0]))
uint8_t readbuff[RxBuffSize];
int main(){
	uint32_t ID;
	//uint8_t buff[4096];		定义在此处会出现栈溢出(硬件错误)，可以定义全局变量
	//uint16_t i;
	led_init();
	usart_config();
	FLASH_SPI_Init();
	printf("SPI 读写 FLASH 测试：\n");
	ID=SPI_Read_ID();
	if(ID == sFLASH_ID){
			printf("检测到串行FLASH W25Q64!");
		//擦除
		SPI_ERASE_SECTOR(0);
		SPI_WaitForWriteEnd();
		printf("\nFLASH擦除完成\n");
		SPI_FLASH_BufferWrite(String,0,sizeof(String));
		/* 掉电模式*/
		//SPI_FLASH_PowerDown();
		SPI_Read_Data(0,readbuff,RxBuffSize);
		//输出擦除后的数据
	//	for( i=0;i < 100; i++){
	//		printf("0x%x ",readbuff[i]);
	//		if((i%10)==0&&(i!=0))
	//			printf("\n");
	//	}
	
		//写入数据
		//SPI_Write_Data(0,writebuff,RxBuffSize);
		printf("写入数据:%s\n",String);
	
		SPI_Read_Data(0,readbuff,RxBuffSize);
		//输出写入后的数据
		printf("读取数据:%s\n",readbuff);
	}
}
