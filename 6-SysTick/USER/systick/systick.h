#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include "stm32f10x.h"
//systick���ں��ϵ�����
#include "core_cm3.h"

void SysTick__Delay_us(uint32_t us);
void SysTick__Delay_ms(uint32_t ms);
#endif

