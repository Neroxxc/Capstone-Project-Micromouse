#ifndef __MOVECTRL_H
#define __MOVECTRL_H


typedef enum
{
	MStop,
	MForward,
	ML90,
	MR90,
	ML180,
	MR180,
	
}MoveCmdTypedef;

//�ƶ����ƻ�����
void mouseMove(MoveCmdTypedef move);
//�ƶ������Ż���һ��
void mouseMoveFastI(MoveCmdTypedef move);
//�ƶ����� ��� �����Ż���һ�汾
void mouseMoveSP(MoveCmdTypedef move);

#endif /*__MOVECTRL_H*/



