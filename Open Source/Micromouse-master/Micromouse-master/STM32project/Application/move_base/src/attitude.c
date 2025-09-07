/**
  ******************************************************************************
  * @file    attitude.c
  * @author  �Ŷ�
  * @version V1.0.0
  * @date    2019-9-23
  * @brief   ���й����еĶ�̬��̬�����������˶���ִ��
  ******************************************************************************
  */
/*include file ---------------------------------------------------------------*/
#include "attitude.h"
#include "InfraredCalc.h"
#include "pidctrller.h"
#include "parameter.h"
#include "odometer.h"

#include	"drive.h"
#include "stdio.h"
/*Golbal data space ----------------------------------------------------------*/
SpeedTypedef 		g_SearcherSpeed = {.lift = 0,.right = 0,};//��Ѱģʽֱ���н����ٶ��趨
AttStaTypedef		g_AttState = AStop;												//��̬����״̬״̬
int16_t 				g_rotateDegrees = 0;											//��̬�����Ƕ�
int8_t 					g_rotateFast = 0;													//������ת��ɱ�־λ
int16_t 				g_FSLGridnum = 0;													//�����ƶ����Ƶ�λ������


//��Ѱģʽֱ������ʱ�������ٶ��趨
void SearcherSpeed(uint16_t lift,uint16_t right)
{
	g_SearcherSpeed.lift = lift;
	g_SearcherSpeed.right = right;
}


//��̬����״̬��״̬����
void AttitudeStateSet(AttStaTypedef ast)
{
	g_AttState = ast;
}
//��̬����״̬״̬��ȡ
AttStaTypedef getAttitudeState(void)
{
	return g_AttState;
}

//��Ѱʱֱ�����е���̬����
void SearcherPidCtrller(void)
{
	//ֱ����̬������ʽ ����ƫ���������ٶ�������ʽ
	//out = target - k * offset
	//ƫ���� ��ֵ ��� - ƫ���� �Ҳ� + ƫ����
	
	//ƫ����ֵ�� �ٶȵ�  -60% ~ 60%
	
	SpeedTypedef speed;//����ٶ�
	
	int16_t offset = getRedOffset();//ƫ����
	float kp = 0.0; //������������̬����
	
	//��������ȡֵĿ�ģ���������ֵ���ٶȰٷֱȵ�-+60% 
	//�������� 3000 * kp = 0.6 * Speed��kp = (0.6 * Speed) / 3000 
	kp = (ATTITUDE_STRAIGHT_KP * 
				(float)((g_SearcherSpeed.lift + g_SearcherSpeed.right) / 2)) / ATTITUDE_STRAIGHT_INE_MAX;
	
	//����������
	offset = (offset >  ATTITUDE_STRAIGHT_INE_MAX) ?  ATTITUDE_STRAIGHT_INE_MAX : offset;
	offset = (offset < -ATTITUDE_STRAIGHT_INE_MAX) ? -ATTITUDE_STRAIGHT_INE_MAX : offset;
	
	speed.lift  = g_SearcherSpeed.lift + (kp * (float)(offset));//����ٶ�����
	speed.right = g_SearcherSpeed.right - (kp * (float)(offset));//�Ҳ��ٶ�����
	
	//�����ٶȵ��ٶȱջ�pid������
	SetTargetSpeed(speed.lift,speed.right);
	
//	//������Ϣ
//	uint8_t str[50];
//	sprintf((char *)str,"%d,%d\r\n",speed.lift,speed.right);
//	UsartSendData(str); 
	
}

//��Ѱģʽ ֱ����̬���������� ִ�����ڷ�Ƶ��
void SPC_Divider(void)
{
	static uint16_t div = 0;
	div++;
	if(div >= 10)//��Ƶ��10��Ƶs
	{
		div = 0;
		SearcherPidCtrller();
	}
}
/*Move fast -----------------------------------------------------------------*/
int32_t etrtemp[2] =  {0,0};

void FSL_OverDataDel(void)
{
		g_FSLGridnum = 0;
		etrtemp[0]	 = 0;
		etrtemp[1]	 = 0;
}
void set_FSLEtrnum(int32_t num)
{
		etrtemp[0]	 = num;
		etrtemp[1]	 = num;
}


