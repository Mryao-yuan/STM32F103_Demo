#ifndef _SRAM_H_
#define _SRAM_H_


#include "stm32f10x.h"

/*	
	芯片名称：SRAM
*/

/* LCD控制信号 */	
//片选
#define  SRAM_CLK  				RCC_APB2Periph_GPIOC
#define  SRAM_PORT				GPIOC
#define  SRAM_PIN					GPIO_Pin_4

/* 复位引脚直接使用NRST,开发板复位的时候会使液晶复位 */

//背光
#define  SRAM_BK_CLK  				RCC_APB2Periph_GPIOD
#define  SRAM_BK_PORT				GPIOD
#define  SRAM_BK_PIN					GPIO_Pin_2
//读使能
#define  SRAM_RD_CLK  				RCC_APB2Periph_GPIOC
#define  SRAM_RD_PORT				GPIOC
#define  SRAM_RD_PIN					GPIO_Pin_5
//写使能
#define  SRAM_WR_CLK  				RCC_APB2Periph_GPIOC
#define  SRAM_WR_PORT				GPIOC
#define  SRAM_WR_PIN					GPIO_Pin_6
/* LCD数据信号线 */
#define  SRAM_DATA_CLK  				RCC_APB2Periph_GPIOB
#define  SRAM_DATA_PORT				GPIOB
#define  SRAM_DATA_PIN					GPIO_Pin_All

/*  信号线控制相关  */
#define SRAM_SET				













#endif /*  _SRAM_H_ */
