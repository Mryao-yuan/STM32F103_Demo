#include "stm32f10x.h"
#include "LED.h"
#include "dma.h"
#include "usart.h"

extern uint8_t SendBuffer[ SendBuffer_Size];

void Delay(uint32_t count){
	for(;count!=0;count--);
	
}

/*
**function:
	*M TO P: SRAM to 串口,同时LED灯闪烁，演示	DMA传输数据不需要占用CPU
**/
int main(){

	USART_DMA_Config();
	Usart_Configue();
	led_init();
	
	//初始化发送数组
	for(uint16_t i=0;i<SendBuffer_Size;i++){
		
		SendBuffer[i]='p';
	}
	//usart1向 DMA 发出 TX 请求 
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
	
	//此时CPU是空闲的，可以干其他事情
	while(1){
		LED_CHANGE;
		Delay(0xFFFFF);
	
	}
}
