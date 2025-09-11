/**
  ******************************************************************************
  * @file    pidctrller.c
  * @author  �Ŷ�
  * @version V1.0.0
  * @date    2019-9-21
  * @brief   �˶������� ����ٶ�΢�ֻ��ֱ��������� 0l 1r
  ******************************************************************************
  */
	
/*include file ---------------------------------------------------------------*/
#include "pidctrller.h"
#include "stm32f10x.h"
#include "drive.h"
#include "stdio.h"
#include "parameter.h"
/*Global Date Space ----------------------------------------------------------*/


const PIDParamBaseTypedef PARAM_PID = {//���pid�ٶȿ��Ʋ���

	.kp = MOTOR_SPEED_PID_KP,				//����Ȩ��
	.ki = MOTOR_SPEED_PID_KI,				//����Ȩ��
	.kd = MOTOR_SPEED_PID_KD,				//΢��Ȩ��
	
};

int16_t g_TargetSpeed[2]	 = {0,0};//Ŀ���ٶ�
int16_t g_MotorEncoder[2]	 = {0,0};//��ǰ�ٶ�

//�ٶ�ֵ ������ ����
void updataEncoderVal(void)
{
	g_MotorEncoder[0] = TIM2->CNT;  //l     
	TIM2->CNT = 0;
	g_MotorEncoder[1] = TIM3->CNT;	//r
	TIM3->CNT = 0;
}
//��õ���ĵ�ǰ״̬ 0 ֹͣ 1 ��ת
int8_t getMotorState(void)
{
	if(
			//�����
			(
				(g_MotorEncoder[0] < PIDCONTROLLER_MOTOR_STOP)  &&
				(g_MotorEncoder[0] > -PIDCONTROLLER_MOTOR_STOP) 
			)
			//�Ҳ���
			&&
			(
				(g_MotorEncoder[1] < PIDCONTROLLER_MOTOR_STOP)  &&
				(g_MotorEncoder[1] > -PIDCONTROLLER_MOTOR_STOP) 
			)
//			//Ŀ��״̬
//			&&
//			(
//				(g_TargetSpeed[0] == 0) &&
//				(g_TargetSpeed[1] == 0)
//			)
		)
	{
		return 0;
	}
	return 1;
}

//��ȡ�ٶ� ������ÿʱ��ֶ�
int16_t getEncoderVal(uint8_t cmd)
{
	if(cmd == 0)
	{
		return g_MotorEncoder[0];
	}
	else
	{	
		return g_MotorEncoder[1];
	}
}

//�趨Ŀ���ٶ�
void SetTargetSpeed(int16_t lift,int16_t right)
{
	g_TargetSpeed[0] = lift;
	g_TargetSpeed[1] = right;
}

//�趨�������
void setMotorPWM(int16_t lift,int16_t right)
{
	//1000
	//���������
	DianJi_TiaoSu(0x0a,lift);
	//�Ҳ�������
	DianJi_TiaoSu(0x03,right);
}
/**
  * @brief  MotorSpeedPidCtrl.
  * @note		PWM��̬���������㷨��λ��ʽ��ɢ��������΢�ַ��̡�
  *         Out = Kp[e(k)]+ki��e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
  *         ���л������΢�����ܴ��Ŷ���ֵ��Χ��Ч��־λ����
  * @retval None
  */
void MotorSpeedPidCtrl(void)
{
	static PIDDateBaseTypedef pid = {
		.de		[0] = 0,		.de		[1] = 0,
		.fe		[0] = 0,    .fe		[1] = 0,
		.de1	[0] = 0,    .de1	[1] = 0,
		.de2	[0] = 0,    .de2	[1] = 0,
		.out	[0] = 0,    .out	[1] = 0,
	};
	
	static int16_t 		lastspd[2]	 = {0,0};  //��һ�ε��ٶ�
	static uint32_t   stptick      = 0;      //ֹͣʱ��
	
	for(int i = 0;i < 2;i++)
	{
		//pid����
		
		//���㵱ǰ���ƶ���ʷ���
		pid.de2[i] 	 =  pid.de1[i];
		pid.de1[i] 	 =  pid.de[i];
		pid.de[i] 	 =  g_TargetSpeed[i] - getEncoderVal(i);
		pid.fe[i] 	+=  pid.de[i];
		//pid���������ķ���
		pid.out[i] 	= 	PARAM_PID.kp * pid.de[i] 										+ 
										PARAM_PID.ki * pid.fe[i] * (pid.de[i] < 100) + 
										PARAM_PID.kd * ( pid.de[i] - 2 * pid.de1[i] + pid.de2[i]) * (pid.de[i] < 100);
		//������Ʒ���
		pid.out[i] = (pid.out[i] > MOTOR_OUTPUT_MAX) ? MOTOR_OUTPUT_MAX : pid.out[i];
		pid.out[i] = (pid.out[i] <-MOTOR_OUTPUT_MAX) ?-MOTOR_OUTPUT_MAX : pid.out[i];
	
				
		//������
		
		//��һ���ٶȲ�Ϊ0��ǰ������
		if(lastspd[i] != 0 && g_TargetSpeed[i] == 0)
		{
			stptick = Sys_GetTick();
		}
		
		//Ŀ���ٶ�Ϊ0��ɲ��ʱ�䳬��3s
		if(g_TargetSpeed[i] == 0 && (Sys_GetTick() - stptick) > 3000)
		{
			pid.out[i] = pid.out[i]*2/3;
		}
	}
	
	//������������
	setMotorPWM(pid.out[0],pid.out[1]);
}

//void senddebug(void)
//{
//	static uint32_t div = 0;
//	div++;
//	if(div > 50)
//	{
//		div = 0;
//		uint8_t str[50];
//		sprintf((char *)str,"%f,%d,%d\r\n",pid.de[1],g_TargetSpeed[1],pid.out[1]);
//		UsartSendData(str); 	
//	}
//}

//pid������ִ�����ڷ�Ƶ��
void PID_Divider(void)
{
	static uint32_t div = 0;
	div++;
	if(div >= 10)
	{
		div = 0;
		updataEncoderVal();	//���±���������
		MotorSpeedPidCtrl();//ִ�е��PID������
	}
//	senddebug();
}
















