// OtdrTrace.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OTDR.h"
#include "OtdrTrace.h"
#include "CursorInfo.h"
extern SorFileArray g_sorFileArray;
CursorInfo g_cursorInfo;

COLORREF g_COLORREF[12];
int g_nTraceYmax;       //lzy2014.06.24

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
FLOAT CGraphElement::m_fRangeX[2];//���浱ǰ��ͼʱ��С�����룬��Զ�������
FLOAT CGraphElement::m_fRangeY[2];//���浱ǰ��ͼʱy�᷶Χ����Զ�������
FLOAT CGraphElement::m_fMaxDistance = 0;//����룬��Զ�������
const unsigned C_zll = 299792458 ; //����еĹ���m
//CGraphElement

// CSorDrawStatic �Ի���

INT CGraphElement::DistanceToIndex(FLOAT fDistance)
{
	ASSERT(0 != m_fDataSpacing);
	return (fDistance-0.001) / m_fDataSpacing;//zll 2013.12.6 (fDistance-0.001)��Ҫ��0.001 ��������ϴ�ȡ����+1
}
//��������
void CGraphElement::DrawCurve(CDC* pDC, CRect rect,int nIndexCurve)
 {
	ASSERT(NULL != m_pData);
	//�ȼ�����Ҫ������Щ��

	INT nStartIndex= DistanceToIndex(m_fRangeX[0]);
	INT nEndIndex = DistanceToIndex(m_fRangeX[1]);
	if (nStartIndex >= m_nPointsCount)
	{
		return;
	}//eif

	if (nEndIndex - nStartIndex < rect.Width())
	{
		nEndIndex++;
	}
	if (nEndIndex >= m_nPointsCount)
	{
		nEndIndex = m_nPointsCount - 1;
	}//eif

	//ASSERT(nStartIndex >= 0);
	if (nStartIndex < 0)
	{
		return;
	}//eif
	//ASSERT(nEndIndex <= m_nPointsCount);
	//TRACE(_T("%d %d  \n"),nStartIndex,nEndIndex);
	//���㵱ǰ����ˮƽ������ռ��������
	INT nXAxis = rect.Width();//
	if (m_fRangeX[1] > m_fLength)
	{//�������󳤶�
		nXAxis = ((m_fLength - m_fRangeX[0]) / (m_fRangeX[1] - m_fRangeX[0])) * nXAxis;
	}//
	
	//�����ͼ��
	INT nPointCountToDraw = nEndIndex - nStartIndex + 1;//�ܹ���ͼ��

	CPoint* pPointsToDraw = new CPoint[nPointCountToDraw];
	FLOAT fTempX = ((FLOAT)nXAxis) / nPointCountToDraw;//1ÿһ��ռx��೤
	FLOAT fTempY = rect.Height() / (m_fRangeY[1] - m_fRangeY[0]);
	FLOAT fYConst =  rect.bottom + rect.Height() * m_fRangeY[0]  / (m_fRangeY[1] - m_fRangeY[0]) + m_nOffsetY;
	int nTemp;
	INT nIndex = 0;
	//fX = (nTemp * m_fDataSpacing - m_fRangeX[OTDR_TRACE_MIN])*rect.Width() / (m_fRangeX[OTDR_TRACE_MAX] - m_fRangeX[OTDR_TRACE_MIN]);
	for (int i = nStartIndex; i <= nEndIndex; i++)
	{
		////pPointsToDraw->x = i * nXAxis / nPointCountToDraw;
		//pPointsToDraw[nIndex].x = nIndex * fTempX;
		pPointsToDraw[nIndex].x = (m_fDataSpacing * i - m_fRangeX[OTDR_TRACE_MIN])*rect.Width() / (m_fRangeX[OTDR_TRACE_MAX] - m_fRangeX[OTDR_TRACE_MIN]);
		////pPointsToDraw[nIndex].y = rect.bottom - ((m_pData[i] - m_fRangeY[0]) / (m_fRangeY[1] - m_fRangeY[0]) * rect.Height());//��������
		//pPointsToDraw[nIndex].y = fYConst - m_pData[i] * fTempY;//
#ifdef COMPILE_SHOW_SMOOTH_DATA////��ʾʱ��ƽ���������ݣ�����ʱ��ʵ������
		pPointsToDraw[nIndex].y = fYConst - m_pDataSmooth[i] * fTempY;//
#else
		//pPointsToDraw[nIndex].y = fYConst - m_pData[i] * fTempY - 40;//zll 2013.6.3 -40 ʹ������y�������ƶ�
		pPointsToDraw[nIndex].y = fYConst - m_pData[i] * fTempY;//����ƽ�ƣ�������Ч��ʾ��Χ������lzy2014.7.16
		
#endif
		nIndex++;
	}//efor
	//����

	//TRACE(_T("first data = %f ; last data = %f\n"), m_pData[nStartIndex + 100], m_pData[nEndIndex]);//lzy2014.7.16

	CPen pen;
	if (bSelected)
	{
		pen.CreatePen(PS_SOLID, 1, OTDR_TRACE_COLOR_SELECT);
	}
	else
	{
		pen.CreatePen(PS_SOLID, 1, g_COLORREF[nIndexCurve]);
	}//
	CPen* pOldPen = pDC->SelectObject(&pen);
	Polyline(pDC->GetSafeHdc(),pPointsToDraw, nPointCountToDraw);
	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
	delete[] pPointsToDraw;

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//���¼��б�
	if (!m_bShowEvent)
	{
		return;
	}//eif

	////zll 2013.5.11 ������A��B���Աߵ���������
	CFont   font;   
	CFont   *pOldFont;   
	pDC->SetTextColor(g_COLORREF[10]);//zll 2013.5.12
	pDC->SetBkMode(TRANSPARENT);
	//font.CreateFont(18, 0, 900, 900, FW_NORMAL,//��������2700
	//	0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
	//	CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	//	DEFAULT_PITCH,_T("@Arial")); 
	font.CreateFont(16, 0, 900, 900, FW_NORMAL,//��������2700,��̫���ˣ���С�㡣lzy2014.7.15
		0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH,_T("@Arial"));
	pOldFont = pDC->SelectObject(&font);

	////zll 2013.5.11 �������½����ο��е���ֵ����������
	CFont   fontValue;   
	CFont   *pOldFontValue;   
	fontValue.CreateFont(16, 0, 0, 0, FW_BOLD,//��������2700
		0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH,_T("@Arial")); 
	pOldFontValue = pDC->SelectObject(&fontValue);


	CPen penLineVertical;//��ֱ��
	penLineVertical.CreatePen(PS_SOLID, 1, OTDR_SQUARE_FRAME_COLOR);
	//if (bSelected)
	//{
	//	pen.CreatePen(PS_SOLID, 1, OTDR_TRACE_COLOR_SELECT);
	//}
	//else
	//{
	//	pen.CreatePen(PS_SOLID, 1, m_LineColor);
	//}//
	CBrush *pOldBrush = (CBrush *)pDC->SelectStockObject(NULL_BRUSH);
	pOldPen = pDC->SelectObject(&penLineVertical);
	CString temp;
	int fX,fY;
	const int LINE_TOP = -10, LINE_BOTTOM = 20;   //���ߵ���ʼ�����λ��ƫ��ֵ��lzy2014.8.6
	CRect rectTemp;
	rectTemp.SetRectEmpty();//zll 2013.5.24 ��ʼ��A��B���Աߵ�����ʾ�򣬷���λ���ҵ���release���޷���ʾA��B���Աߵ���
	for (int i=0; i<m_vAllEvents.size(); i++)
	{		
		CRect rectLineBottom;//rectLineBottom:ֱ���·��ľ���wcq2011.11.16
		nTemp = m_vAllEvents[i].m_nIndex;

		//TRACE(_T("event index:%d\n"),nTemp);
		if ( (nTemp >= nStartIndex ) &&(nTemp <= nEndIndex))
		{
			//fX = (nTemp - nStartIndex) * fTempX;
			fX = (m_vAllEvents[i].m_distance - m_fRangeX[OTDR_TRACE_MIN])*rect.Width() / (m_fRangeX[OTDR_TRACE_MAX] - m_fRangeX[OTDR_TRACE_MIN]);
			//fY = fYConst - m_pData[nTemp] * fTempY - 40;//zll
			fY = fYConst - m_pData[nTemp] * fTempY;   //lzy2014.7.16
			//����ֱ��
			pDC->MoveTo(fX, fY + LINE_TOP);//ʹ��ֱ����y�������ƶ�
			pDC->LineTo(fX, fY + LINE_BOTTOM);
			//��ֱ�������������
			rectLineBottom.SetRect(fX - 8,fY + LINE_BOTTOM,fX - 8 + 16, fY + LINE_BOTTOM + 18);
			if (i>8)//��λ��
			{
				rectLineBottom.right += 2;
			}
			pDC->Rectangle(&rectLineBottom);

			temp.Format(_T("%d"),i + 1);
			pDC->SelectObject(&fontValue);////zll 2013.5.11 ������A��B���Աߵ���������
			rectLineBottom.DeflateRect(2,2);
			pDC->DrawText(temp,&rectLineBottom,DT_CENTER | DT_VCENTER | DT_SINGLELINE);//���ο��е���ֵ

			//�����ı�
			float fVal=m_vAllEvents[i].m_distance/Divisor_toKm;
			if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
			{
				temp.Format(_T("%.3fkm"), fVal);
			}
			else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪmiles
			{
				fVal=KmeterToOther(fVal,UNIT_SHOW_MILE);
				temp.Format(_T("%.3fmiles"), fVal);//zll 2013.5.9
			}
			else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkft
			{
				fVal=KmeterToOther(fVal,UNIT_SHOW_KFEET);
				temp.Format(_T("%.3fkft"), fVal);//zll 2013.5.9
			}

			CSize sizeText = pDC->GetTextExtent(temp);//�õ��ı������
			pOldFont = pDC->SelectObject(&font);
			pDC->DrawText(temp, -1, rectTemp, DT_WORDBREAK | DT_CENTER | DT_CALCRECT);////zll 2013.5.11 ������A��B���Աߵ���������
			//pDC->TextOut(fX - rectTemp.Height() / 2, fY + rectTemp.Width() + 20, temp);
			pDC->TextOut(fX - sizeText.cy / 2, rectLineBottom.bottom + sizeText.cx, temp);

		}//eif
	}//efor

	pDC->SelectObject(&pOldFont);
	pDC->SelectObject(pOldPen);
	font.DeleteObject();
	penLineVertical.DeleteObject();

}
void CGraphElement::SetYOffset(INT nOffset)
{
	m_nOffsetY += nOffset;
}
//void CGraphElement::DrawEvents(CDC* pDC, INT nStartIndex, INT nEndIndex)
//{
//}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
// COtdrTrace

IMPLEMENT_DYNAMIC(COtdrTrace, CStatic)

COtdrTrace::COtdrTrace()
{
	m_bLeftButtonDown = false;
	m_nCurMode = CURVE_ZOOM_IN;
	m_rectMouseSelect.SetRectEmpty();
	m_rectCurve.SetRectEmpty();
	m_bInitPaint = FALSE;
	m_bShowLineAB = FALSE;  //��ʼ������ʾAB��
	m_bShowScale = TRUE;   //��ʼ����ʾ����
	m_cursorA.m_strAOrB = _T("A");
	m_cursorB.m_strAOrB = _T("B");
	m_cursorA.m_strDistance = _T("0km");
	m_cursorB.m_strDistance = _T("0km");
	m_cursorA.m_strDB= _T("0dB");
	m_cursorB.m_strDB= _T("0dB");
	m_nCurABSele = OTDRTRACE_AB_SELE_NONE;//Ĭ��δѡ��AB
	m_nABSele = OTDRTRACE_AB_SELE_A;
	m_nCurveIndex = 0;
	m_fMaxDistance = 0;
	m_bDrawGrid = TRUE;//zll 2013.4.22
	m_nSelectCursor = CURSOR_SELECT::CURSOR_A; //Ĭ��ѡ��A
	m_nPointA = 0;
	m_nPointB = 0;
	m_ptMouse.SetPoint(0, 0);

	m_nMaxIndex = 0;
	m_nStartPoint=0;
	m_nMaxPoint= INVALID_MAX_POINT;//ԭΪ0,wcq2012.02.20

	m_wdGridH = COUNT_GRIDLINE_HORIZONE;  //�������Ը���ʵ�������Զ�������lzy2014.7.16
	m_wdGridV = COUNT_GRIDLINE_VERTICAL;	
}

COtdrTrace::~COtdrTrace()
{
	for(int i=0; i< m_vAllData.size(); i++)
	{
		if (m_vAllData[i].m_pData)
		{
			delete[] m_vAllData[i].m_pData;
		}//eif
	}//efor
}


BEGIN_MESSAGE_MAP(COtdrTrace, CButton)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_AB_SELECT, &COtdrTrace::OnShowAB)
	ON_COMMAND(ID_TRACE_SCALE, &COtdrTrace::OnShowScale)
	ON_COMMAND(ID_EVENT_LIST_SHOW, &COtdrTrace::OnShowEventList)
END_MESSAGE_MAP()



// COtdrTrace ��Ϣ�������



