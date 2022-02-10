#include "stm32f10x.h"
#include "led.h"
#include "./USART/usart.h"
#include "./SPI/spi.h"
#include "./FATFS/diskio.h"
#include "./FATFS/ff.h"


/*
	*@function FatFs�ļ����ܲ���
	*@		����ƫ��(ǰ2M,�������ļ�ϵͳ,6M�����ļ�ϵͳ)
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
UINT br;	//������
///*  �������� */
BYTE ReadBuffer[1024]={0};
///*  д������ */
BYTE WriteBuffer[]="New File System Test Based of FatFs\r\n";


/* FatFs ����ܲ��� */
static FRESULT miscellaneous(void){
	DIR dir;
	FATFS *pfs;
	char dir_name[30]="1:TestDir";
	char file_name[30]="1:TestDir/testdir.txt";
	DWORD fre_clust,fre_sect,tot_sect;
	
	printf("\n***** �豸��Ϣ��ȡ *****\r\n");

	
	
	/* ��ȡ�豸��Ϣ�Ϳմش�С */
	res_flash = f_getfree("1:",&fre_clust,&pfs);
	
	/* ����õ��������������Ϳ���������*/
	
	tot_sect = (pfs->n_fatent -2) * pfs->csize;
	fre_sect = fre_clust * pfs->csize;
	
	/* ��ӡ��Ϣ(4096 �ֽ�/����) */
	printf("���豸�ܿռ�: %10lu KB��\n�����ÿռ�:%10lu KB��\n",
					tot_sect * 4,fre_sect * 4);
	
	
	printf("\n***** �ļ���λ�͸�ʽ��д�빦�ܲ��� *****\r\n");
	
	res_flash = f_open(&fnew,"1:FatFs.txt",
											FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
	if(res_flash == FR_OK){
		/* �ļ���λ����λ���ļ���ĩβ */
		res_flash = f_lseek(&fnew,f_size(&fnew));
		if(res_flash == FR_OK){
			/* ��ʽ��д��*/
			f_printf(&fnew,"\nthis has add a new line content\n");
			f_printf(&fnew,"��Device Total room:%10luKB\n��Available Room:%10luKB\n",
							tot_sect * 4,fre_sect *4);
			/* ��λ�ļ�����ʼλ�� */
			res_flash = f_lseek(&fnew,0);
			
			/* ��ȡ�ļ��������ݵ������� */
			res_flash = f_read(&fnew,ReadBuffer,f_size(&fnew),&fnum);
			printf("has finished write by f_printf\n");
			
			if( res_flash == FR_OK)
				printf("��File content :%s\n",ReadBuffer);
		}
		f_close(&fnew);
		
		printf("\n***** Ŀ¼���������������ܲ��� *****\r\n");
		
		/* ���Դ�Ŀ¼ */
		res_flash = f_opendir(&dir,dir_name);
		if( res_flash != FR_OK)
			/* ��Ŀ¼ʧ�ܣ�����Ŀ¼ */	
			res_flash = f_mkdir(dir_name);
		else
		{
			/* ���Ŀ¼�Ѿ�����,�ر��� */
			res_flash = f_closedir(&dir);
			f_unlink(file_name);
		}
		if( res_flash == FR_OK){
			
			/* ���������ƶ��ļ� */
			res_flash = f_rename("1:FatFs.txt",
											file_name);
			if( res_flash == FR_OK)
					printf(">> Rename And Move File Success��\n");
			else
				printf(">> Rename And Move File Defeat��\n");
			printf(">>res = %d\n",res_flash);
			
		}
	}
	else{
		
		printf("open file error :%d",res_flash);
		
	}
	
	
	return res_flash;
}







/* ͨ�� FatFs �ļ�ϵͳд������ */
void write_file(){
	
	printf("\r\n****** ���������ļ�д�����... ******\r\n");	
	res_flash = f_open(&fnew, "1:FatFs.txt",FA_CREATE_ALWAYS | FA_WRITE );
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
		printf("res_flash=%d",res_flash);
		LED_ON;
		printf("������/�����ļ�ʧ�ܡ�\r\n");
	}

}



int main(){
	
	//char wData[]="ʹ�� FatFs�ļ�ϵͳ����д�������!\r\n";
	//uint8_t buff[4096];		�����ڴ˴������ջ���(Ӳ������)�����Զ���ȫ�ֱ���
	/*  ����ģ���ʼ������ */
	led_init();
	usart_config();
	/* �ڹ��ص�ʱ�����øú���������Ӳ����ʼ��*/
	//SPI_FLASH_Init();
	printf("***** SPI  FLASH �ļ�ϵͳʵ�� *****\r\n");
	
	//���ⲿSPI Flash �����ļ�ϵͳ���ļ�ϵͳ����ʱ��� SPI �豸���г�ʼ��
	// ��ʼ������������������
	//	f_mount()->find_volume()->disk_initialize()->SPI_FLASH_Init()
	//1��(��ʾ���ص�һ���洢�豸:flash)
	res_flash = f_mount(&fs,"1:",1);
	

	/*   =================== ��ʽ������ ========================= */
	//���û���ļ�ϵͳ�͸�ʽ�������ļ�ϵͳ
	if( res_flash == FR_NO_FILESYSTEM){
		printf(">>FLASH ��û���ļ�ϵͳ,�������и�ʽ��...\r\n");
		//��ʽ��
		res_flash=f_mkfs("1:",0,0);
		if(res_flash == FR_OK){
			printf(">>FLASH �Ѿ��ɹ���ʽ���ļ�ϵͳ\r\n");
			
			//��ʽ����,��ȡ������
			res_flash = f_mount(NULL,"1:",1);
			
			//���¹���
			res_flash = f_mount(&fs,"1:",1);
		}
		//��ʽ��ʧ�ܣ���������ѭ��
		else{
			printf("ERROR : ��ʽ��ʧ�ܣ�����\r\n");
			LED_ON;
			while(1);
		}
	}
	else if( res_flash != FR_OK){
		printf("�������ⲿ Flash �����ļ�ϵͳʧ�ܡ�(%d)\r\n",res_flash);
		printf("����������ԭ�� : SPI Flash��ʼ�����ɹ�\r\n");
	}
	else
  {
    printf("���ļ�ϵͳ���سɹ������Խ��ж�д����\r\n");
  }
  
/*----------------------- �ļ�ϵͳ���ԣ�д���� -------------------*/	
	/* ���ļ���ÿ�ζ����½�����ʽ�򿪣�����Ϊ��д */
	
	write_file();
	
/*----------------------- �ļ�ϵͳ���ԣ�����ܲ��� -------------------*/		
	miscellaneous();
/*------------------- �ļ�ϵͳ���ԣ������� --------------------------*/
	printf("****** ���������ļ���ȡ����... ******\r\n");
	res_flash = f_open(&fnew, "1:FatFs.txt", FA_OPEN_EXISTING | FA_READ);
	
	if(res_flash == FR_OK)
	{
		LED_OFF;
		printf("�����ļ��ɹ���\r\n");	
		res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
    if(res_flash==FR_OK)
    {
      printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n",fnum);
      printf("����ȡ�õ��ļ�����Ϊ��\r\n%s\r\n", ReadBuffer);	
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
		printf("�������ļ�ʧ�ܡ�(%d)\n",res_flash);
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

