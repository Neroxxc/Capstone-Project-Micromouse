/**
  ******************************************************************************
  * @file    mapSaver.c
  * @author  �Ŷ�
  * @version V1.0.0
  * @date    2019-10-19
  * @brief   ��ͼ·����ȡ�뱣�����
  ******************************************************************************
  */
/*include file ---------------------------------------------------------------*/
#include "stmflash.h"
#include "mapSaver.h"
#include "myPlanner.h"


/*	��Ҫ������Ϣ�Լ���ַ�ֲ�
 *	 _______________________________________________
 *	|����											|	����				|	�ռ�	|
 *	|��������������������������������������������������|��������������������������|��������������|
 *	|·����Ϣ�Ƿ�������־λ 		|	uint8_t 		|	1			|
 *	|·���ڵ�����							|	uint8_t			|	1			|
 *	|·����������							|	uint8_t[][]	|	200		|
  *	|����											|							|				|
 *	|_______________________________________________|
 *	�ϼƴ�С��202�ֽ�
 *
 *  STM32F103RET6 FLASH Size 512K address 0x8000000 - 0x80800000
 *
 *	�����׵�ַ 0x8080000 - 0x200 = 0x8079800	
 *	��д��ռ� 512�ֽ� ������ռ��202�ֽ� ʣ��310�ֽ�
 *	
 */

/*Golbal Data Space ----------------------------------------------------------*/
#define FLASH_START_ADDRESS 		0X08010000
#define FLASH_BUFFER_SIZE				1024
#define FLASH_PATH_OVER_FLAG		0x55
#define FLASH_PATH_STATIC_FLAG  0xAA
#define FLASH_PATH_START				4

uint8_t flashCustomMap[FLASH_BUFFER_SIZE];		//flash��ȡ��д�뻺���� 400�ֽ�


//��ȡflash��������	400�ֽ�
void readFlashToBuf(void)
{
	STMFLASH_Read(FLASH_START_ADDRESS,(u16 *)flashCustomMap,FLASH_BUFFER_SIZE);
}

//��ȡ·��������ʹ�ܱ�־λ
uint8_t getPathOverFlag(void)
{
	//·��������־λ����λ
	if(flashCustomMap[0] == FLASH_PATH_OVER_FLAG)
	{
		return 1;
	}
	return 0;
}

//��ȡ�̶�·��ʹ�ܱ�־λ
uint8_t getStaticPathFlag(void)
{
	//�����ù̶�·��
	if(flashCustomMap[0] == FLASH_PATH_STATIC_FLAG)
	{
		return 1;
	}
	return 0;
}


//��ȡ·����Ϣ����·����Ϣд��·������
void writePathSeqFromFlash(void)
{
	//����·���ڵ�����
	setPathCurNum(flashCustomMap[1]);
	
	//����Ŀ���
	PosTypedef tg;
	
	tg.x = flashCustomMap[2];
	tg.y = flashCustomMap[3];
	
	setTargetPos(tg);
	
	//��·����Ϣд��·������
	for(int i = 0;i < 256; i++)
	{
		PosTypedef mp;
		mp.x = flashCustomMap[(i * 2 + 0) + FLASH_PATH_START];
		mp.y = flashCustomMap[(i * 2 + 1) + FLASH_PATH_START];
		
		setPathNode(i,mp);
	}
}


//���·����Ϣ
void delPathofFlash(void)
{
	//ɾ��·����ɱ�־λ
	flashCustomMap[0] = 0xFF;
	//��������д��flash
	STMFLASH_Write(FLASH_START_ADDRESS,(u16 *)flashCustomMap,FLASH_BUFFER_SIZE);
}


//д��·����Ϣ����λ��־λ
void writePathToFlash(void)
{
		//д��·�����ʹ�ܱ�־λ��������
	flashCustomMap[0] = FLASH_PATH_OVER_FLAG;
	
	//д��·���ڵ�������������
	flashCustomMap[1] = getPathCurNum();
	
	//д��Ŀ������굽FLASH
	flashCustomMap[2] = getTargetPos().x;
	flashCustomMap[3] =	getTargetPos().y;
	
	
	//д��·�����е�������
	for(int i = 0;i < 256; i++)
	{
		PosTypedef mp = getRoute(i);
		
		flashCustomMap[(i * 2 + 0) + FLASH_PATH_START] = mp.x;
		flashCustomMap[(i * 2 + 1) + FLASH_PATH_START] = mp.y;
	}
	__asm("CPSID I");
	//��������д��flash
	STMFLASH_Write(FLASH_START_ADDRESS,(u16 *)flashCustomMap,FLASH_BUFFER_SIZE);
	__asm("CPSIE I");
}


//д�뾲̬·����Ϣ����λ��־λ
void writeStaticPathToFlash(void)
{
		//д��·�����ʹ�ܱ�־λ��������
	flashCustomMap[0] = FLASH_PATH_STATIC_FLAG;
	
	//д��·���ڵ�������������
	flashCustomMap[1] = getPathCurNum();
	
	//д��Ŀ������굽FLASH
	flashCustomMap[2] = getTargetPos().x;
	flashCustomMap[3] =	getTargetPos().y;
	
	
	//д��·�����е�������
	for(int i = 0;i < 256; i++)
	{
		PosTypedef mp = getRoute(i);
		
		flashCustomMap[(i * 2 + 0) + FLASH_PATH_START] = mp.x;
		flashCustomMap[(i * 2 + 1) + FLASH_PATH_START] = mp.y;
	}
	__asm("CPSID I");
	//��������д��flash
	STMFLASH_Write(FLASH_START_ADDRESS,(u16 *)flashCustomMap,FLASH_BUFFER_SIZE);
	__asm("CPSIE I");
}


