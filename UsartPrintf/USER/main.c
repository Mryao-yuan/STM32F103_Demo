#include "stm32f10x.h"
#include "LED.h"
#include "usart.h"

void Delay(uint32_t count){
	for(;count!=0;count--);
	
}

int main(){

	led_init();
	USART_Config();
	
	
	while(1){
		printf("hello world!");
//	led_init();
//	GPIO_ResetBits( LED_PORT,LED);
//Delay(0xFFFFF);
//GPIO_SetBits( LED_PORT,LED);
//Delay(0xFFFFF);
	}
}
