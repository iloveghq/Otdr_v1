// DrawStaticColor.h : ͷ�ļ�
//

#pragma once

class CDrawStaticColor : public CStatic
{
	DECLARE_DYNAMIC(CDrawStaticColor)

public:

	CDrawStaticColor();
	virtual ~CDrawStaticColor();

	void DrawBackGround(CDC *pDC); //���Ʊ���
	void DrawGridLine(CDC *pDC); //����������
	void DrawLine(CDC *pDC); //����AB��
	void DrawCursor(CDC *pDC); //���ƹ��
	COLORREF GetCurveColor(int nIndex);

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

public:
	CRect m_drawRect;
};
