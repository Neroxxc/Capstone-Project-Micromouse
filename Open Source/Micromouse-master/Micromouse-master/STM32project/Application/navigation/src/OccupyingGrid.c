/**
  ******************************************************************************
  * @file    OccupyingGrid.c.c
  * @author  �Ŷ�
  * @version V1.0.0
  * @date    2019-9-30
  * @brief   ռ��դ���ͼ��д�����ִ�з����ϼ�
  ******************************************************************************
  */
/*include file ---------------------------------------------------------------*/
#include "OccupyingGrid.h"
#include "odometer.h"
#include "InfraredCalc.h"

#include "drive.h"
#include "stdio.h"

/*Global Date Space ----------------------------------------------------------*/
uint8_t g_GridMay[16][16];	//��ͼ�ɻ�Եļ�¼
uint8_t g_GridAct = 0;

/*
	��ͼ�Ĵ洢��ʽ
	
		�ɻ��
			����λ�洢��ͼ��������Ŀɻ����Ϣ
			��ͼ��������ϵ�ο���̼Ƶľ�������ϵ����
			
			L4B 					Active	Inactive
				|---B0	0��     1				0
				|---B1  90��    1        0
				|---B2	180��   1        0
				|---B3	270��	 1        0
		
			H4B
				|---B0	�ѷ���
				|---B1 	����
				|---B2	����
				|---B3	����
 */
 
 
//��ȡ��ͼ
uint8_t readMapGrid(uint8_t x,uint8_t y)
{
	return g_GridMay[x][y];
}
//д���ͼ
void writeMapGrid(uint8_t x,uint8_t y,uint8_t mp)
{
	g_GridMay[x][y] = mp;
}


//��ͼ��ʼ��
void MapInit(void)
{
	for(int i = 0;i < 16;i++)
	{
		for(int j = 0;j < 16;j++)
		{
			g_GridMay[i][j] = 0;
		}
	}
}

void sendMes(uint8_t w)
{
	PosTypedef mp = getMousePos();
	uint8_t str[20];
	
	//��ʼ��־
	str[0] = '<';	
	
	//��Ϣ��ʽ xx,yy,ttt,w
	str[1] = mp.x / 10 % 10 + 0x30;
	str[2] = mp.x % 10 + 0x30;
	
	str[3] = ',';
	
	str[4] = mp.y / 10 % 10 + 0x30;
	str[5] = mp.y % 10 + 0x30;
	
	str[6] = ',';
	
	//��̬
	str[7] = mp.th / 100 % 10 + 0x30;
	str[8] = mp.th / 10 % 10 + 0x30;
	str[9] = mp.th % 10 + 0x30;
	
	str[10] = ',';
	
	//�ϰ���
	str[11] = w;
	
	//����
	str[12] = '>';
	
	str[13] = 0;
	str[14] = 0;
	
	UsartSendData(str);
	UsartSendData(str);
}


//�����ͼ�����Ϣ
void GridActSave(uint8_t num)
{
	if(num == 0)
	{
		g_GridAct = getActDirState() & 0x05;
	}
	else
	{
		g_GridAct |= getActDirState() & 0x02;
	}
}



//դ���ͼ�ļ�¼
void MapActivityRecord(void)
{
	PosTypedef mp = getMousePos();
	
	//��ǰ����δ��¼
//	if(g_GridMay[mp.x][mp.y] == 0)
//	{
	
	
	//��ȡ�ɻ����Ϣ
	//����״̬�ö����Ʊ�ʾ��������Ŀɻ��
	//0000 0LFR L��� Fǰ�� R�Ҳ�
	uint8_t activity = g_GridAct;
	
	//��������Ƕ�
	uint8_t m0   = (((~activity) & 0x02) >> 1);
	uint8_t m90  = ((activity & 0x04) >> 2);
	uint8_t m180 = 0x01;
	uint8_t m270 = (activity & 0x01);
	
	//ʹ���ѷ��ʱ�־λ
	g_GridMay[mp.x][mp.y] |= 0x10;
	
	//ͨ��ƽ̨��̬���о��������ƫ��
	switch(mp.th)
	{
		case 0:
		{
			//0�㲻���б任
			g_GridMay[mp.x][mp.y] |= 0x01 &  m0				 ;//0-0
			g_GridMay[mp.x][mp.y] |= 0x02 & (m90  << 1);//90-90
			g_GridMay[mp.x][mp.y] |= 0x04 & (m180 << 2);//180-180
			g_GridMay[mp.x][mp.y] |= 0x08 & (m270 << 3);//270-270
			break;
		}
		case 90:
		{
			//�任��ʽ 0-90 90-180 270-0 180-270
			g_GridMay[mp.x][mp.y] |= 0x01 & m270			 ;//0-270
			g_GridMay[mp.x][mp.y] |= 0x02 & (m0   << 1);//90-0
			g_GridMay[mp.x][mp.y] |= 0x04 & (m90  << 2);//180-90
			g_GridMay[mp.x][mp.y] |= 0x08 & (m180 << 3);//270-180
			break;
		}
		case 180:
		{
			//�任��ʽ 0-180 90-270 270-90 180-0
			g_GridMay[mp.x][mp.y] |= 0x01 & m180		   ;//0-180
			g_GridMay[mp.x][mp.y] |= 0x02 & (m270 << 1);//90-270
			g_GridMay[mp.x][mp.y] |= 0x04 & (m0   << 2);//180-0
			g_GridMay[mp.x][mp.y] |= 0x08 & (m90	<< 3);//270-90
			break;
		}
		case 270:
		{
			//�任��ʽ 0-270 90-0  270-180 180-90
			g_GridMay[mp.x][mp.y] |= 0x01 & m90				 ;//0-90
			g_GridMay[mp.x][mp.y] |= 0x02 & (m180 << 1);//90-180
			g_GridMay[mp.x][mp.y] |= 0x04 & (m270 << 2);//180-270
			g_GridMay[mp.x][mp.y] |= 0x08 & (m0   << 3);//270-0
			break;
		}
	}
//	}
	
	//�ϴ�λ����Ϣ
	if(g_GridMay[mp.x][mp.y] != 0)
	{
		sendMes(g_GridMay[mp.x][mp.y]);
	}
}

void GridSetup(void)
{
	delay_ms(50);
	//�����ʼ����
	g_GridAct = getActDirState();
	MapInit();
	MapActivityRecord();
}

void GridLoop(void)
{
	//MapActivityRecord();
}





