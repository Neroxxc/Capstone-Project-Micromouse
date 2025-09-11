/**
  ******************************************************************************
  * @file    myPlanner.c
  * @author  �Ŷ�
  * @version V1.0.0
  * @date    2019-10-8
  * @brief   ����·���������ƶ����� ��δ�Ż������԰桿 
	*					 ����ȫ����ͳ�����ŵ� ���� ·���滮 ��ٷ�
  ******************************************************************************
  */
/*include file ---------------------------------------------------------------*/
#include "myPlanner.h"
#include "drive.h"
#include "Odometer.h"
#include "OccupyingGrid.h"
#include "MoveCtrl.h"
#include "mapSaver.h"

#include "stdio.h"
/*Golbal Data Space ----------------------------------------------------------*/

#define NODE_MAX            255     //�·���ڵ�����
#define PATHPLANNER_I				1


PosTypedef g_TargetPos = {.x = 7,.y = 7,.th = 180}; //Ŀ����λ��
PosTypedef g_StartPos  = {.x = 0,.y = 0,.th = 90 }; //��ʼ���λ��

struct
{
	uint8_t          route[NODE_MAX][2];  					 	//�ڵ�����
	uint8_t          num;            									//�ڵ�����
	uint8_t          cursor;         									//��ǰ���

}g_Route; //·����

//�ƶ���������ƫ�Ʊ� 1L1 2U2 3R4 4D8
const int8_t MOVE[4][2] = {
	1, 0,//R
	0, 1,//D
 -1, 0,//L
	0,-1,//U
};

#ifdef PATHPLANNER

/*--------------------ͼ��λ����---------------------*/
//��ȡ�ֽڵ�һ��λ��״̬
uint8_t readBitStatus(uint8_t ch, uint8_t num)
{
	return ((ch & (0x01 << num)) != 0);
}
//д�ֽ�һ��λ��״̬
uint8_t writeBitStatus(uint8_t ch, uint8_t num, uint8_t state)
{
	ch = state ? ch | (0x01 << num) : ch & (~(0x01 << num));
	return ch;
}
//��ͼ״̬��д��
void mapBitWrite(uint8_t * ch, uint8_t num, uint8_t state)
{
	ch = (num < 8) ? ch : ch + 1;
	num = (num < 8) ? num : num - 8;
	*ch = writeBitStatus(*ch, num, state);
}
//��ͼ״̬�Ķ�ȡ
uint8_t mapBitRead(uint8_t * ch, uint8_t num)
{
	ch = (num < 8) ? ch : ch + 1;
	num = (num < 8) ? num : num - 8;
	return ((*ch & (0x01 << num)) != 0);
}
/*--------------------�滮��ά��---------------------*/
//��ÿ��ƶ�����
uint8_t getActiveDir(uint8_t x, uint8_t y)
{
	return  readMapGrid(x,y) & 0x0f;
}
//���������
uint8_t getDirScore(uint8_t x1, uint8_t y1)
{
	uint8_t xt, yt;
	uint8_t x2 = g_TargetPos.x;
	uint8_t y2 = g_TargetPos.y;
	uint8_t temp;

	xt = (x1 > x2) ? x1 - x2 : x2 - x1;
	yt = (y1 > y2) ? y1 - y2 : y2 - y1;

	temp = xt * xt + yt * yt;

	return temp;
}
//��ô�����͵��ƶ�����
uint8_t getLeastCostDir(uint8_t x, uint8_t y, uint8_t dir)
{
	uint8_t ct;
	uint16_t sdir[2];
	sdir[0] = 500;
	//����Ψһ��
	if (dir == 0x08 || dir == 0x04 || dir == 0x02 || dir == 0x01)
	{
		switch (dir)
		{
		case 0x08:return 4;
		case 0x04:return 3;
		case 0x02:return 2;
		case 0x01:return 1;
		}
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			//����÷�����ƶ�
			if ((dir&(0x01 << i)) != 0)
			{
				//�������
				ct = getDirScore(x + MOVE[i][0], y + MOVE[i][1]);
				//ȡ�������ٵķ���
				if (sdir[0] > ct)
				{
					sdir[0] = ct;
					sdir[1] = i;
				}
			}
		}
	}
	return (sdir[1] + 1);
}