void COtdrTrace::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CStatic::OnPaint()
	if (!m_bInitPaint)//��ʼ��DC��
	{
		InitPaint(&dc);
	}//eif
	CDC memDC ;
	CBitmap memBitmap ;
	CBitmap* oldBitmap ; // bitmap originally found in CMemDC

	CRect rectClient;
	GetClientRect(&rectClient);
	//CalculateRect();//zll 2013.5.9
	memDC.CreateCompatibleDC(&dc) ;
	memBitmap.CreateCompatibleBitmap(&dc, rectClient.Width(), rectClient.Height()) ;
	oldBitmap = (CBitmap *)memDC.SelectObject(&memBitmap) ;
	//CalCulDispRect();//zll 2013.5.9
	//DrawCursor(&memDC);//zll 2013.5.9
	//DrawCurves(&m_dcCurve);//zll 2013.5.9
	//long l=GetTickCount();
	if (memDC.GetSafeHdc() != NULL)
	{
		//memDC.BitBlt(0,0,m_nClientWidth, m_nClientHeight,&m_dcGrid, 0, 0, SRCCOPY);//��������������
		//memDC.BitBlt(0, 0, m_nClientWidth, m_nClientHeight,&m_dcPlot, 0,0,SRCPAINT);  //���һ������SRCPAINTָ������DC���ݽ���OR����,����������
		DoDraw(&memDC);
		ShowCatch(&memDC, m_ptMouse);  //��ʾ��꿿��AB���ߺ��ͼ�Ρ�lzy2014.9.9
		//switch(m_iControlMode)
		//{
		//case CURVE_GET_DATA:
		//	{	
		//		this->DrawGetDataCross(&memDC);//��ʾ����ֵwcq
		//		break;
		//	}
		//case CURVE_ZOOM_IN:
		//case CURVE_ZOOM_OUT:
		//	{
		//		this->DrawZoomRect(&memDC,m_recMouseSelect);//�����϶�ʱ�ľ���cwcq
		//		break;
		//	}
		//case CURVE_SELECT:
		//	{
		//		CPoint point;
		//		GetCursorPos(&point);
		//		this->ScreenToClient(&point);
		//		this->DrawSelectFocus(&memDC,point);
		//		break;
		//	}
		//}
		dc.BitBlt(0, 0, rectClient.Width(), rectClient.Height(), &memDC, 0, 0, SRCCOPY); //�����ڴ�DC����Ļ

	}
	//long l2=GetTickCount()-l;
	memDC.SelectObject(oldBitmap) ;
}
void COtdrTrace::InitPaint(CDC* pDC)
{
	//ASSERT(m_dcGrid.GetSafeHdc());
	//ASSERT(m_dcPlot.GetSafeHdc());
	//DC
	if (!m_dcGrid.GetSafeHdc())
	{
		m_dcGrid.CreateCompatibleDC(pDC);
	}//eif
	if (!m_dcCurve.GetSafeHdc())
	{
		m_dcCurve.CreateCompatibleDC(pDC);
	}//eif

	//�ڴ�λͼ
	if (m_bmpCurve.GetSafeHandle())
	{
		m_bmpCurve.DeleteObject();
	}//eif
	if (m_bmpGrid.GetSafeHandle())
	{
		m_bmpGrid.DeleteObject();
	}//eif
	CRect rectClient;
	GetClientRect(&rectClient);
	m_rectCurve.CopyRect(&rectClient);//��ͼ����
	m_bmpCurve.CreateCompatibleBitmap(pDC, m_rectCurve.Width(), m_rectCurve.Height());//
	m_bmpGrid.CreateCompatibleBitmap(pDC, rectClient.Width(), rectClient.Height());//
	m_dcGrid.SelectObject(&m_bmpGrid);
	m_dcCurve.SelectObject(&m_bmpCurve);

	m_bInitPaint = TRUE;
}
void COtdrTrace::DoDraw(CDC* pDC)
{
	CRect rectClient;
	GetClientRect(&rectClient);
	pDC->FillSolidRect(&rectClient, g_COLORREF[8]);//zll 2013.5.12
	DrawGrid(pDC);
	
	//pDC->BitBlt(m_rectCurve.left, m_rectCurve.top, m_rectCurve.Width(), m_rectCurve.Height(), &m_dcCurve, 0, 0, SRCCOPY); //���ߡ������ڴ�DC����Ļ
	TransparentBlt(pDC->GetSafeHdc(), m_rectCurve.left, m_rectCurve.top, m_rectCurve.Width(), m_rectCurve.Height(), m_dcCurve.GetSafeHdc(), 0, 0, m_rectCurve.Width(), m_rectCurve.Height(), RGB(0, 0, 0));

	switch(m_nCurMode)//���ݲ�ͬģʽ
	{
	case CURVE_ZOOM_OUT://��С
	case CURVE_ZOOM_IN://�Ŵ�
		DrawMouseSeleRect(pDC);
		break;
	}//eswitch

	if(m_bShowScale)
		DrawScale(pDC);//���ƿ̶� lzy2014.7.17
	if(m_bShowLineAB)
		DrawLineAB(pDC);//

	
}
//��������϶�����
void COtdrTrace::DrawMouseSeleRect(CDC* pDc)
{
	//TRACE(_T("%d %d %d %d\n"),m_rectMouseSelect.left, m_rectMouseSelect.top, m_rectMouseSelect.right,m_rectMouseSelect.bottom);
	//if (m_rectMouseSelect.IsRectEmpty())
	//{
	//	return;
	//}//eif
	///*TRACE(_T("%d %d %d %d 2222222222222222222222222222222222222\n"),m_rectMouseSelect.left, m_rectMouseSelect.top, m_rectMouseSelect.right,m_rectMouseSelect.bottom);*/
	CPen zoomRectPen(PS_SOLID, 1, RGB(255,0,0));
	CPen* oldPen = pDc->SelectObject(&zoomRectPen);
	pDc->MoveTo(m_rectMouseSelect.TopLeft());
	pDc->LineTo(m_rectMouseSelect.right,m_rectMouseSelect.top);
	pDc->LineTo(m_rectMouseSelect.BottomRight());
	pDc->LineTo(m_rectMouseSelect.left,m_rectMouseSelect.bottom);
	pDc->LineTo(m_rectMouseSelect.TopLeft());
	pDc->SelectObject(oldPen);
}
//������������
void COtdrTrace::DrawCurves(CDC* pDC,INT m_nCurveIndex)
{
	//if(-1!=m_nCurveIndex && 0!=m_vAllData.size())
	//{
	//	m_dcCurve.FillSolidRect(&m_rectCurve, RGB(0,0,0));
	//	if (FALSE == g_sorFileArray.waveConfig.MultiMode)
	//	{
	//		MAIN->m_pWaveFormView->SetHorUnitText(m_nCurveIndex);
	//		m_vAllData[m_nCurveIndex].DrawCurve(pDC, m_rectCurve,m_nCurveIndex);//zll 2013.6.26 �� ѡ�е���������һ�£�����ʾ����������֮ǰ
	//	}
	//	else if (TRUE == g_sorFileArray.waveConfig.MultiMode)
	//	{
	//		for(int i = 0; i < m_vAllData.size(); i++)
	//		{
	//			m_vAllData[i].DrawCurve(pDC, m_rectCurve,i);
	//		}//efor
	//		MAIN->m_pWaveFormView->SetHorUnitText(m_nCurveIndex);
	//		m_vAllData[m_nCurveIndex].DrawCurve(pDC, m_rectCurve,m_nCurveIndex);//zll 2013.6.26 �� ѡ�е���������һ�£�����ʾ����������֮ǰ
	//	}
	//}
	m_dcCurve.FillSolidRect(&m_rectCurve, RGB(0,0,0));
	if(0!=m_vAllData.size())
	{
		for(int i = 0; i < m_vAllData.size(); i++)
		{
			if (m_vAllData[i].m_bShow==1 && m_nCurveIndex!=i)
			{
				m_vAllData[i].DrawCurve(pDC, m_rectCurve,i);
			}
		}//efor
		if (-1!=m_nCurveIndex && m_nCurveIndex<m_vAllData.size())//zll 2013.7.4 m_nCurveIndexС��m_vAllData��С������Խ��
		{
			MAIN->m_pWaveFormView->SetHorUnitText(m_nCurveIndex);
			m_vAllData[m_nCurveIndex].DrawCurve(pDC, m_rectCurve,m_nCurveIndex);//zll 2013.6.26 �� ѡ�е���������һ�£�����ʾ����������֮ǰ
		}
	}
}
//������������
void COtdrTrace::DrawLineAB(CDC* pDC)
{
	if (!m_bShowLineAB)
	{
		return;
	}//eif
	//A
	if (IsInCurRangeX(m_cursorA.m_fPos))//ֻ���ڵ�ǰ��Χ��ʱ���Ż���
	{
		DrawLineABDetail(pDC, &m_cursorA);
	}//eif
	if (IsInCurRangeX(m_cursorB.m_fPos))////ֻ���ڵ�ǰ��Χ��ʱ���Ż���
	{
		DrawLineABDetail(pDC, &m_cursorB);
	}//eif
	//����ȫ�ֱ���g_CursorInfo;
	SetCurveCursorData();
}
//������ƺ���.
void COtdrTrace::DrawLineABDetail(CDC* pDC, CursorAB* pCursorAB)
{
	//�õ�X������
	CRect rect;
	GetClientRect(&rect);
	INT nX = FLOAT2POINTX(pCursorAB->m_fPos);//(pCursorAB->m_fPos - m_fRangeX[OTDR_TRACE_MIN]) * rect.Width() / (m_fRangeX[OTDR_TRACE_MAX] - m_fRangeX[OTDR_TRACE_MIN]);
	CPen pen;//(m_LineColor, PS_SOLID, 1);
	if ((("A" == pCursorAB->m_strAOrB) && (OTDRTRACE_AB_SELE_A == m_nABSele)) || (("B" == pCursorAB->m_strAOrB) && (OTDRTRACE_AB_SELE_B == m_nABSele)))
	{//��ǰѡ��
		pen.CreatePen(PS_SOLID, 1, OTDR_TRACE_CURSOR_AB_COLOR_SELE);
	}
	else
	{
		pen.CreatePen(PS_SOLID, 1, pCursorAB->m_colorLine);
	}//eif
	CPen* pOldPen = pDC->SelectObject(&pen);
	INT nMarginTop = OTDR_TRACE_CURSOR_AB_MARGIN_TOP;
	if ("B" == pCursorAB->m_strAOrB)
	{
		//nMarginTop += 20;
		nMarginTop += 0;    //�����߻��ǵȳ��ã�lzy2014.7.14
	}//eif
	//ֱ��
	pDC->MoveTo(nX, rect.top + nMarginTop);
	pDC->LineTo(nX, rect.bottom);

	//�ı�
	CFont   font;   
	CFont   *pOldFont;   
	/*font.CreateFont(16, 0, 0, 0, FW_NORMAL,	0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH,_T("Arial")); */
	CreateMyFont(pDC, font, _T("Arial"));
	pOldFont = pDC->SelectObject(&font);
	pDC->SetTextColor(pCursorAB->m_colorLabel);

	//zll 2013.5.11
	float fVal=m_cursorA.m_fPos/Divisor_toKm;
	if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
	{
		m_cursorA.m_strDistance.Format(_T("%.3fkm"), fVal);//zll 2013.5.9
	}
	else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪmiles
	{
		fVal=KmeterToOther(fVal,UNIT_SHOW_MILE);
		m_cursorA.m_strDistance.Format(_T("%.3fmiles"), fVal);//zll 2013.5.9
	}
	else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkft
	{
		fVal=KmeterToOther(fVal,UNIT_SHOW_KFEET);
		m_cursorA.m_strDistance.Format(_T("%.3fkft"), fVal);//zll 2013.5.9
	}
	float fDB=GetYDB(m_cursorA.m_fPos);
	//fDB=(65536-g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo->pDataPoints[m_cursorA.m_nPos])/1000.0;
	m_cursorA.m_strDB.Format(_T("%.2fdB"), fDB);

	fVal=m_cursorB.m_fPos/Divisor_toKm;
	if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
	{
		m_cursorB.m_strDistance.Format(_T("%.3fkm"), fVal);//zll 2013.5.9
	}
	else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪmiles
	{
		fVal=KmeterToOther(fVal,UNIT_SHOW_MILE);
		m_cursorB.m_strDistance.Format(_T("%.3fmiles"), fVal);//zll 2013.5.9
	}
	else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkft
	{
		fVal=KmeterToOther(fVal,UNIT_SHOW_KFEET);
		m_cursorB.m_strDistance.Format(_T("%.3fkft"), fVal);//zll 2013.5.9
	}
	fDB=GetYDB(m_cursorB.m_fPos);
	//fDB=(65536-g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo->pDataPoints[m_cursorB.m_nPos])/1000.0;
	m_cursorB.m_strDB.Format(_T("%.2fdB"), fDB);

	///*********************zll 2013.6.25  ����A B ���֮��ľ���
	fVal=(m_cursorA.m_fPos-m_cursorB.m_fPos)/Divisor_toKm;
	if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
	{
		m_cursorAminusB.m_strDistance.Format(_T("%.3fkm"), fVal);//zll 2013.5.9
	}
	else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪmiles
	{
		fVal=KmeterToOther(fVal,UNIT_SHOW_MILE);
		m_cursorAminusB.m_strDistance.Format(_T("%.3fmiles"), fVal);//zll 2013.5.9
	}
	else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkft
	{
		fVal=KmeterToOther(fVal,UNIT_SHOW_KFEET);
		m_cursorAminusB.m_strDistance.Format(_T("%.3fkft"), fVal);//zll 2013.5.9
	}
	/*fDB=abs((65536-g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo->pDataPoints[m_cursorA.m_nPos])/1000.0
		    -(65536-g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo->pDataPoints[m_cursorB.m_nPos])/1000.0);*/
	fDB=abs(GetYDB(m_cursorA.m_fPos)-GetYDB(m_cursorB.m_fPos));
	//fDB = GetYDB(m_cursorA.m_fPos)-GetYDB(m_cursorB.m_fPos);//lzy2014.7.18
	m_cursorAminusB.m_strDB.Format(_T("%.2fdB"), fDB);
	///*********************zll 2013.6.25

	CString strText,strTextAminusB;
	strText.Format(_T("%s %s %s"), pCursorAB->m_strAOrB, pCursorAB->m_strDistance,pCursorAB->m_strDB);
	//strText.Format(_T("%s"), pCursorAB->m_strAOrB);  //��AB���Ա�ֻ��ʾA��B,������λ����Ϣ��ʾ�����Ϸ�lzy2014.7.15
	CSize sizeText = pDC->GetTextExtent(strText);//�õ��ı������
	CRect rectText;

	if ("B" == pCursorAB->m_strAOrB) //Ϊ��ʹB����ʾ�����������ƶ�һЩ�������������غϡ�lzy2014.7.14
	{
		rectText.top = nMarginTop + 20; 
	}//eif
	else
		rectText.top = nMarginTop;

	//rectText.top = nMarginTop;
	rectText.bottom = rectText.top + sizeText.cy;
	if (nX + sizeText.cx + 1 <= rect.right)//�ı�������ʾ�����ұ�,1���ı������߼��
	{
		rectText.left = nX + 1;
		rectText.right = nX + 1 + sizeText.cx;
	}
	else//�ƶ����ұߣ����ı���ʾ���£�����ı���ʾ�����
	{
		rectText.left = nX - (sizeText.cx + 1);
		rectText.right = nX - 1;
	}//eif
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(strText, &rectText, DT_LEFT | DT_SINGLELINE);


	///*********************zll 2013.6.25  ����A B ���֮��ľ���
	strTextAminusB.Format(_T("%s %s %s"), _T("A-B:"), m_cursorAminusB.m_strDistance,m_cursorAminusB.m_strDB);
	CSize sizeTextAminusB = pDC->GetTextExtent(strTextAminusB);//�õ��ı������
	CRect rectTextAminusB;
	rectTextAminusB.top = 0;
	rectTextAminusB.bottom = rectTextAminusB.top + sizeTextAminusB.cy;
	//rectTextAminusB.left = 1;
	//rectTextAminusB.right = 1 + sizeTextAminusB.cx;
	//��������ʾ�ƶ����ұ߿յ���ʹ�䲻����Ӱ�쵽��߿��ܵ�������ʾ�lzy2014.7.15
	rectTextAminusB.left = rect.right - 1 - sizeTextAminusB.cx ;
	rectTextAminusB.right = rect.right - 1;

	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(strTextAminusB, &rectTextAminusB, DT_LEFT | DT_SINGLELINE);
	///*********************zll 2013.6.25

	////��AB�ߵ�������ʾ�����Ϸ�lzy2014.7.15
	//CString strTextAB;
	//strTextAB.Format(_T("%s: %s %s"), pCursorAB->m_strAOrB, pCursorAB->m_strDistance,pCursorAB->m_strDB);
	//CSize sizeTextAB = pDC->GetTextExtent(strTextAB);
	//CRect rcTextAB = rectTextAminusB;
	//if(_T("A") == pCursorAB->m_strAOrB)  //A�ߵĻ�
	//{
	//	rcTextAB.top = rectTextAminusB.bottom + 2;
	//	rcTextAB.bottom = rcTextAB.top + sizeTextAB.cy;
	//}
	//else
	//{
	//	rcTextAB.top = (rectTextAminusB.bottom + 2) * 2;
	//	rcTextAB.bottom = rcTextAB.top + sizeTextAB.cy;
	//}
	//pDC->SetBkMode(TRANSPARENT);
	//pDC->DrawText(strTextAB, &rcTextAB, DT_LEFT | DT_SINGLELINE);


	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
	pOldFont = pDC->SelectObject(pOldFont);
	font.DeleteObject();
}
//����GridLine,������
void COtdrTrace::DrawGrid_old(CDC *pDC)//zll 2013.4.22
{
	//�������Ҫ�ػ����˳�
	if (!m_bDrawGrid)
	{
		return;
	}
	//m_bDrawGrid = FALSE;

	if (g_sorFileArray.waveConfig.ShowGrid)//zll 2013.5.12
	{
		CPen penGridLine;//
		penGridLine.CreatePen (PS_DOT/*����*/, 1, g_COLORREF[9]);//
		CPen* pOldPen = pDC->SelectObject(&penGridLine); //

		float ndx = m_rectCurve.Width ()/COUNT_GRIDLINE_HORIZONE;
		float ndy = m_rectCurve.Height ()/COUNT_GRIDLINE_VERTICAL;
		int bottom = m_rectCurve.Height();
		int left = 0;
		int itemp;//
		for ( int i = 1; i<COUNT_GRIDLINE_HORIZONE; i++)//
		{	
			itemp=left + ndx * i;
			//������̶ȴ��ϵ���-------------------------------------------------------
			pDC->MoveTo (itemp,bottom);
			pDC->LineTo (itemp,0);
		}//efor
		//������:::	������
		for (int i=1; i<COUNT_GRIDLINE_VERTICAL; i++)
		{
			itemp = bottom - ndy * i;
			pDC->MoveTo (left , itemp);
			pDC->LineTo (m_rectCurve.right,itemp);
		}//efor

		pDC->SelectObject(pOldPen);
	}	
}

