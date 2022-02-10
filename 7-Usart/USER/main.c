#include "stm32f10x.h"
#include "LED.h"
#include "exti.h"
#include "usart.h"
#include "systick.h"
/*
	PA0连接到EXTI用于产生中断，PA0的电平变化通过按键来控制
	产生一次中断，LED反转一次
	通过串口显示灯的状态
	
	1.单片机---->电脑,打印
	2.单片机<---- 电脑，打印
	
	
	串口控制RGB灯:1-->LED亮
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
	printf("串口通讯实验！\n");
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
