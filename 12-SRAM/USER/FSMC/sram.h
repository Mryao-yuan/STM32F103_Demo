#ifndef _SRAM_H_
#define _SRAM_H_


#include "stm32f10x.h"

/*	
	оƬ���ƣ�SRAM
*/

/* LCD�����ź� */	
//Ƭѡ
#define  SRAM_CLK  				RCC_APB2Periph_GPIOC
#define  SRAM_PORT				GPIOC
#define  SRAM_PIN					GPIO_Pin_4

/* ��λ����ֱ��ʹ��NRST,�����帴λ��ʱ���ʹҺ����λ */

//����
#define  SRAM_BK_CLK  				RCC_APB2Periph_GPIOD
#define  SRAM_BK_PORT				GPIOD
#define  SRAM_BK_PIN					GPIO_Pin_2
//��ʹ��
#define  SRAM_RD_CLK  				RCC_APB2Periph_GPIOC
#define  SRAM_RD_PORT				GPIOC
#define  SRAM_RD_PIN					GPIO_Pin_5
//дʹ��
#define  SRAM_WR_CLK  				RCC_APB2Periph_GPIOC
#define  SRAM_WR_PORT				GPIOC
#define  SRAM_WR_PIN					GPIO_Pin_6
/* LCD�����ź��� */
#define  SRAM_DATA_CLK  				RCC_APB2Periph_GPIOB
#define  SRAM_DATA_PORT				GPIOB
#define  SRAM_DATA_PIN					GPIO_Pin_All

/*  �ź��߿������  */
#define SRAM_SET				













#endif /*  _SRAM_H_ */
