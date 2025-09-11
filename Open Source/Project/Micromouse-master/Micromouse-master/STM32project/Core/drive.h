#ifndef DRIVE_H
#define DRIVE_H
#include "stm32f10x.h"
#include "rcc.h"
#include "nvic.h"
#include "dian_ji.h"
#include "infrared.h"
#include "ZLG7289.h"
#include "led.h"
#include "delay.h"
#include "drive.h"
#include "usart.h"
#include "time.h"


extern u16 ADC_MeasureBuf[6];     //buf[0]-buf[3]:������ն���U1-U4;  buf[4]: TEMP�¶�AD; buf[5]:RDOUT SUMJ ���ٶ�AD
extern u16 cnt1,cnt2 ;                  //���1��2 ��10ms��ı���������   cnt1:'z'    cnt2:'y';  ����0xffff

//����
void DriveInit(void);                     


//**************��ʱ�������
void delay_ms(u32 ms);                  //ms��ʱ
void delay_us(u32 us);                  //us��ʱ
//**************

//**************������
void DianJi_TiaoSu(u8 wei,int su);       //���pwm���� 
u8 Dian_Ji_Direction(u8 n);             //�������ɼ� n:'z':������ߵ������, 'y':�����ұߵ������;   0:ǰ 1:��
//**************

//**************�������
void Infrared_Send_Control(u8 led);     //���ⷢ������ܿ���
void Adc_Transition(void);              //adc��һ��һ�����(����buf);
//**************

//**************��������
void Zlg7289_Write_Data(u8 instruct,u8 data);   //����ܵ���
void LEDDisplay(int16_t dat1,int16_t dat2);
//**************

//**************�������
void UsartSendData(u8 * tmp); 						//�����ַ�������

//***************ϵͳ����ʱ���
uint32_t Sys_GetTick(void);

//*****************
#endif

