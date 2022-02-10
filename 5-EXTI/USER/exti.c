#include "exti.h"

static void EXTI_NVIC_Config(void){
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}

//中断引脚配置
void EXIT_GPIO_Config(void){
	GPIO_InitTypeDef GPIO_InitSturcture;
	EXTI_InitTypeDef EXTI_InitSturcture;
	
	//配置中断优先级
	EXTI_NVIC_Config();
	
	//初始化GPIO
	RCC_APB2PeriphClockCmd(KEY_GPIO_CLK,ENABLE);
	GPIO_InitSturcture.GPIO_Pin=KEY_GPIO_PIN;
	GPIO_InitSturcture.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitSturcture.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(KEY_GPIO_PORT, &GPIO_InitSturcture);
	
	//初始化EXIT
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	
	EXTI_InitSturcture.EXTI_Line=EXTI_Line0;
	EXTI_InitSturcture.EXTI_LineCmd=ENABLE;
	EXTI_InitSturcture.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitSturcture.EXTI_Trigger=EXTI_Trigger_Rising;//上升延触发
	
	EXTI_Init(&EXTI_InitSturcture);	
	
}



































//重定向c库函数printf到串口，重定向后可以直接使用printf函数
//int fputc(int ch,FILE *f){
//	//发送一个数据到串口
//	USART_SendData(USART1 ,(uchar)(ch));
//	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
//	return ch;
//	
//}

//重定向c库函数到scanf到串口，重定向后可以使用是scanf函数
//int fgetc(FILE *f){
//	//等待串口输入数据
//		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
//		return (int)USART_ReceiveData(USART1);
//	
//}

//void USART_Config(void){
//	//GPIO和USART结构体初始化
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitSturcture;
//	
//	//打开串口时钟(A)
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	
//	//打开串口外设时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
//	
//	//将UASRT的Tx的GPIO设置为推挽复用模式(A 9)
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
//	
//	//将UASRT的Rx的GPIO设置为浮空输入模式(A 10)
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
//	
//	//串口配置的工作参数
//	//波特率
//	USART_InitSturcture.USART_BaudRate=115200;
//	//数据字长
//	USART_InitSturcture.USART_WordLength=USART_WordLength_8b;
//	//停止位
//	USART_InitSturcture.USART_StopBits=USART_StopBits_1;
//	//校验位(无）
//	USART_InitSturcture.USART_Parity =USART_Parity_No;
//	//硬件流控制
//	USART_InitSturcture.USART_HardwareFlowControl=USART_HardwareFlowControl_None ;
//	
//	//工作模式，收发一起
//	USART_InitSturcture.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
//	
//	USART_Init(USART1,&USART_InitSturcture);
//	
//	//使能串口
//	USART_Cmd(USART1,ENABLE);
//}


