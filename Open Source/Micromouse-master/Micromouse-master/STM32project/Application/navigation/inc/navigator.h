#ifndef __NAVIGATOR_H
#define __NAVIGATOR_H

#include "odometer.h"

//ͨ����һ��Ŀ�����㷽��
uint8_t navPosToDir(PosTypedef mp,PosTypedef nmp);
//�������������ƶ�
void moveNavigator(uint8_t dir,PosTypedef mp,void (* func)());
//���ø���ֱ�߿�����
void setLineFastI(uint8_t dis);
//�������֮��ľ���
uint8_t getDisGrid(PosTypedef mp,PosTypedef nmp);

//�ƶ��������԰�
void NavigatorTest(void);

//�ƶ�������̬·��
void NavigatorStaticPath(void);

//�ƶ��������ٵ�һ�汾
void NavigatorFastI(void);
//�ƶ��������ٵڶ��汾
void NavigatorFastII(void);


#endif /*__NAVIGATOR_H*/
