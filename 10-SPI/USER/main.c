#include "stm32f10x.h"
#include "led.h"
#include "./USART/usart.h"
#include "./SPI/spi.h"

/*
	*@function 先测试读取FLASH的ID 验证程序的正确性
	*@		写入数据要先写入使能
	*@		页写入一次只能操作256个数据
	*/
uint8_t readbuff[4096];
uint8_t writebuff[4096];

int main(){
	uint32_t ID;
	//uint8_t buff[4096];		定义在此处会出现栈溢出(硬件错误)，可以定义全局变量
	uint16_t i;
	led_init();
	usart_config();
	FLASH_SPI_Init();
	printf("SPI 读写 FLASH 测试：\n");
	ID=SPI_Read_ID();
	printf("Flash ID =0x%x\n",ID);
	SPI_ERASE_SECTOR(0);
	SPI_WaitForWriteEnd();
	printf("FLASH擦除完成\n");
	
	
	
	SPI_Read_Data(0,readbuff,4096);
	//输出擦除后的数据
	for( i=0;i < 100; i++){
		printf("0x%x ",readbuff[i]);
		if((i%10)==0&&(i!=0))
			printf("\n");
	}
	
	//写入数据赋初值
	for( i=0;i < 4096; i++){
		writebuff[i]=i;
	}
	//写入数据
	SPI_Write_Data(0,writebuff,4096);
	printf("写入数据\n");
	SPI_Read_Data(0,readbuff,4096);
	//输出写入后的数据
	printf("写入的数据：\n");
	for( i=0;i < 1000; i++){
		printf("0x%x ",readbuff[i]);
		if((i%10)==0&&(i!=0))
			printf("\n");
	}
}
