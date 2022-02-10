#include <stdio.h>
#include <usart.h>



//重定向c库函数printf到串口，重定向后可以直接使用printf函数
int fputc(int ch,FILE *f){
	//发送一个数据到串口
	USART_SendData(USART1 ,(uchar)(ch));
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return ch;
	
}

//重定向c库函数到scanf到串口，重定向后可以使用是scanf函数
int fgetc(FILE *f){
	//等待串口输入数据
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
		return (int)USART_ReceiveData(USART1);
	
}

void USART_Config(void){
	//GPIO和USART结构体初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitSturcture;
	
	//打开串口时钟(A)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//打开串口外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//将UASRT的Tx的GPIO设置为推挽复用模式(A 9)
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//将UASRT的Rx的GPIO设置为浮空输入模式(A 10)
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//串口配置的工作参数
	//波特率
	USART_InitSturcture.USART_BaudRate=115200;
	//数据字长
	USART_InitSturcture.USART_WordLength=USART_WordLength_8b;
	//停止位
	USART_InitSturcture.USART_StopBits=USART_StopBits_1;
	//校验位(无）
	USART_InitSturcture.USART_Parity =USART_Parity_No;
	//硬件流控制
	USART_InitSturcture.USART_HardwareFlowControl=USART_HardwareFlowControl_None ;
	
	//工作模式，收发一起
	USART_InitSturcture.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	
	USART_Init(USART1,&USART_InitSturcture);
	
	//使能串口
	USART_Cmd(USART1,ENABLE);
}


