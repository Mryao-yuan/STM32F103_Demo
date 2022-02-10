#include "usart.h"



//���ڳ�ʼ��
void usart_config(void){
	
	
	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//�򿪴���ʱ��(A)
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK,ENABLE);
	
	//�򿪴�������ʱ��
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK,ENABLE);
	
	
	//��UASRT��Tx��GPIO����Ϊ���츴��ģʽ(A 9)
	GPIO_InitStructure.GPIO_Pin=DEBUG_USART_TX_Pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_PORT,&GPIO_InitStructure);
	
	//��UASRT��Rx��GPIO����Ϊ��������ģʽ(A 10)
	GPIO_InitStructure.GPIO_Pin=DEBUG_USART_RX_Pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_PORT,&GPIO_InitStructure);
	
	USART_InitStruct.USART_BaudRate=115200;//������
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	//����ģʽ���շ�ģʽһ��
	USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_Parity=USART_Parity_No ;
	USART_InitStruct.USART_StopBits=USART_StopBits_1;
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;
	//��ɴ��ڵĳ�ʼ��
	USART_Init(USART1, &USART_InitStruct);
	
	//�����ж����ȼ�����
//	NVIC_USART_Configuration();
	
	//ʹ�ܴ��ڽ����ж�
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);
	
	//ʹ�ܴ���
	USART_Cmd(DEBUG_USARTx,ENABLE);
	
}

//�ض���c�⺯��printf�����ڣ��ض�������ֱ��ʹ��printf����
int fputc(int ch,FILE *f){
	//����һ�����ݵ�����
	USART_SendData(USART1 ,(uint8_t)(ch));
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return ch;
}

//�ض���c�⺯����scanf�����ڣ��ض�������ʹ����scanf����
int fgetc(FILE *f){
	//�ȴ�������������
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
		return (int)USART_ReceiveData(USART1);
	
}
