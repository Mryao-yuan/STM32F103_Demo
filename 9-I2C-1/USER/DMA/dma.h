#ifndef _DMA_H_
#define _DMA_H_


#include "stm32f10x.h"

//要发送的数据大小
#define SendBuffer_Size 					5000







#define  USART_DMA_CLK						RCC_AHBPeriph_DMA1

//串口对应的DMA请求通道
#define  USART_DMA_CHANNEL				DMA1_Channel4
//外设寄存器地址=串口基地址+偏移
#define  USART_DR_ADDRESS					(USART1_BASE+0x04)
#define  USART_TX_DMA_FLAG_TC			DMA1_FLAG_TC4

void USART_DMA_Config(void);


uint8_t BufferCompare(const uint32_t *pBUFFER,
											const uint32_t *pBUFFER2,uint16_t BufferLength);
#endif

