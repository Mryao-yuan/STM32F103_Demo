#include <systick.h>


#if 0
static __INLINE uint32_t SysTick_Config(uint32_t ticks)
{ 
	//判断tick是否大于2`24,如果大于。则不符合规则
  if (ticks > SysTick_LOAD_RELOAD_Msk)  return (1);            /* Reload value impossible */
  //初始化设置重装载寄存器                                                        
  SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      /* set reload register */
  //配置中断优先级，配置为15，默认最低优先级
	NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Cortex-M0 System Interrupts */
  //初始化counter的值为0
	SysTick->VAL   = 0;  
	//配置systick的时钟为72M
	//使能中断
	//使能systick
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | 
                   SysTick_CTRL_TICKINT_Msk   | 
                   SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
  return (0);                                                  /* Function successful */
}



#endif
//微秒级定时
void SysTick__Delay_us(uint32_t us){
	uint32_t i;
	SysTick_Config(72);
	for(i=0;i<us;i++){
		//当计数器的值减到0时，CRTL寄存器的位会置1
		while(!((SysTick->CTRL) &(1<<16)));
		
	}
	//关闭寄存器
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

//毫秒级定时
void SysTick__Delay_ms(uint32_t ms){
	uint32_t i;
	SysTick_Config(72000);
	for(i=0; i<ms; i++){
		//状态寄存器为1
		while(!((SysTick->CTRL) &(1<<16)));
		
	}
	//关闭寄存器
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}


//中断实现
