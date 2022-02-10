#ifndef _LED_H_
#define _LED_H_


#include "stm32f10x.h"


#define  LED_PORT GPIOC 

#define   LED  GPIO_Pin_13

#define   LED_CLK  RCC_APB2Periph_GPIOC// ±÷”

void led_init();
#endif

