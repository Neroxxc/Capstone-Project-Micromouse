#ifndef __MOUSEAPP_H
#define __MOUSEAPP_H



typedef enum
{
	REDTEST		,	//�������ģʽ
	MAPCREAT	,	//��ͼ����ģʽ
	START			,	//���ģʽ
	WAIT			, //�ȴ�ģʽ
}MouseStateTypedef;




void mouseApp_Setup(void);
void mouseApp_Loop(void);
void mouseAppInterrupt(void);

#endif /*__MOUSEAPP_H*/
