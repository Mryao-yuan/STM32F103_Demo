#include <usart.h>




//�ض���c�⺯��printf�����ڣ��ض�������ֱ��ʹ��printf����
int fputc(int ch,FILE *f){
	//����һ�����ݵ�����
	USART_SendData(USART1 ,(uchar)(ch));
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return ch;
	
}

//�ض���c�⺯����scanf�����ڣ��ض�������ʹ����scanf����
int fgetc(FILE *f){
	//�ȴ�������������
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
		return (int)USART_ReceiveData(USART1);
	
}




//���ڳ�ʼ��
void usart_config(void){
	
	
	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	
	//�򿪴���ʱ��(A)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//�򿪴�������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	
	//��UASRT��Tx��GPIO����Ϊ���츴��ģʽ(A 9)
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//��UASRT��Rx��GPIO����Ϊ��������ģʽ(A 10)
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitStruct.USART_BaudRate=115200;//������
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_Parity=USART_Parity_No ;
	USART_InitStruct.USART_StopBits=USART_StopBits_1;
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;
	
	USART_Init(USART1, &USART_InitStruct);
	
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//ʹ�ܴ���
	USART_Cmd(USART1,ENABLE);
	
}


