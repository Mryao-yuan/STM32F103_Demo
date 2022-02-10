#include "stm32f10x.h"
#include "LED.h"
#include "usart.h"
#include "KEY.h"
#include "bsp_rccclkconfigure.h"

void Delay(uint32_t count){
	for(;count!=0;count--);
	
}

int main(){

	//来到这里系统时钟已经被配置成72M
	
	//超频8*10=80
	HSE_SetSystemClk(RCC_PLLMul_10);
	led_init();
	
  while(1){
	Delay(0xFFFFF);
	LED_G_TOGGLE;
	Delay(0xFFFFF);	
	LED_G_TOGGLE;
	}
}