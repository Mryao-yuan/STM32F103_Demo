#ifndef _LED_H_
#define _LED_H_


#include "stm32f10x.h"


#define  LED_PORT GPIOC 

#define   LED  GPIO_Pin_13

#define   LED_CLK  RCC_APB2Periph_GPIOC//时钟

//异或
//与1异或改变，与0异或不变
#define LED_G_TOGGLE  {LED_PORT->ODR ^= LED;}
void led_init();
#endif


