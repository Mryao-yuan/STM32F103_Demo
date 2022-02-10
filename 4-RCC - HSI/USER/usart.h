#ifndef _USART_H_
#define _USART_H_


#include "stm32f10x.h"
#include "stdio.h"

typedef unsigned char uchar;
void USART_Config(void);
int fgetc(FILE *f);
int fputc(int ch,FILE *f);
#endif

