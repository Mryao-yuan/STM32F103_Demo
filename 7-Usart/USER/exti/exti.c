#include <stdio.h>
#include "exti.h"

//配置中断优先级
static void EXTI_NVIC_Config(void){
	
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStruct.NVIC_IRQChannel=EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1; //抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;  //	子优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
}


void EXTI_Configue(void){
	
	
	EXTI_NVIC_Config();
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	
	//GPIO初始化(A0为按键）
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//浮空输入，电平由外部决定
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	
	//EXTI初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_EXTILineConfig( GPIO_PortSourceGPIOA, GPIO_PinSource0);
	EXTI_InitStruct.EXTI_Line=EXTI_Line0;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;//上升沿
	
	EXTI_Init(&EXTI_InitStruct);
}
