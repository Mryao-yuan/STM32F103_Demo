#ifndef _OLED_H_
#define _OLED_H_


#include "stm32f10x.h"

/*	
	оƬ���ƣ�ili9341
*/

/* LCD�����ź� */	
//Ƭѡ
#define  ILI9341_CS_CLK  				RCC_APB2Periph_GPIOC
#define  ILI9341_CS_PORT				GPIOC
#define  ILI9341_CS_PIN					GPIO_Pin_4

/* ��λ����ֱ��ʹ��NRST,�����帴λ��ʱ���ʹҺ����λ */

//����
#define  ILI9341_BK_CLK  				RCC_APB2Periph_GPIOD
#define  ILI9341_BK_PORT				GPIOD
#define  ILI9341_BK_PIN					GPIO_Pin_2
//��ʹ��
#define  ILI9341_RD_CLK  				RCC_APB2Periph_GPIOC
#define  ILI9341_RD_PORT				GPIOC
#define  ILI9341_RD_PIN					GPIO_Pin_5
//дʹ��
#define  ILI9341_WR_CLK  				RCC_APB2Periph_GPIOC
#define  ILI9341_WR_PORT				GPIOC
#define  ILI9341_WR_PIN					GPIO_Pin_6
/* LCD�����ź��� */
#define  ILI9341_DATA_CLK  				RCC_APB2Periph_GPIOB
#define  ILI9341_DATA_PORT				GPIOB
#define  ILI9341_DATA_PIN					GPIO_Pin_All

/*  �ź��߿������  */
#define ILI9341_CS_SET				













#endif /*  _OLED_H_ */