////��ӡ��
//void printpoint(uint8_t x, uint8_t y, uint16_t color)
//{
//	block.printxy(x * 2, 19 - y, color, (char *)"��");
//}
////��ʾ·��
//void displayRoute(uint8_t num, uint8_t route[30][2], uint16_t color)
//{
//	for (int i = 0; i < num; i++)
//	{
//		printpoint(route[i][0] + 31, route[i][1] + 1, color);
//	}
//}


/*--------------------·���滮---------------------*/
	uint8_t routenow[NODE_MAX][2];  //�滮·�߽ڵ��б�
	uint8_t posdir[NODE_MAX];		//ÿ���ڵ��δ�滮����
	uint8_t info[2];				//����·������������˶�����
	uint8_t cost[2];                //��ǰ·������������˶�����
	uint8_t nodeClose[16][2];       //ȫ���ڵ�ķ������
	uint8_t turnNode[NODE_MAX][3];	//��¼ת��� xyλ�ú�ԭ���� ���ڻ����˶�����

uint8_t routeSearcher(void)
{
	uint8_t backtrackingFlag = 0;	//���ݱ�־λ
	uint8_t cursor = 0;             //�ڵ���
	uint8_t temp = 0;               //��ʱ��Ա
	uint8_t dir_save = 0;           //��̬��¼
	uint8_t status = 1;             // ·���滮 ״̬��־λ 0 ���� 1����ʱ

	int  backnum = 0;               //���ݴ���
	int  searchnum = 0;             //�����������

	//��ʼ���ڵ���ʱ�
	for (int i = 0; i < 16; i++)
	{
		nodeClose[i][0] = 0;
		nodeClose[i][1] = 0;
	}

	//�趨��ʼλ�ã���̬�ͷ���״̬
	routenow[0][0] = g_StartPos.x;
	routenow[0][1] = g_StartPos.y;
	dir_save = g_StartPos.th / 90;
	//nodeClose[car.gbl.y][car.gbl.x] = 1;
	mapBitWrite(nodeClose[g_StartPos.y],g_StartPos.x, 1);

	//�趨��ʼ��������˶�����
	info[0] = 255;
	info[1] = 255;
	cost[0] = 0;
	cost[1] = 0;
	posdir[0] = 0x0F;

	//��ʼ�滮·��
	while (status == 1)
	{
		//���ݲ���
		if (backtrackingFlag)
		{
			//�ڵ�Ļ��ݲ���
			//nodeClose[routenow[cursor][1]][routenow[cursor][0]] = 0;
			mapBitWrite(nodeClose[routenow[cursor][1]], routenow[cursor][0], 0);
			cursor--;
			backnum++;
			cost[0]--;
			//�˶����۵Ļ��ݲ���
			if (routenow[cursor][0] == turnNode[cost[1] - 1][0] && routenow[cursor][1] == turnNode[cost[1] - 1][1])
			{
				cost[1]--;
				dir_save = turnNode[cost[1]][2];
			}
		}
		//���½ڵ�����
		if (!backtrackingFlag)
		{	//��ÿ��ƶ�����
			posdir[cursor] = getActiveDir(routenow[cursor][0], routenow[cursor][1]);
			//ʧ����Χ�Ѿ���������·��
			for (int i = 0; i < 4; i++)
			{	//����ýڵ��Ѿ�������
				//(nodeClose[routenow[cursor][1] + MOVE[i][1]][routenow[cursor][0] + MOVE[i][0]] == 1)
				if (mapBitRead((nodeClose[routenow[cursor][1] + MOVE[i][1]]), (routenow[cursor][0] + MOVE[i][0])) == 1)
				{
					posdir[cursor] = posdir[cursor] & (~(0x01 << i));
				}
			}
		}
		//�ڵ���
		if (posdir[cursor])
		{
			backtrackingFlag = 0;
			//�����С���۵��ƶ�����
			temp = getLeastCostDir(routenow[cursor][0], routenow[cursor][1], posdir[cursor]);
			posdir[cursor] = posdir[cursor] & (~(0x01 << (temp - 1)));//��Ǹ÷����Ѿ��ƶ�
			//��¼�˶�����
			if (dir_save != temp)
			{
				//��¼�˶����۲�����ԭ����
				turnNode[cost[1]][0] = routenow[cursor][0];
				turnNode[cost[1]][1] = routenow[cursor][1];
				turnNode[cost[1]][2] = dir_save;
				cost[1]++;
			}
			dir_save = temp;
			//���ӹ�����·�߽ڵ㲢ʹ�ܷ���״̬
			routenow[cursor + 1][0] = routenow[cursor][0] + MOVE[temp - 1][0];
			routenow[cursor + 1][1] = routenow[cursor][1] + MOVE[temp - 1][1];
			cursor++;
			if (cursor == NODE_MAX)
			{
//				std::cout << "�ڵ������ѳ������Χ";
				return 0;
			}
			else
			{
				//nodeClose[routenow[cursor][1]][routenow[cursor][0]] = 1;
				mapBitWrite(nodeClose[routenow[cursor][1]], routenow[cursor][0], 1);
			}
			cost[0]++;
		}
		else//�޷��ƶ�
		{	//�����ԭ��
			if (cursor == 0)
			{
				status = 0;//·���滮���
			}
			//���ݲ���
			backtrackingFlag = 1;
		}
		//����Ŀ���
		if (routenow[cursor][0] == g_TargetPos.x && routenow[cursor][1] == g_TargetPos.y)
		{
			searchnum++;
			//�����ǰ���С������·����� ���� �������� �˶����۸��� ��ת���С��4ʱ���ڽ��й滮
			if((cost[0] + cost[1]) < (info[0] + info[1]))//(info[0] > cost[0] || (info[0] == cost[0] && info[1] > cost[1]) || cost[1] < 2)
			{
				//��·����Ϊ����·��
				memcpy(info, cost, 2);
				for (int i = 0; i < cost[1]; i++)
				{
					g_Route.route[i][0] = turnNode[i][0];
					g_Route.route[i][1] = turnNode[i][1];
				}
				g_Route.num = info[1];
				//��Ŀ�����ӽ�����ͼ
				g_Route.route[g_Route.num][0] = g_TargetPos.x;
				g_Route.route[g_Route.num][1] = g_TargetPos.y;
				g_Route.num++;
			}
			//��ת���С��4��ʱ ����Ҫ�ٽ�������
			if (cost[1] < 2)
			{
				status = 0;//·���滮���
			}
			//���ݲ���
			backtrackingFlag = 1;
		}
		//�˶����۴������е�·�� ִ�л��ݲ���
		if(cost[1] > info[1] || cost[0] > info[0])
		{
			//���ݲ���
			backtrackingFlag = 1;
		}
	}
	//·���滮����

//	block.printxy(0, 25, CL_W, (char*)" ");
	//�жϹ滮�Ƿ�ɹ�
	if (info[1] != 100)
	{
		//·���滮���
//		std::cout << "����·���滮���,������滮��� ��������" << backnum << ",�������:" << searchnum << "��";
//		std::cout << "դ��,ת���:" << (int)(info[1] + 1) << "�����߶�����:" << (int)(info[1]) << "��" << std::endl;
//		displayRoute(Pmap.route.num, Pmap.route.route, CL_G);
//		block.printxy(0, 26, CL_W, (char*)" ");
		return 1;
	}
	else
	{
//		std::cout << "����·���滮ʧ�� �޷���������Ч·�� ������:" << backnum << std::endl;
//		block.printxy(0, 26, CL_W, (char*)" ");
		return 0;
	}
	return 0;
}