//���ƿ̶�lzy2014.7.17
void COtdrTrace::DrawScale(CDC *pDC)
{
	//�������Ҫ�ػ����˳�
	if (!m_bDrawGrid)
	{
		return;
	}

	int iYmax = m_fRangeY[1];
	int iXmax = m_fRangeX[1];
	float ndx = m_rectCurve.Width () / COUNT_GRIDLINE_HORIZONE;
	float ndy = m_rectCurve.Height () / COUNT_GRIDLINE_VERTICAL;
	int bottom = m_rectCurve.Height();
	int left = 0;
	int itemp;

	CRect rectText;
	CString strText;
	pDC->SetBkMode(OPAQUE);
	//pDC->SetBkMode(TRANSPARENT);
	//pDC->SetTextColor(g_COLORREF[9]); //color of grid
	pDC->SetTextColor(RGB(180, 180, 180));
	pDC->SetBkColor(g_COLORREF[8]);
	CSize sizeText;

	CFont   font;   
	CFont   *pOldFont;   
	font.CreateFont(15, 0, 0, 0, FW_BOLD,	0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH,_T("Arial")); 
	//CreateMyFont(pDC, font, _T("Arial"));
	pOldFont = pDC->SelectObject(&font);

	//��Y��̶�
	if(iYmax >= 0 && iYmax <= 100)
	{
		rectText.left = m_rectCurve.left + 5;		
		
		for(int i = 1; i < COUNT_GRIDLINE_VERTICAL; i++)
		{
			itemp = bottom - ndy * i;
			strText.Format(_T("%.1f"), m_fRangeY[0] + (m_fRangeY[1] - m_fRangeY[0]) * i / COUNT_GRIDLINE_VERTICAL);
			sizeText = pDC->GetTextExtent(strText);//�õ��ı������
			rectText.top = itemp - sizeText.cy / 2;
			rectText.bottom = rectText.top + sizeText.cy;
			rectText.right = rectText.left + sizeText.cx;
			pDC->DrawText(strText, &rectText, DT_LEFT | DT_SINGLELINE);
		}

		strText = _T("dB");
		sizeText = pDC->GetTextExtent(strText);//�õ��ı������
		rectText.top = m_rectCurve.top + 5;
		rectText.bottom = rectText.top + sizeText.cy;
		rectText.right = rectText.left + sizeText.cx;
		pDC->DrawText(strText, &rectText, DT_LEFT | DT_SINGLELINE);
	}

	//��X��̶�
	if(iXmax > 0)
	{
		rectText.bottom = m_rectCurve.bottom - 3;
		for(int i = 1; i < COUNT_GRIDLINE_HORIZONE; i++)
		{
			itemp = left + ndx * i;
			strText.Format(_T("%.1f"), 0.001 * m_fRangeX[0] + 0.001 * (m_fRangeX[1] - m_fRangeX[0]) * i / COUNT_GRIDLINE_HORIZONE);
			sizeText = pDC->GetTextExtent(strText);//�õ��ı������
			rectText.left = itemp - sizeText.cx / 2;
			rectText.right = rectText.left + sizeText.cx;
			rectText.top = rectText.bottom - sizeText.cy;			
			pDC->DrawText(strText, &rectText, DT_LEFT | DT_SINGLELINE);
		}

		strText = _T("km");
		sizeText = pDC->GetTextExtent(strText);//�õ��ı������
		rectText.right = m_rectCurve.right - 10;
		rectText.left = rectText.right - sizeText.cx;		
		pDC->DrawText(strText, &rectText, DT_LEFT | DT_SINGLELINE);

	}

    pDC->SelectObject(pOldFont);
}

//�Զ������̶�������lzy2014.7.16
void COtdrTrace::DrawGrid(CDC *pDC)
{
	//�������Ҫ�ػ����˳�
	if (!m_bDrawGrid)
	{
		return;
	}
	
	int iYmax = m_fRangeY[1];//y�����ֵ��g_nTraceYmax;
	m_wdGridV = COUNT_GRIDLINE_VERTICAL;
    /*if(g_nTraceYmax)
		m_wdGridV = g_nTraceYmax / 5.0;*/
	//if(g_nTraceYmax)
	//{
	//	int nCount = g_nTraceYmax / 5;

	//	if(nCount > COUNT_GRIDLINE_VERTICAL)
	//	{
	//		if(nCount % 2)  //����
	//		{
	//			nCount ++;
	//			nCount /= 2;
	//			iYmax += 5;  //lzy2014.7.16
	//		}
	//		else  //ż��
	//			nCount /= 2;

	//	}
	//	m_wdGridV = nCount;
	//		
	//	//m_wdGridV = (g_nTraceYmax / 5) > COUNT_GRIDLINE_VERTICAL ? COUNT_GRIDLINE_VERTICAL : (g_nTraceYmax / 5); //lzy2014.7.16
	//}

	if (g_sorFileArray.waveConfig.ShowGrid)//zll 2013.5.12
	{
		CPen penGridLine;//
		//penGridLine.CreatePen (PS_DOT/*����*/, 1, g_COLORREF[9]);//
		LOGBRUSH logBrush;
		logBrush.lbColor = g_COLORREF[9];
		logBrush.lbStyle = PS_SOLID;
		penGridLine.CreatePen(PS_COSMETIC | PS_ALTERNATE, 1, &logBrush, 0, NULL);
		CPen* pOldPen = pDC->SelectObject(&penGridLine); //

		CString strText = _T("00.00");
		CSize sizeText = pDC->GetTextExtent(strText);//�õ��ı������

		float ndx = m_rectCurve.Width ()/COUNT_GRIDLINE_HORIZONE;
		float ndy = m_rectCurve.Height ()/ m_wdGridV;
		int bottom = m_rectCurve.Height();
		int left = 0;//sizeText.cx;
		int itemp;//
		for ( int i = 1; i<COUNT_GRIDLINE_HORIZONE; i++)//
		{	
			itemp=left + ndx * i;
			//������̶ȴ��ϵ���-------------------------------------------------------
			pDC->MoveTo (itemp,bottom);
			pDC->LineTo (itemp,0);
		}//efor
		//������:::	������
		for (int i=1; i<m_wdGridV; i++)
		{
			itemp = bottom - ndy * i;
			pDC->MoveTo (left , itemp);
			pDC->LineTo (m_rectCurve.right,itemp);
		}//efor		

		pDC->SelectObject(pOldPen);
	}	
}
//��������
void COtdrTrace::CreateMyFont(CDC* pDC, CFont& font, CString fontName)
{
	CString strFontName = fontName;
	LOGFONT lf;
	SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lf, 0);
	if ("" == strFontName)
	{
		strFontName = lf.lfFaceName;
	}//eif
	font.CreateFont(-::MulDiv(lf.lfWidth, pDC->GetDeviceCaps(LOGPIXELSY), 72), 0, 0, 0, FW_NORMAL, 0, 0, 0, GB2312_CHARSET, 0, 0, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_MODERN, strFontName);//
}
void COtdrTrace::OnLButtonDown(UINT nFlags, CPoint point)
{
	//// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(m_rectCurve.PtInRect(point))
	{
		m_nCurABSele = GetABSelectState(point);
		if (OTDRTRACE_AB_SELE_NONE != m_nCurABSele)//�������A/B�긽������Ϊ���漴���ƶ�A/B��
		{
			m_nABSele = m_nCurABSele;
			CButton::OnLButtonDown(nFlags, point);
			return;
		}//eif
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//�϶�����
		if(!m_bLeftButtonDown)
		{
			m_rectMouseSelect.BottomRight() = point;
			m_rectMouseSelect.TopLeft() = point;
			m_bLeftButtonDown = true;			
		}//eif
	}//eif
	//TRACE(_T("OnLButtonDown..x:%d y:%d\n"),point.x,point.y);
	CButton::OnLButtonDown(nFlags, point);
	////CRectTracker tracker;
	////double xmin, xmax, ymin, ymax;
	////TRACE(_T("OnLButtonDown..x:%d y:%d\n"),point.x,point.y);
	////if (tracker.TrackRubberBand(this, point) )
	////{
	////	CPoint pt1, pt2;
	////	tracker.m_rect.NormalizeRect ();
	////	pt1= tracker.m_rect.TopLeft ();
	////	pt2= tracker.m_rect.BottomRight ();
	////	TRACE(_T("OnLButtonDown..TrackRubberBand x:%d y:%d x:%d y:%d\n"),pt1.x,pt1.y,pt2.x,pt2.y);

	////	int xmin = 2;
	////	//Invalidate(false);
	////}this->SetFocus();
}

void COtdrTrace::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (!m_bLeftButtonDown)
	{
		m_nCurABSele = OTDRTRACE_AB_SELE_NONE;
		CButton::OnLButtonUp(nFlags, point);
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	if (OTDRTRACE_AB_SELE_NONE != m_nCurABSele)//�϶�AB��
	{
		CButton::OnLButtonUp(nFlags, point);
		return;
	}//eif

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	if(m_rectCurve.PtInRect(point) )
	{
		if(m_bLeftButtonDown)
		{
			m_rectMouseSelect.BottomRight() = point;
		}//eif	
	}//eif
	//TRACE(_T("OnLButtonUp:%d %d %d %d \n"),m_rectMouseSelect.left, m_rectMouseSelect.right,m_rectMouseSelect.top,m_rectMouseSelect.bottom);
	m_bLeftButtonDown = false;
	if ((m_rectMouseSelect.left == m_rectMouseSelect.right) && (m_rectMouseSelect.top == m_rectMouseSelect.bottom))
	{
		ZoomOutClickForBig(m_rectMouseSelect.left, m_rectMouseSelect.top);//
		m_rectMouseSelect.SetRectEmpty();
		CButton::OnLButtonUp(nFlags, point);
		return;////
	}
	SetRange(POINT2FLOATX(m_rectMouseSelect.left), POINT2FLOATX(m_rectMouseSelect.right), POINT2FLOATY(m_rectMouseSelect.bottom), POINT2FLOATY(m_rectMouseSelect.top));
	m_rectMouseSelect.SetRectEmpty();
	CButton::OnLButtonUp(nFlags, point);
}

