#ifndef __PARAMETER_H
#define __PARAMETER_H

//��Ҫ�����궨�弯�� 2019/10/10 ver.1.0.0 �Ŷ�

/* ����ƽ̨��ĵ������� 2019/10/12
 *
 * 1.��̬���� ֤ʵ����ڿɽ��ܷ�Χ��												[ֱ��״������ת�Ƕ�]
 * 2.�˶����� ֤ʵ����̼�����ʱ����ȷ����										[������̼����� ��������������ִ�й̶�·��] 
 * 3.������֪ ֤ʵ���˶�����̬���κ�����ʱ����ȷ������֪
 * 4.�ٶ��Ż� ȷ�ϻ�������ʱ���ٶȵ���
 * 5.�Զ����� ���ò�������̼Ƶ�ǰ���ϰ�λ�ø�������
 *
 */
 //300
 //700
 
/*=================================================���⴫����ϵ�в���*/


/*--------------------ƫ��ֵ��ز���*/
/*ʹ�ý�����ֵ����ƫ���� ʹ���м�ֵ��Ϊƫ�Ʒ���ֵ ʹ��Զ����ֵ��Ϊ�����������*/

#define INFRAREEDCALC_BASE_L				1500//1600//1300		 //�������� 	������ʱ���ֺ͹۲�ֵ��Ϊ�� 900
#define INFRAREEDCALC_BASE_R				2800//2900//2700     //�Ҳ������ 	������ʱ���ֺ͹۲�ֵ��Ϊ�� 900

#define INFRAREEDCALC_MISS_L			  400   	 //���ƫ����ֹ��ֵ	���С�ڸ�ֵʱ�����ò��Ȩ�� -650 + 900
#define INFRAREEDCALC_MISS_R			  700	 	 //�Ҳ�ƫ����ֹ��ֵ	�Ҳ�С�ڸ�ֵʱ�����ò��Ȩ�� -730 + 900

#define INFRAREEDCALC_LOST_L			  400      //��໷����ⶪʧ��ֵ	���С�ڸ�ֵʱ�����ò��Ȩ�� -650 + 900
#define INFRAREEDCALC_LOST_R			  700  	 	 //�Ҳ໷����ⶪʧ��ֵ	�Ҳ�С�ڸ�ֵʱ�����ò��Ȩ�� -730 + 900

/*---------------------�ֽ��ϰ��Ĵ����������� ������̼Ƶ����� [������������ײ����ֵ]*/
#define INFRAREEDCALC_OFFSET_LF			0				//���ƫ��
#define INFRAREEDCALC_OFFSET_RF			0 			//�Ҳ�ƫ��

#define INFRAREEDCALC_ACTVAL_LF			3000	//��༤��ֵ
#define INFRAREEDCALC_ACTVAL_RF			3200    //�Ҳ༤��ֵ

#define INFRAREEDCALC_THRESHOLDS		6000		//����ֵ

/*--------------------ǰ���ϰ���֪*/
#define INFRAREEDCALC_EN_LFACTVAL		1050		//��ǰ������ֵ		����෵����ֵС�ڸ�ֵʱ��Ϊ��ʧ	250
#define INFRAREEDCALC_EN_RFACTVAL		1450 		//��ǰ������ֵ		���Ҳ෵����ֵС�ڸ�ֵʱ��Ϊ��ʧ	250
#define INFRAREEDCALC_EN_FSACTVAL		2000    //ǰ�ͼ�����ֵ		��ǰ��������ֵ�����Ҷ����ڸ�ֵʱ��Ϊ���� 800


/*=================================================����������ϵ�в���*/

/*-----------------ԭ����ת��ر���������*/

#define	ATTITUDE_ROTATION_ETR__90		33000	//��ת90��ʱ�ı��������� 154714//33000
#define	ATTITUDE_ROTATION_ETR_180		65000	//180

#define ATTITUDE_ROTATION_LOST_DR		400		//��̬����ԭ����ת�������������

/*-----------------��̼Ƽ�¼��׼����������*/
#define ODOMETER_STD_GRID_ETR_VAL		52494		//һ����׼դ��ı�������ֵ 46200
#define ODOMETER_STD_GRID_ETR_FIX		0.8			//���� �ֽ��ϰ���ʱ���ٵı����� ������̼Ƶ�����
#define ODOMETER_STD_GRID_FAST_FIX	0.2			  //���� �ֽ��ϰ���ʱ���ٵı����� ������̼Ƶ�����

