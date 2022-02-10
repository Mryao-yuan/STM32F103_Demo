#include "stm32f10x.h"
#include "led.h"
#include "./USART/usart.h"
#include "./SPI/spi.h"

/*
	*@function �Ȳ��Զ�ȡFLASH��ID ��֤�������ȷ��
	*@		д������Ҫ��д��ʹ��
	*@		ҳд��һ��ֻ�ܲ���256������
	*/

uint8_t writebuff[]="���,����SPI����FLASH�Ĳ���!\r\n";
uint8_t String[]="���,����SPI����FLASH�Ĳ���!\r\n";
//#define RxBuffSize  (sizeof(writebuff)/sizeof(writebuff[0]))

#define RxBuffSize  (sizeof(String)/sizeof(String[0]))
uint8_t readbuff[RxBuffSize];
int main(){
	uint32_t ID;
	//uint8_t buff[4096];		�����ڴ˴������ջ���(Ӳ������)�����Զ���ȫ�ֱ���
	//uint16_t i;
	led_init();
	usart_config();
	FLASH_SPI_Init();
	printf("SPI ��д FLASH ���ԣ�\n");
	ID=SPI_Read_ID();
	if(ID == sFLASH_ID){
			printf("��⵽����FLASH W25Q64!");
		//����
		SPI_ERASE_SECTOR(0);
		SPI_WaitForWriteEnd();
		printf("\nFLASH�������\n");
		SPI_FLASH_BufferWrite(String,0,sizeof(String));
		/* ����ģʽ*/
		//SPI_FLASH_PowerDown();
		SPI_Read_Data(0,readbuff,RxBuffSize);
		//��������������
	//	for( i=0;i < 100; i++){
	//		printf("0x%x ",readbuff[i]);
	//		if((i%10)==0&&(i!=0))
	//			printf("\n");
	//	}
	
		//д������
		//SPI_Write_Data(0,writebuff,RxBuffSize);
		printf("д������:%s\n",String);
	
		SPI_Read_Data(0,readbuff,RxBuffSize);
		//���д��������
		printf("��ȡ����:%s\n",readbuff);
	}
}
