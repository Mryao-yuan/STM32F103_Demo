#include "stm32f10x.h"
#include "LED.h"
#include "dma.h"


extern const uint32_t aSRC_Const_Buffer[BUFFER_SIZE];
extern const uint32_t aDST_Buffer[BUFFER_SIZE];
void Delay(uint32_t count){
	for(;count!=0;count--);
	
}

/*
function:
	1.M TO M:FLASH to SRAM,���ڲ���FLASH���ݴ��䵽�ڲ���SRAM


	2.M TO P: SRAM to ����,ͬʱLED����˸����ʾ	DMA�������ݲ���Ҫռ��CPU
*/
int main(){

	uint8_t status=0;
	led_init();
	
	MtM_DMA_Config();
	//��⴫�����
	while(DMA_GetFlagStatus(DMA1_FLAG_TC6)== RESET);
	
	status = BufferCompare(aSRC_Const_Buffer,aDST_Buffer,BUFFER_SIZE);
	if(status==0)
	{
		LED_ON;
	}
	else{
		LED_OFF;
		
	}
	while(1){
	
	}
}