#endif /*PATHPLANNER*/

/*--------------------��ʼ�������Ϣ------------------*/
void plannerInit(void)
{
	g_TargetPos.x = 7;
	g_TargetPos.y = 7;
	g_TargetPos.th = 180;
	
	g_StartPos = getMousePos();
}
/*---------------------�����ṩ��Ϣ-------------------*/
//��ýڵ�������Ϣ
PosTypedef getRoute(uint8_t cur)
{
	static PosTypedef mp;
	
	mp.x = g_Route.route[cur][0];
	mp.y = g_Route.route[cur][1];
	mp.th = 0;
	
	return mp;
}

//���Ŀ����������Ϣ
PosTypedef getTargetPos(void)
{
	return g_TargetPos;
}

//��ȡ�ڵ�����
uint8_t getPathCurNum(void)
{
	return g_Route.num;
}

/*---------------------�޸���Ϣ-----------------*/
//�޸Ľڵ���Ϣ
void setPathNode(uint8_t cur,PosTypedef mp)
{
	g_Route.route[cur][0] = mp.x;
	g_Route.route[cur][1] = mp.y;
}

//�������нڵ�����
void setPathCurNum(uint8_t num)
{
	g_Route.num = num;
}

//�޸�Ŀ���
void setTargetPos(PosTypedef tg)
{
	g_TargetPos = tg;
}


