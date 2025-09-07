/**
  ******************************************************************************
  * @file    odometer.c
  * @author  �Ŷ�
  * @version V1.0.0
  * @date    2019-9-28
  * @brief   ��̼Ƽ���
  ******************************************************************************
  */
/*include file ---------------------------------------------------------------*/
#include "odometer.h"
#include "pidctrller.h"
#include "attitude.h"
#include "InfraredCalc.h"
#include "OccupyingGrid.h"
#include "parameter.h"

#include "drive.h"
#include "stdio.h"

/*Global Date Space ----------------------------------------------------------*/

//��̼Ʋ���								//46179
const int32_t STDGRIDETR = ODOMETER_STD_GRID_ETR_VAL;//��׼դ��ı�������

//ƽ̨��λ�ú���̬
PosTypedef MousePos = { 
	.x  = 0 ,
	.y  = 0 ,
	.th = 90,
};

uint8_t g_StdGridUpdateFlag = 0;
/*----------------------------------------------------------------------------*/

//��ͼ����ϵ����Է���Ķ���
//����ƽ��ֱ������ϵ
/*	
		//������Ķ���
		
		y������
		|	
		|	  	     90 	
		|			180	 th  0
		|				   270
		|
		|
		|
		|��ʼ��
		------------------------ X ������
		x = p * cos(th)
		y = p * sin(th)
		p^2 = x^2 + y^2
		tan(th) = y/x
		
		cos(0)   =  1    sin(0)   =  0
		cos(90)  =  0    sin(90)  =  1
		cos(180) = -1    sin(180) =  0
		cos(270) =  0    sin(270) = -1

*/
/*Golobal data space --------------------------------------------------------*/
//�������ۻ�
int32_t g_OdEtrSum = 0;
//��̸�����������Сƫ������
float g_OdFixOffx = ODOMETER_STD_GRID_ETR_FIX;

//�趨��̼Ƹ�������ƫ����С����ֵ
void setOdometerFixOff(float num)
{
	g_OdFixOffx = num;
}
//��ȡƽ̨λ����Ϣ
PosTypedef getMousePos(void)
{
	return MousePos;
}

//ƽ̨��̬����
void MouseDirUpdate(int th)
{
	int16_t angel;
	
	angel = ((MousePos.th + th) < 0) ? 360 + th : MousePos.th + th;
	
	angel = angel % 360;
	
	MousePos.th = angel;
}

//�����������״̬
uint8_t getPosState(void)
{
	uint8_t temp = g_StdGridUpdateFlag;
	
	//Ϊ�������жϺͺ�̨�Ķ�ȡд���ͻ
	if(g_StdGridUpdateFlag == 1 && temp == 1)
	{
		g_StdGridUpdateFlag = 0;
	}
	
	return temp;
}

//�̶��Ƕȵ�����ֵ
int8_t xcos(int16_t th)
{
	switch(th)
	{
		case 0   : return  1;
		case 90  : return  0;
		case 180 : return -1;
		case 270 : return  0;
		default:while(1);
	}
}

//�̶��Ƕȵ�����ֵ
int8_t ysin(int16_t th)
{
	switch(th)
	{
		case 0   : return  0;
		case 90  : return  1;
		case 180 : return  0;
		case 270 : return -1;
		default:while(1);
	}
}

//ʹ�õ�ǰ��̬�޸����λ��
void odometerPosAdd(uint8_t num)
{
	MousePos.x += num * xcos(MousePos.th);
	MousePos.y += num * ysin(MousePos.th);
}
//������̼Ʊ�����
void setOdometerEtr(int32_t num)
{
	g_OdEtrSum = num;
}
//��ȡ��̼Ʊ�����
int32_t readOdometerEtr(void)
{
	return g_OdEtrSum;
}

//��������� ����̼�������㷢����������ʱ��λ
void odometerFatalErrorReset(void)
{
	//��д�����겻�Ϸ�ʱ ������ͼ��¼�����޷����ݵ���������
	if(MousePos.x > 15 || MousePos.x < 0 ||
		 MousePos.y > 15 || MousePos.y < 0)
	{
		//ϵͳ��λ
		NVIC_SystemReset();
	}
}


//��עդ���ۼ���̼�
void StandardGridCumulative(void)
{

	static uint8_t gridflag = 0;//���µ�ͼ��־λ
	//etrsum += getEncoderVal(0) + getEncoderVal(1);
	//����ֱ��ģʽ��Ч ��ת�������������̼�¼
	if(getAttitudeState() == ASStraight ||
		 getAttitudeState() == AFStraight)
	{
		//��¼����������
		g_OdEtrSum += getEncoderVal(0) + getEncoderVal(1);
		
		//ɨ�軷��
		if(g_OdEtrSum >= (int)(STDGRIDETR * ODOMETER_ENVIR_SIDE_START) && 
			 g_OdEtrSum <= (int)(STDGRIDETR * ODOMETER_ENVIR_SIDE_END) &&
			 gridflag == 0)
		{
			GridActSave(0);
			gridflag = 1;
		}
		
		//�ﵽ��׼դ������
		if(STDGRIDETR <= g_OdEtrSum)
		{
			//ɾ���Ѽ�¼����
			g_OdEtrSum -= STDGRIDETR;
			//��¼��̼�
			MousePos.x += 1 * xcos(MousePos.th);
			MousePos.y += 1 * ysin(MousePos.th);
			//λ�ø��±�־λ
			g_StdGridUpdateFlag = 1;
			//���µ�ͼ
			GridActSave(1);
			MapActivityRecord();
			gridflag = 0;
		}
		//�ֽ��ϰ�������̼Ƹ�������
		else if(getBarrierAct() && ((g_OdEtrSum > (int)(STDGRIDETR* g_OdFixOffx))))
		{
			//�ۻ�����
			g_OdEtrSum = 0;
			//��¼��̼�
			MousePos.x += 1 * xcos(MousePos.th);
			MousePos.y += 1 * ysin(MousePos.th);
			//λ�ø��±�־λ
			g_StdGridUpdateFlag = 1;
			//���µ�ͼ
			GridActSave(1);
			MapActivityRecord();
			gridflag = 0;
		}
	}
	//��̬��¼�������������м���
	else
	{
		g_OdEtrSum = 0;
	}
	//������
	odometerFatalErrorReset();
	
}

//��̼Ƽ�¼��Ƶ�� 
void ETR_Divider(void)
{
	static uint16_t div = 0;
	div++;
	if(div >= 10)
	{
		div = 0;
		StandardGridCumulative();
	}
}

//��̼��ж�ѭ��
void odometerLoop(void)
{
	ETR_Divider();
}










