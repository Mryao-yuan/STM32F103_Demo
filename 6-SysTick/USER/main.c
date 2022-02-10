#include "stm32f10x.h"
#include "LED.h"
#include "exti.h"
#include "usart.h"
#include "systick.h"
/*
	PA0���ӵ�EXTI���ڲ����жϣ�PA0�ĵ�ƽ�仯ͨ������������
	����һ���жϣ�LED��תһ��
	ͨ��������ʾ�Ƶ�״̬
*/
int main(){

	led_init();
	EXTI_Configue();
	usart_config();
	while(1){
		SysTick__Delay_ms(1000);
		GPIO_ResetBits(LED_PORT,LED);
		SysTick__Delay_ms(1000);
		GPIO_SetBits(LED_PORT,LED);
	}
}