/*---------------------------------------------------------·���滮�ڶ��汾*/

/*����˼�� �����Ⱦ��벽����ɢ д�뵽ÿ�����˶����� ���յ��ٻᵽ���
 *
 *���ڵ�ĸ���	�������˶�����С�ڼ������ǵ���˶������򸲸ǲ�����Ϊ�
 *���Ľ�ֹ	����޷�� �޷����1.��ͼ�ϰ����� 2.��Χ���е���˶�����С�ڼ���Ԥ�� 
 *���Ĵ洢	���ζ��� ��ȡ�±� д���±� ����ѭ��д��Ͷ�ȡ
 *����ظ�����	����Ѿ�������д��
 *���Ļ��	���չ���ֹ
 *�ƶ�			���ƶ�ǰ����ɻ�� ���ɻ��ֱ�Ӻ���
 *��ֹ����		�޿ɻ���
 */

#ifdef PATHPLANNER_I

//��ͼ��������˶�����
uint16_t g_MapData[16][16];

//�������ݴ洢�ṹ
typedef struct
{
	PosTypedef cur[256];
	uint8_t wt;
	uint8_t rd;
}CurPosArrTypedef;

CurPosArrTypedef g_mapCurArr;


//��ȡһ�����
PosTypedef getCurPos(void)
{
	PosTypedef p;
	p = g_mapCurArr.cur[g_mapCurArr.rd];
	
	g_mapCurArr.cur[g_mapCurArr.rd].th = 0xFFFF;
	g_mapCurArr.cur[g_mapCurArr.rd].x = 0xFF;
	g_mapCurArr.cur[g_mapCurArr.rd].y = 0xFF;
	
	g_mapCurArr.rd = (g_mapCurArr.rd == 255) ? 0 : g_mapCurArr.rd + 1;
	return p;
}
//д��һ�����
void saveCurPos(PosTypedef p)
{
	g_mapCurArr.cur[g_mapCurArr.wt] = p;
	
	g_mapCurArr.wt = (g_mapCurArr.wt == 255) ? 0 : g_mapCurArr.wt + 1;
}
//��ȡ��ͼһ����Ľ���ɻ����
uint8_t readMapPosX(PosTypedef p)
{
	//Ϊ��ֹ����ͨ�����ð�ȫ����
	//һ������Ļ�����ڽӵ��Ӧ�Ļ����ͬʱ����ʱ��Ч
	uint8_t dir = readMapGrid(p.x, p.y) & 0x0f;
	
	//����������ٴβ�ѯ�ٴ�ȷ�ϻ�������ȷ��
	dir = (p.x == 15) ? dir & (~0x01) : ((readMapGrid(p.x + 1, p.y) & 0x04) != 0) ? dir & (~0x00) : dir & (~0x01);
	dir = (p.x == 0 ) ? dir & (~0x04) : ((readMapGrid(p.x - 1, p.y) & 0x01) != 0) ? dir & (~0x00) : dir & (~0x04);
	dir = (p.y == 15) ? dir & (~0x02) : ((readMapGrid(p.x, p.y + 1) & 0x08) != 0) ? dir & (~0x00) : dir & (~0x02);
	dir = (p.y == 0 ) ? dir & (~0x08) : ((readMapGrid(p.x, p.y - 1) & 0x02) != 0) ? dir & (~0x00) : dir & (~0x08);
 
	return dir;
}
//��ȡ����˶�����
uint16_t readMapDis(PosTypedef p)
{
	return g_MapData[p.x][p.y];
}
//д�����˶�����
void writeMapDis(PosTypedef p, uint16_t dis)
{
	g_MapData[p.x][p.y] = dis;
}


