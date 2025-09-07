#ifndef DIAN_JI_H
#define DIAN_JI_H
#include "stm32f10x.h"

extern u16 cnt1,cnt2 ;            //���1��2 ��10ms��ı���������


typedef struct                    //TIM1PWM��ʼ���ṹ��
{
	u16 psc;
	u16 arr;
	u16 ccr1;
	u16 ccr2;
	u16 ccr3;
	u16 ccr4;
	
}TIMX;
 
void Pwm_Init(TIMX *TIM);           //TIM1��ʼ��
void Dian_Ji_Init(void);            //���PWM�����ʼ��
void Dian_Ji_Read(void);            //�����������ʼ��
void DianJi_TiaoSu(u8 wei,int su);   //�������
void Dian_Ji_T(void);               //������ݲɼ�ʱ���ʼ��(10ms)
u8 Dian_Ji_Direction(u8 n);         //�������ɼ�

#endif

