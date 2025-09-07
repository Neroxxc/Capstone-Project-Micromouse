#include "stm32f10x.h"
#include "delay.h"
#include "time.h"
/*************
��ʱ������ʼ��
*************/

void delay_ms(u32 ms)
{
//	u32 temp;
//	
//	SysTick->LOAD = 9000*ms;//��װ�ظ�ֵ  ע�� 9 : = 72M/8 ;
//	                        //����ϵͳʱ��Ϊ24M ���72M��3000��Ϊ 72/8;
//	SysTick->VAL = 0x00;//��ռ�������
//	SysTick->CTRL = 0x01;//ʹ�ܲ� �ⲿʱ��
//	do
//	{
//		temp=SysTick->CTRL;
//	}while((temp&0x01)&&(!(temp&(1<<16))));//�ȴ�����0
//	SysTick->CTRL = 0x00;//�ر�
//	SysTick->VAL = 0x00;//���
	
	uint32_t lasttick = Sys_GetTick();
	while((Sys_GetTick() - lasttick) < ms);
	
}

void delay_us(u32 us)
{
//	u32 temp;
//	
//	//��msһ��
//	
//	SysTick->LOAD = 9*us;
//	SysTick->VAL = 0x00;
//	SysTick->CTRL = 0x01;
//	do
//	{
//		temp=SysTick->CTRL;
//	}while((temp&0x01)&&(!(temp&(1<<16))));
//	SysTick->CTRL = 0x00;
//	SysTick->VAL = 0x00;
	u16 i=0;  
   while(us--)
   {
      i=5;  //�Լ�����
      while(i--) ;    
   }
}	

