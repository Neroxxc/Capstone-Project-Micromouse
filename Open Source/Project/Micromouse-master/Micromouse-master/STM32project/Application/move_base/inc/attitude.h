#ifndef __ATTITUDE_H
#define __ATTITUDE_H

#include "stm32f10x.h"

typedef enum
{
	AStop					= 0,	//ֹͣģʽ
	AWaiting			= 1,	//��̬�����ȴ�ģʽ
	ASStraight 		= 2,	//��Ѱģʽ ֱ��
	ARotate				= 3,	//ԭ����ת
	ARotateFast		= 4,	//ԭ����ת���ٰ�
	AFStraight		= 5,	//ֱ�߼����Ż�1
	AFRotate			= 6,	//����ת��
	
}AttStaTypedef;//��̬����״̬��


typedef struct
{
	int lift;//����ٶ�
	int right;//�Ҳ��ٶ�
	
}SpeedTypedef;//�ٶ����ݽṹ



//��Ѱģʽֱ������ʱ�������ٶ��趨
void SearcherSpeed(uint16_t lift,uint16_t right);
//�趨��ת�Ƕ�
void RotationAngle(int16_t angel);
//����ת����ɱ�־λ
uint8_t RotateFastOver(void);

//�趨Ŀ��λ��դ������
void setFastStMoveGridNum(uint16_t num);
//�趨����ֱ�߱������ۻ���ʼֵ
void set_FSLEtrnum(int32_t num);

//��̬����״̬��״̬����
void AttitudeStateSet(AttStaTypedef ast);
//��̬����״̬״̬��ȡ
AttStaTypedef getAttitudeState(void);
//��̬����״̬��
void AttitudeStateMachine(void);

#endif /*__ATTITUDE_H*/
