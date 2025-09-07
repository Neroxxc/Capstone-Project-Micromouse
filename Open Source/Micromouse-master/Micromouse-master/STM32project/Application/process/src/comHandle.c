/**
  ******************************************************************************
  * @file    comHandle.c
  * @author  �Ŷ�
  * @version V1.0.0
  * @date    2019-11-26
  * @brief   ͨ�����ݴ���
  ******************************************************************************
  */
/*include file ---------------------------------------------------------------*/
#include "comHandle.h"
#include "stm32f10x.h"
#include "usart.h"
#include "stdio.h"
#include "odometer.h"
#include "myPlanner.h"
#include "mapSaver.h"

/*Golbal data space ----------------------------------------------------------*/
#define UART_COM_STAT   	'<'				//����ͨ�ſ�ʼ�ֽ�
#define UART_COM_END			'>'				//����ͨ�Ž����ֽ�


UartBufTypedef g_ComData;				//���ڽ��ջ�����


//�����ֽ���д�뵽������
void USART_RxToBuf(uint8_t data,USART_TypeDef* USARTx)
{
	uint8_t cur = 0;
	//������д��
	
	//��һ����Ϣ�������
	if(g_ComData.Over == 0)
	{
		//��ʼ
		if(data == UART_COM_STAT )
		{
			//��������
			g_ComData.Len = 0;
		}
		//����
		else if(data == UART_COM_END)
		{
			//���ս���
			g_ComData.Over = 1;
		}
		//����
		else
		{
			//д������
			g_ComData.Buf[g_ComData.Len] = data;
			//�ƶ����
			g_ComData.Len = (g_ComData.Len + 1) % 1024;
		}
	}
}

//����̬·��д��flash
void PathWrite(uint8_t * cmd)
{
	//·��д��Э��
	//<PXXX...XXX>
	// "X"��������Ԫ
	//01234 ����5
	//XXYY,
	
	uint16_t cur = 0; //������Ĺ��
	
	uint16_t x = 0;	//x����
	uint16_t y = 0;	//y����
	
	uint16_t cur_buf = 1;//��������ȡ���
	
	//��ӡ��Ϣ
	UsartSendData("\r\n");
	UsartSendData(cmd);
	UsartSendData("\r\n");
	
	//�������볬�����ֵ����
	while(x != 99 && y != 99 && cur < 250)
	{
		//�������
		x = (cmd[cur_buf + 0] - 0x30) * 10 + 
				(cmd[cur_buf + 1] - 0x30) * 1;
		
		y = (cmd[cur_buf + 2] - 0x30) * 10 + 
		    (cmd[cur_buf + 3] - 0x30) * 1;
		
		
		PosTypedef pos;
		
		pos.x = x;
		pos.y = y;
		
		//������д��·������
		setPathNode(cur,pos);
		
		//�ƶ��ڵ���
		cur++;
		
		//�ƶ���������ȡ���
		cur_buf += 5;
		
		//��ӡ�������Ϣ
		uint8_t str[50];
		sprintf((char *)str,"P%2d,X=%2d,Y=%2d.\r\n",cur,pos.x,pos.y);
		UsartSendData(str);
	}
	
	//���ýڵ�����
	setPathCurNum(cur);
	//������д��flash
	writeStaticPathToFlash();
	
	UsartSendData("\r\n");
	UsartSendData("OK \r\n");
}

//���ݽ��մ���
void USART3_RxBufAnalysis(UartBufTypedef * buf)
{
	//�����Ա�ʹ��
	if(buf->Over == 1)
	{	
		//ͨ��Э��
		//��ʼ ��<�� ���� '>'
		//[0] ��ʶ [n] ����
		switch(buf->Buf[0])
		{
			case 'P'://��̬·��
			{
				PathWrite(buf->Buf);
				break;
			}
		}
		//���δ��ɱ�־λ
		buf->Over = 0;
	}
}

void comHandleInit(void)
{
	//���ý��պ���
	USART_RxFuncConfig(USART_RxToBuf);
}

void comHandleLoop(void)
{
	//���ս���
	USART3_RxBufAnalysis(&g_ComData);
}











