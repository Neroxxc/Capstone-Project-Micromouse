#include "time.h"

#include "process.h"

void Timer4Init()
{
	RCC->APB1ENR|= 0x4;        //��tim4ʱ��
	RCC->APB2ENR|=(1<<3);    //GPIOB clock
	
	GPIOB->CRH&=~(0xf<<0);
	GPIOB->CRH|=(0xb<<0); //PB8���츴�����50M
	
	TIM4->SMCR&=~0x7;     //ѡ���ڲ�ʱ��
	TIM4->PSC=0;       //Ԥ��Ƶ
	TIM4->ARR=3600 - 1;       //��װ��ֵ
	TIM4->CCR3=0;       //�Ƚ�ֵ
	TIM4->CR1&=~(1<<4);   //���ϼ���
  TIM4->CCMR2|=(0x6<<4); //OC3ΪPWM1ģʽ
	TIM4->CCMR2&=~(0x3<<0); //OC3Ϊ���
	TIM4->CCER&=~(1<<9);    //OC3Ϊ�ߵ�ƽ��Ч
  TIM4->CR1|=(1<<7);      //����Ԥװ��
	TIM4->CCMR2|=(1<<3);   //OC3Ԥװ��
	TIM4->EGR|=(0x1);     //��������ʼ��
	TIM4->CCER|=(0x1<<8);    //ocʹ��
	TIM4->CR1|=1;      //ʹ�ܼ�����
	
	TIM3->CR1 |= 1;   
	TIM2->CR1 |= 1;             //ʹ�ܶ�ʱ��
}

#define SYS_CLK   72000000

void Systick_init(void)
{
	 if (SysTick_Config(SYS_CLK/1000))     //1ms
	{        
			while (1);
	}
	NVIC_SetPriority(SysTick_IRQn, 0x04);
}


uint32_t g_SysTick = 0;

uint32_t Sys_GetTick(void)
{
	return g_SysTick;
}


void SysTick_Handler()
{
	MainInterrupt();
	g_SysTick++;
}