void COtdrTrace::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(m_rectCurve.PtInRect(point))
	{
		m_ptMouse = point;//lzy2014.9.9
		//TRACE(_T("x=%d,y=%d\n"), m_ptMouse.x, m_ptMouse.y);
		//�ƶ�A/B��
		if (OTDRTRACE_AB_SELE_NONE != m_nCurABSele)//
		{
			CursorABOnMove(point);
			this->InvalidateRect(m_rectCurve);
			CButton::OnMouseMove(nFlags, point);			
			return;
		}//eif
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//�϶�����
		else if(m_bLeftButtonDown)
		{
			switch(m_nCurMode)
			{
			case CURVE_MOVE:
				{	
					m_rectMouseSelect.BottomRight() = point;
					break;
				}		
			case CURVE_ZOOM_IN:
			case CURVE_ZOOM_OUT:
				{
					m_rectMouseSelect.BottomRight() = point;
					break;
				}
			}//eswitch
			this->InvalidateRect(m_rectCurve);
		}//eif
		else
		{
			this->InvalidateRect(m_rectCurve);
		}
	}//eif
	CButton::OnMouseMove(nFlags, point);
}
//�϶�A/B��ʱ���¼���A/B��λ��
void COtdrTrace::CursorABOnMove(CPoint pointMoving)
{
	if (m_vAllData.size() <= m_nCurveIndex)//�±�Խ��
	{
		return;
	}//
	if (OTDRTRACE_AB_SELE_A == m_nCurABSele)//�϶�A
	{
		FLOAT fPos = m_cursorA.m_fPos;//�����¼�϶�����,����ѡ��AB��ʱ��Ӧ�ƶ�B��
		m_cursorA.m_nPos = pointMoving.x;
		m_cursorA.m_fPos = POINT2FLOATX(pointMoving.x);
		int nIndex = m_vAllData[m_nCurveIndex].DistanceToIndex(m_cursorA.m_fPos);
		m_cursorA.m_fPos = nIndex * m_vAllData[m_nCurveIndex].m_fDataSpacing;
		m_cursorA.m_strDistance.Format(_T("%.3fm"), m_cursorA.m_fPos);
		//float fDB=GetYDB(m_cursorA.m_fPos);
		/*float fTemp=(299792458/1000000.0)/(2*25 * 1.46000) * pointMoving.x;
		fTemp = pointMoving.x*(m_fRangeX[OTDR_TRACE_MAX] - m_fRangeX[OTDR_TRACE_MIN])/818 + m_fRangeX[OTDR_TRACE_MIN];
		TRACE(_T("m_cursorA.m_nPos:%d %.3fm\n"),m_cursorA.m_nPos, fTemp);*/
		if (IsCurrentTypeAB())//��ǰѡ��AB��
		{
			fPos = m_cursorA.m_fPos - fPos;			
			m_cursorB.m_fPos = m_cursorB.m_fPos + fPos;
			m_cursorB.m_nPos = FLOAT2POINTX(m_cursorB.m_fPos);  //lzy2014.9.10
			m_cursorB.m_strDistance.Format(_T("%.3fm"), m_cursorB.m_fPos);
			
		}//eif

		TRACE(_T("CursorABOnMove() posA=%d, posB=%d\n"), m_cursorA.m_nPos, m_cursorB.m_nPos);////////////////////////////
		
	}//eif
	if (OTDRTRACE_AB_SELE_B == m_nCurABSele)//�϶�B
	{
		FLOAT fPos = m_cursorB.m_fPos;//�����¼�϶�����,����ѡ��AB��ʱ��Ӧ�ƶ�A��
		m_cursorB.m_nPos = pointMoving.x;
		m_cursorB.m_fPos = POINT2FLOATX(pointMoving.x);
		int nIndex = m_vAllData[m_nCurveIndex].DistanceToIndex(m_cursorB.m_fPos);
		m_cursorB.m_fPos = nIndex * m_vAllData[m_nCurveIndex].m_fDataSpacing;
		m_cursorB.m_strDistance.Format(_T("%.3fm"), m_cursorB.m_fPos);
		//float fDB=GetYDB(m_cursorB.m_fPos);
		if (IsCurrentTypeAB())//��ǰѡ��AB��
		{
			fPos = m_cursorB.m_fPos - fPos;
			m_cursorA.m_fPos = m_cursorA.m_fPos + fPos;
			m_cursorA.m_nPos = FLOAT2POINTX(m_cursorA.m_fPos);  //lzy2014.9.10
			m_cursorA.m_strDistance.Format(_T("%.3fm"), m_cursorA.m_fPos);
		}//eif
	}//eif
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//����ƶ�ʱ����AB��Ӧ��������
//���ݵ����������Ļ���ص����ʱֱ�ӷ������λ�ã�С��ʱѰ����������ݵ�λ��
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//INT COtdrTrace::GetABPositionOnMove(CPoint pointMoving)
//{
//	if (m_vAllData.size() < m_nCurveIndex + 1)//��ֹԽ��
//	{
//		return 0;
//	}//
//	INT nStartIndex = m_vAllData[m_nCurveIndex].DistanceToIndex(m_fRangeX[0]);
//	INT nEndIndex = m_vAllData[m_nCurveIndex].DistanceToIndex(m_fRangeX[1]);
//	if (nEndIndex - nStartIndex + 1 >= m_rectCurve.Width())//���ݵ����������Ļ���ص����ʱֱ�ӷ������λ��
//	{
//		return pointMoving.x;
//	}//eif
//
//	//С��ʱѰ����������ݵ�λ��
//	INT nFloor,nCeil;
//	nFloor = pointMoving.x / (nEndIndex - nStartIndex + 1);
//	nCeil = ceil(nFloor);
//	nFloor = floor(nFloor);
//	if (nCeil == nFloor);//�պ��ڵ���
//	{
//		return pointMoving.x;
//	}//eif
//	if (pointMoving.x - nStartIndex > nEndIndex - pointMoving.x)//����߽�
//	{
//		return nStartIndex;
//	}//eif
//	return nEndIndex;//���ұ߽�
//}
////�õ�AB������ľ���
//FLOAT COtdrTrace::GetABDistanceOnMove(CPoint pointMoving, INT nXAB)
//{
//	return 0;
//}
//�������ʱ���ж��Ƿ�ѡ��AB��,δѡ�з���OTDRTRACE_AB_SELE_NONE��ѡ��A����OTDRTRACE_AB_SELE_A��ѡ��B����OTDRTRACE_AB_SELE_B
INT COtdrTrace::GetABSelectState(CPoint pointClick)
{
	INT nReturn = OTDRTRACE_AB_SELE_NONE;
	INT nMarginA = -1;
	INT nMarginB = -1;
	int nTemp;
	if (IsInCurRangeX(m_cursorA.m_fPos))//A����Ļ��ʾ��Χ��
	{
		nTemp = FLOAT2POINTX(m_cursorA.m_fPos);//�����Ļλ�ã�����
		if (abs(nTemp - pointClick.x) < OTDR_TRACE_CURSOR_AB_VALID_SELE)
		{
			nMarginA = abs(nTemp - pointClick.x);
		}//eif
	}//eif

	if (IsInCurRangeX(m_cursorB.m_fPos))//B����Ļ��ʾ��Χ��
	{
		nTemp = FLOAT2POINTX(m_cursorB.m_fPos);//�����Ļλ�ã�����
		if (abs(nTemp - pointClick.x) < OTDR_TRACE_CURSOR_AB_VALID_SELE)
		{
			nMarginB = abs(nTemp - pointClick.x);
		}//eif
	}//eif

	if ((-1 != nMarginA) && (-1 != nMarginB))//AB�����ƶ���Ч����ѡ�����
	{
		if (nMarginA <= nMarginB)
		{
			nReturn = OTDRTRACE_AB_SELE_A;
		}
		else{
			nReturn = OTDRTRACE_AB_SELE_B;
		}//eif
	}
	else if (-1 != nMarginA)//ֻ��A����Ч����
	{
		nReturn = OTDRTRACE_AB_SELE_A;
	}
	else if (-1 != nMarginB)//ֻ��B����Ч����
	{
		nReturn = OTDRTRACE_AB_SELE_B;
	}//eif
	return nReturn;
}
//�ж��Ƿ��ڵ�ǰ��ʾ��Χ��,���ˮƽ����
BOOL COtdrTrace::IsInCurRangeX(FLOAT fDistance)
{
	return ((fDistance >= m_fRangeX[OTDR_TRACE_MIN]) && (fDistance <= m_fRangeX[OTDR_TRACE_MAX]));
}
//�ж��Ƿ��ڵ�ǰ��ʾ��Χ��,�����ֱ����
BOOL COtdrTrace::IsInCurRangeY(FLOAT fDistance)
{
	return ((fDistance >= m_fRangeY[OTDR_TRACE_MIN]) && (fDistance <= m_fRangeY[OTDR_TRACE_MAX]));
}
void COtdrTrace::OnSize(UINT nType, int cx, int cy)
{
	CButton::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	GetClientRect(m_rectClient);
	m_rectCurve = m_rectClient;

	if(m_dcCurve.GetSafeHdc())
	{
		CDC *pDC = GetDC(); // Get device context
		InitPaint(pDC);
		DrawCurves(&m_dcCurve,-1);
		ReleaseDC(pDC);
	}//eif
}
//�����ͼ����
void COtdrTrace::CalculateRect()
{
	GetClientRect(m_rectClient);
	m_rectCurve = m_rectClient;
}
void COtdrTrace::SetRange(float xmin, float xmax, float ymin, float ymax, BOOL bSetScrollInof) 
{
	if (xmin == xmax || ymin == ymax)
	{
		return;
	}
	else if (xmin > xmax || ymin > ymax)
	{
		return;
	}//eif

	if (xmin < 0)
	{
		xmin = 0;
	}//eif

	if (0 == m_vAllData.size())
	{
		return;
	}//eif
	if (xmax > m_vAllData[0].m_fMaxDistance)
	{
		xmax =  m_vAllData[0].m_fMaxDistance;
	}//eif
	//TRACE(_T("m_vAllData[0].DistanceToIndex(xmax):%d %d"),m_vAllData[0].DistanceToIndex(xmax), m_vAllData[0].DistanceToIndex(xmin));
	if((!(m_vAllData[0].DistanceToIndex(xmax) - m_vAllData[0].DistanceToIndex(xmin) >= OTDR_TRACE_VALID_NUM_LOWEST)) || (!(ymax - ymin > OTDR_TRACE_Y_MIN_CHANGE)))//ֻ��һ����
	{
		return;
	}//eif
	if (ymax - ymin <= OTDR_TRACE_Y_MIN_CHANGE)
	{
		return;
	}//eif
	if (ymin < OTDR_TRACE_Y_MIN)
	{
		ymin = OTDR_TRACE_Y_MIN;
	}//eif
	if (ymax > g_nTraceYmax)
	{
		ymax = g_nTraceYmax;
	}//eif
	m_vAllData[0].m_fRangeX[0] = xmin;
	m_vAllData[0].m_fRangeX[1] = xmax;
	m_vAllData[0].m_fRangeY[0] = ymin;
	m_vAllData[0].m_fRangeY[1] = ymax;
	 
	
	m_fRangeX[OTDR_TRACE_MIN] = xmin;
	m_fRangeX[OTDR_TRACE_MAX] = xmax;
	m_fRangeY[OTDR_TRACE_MIN] = ymin;
	m_fRangeY[OTDR_TRACE_MAX] = ymax;

	m_fResolutionX = (m_fRangeX[OTDR_TRACE_MAX] - m_fRangeX[OTDR_TRACE_MIN]) / m_rectCurve.Width();
	m_fResolutionY = (m_fRangeY[OTDR_TRACE_MAX] - m_fRangeY[OTDR_TRACE_MIN]) / m_rectCurve.Height();
	DrawCurves(&m_dcCurve,m_nCurveIndex);
	if (bSetScrollInof)
	{
		SetMyScrollInfo(xmin, xmax,ymin,ymax);
	}

	Invalidate(FALSE);
}
//���������
void COtdrTrace::SetMaxDistance(FLOAT fMaxDistance)
{
	if (m_vAllData.size() > 0)
	{
		if(m_vAllData[0].m_fMaxDistance < fMaxDistance)//ֻ��һ����
		{
			m_vAllData[0].m_fMaxDistance = fMaxDistance;
		}//eif
		m_fMaxDistance=m_vAllData[0].m_fMaxDistance;
	}//eif
}
//�õ������
FLOAT COtdrTrace::GetMaxDistance()
{
	if (m_vAllData.size() > 0)
	{
		return m_vAllData[0].m_fMaxDistance;
	}//eif
	
	return 0;
}
//���ݵ�ǰѡ���A/B�꣬��������ʱ������ƫ��
FLOAT COtdrTrace::GetCenterOffset()
{
	if (IsCurrentTypeAB())//��ǰѡ��AB��
	{
		return (m_cursorA.m_fPos + m_cursorB.m_fPos)/2;
	}//eif
	if (OTDRTRACE_AB_SELE_A == m_nABSele)//A
	{
		//return (m_cursorA.m_fPos - m_fRangeX[OTDR_TRACE_MIN])/(m_fRangeX[OTDR_TRACE_MAX] + m_fRangeX[OTDR_TRACE_MIN]);
		return m_cursorA.m_fPos;
	}
	else if (OTDRTRACE_AB_SELE_B == m_nABSele)//B
	{
		//return (m_cursorB.m_fPos - m_fRangeX[OTDR_TRACE_MIN])/(m_fRangeX[OTDR_TRACE_MAX] + m_fRangeX[OTDR_TRACE_MIN]);
		return m_cursorB.m_fPos;// - m_fRangeX[OTDR_TRACE_MIN])/(m_fRangeX[OTDR_TRACE_MAX] + m_fRangeX[OTDR_TRACE_MIN]);
	}//eif
	return (m_cursorA.m_fPos + m_cursorB.m_fPos)/2;
}
//�õ�ĳ���Ӧ��dbֵ��fPos:ĳ����λ��
//����ȵ�ǰ���������������������������һ��dbֵ��Ϊ����ֵ
FLOAT COtdrTrace::GetYDB(FLOAT fPos)
{
	if (m_vAllData.size()<m_nCurveIndex + 1)
	{
		return 0;
	}//
	//INT nIndex = FLOAT2POINTX(y);
	INT nIndex = m_vAllData[m_nCurveIndex].DistanceToIndex(fPos);
	if (nIndex >= m_vAllData[m_nCurveIndex].m_nPointsCount)
	{
		nIndex = m_vAllData[m_nCurveIndex].m_nPointsCount - 1;
	}//
	if(nIndex < 0 || nIndex > m_vAllData[m_nCurveIndex].m_nPointsCount) //���AB��ͬʱ�ƶ�ʱ����Χ�����⡣lzy2014.9.10
		return 0;

	return m_vAllData[m_nCurveIndex].m_pData[nIndex];
}
//ˮƽ�Ŵ� fPercent�Ŵ������Ĭ��2%
void COtdrTrace::ZoomInHorizonForBig(FLOAT fPercent)
{
	FLOAT fCenter = GetCenterOffset();
	FLOAT m_fRangeXMIN = fCenter - (fCenter - m_fRangeX[OTDR_TRACE_MIN])*(1 - fPercent);
	FLOAT m_fRangeXMAX = fCenter + (m_fRangeX[OTDR_TRACE_MAX] - fCenter)*(1 - fPercent);
	SetRange(m_fRangeXMIN, m_fRangeXMAX, m_fRangeY[OTDR_TRACE_MIN], m_fRangeY[OTDR_TRACE_MAX]);
}
//ˮƽ��С fPercent�Ŵ������Ĭ��2%
void COtdrTrace::ZoomOutHorizonForSmall(FLOAT fPercent)
{
	FLOAT fCenter = GetCenterOffset();
	FLOAT m_fRangeXMIN = fCenter - (fCenter - m_fRangeX[OTDR_TRACE_MIN])*(1/(1 - fPercent));
	FLOAT m_fRangeXMAX = fCenter + (m_fRangeX[OTDR_TRACE_MAX] - fCenter)*(1/(1 - fPercent));
	SetRange(m_fRangeXMIN, m_fRangeXMAX, m_fRangeY[OTDR_TRACE_MIN], m_fRangeY[OTDR_TRACE_MAX]);
}
//��ֱ�Ŵ� fPercent�Ŵ������Ĭ��2%
void COtdrTrace::ZoomInVertialForBig(FLOAT fPercent)
{
	FLOAT fCenter = GetCenterOffset();//AB�����
	FLOAT fDb = GetYDB(fCenter);//�õ���괦DBֵ
	FLOAT m_fRangeYMIN = fDb - (fDb - m_fRangeY[OTDR_TRACE_MIN])*(1 - fPercent);
	FLOAT m_fRangeYMAX = fDb + (m_fRangeY[OTDR_TRACE_MAX] - fDb)*(1 - fPercent);
	SetRange(m_fRangeX[OTDR_TRACE_MIN], m_fRangeX[OTDR_TRACE_MAX], m_fRangeYMIN, m_fRangeYMAX);
}
//��ֱ��СfPercent�Ŵ������Ĭ��2%
void COtdrTrace::ZoomOutVertialForSmall(FLOAT fPercent)
{
	FLOAT fCenter = GetCenterOffset();//AB�����
	FLOAT fDb = GetYDB(fCenter);//�õ���괦DBֵ
	FLOAT m_fRangeYMIN = fDb - (fDb - m_fRangeY[OTDR_TRACE_MIN])*(1/(1 - fPercent));
	FLOAT m_fRangeYMAX = fDb + (m_fRangeY[OTDR_TRACE_MAX] - fDb)*(1/(1 - fPercent));
	SetRange(m_fRangeX[OTDR_TRACE_MIN], m_fRangeX[OTDR_TRACE_MAX], m_fRangeYMIN, m_fRangeYMAX);
}
//��ԭ��ˮƽ��ֱ����Ϊ0
void COtdrTrace::ZoomReset()
{
	//��ֱƫ����Ϊ0
	for(int i=0; i<m_vAllData.size(); i++)
	{
		m_vAllData[i].m_nOffsetY = 0;
	}
	SetRange(0, GetMaxDistance(), OTDR_TRACE_Y_MIN, g_nTraceYmax);
	SetScrollRangeX();//����ˮƽ����Χ
}
////�����Ŵ�Ĭ�ϷŴ�10%  nPointX�������ˮƽx����  nPointY�������Y���� 
void COtdrTrace::ZoomOutClickForBig(INT nPointX, INT nPointY, FLOAT fPercent)
{
	m_rectMouseSelect.InflateRect(150,150);
	SetRange(POINT2FLOATX(m_rectMouseSelect.left), POINT2FLOATX(m_rectMouseSelect.right), POINT2FLOATY(m_rectMouseSelect.bottom), POINT2FLOATY(m_rectMouseSelect.top));
	
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void COtdrTrace::SetHorScroll() //����ˮƽ������
{
	//ˮƽ������
	SCROLLINFO siHor;
	siHor.cbSize = sizeof(SCROLLINFO);
	siHor.fMask = SIF_ALL; 
	siHor.nMin = 0;
	if (0 != m_fMaxDistance)
	{
		siHor.nMax = m_fMaxDistance;//��ô����windows������Ĺ�����λ������Ϊsi.nmax-si.npage+1,������si.nmax
	}
	else
	{
		siHor.nMax = m_rectCurve.Width();
	}
	//siHor.nMax = 66;
	//siHor.nPage = 22;//m_rectCurve.Width();
	siHor.nMax = m_rectCurve.Width();//zll 2013.4.23
	siHor.nPage = m_rectCurve.Width();//zll 2013.4.23
	siHor.nPos = 0;
	SetScrollInfo(SB_HORZ, &siHor, TRUE);
}

void COtdrTrace::SetVerScoll() //������ֱ������
{
	SCROLLINFO siVer;
	siVer.cbSize = sizeof(SCROLLINFO);
	siVer.fMask = SIF_ALL; 
	siVer.nMin = OTDR_TRACE_Y_MIN;
	siVer.nMax = g_nTraceYmax;
	siVer.nPage = g_nTraceYmax;
	siVer.nPos = 0;
	//this->SetScrollRange( SB_VERT,0,10000);
	SetScrollInfo(SB_VERT, &siVer, TRUE);
}

//������
//��ʼ��������
void COtdrTrace::InitScrollInfo()
{
	m_nHorzMaxSize = m_rectCurve.Width();
	m_nVertMaxSize = g_nTraceYmax * OTDR_TRACE_Y_FACTOR;

	m_nHorzPageSize = m_rectCurve.Width();
	if (0==m_rectCurve.Height())
	{
		return;
	}
	m_nVertPageSize = (g_nTraceYmax - OTDR_TRACE_Y_MIN) * OTDR_TRACE_Y_FACTOR/m_rectCurve.Height();

	m_nHorzOneStep = 0;
	m_nVertOneStep = (g_nTraceYmax - OTDR_TRACE_Y_MIN) * OTDR_TRACE_Y_FACTOR * OTDR_TRACE_DEFAULT_ZOOM_PERCENT;
	m_nHorzPos = 0;
	m_nVertPos = 0;
	//ˮƽ
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL; 
	si.nMin = 0;
	si.nMax = m_rectCurve.Width();
	si.nPage = m_rectCurve.Width();//
	si.nPos = 0;
	SetScrollInfo(SB_HORZ, &si, TRUE);
	//��ֱ
	si.fMask = SIF_ALL; 
	si.nMin = OTDR_TRACE_Y_MIN * OTDR_TRACE_Y_FACTOR;
	si.nMax = g_nTraceYmax * OTDR_TRACE_Y_FACTOR;
	si.nPage = (g_nTraceYmax - OTDR_TRACE_Y_MIN) * OTDR_TRACE_Y_FACTOR;//
	si.nPos = 0;
	SetScrollInfo(SB_VERT, &si, TRUE);
	//SetScrollRange(OTDR_TRACE_Y_MIN, g_nTraceYmax);
}

//���ù�������Χ
void COtdrTrace::SetMyScrollInfo(float xmin, float xmax, float ymin, float ymax)
{
	SCROLLINFO si;
	//ˮƽ
	GetScrollInfo(SB_HORZ, &si);
	si.nPage = xmax - xmin;
	SetScrollInfo(SB_HORZ, &si, TRUE);

	m_nHorzPos = xmin;
	m_nHorzOneStep = si.nPage * OTDR_TRACE_DEFAULT_ZOOM_PERCENT;
	m_nHorzPageSize = si.nPage;
	SetScrollRange(SB_HORZ, 0, m_fMaxDistance);
	SetScrollPos(SB_HORZ, m_nHorzPos);

	//��ֱ
	ymin *= OTDR_TRACE_Y_FACTOR;
	ymax *= OTDR_TRACE_Y_FACTOR;
	GetScrollInfo(SB_VERT, &si);
	si.nPage = ymax - ymin;
	//si.nMax = ymax;
	SetScrollInfo(SB_VERT, &si, TRUE);

	m_nVertPos = ymin;
	m_nVertOneStep = (ymax - ymin) * OTDR_TRACE_DEFAULT_ZOOM_PERCENT;
	m_nVertPageSize = ymax - ymin;
	//m_nVertMaxSize = 
	SetScrollPos(SB_VERT, m_nVertPos);
	//
}

void COtdrTrace::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	switch(nSBCode)
	{
		//////////////case SB_LEFT://��Home����
		//////////////	m_nHorzPos = 0;
		//////////////	this->SetScrollPos(SB_HORZ,m_nHorzPos);
		//////////////	SetRange(0, m_nHorzPageSize, m_fRangeY[OTDR_TRACE_MIN], m_fRangeY[OTDR_TRACE_MAX], FALSE);
		//////////////	break;
	case SB_LINELEFT:
		if(0 == m_nHorzPos)
		{
			break;
		}
		m_nHorzPos -= m_nHorzOneStep;
		if (m_nHorzPos < 0)
		{
			m_nHorzPos = 0;
		}//

		this->SetScrollPos(SB_HORZ, m_nHorzPos);
		//TRACE(_T("SB_LINELEFT: %d\n"),m_nHorzPos);
		if (0 == m_nHorzPos)
		{
			SetRange(0, m_nHorzPageSize, m_fRangeY[OTDR_TRACE_MIN], m_fRangeY[OTDR_TRACE_MAX], FALSE);
		}
		else
		{
			SetRange(m_fRangeX[OTDR_TRACE_MIN] - m_nHorzOneStep, m_fRangeX[OTDR_TRACE_MAX]  - m_nHorzOneStep, m_fRangeY[OTDR_TRACE_MIN], m_fRangeY[OTDR_TRACE_MAX], FALSE);
		}//eif
		break;
	case SB_PAGELEFT:
		if (m_nHorzPos - m_nHorzPageSize < 0)
		{
			m_nHorzPos = 0;
			this->SetScrollPos(SB_HORZ, 0);
			SetRange(0, m_nHorzPageSize, m_fRangeY[OTDR_TRACE_MIN], m_fRangeY[OTDR_TRACE_MAX], FALSE);
			break;
		}//eif
		m_nHorzPos -= m_nHorzPageSize;
		if(m_nHorzPos<0)
		{
			m_nHorzPos = 0;
		}
		this->SetScrollPos(SB_HORZ,m_nHorzPos);
		SetRange(m_fRangeX[OTDR_TRACE_MIN] - m_nHorzPageSize, m_fRangeX[OTDR_TRACE_MAX]  - m_nHorzPageSize, m_fRangeY[OTDR_TRACE_MIN], m_fRangeY[OTDR_TRACE_MAX], FALSE);
		break;
	case SB_LINERIGHT:
		if (m_nHorzPos + m_nHorzPageSize > m_nHorzMaxSize)
		{
			break;
		}//eif
		m_nHorzPos += m_nHorzOneStep;
		if(m_nHorzPos>m_nHorzMaxSize)
		{
			m_nHorzPos = m_nHorzMaxSize;
		}
		this->SetScrollPos(SB_HORZ,m_nHorzPos);
		//TRACE(_T("SB_LINERIGHT: %d  page: %d max: %d\n"),m_nHorzPos,m_nHorzPageSize,m_nHorzMaxSize);
		SetRange(m_fRangeX[OTDR_TRACE_MIN] + m_nHorzOneStep, m_fRangeX[OTDR_TRACE_MAX] + m_nHorzOneStep, m_fRangeY[OTDR_TRACE_MIN], m_fRangeY[OTDR_TRACE_MAX], FALSE);
		break;
	case SB_PAGERIGHT:
		if (m_nHorzPos + m_nHorzPageSize > m_nHorzMaxSize)
		{
			m_nHorzPos = m_nHorzMaxSize;
			SetRange(m_fMaxDistance - m_nHorzPageSize, m_fMaxDistance, m_fRangeY[OTDR_TRACE_MIN], m_fRangeY[OTDR_TRACE_MAX], FALSE);
			break;
		}//eif
		m_nHorzPos += m_nHorzPageSize;
		this->SetScrollPos(SB_HORZ,m_nHorzPos);
		if (m_nHorzPos + m_nHorzPageSize > m_fMaxDistance)
		{
			SetRange(m_fMaxDistance - m_nHorzPageSize, m_fMaxDistance, m_fRangeY[OTDR_TRACE_MIN], m_fRangeY[OTDR_TRACE_MAX], FALSE);
			break;
		}
		SetRange(m_nHorzPos, m_nHorzPos + m_nHorzPageSize, m_fRangeY[OTDR_TRACE_MIN], m_fRangeY[OTDR_TRACE_MAX], FALSE);
		break;
		//////////////case SB_RIGHT://��End����
		//////////////	m_nHorzPos = m_nHorzMaxSize;
		//////////////	this->SetScrollPos(SB_HORZ,m_nHorzPos);
		//////////////	SetRange(m_fMaxDistance - m_nHorzPageSize, m_fMaxDistance, m_fRangeY[OTDR_TRACE_MIN], m_fRangeY[OTDR_TRACE_MAX], FALSE);
		//////////////	break;
	case SB_THUMBTRACK://�϶�������
		SCROLLINFO si;
		GetScrollInfo(SB_HORZ, &si);
		m_nHorzPos = si.nTrackPos;
		this->SetScrollPos(SB_HORZ,m_nHorzPos);
		//TRACE(_T("SB_THUMBTRACK0: %d\n"),m_nHorzPos);
		SetRange(m_nHorzPos, m_nHorzPos + m_nHorzPageSize, m_fRangeY[OTDR_TRACE_MIN], m_fRangeY[OTDR_TRACE_MAX], FALSE);
		break;
	case SB_THUMBPOSITION://�϶�����
		//SCROLLINFO si;
		GetScrollInfo(SB_HORZ, &si);
		m_nHorzPos = si.nPos;
		this->SetScrollPos(SB_HORZ,m_nHorzPos);
		//TRACE(_T("SB_THUMBPOSITION1: %d\n"),m_nHorzPos);
		SetRange(m_nHorzPos, m_nHorzPos + m_nHorzPageSize, m_fRangeY[OTDR_TRACE_MIN], m_fRangeY[OTDR_TRACE_MAX], FALSE);
		break;
	default:
		break;
	}//eofswitch
	CButton::OnHScroll(nSBCode, nPos, pScrollBar);
}
void COtdrTrace::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(nSBCode)
	{
		//////////////case SB_LEFT://��Home����
		//////////////	m_nHorzPos = 0;
		//////////////	this->SetScrollPos(SB_HORZ,m_nHorzPos);
		//////////////	SetRange(0, m_nHorzPageSize, m_fRangeY[OTDR_TRACE_MIN], m_fRangeY[OTDR_TRACE_MAX], FALSE);
		//////////////	break;
		//////////////case SB_RIGHT://��End����
		//////////////	m_nHorzPos = m_nHorzMaxSize;
		//////////////	this->SetScrollPos(SB_HORZ,m_nHorzPos);
		//////////////	SetRange(m_fMaxDistance - m_nHorzPageSize, m_fMaxDistance, m_fRangeY[OTDR_TRACE_MIN], m_fRangeY[OTDR_TRACE_MAX], FALSE);
		//////////////	break;
	case SB_LINELEFT:
		if(0 == m_fRangeY[OTDR_TRACE_MIN])
		{
			break;
		}
		m_nVertPos -= m_nVertOneStep;
		if (m_nVertPos < 0)
		{
			m_nVertPos = 0;
		}//

		this->SetScrollPos(SB_VERT, m_nVertPos);
		//TRACE(_T("SB_LINELEFT: %d\n"),m_nVertPos);
		if (0 == m_nVertPos)
		{
			SetRange(m_fRangeX[OTDR_TRACE_MIN], m_fRangeX[OTDR_TRACE_MAX], OTDR_TRACE_Y_MIN, OTDR_TRACE_Y_MIN + m_nVertPageSize / OTDR_TRACE_Y_FACTOR, FALSE);
		}
		else
		{
			SetRange(m_fRangeX[OTDR_TRACE_MIN], m_fRangeX[OTDR_TRACE_MAX], m_fRangeY[OTDR_TRACE_MIN] - m_nVertOneStep / OTDR_TRACE_Y_FACTOR, m_fRangeY[OTDR_TRACE_MAX] - m_nVertOneStep / OTDR_TRACE_Y_FACTOR, FALSE);
		}//eif
		break;
	case SB_PAGELEFT:
		m_nVertPos -= m_nVertPageSize;
		if(m_nVertPos<0)
		{
			m_nVertPos = 0;
		}
		this->SetScrollPos(SB_VERT,m_nVertPos);
		SetRange(m_fRangeX[OTDR_TRACE_MIN], m_fRangeX[OTDR_TRACE_MAX], m_nVertPos / OTDR_TRACE_Y_FACTOR, (m_nVertPos + m_nVertPageSize) / OTDR_TRACE_Y_FACTOR, FALSE);
		break;
	case SB_LINERIGHT:
		if (m_nVertPos == m_nVertMaxSize)
		{
			break;
		}//eif
		m_nVertPos += m_nVertOneStep;
		if(m_nVertPos > m_nVertMaxSize)
		{
			m_nVertPos = m_nVertMaxSize;
		}
		this->SetScrollPos(SB_VERT,m_nVertPos);
		//TRACE(_T("SB_LINERIGHT: %d  page: %d max: %d\n"),m_nVertPos,m_nVertPageSize,m_nVertMaxSize);
		SetRange(m_fRangeX[OTDR_TRACE_MIN], m_fRangeX[OTDR_TRACE_MAX], m_fRangeY[OTDR_TRACE_MIN] + m_nVertOneStep / OTDR_TRACE_Y_FACTOR, m_fRangeY[OTDR_TRACE_MAX] + m_nVertOneStep / OTDR_TRACE_Y_FACTOR, FALSE);
		break;
	case SB_PAGERIGHT:
		m_nVertPos += m_nVertPageSize;
		if(m_nVertPos>m_nVertMaxSize)
		{
			m_nVertPos = m_nVertMaxSize;
			this->SetScrollPos(SB_VERT,m_nVertPos);
			//TRACE(_T("SB_LINERIGHT: %d  page: %.2f max: %.2f\n"),m_nVertPos,m_fRangeY[OTDR_TRACE_MIN] - m_nVertPageSize,m_fRangeY[OTDR_TRACE_MAX] - m_nVertPageSize);
			SetRange(m_fRangeX[OTDR_TRACE_MIN], m_fRangeX[OTDR_TRACE_MAX], g_nTraceYmax - m_nVertPageSize / OTDR_TRACE_Y_FACTOR, g_nTraceYmax, FALSE);
			break;
		}//eif
		this->SetScrollPos(SB_VERT,m_nVertPos);
		//TRACE(_T("SB_LINERIGHT: %d  page: %.2f max: %.2f\n"),m_nVertPos,m_fRangeY[OTDR_TRACE_MIN] - m_nVertPageSize,m_fRangeY[OTDR_TRACE_MAX] - m_nVertPageSize);
		SetRange(m_fRangeX[OTDR_TRACE_MIN], m_fRangeX[OTDR_TRACE_MAX], m_nVertPos / OTDR_TRACE_Y_FACTOR, (m_nVertPos + m_nVertPageSize) / OTDR_TRACE_Y_FACTOR, FALSE);
		break;
	case SB_THUMBTRACK://�϶�������
		SCROLLINFO si;
		GetScrollInfo(SB_VERT, &si);
		m_nVertPos = si.nTrackPos;
		this->SetScrollPos(SB_VERT,m_nVertPos);
		//TRACE(_T("SB_THUMBTRACK0: %d\n"),m_nVertPos);
		SetRange(m_fRangeX[OTDR_TRACE_MIN], m_fRangeX[OTDR_TRACE_MAX], m_nVertPos / OTDR_TRACE_Y_FACTOR, (m_nVertPos + m_nVertPageSize) / OTDR_TRACE_Y_FACTOR, FALSE);
		break;
	case SB_THUMBPOSITION://�϶�����
		//SCROLLINFO si;
		GetScrollInfo(SB_VERT, &si);
		m_nVertPos = si.nPos;
		this->SetScrollPos(SB_VERT,m_nVertPos);
		//TRACE(_T("SB_THUMBPOSITION1: %d\n"),m_nVertPos);
		SetRange(m_fRangeX[OTDR_TRACE_MIN], m_fRangeX[OTDR_TRACE_MAX], m_nVertPos / OTDR_TRACE_Y_FACTOR, (m_nVertPos + m_nVertPageSize) / OTDR_TRACE_Y_FACTOR, FALSE);
		break;
	default:
		break;
	}//eofswitch

	CButton::OnVScroll(nSBCode, nPos, pScrollBar);
}