//����ֱ���˶�λ�ƿ�����
void FSL_moveController(void)
{
	SpeedTypedef fs;

	
	//�ۻ�������
	etrtemp[0] += getEncoderVal(0);
	etrtemp[1] += getEncoderVal(1);
	
	//�����Լ����
	float xkp = ATTITUDE_STRAIGHT_FAST_KP;
	float et0 = ((g_FSLGridnum * ODOMETER_STD_GRID_ETR_VAL / 2) - etrtemp[0]);
	float et1 = ((g_FSLGridnum * ODOMETER_STD_GRID_ETR_VAL / 2) - etrtemp[1]);	
	
	//λ�õ���ٶȼ���
	//λ�ñ�������ʽ���ٶȼ���
	fs.lift  = xkp * et0;
	fs.right = xkp * et1;
	
	//�ٶȷ�������
	fs.lift  = (fs.lift  > ATTITUDE_STRAIGHT_SPD_LTD) ? ATTITUDE_STRAIGHT_SPD_LTD : fs.lift;
	fs.right = (fs.right > ATTITUDE_STRAIGHT_SPD_LTD) ? ATTITUDE_STRAIGHT_SPD_LTD : fs.right;
																					 
	fs.lift  = (fs.lift  < -ATTITUDE_STRAIGHT_SPD_LTD) ? -ATTITUDE_STRAIGHT_SPD_LTD : fs.lift;
	fs.right = (fs.right < -ATTITUDE_STRAIGHT_SPD_LTD) ? -ATTITUDE_STRAIGHT_SPD_LTD : fs.right;
	
	//��ǰ25%ʱ��֤�������ȷ������ǰ�ĺ�������
	//0-25%��̶�Ӧ�ٶ������ϵ 0-100% 
	float ksl = ((float)etrtemp[0] / (float)(g_FSLGridnum * ODOMETER_STD_GRID_ETR_VAL / 2)) / 0.2;
	float ksr = ((float)etrtemp[1] / (float)(g_FSLGridnum * ODOMETER_STD_GRID_ETR_VAL / 2)) / 0.2;
	
	//�ں�25%�͹������ȷ��ͣ������ڿɽ��ܷ�Χ��
	float kel = (et0 / (float)(g_FSLGridnum * ODOMETER_STD_GRID_ETR_VAL / 2)) / 0.1;
	float ker = (et1 / (float)(g_FSLGridnum * ODOMETER_STD_GRID_ETR_VAL / 2)) / 0.1;
	
	//ks ��������
	ksl = (ksl > 1.0) ? 1.0 : ksl;
	ksr = (ksr > 1.0) ? 1.0 : ksr;
	
	//ke ��������
	kel = (kel > 1.0) ? 1.0 : kel;
	ker = (ker > 1.0) ? 1.0 : ker;
	//Ԥ�Ⱥͽ������Ƽ��ٿ���
	fs.lift = fs.lift * ksl * kel;
	fs.right = fs.right * ksr * ker;
	
	//�ٶȾ�̬ƫ��
	fs.lift  = fs.lift + ATTITUDE_STRAIGHT_FAST_SET;
	fs.right = fs.right + ATTITUDE_STRAIGHT_FAST_SET;
	
	//ֱ����̬������ʽ ����ƫ���������ٶ�������ʽ
	//out = target - k * offset
	//ƫ���� ��ֵ ��� - ƫ���� �Ҳ� + ƫ����
	
	//ƫ����ֵ�� �ٶȵ�  -60% ~ 60%
	
	int16_t offset = getRedOffset();//ƫ����
	
	//������̬�������ٶ�ֵ��Χ�Ķ�̬���� 650-MAX = 0.3 - 0.1
	float kv = 0.1 + 0.2 * (
														(float)(
																			(ATTITUDE_STRAIGHT_SPD_LTD + ATTITUDE_STRAIGHT_FAST_SET) -
																			(((float)(getEncoderVal(0) + getEncoderVal(1)) / 2))
																		)
														/ //--------------------------------------------------------------------------
														(float)(
																				ATTITUDE_STRAIGHT_SPD_LTD + ATTITUDE_STRAIGHT_FAST_SET
																		)
													);
														
	//��������ȡֵĿ�ģ���������ֵ���ٶȰٷֱȵ�-+60% 
	//�������� 3000 * kp = 0.6 * Speed��kp = (0.6 * Speed) / 3000 
	float kp = (kv * (float)((fs.lift + fs.right) / 2)) / ATTITUDE_STRAIGHT_INE_MAX;
	
	//����������
	offset = (offset >  ATTITUDE_STRAIGHT_INE_MAX) ?  ATTITUDE_STRAIGHT_INE_MAX : offset;
	offset = (offset < -ATTITUDE_STRAIGHT_INE_MAX) ? -ATTITUDE_STRAIGHT_INE_MAX : offset;
	
	fs.lift = fs.lift + (kp * (float)(offset));//����ٶ�����
	fs.right = fs.right - (kp * (float)(offset));//�Ҳ��ٶ�����
	
	//�����ٶȵ��ٶȱջ�pid������
	SetTargetSpeed(fs.lift,fs.right);

}

