#ifndef USART_H
#define USART_H
#include "stm32f10x.h"

typedef struct
{
	uint8_t Buf[1024];	//���ڽ��ջ�����
	uint8_t Over;			//���ڽ��ռ��
	uint16_t Len;			//���ڽ��ճ���
}UartBufTypedef;




//���ڳ�ʼ��
void UsartInit(u32 boud);
//���ڷ��ͺ���
void UsartSendData(u8 * tmp);
//���ڽ��պ�������
void USART_RxFuncConfig(void (* func)(uint8_t data,USART_TypeDef* USARTx));

#endif