//����ˮƽ����Χ//zll2013.4.23
void COtdrTrace::SetScrollRangeX()
{
	SetScrollRange(SB_HORZ, 0, m_fMaxDistance);
	m_nHorzMaxSize = m_fMaxDistance;
}

void COtdrTrace::AddEvent()
{
	TestResultInfo* pData = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;
	int nSelectPoint = 0;
	if (m_nSelectCursor == CURSOR_SELECT::CURSOR_A)
	{
		nSelectPoint = m_nPointA;
	}
	else if (m_nSelectCursor == CURSOR_SELECT::CURSOR_B)
	{
		nSelectPoint = m_nPointB;
	}
	else
		return;

	int waveNum = pData->WaveLength; 
	__try
	{
		pData->m_GaborResult = m_endgabor.insertEvent(nSelectPoint, //��Ҫ������¼�������ݵ���
			waveNum, //���߲���
			pData->PulseWidth, //��������
			pData->pDataPoints, //���Ե�����
			pData->DataPointsNum, //����
			pData->m_GaborResult,
			pData->Refraction,//������zll
			pData->Frequency,
			g_sorFileArray.eventAnalConfig.BackScatter);//g_sorFileArray.eventAnalConfig.BackScatter����ɢ��ϵ��
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return;
	}
	GaborResultToEvent(pData);
	//�ػ�
	InvalidateRect(m_rectCurve);
	MAIN->m_pEventFormView->FreshEventList();//ˢ���¼��б�
	//����ѡ����
	MAIN->m_pEventFormView->m_Eventlist.SetItemState(MAIN->m_pEventFormView->m_Eventlist.GetItemCount()-1, LVIS_SELECTED, LVIS_SELECTED);
}

