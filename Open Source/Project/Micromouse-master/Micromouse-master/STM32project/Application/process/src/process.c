/**
  ******************************************************************************
  * @file    process.c
  * @author  �Ŷ�
  * @version V1.0.0
  * @date    2019-9-21
  * @brief   ��������ڳ��󷽷���
  ******************************************************************************
  */
	
/*include file ---------------------------------------------------------------*/
#include "process.h"
#include "NavigationMain.h"
#include "MoveMain.h"
#include "mouseApp.h"
#include "comHandle.h"

void MainSetup(void)//system main setup
{
	comHandleInit();
	mouseApp_Setup();
	MoveSetup();
	NavigaSetup();
}

void MainLoop(void)//system main loop
{
	comHandleLoop();
	mouseApp_Loop();
	MoveLoop();
	NavigaLoop();
}

void MainInterrupt(void)//time 4 interrupt 1ms
{
	MoveInterrupt();
	NavigaInterrupt();
	mouseAppInterrupt();
}
