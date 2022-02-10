#include "stm32f10x.h"
#include "led.h"
#include "./USART/usart.h"
#include "./SPI/spi.h"
#include "./FATFS/diskio.h"
#include "./FATFS/ff.h"


/*
	*@function FatFs文件功能测试
	*@		扇区偏移(前2M,留给非文件系统,6M留给文件系统)
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
UINT br;	//读返回
///*  读缓冲区 */
BYTE ReadBuffer[1024]={0};
///*  写缓冲区 */
BYTE WriteBuffer[]="New File System Test Based of FatFs\r\n";


/* FatFs 多项功能测试 */
static FRESULT miscellaneous(void){
	DIR dir;
	FATFS *pfs;
	char dir_name[30]="1:TestDir";
	char file_name[30]="1:TestDir/testdir.txt";
	DWORD fre_clust,fre_sect,tot_sect;
	
	printf("\n***** 设备信息获取 *****\r\n");

	
	
	/* 获取设备信息和空簇大小 */
	res_flash = f_getfree("1:",&fre_clust,&pfs);
	
	/* 计算得到的总扇区个数和空扇区个数*/
	
	tot_sect = (pfs->n_fatent -2) * pfs->csize;
	fre_sect = fre_clust * pfs->csize;
	
	/* 打印信息(4096 字节/扇区) */
	printf("》设备总空间: %10lu KB。\n》可用空间:%10lu KB。\n",
					tot_sect * 4,fre_sect * 4);
	
	
	printf("\n***** 文件定位和格式化写入功能测试 *****\r\n");
	
	res_flash = f_open(&fnew,"1:FatFs.txt",
											FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
	if(res_flash == FR_OK){
		/* 文件定位，定位到文件的末尾 */
		res_flash = f_lseek(&fnew,f_size(&fnew));
		if(res_flash == FR_OK){
			/* 格式化写入*/
			f_printf(&fnew,"\nthis has add a new line content\n");
			f_printf(&fnew,"》Device Total room:%10luKB\n》Available Room:%10luKB\n",
							tot_sect * 4,fre_sect *4);
			/* 定位文件到起始位置 */
			res_flash = f_lseek(&fnew,0);
			
			/* 读取文件所有内容到缓冲区 */
			res_flash = f_read(&fnew,ReadBuffer,f_size(&fnew),&fnum);
			printf("has finished write by f_printf\n");
			
			if( res_flash == FR_OK)
				printf("》File content :%s\n",ReadBuffer);
		}
		f_close(&fnew);
		
		printf("\n***** 目录创建和重命名功能测试 *****\r\n");
		
		/* 尝试打开目录 */
		res_flash = f_opendir(&dir,dir_name);
		if( res_flash != FR_OK)
			/* 打开目录失败，创建目录 */	
			res_flash = f_mkdir(dir_name);
		else
		{
			/* 如果目录已经存在,关闭它 */
			res_flash = f_closedir(&dir);
			f_unlink(file_name);
		}
		if( res_flash == FR_OK){
			
			/* 重命名并移动文件 */
			res_flash = f_rename("1:FatFs.txt",
											file_name);
			if( res_flash == FR_OK)
					printf(">> Rename And Move File Success！\n");
			else
				printf(">> Rename And Move File Defeat！\n");
			printf(">>res = %d\n",res_flash);
			
		}
	}
	else{
		
		printf("open file error :%d",res_flash);
		
	}
	
	
	return res_flash;
}







/* 通过 FatFs 文件系统写入数据 */
void write_file(){
	
	printf("\r\n****** 即将进行文件写入测试... ******\r\n");	
	res_flash = f_open(&fnew, "1:FatFs.txt",FA_CREATE_ALWAYS | FA_WRITE );
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
		printf("res_flash=%d",res_flash);
		LED_ON;
		printf("！！打开/创建文件失败。\r\n");
	}

}



int main(){
	
	//char wData[]="使用 FatFs文件系统挂载写入的数据!\r\n";
	//uint8_t buff[4096];		定义在此处会出现栈溢出(硬件错误)，可以定义全局变量
	/*  各个模块初始化配置 */
	led_init();
	usart_config();
	/* 在挂载的时候会调用该函数，进行硬件初始化*/
	//SPI_FLASH_Init();
	printf("***** SPI  FLASH 文件系统实验 *****\r\n");
	
	//在外部SPI Flash 挂载文件系统，文件系统挂载时会对 SPI 设备进行初始化
	// 初始化函数调用流程如下
	//	f_mount()->find_volume()->disk_initialize()->SPI_FLASH_Init()
	//1：(表示挂载第一个存储设备:flash)
	res_flash = f_mount(&fs,"1:",1);
	

	/*   =================== 格式化测试 ========================= */
	//如果没有文件系统就格式化创建文件系统
	if( res_flash == FR_NO_FILESYSTEM){
		printf(">>FLASH 还没有文件系统,即将进行格式化...\r\n");
		//格式化
		res_flash=f_mkfs("1:",0,0);
		if(res_flash == FR_OK){
			printf(">>FLASH 已经成功格式化文件系统\r\n");
			
			//格式化后,先取消挂载
			res_flash = f_mount(NULL,"1:",1);
			
			//重新挂载
			res_flash = f_mount(&fs,"1:",1);
		}
		//格式化失败，灯亮，死循环
		else{
			printf("ERROR : 格式化失败！！！\r\n");
			LED_ON;
			while(1);
		}
	}
	else if( res_flash != FR_OK){
		printf("！！！外部 Flash 挂载文件系统失败。(%d)\r\n",res_flash);
		printf("！！！可能原因 : SPI Flash初始化不成功\r\n");
	}
	else
  {
    printf("》文件系统挂载成功，可以进行读写测试\r\n");
  }
  
/*----------------------- 文件系统测试：写测试 -------------------*/	
	/* 打开文件，每次都以新建的形式打开，属性为可写 */
	
	write_file();
	
/*----------------------- 文件系统测试：多项功能测试 -------------------*/		
	miscellaneous();
/*------------------- 文件系统测试：读测试 --------------------------*/
	printf("****** 即将进行文件读取测试... ******\r\n");
	res_flash = f_open(&fnew, "1:FatFs.txt", FA_OPEN_EXISTING | FA_READ);
	
	if(res_flash == FR_OK)
	{
		LED_OFF;
		printf("》打开文件成功。\r\n");	
		res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
    if(res_flash==FR_OK)
    {
      printf("》文件读取成功,读到字节数据：%d\r\n",fnum);
      printf("》读取得的文件数据为：\r\n%s\r\n", ReadBuffer);	
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
		printf("！！打开文件失败。(%d)\n",res_flash);
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