//����dll�õ����ݺ�ת��Ϊ�¼��б�
void COtdrTrace::GaborResultToEvent(TestResultInfo* pCurveData)
{
	pCurveData->EventList.vEventInfo.clear();
	EventInfo eventinfo;
	int len = pCurveData->m_GaborResult.Valid_Num;//wcq2012.06.07
	float fDisTwo=Get_XCoord(1,pCurveData);
	for(int i=0;i<len;i++)
	{
		if (0==fDisTwo)
		{
			int ff=0;
		}//eif
		eventinfo.index = pCurveData->m_GaborResult.Event_Array[i];//�±꣬�ڼ���
		eventinfo.nEventType = pCurveData->m_GaborResult.Reflect_Type[i];//�¼�����
		eventinfo.fLocation = fDisTwo * pCurveData->m_GaborResult.Event_Array[i];//Get_XCoord(sGabor_Result..Event_Array[i],pCurveData);//�������룬��kmΪ��λ
		eventinfo.fEventloss = pCurveData->m_GaborResult.Event_Loss[i];
		eventinfo.fEventHigh = pCurveData->m_GaborResult.Event_Hight[i];
		eventinfo.fLineK = pCurveData->m_GaborResult.Line_k[i];// / fDisTwo;//2012.8.7 rzg�޸�
		eventinfo.fTotalLoss = pCurveData->m_GaborResult.Total_Loss[i];
		eventinfo.fReflect =pCurveData->m_GaborResult.Reflect;
		eventinfo.fTrailEnd = pCurveData->m_GaborResult.Trail_End;
		eventinfo.cEventType=pCurveData->m_GaborResult.Event_type[i];//event code
		pCurveData->EventList.vEventInfo.push_back(eventinfo);
	}//efor
}
/*
***************************************************************
��������:             Get_Xcoord()                                                                               
����˵��:             ���ɼ�������ת�������ߵĺ������                                                                              
��ڲ���:             xpoint�����ݵ�����                                                                                             
���ڲ���:             X��ת����ĺ������   ��λ  km                                                                                          
����ʱ��:             2010-06-24                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
float COtdrTrace::Get_XCoord(unsigned int xpoint,TestResultInfo* pData)
{
	//return xpoint * pData->DataSpacing * 1.0/100000000;//wcq2011.11.25
	//return C/(5000 * pData->Refraction * 1000000/*MHz*/) * xpoint
	return C_zll/( pData->Refraction)/pData->Frequency/1000000/2* xpoint;//2012.8.3 rzg�޸Ĺ�ʽ /1000��ʾ����Ϊ��KmΪ��λ
}

//ѡ�����¼��б�����»�������;wcq2011-11.15
//nIndex:��0��ʼ��Ӧ���ߵڼ����¼���
void COtdrTrace::MoveEventList(INT nIndex)
{
	if (m_vAllData.size()==0)//��m_vAllData�������������˳������򳬳���Χ
	{
		return;
	}
	if (OTDRTRACE_AB_SELE_A == m_nABSele) //ѡ��A
	{
		m_cursorA.m_fPos = m_vAllData[m_nCurveIndex].m_vAllEvents[nIndex].m_distance;
	}	
	else if (OTDRTRACE_AB_SELE_B == m_nABSele)////ѡ��B
	{
		m_cursorB.m_fPos = m_vAllData[m_nCurveIndex].m_vAllEvents[nIndex].m_distance;
	}
	Invalidate();//wcq2011.11.16
}

//����һ������wcq 2011.09.21
void COtdrTrace::HorizonZoom(TestResultInfo* pData)
{
	if(CLEAR_DATAPOINTS_NUM == g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->DataPointsNum)//�տ�ʼ����ʱ��գ�������.wcq2011.12.23
	{
		return;
	}

	int m_nPointLabel; //m_nSelectCursor:Seleced��־,1ѡ��A��2ѡ��B, 3ѡ��AB

	if (CURSOR_SELECT::CURSOR_A == m_nSelectCursor) //ѡ��A
	{
		m_nPointLabel=m_nPointA;
	}	
	else if (CURSOR_SELECT::CURSOR_B == m_nSelectCursor)
	{
		m_nPointLabel=m_nPointB;
	}
	else//AB
	{
		m_nPointLabel=(m_nPointA + m_nPointB)/2;
	}//eif

	float fLeftRightCoeff=m_nPointLabel * 1.0/g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo->DataPointsNum;//����ʱ���ұ��������Ҳ�һ
	//������������������Ҷನ����ʾģʽ����������������ߵ�A/B��λ�ý���ת��
	if (!pData->IsMaxLength && g_sorFileArray.waveConfig.MultiMode)
	{
		float fTemp=pData->DataSpacing / g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo->DataSpacing;//���������ߵ�ԭ�������ȡ�wcq2011.12.01
		m_nPointLabel=m_nPointLabel * fTemp;
	}

	if(0 == m_iHorizonZoomNum) //δ����
	{
		pData->StartPoint=0;
		pData->MaxPoint=pData->DataPointsNum;
	}
	else
	{
		//ԭ����cwcq2012.10.26
		//////float ZoomCoeff;
		//////ZoomCoeff=(float)pow(2.0,m_iHorizonZoomNum+1);//�Ŵ�����1/2
		//////if (m_nPointLabel >= pData->MaxPoint/ZoomCoeff)
		//////{
		//////	pData->StartPoint=(unsigned int)(m_nPointLabel - pData->MaxPoint/ZoomCoeff+1);
		//////	pData->MaxPoint=(unsigned int)(m_nPointLabel+pData->MaxPoint/ZoomCoeff);
		//////}				  
		//////else 
		//////{
		//////	pData->StartPoint=0;
		//////	pData->MaxPoint=(unsigned int)(2*pData->MaxPoint/ZoomCoeff);
		//////}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		int multi=1;//���ٱ����߿��
		int pointCount=0;
		if (m_iHorizonZoomNum ==m_nZoomCount)//������ű���
		{
			//pointCount=pData->m_TestResult.LastZoomPointCount;
			pointCount=pData->FullPointCountMin;
		}
		else
		{
			pointCount=pData->FullPointCountMin;//wcq2011.11.22
			int count= m_nZoomCount - m_iHorizonZoomNum;//���һ�����ŵ�����һ��
			double dPow=pow(2.0,m_iHorizonZoomNum);
			pointCount =pData->FullPointCountMin/dPow;
			//if (count < 20)//40++
			//{
			//	pointCount += count * pData->FullPointCountMin;
			//}
			//else if (count < 30)//30~39
			//{
			//	pointCount += ( 20 + 2 * (count - 19) ) * pData->FullPointCountMin;
			//}
			//else if (count < 40)//20~29
			//{
			//	pointCount +=( 40 + 3 * (count - 29) ) * pData->FullPointCountMin;
			//}
			//else// if (count < 50)//30~39
			//{
			//	pointCount +=( 70 + 4 * (count - 39) ) * pData->FullPointCountMin;
			//}
		}
		int itemp=pointCount * fLeftRightCoeff;//
		if (itemp > m_nPointLabel)
		{
			pData->StartPoint=0;
			pData->MaxPoint = m_nPointLabel + ( pointCount - itemp)-1;
		}
		else
		{
			pData->StartPoint = m_nPointLabel - itemp;//���λ��(��ʼ��λ��)
			pData->MaxPoint = m_nPointLabel + ( pointCount - itemp)-1;//�ұ�λ��(������λ��),1:��ȥ�������ĵ㱾��
		}

		if (pData->StartPoint > pData->DataPointsNum)
		{
			pData->StartPoint = pData->DataPointsNum-1;//wcq2011.11.29
		}
		if (pData->MaxPoint > pData->DataPointsNum)
		{
			pData->MaxPoint = pData->DataPointsNum;
		}		
		else if (pData->MaxPoint <0)
		{
			pData->MaxPoint = 0;
		}
		pData->FullPointCount=pointCount;//wcq2011.11.29
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}//eif
	if (pData->StartPoint >= pData->MaxPoint)
	{
		pData->MaxPoint = pData->StartPoint + 1; //��ֹ��0��
	}
	if (pData->IsMaxLength || (!g_sorFileArray.waveConfig.MultiMode))//��������ʾ
	{
		m_nStartPoint = pData->StartPoint;
		m_nMaxPoint = pData->MaxPoint;
	}
}

//ÿ�μ��ػ����һ���µ���������ʱ���¼���A/B��λ�ã���ȷ�����������߲�������������m_curABWave.
//����и����������ߵ����������¼������Ų���
//wcq 2011.0913
void COtdrTrace::ReCalABLabelPos()
{
	CalMaxDistance();//�ȼ���������������ľ��벢�����������߽ṹ��Ӧ����

	BOOL bTheSame=TRUE;//�����������Ƿ���ϴε�һ����Ĭ��һ��
	DWORD maxWave=0;//��������߶�Ӧ��map�Ĳ�����ֵ
	//ͨ������ȷ���Ƿ�һ��
	for(int nCount = 0; nCount < g_sorFileArray.totalCurveNumber; nCount++)
	{
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
		if (pCurve && pCurve->IsMaxLength)
		{
			if (nCount != m_nMaxIndex)
			{
				bTheSame = FALSE;
				m_nMaxIndex = nCount;
				break;
			}
		}
	}

	//�����һ�������μ�������������Ӧ���ߵ�A/B��λ��
	if (bTheSame)//һ��������
	{
		return;
	}
	ReCalZoomParas();//�и��������������ݵ��������¼���������ز�����//wcq2011.09.23
	//�ȼ����µ�������A/B��λ��
	//������Ϊ����
	//float fTemp=m_pMapCurveData[maxWave]->m_TestResult.Frequency * m_pMapCurveData[maxWave]->m_TestResult.Refraction * m_pMapCurveData[m_curABWave]->m_TestResult.DataPointsNum;//wcq2011.11.25
	//fTemp=fTemp/(m_pMapCurveData[m_curABWave]->m_TestResult.Frequency * m_pMapCurveData[m_curABWave]->m_TestResult.Refraction * m_pMapCurveData[maxWave]->m_TestResult.DataPointsNum);//wcq2011.11.25
	float fTemp=g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo->Length / g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->Length;//wcq2011.11.25
	m_nPointA=fTemp * m_nPointA;
	m_nPointB=fTemp * m_nPointB;
}

/*
***************************************************************
��������:             VerticalZoom                                                                              
����˵��:             ��ֱ����仯����                                                                           
��ڲ���:             ��                                                                                      
���ڲ���:             ��                                                                                        
����ʱ��:             2011-07-25                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void COtdrTrace::VerticalZoom()
{
	if (g_sorFileArray.totalCurveNumber == 0) //û������ʱ
		return;

	int m_nPointLabel; //m_nSelectCursor:Seleced��־,1ѡ��A��2ѡ��B, 3ѡ��AB
	//ȷ��ѡ�е���A/B
	if (CURSOR_SELECT::CURSOR_A == m_nSelectCursor) //ѡ��A
	{
		m_nPointLabel=m_nPointA;
	}
	else if (CURSOR_SELECT::CURSOR_B == m_nSelectCursor)
	{
		m_nPointLabel=m_nPointB;
	}
	else//AB
	{
		m_nPointLabel=(m_nPointA + m_nPointB)/2;
	}//eif

	//��ֹԽ��
	if (m_nPointLabel<0)
	{
		m_nPointLabel=0;
	}//eif
	if (m_nPointLabel>g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo->DataPointsNum)
	{
		m_nPointLabel=g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo->DataPointsNum;
	}//eif
	//
	float fPointLabelDb;
	//fPointLabelDb=m_pMapCurveData[m_curABWave]->m_TestResult.pDataPoints[m_nPointLabel];//???????????
	fPointLabelDb=g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo->pDataPoints[m_nPointLabel]/(-1000.0);//?????�Ƿ�Ҫ��-1000

	CalVerticalRange(m_iVerticalZoomNum, fPointLabelDb);//������ֱ��������ʱ���ֵ��Сֵ.����m_fStartDB,m_fMaxDB.wcq2011.09.28

	//�������û�ͼ�����С
	if (m_iVerticalZoomNum == 0) //������
	{
		m_fCoeff = 1.0;
		m_nDrawHeight = m_rectCurve.Height();
	}
	else
	{
		m_nDrawHeight = m_rectCurve.Height()/m_fCoeff;
	}
	SetVerScoll();
}

//���²��Ժ��ʼ��һЩ��������A/B��λ�� wcq2012.02.21
void COtdrTrace::NewTestInit(TestResultInfo* pData)
{
	if (pData->Length < m_lastTestInfo.m_distanceA)//ԭ����A��λ�ñȱ��β��Թ��˳��ȴ�
	{
		m_nPointA = pData->DataPointsNum / 2;//ȡ�м�λ��
		m_iHorizonZoomNum = 0;
	}
	else
	{
		m_nPointA = Get_PosPoint(m_lastTestInfo.m_distanceA,pData);//���Ӧ��λ��
	}

	if (pData->Length < m_lastTestInfo.m_distanceB)//ԭ����A��λ�ñȱ��β��Թ��˳��ȴ�
	{
		m_nPointB = pData->DataPointsNum / 2;//ȡ�м�λ��
		m_iHorizonZoomNum =0;
	}
	else
	{
		m_nPointB = Get_PosPoint(m_lastTestInfo.m_distanceB,pData);//���Ӧ��λ��
	}
}

/*
***************************************************************
��ڲ���:
���ڲ���:
����ʱ��:2011-09-09
Write By:wcq
����˵��:����������������ľ��벢�����������߽ṹ��Ӧ����
***************************************************************
*/
void COtdrTrace::CalMaxDistance()
{
	int nMaxIndex=0;//��������߶�Ӧ������
	float fMaxDistance=0;//������
	float fTemp=0;//��ʱ����
	bool bFirst=true;//�Ƿ��һ��������Ч���ݵ�����
	//����
	for(int nCount = 0; nCount < g_sorFileArray.totalCurveNumber; nCount++)
	{
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
		if ((!pCurve) || (CLEAR_DATAPOINTS_NUM == pCurve->DataPointsNum) )//wcq2011.12.23
		{
			continue;
		}
		if (bFirst)
		{
			fMaxDistance= pCurve->Length;//wcq2011.11.25//(C/(2 * pCurve->m_TestResult.Frequency * 1000000 * pCurve->m_TestResult.Refraction)) * (pCurve->m_TestResult.DataPointsNum);
			bFirst=false;
		}
		else{
			fTemp= pCurve->Length;//wcq2011.11.25//(C/(2 * pCurve->m_TestResult.Frequency * 1000000 * pCurve->m_TestResult.Refraction)) * (pCurve->m_TestResult.DataPointsNum);
			if(fTemp > (fMaxDistance+0.0001) )//0.0001:�����ܽӽ���float�Ƚ�ʱ�������ȷ��
			{
				fMaxDistance = fTemp;
				nMaxIndex = nCount;//����
			}
		}
		//pCurve->m_TestResult.Length = fMaxDistance;//���߳���wcq2011.11.10
	}
	//��ֵ
	for(int nCount = 0; nCount < g_sorFileArray.totalCurveNumber; nCount++)
	{
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
		if (pCurve)
		{
			//�Ƿ�Ϊ��������
			if (nMaxIndex == nCount)
			{
				pCurve->IsMaxLength=TRUE;
				//m_curABWave = it->first;
				m_nMaxIndex = nMaxIndex;
			}
			else
			{
				pCurve->IsMaxLength=FALSE;
			}
			//�����
			pCurve->MaxTestDistance=fMaxDistance;
		}
	}
}
//��һ�λ��и��������������ݵ��������¼���������ز��������������GetZoomCount,CalFullPointCount,GetLastZoomCount.wcq2011.09.23
void COtdrTrace::ReCalZoomParas()
{
	CalMaxDistance();//�ȼ���������������ľ��벢�����������߽ṹ��Ӧ����
	m_nZoomCount=GetZoomCount(g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo->DataPointsNum);//�õ����Ŵ���
	CalFullPointCount();//������������δ����ʱ������ͼ����ĵ���
	GetLastZoomCounts();//�õ����������������һ�����ŵ����ݵ����
}

