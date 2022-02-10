#include "stm32f10x.h"
#include "led.h"
#include "./USART/usart.h"
#include "./SPI/spi.h"

/*
	*@function �Ȳ��Զ�ȡFLASH��ID ��֤�������ȷ��
	*@		д������Ҫ��д��ʹ��
	*@		ҳд��һ��ֻ�ܲ���256������
	*/
uint8_t readbuff[4096];
uint8_t writebuff[4096];

int main(){
	uint32_t ID;
	//uint8_t buff[4096];		�����ڴ˴������ջ���(Ӳ������)�����Զ���ȫ�ֱ���
	uint16_t i;
	led_init();
	usart_config();
	FLASH_SPI_Init();
	printf("SPI ��д FLASH ���ԣ�\n");
	ID=SPI_Read_ID();
	printf("Flash ID =0x%x\n",ID);
	SPI_ERASE_SECTOR(0);
	SPI_WaitForWriteEnd();
	printf("FLASH�������\n");
	
	
	
	SPI_Read_Data(0,readbuff,4096);
	//��������������
	for( i=0;i < 100; i++){
		printf("0x%x ",readbuff[i]);
		if((i%10)==0&&(i!=0))
			printf("\n");
	}
	
	//д�����ݸ���ֵ
	for( i=0;i < 4096; i++){
		writebuff[i]=i;
	}
	//д������
	SPI_Write_Data(0,writebuff,4096);
	printf("д������\n");
	SPI_Read_Data(0,readbuff,4096);
	//���д��������
	printf("д������ݣ�\n");
	for( i=0;i < 1000; i++){
		printf("0x%x ",readbuff[i]);
		if((i%10)==0&&(i!=0))
			printf("\n");
	}
}
