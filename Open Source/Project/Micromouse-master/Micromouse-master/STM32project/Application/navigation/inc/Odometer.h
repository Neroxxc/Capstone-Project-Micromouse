#ifndef __ODOMETER_H
#define __ODOMETER_H

#include "stm32f10x.h"

//��ͼ����ϵ����Է���Ķ���
//����ƽ��ֱ������ϵ
/*	
		//������Ķ���
		
		y������
		|	
		|	th	      90 	
		|			180	 ��̬  0
		|					 270
		|
		|
		|
		|��ʼ��
		------------------------ X ������
*/




typedef struct
{
	int x;
	int y;
	int th;
}PosTypedef;//λ����̬���Ͷ���


//�趨��̼Ƹ�������ƫ����С����ֵ
void setOdometerFixOff(float num);
//�趨�������ۼ�ֵ
void setOdometerEtr(int32_t num);
//��ȡ�������ۼ�ֵ
int32_t readOdometerEtr(void);
//ʹ�õ�ǰ��̬�޸����λ��
void odometerPosAdd(uint8_t num);

//��ȡƽ̨λ����Ϣ
PosTypedef getMousePos(void);
//ƽ̨��̬����
void MouseDirUpdate(int th);
//�����������״̬
uint8_t getPosState(void);
//��̼Ƽ�¼ѭ��
void odometerLoop(void);

#endif /*__ODOMETER_H*/
