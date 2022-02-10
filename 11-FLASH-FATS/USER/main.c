#include "stm32f10x.h"
#include "led.h"
#include "./USART/usart.h"
#include "./SPI/spi.h"
#include "./FATFS/diskio.h"
#include "./FATFS/ff.h"


/*
	*@function FatFs�ļ���ֲ
	*@		��ʽ������
	*@		�ļ�д�����
	*@		�ļ���ȡ����
	*/
/* FatFs �ļ�ϵͳ���� */
FATFS fs;
/*  �ļ����� */
FIL fnew;
/*  �ļ�������� */
FRESULT res_flash;
/*  �ļ��ɹ���д���� */
UINT fnum;
/*  д������ */
BYTE WriteBuffer[]="Hello World!\r\n"; 
/*  �������� */
char ReadBuffer[1024]={0};


int main(){

	//uint8_t buff[4096];		�����ڴ˴������ջ���(Ӳ������)�����Զ���ȫ�ֱ���
	/*  ����ģ���ʼ������ */
	led_init();
	usart_config();
	printf("***** SPI  FLASH �ļ�ϵͳʵ�� ******\r\n");
	
	//���ⲿSPI Flash �����ļ�ϵͳ���ļ�ϵͳ����ʱ��� SPI �豸���г�ʼ��
	// ��ʼ������������������
	//	f_mount()->find_volume()->disk_initialize()->SPI_FLASH_Init()
	//1��(��ʾ���ص�һ���洢�豸:flash)
	res_flash = f_mount(&fs,"1:",1);

	/*   =================== ��ʽ������ ========================= */
	//���û���ļ�ϵͳ�͸�ʽ�������ļ�ϵͳ
//	if( res_flash == FR_NO_FILESYSTEM){
//		printf(">>FLASH ��û���ļ�ϵͳ,�������и�ʽ��...\r\n");
//		//��ʽ��
//		res_flash=f_mkfs("1:",0,0);
//		if(res_flash == FR_OK){
//			printf(">>FLASH �Ѿ��ɹ���ʽ���ļ�ϵͳ\r\n");
//			
//			//��ʽ����,��ȡ������
//			res_flash = f_mount(NULL,"1:",1);
//			printf(">>��ʽ����,��ȡ������=%d\r\n",res_flash);	
//			//���¹���
//			res_flash = f_mount(&fs,"1:",1);
//				printf(">>���¹���=%d\r\n",res_flash);	
//		}
//		//��ʽ��ʧ�ܣ���������ѭ��
//		else{
//			printf("ERROR : ��ʽ��ʧ�ܣ�����\r\n");
//			LED_ON;
//			while(1);
//		}
//	}
//	else if( res_flash != FR_OK){
//		printf("�������ⲿ Flash �����ļ�ϵͳʧ�ܡ�(%d)\r\n",res_flash);
//		printf("����������ԭ�� : SPI Flash��ʼ�����ɹ�\r\n");
//	}
//	else


if(res_flash ==FR_OK)
  {
    printf("���ļ�ϵͳ���سɹ������Խ��ж�д����\r\n");
  }
  
/*----------------------- �ļ�ϵͳ���ԣ�д���� -------------------*/
	/* ���ļ���ÿ�ζ����½�����ʽ�򿪣�����Ϊ��д */
	printf("\r\n****** ���������ļ�д�����... ******\r\n");	
	res_flash = f_open(&fnew, "1:FatFs.txt",FA_CREATE_ALWAYS | FA_WRITE );
	fnum=0;
	if ( res_flash == FR_OK )
	{
		printf("����/����FatFs.txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
    /* ��ָ���洢������д�뵽�ļ��� */
		res_flash=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
    if(res_flash==FR_OK)
    {
      printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n",fnum);
      printf("�����ļ�д�������Ϊ��\r\n%s\r\n",WriteBuffer);
    }
    else
    {
      printf("�����ļ�д��ʧ�ܣ�(%d)\n",res_flash);
    }    
		/* ���ٶ�д���ر��ļ� */
    f_close(&fnew);
	}
	else
	{	
		LED_ON; printf("�����ļ�д��ʧ�ܣ�(%d)\n",res_flash);
		printf("������/�����ļ�ʧ�ܡ�\r\n");
	}
	
/*------------------- �ļ�ϵͳ���ԣ������� --------------------------*/
	printf("****** ���������ļ���ȡ����... ******\r\n");
	res_flash = f_open(&fnew, "1:FatFs.txt",FA_OPEN_EXISTING|FA_READ); 	 
	if(res_flash == FR_OK)
	{
		LED_OFF;
		printf("�����ļ��ɹ���\r\n");
		res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
    if(res_flash==FR_OK)
    {
      printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n",fnum);
      printf("����ȡ���ļ�����Ϊ��\r\n %s \r\n", ReadBuffer);
				
    }
    else
    {
      printf("�����ļ���ȡʧ�ܣ�(%d)\n",res_flash);
    }		
	}
	else
	{
		LED_ON;
		printf("�������ļ�ʧ�ܡ�\r\n");
	}
	/* ���ٶ�д���ر��ļ� */
	f_close(&fnew);	
  
	/* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
	f_mount(NULL,"1:",1);
  
  /* ������ɣ�ͣ�� */
	while(1)
	{
	}
}
