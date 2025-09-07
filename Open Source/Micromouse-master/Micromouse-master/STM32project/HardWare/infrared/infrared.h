#ifndef ADC_H
#define ADC_H

#include "stm32f10x.h"
#include "ZLG7289.h"

#define G_ODR_A   (GPIOA_BASE  + 0x0C)  
#define G_ODR_C   (GPIOC_BASE  + 0x0C)  
#define G_IDR_C   (GPIOC_BASE  + 0x08)  

#define Out_A(n)  BitBand(G_ODR_A,n)
#define Out_C(n)  BitBand(G_ODR_C,n)
#define In_C(n)   BitBand(G_IDR_C,n)

extern u16 ADC_MeasureBuf[6];

void Adc_Init(void);                    //adc1��ʼ��
void Dma_Init(void);                    //DMA1��ʼ��
void Infrared_Send_Init(void);          //���ⷢ����������IO��ʼ��
void Infrared_Send_Control(u8 led);     //���ⷢ������ܿ���
//void Dma_Start(void);                   //DMA����
void Adc_Transition(void);              //adc��һ��һ�����(����buf);
#endif

