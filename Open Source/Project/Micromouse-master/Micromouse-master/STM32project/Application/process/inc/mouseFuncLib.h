#ifndef __MOUSEFUNCLIB_H
#define __MOUSEFUNCLIB_H


#include "drive.h"


void LEDFlash(uint32_t num);
//������ת����
void TestFuncMoveFastTern(void);

void SendMaoToUsart(void);

//���ͺ����������
void SendInfrared(void);

void ETR_Test(void);

//ֱ�߸��ٲ���
void TestFuncMoveFast(void);

//���ʹ���
void sendSMCCode(void);

#endif /*__MOUSEFUNCLIB_H*/