//�趨Ŀ��λ��դ������
void setFastStMoveGridNum(uint16_t num)
{
	g_FSLGridnum = num;
}

//ֱ���˶����Ʒ�Ƶ��
void FSL_DividerFast(void)
{
	static uint16_t div = 0;
	div++;
	if(div >= 10)//��Ƶ��10��Ƶms
	{
		div = 0;
		FSL_moveController();
	}
}

/*Rotation ---------------------------------------------------------------*/

#define MAXSPEED 				ATTITUDE_ROTATION_OUT_MAX
#define POSD 						ATTITUDE_ROTATION_LOST_DR

//��ת��������������
const int32_t P90D 		= ATTITUDE_ROTATION_ETR__90;	//+90
const int32_t D90D 		= -ATTITUDE_ROTATION_ETR__90;	//-90
const int32_t P180D 	= ATTITUDE_ROTATION_ETR_180;	//+180
const int32_t D180D 	= -ATTITUDE_ROTATION_ETR_180;	//-180
const float		WKP			= ATTITUDE_ROTATION_KP_VALE;	//����������Ȩ��


//�趨��ת�Ƕ�
void RotationAngle(int16_t angel)
{
	g_rotateDegrees = angel;
}

//��ת������ ��Ҫ����ִ����� ��ɺ��Զ��޸���̬����״̬ �� ֹͣ
void RotateCtrller(int16_t de)
{

	static int32_t encoder[2] = {0,0};	//����������
	
	int32_t speed[2] = {0,0};
	int32_t angel = 0;//Ŀ�ĽǶ�
	
	switch(de)
	{
		case 90  	: angel = P90D; break;
		case -90 	: angel = D90D;break;
		case 180 	: angel = P180D;break;
		case -180 : angel = D180D;break;
	}
	
	//��ȡ��������������¼
	encoder[0] += getEncoderVal(0);
	encoder[1] -= getEncoderVal(1);
	
	//��ת��̬���� ���ñ���λ��ʽ������
	speed[0] = WKP * (angel/2  - encoder[0]);
	speed[1] = WKP * (angel/2  - encoder[1]);

	
	//�ٶȷ�������
	speed[0] = (speed[0] > MAXSPEED) ? MAXSPEED : speed[0];
	speed[1] = (speed[1] > MAXSPEED) ? MAXSPEED : speed[1];
	
	speed[0] = (speed[0] < -MAXSPEED) ? -MAXSPEED : speed[0];
	speed[1] = (speed[1] < -MAXSPEED) ? -MAXSPEED : speed[1];
	
	//Ŀ�ĽǶ�����ж�
	if((-POSD < ((angel/2) - encoder[0])) && (((angel/2) - encoder[0]) < POSD) &&
		 (-POSD < ((angel/2) - encoder[1])) && (((angel/2) - encoder[1]) < POSD))
	{
		//�����̬����״̬
		AttitudeStateSet(AStop);
		//���������������¼
		encoder[0] = 0;
		encoder[1] = 0;
		//�˳�������
		return;
	}
	
	//�����ٶ�
	SetTargetSpeed(speed[0],-speed[1]);
	
//	uint8_t str[50];
//	sprintf((char *)str,"%d,%d,%d,%d \r\n",red.LS,red.RS,red.LF,red.RF);
//	UsartSendData(str); 
}

