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
	*M TO P: SRAM to ����,ͬʱLED����˸����ʾ	DMA�������ݲ���Ҫռ��CPU
**/
int main(){

	USART_DMA_Config();
	Usart_Configue();
	led_init();
	
	//��ʼ����������
	for(uint16_t i=0;i<SendBuffer_Size;i++){
		
		SendBuffer[i]='p';
	}
	//usart1�� DMA ���� TX ���� 
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
	
	//��ʱCPU�ǿ��еģ����Ը���������
	while(1){
		LED_CHANGE;
		Delay(0xFFFFF);
	
	}
}
