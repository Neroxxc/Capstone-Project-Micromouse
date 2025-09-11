/**
  ******************************************************************************
  * @file    ErgodicMap.c
  * @author  �Ŷ�
  * @version V1.0.0
  * @date    2019-10-5
  * @brief   ��ͼ��ɨ���뽨��
  ******************************************************************************
  */
/*include file ---------------------------------------------------------------*/
#include "ErgodicMap.h"
#include "MoveCtrl.h"
#include "odometer.h"
#include "OccupyingGrid.h"
#include "parameter.h"
#include "myPlanner.h"
#include "navigator.h"
#include "mapSaver.h"

#include "math.h"
/*Golbal data space ----------------------------------------------------------*/
//�ɻ�����
uint8_t g_actdir[255];
//�������� 0x 1y
uint8_t g_actpos[255][2];
//���ݴ洢���
uint8_t g_actCur = 0;

//ɨ��·�� 0x 1y
PosTypedef g_pathPos[255];
//·�����
uint16_t g_pathCur = 0;

//��Ѱ�ƻ�
uint16_t g_SearchPlant = ERGODICMAP_SEARCHOR_PLAN;

//������Ѱ�ƻ�
void setSearchPlant(uint8_t p)
{
	g_SearchPlant = p;
}


//�ִ��յ�
uint8_t getOver(void)
{
	//����Ϊ�յ������
	const PosTypedef tg[4] = {
		ERGODICMAP_TARGET_POS_1	,
		ERGODICMAP_TARGET_POS_2	,  
		ERGODICMAP_TARGET_POS_3	, 
		ERGODICMAP_TARGET_POS_4 ,
	};
	
	PosTypedef mp = getMousePos();
	
	for(int i = 0;i < 4;i++)
	{
		if(	mp.x == tg[i].x &&
				mp.y == tg[i].y	)
		{
			//����һ���ִ�ĵ���ΪĿ���
			setTargetPos(tg[i]);
			return 1;
		}
	}
	return 0;
}

//��ͼ��������ж�
//���ȫ���ɷ��ʵ���ѷ�����
//����Ӧ�κγߴ��Լ���״�ĵ�ͼ
uint8_t getMapOver(void)
{
	/*
				L4B 					Active	Inactive
				|---B0	0��     1				0
				|---B1  90��    1        0
				|---B2	180��   1        0
				|---B3	270��	 1        0
	*/
	
	uint8_t dir = 0;
	for(int x = 0;x < 16;x++)
	{
		for(int y = 0;y < 16;y++)
		{
			//��ȡ�ɻ����
			dir = readMapGrid(x,y);
			
			//���пɻ��������δ֪�㼴Ϊ���
			if(dir != 0)
			{
				if((dir & 0x01)!=0)//0
				{
					if(x != 15)
					{
						if(readMapGrid(x + 1,y) == 0)
						{
							return 0;
						}
					}
				}
				if((dir & 0x02)!=0)//90
				{
					if(y != 15)
					{
						if(readMapGrid(x,y + 1) == 0)
						{
							return 0;
						}
					}
				}
				if((dir & 0x04)!=0)//180
				{
					if(x != 0)
					{
						if(readMapGrid(x - 1,y) == 0)
						{
							return 0;
						}
					}
				}
				if((dir & 0x08)!=0)//270
				{
					if(y != 0)
					{
						if(readMapGrid(x,y - 1) == 0)
						{
							return 0;
						}
					}
				}
			}
		}
	}
	return 1;
}

uint8_t overFlag(void)
{
	//�������ȵִ��յ��������Ϣ
	getOver();
	
	//��Ѱ��ֹ����
	if(
			((
	
				getOver() //�ִ��յ�
	
			)&&(ERGODICMAP_STOP_ALLCPLT == 0)) ||

			getMapOver()//ȷ��ȫ��ɨ�����
		)
	{
		return 0;
	}
	return 1;
}

//��ÿɻ�Է��������
uint8_t getDirnum(uint8_t dir)
{
	//�ɻ��������
	uint8_t num = 0;
	
	if(dir&0x01)num++;
	if(dir&0x02)num++;
	if(dir&0x04)num++;
	if(dir&0x08)num++;
	
	return num;
}