//ԭ�� ��̬����������ִ�����ڷ�Ƶ��
void RDD_Divider(void)
{
	static uint16_t div = 0;
	div++;
	if(div >= 10)//��Ƶ��20��Ƶs
	{
		div = 0;
		RotateCtrller(g_rotateDegrees);
	}
}

//��ת������ �Ż��汾
void RotateCtrllerFast(int16_t de)
{
	static int32_t encoder[2] = {0,0};	//����������
	
	int32_t speed[2] = {0,0};
	int32_t angel[2] = {0,0};//Ŀ�ĽǶ�
	
	switch(de)
	{
		case 90  	: 
			angel[0] = P90D * ATTITUDE_ROTATION_FAST_VB * 0.5;
			angel[1] = P90D * ATTITUDE_ROTATION_FAST_VB * 0.5;
		break;
		case -90 	: 
			angel[0] = D90D * ATTITUDE_ROTATION_FAST_VB * 0.5;
			angel[1] = D90D * ATTITUDE_ROTATION_FAST_VB * 0.5;
		break;
		case 180 	: 
			angel[0] = P180D * ATTITUDE_ROTATION_FAST_VB * 0.5;
			angel[1] = P180D * ATTITUDE_ROTATION_FAST_VB * 0.5;
		break;
		case -180 : 
			angel[0] = D180D * ATTITUDE_ROTATION_FAST_VB * 0.5;
			angel[1] = D180D * ATTITUDE_ROTATION_FAST_VB * 0.5;
		break;
	}
	
	//��ȡ��������������¼
	encoder[0] += getEncoderVal(0) * 2;
	encoder[1] -= getEncoderVal(1) * 2;
	
	//��ת��̬���� ���ñ���λ��ʽ������
	speed[0] = WKP * (angel[0]  - encoder[0]);
	speed[1] = WKP * (angel[1]  - encoder[1]);
	
	//�ٶȷ�������
	speed[0] = (speed[0] > MAXSPEED) ? MAXSPEED : speed[0];
	speed[1] = (speed[1] > MAXSPEED) ? MAXSPEED : speed[1];
	
	speed[0] = (speed[0] < -MAXSPEED) ? -MAXSPEED : speed[0];
	speed[1] = (speed[1] < -MAXSPEED) ? -MAXSPEED : speed[1];
	
	//Ŀ�ĽǶ�����ж�
	if((-POSD < ((angel[0]) - encoder[0])) && (((angel[0]) - encoder[0]) < POSD) &&
		 (-POSD < ((angel[1]) - encoder[1])) && (((angel[1]) - encoder[1]) < POSD))
	{
		//���������������¼
		encoder[0] = 0;
		encoder[1] = 0;
		//�˳�������
		g_rotateFast = 1;//����
		return;
	}
	
	//�����ٶ�
	SetTargetSpeed(speed[0],-speed[1]);
}

uint8_t RotateFastOver(void)
{
	uint8_t temp = g_rotateFast;
	if(temp == 1 && g_rotateFast == 1)
	{
		g_rotateFast = 0;
		return 0;
	}
	return 1;
}

//ԭ�� ��̬����������ִ�����ڷ�Ƶ��
void RDD_DividerFast(void)
{
	static uint16_t div = 0;
	div++;
	if(div >= 10)//��Ƶ��20��Ƶs
	{
		div = 0;
		RotateCtrllerFast(g_rotateDegrees);
	}
}

/*turn move ctrl sp ---------------------------------------------------------*/
//2916.3etr/cm
const int32_t L1ETRSUM = ATTITUDE_FROTATION_ETR_L1; 	//120mm
const int32_t L2ETRSUM = ATTITUDE_FROTATION_ETR_L2;	//55mm

