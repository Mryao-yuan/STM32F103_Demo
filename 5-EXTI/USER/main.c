#include "stm32f10x.h"
#include "LED.h"
#include "exti.h"

void Delay(uint32_t count){
	for(;count!=0;count--);
	
}

int main(){

	led_init();
	//EXIT_GPIO_Config();
  while(1){
	
	}
}