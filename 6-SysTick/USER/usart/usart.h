#ifndef _USART_H_
#define _USART_H_


#include "stm32f10x.h"
#include "stdio.h"

#define  LED_PORT GPIOC 

#define   LED  GPIO_Pin_13

#define   LED_CLK  RCC_APB2Periph_GPIOC//Ê±ÖÓ

#define  LED_G_CHANGE  {GPIOC->ODR ^=GPIO_Pin_13; } 

typedef unsigned char uchar;


int fputc(int ch,FILE *f);
int fgetc(FILE *f);
void usart_config(void);
#endif

