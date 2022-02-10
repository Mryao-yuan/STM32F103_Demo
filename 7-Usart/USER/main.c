#include "stm32f10x.h"
#include "LED.h"
#include "exti.h"
#include "usart.h"
#include "systick.h"
/*
	PA0���ӵ�EXTI���ڲ����жϣ�PA0�ĵ�ƽ�仯ͨ������������
	����һ���жϣ�LED��תһ��
	ͨ��������ʾ�Ƶ�״̬
	
	1.��Ƭ��---->����,��ӡ
	2.��Ƭ��<---- ���ԣ���ӡ
	
	
	���ڿ���RGB��:1-->LED��
*/
int main(){
	
	uint8_t Array[]={1,2,3,4,5,6,7,8,9},ch;
	led_init();
	usart_config();
	//EXTI_Configue();
	//Usart_SendByte(DEBUG_USARTx,100);
	//Usart_SendHalfWord(DEBUG_USARTx,0xff11);
	//Usart_SendArray(DEBUG_USARTx,Array ,9);
	//Usart_SendString(DEBUG_USARTx,"Hello World!\n");
	//	putchar('A');
	printf("����ͨѶʵ�飡\n");
	while(1){
		ch=getchar();
		switch(ch){
			case '1': 	GPIO_ResetBits(LED_PORT, LED); 
				break;
			case '2':	printf("ERROR\n");
				break;		
		}
	}
}
