#ifndef __PIDCTRLLER_H
#define __PIDCTRLLER_H

#include "stm32f10x.h"

/* Structure type definition -------------------------------------------------*/
typedef struct
{
	float			de  [2];			//���������ǰ���ֵ difference error
	float			fe  [2];			//�������������
	float			de1 [2];			//���������ʷ���1
	float			de2 [2];			//���������ʷ���2
	int				out [2];			//����ͨ������PWM���
	
}PIDDateBaseTypedef;			//pid���ݽṹ����

typedef struct
{
	float			kp;						//����Ȩ��
	float			ki;						//����Ȩ��
	float			kd;						//΢��Ȩ��
	
}PIDParamBaseTypedef;			//pid�����ṹ����


void SetTargetSpeed(int16_t lift,int16_t right);
void MotorSpeedPidCtrl(void);
void PID_Divider(void);
int16_t getEncoderVal(uint8_t cmd);	//��ȡ�ٶ� ������ÿʱ��ֶ�
int8_t getMotorState(void);					//��õ���ĵ�ǰ״̬ 0 ֹͣ 1 ��ת
#endif /*PIDCTRLLER_H*/
