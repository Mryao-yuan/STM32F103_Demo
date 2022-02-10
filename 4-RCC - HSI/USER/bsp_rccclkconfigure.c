#include <stdio.h>
#include <bsp_rccclkconfigure.h>

//HSI����ϵͳʱ��
void HSI_SetSystemClk(uint32_t RCC_PLLMul_x){
	
	//HSI��״̬
  __IO uint32_t  HSIStatus=0;
	
	//��RCC�Ĵ�����λ�ɸ�λֵ
	RCC_DeInit();
	
	//ʹ��HSI
	RCC_HSICmd(ENABLE);
	
	//��ȡHSI״̬(����غ�����ֱ�Ӳ����Ĵ�����
	HSIStatus=RCC->CR & RCC_CR_HSIRDY;

	if(HSIStatus == RCC_CR_HSIRDY){
		//ʹ��Ԥȡֵ
		FLASH_GetPrefetchBufferStatus();
		
		//����FLASH�ĵȴ�����(2)
		FLASH_SetLatency(FLASH_Latency_2);
		
		//����HCLK
		RCC_HCLKConfig(RCC_SYSCLKSource_HSE);
		
		/* PCLK2 = HCLK=72M */
		RCC_PCLK2Config(RCC_SYSCLK_Div1);
		
		/* PCLK1 = HCLK=32M ��2��Ƶ��*/
		RCC_PCLK1Config(RCC_SYSCLK_Div2);
		
		//���໷����: PLLCLK = HSE * RCC_PLLMul_x ��4*9=36)
		RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_x);
		
		/* ʹ�� PLL */
		RCC_PLLCmd(ENABLE);
		
		//�ȴ�PLLʱ���ȶ�
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);
			
			/* ѡ�� PLL ��Ϊϵͳʱ��Դ */
			RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

			/* �ȴ� PLLCLK �л�Ϊϵͳʱ��   */
			while(RCC_GetSYSCLKSource() != 0x08);
	}
	 //HSE����ʧ�ܣ��û����ڴ˴���Ӵ���Ĵ������
	else{
		
		
		
	}
}



















//�ض���c�⺯��printf�����ڣ��ض�������ֱ��ʹ��printf����
//int fputc(int ch,FILE *f){
//	//����һ�����ݵ�����
//	USART_SendData(USART1 ,(uchar)(ch));
//	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
//	return ch;
//	
//}

////�ض���c�⺯����scanf�����ڣ��ض�������ʹ����scanf����
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