#define ODOMETER_ENVIR_SIDE_START		0.45		//�����⿪ʼ ǰ����������׼դ��ʱ�������༤�����
#define ODOMETER_ENVIR_SIDE_END			0.50		//���������ֵ

/*--------------------���ֹͣ�ж�*/
#define PIDCONTROLLER_MOTOR_STOP		10			//�������ֶ�ֵ����ֵС�����ֵ��ʱ������Ϊֹͣ

/*---------------------����ת��*/
#define ATTITUDE_FROTATION_ETR_L1		18000 //54970//22000//28402
#define ATTITUDE_FROTATION_ETR_L2		0	//25195//0//12826

/*=================================================�㷨 �ٶ�PID������ ��̬PID������ ϵ�в���*/

/*--------------����ٶȱջ�����PID����������*/

#define MOTOR_SPEED_PID_KP					1.500		//�ٶ�PID����������Ȩ��
#define MOTOR_SPEED_PID_KI					0.080		//�ٶ�PID����������Ȩ��
#define MOTOR_SPEED_PID_KD					0.000		//�ٶ�PID����������Ȩ��

#define MOTOR_OUTPUT_MAX						900			//���������ֵ���� MAX 1000

/*-------------ֱ��������̬����������ϵ�в���*/

/*------------ֱ�ߵ���*/
#define ATTITUDE_STRAIGHT_KP 				0.2		//ֱ�߱�����������̬�������� ��������ȡֵĿ�ģ���������ֵ���ٶȰٷֱ�
#define ATTITUDE_STRAIGHT_INE_MAX		1000		//ֱ������������ƫ��ֵ 

/*------------��ת����*/
#define	ATTITUDE_ROTATION_KP_VALE		0.1	//��ת�ٶȱ���������Ȩ��
#define ATTITUDE_ROTATION_OUT_MAX		650			//��̬����ԭ����ת����������������
#define ATTITUDE_ROTATION_FAST_VB		1.0			//���ٵ���ʱ�Ĳ��� 1.0Ϊ������ ���ջ���ֵ��������

/*----------------�����˶�����������*/

/*-------------����ת��*/
#define ATTITUDE_FROTATION_ETR_ET		3000		//������Χ
#define ATTITUDE_FROTATION_X_KP			0.05 		//��������������
#define ATTITUDE_FROTATION_CRP			2.81	//��ת�뾶��
#define ATTITUDE_FROTATION_TPOS			0.81		//ת��ǰ�ľ���

/*-------------����ֱ��*/
#define ATTITUDE_STRAIGHT_FAST_KP		0.024		//����ֱ��ʱ��λ�Ʊջ����Ƶ��ٶȱ���
#define ATTITUDE_STRAIGHT_FAST_SET	650			//����ֱ��ʱ���ٶ�ƫ�� ���ã���������ĩ�ڱ��������С���޷��ﵽλ��
#define ATTITUDE_STRAIGHT_SPD_LTD   1400		//����ֱ�е�����ٶ�����

/*=================================================���� ��ͼɨ����� �� ��ֹ��������*/

/*-----------------------------------��ͼɨ�����*/
#define ERGODICMAP_SEARCHOR_PLAN		2				//��ͼ��Ѱ�߼ƻ� 0 ���������� 1����������

/*-----------------------------------ɨ����ֹ��������*/
#define ERGODICMAP_STOP_ALLCPLT			0			//ȷ�ϵ�ͼ������


/*-----------------------------------�յ����������*/

#define ERGODICMAP_TARGET_POS_1			[0].x = 7,[0].y = 7		//�������4��Ŀ��� ����ο�������̼�����ϵ
#define ERGODICMAP_TARGET_POS_2		  [1].x = 7,[1].y = 8
#define ERGODICMAP_TARGET_POS_3			[2].x = 8,[2].y = 7
#define ERGODICMAP_TARGET_POS_4		  [3].x = 8,[3].y = 8


#endif /*__PARAMETER_H*/

