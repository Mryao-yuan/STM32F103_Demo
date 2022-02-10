#ifndef _KEY_H_
#define _KEY_H_


#include "stm32f10x.h"

#define KEY_GPIO_PIN GPIO_Pin_0
#define KEY_GPIO_PORT GPIOA
#define KEY_GPIO_CLK RCC_APB2Periph_GPIOA  //GPIOAµÄÊ±ÖÓ


#define KEY_ON 1
#define KEY_OFF 0


void KEY_GPIO_Config(void);
uint8_t Key_Scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin);
#endif

