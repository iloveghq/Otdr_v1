// DrawStaticWave.h : ͷ�ļ�
//

#pragma once

class CDrawStaticWave : public CStatic
{
	DECLARE_DYNAMIC(CDrawStaticWave)

public:

	CDrawStaticWave();
	virtual ~CDrawStaticWave();

	void DrawBackGround(CDC *pDC); //���Ʊ���
	void DrawGridLine(CDC *pDC); //����������
	void DrawLine(CDC *pDC); //����AB��
	void DrawCursor(CDC *pDC); //���ƹ��

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

public:
	CRect m_drawRect;
};
