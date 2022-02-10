#include "stm32f10x.h"
#include "LED.h"
#include "usart.h"
#include "KEY.h"
void Delay(uint32_t count){
	for(;count!=0;count--);
	
}

int main(){

	led_init();
	USART_Config();
	KEY_GPIO_Config();
	
  while(1){
		if(Key_Scan(KEY_GPIO_PORT,KEY_GPIO_PIN)==KEY_ON)
			LED_G_TOGGLE;
//			GPIO_ResetBits(LED_PORT,LED);
//		else
//			GPIO_SetBits(LED_PORT,LED);
	}
}
