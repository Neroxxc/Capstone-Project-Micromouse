#include "dian_ji.h"
#include "ZLG7289.h"

u16 cnt1 = 0,cnt2 = 0;        //46 9 25mm


void Pwm_Init(TIMX *TIM)
{
	RCC->APB2ENR |= (1<<2);     //��GPIOAʱ��
	RCC->APB2ENR |= (1<<11);    //��TIM1ʱ��	
	GPIOA->CRH &= ~(0xffff);
	
	GPIOA->CRH |= (0xbbbb);     //PA8 9 10 11���츴�����50M
	
	TIM1->SMCR &= ~0x7;         //ѡ���ڲ�ʱ��
	
	TIM1->PSC = TIM->psc;       //Ԥ��Ƶ
	TIM1->ARR = TIM->arr;       //��װ��ֵ
	
	TIM1->CCR1 = TIM->ccr1;     //�Ƚ�ֵ  
	TIM1->CCR2 = TIM->ccr2;    
	TIM1->CCR3 = TIM->ccr3;
	TIM1->CCR4 = TIM->ccr4;
	
	TIM1->CR1 &= ~(1<<4);       //���ϼ���
	TIM1->CR1|=(1<<7);          //����Ԥװ��

	
	TIM1->CCMR1|=(0x6<<4);      //OC1ΪPWM1ģʽ
	TIM1->CCMR1&=~(0x3<<0);     //OC1Ϊ���
	TIM1->CCER &= ~(1<<1);      //OC1Ϊ�ߵ�ƽ��Ч
	
  TIM1->CCMR1 |= (0x6<<12);   //OC2ΪPWM1ģʽ
	TIM1->CCMR1&=~(0x3<<8);     //OC2Ϊ���
	TIM1->CCER &= ~(1<<5);      //OC2Ϊ�ߵ�ƽ��Ч
	
	TIM1->CCMR2|=(0x6<<4);      //OC3ΪPWM1ģʽ
	TIM1->CCMR2&=~(0x3<<0);     //OC3Ϊ���
	TIM1->CCER &= ~(1<<9);      //OC3Ϊ�ߵ�ƽ��Ч
	
	TIM1->CCMR2|=(0x6<<12);     //OC4ΪPWM1ģʽ
	TIM1->CCMR1&=~(0x3<<8);     //OC4Ϊ���
	TIM1->CCER &= ~(1<<13);     //OC4Ϊ�ߵ�ƽ��Ч
	
	TIM1->CCMR1|=(1<<11);       //OC2Ԥװ��ʹ��
	TIM1->CCMR1|=(1<<3);        //OC1Ԥװ��ʹ��
	TIM1->CCMR2|=(1<<11);       //OC4Ԥװ��ʹ��
	TIM1->CCMR2|=(1<<3);        //OC3Ԥװ��ʹ��
	
	TIM1->EGR|=(0x1);           //��������ʼ��
	
	
  TIM1->CCER|=(0x1<<4);      //oc2ʹ��
	TIM1->CCER|=(0x1<<0);      //oc1ʹ��
	TIM1->CCER|=(0x1<<8);      //oc3ʹ��
	TIM1->CCER|=(0x1<<12);     //oc4ʹ��
	
	TIM1->CR1|=1;              //ʹ�ܼ�����	
	TIM1->BDTR|=(1<<15);       //��ֹɲ������������OCͨ��
}

void Dian_Ji_Init(void)
{
	TIMX TIM;
	
	TIM.arr = 999;
	TIM.psc = 8;
	TIM.ccr1 = 0;
	TIM.ccr2 = 0;
	TIM.ccr3 = 0;
	TIM.ccr4 = 0;
	
	Pwm_Init(&TIM);
}


void Dian_Ji_Read()
{
	
	RCC->APB2ENR |= (1<<2);
	
	GPIOA->CRL &= 0x00ffff00;
	GPIOA->CRL |= 0x44000044;     //��������
	
	RCC->APB1ENR |= 0x1;          //��TIM2 3ʱ��
	RCC->APB1ENR |= 0x2;
	
	TIM3->SMCR &= ~0x7;
	TIM2->SMCR &= ~0x7;         //ѡ���ڲ�ʱ��
	
	TIM3->CR1 &= ~(1<<4);
	TIM2->CR1 &= ~(1<<4);       //���ϼ���
	TIM3->PSC =  0;            
	TIM2->PSC =  0;             //Ԥ��Ƶ
	TIM3->ARR = 0xffff;   
	TIM2->ARR = 0xffff;         //��װ��ֵ
	TIM3->SMCR |= 0x2;      
	TIM2->SMCR |= 0x2;          //������ģʽ2
	
	TIM3->CCMR1 |= 0x1;         
	TIM2->CCMR1 |= 0x1;         //IC1 -> TI1
	TIM3->CCMR1 |= (0x1<<8);    
	TIM2->CCMR1 |= (0x1<<8);    //IC2 -> TI2
	TIM3->CCMR1 |= (0x3<<4);
	TIM2->CCMR1 |= (0x3<<4);
	TIM3->CCMR1 |= (0x3<<12); 
	TIM2->CCMR1 |= (0x3<<12);   //ͨ��һ���˲��˴�
	
	TIM3->CCER |=  (1<<1);      //TIM3 IC1����
	
	TIM2->CCER &= ~(1<<1);
	TIM3->CCER &= ~(1<<5); 
	TIM2->CCER &= ~(1<<5);       //ͨ��1 2������
	
	TIM3->CR1|=(1<<7);  
	TIM2->CR1|=(1<<7);          //Ԥװ��
	
	TIM2->SR = 0x0000;
	//TIM3->CR1 |= 1;   
	//TIM2->CR1 |= 1;             //ʹ�ܶ�ʱ��
}

void DianJi_TiaoSu(u8 wei,int su)
{
	switch(wei)
	{
		case 0x03:if(su>=0)
		           { 
			           TIM1->CCR1 = 0;TIM1->CCR2 = su;break;		
							 }
							else
							{
								 TIM1->CCR2 = 0;TIM1->CCR1 = -su;break;
							}
		case 0x0a:if(su>=0)
		           {
								TIM1->CCR3 = 0;TIM1->CCR4 = su;break;
							}
							else
							{
								TIM1->CCR4 = 0;TIM1->CCR3 = -su;break;
							}
		case 0x01:TIM1->CCR1 = TIM1->CCR2 = 0;break;
		case 0x11:TIM1->CCR1 = TIM1->CCR2 = 1000;break;
		case 0x04:TIM1->CCR3 = TIM1->CCR4 = 0;break;
		case 0x14:TIM1->CCR3 = TIM1->CCR4 = 1000;break;
		default:break;
	}
}



u8 Dian_Ji_Direction(u8 n)
{
	if(n == 'z')
	{
		if( (TIM2->CR1 & 0x10)!= 0)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(n == 'y')
	{
		if( (TIM3->CR1& 0x10)!= 0)
		{
			return 0x01;
		}
		else
		{
			return 0x00;
		}
	}
	else
	{
		return 12;
	}
}

void getEtrVal(void)
{
	 TIM3->CR1 &= ~0x1;   
	 TIM2->CR1 &= ~0x1;          //ʧ�ܶ�ʱ��

	 cnt1 = TIM2->CNT;           //�ɼ�����
	 cnt2 = TIM3->CNT;
	 TIM2->CNT = 0;
	 TIM3->CNT = 0;
	 
	 TIM4->CR1 |= 1;            //��tim4	

	 TIM3->CR1 |= 1;   
	 TIM2->CR1 |= 1;            //ʹ�ܶ�ʱ��
}