//�������ѡ��ķ���
uint8_t getDirFirst(uint8_t wall,uint8_t plant)
{
	switch(plant)
	{
		case 0://xy����������
		{
			if((wall & 0x01) != 0)return 0x01;
			if((wall & 0x02) != 0)return 0x02;
			if((wall & 0x04) != 0)return 0x04;
			if((wall & 0x08) != 0)return 0x08;
			break;
		}
		case 1://xy����������
		{
			if((wall & 0x08) != 0)return 0x08;
			if((wall & 0x04) != 0)return 0x04;
			if((wall & 0x01) != 0)return 0x01;
			if((wall & 0x02) != 0)return 0x02;
			break;
		}
		case 2:
		{
			//����� 270 180 90 0
			
			//���ŷ���
			uint8_t dir = 0;
			//�������ĵ�ľ���
			float dis = 1000;
			
			//�����ĸ�����ȡ�����ž���
			for(int i = 0;i < 4;i++)
			{
				//�÷���ɻ��
				if((wall & (0x01 << i)) != 0)
				{
					//����ƫ������
					PosTypedef nmp = getMousePos();
					if(i == 0)nmp.x++;
					else if(i == 1)nmp.y++;
					else if(i == 2)nmp.x--;
					else if(i == 3)nmp.y--;
					
					float dis_t = sqrt((7.5 - nmp.x) * (7.5 - nmp.x) +
													(7.5 - nmp.y) * (7.5 - nmp.y) );
					
					if(dis_t < dis)
					{
						dis = dis_t;
						dir = (0x01 << i);
					}
				}
			}
			return dir;
		}
	}
}

//�����һ����
PosTypedef getNextPos(uint8_t dirf,PosTypedef mp)
{

	//ƫ������
	if((dirf & 0x01) != 0)mp.x++;
	if((dirf & 0x02) != 0)mp.y++;
	if((dirf & 0x04) != 0)mp.x--;
	if((dirf & 0x08) != 0)mp.y--;
	
	return mp;
}

//ͨ����һ��Ŀ�����㷽��
uint8_t nPosToDir(PosTypedef mp,PosTypedef nmp)
{
	if(mp.x > nmp.x)return 0x04;
	if(mp.y > nmp.y)return 0x08;
	if(mp.x < nmp.x)return 0x01;
	if(mp.y < nmp.y)return 0x02;
	return 0;
}


//�������������ƶ�
void moveToNext(uint8_t dir,PosTypedef mp)
{
	//������̬
	switch(dir)
	{
		case 0x01:
			switch (mp.th)
			{
				case 90  :mouseMove(MR90);break;
				case 180 :mouseMove(ML180);break;
				case 270 :mouseMove(ML90);break;
			}break;
		case 0x02:
			switch (mp.th)
			{
				case 0   :mouseMove(ML90);break;
				case 180 :mouseMove(MR90);break;
				case 270 :mouseMove(ML180);break;
			}break;
		case 0x04:
			switch (mp.th)
			{
				case 0   :mouseMove(ML180);break;
				case 90  :mouseMove(ML90);break;
				case 270 :mouseMove(MR90);break;
			}break;
		case 0x08:
			switch (mp.th)
			{
				case 0   :mouseMove(MR90);break;
				case 90  :mouseMove(ML180);break;
				case 180 :mouseMove(ML90);break;
			}break;
	}
	//ǰ��
	mouseMove(MForward);
}

//����ɻ���ݵ�
void savePathPoint(PosTypedef mp,uint8_t dir)
{
	//����·��
	g_pathPos[g_pathCur].x = mp.x;
	g_pathPos[g_pathCur].y = mp.y;
	g_pathPos[g_pathCur].th = mp.th;
	
	//�ƶ�·�����
	g_pathCur++;
	
	
	//����ɻ��ݵĵ�
	if(getDirnum(dir) > 0)
	{
		//������
		g_actdir[g_actCur] = dir;
		//��������
		g_actpos[g_actCur][0] = mp.x;
		g_actpos[g_actCur][1] = mp.y;
		
		//�ƶ��ɻ��ݵ�Ĺ��
		g_actCur++;
	}
}

