#include <KEY.h>


void KEY_GPIO_Config(void){
	GPIO_InitTypeDef GPIO_InitSturcture;
	
	RCC_APB2PeriphClockCmd(KEY_GPIO_CLK,ENABLE);
	
	GPIO_InitSturcture.GPIO_Pin=KEY_GPIO_PIN;
	GPIO_InitSturcture.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitSturcture.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(KEY_GPIO_PORT,&GPIO_InitSturcture);
	
}
//����ɨ�裨GPIO�˿ڣ�
uint8_t Key_Scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin){
	
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==KEY_ON)
	{
		//���ּ��
		while(GPIO_ReadInputDataBit (GPIOx,GPIO_Pin)==KEY_ON)
			return KEY_ON;
	}
	else 
		return KEY_OFF;
	
}




































//�ض���c�⺯��printf�����ڣ��ض�������ֱ��ʹ��printf����
//int fputc(int ch,FILE *f){
//	//����һ�����ݵ�����
//	USART_SendData(USART1 ,(uchar)(ch));
//	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
//	return ch;
//	
//}

//�ض���c�⺯����scanf�����ڣ��ض�������ʹ����scanf����
//int fgetc(FILE *f){
//	//�ȴ�������������
//		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
//		return (int)USART_ReceiveData(USART1);
//	
//}

//void USART_Config(void){
//	//GPIO��USART�ṹ���ʼ��
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitSturcture;
//	
//	//�򿪴���ʱ��(A)
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	
//	//�򿪴�������ʱ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
//	
//	//��UASRT��Tx��GPIO����Ϊ���츴��ģʽ(A 9)
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
//	
//	//��UASRT��Rx��GPIO����Ϊ��������ģʽ(A 10)
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
//	
//	//�������õĹ�������
//	//������
//	USART_InitSturcture.USART_BaudRate=115200;
//	//�����ֳ�
//	USART_InitSturcture.USART_WordLength=USART_WordLength_8b;
//	//ֹͣλ
//	USART_InitSturcture.USART_StopBits=USART_StopBits_1;
//	//У��λ(�ޣ�
//	USART_InitSturcture.USART_Parity =USART_Parity_No;
//	//Ӳ��������
//	USART_InitSturcture.USART_HardwareFlowControl=USART_HardwareFlowControl_None ;
//	
//	//����ģʽ���շ�һ��
//	USART_InitSturcture.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
//	
//	USART_Init(USART1,&USART_InitSturcture);
//	
//	//ʹ�ܴ���
//	USART_Cmd(USART1,ENABLE);
//}