//�ɻ���򵽽Ƕ�ת��
uint16_t dirToTh(uint8_t dir)
{
	if (dir == 0x01) return 0;
	if (dir == 0x02) return 90;
	if (dir == 0x04) return 180;
	if (dir == 0x08) return 270;
	return 0;
}
//���ƫ�ƺ������
PosTypedef getPosAfterMove(PosTypedef p, uint8_t dir)
{
	if (dir == 0x01)p.x++;
	if (dir == 0x02)p.y++;
	if (dir == 0x04)p.x--;
	if (dir == 0x08)p.y--;

	p.th = dirToTh(dir);

	return p;
}

////��ʾ��ǰ����˶�����
//void viewDisXY(PosTypedef p,uint32_t color)
//{
//	block.printxy(p.x * 3 + 60, 18 - p.y, color, (char *)"");
//	int dis = readMapDis(p);
//	cout << dis;
//}

//·���ƻ���I
void pathPlannerI(void)
{
	//�滮��ѭ��
	for(PosTypedef cur;(cur = getCurPos()).th != 0xFFFF;)
	{
		//��ǰ����ĸ�������ƶ�
		for (int i = 0; i < 4; i++)
		{
			//��ǰ�����
			uint8_t dir = (0x01 << i);

			//��ǰ�����ͼ�����ƶ�
			if ((readMapPosX(cur) & dir) != 0)
			{
				//��ǰλ�õ��˶�����
				uint8_t dis = readMapDis(cur) + 1;

				//��ǰ�����̬�뵱ǰ�����ͬ
				if (cur.th != dirToTh(dir))
				{
					dis += 1;//ת�����+1
				}
				//����ƫ��
				PosTypedef nmp = getPosAfterMove(cur, dir);

				//�����ִ����˶����۴��ڵ�ǰ�˶�����
				if (readMapDis(nmp) > dis)
				{
					//д���˶�����
					writeMapDis(nmp, dis);
					//д����
					saveCurPos(nmp);

					//��ʾ��ǰ������˶�����
					//viewDisXY(nmp,CL_W);
				}
			}
		}
	}
}

//·��������
PosTypedef g_pathAnl[256];
//�����㽹��
PosTypedef navigNode[100];

