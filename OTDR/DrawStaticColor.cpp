#include "stdafx.h"
#include "DrawStaticColor.h"

extern SorFileArray g_sorFileArray;

IMPLEMENT_DYNAMIC(CDrawStaticColor, CStatic)

CDrawStaticColor::CDrawStaticColor()
{
}

CDrawStaticColor::~CDrawStaticColor()
{
}

BEGIN_MESSAGE_MAP(CDrawStaticColor, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



void CDrawStaticColor::DrawBackGround(CDC *pDC) //���Ʊ���
{
	GetClientRect(m_drawRect);
	pDC->FillSolidRect(&m_drawRect, g_sorFileArray.waveConfig.ColorBack);  //�������ڵľ�������
}

void CDrawStaticColor::DrawGridLine(CDC *pDC) //����������
{
	//����ʾ����

	GetClientRect(m_drawRect);
	CPen penGridLine;
	penGridLine.CreatePen (PS_DOT/*��*/, 1, g_sorFileArray.waveConfig.ColorGrid);

	float ndx = m_drawRect.Width () / 5;
	float ndy = m_drawRect.Height () / 4;

	CPen* pOldPen = pDC->SelectObject (&penGridLine);
	//pDC->SetBkColor(CLR_OTDR_BACKGROUND);
	int bottom = m_drawRect.Height();
	int left = 0;
	int itemp;//
	for ( int i = 1; i <5; i++)
	{	
		itemp=left + ndx * i;
		//������̶ȴ��ϵ���-------------------------------------------------------
		pDC->MoveTo (itemp,bottom);
		pDC->LineTo (itemp,0);
	}
	//������:::	������
	for (int i=1; i < 4; i++)
	{
		itemp = bottom - ndy * i;

		pDC->MoveTo (left , itemp);
		pDC->LineTo (m_drawRect.right,itemp);

	}

	pDC->SelectObject(pOldPen);
	//release the gdi
	penGridLine.DeleteObject();
}

void CDrawStaticColor::DrawLine(CDC *pDC) //����������ɫ
{
	GetClientRect(m_drawRect);

	CPen penCursor;
	
	float ndy = m_drawRect.Height() / 10;

	CPen* pOldPen;
	int top = 0;
	int left = 10;
	int right = 90;
	int itemp;//
	COLORREF colCurve;
	for ( int i = 1; i <= 8; i++)
	{	
		colCurve = GetCurveColor(i);
		penCursor.CreatePen (PS_SOLID/*ʵ��*/, 1, colCurve);
		pOldPen = pDC->SelectObject (&penCursor);
		itemp = top + ndy * i;
		//������̶ȴ��ϵ���-------------------------------------------------------
		pDC->MoveTo (left,itemp);
		pDC->LineTo (right,itemp);
		//release the gdi
		penCursor.DeleteObject();
	}

	pDC->SelectObject(pOldPen);
}

COLORREF CDrawStaticColor::GetCurveColor(int nIndex)
{
	COLORREF colCurve;
	switch (nIndex)
	{
	case 1:
		colCurve = g_sorFileArray.waveConfig.ColorCurve1;
		break;
	case 2:
		colCurve = g_sorFileArray.waveConfig.ColorCurve2;
		break;
	case 3:
		colCurve =g_sorFileArray.waveConfig.ColorCurve3;
		break;
	case 4:
		colCurve = g_sorFileArray.waveConfig.ColorCurve4;
		break;
	case 5:
		colCurve = g_sorFileArray.waveConfig.ColorCurve5;
		break;
	case 6:
		colCurve = g_sorFileArray.waveConfig.ColorCurve6;
		break;
	case 7:
		colCurve = g_sorFileArray.waveConfig.ColorCurve7;
		break;
	case 8:
		colCurve =g_sorFileArray.waveConfig.ColorCurve8;
		break;
	Default:
		colCurve = RGB(0,0,0);
	}

	return colCurve;
}

void CDrawStaticColor::DrawCursor(CDC *pDC) //���ƹ��
{
	CPen penCursor;
	penCursor.CreatePen (PS_SOLID/*ʵ��*/, 1, g_sorFileArray.waveConfig.ColorCursor);

	float ndx = m_drawRect.Width() / 3;
	float ndy = m_drawRect.Height() / 6;

	CPen* pOldPen = pDC->SelectObject (&penCursor);
	int left = 2 * ndx;
	int height = 5 * ndy;
	int top = 0;
	int right = m_drawRect.Width();
	//Line 1
	pDC->MoveTo (left, top);
	pDC->LineTo (left, m_drawRect.Height());
	//Line 2
	pDC->MoveTo (0, height);
	pDC->LineTo (m_drawRect.Width(), height);

	pDC->SelectObject(pOldPen);
	//release the gdi
	penCursor.DeleteObject();
}

void CDrawStaticColor::OnPaint()
{
	CPaintDC dc(this);

	//ʹ��˫���弼����ͼ
	CRect ClientRect;
	GetClientRect(&ClientRect);

	CRect InvalidRect;
	dc.GetClipBox(&InvalidRect); //��ȡ��Ч��,����Ҫ�ػ������

	CDC MemDC;
	CBitmap MemBitMap;
	CBitmap *pOldBitMap;

	//����һ����dc���ݵ��ڴ��ڴ��豸����
	if (MemDC.CreateCompatibleDC(&dc))
	{
		//����һ����dc���ݵ�λͼ����СΪ�����ͻ���
		if (MemBitMap.CreateCompatibleBitmap(&dc,ClientRect.Width(),ClientRect.Height()))
		{
			//��λͼѡ���ڴ滷��
			pOldBitMap=MemDC.SelectObject(&MemBitMap);

			//ʹMemDC�������ͻ���������Ч��
			MemDC.SelectClipRgn(NULL);
			//�� "����" ��ȡ����Ч����ʹ�ڴ滷����dc��ȡ����Ч�����
			MemDC.IntersectClipRect(InvalidRect);

			//�ñ���ɫ��λͼ����ɾ�
			MemDC.FillSolidRect(&ClientRect,RGB(236,233,216));
		}

	}

	DrawBackGround(&MemDC);
	DrawGridLine(&MemDC);
	DrawLine(&MemDC);
	DrawCursor(&MemDC);

	//�ѻ��ƺõ�ͼ����BitBlt()"����"����Ļ��
	dc.BitBlt(0,0,ClientRect.Width(),ClientRect.Height(),&MemDC, 0, 0,SRCCOPY); 
	MemDC.SelectObject(pOldBitMap);
	MemBitMap.DeleteObject();
	MemDC.DeleteDC();
}
