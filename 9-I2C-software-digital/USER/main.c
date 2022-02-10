#include "stm32f10x.h"
#include "LED.h"
#include "dma.h"
#include "usart.h"
#include "bsp_i2c_gpio.h"
#include "bsp_i2c_ee.h"


/*Function：
**			利用EEPROM存取小数
**			
**
**/

uint8_t cal_flag =0;
uint8_t k;

//地址(小数和整数分开存储)
#define	 DOUBLE_ADDR  10
#define	 LONGINT_ADDR  70
//字符串地址
#define	 CHAR_ADDR  100

/*存储小数和整数的数组，各7个*/
long double double_buffer[7] = {0};
int int_bufffer[7] = {0};
//存储字符串
char string[20]="黄森仪,你好！";
char string_out[20]="";
int main(){
	
	Usart_Configue();
	I2C_Configue();
	led_init();
	
	printf("EEPROM 读写小数和长整数实验:\n");
	
	if(ee_CheckDevice(EEPROM_DEV_ADDR)==1){
		printf("没有检测到串行EEPROM\r\n");
		return 0;
	}
	ee_ReadBytes(&cal_flag,0,1);/*读取数据标志位*/
	 if( cal_flag != 0x11 )	/*若标志等于0xcd，表示之前已有写入数据(可以通过修改此处来进行数据的写入)*/
    {      
        printf("\r\n没有检测到数据标志，FLASH没有存储数据，即将进行小数写入实验\r\n");
        cal_flag =0x11;
        
        /*写入标志到0地址*/
        ee_WriteBytes(&cal_flag, 0, 1); 
        
        /*生成要写入的数据*/
        for( k=0; k<7; k++ )
        {
           double_buffer[k] = k +0.1;
           int_bufffer[k]=k*500+1 ;
        }

        /*写入小数数据到地址10*/
        ee_WriteBytes((void*)double_buffer,DOUBLE_ADDR, sizeof(double_buffer));
        /*写入整数数据到地址60*/
        ee_WriteBytes((void*)int_bufffer, LONGINT_ADDR, sizeof(int_bufffer));
         /*写入字符串数据到地址100*/
        ee_WriteBytes((void*)string, CHAR_ADDR, sizeof(string));     
        printf("向芯片写入数据：\n");
        /*打印到串口*/
        for( k=0; k<7; k++ )
        {
          printf("小数tx = %LF\r\n",double_buffer[k]);
          printf("整数tx = %d\r\n",int_bufffer[k]);
        }
				printf("%s",string);
        //非易失性，断电可以存储
        printf("\r\n请复位开发板，以读取数据进行检验\r\n");      
		
    }    
    else
    {      
      	 printf("\r\n检测到数据标志\r\n");

				/*读取小数数据*/
        ee_ReadBytes((void*)double_buffer, DOUBLE_ADDR, sizeof(double_buffer));
				
				/*读取整数数据*/
        ee_ReadBytes((void*)int_bufffer, LONGINT_ADDR, sizeof(int_bufffer));
				/*读取字符串数据*/
        ee_ReadBytes((void*)string_out, CHAR_ADDR, sizeof(string_out));
			
				printf("\r\n从芯片读到数据：\r\n");			
        for( k=0; k<7; k++ )
				{
					printf("小数 rx = %LF \r\n",double_buffer[k]);
					printf("整数 rx = %d \r\n",int_bufffer[k]);				
				} 
					printf("%s",string_out);
    }  


	/*
			通过EEPROM来存储字符串
	*/	


		
	while(1);
}
