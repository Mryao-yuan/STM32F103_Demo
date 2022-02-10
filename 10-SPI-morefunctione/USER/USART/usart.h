#ifndef _USART_H_
#define _USART_H_


#include "stm32f10x.h"
#include "stdio.h"

//串口移植
#define DEBUG_USART1										1  
#define DEBUG_USART2  									0
#define DEBUG_USART3  									0
#define DEBUG_USART4  									0
#define DEBUG_USART5  									0


#if DEBUG_USART1

//串口1-USART1

#define DEBUG_USARTx   									USART1
#define DEBUG_USART_CLK  								RCC_APB2Periph_USART1
#define DEBUG_USART_APBxClkCmd					RCC_APB2PeriphClockCmd
#define DEBUG_USART_BaudRate  					115200

//USART GPIO 引脚宏定义
#define DEBUG_USART_GPIO_CLK  					RCC_APB2Periph_GPIOA
#define DEBUG_USART_GPIO_APBxClkCmd			RCC_APB2PeriphClockCmd
//TX:A9,RX:A10
#define  DEBUG_USART_TX_PORT 						GPIOA 
#define  DEBUG_USART_TX_Pin							GPIO_Pin_9
#define  DEBUG_USART_RX_PORT 						GPIOA 
#define  DEBUG_USART_RX_Pin							GPIO_Pin_10

#define  DEBUG_USART_IRQ 								USART1_IRQn 
#define  DEBUG_USART_IRQHandler 				USART1_IRQHandler


#elif DEBUG_USART2

//串口2-USART2

#define DEBUG_USARTx   									USART2
#define DEBUG_USART_CLK  								RCC_APB1Periph_USART2
#define DEBUG_USART_APBxClkCmd					RCC_APB1PeriphClockCmd
#define DEBUG_USART_BaudRate  					115200

//USART GPIO 引脚宏定义
#define DEBUG_USART_GPIO_CLK  					RCC_APB2Periph_GPIOA
#define DEBUG_USART_GPIO_APBxClkCmd			RCC_APB2PeriphClockCmd
//TX:A2,RX:A3          
#define  DEBUG_USART_TX_PORT 						GPIOA 
#define  DEBUG_USART_TX_Pin							GPIO_Pin_2
#define  DEBUG_USART_RX_PORT 						GPIOA 
#define  DEBUG_USART_RX_Pin							GPIO_Pin_3

#define  DEBUG_USART_IRQ 								USART2_IRQn 
#define  DEBUG_USART_IRQHandler 				USART2_IRQHandler


#elif DEBUG_USART3

//串口3-USART3

#define DEBUG_USARTx   									USART3
#define DEBUG_USART_CLK  								RCC_APB1Periph_USART3
#define DEBUG_USART_APBxClkCmd					RCC_APB1PeriphClockCmd
#define DEBUG_USART_BaudRate  					115200

//USART GPIO 引脚宏定义
#define DEBUG_USART_GPIO_CLK  					RCC_APB2Periph_GPIOB
#define DEBUG_USART_GPIO_APBxClkCmd			RCC_APB2PeriphClockCmd
//TX:B10,RX:B11          
#define  DEBUG_USART_TX_PORT 						GPIOB 
#define  DEBUG_USART_TX_Pin							GPIO_Pin_10
#define  DEBUG_USART_RX_PORT 						GPIOB 
#define  DEBUG_USART_RX_Pin							GPIO_Pin_11

#define  DEBUG_USART_IRQ 								USART3_IRQn 
#define  DEBUG_USART_IRQHandler 				USART3_IRQHandler


#elif DEBUG_USART4
//串口4-USART4

#define DEBUG_USARTx   									USART4
#define DEBUG_USART_CLK  								RCC_APB1Periph_USART4
#define DEBUG_USART_APBxClkCmd					RCC_APB1PeriphClockCmd
#define DEBUG_USART_BaudRate  					115200

//USART GPIO 引脚宏定义
#define DEBUG_USART_GPIO_CLK  					RCC_APB2Periph_GPIOC
#define DEBUG_USART_GPIO_APBxClkCmd			RCC_APB2PeriphClockCmd
//TX:C10,RX:C11          
#define  DEBUG_USART_TX_PORT 						GPIOC
#define  DEBUG_USART_TX_Pin							GPIO_Pin_10
#define  DEBUG_USART_RX_PORT 						GPIOC 
#define  DEBUG_USART_RX_Pin							GPIO_Pin_11

#define  DEBUG_USART_IRQ 								USART4_IRQn 
#define  DEBUG_USART_IRQHandler 				USART4_IRQHandler

#elif DEBUG_USART5
//串口5-USART5

#define DEBUG_USARTx   									USART5
#define DEBUG_USART_CLK  								RCC_APB1Periph_USART5
#define DEBUG_USART_APBxClkCmd					RCC_APB1PeriphClockCmd
#define DEBUG_USART_BaudRate  					115200

//USART GPIO 引脚宏定义
#define DEBUG_USART_GPIO_CLK  					(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD)
#define DEBUG_USART_GPIO_APBxClkCmd			RCC_APB2PeriphClockCmd
//TX:C10,RX:C11          
#define  DEBUG_USART_TX_PORT 						GPIOC
#define  DEBUG_USART_TX_Pin							GPIO_Pin_12
#define  DEBUG_USART_RX_PORT 						GPIOD 
#define  DEBUG_USART_RX_Pin							GPIO_Pin_2

#define  DEBUG_USART_IRQ 								USART5_IRQn 
#define  DEBUG_USART_IRQHandler 				USART5_IRQHandler



#endif

















int fputc(int ch,FILE *f);
int fgetc(FILE *f);
void usart_config(void);
static void NVIC_USART_Configuration(void);
void Usart_SendByte(USART_TypeDef* pUSARTx, uint8_t data);
void Usart_SendHalfWord(USART_TypeDef* pUSARTx,uint16_t data);
void Usart_SendArray(USART_TypeDef* pUSARTx,uint8_t *array ,uint8_t num);
void Usart_SendString(USART_TypeDef* pUSARTx,uint8_t *str);
















#endif

