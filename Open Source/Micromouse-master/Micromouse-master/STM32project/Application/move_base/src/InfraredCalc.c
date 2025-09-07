/**
  ******************************************************************************
  * @file    InfraredCalc.c
  * @author  �Ŷ�
  * @version V1.0.0
  * @date    2019-9-22
  * @brief   ���⴫�������ݲɼ��������㷨�ͻ�����֪�����
  ******************************************************************************
  */
/*include file ---------------------------------------------------------------*/
#include "InfraredCalc.h"
#include "parameter.h"

#include "drive.h"

#include "stdio.h"

/*Golbal data space ----------------------------------------------------------*/
RedValTypedef g_RedSenVal;//����ɨ����ÿ20ms����һ��

/*ƫ������ȡ����----------------------------------------------------*/
const int16_t BASE_L = INFRAREEDCALC_BASE_L;			//�������� ������ʱ������
const int16_t BASE_R = INFRAREEDCALC_BASE_R;			//�Ҳ������ ������ʱ������

const int16_t MISS_L = INFRAREEDCALC_MISS_L - INFRAREEDCALC_BASE_L;			//���ƫ����ֹ��ֵ	���С�ڸ�ֵʱ�����ò��Ȩ�� -650 + 900
const int16_t MISS_R = INFRAREEDCALC_MISS_R	- INFRAREEDCALC_BASE_R;			//�Ҳ�ƫ����ֹ��ֵ	�Ҳ�С�ڸ�ֵʱ�����ò��Ȩ�� -730 + 900

const int16_t LOST_L = INFRAREEDCALC_LOST_L - INFRAREEDCALC_BASE_L;			//��໷����ⶪʧ��ֵ	���С�ڸ�ֵʱ�����ò��Ȩ�� -650 + 900
const int16_t LOST_R = INFRAREEDCALC_LOST_R - INFRAREEDCALC_BASE_R;			//�Ҳ໷����ⶪʧ��ֵ	�Ҳ�С�ڸ�ֵʱ�����ò��Ȩ�� -730 + 900

/*�ֽ�����ȡ����---------------------------------------------------*/
const int16_t OFFSET_LF = INFRAREEDCALC_OFFSET_LF;		//���ƫ��
const int16_t OFFSET_RF = INFRAREEDCALC_OFFSET_RF;		//�Ҳ�ƫ��

const int16_t ACTVAL_LF = INFRAREEDCALC_ACTVAL_LF;	//��༤��ֵ
const int16_t ACTVAL_RF = INFRAREEDCALC_ACTVAL_RF;	//�Ҳ༤��ֵ

const int16_t THRESHOLDS = INFRAREEDCALC_THRESHOLDS;	//����ֵ0
/*------------------------------------------------------------------*/



#define RED_DELAY delay_us(1)

//�����Χ��������ֵ
RedValTypedef getRedVal(void)
{
	RedValTypedef red;
	
	//��û���ֵ
	Adc_Transition();
	red.LS = ADC_MeasureBuf[2];
	red.RS = ADC_MeasureBuf[3];
	red.LF = ADC_MeasureBuf[0];
	red.RF = ADC_MeasureBuf[1];
	
	
	//���������������ֵ
	Infrared_Send_Control(LS); 
	RED_DELAY;
	Adc_Transition();
	red.LS = ADC_MeasureBuf[2] - red.LS;
	red.LS = (red.LS < 4096) ? red.LS : 0;
	
	
	Infrared_Send_Control(RS); 
	RED_DELAY;
	Adc_Transition();
	red.RS = ADC_MeasureBuf[3] - red.RS;
	red.RS = (red.RS < 4096) ? red.RS : 0;
	
	Infrared_Send_Control(LF);
	RED_DELAY;
	Adc_Transition();
	red.LF = ADC_MeasureBuf[0] - red.LF;
	red.LF = (red.LF < 4096) ? red.LF : 0;
	
	Infrared_Send_Control(RF); 
	RED_DELAY;
	Adc_Transition();
	red.RF = ADC_MeasureBuf[1] - red.RF;
	red.RF = (red.RF < 4096) ? red.RF : 0;

	//�ر�ȫ��������
	Infrared_Send_Control(0);
	
//	uint8_t str[50];
//	sprintf((char *)str,"%d,%d,%d,%d \r\n",red.LS,red.RS,red.LF,red.RF);
//	UsartSendData(str); 
//	
	return red;
}