//ɾ�������ʷ���
uint8_t delFromDir(uint8_t dir,PosTypedef mp)
{
	switch(mp.th)
	{
		case 0:		return dir & (~0x04);
		case 90:	return dir & (~0x08);
		case 180:	return dir & (~0x01);
		case 270:	return dir & (~0x02);
	}
}

//�ѷ���״̬
uint8_t visitedState(uint16_t x,uint16_t y)
{
	for(int i = 0;i < g_pathCur;i++)
	{
		if((g_pathPos[i].x == x &&
			 g_pathPos[i].y == y) ||
			 (readMapGrid(x,y) != 0))
		{
			return 1;
		}
	}
	return 0;
}
//��һ��ķ������
uint8_t visitedCheck(PosTypedef mp,uint8_t dir)
{
	if(visitedState(mp.x + 1,mp.y))dir = dir & (~0x01);
	if(visitedState(mp.x,mp.y + 1))dir = dir & (~0x02);
	if(visitedState(mp.x - 1,mp.y))dir = dir & (~0x04);
	if(visitedState(mp.x,mp.y - 1))dir = dir & (~0x08);
	return dir;
}

uint8_t BackActCheck(void)
{
	PosTypedef mp;

	
	//��һ�����ݵ�ļ��
	g_actCur--;
	
	mp.x = g_actpos[g_actCur][0];
	mp.y = g_actpos[g_actCur][1];
	
	uint8_t dir = g_actdir[g_actCur];
	//�ڿɷ��ʷ���������ڽӵ��Ѿ�������˷���ʧ��
	g_actdir[g_actCur] = visitedCheck(mp,dir);
	
	if(g_actdir[g_actCur] != 0)
	{
		return 0;
	}
	return 1;
}

//ɾ������
void delDeathGrid(void)
{
	for (int x = 0; x < 16; x++)
	{
		for (int y = 0; y < 16; y++)
		{
			//��ǰ��δʹ��
			if (readMapGrid(x, y) == 0)
			{
				//��Χ�ĸ�����ѷ���
				if (((readMapGrid(x + 1, y) != 0) || (x == 15)) &&
					((readMapGrid(x - 1, y) != 0) || (x == 0)) &&
					((readMapGrid(x, y + 1) != 0) || (y == 15)) &&
					((readMapGrid(x, y - 1) != 0) || (y == 0))
					)
				{
					//������״̬
					uint8_t state = 0;
					state |= 0x10;//״̬�ѷ���
					state |= (x != 15) ? (readMapGrid(x + 1, y) & 0x04) >> 2 : 0;//0		1
					state |= (y != 15) ? (readMapGrid(x, y + 1) & 0x08) >> 2 : 0;//90	2
					state |= (x != 0) ? (readMapGrid(x - 1, y) & 0x01) << 2 : 0;//180	4
					state |= (y != 0) ? (readMapGrid(x, y - 1) & 0x02) << 2 : 0;//270	8
					//д���ͼ
					writeMapGrid(x,y,state);
				}
			}
		}
	}
}

//ɾ��·���д��ڵĻ� �����
void pathRingDel(void)
{
	
	
}

/*come back to start -----------------------------------------------------------*/

//���ص����
void backtoStart(void)
{
	//ɾ��·���д��ڵĻ�
	 pathRingDel();
	
	//����·��
	g_pathCur--;
	
	//��ǰλ�ò���Ŀ�Ļ���λ��
	while(getMousePos().x != 0 ||
				getMousePos().y != 0)
	{
		//��ȡ��һ��·������ΪĿ���
		uint8_t dir = nPosToDir(getMousePos(),g_pathPos[g_pathCur]);
		
		//�ƶ�
		moveToNext(dir,getMousePos());		
		
		//�ȴ�
		while(getMousePos().x != g_pathPos[g_pathCur].x ||
					getMousePos().y != g_pathPos[g_pathCur].y );
		//������
		g_pathCur--; 
	}
	
	mouseMove(MStop);
	mouseMove(ML180);
	mouseMove(MStop);
}

