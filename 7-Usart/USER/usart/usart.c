#include <usart.h>




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
//�����ж����ȼ�����
//static void NVIC_USART_Configuration(void){
//	
//	NVIC_InitTypeDef NVIC_InitStruct;
//	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_1);
//	//����USARTΪ�ж�Դ
//	NVIC_InitStruct.NVIC_IRQChannel=DEBUG_USART_IRQ;
//	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;

//	NVIC_Init(&NVIC_InitStruct);	
//}



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

//���ڷ���һ���ֽ�(8λ���ͣ�
void Usart_SendByte(USART_TypeDef* pUSARTx, uint8_t data){
	//���ڷ�������
	USART_SendData(pUSARTx,data);
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==	RESET);
	
}

//���������ֽڵ�����
void Usart_SendHalfWord(USART_TypeDef* pUSARTx, uint16_t data){
	
	uint8_t temp_h,temp_l;
	
	//ȡ��8λ
	temp_h=(data&0xff00)>>8;
	//ȡ��8λ
	temp_l=data&0xff;
	
	USART_SendData(pUSARTx,temp_h);
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==	RESET);
	
	USART_SendData(pUSARTx,temp_l);
	//TXE���������ݼĴ���Ϊ��
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==	RESET);

}
//����8λ���ݵ�����
void Usart_SendArray(USART_TypeDef* pUSARTx,uint8_t *array ,uint8_t num){
	
	uint8_t i;
	for(i=0;i<num;i++)
		Usart_SendByte(pUSARTx, array[i]);

	//��ʾλ�Ĵ���ʹ��TC(������ɱ�־)
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
	
}
//�����ַ���
void Usart_SendString(USART_TypeDef* pUSARTx,uint8_t *str){
	uint8_t i=0;
	do{
		Usart_SendByte(pUSARTx, *(str+i));
		i++;
	}while(*(str+i) !='\0');
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);	
}
