#ifndef _USART_H_
#define _USART_H_


#include "stm32f10x.h"
#include <stdio.h>


//USART1∫Í∂®“Â
#define   USART1_CLK  		RCC_APB2Periph_USART1
#define   GPIO_CLK 				RCC_APB2Periph_GPIOA

#define   USARTx					USART1
#define   USART_TX				GPIOA
#define   USART_TX_PIN 		GPIO_Pin_9
#define   USART_RX				GPIOA
#define   USART_RX_PIN 		GPIO_Pin_10



int fputc(int ch,FILE *f);
void Usart_Configue(void);
void Usart_SendData(USART_TypeDef* pUSARTx, uint8_t Data);
#endif