//·������
void pathAnalysis(void)
{

	//·������������
	uint8_t cur = 0;
	//�����㽹��
	uint8_t ncur = 0;
	//·����ʼ��
	g_pathAnl[0] = getTargetPos();
	g_pathAnl[0].th = 360;

	//·����ȡ ���յ㿪ʼ��������˶����۵����
	while (g_pathAnl[cur].x != 0 || g_pathAnl[cur].y != 0)
	{
		//��һ���˶����۵ı���
		uint8_t dis = 255;

		//�ĸ��������С�˶�������Ѱ
		for (int i = 0; i < 4; i++)
		{
			//����Ԥ�ڻ����
			uint8_t dir = (0x01 << i);

			//��ǰ�����ͼ�����ƶ�
			if ((readMapPosX(g_pathAnl[cur]) & dir) != 0)
			{
				//����ƫ��
				PosTypedef nmp = getPosAfterMove(g_pathAnl[cur], dir);
				//��ǰ����˶�����С���ѱ�����˶�����
				if (readMapDis(nmp) < dis)
				{
					//�����˶�����
					dis = readMapDis(nmp);
					//�����
					g_pathAnl[cur + 1] = nmp;
				}
			}
		}
		//�ƶ����
		cur++;
		//��ʾ·��
		//viewDisXY(path[cur], CL_G);
		//Sleep(200);


		if (g_pathAnl[cur - 1].th != g_pathAnl[cur].th)
		{
			navigNode[ncur] = g_pathAnl[cur - 1];
			//viewDisXY(navigNode[ncur], CL_R);
			ncur++;
			//Sleep(200);
		}

	}
	//�������ӽ�����������
	navigNode[ncur].x = 0;
	navigNode[ncur].y = 0;
	ncur++;


	//���浼����
	for (int i = 0; i < ncur; i++)
	{
		setPathNode(i,navigNode[ncur - 1 - i]);
	}

	while (0);
}
/*
uint8_t map[] =
{
	
	0x12,0x13,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x16,
	0x1A,0x1A,0x13,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x16,0x1A,
	0x1B,0x1E,0x1A,0x11,0x17,0x15,0x15,0x15,0x15,0x15,0x17,0x16,0x13,0x16,0x1A,0x1A,
	0x1A,0x1A,0x1A,0x13,0x1D,0x16,0x13,0x15,0x17,0x14,0x1A,0x19,0x1C,0x19,0x1E,0x1A,
	0x1A,0x1A,0x1A,0x1A,0x12,0x18,0x1B,0x16,0x19,0x16,0x1A,0x11,0x15,0x17,0x1C,0x1A,
	0x1A,0x1A,0x1A,0x19,0x1E,0x12,0x18,0x19,0x16,0x19,0x1C,0x12,0x13,0x1C,0x12,0x1A,
	0x1A,0x1A,0x1A,0x12,0x19,0x1E,0x13,0x15,0x1D,0x16,0x12,0x1B,0x1C,0x13,0x1E,0x1A,
	0x1A,0x1A,0x1A,0x19,0x17,0x1E,0x1B,0x17,0x16,0x1A,0x1B,0x1E,0x13,0x1C,0x1A,0x1A,
	0x1A,0x1A,0x1A,0x13,0x1C,0x1A,0x1A,0x19,0x1C,0x1A,0x1A,0x1A,0x1A,0x13,0x1C,0x1A,
	0x1A,0x1A,0x1A,0x1A,0x12,0x18,0x19,0x15,0x15,0x1C,0x1A,0x1A,0x1A,0x19,0x16,0x1A,
	0x1A,0x1A,0x1A,0x19,0x1F,0x14,0x13,0x15,0x15,0x15,0x1E,0x1A,0x1A,0x13,0x1C,0x1A,
	0x1A,0x1A,0x1B,0x15,0x1D,0x16,0x19,0x15,0x15,0x16,0x1A,0x1A,0x1A,0x1A,0x12,0x1A,
	0x1A,0x1A,0x1A,0x13,0x15,0x1C,0x13,0x15,0x15,0x1C,0x18,0x19,0x1C,0x19,0x1E,0x1A,
	0x1A,0x1A,0x19,0x1D,0x15,0x15,0x1D,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x1D,0x1E,
	0x1A,0x19,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x1E,
	0x19,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x1C,
	
	0x1A,0x12,0x12,0x12,0x13,0x15,0x15,0x15,0x15,0x17,0x15,0x17,0x14,0x11,0x15,0x16,
	0x1A,0x1A,0x1B,0x1C,0x1B,0x16,0x13,0x15,0x17,0x1D,0x16,0x19,0x16,0x13,0x15,0x1E,
	0x1A,0x19,0x1D,0x17,0x1C,0x19,0x1F,0x16,0x1B,0x16,0x1B,0x16,0x19,0x1E,0x12,0x1A,
	0x1B,0x15,0x15,0x1D,0x17,0x14,0x1A,0x19,0x1E,0x1A,0x18,0x1B,0x16,0x19,0x1E,0x1A,
	0x1A,0x12,0x13,0x16,0x1B,0x14,0x1B,0x15,0x1E,0x1A,0x12,0x18,0x19,0x16,0x19,0x1E,
	0x1B,0x1F,0x1C,0x19,0x1D,0x14,0x19,0x15,0x1F,0x1C,0x19,0x15,0x16,0x19,0x16,0x1A,
	0x1A,0x1A,0x12,0x13,0x15,0x14,0x12,0x11,0x1C,0x13,0x16,0x13,0x1D,0x15,0x1E,0x1A,
	0x1A,0x1A,0x19,0x1F,0x15,0x15,0x1C,0x13,0x16,0x1A,0x1B,0x1C,0x13,0x14,0x1A,0x1A,
	0x1B,0x1D,0x15,0x1F,0x14,0x13,0x15,0x1D,0x1C,0x1B,0x1C,0x13,0x1D,0x17,0x1D,0x1E,
	0x1B,0x14,0x11,0x1E,0x13,0x1C,0x13,0x15,0x17,0x1C,0x13,0x1D,0x16,0x1A,0x12,0x1A,
	0x1A,0x13,0x17,0x1E,0x1B,0x17,0x1D,0x15,0x1C,0x12,0x1A,0x13,0x1E,0x19,0x1F,0x14,
	0x19,0x1E,0x1A,0x1A,0x18,0x19,0x15,0x14,0x13,0x1D,0x1D,0x1C,0x19,0x17,0x1D,0x16,
	0x13,0x1D,0x1F,0x1D,0x17,0x17,0x15,0x17,0x1C,0x13,0x14,0x13,0x14,0x19,0x16,0x1A,
	0x1A,0x11,0x1E,0x00,0x1A,0x1B,0x14,0x1A,0x12,0x1B,0x16,0x1B,0x15,0x15,0x1D,0x1E,
	0x1B,0x14,0x1A,0x00,0x1A,0x1A,0x00,0x1A,0x1B,0x1E,0x1B,0x1D,0x17,0x14,0x11,0x1E,
	0x19,0x15,0x1D,0x15,0x1D,0x1D,0x15,0x1D,0x1C,0x19,0x1D,0x15,0x1D,0x15,0x15,0x1C,
	

};*/