/*
***************************************************************
��ڲ���:maxDistanceDataCount:��������ߵ����ݵ���
���ڲ���:���Ŵ���
����ʱ��:2011.09.20
Write By:wcq 
����˵��:�õ��������Ŵ���,1~19:1����20~29:2����30~39:3����40~49:4��
***************************************************************
*/
int COtdrTrace::GetZoomCount(int maxDistanceDataCount)
{
	//int pointCountPerTime=m_rectCurve.Width();//���ŵ�������,686
	int pointCountPerTime=m_rectCoord.Width();//���ŵ�������,686//wcq2011.11.22
	int zoomCount=0;//���Ŵ���
	int count;
	if (pointCountPerTime * (20 + ZOOM_LAST_COEFF) >= maxDistanceDataCount)//1~19
	{
		zoomCount=0;
		for(int i=1;i<20;i++)
		{
			zoomCount++;
			if(pointCountPerTime * (i + ZOOM_LAST_COEFF) >= maxDistanceDataCount)
			{
				break;
			}
		}
	}
	else if (pointCountPerTime * 40 + pointCountPerTime * 2 * ZOOM_LAST_COEFF>= maxDistanceDataCount)//20~29
	{
		zoomCount=19;
		count=pointCountPerTime * 20;
		for(int i=1;i<11;i++)
		{
			zoomCount++;
			if(count + pointCountPerTime * 2 * (i + ZOOM_LAST_COEFF) >= maxDistanceDataCount)
			{
				break;
			}
		}
	}
	else if (pointCountPerTime * 70 + pointCountPerTime * 3 * ZOOM_LAST_COEFF>= maxDistanceDataCount)//30~39
	{
		zoomCount=29;
		count=pointCountPerTime * 40;
		for(int i=1;i<11;i++)
		{
			zoomCount++;
			if(count + pointCountPerTime * 3 * (i + ZOOM_LAST_COEFF) >= maxDistanceDataCount)
			{
				break;
			}
		}
	}
	else if (pointCountPerTime * 110 >= maxDistanceDataCount)//40~49 49:�����ﵽ96040
	{
		zoomCount=39;
		count=pointCountPerTime * 70;
		for(int i=1;i<11;i++)
		{
			zoomCount++;
			if(count + pointCountPerTime * 4 * (i + ZOOM_LAST_COEFF) >= maxDistanceDataCount)
			{
				break;
			}
		}
	}
	else if (pointCountPerTime * 160 >= maxDistanceDataCount)//50~59 59:�����ﵽ109760 wcq2012.03.07
	{
		zoomCount=49;
		count=pointCountPerTime * 110;
		for(int i=1;i<11;i++)
		{
			zoomCount++;
			if(count + pointCountPerTime * 5 * (i + ZOOM_LAST_COEFF) >= maxDistanceDataCount)
			{
				break;
			}
		}
	}
	else if (pointCountPerTime * 220 >= maxDistanceDataCount)//60~69 69:�����ﵽ150920 wcq2012.03.07
	{
		zoomCount=59;
		count=pointCountPerTime * 160;
		for(int i=1;i<11;i++)
		{
			zoomCount++;
			if(count + pointCountPerTime * 6 * (i + ZOOM_LAST_COEFF) >= maxDistanceDataCount)
			{
				break;
			}
		}
	}
	return zoomCount;
}

//������������δ����ʱ������ͼ����ĵ��� wcq 2011.09.21
void COtdrTrace::CalFullPointCount()
{
	for(int nCount = 0; nCount < g_sorFileArray.totalCurveNumber; nCount++)
	{
		TestResultInfo* pData = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
		if (pData)
		{
			if (pData->IsMaxLength)
			{
				//pData->FullPointCountMin = m_rectCurve.Width();
				pData->FullPointCountMin = m_rectCoord.Width();
				//pData->FullPointCount = maxDistanceCount;
			}
			else{
				//��������������Ҫ��һ������,m_curABWave��ʾ������������ڲ���
				//float fTemp=m_pMapCurveData[m_curABWave]->m_TestResult.Frequency * m_pMapCurveData[m_curABWave]->m_TestResult.Refraction * pData->DataPointsNum;//wcq2011.11.25
				//fTemp=fTemp/(pData->Frequency * pData->Refraction * m_pMapCurveData[m_curABWave]->m_TestResult.DataPointsNum);//wcq2011.11.25
				//pData->FullPointCountMin = m_rectCoord.Width() * fTemp;//wcq2011.11.25
				//����ͬһ��(m_rectCoord.Width())��,�������߳������//wcq2011.11.29
				pData->FullPointCountMin = m_rectCoord.Width() * (g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo->DataSpacing  /  pData->DataSpacing);//wcq2011.11.29
			}
		}
	}
}

/*
***************************************************************
��ڲ���:
���ڲ���:
����ʱ��:2011.09.23
Write By:wcq 
����˵��:�õ����������������һ�����ŵ����ݵ����
***************************************************************
*/
void COtdrTrace::GetLastZoomCounts()
{
	//
	for(int nCount = 0; nCount < g_sorFileArray.totalCurveNumber; nCount++)
	{
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
		if (pCurve && pCurve->DataPointsNum>NUM_VALIDE_LEAST)
		{
			pCurve->LastZoomPointCount = GetLastZoomCount(pCurve->FullPointCountMin,pCurve->DataPointsNum,m_nZoomCount);
			//DEBUGMSG(true,(_T("lastzoompointcount:%d wave:%d\n"),pCurve->m_TestResult.LastZoomPointCount,it->first));
		}
	}
}

/*
***************************************************************
��ڲ���:countPerTime:���������ÿ�����ŵ���;dataCount:���ݵ�������;zoomCount:���Ŵ���
���ڲ���:���Ŵ���
����ʱ��:2011.09.21
Write By:wcq 
����˵��:�õ������������һ�����ŵ����ݵ����wcq 
***************************************************************
*/
int COtdrTrace::GetLastZoomCount(int countPerTime,int dataCount,int zoomCount)
{
	int count = 0;
	int nTemp;
	if (zoomCount < 20)
	{
		count = dataCount % countPerTime;
		nTemp = 1;
	}
	else if (zoomCount < 30)
	{
		count = (dataCount - countPerTime * 20) % (countPerTime * 2);
		nTemp = 2;
	}
	else if (zoomCount < 40)
	{
		count = (dataCount - countPerTime * 40) % (countPerTime * 3);
		nTemp = 3;
	}
	else if (zoomCount < 50)//40~49
	{
		count = (dataCount - countPerTime * 70) % (countPerTime * 4);
		nTemp = 4;
	}
	else if (zoomCount < 60)//50~59  wcq2012.03.07
	{
		count = (dataCount - countPerTime * 110) % (countPerTime * 5);
		nTemp = 5;
	}
	else//60~69  wcq2012.03.07
	{
		count = (dataCount - countPerTime * 160) % (countPerTime * 6);
		nTemp = 6;
	}
	if (0 == count)//�պ�����
	{
		count += countPerTime * nTemp;
	}
	else if (count < countPerTime * nTemp * ZOOM_LAST_COEFF)//С��һ������������һ�ε�
	{
		count += countPerTime * nTemp;
	}
	return count;
}

/*
***************************************************************
��ڲ���:zoomNum:��ֱ�������Ŵ���,fStandPosDb:���ĵ�Dbֵ
���ڲ���:
����ʱ��:2011.09.26
Write By:wcq
����˵��:������ֱ��������ʱ���ֵ��Сֵ.����m_fStartDB,m_fMaxDB.�Ŵ���С����һ��
1~19:0.8dB��20~29:1.6dB��30~34:2dB��35~42:3dB��43~50:4dB,���98dB����С0.8dB
***************************************************************
*/
void COtdrTrace::CalVerticalRange(int zoomNum,float fStandPosDb)
{
	float fRange=0;
	float fCoeff=1.0;

	//δ�Ŵ�ԭʼ��Ĭ�ϣ���С,�����Ƿ�>ĳһ��׼dB(-30,ZOOM_VERTICAL_STANDARD)
	if (0 == zoomNum)//
	{
		if (IsLowerStandardAll())//
		{
			m_fStartDB = M_FSTARTDB0;
			m_fMaxDB = M_FMAXDB0;
		}
		else
		{
			m_fStartDB = M_FSTARTDB1;
			m_fMaxDB = M_FMAXDB1;
		}
		return;
	}
	int zoom = ZOOM_VERTICAL_COUNT - zoomNum;

	//���й��Ŵ�
	if (0 == zoom)//������С
	{
		fRange = ZOOM_VERTICAL_LEAST_RANGE;
	}
	else if (zoom <20)//1~19
	{
		fRange = zoom * ZOOM_VERTICAL_PER_TIME + ZOOM_VERTICAL_LEAST_RANGE;
	}
	else if (zoom <30)//20~29
	{
		fRange = 19 * ZOOM_VERTICAL_PER_TIME + ZOOM_VERTICAL_LEAST_RANGE + (zoom - 19) * 2 * ZOOM_VERTICAL_PER_TIME;
	}
	else if (zoom <40)//30~39
	{
		fRange = 19 * ZOOM_VERTICAL_PER_TIME + ZOOM_VERTICAL_LEAST_RANGE + 10 * 2 * ZOOM_VERTICAL_PER_TIME + (zoom - 29) * 3 * ZOOM_VERTICAL_PER_TIME;
	}
	else//40+
	{
		fRange = 19 * ZOOM_VERTICAL_PER_TIME + ZOOM_VERTICAL_LEAST_RANGE + 10 * 2 * ZOOM_VERTICAL_PER_TIME + 10 * 3 * ZOOM_VERTICAL_PER_TIME + (zoom - 39) * 4 * ZOOM_VERTICAL_PER_TIME;
	}

	//
	if (IsLowerStandardAll())//�ϵ�����
	{
		fCoeff = (fStandPosDb - M_FSTARTDB0) / (M_FMAXDB0 - M_FSTARTDB0);//
	}
	else
	{
		fCoeff = (fStandPosDb - M_FSTARTDB1)/ (M_FMAXDB1 - M_FSTARTDB1);//
	}
	//;//���±���
	float fTemp = fRange * fCoeff;//
	m_fStartDB = fStandPosDb - fTemp;
	m_fMaxDB = fStandPosDb + (fRange - fTemp);

	m_fCoeff = (m_fMaxDB - m_fStartDB) / (M_FMAXDB0 - M_FSTARTDB0);
}

/*
***************************************************************
��ڲ���:
���ڲ���:
����ʱ��:2011.09.26
Write By:wcq 
����˵��:�鲼�����������ݣ����Ƿ�������IsLowerStandardΪ��
***************************************************************
*/
BOOL COtdrTrace::IsLowerStandardAll()
{
	BOOL re=FALSE;
	//
	for(int nCount = 0; nCount < g_sorFileArray.totalCurveNumber; nCount++)
	{
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
		if (pCurve && pCurve->IsLowerStandard)
		{
			re = TRUE;
			break;
		}
	}
	return re;
}
//��֪���뷴�������±�
UINT COtdrTrace::Get_PosPoint(float distance,TestResultInfo* pData)
{
	return distance/(C_zll/(2*pData->Frequency * pData->Refraction * 1000000/*MHz*/));
}

//�������,��c/2nf��ʽ����
float COtdrTrace::Get_XCoord2nf(unsigned int xpoint,TestResultInfo* pData)
{
	return C_zll/(2*pData->Frequency * pData->Refraction * 1000000/*MHz*/) * xpoint;
}
/*
***************************************************************
��ڲ���:pData:��ǰ���ݽṹ��
���ڲ���:
����ʱ��:2011.09.26
Write By:wcq 
����˵��:����CCurveData�е�IsLowerStandard�������Է���������ֱ�������š�Ϊ��Լʱ�䣬ÿһˮƽǦ����ֻȡһ��
***************************************************************
*/
void COtdrTrace::SetLowerStandard(TestResultInfo* pData)
{
	int len=pData->DataPointsNum;
	int nStep=len / m_rectCoord.Width();
	pData->IsLowerStandard = FALSE;//���ж� >ZOOM_VERTICAL_STANDARD
	for (int i=0;i<len; i+=nStep)
	{
		if(pData->pDataPoints[i] > ZOOM_VERTICAL_STANDARD)//ԭʼ����Ҫ/-1000
		{
			pData->IsLowerStandard = TRUE;//ֻҪ��һ��С�ھ���
			break;
		}
	}
}
/*
***************************************************************
��������:             EventIsSelected                                                                              
����˵��:             �ƶ����ʱ�����˾���ĳ���¼��ľ���С�������¼���������¼���ѡ��                                              
��ڲ���:             PointEvent, �¼��б�˵ĵ���                                                                                     
���ڲ���:             ��                                                                                        
����ʱ��:             2011-07-06                                                              
Write By:             XJF                                                                                          
***************************************************************
*/
void COtdrTrace::EventIsSelected(int PointEvent)
{
	//int iPoint=0;
	//int RowNum; //iPointAΪ���Event�����ʵ�ʾ���,RowNum��ʾ�¼����е��¼���Ŀ
	//int i,j,k; //i,j����ѭ��
	//RowNum = m_lstEventList.GetItemCount();

	//////if (m_bISCurveFromNodeSelected)//wcq2012.03.09
	//////{
	//////	iPoint=Get_XCoord(PointEvent);
	//////}
	//////if (m_bISCurveFromFileSelected)
	//////{
	//////	iPoint=Get_XCoordForFile(PointEvent);
	//////}
	//for (i = 0;i< RowNum; i++)
	//{	
	//	for (k = 0;k < RowNum; k++)
	//	{
	//		if (k != i)
	//		{
	//			if (abs(_wtoi(m_lstEventList.GetItemText(i,2)) - iPoint) < abs(_wtoi(m_lstEventList.GetItemText(k,2))-iPoint)) //�ҳ�������������¼���i 
	//			{
	//				continue;
	//			}
	//			else
	//			{
	//				break;
	//			}
	//		}

	//		else
	//		{
	//			continue;
	//		}

	//	}
	//	if (k == RowNum)
	//	{
	//		for (j = 0;j < RowNum; j++)
	//		{	
	//			m_lstEventList.SetItemState(j, 0, LVIS_SELECTED|LVIS_FOCUSED); //����ѡ��״̬
	//		}
	//		m_lstEventList.SetItemState(i,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED); //��Ϊѡ��״̬
	//	}
	//}
}

//ÿ�յ�һ�������ݣ������������ڻ������ߵĵ�.wcq2011.11.21
void COtdrTrace::GetDataCurve(TestResultInfo* pCurveData)
{
	if (!pCurveData || pCurveData->DataPointsNum<NUM_VALIDE_LEAST)//û�л����ݺ���
		return;

	//�������
	int pointCount;//�����
	int dataPointNum = pCurveData->DataPointsNum;//ԭʼ���ݵ����
	if (dataPointNum < COUNT_POINT_MAX_MIN * 2)//�������٣�ֱ�Ӹ��ƣ��������ر���
	{
		//pointCount = dataPointNum;
		//�����ڴ�
		if (pCurveData->dataCurveCount < dataPointNum)//�ϴαȱ��ε�����٣����һ��(��ʱΪ0)
		{
			if (NULL!= pCurveData->pDataCurve)//����Ѵ��ڣ���ɾ��
			{
				delete[] pCurveData->pDataCurve;
				pCurveData->pDataCurve = NULL;
			}
			pCurveData->pDataCurve= new CPoint[dataPointNum];

#ifdef TRACE_FIND_MEMORY
			m_pMainDlg->SetStatusBarText(_T("OnDataReady GetDataCurve") );
#endif
		}
		//
		pCurveData->dataCurveCount = dataPointNum;
		CPoint* pData = pCurveData->pDataCurve;
		WORD* pWord = pCurveData->pDataPoints;
		int i=dataPointNum-1;
		while(i >= 0)//2011.12.01
		{
			pData[i].x = i;
			pData[i].y = pWord[i];
			i--;//2011.12.01
		}//ewhile
	}
	else//
	{//�ܹ�ȡCOUNT_POINT_MAX_MIN * 2���㣬���������Сֵ��x:������ԭ���������е��±꣬y:�����Ӧ�Ĺ���ֵ��ԭʼδ����1000��
		pointCount = COUNT_POINT_MAX_MIN;
		//�����ڴ�
		if (pCurveData->dataCurveCount < COUNT_POINT_MAX_MIN * 2)//�ϴαȱ��ε�����٣����һ��(��ʱΪ0)
		{
			if (NULL!= pCurveData->pDataCurve)//����Ѵ��ڣ���ɾ��
			{
				delete[] pCurveData->pDataCurve;
				pCurveData->pDataCurve = NULL;
			}
			pCurveData->pDataCurve= new CPoint[COUNT_POINT_MAX_MIN * 2];//2*:ȡ�����С�����㣻2:��βwcq2011.11.22

#ifdef TRACE_FIND_MEMORY
			m_pMainDlg->SetStatusBarText(_T("OnDataReady GetDataCurve else") );
#endif
		}//eif
		//
		pCurveData->dataCurveCount = COUNT_POINT_MAX_MIN * 2;//2*:ȡ�����С�����㣻wcq2011.11.22
		CPoint* pData = pCurveData->pDataCurve;
		WORD* pWord = pCurveData->pDataPoints;
		float fStepCoeff = dataPointNum * 1.0 / (COUNT_POINT_MAX_MIN);
		int lastStepPoint = 0;
		int step = 0;
		int maxIndex,minIndex;//ȡ��ʱĳһ���ض�Ӧ�������Сֵ������������±�.wcq2011.09.29
		int wMax,wMin;//ĳһ���ض�Ӧ�������Сֵ���ֵ
		int index = 0;//�±�
		for(int i=0; i< COUNT_POINT_MAX_MIN; i++)
		{
			maxIndex=lastStepPoint;//Ĭ��ÿһ���ص�һ��
			minIndex=lastStepPoint;//Ĭ��ÿһ���ص�һ��
			wMax=pWord[lastStepPoint];//Ĭ��ÿһ���ص�һ��
			wMin=pWord[lastStepPoint];//Ĭ��ÿһ���ص�һ��
			step =(i+1)*fStepCoeff;//���ᳬ���������??????
			//DEBUGMSG(true,(_T("step:%d\n"),step));
			for(int j=lastStepPoint+1;j<step;j++)
			{
				if (j == dataPointNum)//��ֹԽ��
				{
					break;
				}//eif
				if (pWord[j] > wMax)
				{
					wMax = pWord[j];
					maxIndex=j;
				}
				else if (pWord[j] < wMin)
				{
					wMin = pWord[j];
					minIndex=j;
				}//eif
				//DEBUGMSG(true,(_T("CSorDrawStatic::GetDataCurve dataPointNum:%d  i:%d  j:%d index:%d\n"),dataPointNum,i,j,i * 2 + 1));
			}//efor
			//˳��ŵ�����
			if (minIndex < maxIndex)
			{
				pData[index].x = minIndex;
				pData[index++].y = wMin;//++:ת����һ��
				pData[index].x = maxIndex;
				pData[index++].y = wMax;
			}
			else
			{
				pData[index].x = maxIndex;
				pData[index++].y = wMax;
				pData[index].x = minIndex;
				pData[index++].y = wMin;
			}//eif
			lastStepPoint= step;//���汾�ε����һ��
		}//efor
		//std::vector<CPoint> vp;
		//vp.assign(pData,pData+pointCount);
	}//eif
}

