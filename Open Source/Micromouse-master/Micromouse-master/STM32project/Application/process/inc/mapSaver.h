#ifndef __MAPSAVER_H
#define __MAPSAVER_H

#include "odometer.h"

//��ȡflash��������	400�ֽ�
void readFlashToBuf(void);
//��ȡ·��������ʹ�ܱ�־λ
uint8_t getPathOverFlag(void);
//��ȡ�̶�·��ʹ�ܱ�־λ
uint8_t getStaticPathFlag(void);
	
//��ȡ·����Ϣ����·����Ϣд��·������
void writePathSeqFromFlash(void);
//���·����Ϣ
void delPathofFlash(void);
//д��·����Ϣ����λ��־λ
void writePathToFlash(void);
//д�뾲̬·����Ϣ����λ��־λ
void writeStaticPathToFlash(void);
#endif /*__MAPSVER_H*/
