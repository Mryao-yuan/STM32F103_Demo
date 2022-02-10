#include <systick.h>


#if 0
static __INLINE uint32_t SysTick_Config(uint32_t ticks)
{ 
	//�ж�tick�Ƿ����2`24,������ڡ��򲻷��Ϲ���
  if (ticks > SysTick_LOAD_RELOAD_Msk)  return (1);            /* Reload value impossible */
  //��ʼ��������װ�ؼĴ���                                                        
  SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      /* set reload register */
  //�����ж����ȼ�������Ϊ15��Ĭ��������ȼ�
	NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Cortex-M0 System Interrupts */
  //��ʼ��counter��ֵΪ0
	SysTick->VAL   = 0;  
	//����systick��ʱ��Ϊ72M
	//ʹ���ж�
	//ʹ��systick
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | 
                   SysTick_CTRL_TICKINT_Msk   | 
                   SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
  return (0);                                                  /* Function successful */
}



#endif
//΢�뼶��ʱ
void SysTick__Delay_us(uint32_t us){
	uint32_t i;
	SysTick_Config(72);
	for(i=0;i<us;i++){
		//����������ֵ����0ʱ��CRTL�Ĵ�����λ����1
		while(!((SysTick->CTRL) &(1<<16)));
		
	}
	//�رռĴ���
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

//���뼶��ʱ
void SysTick__Delay_ms(uint32_t ms){
	uint32_t i;
	SysTick_Config(72000);
	for(i=0; i<ms; i++){
		//״̬�Ĵ���Ϊ1
		while(!((SysTick->CTRL) &(1<<16)));
		
	}
	//�رռĴ���
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}


//�ж�ʵ��
