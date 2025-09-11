/**
  ******************************************************************************
  * @file    navigator.c
  * @author  ��10
  * @version V1.0.0
  * @date    2019-10-13
  * @brief   �ƶ�������·��ת����س��� ͨ�����������п���ƽ̨�ƶ�
  ******************************************************************************
  */
	
/*include file ---------------------------------------------------------------*/
#include "navigator.h"
#include "movectrl.h"
#include "odometer.h"
#include "myPlanner.h"
#include "attitude.h"
#include "parameter.h"
#include "InfraredCalc.h"

/*Golbal Data Space ----------------------------------------------------------*/


/*=================================�ƶ��������԰�==============================*/

//�������������ƶ�
void moveNavigator(uint8_t dir,PosTypedef mp,void (* func)())
{
	//������̬
	switch(dir)
	{
		case 0x01:
			switch (mp.th)
			{
				case 90  :func(MR90);break;
				case 180 :func(ML180);break;
				case 270 :func(ML90);break;
			}break;
		case 0x02:
			switch (mp.th)
			{
				case 0   :func(ML90);break;
				case 180 :func(MR90);break;
				case 270 :func(ML180);break;
			}break;
		case 0x04:
			switch (mp.th)
			{
				case 0   :func(ML180);break;
				case 90  :func(ML90);break;
				case 270 :func(MR90);break;
			}break;
		case 0x08:
			switch (mp.th)
			{
				case 0   :func(MR90);break;
				case 90  :func(ML180);break;
				case 180 :func(ML90);break;
			}break;
	}

}

//ͨ����һ��Ŀ�����㷽��
uint8_t navPosToDir(PosTypedef mp,PosTypedef nmp)
{
	if(mp.x > nmp.x)return 0x04;
	if(mp.y > nmp.y)return 0x08;
	if(mp.x < nmp.x)return 0x01;
	if(mp.y < nmp.y)return 0x02;
	return 0;
}

//�ƶ��������԰�
void NavigatorTest(void)
{
	uint8_t cursor = 0;
	
	
	//ǰ���յ�
	while(getMousePos().x != getTargetPos().x ||
				getMousePos().y != getTargetPos().y)
	{
		//��ý������Ļ����
		PosTypedef mp = getMousePos();
		PosTypedef nmp;
		
		nmp.x = getRoute(cursor).x;
		nmp.y = getRoute(cursor).y;
		
		uint8_t dir = navPosToDir(mp,nmp);
		
		//����
		moveNavigator(dir,mp,mouseMoveFastI);
		//ǰ��
		mouseMoveFastI(MForward);
		//�ȴ��ִ�
		while(getMousePos().x != getRoute(cursor).x ||
		      getMousePos().y != getRoute(cursor).y);
		
		cursor++;
	}
	
	
	//�������
	cursor -= 2;
	while(getMousePos().x != getRoute(0).x ||
				getMousePos().y != getRoute(0).y)
	{
		//��ý������Ļ����
		PosTypedef mp = getMousePos();
		PosTypedef nmp;
		
		nmp.x = getRoute(cursor).x;
		nmp.y = getRoute(cursor).y;
		
		
		uint8_t dir = navPosToDir(mp,nmp);
		
		//�˶�
		moveNavigator(dir,mp,mouseMoveFastI);
		//ǰ��
		mouseMoveFastI(MForward);
		//�ȴ��ִ�
		while(getMousePos().x != getRoute(cursor).x ||
		      getMousePos().y != getRoute(cursor).y);
		
		cursor--;
	}
	
	
	//��ý������Ļ����
	PosTypedef mp = getMousePos();
	PosTypedef nmp;
	
	nmp.x = 0;
	nmp.y = 0;
	
	uint8_t dir = navPosToDir(mp,nmp);
	
	//�˶�
	moveNavigator(dir,mp,mouseMoveFastI);
	//ǰ��
	mouseMoveFastI(MForward);
	//�ȴ��ִ�
	while(getMousePos().x != 0||
				getMousePos().y != 0);
	
	mouseMove(MStop);
	mouseMove(ML180);
	mouseMove(MStop);
}
//�ƶ�������̬·��
void NavigatorStaticPath(void)
{
	uint8_t cursor = 0;
	
	//ǰ���յ�
	while(getRoute(cursor).x != 99 ||
				getRoute(cursor).y != 99)
	{
		//��ý������Ļ����
		PosTypedef mp = getMousePos();
		PosTypedef nmp;
		
		nmp.x = getRoute(cursor).x;
		nmp.y = getRoute(cursor).y;
		
		uint8_t dir = navPosToDir(mp,nmp);
		
		//����
		moveNavigator(dir,mp,mouseMoveFastI);
		//ǰ��
		mouseMoveFastI(MForward);
		//�ȴ��ִ�
		while(getMousePos().x != getRoute(cursor).x ||
		      getMousePos().y != getRoute(cursor).y);
		//�ƶ����
		cursor++;
	}
	mouseMove(MStop);
}
/*================================�ƶ�����������I==============================*/