TestResultInfo* COtdrTrace::GetMaxLengthCurveData()//��ȡ��������������
{
	if (g_sorFileArray.totalCurveNumber == 0)//û������
		return NULL;

	return g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo;
}
//�õ���ǰ����Ӧ��ǰ�����±�
INT COtdrTrace::GetCurrentCursorIndex()
{
	if (0  == m_nABSele)
	{
		return 0;
	}//
	int nIndex = 0;
	if (OTDRTRACE_AB_SELE_A == m_nABSele)//�϶�A
	{
		nIndex = m_vAllData[m_nCurveIndex].DistanceToIndex(m_cursorA.m_fPos);
	}//eif
	else if(OTDRTRACE_AB_SELE_B == m_nABSele)//�϶�B
	{
		nIndex = m_vAllData[m_nCurveIndex].DistanceToIndex(m_cursorB.m_fPos);
	}//eif
	return nIndex;
}

/*
***************************************************************
��������:             CalculDispRect()                                                                               
����˵��:             ����������ʾ����XY����Ϣ��ʾ����������Ϣ��ʾ���ľ���                                            
��ڲ���:             bDisplayEventList,�Ƿ���Ҫ��ʾ�¼��б�                                                          
���ڲ���:             ��                                                                                             
����ʱ��:             2011-07-23                                                                
Write By:             XJF                                                                                           
***************************************************************
*/
void COtdrTrace::CalCulDispRect()
{
	//������ʾ��
	GetClientRect(m_rectCurve);
	//������ʾ��
	m_rectCoord.left=m_rectCurve.left;
	m_rectCoord.right=m_rectCurve.right;//wcq2012.02.14
	m_rectCoord.top=m_rectCurve.top;//wcq2011.12.09
	m_rectCoord.bottom=m_rectCurve.bottom;
}

CGraphElement* COtdrTrace::GetSeleCurve()
{
	TRACE(_T("%d\n"),m_nCurveIndex);
	return &m_vAllData[m_nCurveIndex];
}
//��ǰ�Ƿ�ѡ��AB�꣬ͬʱ�ƶ�AB��
BOOL COtdrTrace::IsCurrentTypeAB()
{
	return OTDRTRACE_AB_SELE_DOUBLE_AB == m_nABType;
}
//OTDRTRACE_AB_TYPE_SINGLE_A_OR_B��OTDRTRACE_AB_SELE_DOUBLE_AB
void COtdrTrace::SetCurrentTypeAB(INT nABType)
{
	m_nABType = nABType;
	Invalidate(FALSE);
}
//���¼���������룬��Ҫ�����ɾ��һ������
void COtdrTrace::ReCalMaxDistance()
{
	FLOAT fMax = 0;
	if (0 == m_vAllData.size())
	{
		return;
	}//eif

	for (int i=0; i<m_vAllData.size(); i++)
	{
		fMax = max(m_vAllData[i].m_fLength, fMax);
	}//efor
	m_vAllData[0].m_fMaxDistance = fMax;
	m_fMaxDistance = fMax;
}
//���������߻�ɾ�������ߣ�������������
void COtdrTrace::TraceCountChanged()
{
	if (m_vAllData.size()<m_nCurveIndex+1)
	{
		m_nCurveIndex=m_vAllData.size()-1;
		if (m_nCurveIndex<0)
		{
			m_nCurveIndex=0;
		}
	}
	
	if(m_vAllData.size() > 0)
	{
		//m_vAllData[m_nCurveIndex].m_bShowEvent = TRUE;//��ʾ�¼���Ϣlzy2014.9.9//cwcq2015.05.06
		m_vAllData[m_nCurveIndex].m_bShowEvent = m_bShowEventList;//wcq2015.05.06
		ReCalMaxDistance();//���¼���������룬��Ҫ�����ɾ��һ������
		SetRange(m_fRangeX[OTDR_TRACE_MIN], m_fRangeX[OTDR_TRACE_MAX], m_fRangeY[OTDR_TRACE_MIN], m_fRangeY[OTDR_TRACE_MAX]);
		OnInitAB();  //lzy2014.9.10
	}
	
}
//��Ϊ��ǰѡ������
void COtdrTrace::SetCurrentSeleTrace(INT nIndex)
{
	INT nCount = m_vAllData.size();
	
	if (nIndex >= nCount)
	{
		if (m_nCurveIndex>= nCount)
		{
			m_nCurveIndex=nCount-1;
		}
		return;
	}//eif
	m_nCurveIndex = nIndex;

	/////////////////////////zll 2013.7.1
	if (g_sorFileArray.waveConfig.MultiMode == FALSE)
	{
		for(int i=0;i<m_vAllData.size();i++)
		{
			if (m_nCurveIndex!=i)//zll 2013.7.1
			{
				SetShowCurve(i,FALSE);
			}
			else
			{
				SetShowCurve(i,TRUE);//zll 2013.7.4
			}
		}		
	}
	else
	{
		for(int i=0;i<m_vAllData.size();i++)
		{
			SetShowCurve(i,TRUE);
		}
	}
	///////////////////////////////////

	for (int i=0; i<nCount; i++)
	{
		if (nIndex == i)
		{
			if (m_bShowEventList)//wcq2015.05.06ԭ��û���ж�
			{
				m_vAllData[i].m_bShowEvent = TRUE;
				m_vAllData[i].bSelected = TRUE;
			}//eif
		}
		else
		{
			m_vAllData[i].m_bShowEvent = FALSE;
			m_vAllData[i].bSelected = FALSE;
		}//eif
	}//efor
	DrawCurves(&m_dcCurve,m_nCurveIndex);
	Invalidate(FALSE);
}
void COtdrTrace::SetShowCurve(int iNum,BOOL boolShow)
{
    if (m_vAllData.size()>iNum)
    {
		m_vAllData[iNum].m_bShow=boolShow;
    }    
}

void COtdrTrace:: SetCurveCursorData()
{
	g_cursorInfo.bDrawAB = m_bShowLineAB;

	g_cursorInfo.fRatioA = FLOAT2RATIO(m_cursorA.m_fPos);
	g_cursorInfo.strDistanceA = m_cursorA.m_strDistance;
	g_cursorInfo.strDBA = m_cursorA.m_strDB;
	g_cursorInfo.strA = _T("A");

	g_cursorInfo.fRatioB = FLOAT2RATIO(m_cursorB.m_fPos);
	g_cursorInfo.strDistanceB = m_cursorB.m_strDistance;
	g_cursorInfo.strDBB = m_cursorB.m_strDB;
	g_cursorInfo.strB = _T("B");
}

//��������Ҽ�����ѡ��˵���lzy2014.9.9
void COtdrTrace::OnContextMenu(CWnd* /*pWnd*/, CPoint point /*point*/)
{
	CMenu menu;
	menu.LoadMenu(IDR_MENU_TRACE);
	
	CMenu *pMenu;
	pMenu = menu.GetSubMenu(0);
	//UINT state = pMenu->GetMenuState(ID_AB_SELECT, MF_BYCOMMAND);
	//ASSERT(state != 0xFFFFFFFF);

	/*if (state & MF_CHECKED)
		pMenu->CheckMenuItem(ID_AB_SELECT, MF_UNCHECKED | MF_BYCOMMAND);
	else
		pMenu->CheckMenuItem(ID_AB_SELECT, MF_CHECKED | MF_BYCOMMAND);*/
	int iPreState;
	if(!m_bShowLineAB)
		iPreState = pMenu->CheckMenuItem(ID_AB_SELECT, MF_UNCHECKED | MF_BYCOMMAND);
	else
		iPreState = pMenu->CheckMenuItem(ID_AB_SELECT, MF_CHECKED | MF_BYCOMMAND);
	
	if(!m_bShowScale)
		iPreState = pMenu->CheckMenuItem(ID_TRACE_SCALE, MF_UNCHECKED | MF_BYCOMMAND);
	else
		iPreState = pMenu->CheckMenuItem(ID_TRACE_SCALE, MF_CHECKED | MF_BYCOMMAND);

	if(!m_bShowEventList)
	{
		iPreState = pMenu->CheckMenuItem(ID_EVENT_LIST_SHOW, MF_UNCHECKED | MF_BYCOMMAND);
	}
	else
	{
		iPreState = pMenu->CheckMenuItem(ID_EVENT_LIST_SHOW, MF_CHECKED | MF_BYCOMMAND);
	}//eif

	pMenu->TrackPopupMenu(TPM_LEFTALIGN , point.x, point.y, this, NULL);

	menu.DestroyMenu();
}

//�˵�����Ӧ��AB�����ʾ,lzy2014.9.9
void COtdrTrace::OnShowAB()
{	
	if(!m_bShowLineAB)
	{
		m_bShowLineAB = TRUE;
		OnInitAB();
		MAIN->m_pWaveFormView->ChangeABStatus(TRUE);//
	}
	else
	{
		m_bShowLineAB = FALSE;
		MAIN->m_pWaveFormView->ChangeABStatus(FALSE);//
	}
}

//�˵���Ӧ����ʾ���꣬lzy2014.9.9
void COtdrTrace::OnShowScale()
{
	if(!m_bShowScale)
		m_bShowScale = TRUE;
	else
		m_bShowScale = FALSE;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Access:    public 
// Returns:   void
// Comment(2015-5-6):
////////////////////////////////////////////////////////////////////////////////
void COtdrTrace::OnShowEventList()
{
	if(!m_bShowEventList)
	{
		m_bShowEventList = TRUE;
	}
	else
	{
		m_bShowEventList = FALSE;
	}//eif
	for (INT i=0; i<m_vAllData.size(); i++)
	{
		m_vAllData[i].m_bShowEvent = m_bShowEventList;
	}//efor
	DrawCurves(&m_dcCurve, -1);
}

//��ʼ��AB�꣬lzy2014.9.9
void COtdrTrace::OnInitAB(void)
{		
	if(!g_sorFileArray.totalCurveNumber)
		return;

	m_cursorA.m_nPos = m_rectCurve.left + m_rectCurve.Width() / 2 ;
	m_cursorA.m_fPos = POINT2FLOATX(m_cursorA.m_nPos);

	m_cursorB.m_nPos = m_rectCurve.left + m_rectCurve.Width() / 3;
	m_cursorB.m_fPos = POINT2FLOATX(m_cursorB.m_nPos);
}

//��ʾץȡAB���Сͼ��
void COtdrTrace::ShowCatch(CDC *pDC, CPoint pt)
{
	int n1 = 3;  
	int n2 = 3;
	int n3 = OTDR_TRACE_CURSOR_AB_VALID_SELE;  //��Χ
	int nOffset = 3;
	int nA = abs(m_cursorA.m_nPos - pt.x);
	int nB = abs(m_cursorB.m_nPos - pt.x);
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
	CPen *oldPen = pDC->SelectObject(&pen);

	
	switch(m_nCurABSele)
	{
	case OTDRTRACE_AB_SELE_A:
		pDC->MoveTo(m_cursorA.m_nPos - n1 - nOffset, pt.y - n2);
		pDC->LineTo(m_cursorA.m_nPos - n1 - n2 - nOffset, pt.y);
		pDC->LineTo(m_cursorA.m_nPos - n1 - nOffset, pt.y + n2);
		pDC->MoveTo(m_cursorA.m_nPos + n1, pt.y - n2);
		pDC->LineTo(m_cursorA.m_nPos + n1 + n2, pt.y);
		pDC->LineTo(m_cursorA.m_nPos + n1, pt.y + n2);
		break;

	case OTDRTRACE_AB_SELE_B:
		pDC->MoveTo(m_cursorB.m_nPos - n1 - nOffset, pt.y - n2);
		pDC->LineTo(m_cursorB.m_nPos - n1 - n2 - nOffset, pt.y);
		pDC->LineTo(m_cursorB.m_nPos - n1 - nOffset, pt.y + n2);
		pDC->MoveTo(m_cursorB.m_nPos + n1, pt.y - n2);
		pDC->LineTo(m_cursorB.m_nPos + n1 + n2, pt.y);
		pDC->LineTo(m_cursorB.m_nPos + n1, pt.y + n2);
		break;

	case OTDRTRACE_AB_SELE_AB:
	case OTDRTRACE_AB_SELE_NONE:		
		//TRACE(_T("ShowCatch() posA=%d, posB=%d, nA=%d, nB=%d\n"), m_cursorA.m_nPos, m_cursorB.m_nPos,nA, nB);////////////////////////////

		if(nA <= n3 || nB <= n3)
		{
			if(nA <= nB)
			{
				pDC->MoveTo(m_cursorA.m_nPos - n1 - nOffset, pt.y - n2);
				pDC->LineTo(m_cursorA.m_nPos - n1 - n2 - nOffset, pt.y);
				pDC->LineTo(m_cursorA.m_nPos - n1 - nOffset, pt.y + n2);
				pDC->MoveTo(m_cursorA.m_nPos + n1, pt.y - n2);
				pDC->LineTo(m_cursorA.m_nPos + n1 + n2, pt.y);
				pDC->LineTo(m_cursorA.m_nPos + n1, pt.y + n2);
			}
			else
			{
				pDC->MoveTo(m_cursorB.m_nPos - n1 - nOffset, pt.y - n2);
				pDC->LineTo(m_cursorB.m_nPos - n1 - n2 - nOffset, pt.y);
				pDC->LineTo(m_cursorB.m_nPos - n1 - nOffset, pt.y + n2);
				pDC->MoveTo(m_cursorB.m_nPos + n1, pt.y - n2);
				pDC->LineTo(m_cursorB.m_nPos + n1 + n2, pt.y);
				pDC->LineTo(m_cursorB.m_nPos + n1, pt.y + n2);
			}

		}
		break;
	

	}
	

	//TRACE(_T("m_cursorA.m_nPos=%d; mouse.x=%d \n"), m_cursorA.m_nPos, pt.x);

	pen.DeleteObject();
	pDC->SelectObject(oldPen);
}
