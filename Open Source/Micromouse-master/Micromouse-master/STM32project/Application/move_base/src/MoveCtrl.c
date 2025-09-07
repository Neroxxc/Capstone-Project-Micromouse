/**
  ******************************************************************************
  * @file    MoveCtrl.c
  * @author  �Ŷ�
  * @version V1.0.0
  * @date    2019-9-28
  * @brief   ƽ̨�ƶ����Ƶ�������ִ�з������ٷ�װ
  ******************************************************************************
  */
/*include file ---------------------------------------------------------------*/
#include "MOveCtrl.h"
#include "attitude.h"
#include "pidctrller.h"
#include "odometer.h"
#include "parameter.h"
#include "InfraredCalc.h"

#define FASTENABLE 1

#define MCSPEED 700

/*ƽ̨���ƶ����ٷ�װ0
 *
 *	���·�����Ϊ����ִ��
 *	��ǰֱ������
 *	ԭ����תһ���̶��Ƕ�
 */
 
void mouseMove(MoveCmdTypedef move)
{
	
#ifdef FASTENABLE
	
	mouseMoveFastI(move);
	
#endif /*FASTENABLE*/
	
#ifndef FASTENABLE
	
	switch(move)
	{
		case MStop:
		{
			//�趨�ٶ�
			SearcherSpeed(0,0);
			//����ֹͣģʽ
			AttitudeStateSet(AStop);
			//�ȴ����ֹͣ
			while(getMotorState());
			break;
		}
		case MForward:
		{
			//�趨�ٶ�
			SearcherSpeed(MCSPEED,MCSPEED);
			//��������ģʽ
			AttitudeStateSet(ASStraight);
			break;
		}
		case ML90:
		{
			AttitudeStateSet(AStop);
			//�ȴ����ֹͣ
			while(getMotorState());
			//�趨�Ƕ�
			RotationAngle(-90);
			//������̬����ģʽ
			AttitudeStateSet(ARotate);
			//�ȴ���������
			while(getAttitudeState() != AStop);
			//�ȴ����ֹͣ
			while(getMotorState());
			//��̼���̬����
			MouseDirUpdate(90);
			break;
		}
		case MR90:
		{
			AttitudeStateSet(AStop);
			//�ȴ����ֹͣ
			while(getMotorState());
			//�趨�Ƕ�
			RotationAngle(90);
			//������̬����ģʽ
			AttitudeStateSet(ARotate);
			//�ȴ���������
			while(getAttitudeState() != AStop);
			//�ȴ����ֹͣ
			while(getMotorState());
			//��̼���̬����
			MouseDirUpdate(-90);
			
			break;
		}
		case ML180:
		{
			AttitudeStateSet(AStop);
			//�ȴ����ֹͣ
			while(getMotorState());
			//�趨�Ƕ�
			RotationAngle(-180);
			//������̬����ģʽ
			AttitudeStateSet(ARotate);
			//�ȴ���������
			while(getAttitudeState() != AStop);
			//�ȴ����ֹͣ
			while(getMotorState());
			//��̼���̬����
			MouseDirUpdate(180);
			break;
		}
		case MR180:
		{
			AttitudeStateSet(AStop);
			//�ȴ����ֹͣ
			while(getMotorState());
			//�趨�Ƕ�
			RotationAngle(180);
			//������̬����ģʽ
			AttitudeStateSet(ARotate);
			//�ȴ���������
			while(getAttitudeState() != AStop);
			//�ȴ����ֹͣ
			while(getMotorState());
			//��̼���̬����
			MouseDirUpdate(-180);
			break;
		}
		default:break;
	}
	
#endif /*FASTENABLE*/

}