//���ø���ֱ�߿�����
void setLineFastI(uint8_t dis)
{
	setFastStMoveGridNum(dis);
	AttitudeStateSet(AFStraight);
}
//�������֮��ľ���
uint8_t getDisGrid(PosTypedef mp,PosTypedef nmp)
{
	uint8_t dis = 0;
	
	//X������ͬ
	if(mp.x == nmp.x)
	{
		if(mp.y >nmp.y)
		{
			dis = mp.y - nmp.y;
		}
		else if(mp.y < nmp.y)
		{
			dis = nmp.y - mp.y;
		}
	}
	
	//y������ͬ
	else if(mp.y == nmp.y)
	{
		if(mp.x > nmp.x)
		{
			dis = mp.x - nmp.x;
		}
		else if(mp.x < nmp.x)
		{
			dis = nmp.x - mp.x;
		}
	}
	return dis;
}

//�ƶ��������ٵ�һ�汾
void NavigatorFastI(void)
{
	uint8_t cursor = 0;
	
	TIM4->CCR3 = 600;
	
	//ǰ���յ�
	while(getMousePos().x != getTargetPos().x ||
				getMousePos().y != getTargetPos().y)
	{
		//��ý������Ļ����
		PosTypedef mp = getMousePos();
		PosTypedef nmp;
		
		nmp.x = getRoute(cursor).x;
		nmp.y = getRoute(cursor).y;
		
		uint8_t dir = navPosToDir(mp,nmp);
		
		//��̬����
		moveNavigator(dir,mp,mouseMoveFastI);
		//ֱ���˶�
		setLineFastI(getDisGrid(mp,nmp));
		
		//�ȴ��ִ�
		while(getMousePos().x != getRoute(cursor).x ||
		      getMousePos().y != getRoute(cursor).y);
		
		cursor++;
	}
	
	
	//�������
	cursor -= 2;
	while(getMousePos().x != getRoute(0).x ||
				getMousePos().y != getRoute(0).y)
	{
		//��ý������Ļ����
		PosTypedef mp = getMousePos();
		PosTypedef nmp;
		
		nmp.x = getRoute(cursor).x;
		nmp.y = getRoute(cursor).y;
		
		
		uint8_t dir = navPosToDir(mp,nmp);
		
		//��̬����
		moveNavigator(dir,mp,mouseMoveFastI);
		//ֱ���˶�
		setLineFastI(getDisGrid(mp,nmp));
		
		//�ȴ��ִ�
		while(getMousePos().x != getRoute(cursor).x ||
		      getMousePos().y != getRoute(cursor).y);
		
		cursor--;
	}
	
	
	//��ý������Ļ����
	PosTypedef mp = getMousePos();
	PosTypedef nmp;
	
	nmp.x = 0;
	nmp.y = 0;
	
	uint8_t dir = navPosToDir(mp,nmp);
	
	//��̬����
	moveNavigator(dir,mp,mouseMoveFastI);
	//ֱ���˶�
	setLineFastI(getDisGrid(mp,nmp));
	
	//�ȴ��ִ�
	while(getMousePos().x != 0||
				getMousePos().y != 0);
	
	mouseMove(MStop);
	mouseMove(ML180);
	mouseMove(MStop);
	
	TIM4->CCR3 = 0;
}

/*================================�ƶ��������ٰ�I==============================*/