//�滮���ݳ�ʼ��
void plannerDataInit(void)
{
	for (int x = 0; x < 16; x++)
	{
		for (int y = 0; y < 16; y++)
		{
			g_MapData[x][y] = 0xFFFF;
		}
	}
	for (int i = 0; i < 256; i++)
	{
		g_mapCurArr.cur[i].x = 0;
		g_mapCurArr.cur[i].y = 0;
		g_mapCurArr.cur[i].th = 0;
	}
	g_mapCurArr.cur[0].x = 0;
	g_mapCurArr.cur[0].y = 0;
	g_mapCurArr.cur[0].th = 90;
	g_mapCurArr.wt = 1;
	g_mapCurArr.rd = 0;

	//map[0] &= 0x07;
	
	g_MapData[0][0] = 0;
	
/*
	for(int8_t x = 0;x < 16;x++)
	{
		for(int8_t y = 0;y < 16;y++)
		{
			writeMapGrid(x,y,map[x + y*16]);
		}
	}
	*/
	
}


uint8_t routeSearcher(void)
{
	//�������ݳ�ʼ��
	plannerDataInit();
	//·���滮
	pathPlannerI();
	//����·��
	writePathToFlash();
	//·������
	pathAnalysis();
	
	return 1;
}

#endif /*PATHPLANNER_I*/
	