//���ƫ����
int16_t getRedOffset(void)
{
	//����ԭ��
	//ƫ������ = ������ - �����ֵ
	//ƫ������ = �Ҳ���� - �Ҳ���ֵ
	//ƫ���� = ƫ������ - ƫ������
	
	
	
	int16_t lift_val  = g_RedSenVal.LS - BASE_L;
	int16_t right_val = g_RedSenVal.RS - BASE_R;
	
	int16_t val = 0;
	
	val = lift_val - right_val;
	
	//�����޷����Ļ�����֪�����������
	//������һ�෵��ֵ���ص�������ֵ���϶�Ϊ�޷���
	//���ԣ�ƫ������ֻ��һ�����Ȩ��
	//���������෵��ֵ����������ֵ
	//���ԣ�λ�õ�ǰ��̬�����ı�
	if(lift_val < MISS_L)//���ʧȥ����
	{
		val = -right_val;
	}
	if(right_val < MISS_R)//�Ҳ�ʧȥ����
	{
		val = lift_val;
	}
	if((lift_val < MISS_L) && (right_val < MISS_R))//ͬʱʧȥ����
	{
		val = 0;
	}

	
	
//	uint8_t str[50];
//	sprintf((char *)str,"%d,%d     ",lift_val,right_val);
//	UsartSendData(str); 
//	sprintf((char *)str,"%d\r\n",val);
//	UsartSendData(str); 
//	LEDDisplay(lift_val,right_val);
//	LEDDisplay(val,0);
	
	return val;
}



//�ϰ���ֽ���������
uint8_t getBarrierAct(void)
{
	//Ԥ���� ����ֵ��ƫ��
	//��������
	//������������Ҳ�����ڼ���ֵ
	//��ֵ�ʹ��ں���ֵ
	
	int16_t adcVal_LF = g_RedSenVal.LF + OFFSET_LF;
	int16_t adcVal_RF = g_RedSenVal.RF + OFFSET_RF;
	
	if( ((adcVal_LF > ACTVAL_LF) && ((adcVal_LF + adcVal_RF) > THRESHOLDS)) ||
			((adcVal_RF > ACTVAL_RF) && ((adcVal_LF + adcVal_RF) > THRESHOLDS))
			)
	{
		return 1;
	}
	return 0;
}



//��õ�ǰ����Ŀɻ��Ϣ
uint8_t getActDirState(void)
{
	//����״̬�ö����Ʊ�ʾ��������Ŀɻ��
	//0000 0LFR L��� Fǰ�� R�Ҳ�
	const uint16_t LFACTVAL = INFRAREEDCALC_EN_LFACTVAL;	//��ǰ������ֵ
	const uint16_t RFACTVAL = INFRAREEDCALC_EN_RFACTVAL;	//��ǰ������ֵ
	const uint16_t FSACTVAL = INFRAREEDCALC_EN_FSACTVAL;	//ǰ�ͼ�����ֵ
	
	int16_t lift_val  = g_RedSenVal.LS - BASE_L;
	int16_t right_val = g_RedSenVal.RS - BASE_R;
	
	uint8_t actdir = 0x00;
	
	if(lift_val < LOST_L)//���ʧȥ����
	{
		actdir |= 0x04;
	}
	if(right_val < LOST_R)//�Ҳ�ʧȥ����
	{
		actdir |= 0x01;
	}
	
	int16_t redsum = g_RedSenVal.LF + g_RedSenVal.RF;
	//ǰ������
	if((g_RedSenVal.LF > LFACTVAL && (redsum > FSACTVAL)) ||
		 (g_RedSenVal.RF > RFACTVAL && (redsum > FSACTVAL)) )
	{
		actdir |= 0x02;
	}
	return actdir;
}
//int16_t pianyizhi = 0;
void InfraredLoop(void)
{
	static uint32_t div = 0;
	div++;
	if(div >= 10)
	{
		div = 0;
		g_RedSenVal = getRedVal();
		//pianyizhi = getRedOffset();
	}
}


////��þͼ����ֽ�1/2դ���״̬
//uint8_t getHalfGridIrEn(void)
//{
//	const uint16_t LFACTIVE =  200;
//	const uint16_t RFACTIVE =  400;
//	const uint16_t ENSUM =  600;
//	
//	const uint16_t LFDIS =  500;
//	const uint16_t RFDIS =  1400;
//	const uint16_t ENDIS =  1600;
//	
//	//ǰ��������Ϊ������׼
//	if((g_RedSenVal.LF > LFACTIVE && (g_RedSenVal.LF + g_RedSenVal.RF) > ENSUM)||
//		 (g_RedSenVal.RF > RFACTIVE && (g_RedSenVal.LF + g_RedSenVal.RF) > ENSUM))
//	{
//		//�ִ�Ҫ��λ��
//		if((g_RedSenVal.LF > LFDIS && (g_RedSenVal.LF + g_RedSenVal.RF) > ENDIS)||
//			(g_RedSenVal.RF > RFDIS && (g_RedSenVal.LF + g_RedSenVal.RF) > ENDIS))
//		
//		{
//			return 0;
//		}
//		return 1;
//	}
//	//������Ϊ����
//	return 0;
//	
//}

//��ú��ⴥ�����
uint8_t getInfraredEventVal(void)
{
	//���ⴥ��λ
	uint8_t actbit = 0;
	if(g_RedSenVal.LF > 2800)actbit |= 0x01;
	if(g_RedSenVal.LS > 2800)actbit |= 0x02;
	if(g_RedSenVal.RS > 3700)actbit |= 0x04;
	if(g_RedSenVal.RF > 2800)actbit |= 0x08;
	return actbit;
}













