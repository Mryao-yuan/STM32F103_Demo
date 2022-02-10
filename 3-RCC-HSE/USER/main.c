#include "stm32f10x.h"
#include "LED.h"
#include "usart.h"
#include "KEY.h"
#include "bsp_rccclkconfigure.h"

void Delay(uint32_t count){
	for(;count!=0;count--);
	
}

int main(){

	//��������ϵͳʱ���Ѿ������ó�72M
	
	//��Ƶ8*10=80
	HSE_SetSystemClk(RCC_PLLMul_10);
	led_init();
	
  while(1){
	Delay(0xFFFFF);
	LED_G_TOGGLE;
	Delay(0xFFFFF);	
	LED_G_TOGGLE;
	}
}