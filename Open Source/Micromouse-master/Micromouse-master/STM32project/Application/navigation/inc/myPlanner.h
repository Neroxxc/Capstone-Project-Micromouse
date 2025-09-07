#ifndef __MYPLANNER_H
#define __MYPLANNER_H

#include "Odometer.h"


void plannerInit(void);



//·���滮
uint8_t routeSearcher(void);
//���Ŀ����������Ϣ
PosTypedef getTargetPos(void);
//��ýڵ�������Ϣ
PosTypedef getRoute(uint8_t cur);
//��ȡ�ڵ�����
uint8_t getPathCurNum(void);


//�޸�Ŀ���
void setTargetPos(PosTypedef tg);
//�޸Ľڵ���Ϣ
void setPathNode(uint8_t cur,PosTypedef mp);
//�������нڵ�����
void setPathCurNum(uint8_t num);



#endif /*__MYPLANNER_H*/

