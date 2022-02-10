#include <usart.h>

//重定向c库函数printf到串口，重定向后可以直接使用printf函数
int fputc(int ch,FILE *f){
	//发送一个数据到串口
	USART_SendData(USART1 ,(uint8_t)(ch));
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return ch;
}
//重定向c库函数到scanf到串口，重定向后可以使用是scanf函数
int fgetc(FILE *f){
	//等待串口输入数据
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
		return (int)USART_ReceiveData(USART1);
	
}
void Usart_Configue(void){
	
	GPIO_InitTypeDef		GPIO_InitStructure;
	USART_InitTypeDef		USART_InitStructure;
	
	//开GPIO时钟
	RCC_APB2PeriphClockCmd(GPIO_CLK,ENABLE);
	//开串口时钟
	RCC_APB2PeriphClockCmd(USART1_CLK,ENABLE);
	
	//A9:TX
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Pin=USART_TX_PIN ;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(USART_TX, &GPIO_InitStructure);
	//A10:RX
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=USART_RX_PIN;
	GPIO_Init(USART_RX, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate=115200;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None ;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_Parity=USART_Parity_No;

	USART_Init(USARTx, &USART_InitStructure);
	
	USART_Cmd(USARTx, ENABLE);
}



//写一个数据
void Usart_SendData(USART_TypeDef* pUSARTx, uint8_t Data){
	USART_SendData( pUSARTx, Data);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
}
