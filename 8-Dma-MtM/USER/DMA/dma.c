#include "dma.h"

/* 定义 aSRC_Const_Buffer 数组作为 DMA 传输数据源
* const 关键字将 aSRC_Const_Buffer 数组变量定义为常量类型
* 表示数据存储在内部的 FLASH 中
*/
const uint32_t aSRC_Const_Buffer[BUFFER_SIZE]= {  
					0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10, 
					0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20, 
					0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30, 
					0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40, 
					0x41424344,0x45464748,0x494A4B4C,0x4D4E4F50, 
					0x51525354,0x55565758,0x595A5B5C,0x5D5E5F60, 
					0x61626364,0x65666768,0x696A6B6C,0x6D6E6F70, 
					0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80 
}; 

/* 定义 DMA 传输目标存储器
*存储在内部的 SRAM 中
*/
const uint32_t aDST_Buffer[BUFFER_SIZE];


void MtM_DMA_Config(void){


	DMA_InitTypeDef  DMA_InitStructure;
	//开DMA1时钟
	RCC_AHBPeriphClockCmd(MTM_DMA_CLK,ENABLE);
	
	//初始化DMA结构体
	//源数据地址
	 DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t)aSRC_Const_Buffer;
	//存储器地址
	 DMA_InitStructure.DMA_MemoryBaseAddr=(uint32_t)aDST_Buffer;
	//传输方向
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;
	
	//传输数目
	DMA_InitStructure.DMA_BufferSize=BUFFER_SIZE;
	//外设地址增量模式
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Enable;
	//外设数据宽度
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Word;
	//存储器增量模式
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
	//存储器数据宽度
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Word;
	
	//模式
	DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
	//通道优先级
	DMA_InitStructure.DMA_Priority =DMA_Priority_High;
	//存储器到存储器模式
	DMA_InitStructure.DMA_M2M=DMA_M2M_Enable ;
	
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);
	
	DMA_ClearFlag(MTM_DMA_FLAG_TC);
	
	DMA_Cmd(MTM_DMA_CHANNEL,ENABLE);
	
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
