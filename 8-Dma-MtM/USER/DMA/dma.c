#include "dma.h"

/* ���� aSRC_Const_Buffer ������Ϊ DMA ��������Դ
* const �ؼ��ֽ� aSRC_Const_Buffer �����������Ϊ��������
* ��ʾ���ݴ洢���ڲ��� FLASH ��
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

/* ���� DMA ����Ŀ��洢��
*�洢���ڲ��� SRAM ��
*/
const uint32_t aDST_Buffer[BUFFER_SIZE];


void MtM_DMA_Config(void){


	DMA_InitTypeDef  DMA_InitStructure;
	//��DMA1ʱ��
	RCC_AHBPeriphClockCmd(MTM_DMA_CLK,ENABLE);
	
	//��ʼ��DMA�ṹ��
	//Դ���ݵ�ַ
	 DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t)aSRC_Const_Buffer;
	//�洢����ַ
	 DMA_InitStructure.DMA_MemoryBaseAddr=(uint32_t)aDST_Buffer;
	//���䷽��
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;
	
	//������Ŀ
	DMA_InitStructure.DMA_BufferSize=BUFFER_SIZE;
	//�����ַ����ģʽ
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Enable;
	//�������ݿ��
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Word;
	//�洢������ģʽ
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
	//�洢�����ݿ��
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Word;
	
	//ģʽ
	DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
	//ͨ�����ȼ�
	DMA_InitStructure.DMA_Priority =DMA_Priority_High;
	//�洢�����洢��ģʽ
	DMA_InitStructure.DMA_M2M=DMA_M2M_Enable ;
	
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);
	
	DMA_ClearFlag(MTM_DMA_FLAG_TC);
	
	DMA_Cmd(MTM_DMA_CHANNEL,ENABLE);
	
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
