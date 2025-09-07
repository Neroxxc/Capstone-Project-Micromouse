/**
  ******************************************************************************
  * @file    mouseApp.c
  * @author  �Ŷ�
  * @version V1.0.0
  * @date    2019-10-7
  * @brief   �ƶ�ƽ̨�߼�Ӧ��������
  ******************************************************************************
  */
/*include file ---------------------------------------------------------------*/
#include "mouseapp.h"
#include "drive.h"
#include "mouseFuncLib.h"
#include "ErgodicMap.h"
#include "myplanner.h"
#include "OccupyingGrid.h"
#include "MoveCtrl.h"
#include "InfraredCalc.h"
#include "navigator.h"
#include "attitude.h"
#include "mapSaver.h"
#include "parameter.h"
//C11key
//B12LED

/*Golbla data space ----------------------------------------------------------*/
MouseStateTypedef g_MAppSt 	= REDTEST; 	//������״̬��
uint32_t g_LEDFlashFreq 		= 100;				//LED��˸Ƶ�� 0�ǳ��� ������ֵ�Ǳ任�仯���

/*func -----------------------------------------------------------------------*/
void moveSPTest(void)
{
	//while(1)
	mouseMoveSP(MR90);
	mouseMove(MStop);
	while(1);
}




/*key enven -------------------------------------------------------------------*/
//��ʼ�����¼�
void startkeyEvent(void)
{
	if((GPIOC->IDR & (0x01 << 11)) == 0)
	{
		while((GPIOB->IDR & (0x01 << 11)) == 0);
		

		//��ʼ��Ѱ��ͼ
		g_MAppSt = MAPCREAT;
		
		g_LEDFlashFreq = 0;
		
		//�ȴ���Ѱ���Ե�ѡ��
		while(getInfraredEventVal() == 0);
		//��Ч����ѡ��
		switch(getInfraredEventVal())
		{
			case 0x01:setSearchPlant(0);break;
			case 0x02:setSearchPlant(1);break;
			case 0x04:setSearchPlant(2);break;
			case 0x08:setSearchPlant(2);break;
			default:break;
		}
		
		g_LEDFlashFreq = 100;
		
		delay_ms(2000);
		
		//��ʼ��դ���ͼ
		GridSetup();
		
//		moveSPTest();
//		TestFuncMoveFast();
//		TestFuncMoveFastTern();
//		ETR_Test();
//		while(1)
//		SendInfrared();
	}
}
//�̶�·����ʼ�¼�
void staticPathEvent(void)
{
	g_LEDFlashFreq = 0;
	if(getInfraredEventVal() != 0)
	{
		g_LEDFlashFreq = 100;
		delay_ms(2000);
		NavigatorStaticPath();
		while(1);
	}
}

//���Ͱ����¼�
void sendkeyEvent(void)
{
	if((GPIOC->IDR & (0x01 << 11)) == 0)
	{
		while((GPIOB->IDR & (0x01 << 11)) == 0);
		//SendMaoToUsart();
		delay_ms(2000);
//		moveSPTest();
		NavigatorFastII();
	}
}
	
//������Ϣ���
void clearkeyEvent(void)
{
	delay_ms(100);
	if((GPIOC->IDR & (0x01 << 11)) == 0)
	{
		g_LEDFlashFreq = 0;//LED����
		delay_ms(500);
		while((GPIOB->IDR & (0x01 << 11)) == 0);
		//���·��������ʹ�ܱ�־λ����λ
		if(getPathOverFlag()||getStaticPathFlag())
		{
			//���·��
			delPathofFlash();
		}
		//����SMC����
		else
		{
			sendSMCCode();
		}
		delay_ms(1000);
	}
}

/*Setup and Loop func body ---------------------------------------------------*/	
void mouseApp_Setup(void)
{
	//��ȡflash��������
	readFlashToBuf();
	
	//���������ͼ
	clearkeyEvent();
	
	//·������ʹ��
	if(getPathOverFlag())
	{
		//��ȡ·����Ϣ����·����Ϣд��·������
		writePathSeqFromFlash();
		//������̼Ƹ�������ģʽ
		setOdometerFixOff(ODOMETER_STD_GRID_FAST_FIX);
		//����ģʽ����̵ȴ�
		g_MAppSt = WAIT;
	}
	
	//�����þ�̬·��
	if(getStaticPathFlag())
	{
		//��ȡ·����Ϣ����·����Ϣд��·������
		writePathSeqFromFlash();
		//������̼Ƹ�������ģʽ
		setOdometerFixOff(ODOMETER_STD_GRID_FAST_FIX);
		//����ģʽ���̶�·��
		g_MAppSt = START;
	}
	
	UsartSendData("Sys_OK \r\n");
}

void mouseApp_Loop(void)
{
	switch(g_MAppSt)
	{
		case REDTEST://�ȴ���ʼ
		{
			g_LEDFlashFreq = 500;
			startkeyEvent();
			break;
		}
		case MAPCREAT://��ͼ����ģʽ
		{
			
			g_LEDFlashFreq = 1000;//��˸Ƶ�ʵ�500
			
			//������ͼ
			MapCreatLoop();
			
			g_LEDFlashFreq = 500;
			//·���滮
			routeSearcher();
			//����·��
			writePathToFlash();
			//������̼Ƹ�������ģʽ
			setOdometerFixOff(ODOMETER_STD_GRID_FAST_FIX);
			//���ģʽ
			NavigatorFastII();
			g_MAppSt = WAIT;
			break;
		}
		case WAIT://�ȴ�ģʽ
		{
			g_LEDFlashFreq = 100;
			sendkeyEvent();
			break;
		}
		case START://��ʼ�̶�·��
		{
			//�̶�·����ʼ�¼�
			staticPathEvent();
			break;
		}
	}
}
//�ж�
void mouseAppInterrupt(void)
{
	LEDFlash(g_LEDFlashFreq);
}
	
	
	
	
	
	