/*Map Creator ------------------------------------------------------------------*/

//���ݵ���һ���ɻ������ ����ִ��
void backToLastAct(void)
{
	//��ǰ���ݵ���Ч�Ը���
	while(BackActCheck());
	//����·��
	g_pathCur--;

	//��ǰλ�ò���Ŀ�Ļ���λ��
	while(getMousePos().x != g_actpos[g_actCur][0] ||
				getMousePos().y != g_actpos[g_actCur][1] )
	{

		//��ȡ��һ��·������ΪĿ���
		uint8_t dir = nPosToDir(getMousePos(),g_pathPos[g_pathCur]);
		
		//�ƶ�
		moveToNext(dir,getMousePos());	
		
		//�ȴ�
		while(getMousePos().x != g_pathPos[g_pathCur].x ||
					getMousePos().y != g_pathPos[g_pathCur].y );
		//������
		g_pathCur--; 
	}
	
	g_pathCur += 1; 
	//�ƶ��������
	uint8_t dir = g_actdir[g_actCur];
	uint8_t dirf = getDirFirst(dir,g_SearchPlant);//������ȷ���
	PosTypedef nmp  = getNextPos(dirf,getMousePos());//�����һ�����λ��
	//����·��
	savePathPoint(getMousePos(),dir&(~dirf));
	//�ƶ�
	moveToNext(dirf,getMousePos());
	//�ȴ�
	while(getMousePos().x != nmp.x ||
				getMousePos().y != nmp.y );
}

//��ͼ���� ÿ�ƶ���һ�������������һ��
void MapCreator(void)
{
	//ƽ̨λ��
	//ƽ̨����
	PosTypedef mp,nmp;
	//ƽ̨�ɻ����
	uint8_t dir,dirf;
	
	//�ȴ���ͼ������� ȫͼ��δ֪�����ȫͼ�޿ɻ����
	while(overFlag())//(getOver())//(getMapOver())//
	{
		//ɾ������
		delDeathGrid();
		
		//��ÿɻ����
		mp = getMousePos();
		dir = readMapGrid(mp.x,mp.y);
		
		//�ڿɷ��ʷ���������ڽӵ��Ѿ�������˷���ʧ��
		dir = visitedCheck(mp,dir);
		
		//�пɻ���� ��ǰ��δɨ�� - �ƶ�����һ��
		if(getDirnum(dir) > 0)
		{
			dir = delFromDir(dir,mp);
			//�ƶ�����ľ��� �õ���һ��Ŀ���
			dirf = getDirFirst(dir,g_SearchPlant);//������ȷ���
			nmp  = getNextPos(dirf,mp);//�����һ�����λ��
			//������·��
			savePathPoint(mp,dir&(~dirf));
			//�ƶ�
			moveToNext(dirf,mp);
			
			//�ȴ��ִ����һ����
			while(getMousePos().x != nmp.x ||
						getMousePos().y != nmp.y );
		}
		//�޿ɻ����	�����Ѿ����� -	���˵���һ���ɻ�ĵ�
		else
		{
			backToLastAct();
		}
	}
}

/*Setup loop ----------------------------------------------------------------------*/
//��ͼ������ʼ��
void MapCreatorInit(void)
{
	for(int i = 0;i < 100;i++)
	{
		g_actdir[i] = 0;
		g_actpos[i][0] = 0;
		g_actpos[i][1] = 0;
	}
	g_actCur = 0;
	
	for(int i = 0;i < 255;i++)
	{
		g_pathPos[i].x = 0;
		g_pathPos[i].y = 0;
		g_pathPos[i].th = 0;
	}
	g_pathCur = 0;
}

void MapCreatLoop(void)
{
	//������ͼ
	MapCreator();
	//ֹͣ
	mouseMove(MStop);
	//·���滮
	routeSearcher();	
	//����·��
	writePathToFlash();
	
	//����3�������Ϊ�յ�ʲ���Ҫ����
	if(ERGODICMAP_SEARCHOR_PLAN != 3)
	{
		backtoStart();
	}
	else
	{
		mouseMove(MStop);
		mouseMove(ML180);
		mouseMove(MStop);
	}
}
