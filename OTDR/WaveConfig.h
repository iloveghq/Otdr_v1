#pragma once

struct WaveConfig 
{
	COLORREF ColorCurve1;  //����1��ɫ
	COLORREF ColorCurve2;  //����2��ɫ
	COLORREF ColorCurve3;  //����3��ɫ
	COLORREF ColorCurve4;  //����4��ɫ
	COLORREF ColorCurve5;  //����5��ɫ
	COLORREF ColorCurve6;  //����6��ɫ
	COLORREF ColorCurve7;  //����7��ɫ
	COLORREF ColorCurve8;  //����8��ɫ
	COLORREF ColorBack;  //������ɫ
	COLORREF ColorGrid;  //դ����ɫ
	COLORREF ColorEvent;  //�¼���ɫ
	COLORREF ColorCursor;  //�����ɫ
	BOOL ShowGrid; //�Ƿ���ʾ����
	BOOL ShowCursor; //�Ƿ���ʾ���
	BOOL ShowDoubleCursor; //�Ƿ���ʾ˫���
	BOOL MultiMode; //�Ƿ�Ϊ��������ʾģʽ
	//Unit Page
	int UnitDbFig; //Dt�̶ȵ�λ����5\4\3
	int UnitDistFig; //����̶ȵ�λ����
	CString DistUnit; //���뵥λ
};