#ifndef __OCCUPYINGGRID_H
#define __OCCUPYINGGRID_H

#include "stm32f10x.h"

/*
				L4B 					Active	Inactive
				|---B0	0��     1				0
				|---B1  90��    1        0
				|---B2	180��   1        0
				|---B3	270��	 1        0

*/

//��ȡ��ͼ
uint8_t readMapGrid(uint8_t x,uint8_t y);
//д���ͼ
void writeMapGrid(uint8_t x,uint8_t y,uint8_t mp);

//�����ͼ�����Ϣ 0����1ǰ��
void GridActSave(uint8_t num);
//դ���ͼ�ļ�¼
void MapActivityRecord(void);

void GridSetup(void);
void GridLoop(void);


#endif /*__OCCUPYINGGRID_H*/
