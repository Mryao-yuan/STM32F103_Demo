#include "stm32f10x.h"
#include "LED.h"
#include "exti.h"
#include "usart.h"

/*
	PA0连接到EXTI用于产生中断，PA0的电平变化通过按键来控制
	产生一次中断，LED反转一次
	通过串口显示灯的状态
*/
int main(){

	led_init();
	EXTI_Configue();
	usart_config();
	
	while(1){
//	printf("hello world!!");
	}
}
