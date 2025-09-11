/**
  ******************************************************************************
  * @file    NavigationMain.c
  * @author  �Ŷ�
  * @version V1.0.0
  * @date    2019-9-28
  * @brief   ������������
  ******************************************************************************
  */
/*include file ---------------------------------------------------------------*/
#include "NavigationMain.h"
#include "odometer.h"
#include "OccupyingGrid.h"
#include "ErgodicMap.h"


void NavigaSetup(void)
{
	GridSetup();
	MapCreatorInit();
}

void NavigaLoop(void)
{
	//MapCreatLoop();
	
}
void NavigaInterrupt(void)
{
	odometerLoop();
	
}










