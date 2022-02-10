#include "stm32f10x.h"
#include "LED.h"
#include "dma.h"
#include "usart.h"
#include "bsp_i2c_gpio.h"
#include "bsp_i2c_ee.h"


/*Function��
**			����EEPROM��ȡС��
**			
**
**/

uint8_t cal_flag =0;
uint8_t k;

//��ַ(С���������ֿ��洢)
#define	 DOUBLE_ADDR  10
#define	 LONGINT_ADDR  70
//�ַ�����ַ
#define	 CHAR_ADDR  100

/*�洢С�������������飬��7��*/
long double double_buffer[7] = {0};
int int_bufffer[7] = {0};
//�洢�ַ���
char string[20]="��ɭ��,��ã�";
char string_out[20]="";
int main(){
	
	Usart_Configue();
	I2C_Configue();
	led_init();
	
	printf("EEPROM ��дС���ͳ�����ʵ��:\n");
	
	if(ee_CheckDevice(EEPROM_DEV_ADDR)==1){
		printf("û�м�⵽����EEPROM\r\n");
		return 0;
	}
	ee_ReadBytes(&cal_flag,0,1);/*��ȡ���ݱ�־λ*/
	 if( cal_flag != 0x11 )	/*����־����0xcd����ʾ֮ǰ����д������(����ͨ���޸Ĵ˴����������ݵ�д��)*/
    {      
        printf("\r\nû�м�⵽���ݱ�־��FLASHû�д洢���ݣ���������С��д��ʵ��\r\n");
        cal_flag =0x11;
        
        /*д���־��0��ַ*/
        ee_WriteBytes(&cal_flag, 0, 1); 
        
        /*����Ҫд�������*/
        for( k=0; k<7; k++ )
        {
           double_buffer[k] = k +0.1;
           int_bufffer[k]=k*500+1 ;
        }

        /*д��С�����ݵ���ַ10*/
        ee_WriteBytes((void*)double_buffer,DOUBLE_ADDR, sizeof(double_buffer));
        /*д���������ݵ���ַ60*/
        ee_WriteBytes((void*)int_bufffer, LONGINT_ADDR, sizeof(int_bufffer));
         /*д���ַ������ݵ���ַ100*/
        ee_WriteBytes((void*)string, CHAR_ADDR, sizeof(string));     
        printf("��оƬд�����ݣ�\n");
        /*��ӡ������*/
        for( k=0; k<7; k++ )
        {
          printf("С��tx = %LF\r\n",double_buffer[k]);
          printf("����tx = %d\r\n",int_bufffer[k]);
        }
				printf("%s",string);
        //����ʧ�ԣ��ϵ���Դ洢
        printf("\r\n�븴λ�����壬�Զ�ȡ���ݽ��м���\r\n");      
		
    }    
    else
    {      
      	 printf("\r\n��⵽���ݱ�־\r\n");

				/*��ȡС������*/
        ee_ReadBytes((void*)double_buffer, DOUBLE_ADDR, sizeof(double_buffer));
				
				/*��ȡ��������*/
        ee_ReadBytes((void*)int_bufffer, LONGINT_ADDR, sizeof(int_bufffer));
				/*��ȡ�ַ�������*/
        ee_ReadBytes((void*)string_out, CHAR_ADDR, sizeof(string_out));
			
				printf("\r\n��оƬ�������ݣ�\r\n");			
        for( k=0; k<7; k++ )
				{
					printf("С�� rx = %LF \r\n",double_buffer[k]);
					printf("���� rx = %d \r\n",int_bufffer[k]);				
				} 
					printf("%s",string_out);
    }  


	/*
			ͨ��EEPROM���洢�ַ���
	*/	


		
	while(1);
}