//�ƶ��������ٵڶ��汾
void NavigatorFastII(void)
{
	uint8_t cursor = 0;
	
	
	
	//���ٵڶ����ƶ�����·����Ϊ�����׶�
	//1.��㵽��һ��������
	//2.��x�������㵽��x+1������
	//3.��n-1�������㵽�յ�-0.5��׼դ��
	
	cursor++;
	//��һ�׶δ���㵽��һ��������
	{
		//��ý������Ļ����
		PosTypedef mp = getMousePos();
		PosTypedef nmp;
		
		nmp.x = getRoute(cursor).x;
		nmp.y = getRoute(cursor).y;
		
		uint8_t dir = navPosToDir(mp,nmp);
		
		
		//��̬����
		moveNavigator(dir,mp,mouseMoveFastI);
		
		//ֱ���˶�
		setLineFastI(getDisGrid(mp,nmp));
		
		TIM4->CCR3 = 400;//�򿪷���
		
		//�ȴ��ִ��һ��������ǰ1/2����׼դ��
		while(getDisGrid(getMousePos(),nmp) != 1);
		TIM4->CCR3 = 800;//�򿪷���
		while(readOdometerEtr() < ODOMETER_STD_GRID_ETR_VAL* ATTITUDE_FROTATION_TPOS &&
					getBarrierAct() == 0);
		
		//�ִ��һ���ٽ��
	}
	
	TIM4->CCR3 = 400;//�򿪷���
	
	//�ڶ��׶� �ӵ�x�������㵽��x+1��������
	{
		//�����ִ�ĵ����㲻���յ�
		while( getRoute(cursor).x != getTargetPos().x ||
					 getRoute(cursor).y != getTargetPos().y  )
		{
			//�����ִ�ĵ�������Ϊ��ǰλ�û�ÿ��ƶ�����
			PosTypedef mp = getMousePos();
			PosTypedef nmp;
			
			mp.x = getRoute(cursor).x;
			mp.y = getRoute(cursor).y;
			
			nmp.x = getRoute(cursor + 1).x;
			nmp.y = getRoute(cursor + 1).y;
			
			uint8_t dir = navPosToDir(mp,nmp);
			
			//��̬����
			moveNavigator(dir,mp,mouseMoveSP);
//			mouseMove(MStop);
//			while(1);
			//ֱ���˶�
			setLineFastI(getDisGrid(mp,nmp));
			
			TIM4->CCR3 = 400;//�򿪷���
			
			//�ȴ��ִ��һ��������ǰ1/2����׼դ��
			while(getDisGrid(getMousePos(),nmp) != 1);
			
			TIM4->CCR3 = 700;//�򿪷���
			
			while(readOdometerEtr() < ODOMETER_STD_GRID_ETR_VAL* ATTITUDE_FROTATION_TPOS &&
						getBarrierAct() == 0);
			
//			mouseMove(MStop);
//			delay_ms(1000);
				
			
//			while(0);
			
			cursor++;
		}
	}
	
	//�����׶� ��n-1�������㵽�յ�-0.5��׼դ��
	{
//		//��ý������Ļ����
//		PosTypedef mp = getMousePos();
//		PosTypedef nmp = getTargetPos();
//		
//		//ֱ���˶�
//		setLineFastI(getDisGrid(mp,nmp));
		
		//�ȴ��ִ�
		while(getMousePos().x != getTargetPos().x ||
		      getMousePos().y != getTargetPos().y);
		
		mouseMoveFastI(MStop);
		//while(1);
	}
	
	/*----------------------------------------------------------------�������*/
	TIM4->CCR3 = 0;
		//�������
	cursor--;
	while(getMousePos().x != getRoute(0).x ||
				getMousePos().y != getRoute(0).y)
	{
		//��ý������Ļ����
		PosTypedef mp = getMousePos();
		PosTypedef nmp;
		
		nmp.x = getRoute(cursor).x;
		nmp.y = getRoute(cursor).y;
		
		
		uint8_t dir = navPosToDir(mp,nmp);
		
		//��̬����
		moveNavigator(dir,mp,mouseMoveFastI);
		//ֱ���˶�
		setLineFastI(getDisGrid(mp,nmp));
		
		//�ȴ��ִ�
		while(getMousePos().x != getRoute(cursor).x ||
		      getMousePos().y != getRoute(cursor).y);
		
		cursor--;
	}
	
	
	//��ý������Ļ����
	PosTypedef mp = getMousePos();
	PosTypedef nmp;
	
	nmp.x = 0;
	nmp.y = 0;
	
	uint8_t dir = navPosToDir(mp,nmp);
	
	//��̬����
	moveNavigator(dir,mp,mouseMoveFastI);
	//ֱ���˶�
	setLineFastI(getDisGrid(mp,nmp));
	
	//�ȴ��ִ�
	while(getMousePos().x != 0||
				getMousePos().y != 0);
	
	mouseMove(MStop);
	mouseMove(ML180);
	mouseMove(MStop);
	
	
}







