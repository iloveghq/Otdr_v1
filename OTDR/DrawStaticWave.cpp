#include "stdafx.h"
#include "DrawStaticWave.h"

extern SorFileArray g_sorFileArray;

IMPLEMENT_DYNAMIC(CDrawStaticWave, CStatic)

CDrawStaticWave::CDrawStaticWave()
{
}

CDrawStaticWave::~CDrawStaticWave()
{
}

BEGIN_MESSAGE_MAP(CDrawStaticWave, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



void CDrawStaticWave::DrawBackGround(CDC *pDC) //���Ʊ���
{
	//��ȡ������ɫ
	CString strBack = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Back"));
	COLORREF colBack = _wtol(strBack);
	GetClientRect(m_drawRect);
	pDC->FillSolidRect(&m_drawRect, colBack);  //�������ڵľ�������
}

void CDrawStaticWave::DrawGridLine(CDC *pDC) //����������
{
	//����ʾ����
	if (!g_sorFileArray.waveConfig.ShowGrid)
		return;

	//��ȡդ������ɫ
	CString strGrid = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Grid"));
	COLORREF colGrid = _wtol(strGrid);
	GetClientRect(m_drawRect);

	CPen penGridLine;
	penGridLine.CreatePen (PS_DOT/*��*/, 1, colGrid);

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

void CDrawStaticWave::DrawLine(CDC *pDC) //����AB��
{
	//��ȡ�������ɫ
	CString strCursor = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Cursor"));
	COLORREF colCursor = _wtol(strCursor);
	GetClientRect(m_drawRect);

	CPen penCursorA, penCursorB;
	penCursorA.CreatePen (PS_SOLID/*ʵ��*/, 1, colCursor);
	penCursorB.CreatePen (PS_DOT/*��*/, 1, colCursor);

	float ndx = m_drawRect.Width() / 3;

	CPen* pOldPen = pDC->SelectObject (&penCursorA);
	int left1 = ndx;
	int left2 = 2 * ndx;
	int top = 0;
	int height = m_drawRect.Height();
	//Line 1
	pDC->MoveTo (left1, top);
	pDC->LineTo (left1, height);
	//Line 2
	pOldPen = pDC->SelectObject (&penCursorB);
	//double cursor
	if (g_sorFileArray.waveConfig.ShowDoubleCursor)
	{
		pDC->MoveTo (left2, top);
		pDC->LineTo (left2, height);
	}
	pDC->SelectObject(pOldPen);
	//release the gdi
	penCursorA.DeleteObject();
	penCursorB.DeleteObject();
}

void CDrawStaticWave::DrawCursor(CDC *pDC) //���ƹ��
{
	if (!g_sorFileArray.waveConfig.ShowCursor)
		return;

	//��ȡ�����ɫ
	CString strCursor = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Cursor"));
	COLORREF colCursor = _wtol(strCursor);
	GetClientRect(m_drawRect);

	CPen penCursor;
	penCursor.CreatePen (PS_SOLID/*��*/, 1, colCursor);

	float ndy = m_drawRect.Height () / 3;

	CPen* pOldPen = pDC->SelectObject (&penCursor);
	int right = m_drawRect.Width();
	int left = 0;
	int height = ndy;

	pDC->MoveTo (left,height);
	pDC->LineTo (right,height);

	pDC->SelectObject(pOldPen);
	//release the gdi
	penCursor.DeleteObject();
}

void CDrawStaticWave::OnPaint()
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
