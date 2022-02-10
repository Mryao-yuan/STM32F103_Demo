#ifndef _DMA_H_
#define _DMA_H_


#include "stm32f10x.h"

//Ҫ���͵����ݴ�С
#define SendBuffer_Size 					5000







#define  USART_DMA_CLK						RCC_AHBPeriph_DMA1

//���ڶ�Ӧ��DMA����ͨ��
#define  USART_DMA_CHANNEL				DMA1_Channel4
//����Ĵ�����ַ=���ڻ���ַ+ƫ��
#define  USART_DR_ADDRESS					(USART1_BASE+0x04)
#define  USART_TX_DMA_FLAG_TC			DMA1_FLAG_TC4

void USART_DMA_Config(void);


uint8_t BufferCompare(const uint32_t *pBUFFER,
											const uint32_t *pBUFFER2,uint16_t BufferLength);
#endif

