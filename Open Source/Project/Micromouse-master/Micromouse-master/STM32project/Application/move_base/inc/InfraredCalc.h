#ifndef __INFRAREDCALC_H
#define __INFRAREDCALC_H

#include "stm32f10x.h"

typedef enum
{
	LS = 4,
	LF = 1,
	RF = 2,
	RS = 8,
	
}InfredTypedef;

typedef struct
{
	
	uint16_t LS;
	uint16_t LF;
	uint16_t RF;
	uint16_t RS;
	
}RedValTypedef;





void InfraredLoop(void);//��������ж���ѯ

int16_t getRedOffset(void);//���ƫ���� ʸ����ֵ �� ����
uint8_t getBarrierAct(void); //�ϰ���ֽ���������
uint8_t getActDirState(void);//��õ�ǰ����Ŀɻ��Ϣ
uint8_t getInfraredEventVal(void);//��ú��ⴥ�����
#endif /*__INFRAREDCALC_H*/
