#include "dma.h"


uint8_t SendBuffer[ SendBuffer_Size];

void USART_DMA_Config(void){


	DMA_InitTypeDef  DMA_InitStructure;
	//��DMA1ʱ��
	RCC_AHBPeriphClockCmd(USART_DMA_CLK,ENABLE);
	
	//��ʼ��DMA�ṹ��
	//Դ���ݵ�ַ
	 DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t)USART_DR_ADDRESS;
	//�洢����ַ
	 DMA_InitStructure.DMA_MemoryBaseAddr=(uint32_t)SendBuffer;
	//���䷽��
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralDST;
	
	//������Ŀ
	DMA_InitStructure.DMA_BufferSize=SendBuffer_Size ;
	//�����ַ����ģʽ
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	//�������ݿ��(8λ=1�ֽڣ�
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	//�洢������ģʽ
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
	//�洢�����ݿ��
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	
	//ģʽ
	DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
	//ͨ�����ȼ�
	DMA_InitStructure.DMA_Priority =DMA_Priority_High;
	//�洢�����洢��ģʽ
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable ;
	
	DMA_Init(USART_DMA_CHANNEL, &DMA_InitStructure);
	
	DMA_ClearFlag(USART_TX_DMA_FLAG_TC);
	
	DMA_Cmd(USART_DMA_CHANNEL,ENABLE);
	
}


/**
	*�ж�ָ�����ȵ���������Դ�Ƿ���ȫ���
	*����1��ʾһ�£�����0��ʾ��һ��
**/

uint8_t BufferCompare(const uint32_t *pBUFFER,
											const uint32_t *pBUFFER2,uint16_t BufferLength){
		//���ݳ��ȵݼ�
		while(BufferLength--){
			//�ж���������Դ�Ƿ�һ��
			if(*pBUFFER != *pBUFFER2)
				return 0;
			pBUFFER++;
			pBUFFER2++;
		}
		return 1;
	}
