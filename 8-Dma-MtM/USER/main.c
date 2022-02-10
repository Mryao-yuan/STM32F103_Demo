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
	1.M TO M:FLASH to SRAM,把内部的FLASH数据传输到内部的SRAM


	2.M TO P: SRAM to 串口,同时LED灯闪烁，演示	DMA传输数据不需要占用CPU
*/
int main(){

	uint8_t status=0;
	led_init();
	
	MtM_DMA_Config();
	//检测传输完成
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
