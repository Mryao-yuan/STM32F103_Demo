#include "stm32f10x.h"
#include "led.h"
#include "./USART/usart.h"
#include "./SPI/spi.h"
#include "./FATFS/diskio.h"
#include "./FATFS/ff.h"


/*
	*@function FatFs文件移植
	*@		格式化测试
	*@		文件写入测试
	*@		文件读取测试
	*/
/* FatFs 文件系统对象 */
FATFS fs;
/*  文件对象 */
FIL fnew;
/*  文件操作结果 */
FRESULT res_flash;
/*  文件成功读写数量 */
UINT fnum;
/*  写缓冲区 */
BYTE WriteBuffer[]="Hello World!\r\n"; 
/*  读缓冲区 */
char ReadBuffer[1024]={0};


int main(){

	//uint8_t buff[4096];		定义在此处会出现栈溢出(硬件错误)，可以定义全局变量
	/*  各个模块初始化配置 */
	led_init();
	usart_config();
	printf("***** SPI  FLASH 文件系统实验 ******\r\n");
	
	//在外部SPI Flash 挂载文件系统，文件系统挂载时会对 SPI 设备进行初始化
	// 初始化函数调用流程如下
	//	f_mount()->find_volume()->disk_initialize()->SPI_FLASH_Init()
	//1：(表示挂载第一个存储设备:flash)
	res_flash = f_mount(&fs,"1:",1);

	/*   =================== 格式化测试 ========================= */
	//如果没有文件系统就格式化创建文件系统
//	if( res_flash == FR_NO_FILESYSTEM){
//		printf(">>FLASH 还没有文件系统,即将进行格式化...\r\n");
//		//格式化
//		res_flash=f_mkfs("1:",0,0);
//		if(res_flash == FR_OK){
//			printf(">>FLASH 已经成功格式化文件系统\r\n");
//			
//			//格式化后,先取消挂载
//			res_flash = f_mount(NULL,"1:",1);
//			printf(">>格式化后,先取消挂载=%d\r\n",res_flash);	
//			//重新挂载
//			res_flash = f_mount(&fs,"1:",1);
//				printf(">>重新挂载=%d\r\n",res_flash);	
//		}
//		//格式化失败，灯亮，死循环
//		else{
//			printf("ERROR : 格式化失败！！！\r\n");
//			LED_ON;
//			while(1);
//		}
//	}
//	else if( res_flash != FR_OK){
//		printf("！！！外部 Flash 挂载文件系统失败。(%d)\r\n",res_flash);
//		printf("！！！可能原因 : SPI Flash初始化不成功\r\n");
//	}
//	else


if(res_flash ==FR_OK)
  {
    printf("》文件系统挂载成功，可以进行读写测试\r\n");
  }
  
/*----------------------- 文件系统测试：写测试 -------------------*/
	/* 打开文件，每次都以新建的形式打开，属性为可写 */
	printf("\r\n****** 即将进行文件写入测试... ******\r\n");	
	res_flash = f_open(&fnew, "1:FatFs.txt",FA_CREATE_ALWAYS | FA_WRITE );
	fnum=0;
	if ( res_flash == FR_OK )
	{
		printf("》打开/创建FatFs.txt文件成功，向文件写入数据。\r\n");
    /* 将指定存储区内容写入到文件内 */
		res_flash=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
    if(res_flash==FR_OK)
    {
      printf("》文件写入成功，写入字节数据：%d\n",fnum);
      printf("》向文件写入的数据为：\r\n%s\r\n",WriteBuffer);
    }
    else
    {
      printf("！！文件写入失败：(%d)\n",res_flash);
    }    
		/* 不再读写，关闭文件 */
    f_close(&fnew);
	}
	else
	{	
		LED_ON; printf("！！文件写入失败：(%d)\n",res_flash);
		printf("！！打开/创建文件失败。\r\n");
	}
	
/*------------------- 文件系统测试：读测试 --------------------------*/
	printf("****** 即将进行文件读取测试... ******\r\n");
	res_flash = f_open(&fnew, "1:FatFs.txt",FA_OPEN_EXISTING|FA_READ); 	 
	if(res_flash == FR_OK)
	{
		LED_OFF;
		printf("》打开文件成功。\r\n");
		res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
    if(res_flash==FR_OK)
    {
      printf("》文件读取成功,读到字节数据：%d\r\n",fnum);
      printf("》读取的文件数据为：\r\n %s \r\n", ReadBuffer);
				
    }
    else
    {
      printf("！！文件读取失败：(%d)\n",res_flash);
    }		
	}
	else
	{
		LED_ON;
		printf("！！打开文件失败。\r\n");
	}
	/* 不再读写，关闭文件 */
	f_close(&fnew);	
  
	/* 不再使用文件系统，取消挂载文件系统 */
	f_mount(NULL,"1:",1);
  
  /* 操作完成，停机 */
	while(1)
	{
	}
}