//�ƶ����� ��Ѱ �����Ż���һ�汾
void mouseMoveFastI(MoveCmdTypedef move)
{
	switch(move)
	{
		case MStop://ֹͣ
		{
			//�趨�ٶ�
			SearcherSpeed(0,0);
			//����ֹͣģʽ
			AttitudeStateSet(AStop);
			//�ȴ����ֹͣ
			while(getMotorState());
			break;
		}
		case MForward://ǰ��
		{			
			//�趨�ٶ�
			SearcherSpeed(MCSPEED,MCSPEED);
			//��������ģʽ
			AttitudeStateSet(ASStraight);
			break;
		}
		case ML90://��ת90��
		{
			AttitudeStateSet(AStop);
			//�ȴ����ֹͣ
			while(getMotorState());
			//�趨�Ƕ�
			RotationAngle(-90);
			//������̬����ģʽ
			AttitudeStateSet(ARotateFast);
			//�ȴ���������
			while(RotateFastOver());
			//��̼���̬����
			MouseDirUpdate(90);
			break;
		}
		case MR90://��ת90��
		{
			AttitudeStateSet(AStop);
			//�ȴ����ֹͣ
			while(getMotorState());
			//�趨�Ƕ�
			RotationAngle(90);
			//������̬����ģʽ
			AttitudeStateSet(ARotateFast);
			//�ȴ���������
			while(RotateFastOver());
			//��̼���̬����
			MouseDirUpdate(-90);
			break;
		}
		case ML180://��ת180��
		{
			AttitudeStateSet(AStop);
			//�ȴ����ֹͣ
			while(getMotorState());
			//�趨�Ƕ�
			RotationAngle(-180);
			//������̬����ģʽ
			AttitudeStateSet(ARotateFast);
			//�ȴ���������
			while(RotateFastOver());
			//��̼���̬����
			MouseDirUpdate(180);
			break;
		}
		case MR180://��ת180��
		{
			AttitudeStateSet(AStop);
			//�ȴ����ֹͣ
			while(getMotorState());
			//�趨�Ƕ�
			RotationAngle(180);
			//������̬����ģʽ
			AttitudeStateSet(ARotateFast);
			//�ȴ���������
			while(RotateFastOver());
			//��̼���̬����
			MouseDirUpdate(-180);
			break;
		}
	}
}

//�ƶ����� ��� �����Ż���һ�汾
void mouseMoveSP(MoveCmdTypedef move)
{
		switch(move)
	{
		case MStop://ֹͣ
		{
			//�趨�ٶ�
			SearcherSpeed(0,0);
			//����ֹͣģʽ
			AttitudeStateSet(AStop);
			//�ȴ����ֹͣ
			while(getMotorState());
			break;
		}
		case MForward://ǰ��
		{			
			
			break;
		}
		case MR90://��ת90��
		{
			//while(getHalfGridIrEn());
			//�趨��ת�Ƕ�
			RotationAngle(90);
			//�趨��תģʽ
			AttitudeStateSet(AFRotate);
			//�ȴ���ת����
			while(getAttitudeState() == AFRotate);
			//��̼�����
			odometerPosAdd(1);
			//����ֱ�߿���������
			set_FSLEtrnum(ODOMETER_STD_GRID_ETR_VAL * (1 - ATTITUDE_FROTATION_TPOS));
			//��̼���̬����
			MouseDirUpdate(-90);
			setOdometerEtr(ODOMETER_STD_GRID_ETR_VAL * (1 - ATTITUDE_FROTATION_TPOS));
			
			setFastStMoveGridNum(15);
			AttitudeStateSet(AFStraight);
			break;
		}
		case ML90://��ת90��
		{
			//while(getHalfGridIrEn());
			//�趨��ת�Ƕ�
			RotationAngle(-90);
			//�趨��תģʽ
			AttitudeStateSet(AFRotate);
			//�ȴ���ת����
			while(getAttitudeState() == AFRotate);
			//��̼�����
			odometerPosAdd(1);
			//����ֱ�߿���������
			set_FSLEtrnum(ODOMETER_STD_GRID_ETR_VAL * (1 - ATTITUDE_FROTATION_TPOS));
			//��̼���̬����
			MouseDirUpdate(90);
			setOdometerEtr(ODOMETER_STD_GRID_ETR_VAL * (1 - ATTITUDE_FROTATION_TPOS));
			
			setFastStMoveGridNum(15);
			AttitudeStateSet(AFStraight);
			break;
		}
		case ML180://��ת180��
		{
			mouseMoveFastI(ML180);
			break;
		}
		case MR180://��ת180��
		{
			mouseMoveFastI(ML180);
			break;
		}
	}
}

