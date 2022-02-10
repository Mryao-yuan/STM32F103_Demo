#include "stm32f10x.h"
#include "LED.h"
#include "usart.h"
#include "KEY.h"


//位带地址计算公式((addr&0xF0000000)+0x02000000+((addr&0x00FFFFFF)<<5)\
+(bitnum<<2))

//基地址+偏移位置
#define GPIOC_ODR_Addr (GPIOC_BASE+0X0C)

#define GPIOA_IDR_Addr (GPIOA_BASE+0X08)

#define PCout(n) *(unsigned int *)((GPIOC_ODR_Addr&0xF0000000)+0x02000000+((GPIOC_ODR_Addr&0x00FFFFFF)<<5)+(n<<2))

#define PAin(n) *(unsigned int *)((GPIOA_IDR_Addr&0xF0000000)+0x02000000+((GPIOA_IDR_Addr&0x00FFFFFF)<<5)+(n<<2))



void Delay(uint32_t count){
	for(;count!=0;count--);
	
}

int main(){

	led_init();
	KEY_GPIO_Config();
#if 0
	while(1){
	//	GPIO_SetBits(LED_PORT,LED);
	PCout(13)=1;
		Delay(0xFFFFFF);
		//GPIO_ResetBits(LED_PORT,LED);
	PCout(13)=0;
	Delay(0xFFFFFF);
	}
	
#else 
	while(1){
		if(PAin(0)==KEY_ON)
		//检测按键释放
		{
			while(PAin(0)==KEY_ON)
				LED_G_TOGGLE;
		}
		
		
		
		//if(Key_Scan(KEY_GPIO_PORT,KEY_GPIO_PIN)==KEY_ON)
			//LED_G_TOGGLE;
//			GPIO_ResetBits(LED_PORT,LED);
//		else
//			GPIO_SetBits(LED_PORT,LED);
	}
#endif
}
