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
//串口中断优先级配置
//static void NVIC_USART_Configuration(void){
//	
//	NVIC_InitTypeDef NVIC_InitStruct;
//	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_1);
//	//配置USART为中断源
//	NVIC_InitStruct.NVIC_IRQChannel=DEBUG_USART_IRQ;
//	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;

//	NVIC_Init(&NVIC_InitStruct);	
//}



//串口初始化
void usart_config(void){
	
	
	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//打开串口时钟(A)
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK,ENABLE);
	
	//打开串口外设时钟
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK,ENABLE);
	
	
	//将UASRT的Tx的GPIO设置为推挽复用模式(A 9)
	GPIO_InitStructure.GPIO_Pin=DEBUG_USART_TX_Pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_PORT,&GPIO_InitStructure);
	
	//将UASRT的Rx的GPIO设置为浮空输入模式(A 10)
	GPIO_InitStructure.GPIO_Pin=DEBUG_USART_RX_Pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_PORT,&GPIO_InitStructure);
	
	USART_InitStruct.USART_BaudRate=115200;//波特率
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	//工作模式：收发模式一起
	USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_Parity=USART_Parity_No ;
	USART_InitStruct.USART_StopBits=USART_StopBits_1;
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;
	//完成串口的初始化
	USART_Init(USART1, &USART_InitStruct);
	
	//串口中断优先级配置
//	NVIC_USART_Configuration();
	
	//使能串口接收中断
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);
	
	//使能串口
	USART_Cmd(DEBUG_USARTx,ENABLE);
	
}

//串口发送一个字节(8位发送）
void Usart_SendByte(USART_TypeDef* pUSARTx, uint8_t data){
	//串口发送数据
	USART_SendData(pUSARTx,data);
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==	RESET);
	
}

//发送两个字节的数据
void Usart_SendHalfWord(USART_TypeDef* pUSARTx, uint16_t data){
	
	uint8_t temp_h,temp_l;
	
	//取高8位
	temp_h=(data&0xff00)>>8;
	//取低8位
	temp_l=data&0xff;
	
	USART_SendData(pUSARTx,temp_h);
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==	RESET);
	
	USART_SendData(pUSARTx,temp_l);
	//TXE：传输数据寄存器为空
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==	RESET);

}
//发送8位数据的数组
void Usart_SendArray(USART_TypeDef* pUSARTx,uint8_t *array ,uint8_t num){
	
	uint8_t i;
	for(i=0;i<num;i++)
		Usart_SendByte(pUSARTx, array[i]);

	//表示位寄存器使用TC(传输完成标志)
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
	
}
//发送字符串
void Usart_SendString(USART_TypeDef* pUSARTx,uint8_t *str){
	uint8_t i=0;
	do{
		Usart_SendByte(pUSARTx, *(str+i));
		i++;
	}while(*(str+i) !='\0');
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);	
}
