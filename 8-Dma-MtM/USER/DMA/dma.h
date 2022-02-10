#ifndef _DMA_H_
#define _DMA_H_


#include "stm32f10x.h"


#define  MTM_DMA_CLK					RCC_AHBPeriph_DMA1
#define  MTM_DMA_CHANNEL			DMA1_Channel6
#define  MTM_DMA_FLAG_TC			DMA1_FLAG_TC6
//要发送的数据大小
#define BUFFER_SIZE 32


void MtM_DMA_Config(void);
uint8_t BufferCompare(const uint32_t *pBUFFER,
											const uint32_t *pBUFFER2,uint16_t BufferLength);
#endif

