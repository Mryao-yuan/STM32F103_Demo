#ifndef _OLED_H_
#define _OLED_H_


#include "stm32f10x.h"

/*	
	芯片名称：ili9341
*/

/* LCD控制信号 */	
//片选
#define  ILI9341_CS_CLK  				RCC_APB2Periph_GPIOC
#define  ILI9341_CS_PORT				GPIOC
#define  ILI9341_CS_PIN					GPIO_Pin_4

/* 复位引脚直接使用NRST,开发板复位的时候会使液晶复位 */

//背光
#define  ILI9341_BK_CLK  				RCC_APB2Periph_GPIOD
#define  ILI9341_BK_PORT				GPIOD
#define  ILI9341_BK_PIN					GPIO_Pin_2
//读使能
#define  ILI9341_RD_CLK  				RCC_APB2Periph_GPIOC
#define  ILI9341_RD_PORT				GPIOC
#define  ILI9341_RD_PIN					GPIO_Pin_5
//写使能
#define  ILI9341_WR_CLK  				RCC_APB2Periph_GPIOC
#define  ILI9341_WR_PORT				GPIOC
#define  ILI9341_WR_PIN					GPIO_Pin_6
/* LCD数据信号线 */
#define  ILI9341_DATA_CLK  				RCC_APB2Periph_GPIOB
#define  ILI9341_DATA_PORT				GPIOB
#define  ILI9341_DATA_PIN					GPIO_Pin_All

/*  信号线控制相关  */
#define ILI9341_CS_SET				













#endif /*  _OLED_H_ */