//��ת90��
void RotateMoveFPL90(void)
{
	//λ�Ʊջ�
	static int32_t etrsum[2] = {0,0,};
	
	//��¼����������
	etrsum[0] += getEncoderVal(0);
	etrsum[1] += getEncoderVal(1);
	
	//λ�Ʊջ��ٶȿ��Ʊ�������
	float kpx = ATTITUDE_FROTATION_X_KP;	//max 1500
	
	//���
	float et0 = ((float)L2ETRSUM - (float)etrsum[0]);
	float et1 = ((float)L1ETRSUM - (float)etrsum[1]);
	
	//�����ж�
	if(et0 < ATTITUDE_FROTATION_ETR_ET && et0 > -ATTITUDE_FROTATION_ETR_ET &&
		 (et1 / 1) < ATTITUDE_FROTATION_ETR_ET && 
		 (et1 / 1) > -ATTITUDE_FROTATION_ETR_ET)
	{
		etrsum[0] = 0;
		etrsum[1] = 0;
		AttitudeStateSet(AStop);
		return ;
	}
	//���㵱ǰ����ٶ�
	SpeedTypedef rs;
	rs.lift  = kpx * et0;
	rs.right = kpx * et1;
	
	rs.lift  *= 1;
	rs.right *= ATTITUDE_FROTATION_CRP;
	
	rs.right = (rs.right > 3000)?3000:rs.right;
	
	//�����ٶ�
	SetTargetSpeed(rs.lift,rs.right);
}
//��ת90��
void RotateMoveFPR90(void)
{
	//λ�Ʊջ�
	static int32_t etrsum[2] = {0,0,};
	
	//��¼����������
	etrsum[0] += getEncoderVal(0);
	etrsum[1] += getEncoderVal(1);
	
	//λ�Ʊջ��ٶȿ��Ʊ�������
	float kpx = ATTITUDE_FROTATION_X_KP;	//max 1500
	
	//���
	float et0 = ((float)L1ETRSUM - (float)etrsum[0]);
	float et1 = ((float)L2ETRSUM - (float)etrsum[1]);

	//�����ж�
	if((et0 / 1) < ATTITUDE_FROTATION_ETR_ET && 
		 (et0 / 1) > -ATTITUDE_FROTATION_ETR_ET &&
		 et1 < ATTITUDE_FROTATION_ETR_ET && et1 > -ATTITUDE_FROTATION_ETR_ET)
	{
		etrsum[0] = 0;
		etrsum[1] = 0;
		AttitudeStateSet(AStop);
		return ;
	}
	//���㵱ǰ����ٶ�
	SpeedTypedef rs;
	rs.lift  = kpx * et0;
	rs.right = kpx * et1;
	
	rs.lift  *= ATTITUDE_FROTATION_CRP;
	rs.right *= 1;
	
	
	rs.lift = (rs.lift > 3000)?3000:rs.lift;
	
	
	//�����ٶ�
	SetTargetSpeed(rs.lift,rs.right);
}


void RotateCtrllerFastI(int16_t de)
{
	switch(de)
	{
		case 90:
		{
			RotateMoveFPR90();
			break;
		}
		case -90:
		{
			RotateMoveFPL90();
			break;
		}
		default:break;
	}
}



//����ת���˶����Ʒ�Ƶ��
void FTR_DividerFast(void)
{
	static uint16_t div = 0;
	div++;
	if(div >= 10)//��Ƶ��10��Ƶms
	{
		div = 0;
		RotateCtrllerFastI(g_rotateDegrees);
	}
}
/*State Machine -------------------------------------------------------------*/

//��̬����״̬��
void AttitudeStateMachine(void)
{
	
	//
	switch(g_AttState)
	{
		case AStop://ֹͣ״̬ ֹͣ������в�ά��ֹͣ״̬
		{
			SetTargetSpeed(0,0);
			break;
		}
		case AWaiting://�ȴ�״̬ ���ٶȱջ������κβ���
		{
			break;
		}
		
		/*---------------------------------------------------------ֱ��*/
		case ASStraight://��Ѱģʽ ֱ�����еĶ�̬��̬����
		{
			SPC_Divider();
			break;
		}
		case AFStraight://ֱ�߼���ģʽ
		{
			FSL_DividerFast();
			break;
		}
		/*---------------------------------------------------------ת��*/
		case ARotate://ԭ�� ��̬���� ��תָ���Ƕ� 
		{
			RDD_Divider();
			break;
		}
		case ARotateFast://ԭ�� ��̬���� �Ż������һ�汾
		{
			RDD_DividerFast();
			break;
		}
		case AFRotate:	//����ת��
		{
			FTR_DividerFast();
			break;
		}
		default:break;
	}	
	
	//����ֱ��λ���������
	if(g_AttState != AFStraight)
	{
		FSL_OverDataDel();
	}
}



















