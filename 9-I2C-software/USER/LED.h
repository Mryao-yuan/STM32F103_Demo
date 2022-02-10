#ifndef _LED_H_
#define _LED_H_


#include "stm32f10x.h"


#define  	LED_PORT 				GPIOC 

#define   LED  						GPIO_Pin_13

#define   LED_CLK  				RCC_APB2Periph_GPIOC//Ê±ÖÓ

#define 	LED_ON  				GPIO_ResetBits(LED_PORT,LED);
#define 	LED_OFF  				GPIO_SetBits(LED_PORT,LED);

#define  	LED_CHANGE			{LED_PORT->ODR ^= LED;}
void led_init(void);
#endif

