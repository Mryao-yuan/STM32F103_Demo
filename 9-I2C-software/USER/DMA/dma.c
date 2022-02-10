#include "dma.h"


uint8_t SendBuffer[ SendBuffer_Size];

void USART_DMA_Config(void){


	DMA_InitTypeDef  DMA_InitStructure;
	//开DMA1时钟
	RCC_AHBPeriphClockCmd(USART_DMA_CLK,ENABLE);
	
	//初始化DMA结构体
	//源数据地址
	 DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t)USART_DR_ADDRESS;
	//存储器地址
	 DMA_InitStructure.DMA_MemoryBaseAddr=(uint32_t)SendBuffer;
	//传输方向
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralDST;
	
	//传输数目
	DMA_InitStructure.DMA_BufferSize=SendBuffer_Size ;
	//外设地址增量模式
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	//外设数据宽度(8位=1字节）
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	//存储器增量模式
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
	//存储器数据宽度
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	
	//模式
	DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
	//通道优先级
	DMA_InitStructure.DMA_Priority =DMA_Priority_High;
	//存储器到存储器模式
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable ;
	
	DMA_Init(USART_DMA_CHANNEL, &DMA_InitStructure);
	
	DMA_ClearFlag(USART_TX_DMA_FLAG_TC);
	
	DMA_Cmd(USART_DMA_CHANNEL,ENABLE);
	
}


/**
	*判断指定长度的两个数据源是否完全相等
	*返回1表示一致，返回0表示不一致
**/

uint8_t BufferCompare(const uint32_t *pBUFFER,
											const uint32_t *pBUFFER2,uint16_t BufferLength){
		//数据长度递减
		while(BufferLength--){
			//判断两个数据源是否一致
			if(*pBUFFER != *pBUFFER2)
				return 0;
			pBUFFER++;
			pBUFFER2++;
		}
		return 1;
	}
