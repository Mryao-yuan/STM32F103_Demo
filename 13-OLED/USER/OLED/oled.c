#include "oled.h"



void ILI9341_GPIO_Config (void){
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(ILI9341_CS_CLK|ILI9341_BK_CLK|ILI9341_RD_CLK|ILI9341_DATA_CLK
	,ENABLE);//��ʱ��
	
	
	
	
	
	//����Ϊ����ģʽ
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=ILI9341_BK_PIN;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(ILI9341_BK_PORT,&GPIO_InitStruct);
	
	//����Ϊ���츴��ģʽ
	
	
	
	
	
	
}

void ILI9341_FSMC_Config(void){
	FSMC_NORSRAMInit 
	
	
	
	
}