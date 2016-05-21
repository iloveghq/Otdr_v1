// PrintView.cpp : implementation file
//

#include "stdafx.h"

#include "PrintView.h"
#include "MainFrm.h"
#include <math.h>

extern LANG g_currentLang;
extern SorFileArray g_sorFileArray;

#define PRINTMARGIN 2
/////////////////////////////////////////////////////////////////////////////
// CPrintView

IMPLEMENT_DYNCREATE(CPrintView, CView)

CPrintView::CPrintView()
{
	m_bPrint = TRUE;
	m_bPrintCurve = TRUE;
	m_bPrintHead = TRUE;
}

CPrintView::~CPrintView()
{
}


BEGIN_MESSAGE_MAP(CPrintView, CView)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintView drawing

void CPrintView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CPrintView diagnostics

#ifdef _DEBUG
void CPrintView::AssertValid() const
{
	CView::AssertValid();
}

void CPrintView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPrintView message handlers
void CPrintView::OnMyPrint()
{
	//GetParent()->ShowWindow(SW_SHOWMINIMIZED);
	m_bPrint = TRUE;
	{
		CView::OnFilePrint();
	}

}
void CPrintView::OnMyPrintPreview()
{
	//GetParent()->ShowWindow(SW_SHOWMAXIMIZED);
	m_bPrint=FALSE;
	CView::OnFilePrintPreview();
}

BOOL CPrintView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	if(DoPreparePrinting(pInfo))
		return TRUE;
	else
	{
		return FALSE;
	}
}

void CPrintView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	CView::OnBeginPrinting(pDC, pInfo);

	if(m_fontPrinter.m_hObject==NULL)
		m_fontPrinter.CreatePointFont(120,_T("FixedSys"),pDC);
	if(m_dataFont.m_hObject==NULL)
		m_dataFont.CreatePointFont(80,_T("����"),pDC);
	TEXTMETRIC tm;
	CFont* pOldFont = pDC->SelectObject (&m_fontPrinter);
	pDC->GetTextMetrics (&tm);
	m_cyPrinter = tm.tmHeight + tm.tmExternalLeading;
	CSize size = pDC->GetTextExtent (_T ("---------1---------2---------3---------4---------5---------6---------7---------8---"), 81);
	pDC->SelectObject (pOldFont);
	m_cxWidth = size.cx;

	m_nLinesPerPage = (pDC->GetDeviceCaps (VERTRES) -
		(m_cyPrinter * (3 + (2 * PRINTMARGIN)))) / m_cyPrinter;

	if (1==MAIN->printType)  //zll 2012.10.29 printType=1ʱ����ӡ�����������������ļ�
	{
		int nCurveNumberPerPage=0;//��ӡÿҳ��ʾ��������
		int nCurveNumber = _wtoi(g_sorFileArray.reportConfig.WaveList);
		if (nCurveNumber == 0)
			nCurveNumberPerPage=1;
		else if (nCurveNumber == 1)
			nCurveNumberPerPage=2;
		else if (nCurveNumber == 2)
			nCurveNumberPerPage=4;
		else if (nCurveNumber == 3)
			nCurveNumberPerPage=6;
		else if (nCurveNumber == 4)
			nCurveNumberPerPage=8;
		int maxPage=g_sorFileArray.totalCurveNumber/nCurveNumberPerPage;//����ҳ��
		if (0!=g_sorFileArray.totalCurveNumber%nCurveNumberPerPage)//����������ҳ�����һҳ
		{
			maxPage+=1;
		}
		pInfo->SetMaxPage (maxPage);
	}
	else if (0==MAIN->printType) //zll 2012.10.29
	{
        pInfo->SetMaxPage(1);
	}

	m_cxOffset = (pDC->GetDeviceCaps (HORZRES) - size.cx) / 2;
	//���ô�ӡ����
	m_rectDraw.SetRect(0,0,pDC->GetDeviceCaps (HORZRES), pDC->GetDeviceCaps (VERTRES));
}

void CPrintView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	CView::OnEndPrinting(pDC, pInfo);

	if(m_fontPrinter.m_hObject!=NULL)
		m_fontPrinter.DeleteObject ();

	if(m_dataFont.m_hObject!=NULL)
		m_dataFont.DeleteObject();
}

void CPrintView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	TRACE(_T("%d\n"),pInfo->m_nCurPage);
	if(m_bPrintHead)
		PrintPageHeader(pDC,pInfo->m_nCurPage,  pInfo);
	InnerDraw(pDC,pInfo->m_nCurPage);
}

void CPrintView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView) 
{
	CView::OnEndPrintPreview(pDC, pInfo, point, pView);
	//�ָ��������
	//MAIN->ShowReport(TRUE);
	//��ӡ
	//OnMyPrint();//zll 2013.6.26

}

void CPrintView::PrintPageHeader(CDC* pDC, UINT nPageNumber, CPrintInfo* pInfo)
{
	CFont *pOldFont;
	CFont fontHeader;
	fontHeader.CreatePointFont(100,_T("FixedSys"),pDC);
	pOldFont=(CFont *)(pDC->SelectObject(&fontHeader));
	int nPosY,nPosX;
	nPosY=m_cyPrinter;
	nPosX=m_cxOffset;
	pDC->TextOut(nPosX,nPosY,CString(_T("")));
	//CString str;
	//str.Format(_T("�� %d ҳ / �� %d ҳ"),nPageNumber,pInfo->GetMaxPage());
	//CSize size=pDC->GetTextExtent(str);
	//nPosX=m_cxOffset+m_cxWidth-size.cx;
	//pDC->TextOut(nPosX,nPosY,str);
	//pDC->MoveTo(m_cxOffset,1.5*m_cyPrinter);
	//pDC->LineTo(m_cxOffset+m_cxWidth,1.5*m_cyPrinter);
	pDC->SelectObject(pOldFont);
	fontHeader.DeleteObject();
}

void CPrintView::DrawReport(CDC* pDC)
{
	int nSaveDC = pDC->SaveDC();
	CPen pen;
	pen.CreatePen (PS_SOLID, 1, RGB(0,0,0));
	pDC->SelectObject(&pen);//
	//����
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	int nHleftspace = m_rectDraw.Width() / 10;
	int nHmidspace = m_rectDraw.Width() / 20;
	int nLittleTableMaxRowCoord = 0;
	//����
	pDC->MoveTo(m_rectDraw.Width() / 3, nCurHeight);
	pDC->LineTo(m_rectDraw.Width() * 2/3, nCurHeight);
	CRect titleRect;
	titleRect.SetRect(m_rectDraw.Width() / 3, nCurHeight-nVspace, m_rectDraw.Width() * 2/3, nCurHeight);
	pDC->DrawText(g_sorFileArray.reportConfig.Title, titleRect, DT_CENTER);
	//����
	for (int i=1; i<=6; i++)
	{
		nCurHeight = nCurHeight + nVspace;
		pDC->MoveTo(nHleftspace, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace)/2 ,nCurHeight);
		
		CRect itemRect;
		itemRect.SetRect(nHleftspace, nCurHeight - nVspace,(m_rectDraw.Width() - nHmidspace)/2, nCurHeight);
		DrawReportItem(pDC, itemRect, i*2-1);

		pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
		itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight - nVspace,(m_rectDraw.Width() - nHleftspace), nCurHeight);
		DrawReportItem(pDC, itemRect, i*2);
	}
	//����
	{
		nCurHeight = nCurHeight + nVspace * 1.5;
		int nCurColCoord = nCurHeight + nVspace * 1.5;//��ǰ���������
		//���Ʊ�������
		DrawReportContent(pDC, nHleftspace, nCurHeight);
		//������
		for (int row=1; row<=30; row++)
		{
			nCurHeight = nCurHeight + nVspace * 1.5;
			pDC->MoveTo(nHleftspace, nCurHeight);
			pDC->LineTo(m_rectDraw.Width() - nHleftspace * 2, nCurHeight);
		}
		//�������
		int nCurRowCoord = nHleftspace;//��ǰ��������
		int nRowSpan = (m_rectDraw.Width() - nHleftspace * 3)/9;

		for (int col=1; col<=10; col++)
		{
			if (col > 1 &&
				col <= 9)
			{
				//int nListFormat = _wtoi(g_sorFileArray.reportConfig.ListFormat);
				//if (nListFormat == 1 ||
				//	nListFormat == 2)
				{
					pDC->MoveTo(nCurRowCoord, nCurColCoord + nVspace*1.5);
					pDC->LineTo(nCurRowCoord, nCurHeight);
				}
				//else
				//{
				//	pDC->MoveTo(nCurRowCoord, nCurColCoord);
				//	pDC->LineTo(nCurRowCoord, nCurHeight);
				//}
			}
			else
			{
				pDC->MoveTo(nCurRowCoord, nCurColCoord);
				pDC->LineTo(nCurRowCoord, nCurHeight);
			}

			if (col == 8)//�������
			{
				nCurRowCoord = nCurRowCoord + nRowSpan;
				nLittleTableMaxRowCoord = nCurRowCoord;
			}
			else if(col == 9)//���һ��
				nCurRowCoord = m_rectDraw.Width() - nHleftspace * 2;
			else
				nCurRowCoord = nCurRowCoord + nRowSpan;
		}
	}
	//С���
	{
		nCurHeight = nCurHeight + nVspace * 1.5;
		int nCurColCoord = nCurHeight + nVspace * 1.5;//��ǰ���������
		//������
		for (int row=1; row<=4; row++)
		{
			nCurHeight = nCurHeight + nVspace * 1.5;
			pDC->MoveTo(nHleftspace, nCurHeight);
			pDC->LineTo(nLittleTableMaxRowCoord, nCurHeight);
		}
		//�������
		int nCurRowCoord = nHleftspace;//��ǰ��������
		int nRowSpan = (m_rectDraw.Width() - nHleftspace * 3)/9;
		for (int col=1; col<=9; col++)
		{
			if (col == 2 ||
				col == 4 )//����Ҫ����
			{
				nCurRowCoord = nCurRowCoord + nRowSpan;
				continue;
			}
			pDC->MoveTo(nCurRowCoord, nCurColCoord);
			pDC->LineTo(nCurRowCoord, nCurHeight);
			nCurRowCoord = nCurRowCoord + nRowSpan;
		}
	}

	pDC->RestoreDC(nSaveDC);
	pen.DeleteObject();
}
/**********************************************************************************/
/* ���Ʊ�����                                                                     */
/* param nItemIndex ������������ 1-12                                            */
/*********************************************************************************/
void CPrintView::DrawReportItem(CDC* pDC, CRect& rect, int nItemIndex) //���Ʊ�����Ŀ
{
	CFont* pOldFont = pDC->SelectObject (&m_dataFont);
	CString strItemText;
	switch (nItemIndex)
	{
	case 1:
		{
			strItemText.Format(_T("%s:    %s"), g_sorFileArray.reportConfig.Item1, g_sorFileArray.reportConfig.Item1Val);
			break;
		}
	case 2:
		{
			strItemText.Format(_T("%s:    %s"), g_sorFileArray.reportConfig.Item2, g_sorFileArray.reportConfig.Item2Val);
			break;
		}
	case 3:
		{
			strItemText.Format(_T("%s:    %s"), g_sorFileArray.reportConfig.Item3, g_sorFileArray.reportConfig.Item3Val);
			break;
		}
	case 4:
		{
			strItemText.Format(_T("%s:    %s"), g_sorFileArray.reportConfig.Item4, g_sorFileArray.reportConfig.Item4Val);
			break;
		}
	case 5:
		{
			strItemText.Format(_T("%s:    %s"), g_sorFileArray.reportConfig.Item5, g_sorFileArray.reportConfig.Item5Val);
			break;
		}
	case 6:
		{
			strItemText.Format(_T("%s:    %s"), g_sorFileArray.reportConfig.Item6, g_sorFileArray.reportConfig.Item6Val);
			break;
		}
	case 7:
		{
			strItemText.Format(_T("%s:    %s"), g_sorFileArray.reportConfig.Item7, g_sorFileArray.reportConfig.Item7Val);
			break;
		}
	case 8:
		{
			strItemText.Format(_T("%s:    %s"), g_sorFileArray.reportConfig.Item8, g_sorFileArray.reportConfig.Item8Val);
			break;
		}
	case 9:
		{
			strItemText.Format(_T("%s:    %s"), g_sorFileArray.reportConfig.Item9, g_sorFileArray.reportConfig.Item9Val);
			break;
		}
	case 10:
		{
			strItemText.Format(_T("%s:    %s"), g_sorFileArray.reportConfig.Item10, g_sorFileArray.reportConfig.Item10Val);
			break;
		}
	case 11:
		{
			strItemText.Format(_T("%s:    %s"), g_sorFileArray.reportConfig.Item11, g_sorFileArray.reportConfig.Item11Val);
			break;
		}
	case 12:
		{
			strItemText.Format(_T("%s:    %s"), g_sorFileArray.reportConfig.Item12, g_sorFileArray.reportConfig.Item12Val);
			break;
		}
	}
	pDC->DrawText(strItemText, rect, DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
	//pDC->ExtTextOut(rect.left,rect.top + 80,ETO_CLIPPED,rect,strItemText,NULL);
	pDC->SelectObject (pOldFont);
}

/**********************************************************************************/
/* ����ÿ��������Ŀ                                                                     */
/* param nItemIndex ������Ŀ������ 1-3                                            */
/*********************************************************************************/
void CPrintView::DrawCurveItem(CDC* pDC, CRect& rect, int nItemIndex,int m_CurveNum,CString m_sorName) //����������Ŀ
{
	CFont* pOldFont = pDC->SelectObject (&m_dataFont);
	CString strItemText;
	CString str_m_CurveNum=_T("");
	str_m_CurveNum.Format(_T("%d"),m_CurveNum);
	switch (nItemIndex)
	{
	/*case 1:
		{
			strItemText.Format(_T("%s    %s"), g_sorFileArray.reportConfig.Step3Item1, g_sorFileArray.reportConfig.Step3Edit1);
			break;
		}*/ //zll 2012.9.27ע��
	case 2:
		{
			strItemText.Format(_T("%s:%s  %s"), g_sorFileArray.reportConfig.Step3Item2, str_m_CurveNum, m_sorName);
			break;
		}
	/*case 3:
		{
			strItemText.Format(_T("%s    %s"), g_sorFileArray.reportConfig.Step3Item3, g_sorFileArray.reportConfig.Step3Edit3);
			break;
		}*/ //zll 2012.9.27ע��
	}
	pDC->DrawText(strItemText, rect, DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
	//pDC->ExtTextOut(rect.left,rect.top + 80,ETO_CLIPPED,rect,strItemText,NULL);
	pDC->SelectObject (pOldFont);
}

/**********************************************************************************/
/* ���Ʊ�������                                                                     */
/* param                                            */
/*********************************************************************************/
void CPrintView::DrawReportContent(CDC* pDC, int nLeft, int nTop) //���Ʊ�������
{
	CFont* pOldFont = pDC->SelectObject (&m_dataFont);
	int nVspace = m_rectDraw.Height() / 42;
	int nHspace = m_rectDraw.Width() / 10;
	int nStartLine = 1;

	CString strText;
	CRect textRect;

	// �������
	{
		textRect.left = nLeft + 3 * nHspace;
		textRect.top = nTop + nStartLine * nVspace;
		textRect.right = nLeft + 8 * nHspace;
		textRect.bottom = nTop + (nStartLine+1) * nVspace;
		int nListFormat = _wtoi(g_sorFileArray.reportConfig.ListFormat);

		switch (nListFormat)
		{
		case 0:
			strText = _T("���");
			break;
		case 1:
			strText = _T("��ļ��ز����");
			break;
		case 2:
			strText = _T("��ļ�����");
			break;
		case 3:
			strText = _T("");
			break;
		}
		pDC->ExtTextOut(textRect.left,textRect.top + 80,ETO_CLIPPED,textRect,strText,NULL);
		//pDC->DrawText(strText, &textRect,DT_CENTER|DT_BOTTOM);//����
	}
	
	pDC->SelectObject (pOldFont);
}

/**********************************************************************************/
/* ���Ʋ��κ������ı�                                                             */
/* param rect ���ƾ�������                                                       */
/*********************************************************************************/
void CPrintView::DrawHorCoordUnit(CDC* pDC, CRect& rect, CString strText) //���Ʋ��κ������ı�
{
	CFont* pOldFont = pDC->SelectObject (&m_dataFont);
	rect.OffsetRect(0, 50);
	pDC->DrawText(strText, rect, DT_CENTER | DT_BOTTOM);
	pDC->SelectObject (pOldFont);
}

/**********************************************************************************/
/* ���Ʋ����������ı�                                                             */
/* param rect ���ƾ�������                                                       */
/*********************************************************************************/
void CPrintView::DrawVerCoordUnit(CDC* pDC, CRect& rect) //���Ʋ��ݺ������ı�
{
	CFont* pOldFont = pDC->SelectObject (&m_dataFont);
	CString strUnitText = MAIN->m_pWaveFormView->m_strUnitDiv;
	rect.OffsetRect(0, 50);
	pDC->DrawText(strUnitText, rect, DT_CENTER | DT_BOTTOM);
	pDC->SelectObject (pOldFont);
}


void CPrintView::Draw1Curve(CDC* pDC,int m_nCurPage)
{
	int m_CurveNum=m_nCurPage-1;
	//����
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	int nHleftspace = m_rectDraw.Width() / 10;
	int nHmidspace = m_rectDraw.Width() / 20;
	//����
	pDC->MoveTo(m_rectDraw.Width() / 3, nCurHeight);
	pDC->LineTo(m_rectDraw.Width() * 2/3, nCurHeight);
	CRect titleRect;
	titleRect.SetRect(m_rectDraw.Width() / 3, nCurHeight-nVspace, m_rectDraw.Width() * 2/3, nCurHeight);
	pDC->DrawText(g_sorFileArray.reportConfig.Title, titleRect, DT_CENTER);
	//����
	for (int i=1; i<=6; i++)
	{
		nCurHeight = nCurHeight + nVspace;
		pDC->MoveTo(nHleftspace, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace)/2 ,nCurHeight);

		CRect itemRect;
		itemRect.SetRect(nHleftspace, nCurHeight - nVspace,(m_rectDraw.Width() - nHmidspace)/2, nCurHeight);
		DrawReportItem(pDC, itemRect, i*2-1);

		pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
		itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight - nVspace,(m_rectDraw.Width() - nHleftspace), nCurHeight);
		DrawReportItem(pDC, itemRect, i*2);
	}
	//�ָ��ߣ����߱���
	{
		nCurHeight = nCurHeight + nVspace * 3;  //Item1
		pDC->MoveTo(nHleftspace, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace)/2 ,nCurHeight);
		CRect itemRect;
		itemRect.SetRect(nHleftspace, nCurHeight - nVspace,(m_rectDraw.Width() - nHmidspace)/2, nCurHeight);
		DrawCurveItem(pDC, itemRect, 2,m_nCurPage,GetSorName(m_nCurPage));//  zll  2012.9.27 �޸� ֻ��ʾо��

        //SetSorName(m_nCurPage,m_CurveNum+1,sorName);//����.sor�����ļ���

		//  zll  2012.9.27 ע��
		//pDC->MoveTo((m_rectDraw.Width()+ nHmidspace)/3, nCurHeight);  //Item2
		//pDC->LineTo((m_rectDraw.Width() - nHmidspace) *2/3 , nCurHeight);
		//itemRect.SetRect((m_rectDraw.Width()+ nHmidspace)/3, nCurHeight - nVspace,(m_rectDraw.Width() - nHmidspace) *2/3, nCurHeight);
		//DrawCurveItem(pDC, itemRect, 2);

		//pDC->MoveTo(m_rectDraw.Width()* 2/3, nCurHeight);  //Item3
		//pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
		//itemRect.SetRect(m_rectDraw.Width()* 2/3, nCurHeight - nVspace,m_rectDraw.Width() - nHleftspace, nCurHeight);
		//DrawCurveItem(pDC, itemRect, 3);
	}
	//����
	{
		//�����
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, m_rectDraw.Width() - nHleftspace, nCurHeight + nVspace * 20);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//����դ�� wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurve(pDC, curveRect,m_nCurPage-1);
		//���С����
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-10*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-8*nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//�ײ�С����
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+3*nHleftspace, curveRect.bottom +nVspace*3);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //���ƺ������ı����

		CRect mideUnitRect;
		mideUnitRect.SetRect(bottomRect.left-nHleftspace, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //���ƺ������ı���λ����
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //���ƺ������ı���λ����//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //���ƺ������ı���λ����//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -400-nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		///DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //���ƺ������ı�
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //���ƺ������ı�//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//���ƺ������ı� //�������ұ���󳤶� cwcq2012.11.02

	}
	//�¼��б�
	{
		nCurHeight = nCurHeight + nVspace * 27;
		int nCurLeft = 0;
	/*	for (int nLine=0; nLine<=12;nLine++)
		{
			nCurLeft = nHleftspace;
			for (int nRow=0; nRow<8;nRow++)
			{
				pDC->MoveTo(nCurLeft, nCurHeight);
				pDC->LineTo(nCurLeft+nHleftspace/1.1, nCurHeight);
				nCurLeft = nCurLeft + nHleftspace;
			}
			nCurHeight = nCurHeight + nVspace * 1.5;
		}*/
		//�����¼��б��ͷ
		DrawEventHead(pDC, nHleftspace, nCurHeight);
		//�����¼���
		DrawEventBody1(pDC, nHleftspace, nCurHeight,m_nCurPage);
	}
}

void CPrintView::Draw2Curve(CDC* pDC,int m_nCurPage)
{
	//����
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	int nHleftspace = m_rectDraw.Width() / 10;
	int nHmidspace = m_rectDraw.Width() / 20;
	//����
	pDC->MoveTo(m_rectDraw.Width() / 3, nCurHeight);
	pDC->LineTo(m_rectDraw.Width() * 2/3, nCurHeight);
	CRect titleRect;
	titleRect.SetRect(m_rectDraw.Width() / 3, nCurHeight-nVspace, m_rectDraw.Width() * 2/3, nCurHeight);
	pDC->DrawText(g_sorFileArray.reportConfig.Title, titleRect, DT_CENTER);
	//����
	for (int i=1; i<=6; i++)
	{
		nCurHeight = nCurHeight + nVspace;
		pDC->MoveTo(nHleftspace, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace)/2 ,nCurHeight);

		CRect itemRect;
		itemRect.SetRect(nHleftspace, nCurHeight - nVspace,(m_rectDraw.Width() - nHmidspace)/2, nCurHeight);
		DrawReportItem(pDC, itemRect, i*2-1);

		pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
		itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight - nVspace,m_rectDraw.Width() - nHleftspace, nCurHeight);
		DrawReportItem(pDC, itemRect, i*2);
	}
	//�ָ���:���߱���
	int m_CurveNum=2*(m_nCurPage-1);//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 3;
		pDC->MoveTo(nHleftspace, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace)/2 ,nCurHeight);
		CRect itemRect;
		itemRect.SetRect(nHleftspace, nCurHeight - nVspace,(m_rectDraw.Width() - nHmidspace)/2, nCurHeight);
		DrawCurveItem(pDC, itemRect, 2,m_CurveNum+1,GetSorName(m_CurveNum+1));//  zll  2012.9.27 �޸� ֻ��ʾо��

		//  zll  2012.9.27 ע��
		/*pDC->MoveTo((m_rectDraw.Width()+ nHmidspace)/3, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace) *2/3 , nCurHeight);
		itemRect.SetRect((m_rectDraw.Width()+ nHmidspace)/3, nCurHeight - nVspace,(m_rectDraw.Width() - nHmidspace) *2/3, nCurHeight);
		DrawCurveItem(pDC, itemRect, 2);

		pDC->MoveTo(m_rectDraw.Width()* 2/3, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
		itemRect.SetRect(m_rectDraw.Width()* 2/3, nCurHeight - nVspace,m_rectDraw.Width() - nHleftspace, nCurHeight);
		DrawCurveItem(pDC, itemRect, 3);*/
	}
	//����1
	m_CurveNum=2*(m_nCurPage-1);//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//�����
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight - nVspace, m_rectDraw.Width() - nHleftspace, nCurHeight + nVspace * 20);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//����դ�� wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//��������1
		//���С����
		CRect leftRect;
		leftRect.SetRect(nHleftspace, nCurHeight - nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-nVspace*17/2);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//�ײ�С����
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+3*nHleftspace, curveRect.bottom +nVspace*3);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //���ƺ������ı����

		CRect mideUnitRect;
		mideUnitRect.SetRect(bottomRect.left-nHleftspace, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //���ƺ������ı���λ����//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //���ƺ������ı���λ����//wcq2012.11.02
		
		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -400-nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //���ƺ������ı�//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//���ƺ������ı� //�������ұ���󳤶� cwcq2012.11.02

	}
	//����2�ָ���
	m_CurveNum=2*(m_nCurPage-1)+1;//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 23;
		pDC->MoveTo(nHleftspace, nCurHeight+ nVspace);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace)/2 ,nCurHeight+ nVspace);
		CRect itemRect;
		itemRect.SetRect(nHleftspace, nCurHeight,(m_rectDraw.Width() - nHmidspace)/2, nCurHeight+ nVspace);
		DrawCurveItem(pDC, itemRect, 2,m_CurveNum+1,GetSorName(m_CurveNum+1));//  zll  2012.9.27 �޸� ֻ��ʾо��

		//  zll  2012.9.27 ע��
		/*pDC->MoveTo((m_rectDraw.Width()+ nHmidspace)/3, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace) *2/3 , nCurHeight);
		itemRect.SetRect((m_rectDraw.Width()+ nHmidspace)/3, nCurHeight - nVspace,(m_rectDraw.Width() - nHmidspace) *2/3, nCurHeight);
		DrawCurveItem(pDC, itemRect, 2);

		pDC->MoveTo(m_rectDraw.Width()* 2/3, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
		itemRect.SetRect(m_rectDraw.Width()* 2/3, nCurHeight - nVspace,m_rectDraw.Width() - nHleftspace, nCurHeight);
		DrawCurveItem(pDC, itemRect, 3);*/
	}
	//����2
	m_CurveNum=2*(m_nCurPage-1)+1;//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//����2�����
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, m_rectDraw.Width() - nHleftspace, nCurHeight + nVspace * 20);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//����դ�� wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//��������2
		//����2���С����
		CRect leftRect;
		leftRect.SetRect(nHleftspace, nCurHeight, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-nVspace*8);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//����2�ײ�С����
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+3*nHleftspace, curveRect.bottom +nVspace*3);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //���ƺ������ı����

		CRect mideUnitRect;
		mideUnitRect.SetRect(bottomRect.left-nHleftspace, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //���ƺ������ı���λ����//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //���ƺ������ı���λ����//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -400-nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //���ƺ������ı�//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//���ƺ������ı� //�������ұ���󳤶� cwcq2012.11.02

	}
}


void CPrintView::Draw4Curve(CDC* pDC,int m_nCurPage)
{
	//����
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	int nHleftspace = m_rectDraw.Width() / 10;
	int nHmidspace = m_rectDraw.Width() / 20;
	int nSeparateSpace = (m_rectDraw.Width() - 2 * (nHleftspace + nHmidspace)) / 6;
	int nCurSeparate = nHleftspace;
	//����
	pDC->MoveTo(m_rectDraw.Width() / 3, nCurHeight);
	pDC->LineTo(m_rectDraw.Width() * 2/3, nCurHeight);
	CRect titleRect;
	titleRect.SetRect(m_rectDraw.Width() / 3, nCurHeight-nVspace, m_rectDraw.Width() * 2/3, nCurHeight);
	pDC->DrawText(g_sorFileArray.reportConfig.Title, titleRect, DT_CENTER);
	//����
	for (int i=1; i<=6; i++)
	{
		nCurHeight = nCurHeight + nVspace;
		pDC->MoveTo(nHleftspace, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace)/2 ,nCurHeight);
		CRect itemRect;
		itemRect.SetRect(nHleftspace, nCurHeight - nVspace,(m_rectDraw.Width() - nHmidspace)/2, nCurHeight);
		DrawReportItem(pDC, itemRect, i*2-1);

		pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
		itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight - nVspace,m_rectDraw.Width() - nHleftspace, nCurHeight);
		DrawReportItem(pDC, itemRect, i*2);
	}
	//����12�ָ��� zll 2012.9.20
	int m_CurveNum=4*(m_nCurPage-1)+1;//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 3;
		for (int i=1; i<=6; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
			else if (5==i)
			{
				pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
				pDC->LineTo((m_rectDraw.Width() + nHmidspace)/2 + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight- nVspace, nCurSeparate*3 + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum+1,GetSorName(m_CurveNum+1));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	else if (g_sorFileArray.totalCurveNumber==m_CurveNum)//����1�ָ���  zll 2012.9.20
	{
		nCurHeight = nCurHeight + nVspace * 3;
		for (int i=1; i<=3; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}

	//����1
	m_CurveNum=4*(m_nCurPage-1);//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//����1�����
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2, nCurHeight, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//����դ�� wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//��������1
		//���С����
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//�ײ�С����
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //���ƺ������ı����

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //���ƺ������ı���λ����//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //���ƺ������ı���λ����//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //���ƺ������ı�//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//���ƺ������ı� //�������ұ���󳤶� cwcq2012.11.02

	}
	//����2
	m_CurveNum=4*(m_nCurPage-1)+1;//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//����2�����
		CRect curveRect;
		curveRect.SetRect((m_rectDraw.Width() + nHmidspace)/2 + nHleftspace, nCurHeight, m_rectDraw.Width() - nHleftspace*6/5, nCurHeight + nVspace * 10);
		pDC->Rectangle(&curveRect);
		//���С����
		CRect leftRect;
		leftRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		pDC->Rectangle(&leftRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//����դ�� wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//��������2
		DrawVerCoordUnit(pDC, leftRect);
		//�ײ�С����
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //���ƺ������ı����

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //���ƺ������ı���λ����//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //���ƺ������ı���λ����//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //���ƺ������ı�//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//���ƺ������ı� //�������ұ���󳤶� cwcq2012.11.02

	}
	//����34�ָ���
	m_CurveNum=4*(m_nCurPage-1)+3;//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 27;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=6; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight-4*nVspace);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight-4*nVspace);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- 5*nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight-4*nVspace);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
			else if (5==i)
			{
				pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight-4*nVspace);
				pDC->LineTo((m_rectDraw.Width() + nHmidspace)/2 + nSeparateSpace*5/2, nCurHeight-4*nVspace);
				CRect itemRect;
				itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight- 5*nVspace, nCurSeparate*3 + 3*nSeparateSpace, nCurHeight-4*nVspace);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum+1,GetSorName(m_CurveNum+1));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	else if (g_sorFileArray.totalCurveNumber==m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 27;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=3; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight-4*nVspace);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight-4*nVspace);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- 5*nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight-4*nVspace);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	//����3
	m_CurveNum=4*(m_nCurPage-1)+2;//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//����3�����
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2, nCurHeight-4*nVspace, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 6);
		//curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//����դ�� wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//��������3
		//���С����
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		//leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//�ײ�С����
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //���ƺ������ı����

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //���ƺ������ı���λ����//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //���ƺ������ı���λ����//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //���ƺ������ı�//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//���ƺ������ı� //�������ұ���󳤶� cwcq2012.11.02

	}
	//����4
	m_CurveNum=4*(m_nCurPage-1)+3;//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//����4�����
		CRect curveRect;
		curveRect.SetRect((m_rectDraw.Width() + nHmidspace)/2 + nHleftspace, nCurHeight-4*nVspace, m_rectDraw.Width() - nHleftspace*6/5, nCurHeight + nVspace * 6);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//����դ�� wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//��������4
		//���С����
		CRect leftRect;
		leftRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//�ײ�С����
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //���ƺ������ı����

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //���ƺ������ı���λ����//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //���ƺ������ı���λ����//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //���ƺ������ı�//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//���ƺ������ı� //�������ұ���󳤶� cwcq2012.11.02

	}

}

void CPrintView::Draw6Curve(CDC* pDC,int m_nCurPage)
{
	//����
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	int nHleftspace = m_rectDraw.Width() / 10;
	int nHmidspace = m_rectDraw.Width() / 20;
	int nSeparateSpace = (m_rectDraw.Width() - 2 * (nHleftspace + nHmidspace)) / 6;
	int nCurSeparate = nHleftspace;
	//����
	pDC->MoveTo(m_rectDraw.Width() / 3, nCurHeight);
	pDC->LineTo(m_rectDraw.Width() * 2/3, nCurHeight);
	CRect titleRect;
	titleRect.SetRect(m_rectDraw.Width() / 3, nCurHeight-nVspace, m_rectDraw.Width() * 2/3, nCurHeight);
	pDC->DrawText(g_sorFileArray.reportConfig.Title, titleRect, DT_CENTER);
	//����
	for (int i=1; i<=6; i++)
	{
		nCurHeight = nCurHeight + nVspace;
		pDC->MoveTo(nHleftspace, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace)/2 ,nCurHeight);

		CRect itemRect;
		itemRect.SetRect(nHleftspace, nCurHeight - nVspace,(m_rectDraw.Width() - nHmidspace)/2, nCurHeight);
		DrawReportItem(pDC, itemRect, i*2-1);

		pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
		itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight - nVspace,m_rectDraw.Width() - nHleftspace, nCurHeight);
		DrawReportItem(pDC, itemRect, i*2);
	}
	//����12�ָ���
	int m_CurveNum=6*(m_nCurPage-1)+1;//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 3;
		for (int i=1; i<=6; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
			else if (5==i)
			{
				pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
				pDC->LineTo((m_rectDraw.Width() + nHmidspace)/2 + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight- nVspace, nCurSeparate*3 + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum+1,GetSorName(m_CurveNum+1));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	else if (g_sorFileArray.totalCurveNumber==m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 3;
		for (int i=1; i<=6; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	//����1
	m_CurveNum=6*(m_nCurPage-1);//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//����1�����
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2, nCurHeight-nVspace, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 9);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//����դ�� wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//��������1
		//���С����
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		//leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//�ײ�С����
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //���ƺ������ı����

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //���ƺ������ı���λ����//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //���ƺ������ı���λ����//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //���ƺ������ı�//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//���ƺ������ı� //�������ұ���󳤶� cwcq2012.11.02

	}
	//����2
	m_CurveNum=6*(m_nCurPage-1)+1;//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//����2�����
		CRect curveRect;
		curveRect.SetRect((m_rectDraw.Width() + nHmidspace)/2 + nHleftspace, nCurHeight-nVspace, m_rectDraw.Width() - nHleftspace*6/5, nCurHeight + nVspace * 9);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//����դ�� wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//��������2
		//���С����
		CRect leftRect;
		leftRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//�ײ�С����
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //���ƺ������ı����

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //���ƺ������ı���λ����//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //���ƺ������ı���λ����//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //���ƺ������ı�//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//���ƺ������ı� //�������ұ���󳤶� cwcq2012.11.02

	}
	//����34�ָ���
	m_CurveNum=6*(m_nCurPage-1)+3;//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 17;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=6; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight-2*nVspace);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight-2*nVspace);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- 3*nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight-2*nVspace);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
			else if (5==i)
			{
				pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight-2*nVspace);
				pDC->LineTo((m_rectDraw.Width() + nHmidspace)/2 + nSeparateSpace*5/2, nCurHeight-2*nVspace);
				CRect itemRect;
				itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight- 3*nVspace, nCurSeparate*3 + 3*nSeparateSpace, nCurHeight-2*nVspace);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum+1,GetSorName(m_CurveNum+1));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	else if (g_sorFileArray.totalCurveNumber==m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 17;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=3; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight-2*nVspace);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight-2*nVspace);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- 3*nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight-2*nVspace);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	//����3
	m_CurveNum=6*(m_nCurPage-1)+2;//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//����3�����
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2, nCurHeight-3*nVspace, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 7);
		//curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//����դ�� wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum); //��������3
		//���С����
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		//leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//�ײ�С����
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //���ƺ������ı����

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //���ƺ������ı���λ����//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //���ƺ������ı���λ����//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //���ƺ������ı�//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//���ƺ������ı� //�������ұ���󳤶� cwcq2012.11.02

	}
	//����4
	m_CurveNum=6*(m_nCurPage-1)+3;//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//����4�����
		CRect curveRect;
		curveRect.SetRect((m_rectDraw.Width() + nHmidspace)/2 + nHleftspace, nCurHeight-3*nVspace, m_rectDraw.Width() - nHleftspace*6/5, nCurHeight + nVspace * 7);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//����դ�� wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum); //��������4
		//���С����
		CRect leftRect;
		leftRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//�ײ�С����
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //���ƺ������ı����

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //���ƺ������ı���λ����//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //���ƺ������ı���λ����//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //���ƺ������ı�//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//���ƺ������ı� //�������ұ���󳤶� cwcq2012.11.02

	}
	//����56�ָ���
	m_CurveNum=6*(m_nCurPage-1)+5;//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 17;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=6; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight-4*nVspace);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight-4*nVspace);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- 5*nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight-4*nVspace);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
			else if (5==i)
			{
				pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight-4*nVspace);
				pDC->LineTo((m_rectDraw.Width() + nHmidspace)/2 + nSeparateSpace*5/2, nCurHeight-4*nVspace);
				CRect itemRect;
				itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight- 5*nVspace, nCurSeparate*3 + 3*nSeparateSpace, nCurHeight-4*nVspace);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum+1,GetSorName(m_CurveNum+1));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	else if (g_sorFileArray.totalCurveNumber==m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 17;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=3; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight-4*nVspace);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight-4*nVspace);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- 5*nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight-4*nVspace);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	//����5
	m_CurveNum=6*(m_nCurPage-1)+4;//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//����5�����
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2, nCurHeight-5*nVspace, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 5);
		//curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//����դ�� wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//��������5
		//���С����
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		//leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//�ײ�С����
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //���ƺ������ı����

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //���ƺ������ı���λ����//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //���ƺ������ı���λ����//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //���ƺ������ı�//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//���ƺ������ı� //�������ұ���󳤶� cwcq2012.11.02

	}
	//����6
	m_CurveNum=6*(m_nCurPage-1)+5;//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//����6�����
		CRect curveRect;
		curveRect.SetRect((m_rectDraw.Width() + nHmidspace)/2 + nHleftspace, nCurHeight-5*nVspace, m_rectDraw.Width() - nHleftspace*6/5, nCurHeight + nVspace * 5);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//����դ�� wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//��������6
		//���С����
		CRect leftRect;
		leftRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//�ײ�С����
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //���ƺ������ı����

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //���ƺ������ı���λ����//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //���ƺ������ı���λ����//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //���ƺ������ı�//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//���ƺ������ı� //�������ұ���󳤶� cwcq2012.11.02

	}
}

void CPrintView::Draw8Curve(CDC* pDC,int m_nCurPage)
{
	//����
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	int nHleftspace = m_rectDraw.Width() / 10;
	int nHmidspace = m_rectDraw.Width() / 20;
	int nSeparateSpace = (m_rectDraw.Width() - 2 * (nHleftspace + nHmidspace)) / 6;
	int nCurSeparate = nHleftspace;

	//����
	pDC->MoveTo(m_rectDraw.Width() / 3, nCurHeight);
	pDC->LineTo(m_rectDraw.Width() * 2/3, nCurHeight);
	CRect titleRect;
	titleRect.SetRect(m_rectDraw.Width() / 3, nCurHeight-nVspace, m_rectDraw.Width() * 2/3, nCurHeight);
	pDC->DrawText(g_sorFileArray.reportConfig.Title, titleRect, DT_CENTER);
	//����12�ָ���
	int m_CurveNum=8*(m_nCurPage-1)+1;//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 2;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=6; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
			else if (5==i)
			{
				pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
				pDC->LineTo((m_rectDraw.Width() + nHmidspace)/2 + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight- nVspace, nCurSeparate*3 + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum+1,GetSorName(m_CurveNum+1));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	else if (g_sorFileArray.totalCurveNumber==m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 2;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=3; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}

	}
	//����1
	m_CurveNum=8*(m_nCurPage-1);//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//����1�����
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2, nCurHeight-nVspace, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10-nVspace);
		//curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//����դ�� wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//��������1
		//���С����
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		//leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//�ײ�С����
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //���ƺ������ı����

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //���ƺ������ı���λ����//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //���ƺ������ı���λ����//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //���ƺ������ı�//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//���ƺ������ı� //�������ұ���󳤶� cwcq2012.11.02

	}
	//����2
	m_CurveNum=8*(m_nCurPage-1)+1;//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//����2�����
		CRect curveRect;
		curveRect.SetRect((m_rectDraw.Width() + nHmidspace)/2 + nHleftspace, nCurHeight-nVspace, m_rectDraw.Width() - nHleftspace*6/5, nCurHeight + nVspace * 10-nVspace);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//����դ�� wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//��������2
		//���С����
		CRect leftRect;
		leftRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//�ײ�С����
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //���ƺ������ı����

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //���ƺ������ı���λ����//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //���ƺ������ı���λ����//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //���ƺ������ı�//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//���ƺ������ı� //�������ұ���󳤶� cwcq2012.11.02

	}
	//����34�ָ���
	m_CurveNum=8*(m_nCurPage-1)+3;//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 13;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=6; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
			else if (5==i)
			{
				pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
				pDC->LineTo((m_rectDraw.Width() + nHmidspace)/2 + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight- nVspace, nCurSeparate*3 + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum+1,GetSorName(m_CurveNum+1));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	else if (g_sorFileArray.totalCurveNumber==m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 13;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=3; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	//����3
	m_CurveNum=8*(m_nCurPage-1)+2;//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//����3�����
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2, nCurHeight-nVspace, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10-nVspace);
		//curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//����դ�� wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum); //��������3
		//���С����
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		//leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//�ײ�С����
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //���ƺ������ı����

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //���ƺ������ı���λ����//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //���ƺ������ı���λ����//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //���ƺ������ı�//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//���ƺ������ı� //�������ұ���󳤶� cwcq2012.11.02

	}
	//����4
	m_CurveNum=8*(m_nCurPage-1)+3;//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//����4�����
		CRect curveRect;
		curveRect.SetRect((m_rectDraw.Width() + nHmidspace)/2 + nHleftspace, nCurHeight-nVspace, m_rectDraw.Width() - nHleftspace*6/5, nCurHeight + nVspace * 10-nVspace);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//����դ�� wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum); //��������4
		//���С����
		CRect leftRect;
		leftRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//�ײ�С����
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //���ƺ������ı����

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //���ƺ������ı���λ����//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //���ƺ������ı���λ����//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //���ƺ������ı�//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//���ƺ������ı� //�������ұ���󳤶� cwcq2012.11.02

	}
	//����56�ָ���
	m_CurveNum=8*(m_nCurPage-1)+5;//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 13;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=6; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
			else if (5==i)
			{
				pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
				pDC->LineTo((m_rectDraw.Width() + nHmidspace)/2 + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight- nVspace, nCurSeparate*3 + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum+1,GetSorName(m_CurveNum+1));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	else if (g_sorFileArray.totalCurveNumber==m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 13;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=3; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	//����5
	m_CurveNum=8*(m_nCurPage-1)+4;//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//����5�����
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2, nCurHeight-nVspace, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10-nVspace);
		//curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//����դ�� wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//��������5
		//���С����
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		//leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//�ײ�С����
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //���ƺ������ı����

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //���ƺ������ı���λ����//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //���ƺ������ı���λ����//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //���ƺ������ı�//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//���ƺ������ı� //�������ұ���󳤶� cwcq2012.11.02

	}
	//����6
	m_CurveNum=8*(m_nCurPage-1)+5;//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//����6�����
		CRect curveRect;
		curveRect.SetRect((m_rectDraw.Width() + nHmidspace)/2 + nHleftspace, nCurHeight-nVspace, m_rectDraw.Width() - nHleftspace*6/5, nCurHeight + nVspace * 10-nVspace);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//����դ�� wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//��������6
		//���С����
		CRect leftRect;
		leftRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//�ײ�С����
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //���ƺ������ı����

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //���ƺ������ı���λ����//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //���ƺ������ı���λ����//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //���ƺ������ı�//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//���ƺ������ı� //�������ұ���󳤶� cwcq2012.11.02

	}
	//����78�ָ���
	m_CurveNum=8*(m_nCurPage-1)+7;//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 13;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=6; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
			else if (5==i)
			{
				pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
				pDC->LineTo((m_rectDraw.Width() + nHmidspace)/2 + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight- nVspace, nCurSeparate*3 + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum+1,GetSorName(m_CurveNum+1));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	else if (g_sorFileArray.totalCurveNumber==m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 13;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=3; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	//����7
	m_CurveNum=8*(m_nCurPage-1)+6;//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//����7�����
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2, nCurHeight-nVspace, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10-nVspace);
		//curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//����դ�� wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//��������7
		//���С����
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		//leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//�ײ�С����
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //���ƺ������ı����

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //���ƺ������ı���λ����//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //���ƺ������ı���λ����//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //���ƺ������ı�//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//���ƺ������ı� //�������ұ���󳤶� cwcq2012.11.02

	}
	//����8
	m_CurveNum=8*(m_nCurPage-1)+7;//�ڼ�������
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//����8�����
		CRect curveRect;
		curveRect.SetRect((m_rectDraw.Width() + nHmidspace)/2 + nHleftspace, nCurHeight-nVspace, m_rectDraw.Width() - nHleftspace*6/5, nCurHeight + nVspace * 10-nVspace);
		pDC->Rectangle(&curveRect);
		//���С����
		CRect leftRect;
		leftRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		pDC->Rectangle(&leftRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//����դ�� wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//��������8
		DrawVerCoordUnit(pDC, leftRect);
		//�ײ�С����
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //���ƺ������ı����

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //���ƺ������ı���λ����//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //���ƺ������ı���λ����//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //���ƺ������ı�//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//���ƺ������ı� //�������ұ���󳤶� cwcq2012.11.02

	}
}

void CPrintView::DrawCurCurve(CDC* pDC)
{
	//����
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	int nHleftspace = m_rectDraw.Width() / 10;
	int nHmidspace = m_rectDraw.Width() / 20;
	//����
	pDC->MoveTo(m_rectDraw.Width() / 3, nCurHeight);
	pDC->LineTo(m_rectDraw.Width() * 2/3, nCurHeight);
	CRect titleRect;
	titleRect.SetRect(m_rectDraw.Width() / 3, nCurHeight-nVspace, m_rectDraw.Width() * 2/3, nCurHeight);
	pDC->DrawText(g_sorFileArray.reportConfig.Title, titleRect, DT_CENTER);
	//����
	for (int i=1; i<=6; i++)
	{
		nCurHeight = nCurHeight + nVspace;
		pDC->MoveTo(nHleftspace, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace)/2 ,nCurHeight);

		CRect itemRect;
		itemRect.SetRect(nHleftspace, nCurHeight - nVspace,(m_rectDraw.Width() - nHmidspace)/2, nCurHeight);
		DrawReportItem(pDC, itemRect, i*2-1);

		pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
		itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight - nVspace,(m_rectDraw.Width() - nHleftspace), nCurHeight);
		DrawReportItem(pDC, itemRect, i*2);
	}
	//�ָ��ߣ����߱��� // zll 2012.9.27ע��
	//{
	//	nCurHeight = nCurHeight + nVspace * 3;  //Item1
	//	pDC->MoveTo(nHleftspace, nCurHeight);
	//	pDC->LineTo((m_rectDraw.Width() - nHmidspace)/3 ,nCurHeight);
	//	CRect itemRect;
	//	itemRect.SetRect(nHleftspace, nCurHeight - nVspace,(m_rectDraw.Width() - nHmidspace)/3, nCurHeight);
	//	DrawCurveItem(pDC, itemRect, 1);

	//	pDC->MoveTo((m_rectDraw.Width()+ nHmidspace)/3, nCurHeight);  //Item2
	//	pDC->LineTo((m_rectDraw.Width() - nHmidspace) *2/3 , nCurHeight);
	//	itemRect.SetRect((m_rectDraw.Width()+ nHmidspace)/3, nCurHeight - nVspace,(m_rectDraw.Width() - nHmidspace) *2/3, nCurHeight);
	//	DrawCurveItem(pDC, itemRect, 2);

	//	pDC->MoveTo(m_rectDraw.Width()* 2/3, nCurHeight);  //Item3
	//	pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
	//	itemRect.SetRect(m_rectDraw.Width()* 2/3, nCurHeight - nVspace,m_rectDraw.Width() - nHleftspace, nCurHeight);
	//	DrawCurveItem(pDC, itemRect, 3);
	//}
	//����
	{
		//�����
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, m_rectDraw.Width() - nHleftspace, nCurHeight + nVspace * 20);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//����դ�� wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurve(pDC, curveRect, g_sorFileArray.curSelection);
		//���С����
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-10*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-8*nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect); //�����������ı�
		//�ײ�С����
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+3*nHleftspace, curveRect.bottom +nVspace*3);
		pDC->Rectangle(&bottomRect);
		
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //���ƺ������ı����
		
		CRect mideUnitRect;
		mideUnitRect.SetRect(bottomRect.left-nHleftspace, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //���ƺ������ı���λ����//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //���ƺ������ı���λ����//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -400-nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //���ƺ������ı�
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //���ƺ������ı� //�������ұ���󳤶� cwcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//���ƺ������ı� //�������ұ���󳤶� cwcq2012.11.02

	}

	//���߲���
	{
		nCurHeight = nCurHeight + nVspace * 22;
		//�������߲���
		DrawCurveParam(pDC, nHleftspace, nCurHeight);
	}
	//�¼��б�
	{
		nCurHeight = nCurHeight + nVspace * 10;
		//�����¼��б��ͷ
		DrawEventHead(pDC, nHleftspace, nCurHeight);
		//�����¼���
		DrawEventBody(pDC, nHleftspace, nCurHeight);
	}
}

/*
***************************************************************
��������:             DrawEventHead()                                                                               
����˵��:             �������¼��б�ͷ                                                                             
��ڲ���:             nLeft ����㣬 nTop �����                                                                   
���ڲ���:             ��                                                                                        
����ʱ��:             2011-07-24                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CPrintView::DrawEventHead(CDC* pDC, int nLeft, int nTop)//�����¼��б�
{
	CFont* pOldFont = pDC->SelectObject (&m_dataFont);
	//���Ʊ�����
	LV_COLUMN lvColumn;
	TCHAR uin[50];
	lvColumn.pszText=uin;
	lvColumn.cchTextMax=50;
	lvColumn.mask=LVCF_TEXT;
	CString header=_T("");
	int nLen = MAIN->m_pEventFormView->m_Eventlist.GetHeaderCtrl()->GetItemCount();//�õ�����
	int nVspace = m_rectDraw.Height() / 42;
	int nHspace = m_rectDraw.Width() / 10;
	//��ͷ����
	if (nLen > 8) //���9��
	{
		nLen = 8;
	}
	CRect textRect;
	for(int i=0;i<nLen;i++)
	{
		textRect.left = nLeft + i * nHspace;
		textRect.top = nTop;
		textRect.right = nLeft + (i+1) * nHspace;
		textRect.bottom = nTop + nVspace;
		if(MAIN->m_pEventFormView->m_Eventlist.GetColumn(i,&lvColumn))
		{
			header=lvColumn.pszText;
			pDC->DrawText(header, &textRect,DT_CENTER);//����
		}
		//nStartLine++;
	}
	pDC->SelectObject (pOldFont);
}

/*
***************************************************************
��������:             DrawEventBody()                                                                               
����˵��:             �������¼��б���                                                                             
��ڲ���:             nLeft ����㣬 nTop �����                                                                   
���ڲ���:             ��                                                                                        
����ʱ��:             2011-07-24                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CPrintView::DrawEventBody(CDC* pDC, int nLeft, int nTop)//�����¼��б�
{
	CFont* pOldFont = pDC->SelectObject (&m_dataFont);
	int nVspace = m_rectDraw.Height() / 42;
	int nHspace = m_rectDraw.Width() / 10;
	int nStartLine = 1;

	float preDistance=0;

	TestResultInfo* pData = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;
	if (NULL == pData)
		return;

	CString strText=_T("");
	CRect textRect;
	std::vector<EventInfo> ve = pData->EventList.vEventInfo;//�ٶ�??
	int nSize=ve.size();
	for (int i=0;i<nSize;i++)//
	{
		// ���
		{
			textRect.left = nLeft;
			textRect.top = nTop + nStartLine * nVspace;
			textRect.right = nLeft + nHspace;
			textRect.bottom = nTop + (nStartLine+1) * nVspace;

			//����
			strText.Format(_T("%d"),i+1);
			if (g_currentLang == LANG::CHS)
			{
				if (0 == ve[i].nEventType)
				{
					strText += _T(" �Ƿ���");//0����Ƿ����¼�����
				}
				else if (1 == ve[i].nEventType)
				{
					strText += _T(" ����");;//1�������¼�����
				}
			}
			else if (g_currentLang == LANG::ENG)
			{
				if (0 == ve[i].nEventType)
				{
					strText += _T(" NoReflect");//0����Ƿ����¼�����
				}
				else if (1 == ve[i].nEventType)
				{
					strText += _T(" Reflect");;//1�������¼�����
				}
			}

			if (_T('S')==ve[i].cEventType)
			{
				strText+=_T("(S)");
			}
			else if (_T('E')==ve[i].cEventType)
			{
				strText+=_T("(E)");
			}
			else if (_T('A')==ve[i].cEventType)
			{
				strText+=_T("(A)");
			}
			else if (_T('F')==ve[i].cEventType)
			{
				strText+=_T("(F)");
			}
			else if (_T('O')==ve[i].cEventType)
			{
				strText+=_T("(O)");
			}

			pDC->DrawText(strText, &textRect,DT_LEFT);//����
		}
		// �¼�����
		//{
		//	textRect.left = nLeft + nHspace;
		//	textRect.top = nTop + nStartLine * nVspace;
		//	textRect.right = nLeft + 2 * nHspace;
		//	textRect.bottom = nTop + (nStartLine+1) * nVspace;

		//	//����
		//	if (0 == ve[i].nEventType)
		//	{
		//		strText = _T("��ʼ�¼�");
		//	}
		//	else if (1 == ve[i].nEventType)
		//	{
		//		strText = _T("�����¼�");
		//	}
		//	else if (2 == ve[i].nEventType)
		//	{
		//		strText = _T("�Ƿ����¼�");
		//	}
		//	else if (3 == ve[i].nEventType)
		//	{
		//		strText = _T("�����¼�");
		//	}
		//	else if (4 == ve[i].nEventType)
		//	{
		//		strText = _T("�����¼�");
		//	}
		//	else
		//	{
		//		strText = _T("--");
		//	}
		//	pDC->DrawText(strText, &textRect,DT_CENTER);//����
		//}
		// ����
		{
			textRect.left = nLeft + nHspace;
			textRect.top = nTop + nStartLine * nVspace;
			textRect.right = nLeft + 2 * nHspace;
			textRect.bottom = nTop + (nStartLine+1) * nVspace;
			float m_float_zll=0.0f;
			if (0==i)
			{
				strText.Format(_T("%.3f"),0.0f);//����
			}
			else
			{
				if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
					strText.Format(_T("%.3f"),ve[i].fLocation/1000);//����
				else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪmiles
				{
					m_float_zll=KmeterToOther(ve[i].fLocation/1000,UNIT_SHOW_MILE);
					strText.Format(_T("%.3f"),m_float_zll);//����
				}
				else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkft
				{
					m_float_zll=KmeterToOther(ve[i].fLocation/1000,UNIT_SHOW_KFEET);
					strText.Format(_T("%.3f"),m_float_zll);//����
				}
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}
		// ��
		{
			textRect.left = nLeft + 2 * nHspace;
			textRect.top = nTop + nStartLine * nVspace;
			textRect.right = nLeft + 3 * nHspace;
			textRect.bottom = nTop + (nStartLine+1) * nVspace;
			float a,b=0.0f;
			if (0==i)
			{
				strText.Format(_T("%.3f"),0.0f);//��
			}
			else
			{
				if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
					strText.Format(_T("%.3f"),(ve[i].fLocation-preDistance)/1000);//��,���¼�������
				else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪmiles
				{
					b=(ve[i].fLocation-preDistance)/1000;
					a=KmeterToOther(b,UNIT_SHOW_MILE);
					strText.Format(_T("%.3f"),a);//��,���¼�������
				}
				else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkft
				{
					b=(ve[i].fLocation-preDistance)/1000;
					a=KmeterToOther(b,UNIT_SHOW_KFEET);
					strText.Format(_T("%.3f"),a);//��,���¼�������
				}
			}
			if (0==i)
			{
				preDistance = 0.0f;
			}
			else
			{
				preDistance = ve[i].fLocation;
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}
		// ���
		{
			textRect.left = nLeft + 3 * nHspace;
			textRect.top = nTop + nStartLine * nVspace;
			textRect.right = nLeft + 4 * nHspace;
			textRect.bottom = nTop + (nStartLine+1) * nVspace;
			if (0 == ve[i].fEventloss)//Ϊ0ʱ��ʾ"-"wcq2012.07.09
			{
				strText.Format(_T("--"));//�����
			}
			else
			{
				strText.Format(_T("%.3f"), ve[i].fEventloss);//���
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}
		// �����
		{
			textRect.left = nLeft + 4 * nHspace;
			textRect.top = nTop + nStartLine * nVspace;
			textRect.right = nLeft + 5 * nHspace;
			textRect.bottom = nTop + (nStartLine+1) * nVspace;
			if (0 == ve[i].fTotalLoss)//Ϊ0ʱ��ʾ"-"wcq2012.07.09
			{
				strText.Format(_T("--"));//�����
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fTotalLoss);//�����
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}
		// б��
		{
			textRect.left = nLeft + 5 * nHspace;
			textRect.top = nTop + nStartLine * nVspace;
			textRect.right = nLeft + 6 * nHspace;
			textRect.bottom = nTop + (nStartLine+1) * nVspace;
			//б��
			if (fabs(ve[i].fLineK)>0.5)//zll 2012.10.31
			{
				strText.Format(_T("--"));
			}
			else if (0 == ve[i].fLineK)//Ϊ0ʱ��ʾ"-" zll 2012.10.31
			{
				strText.Format(_T("--"));
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fLineK);//б��
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}
		// ����
		{
			textRect.left = nLeft + 6 * nHspace;
			textRect.top = nTop + nStartLine * nVspace;
			textRect.right = nLeft + 7 * nHspace;
			textRect.bottom = nTop + (nStartLine+1) * nVspace;
			if (0 == ve[i].nEventType)//����
			{
				strText=_T("--");
			}
			else if (0 == ve[i].fEventHigh)//Ϊ0ʱ��ʾ"-"wcq2012.07.09
			{
				strText=_T("--");
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fEventHigh);//����
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}

		nStartLine++; //������1
	}
	pDC->SelectObject (pOldFont);
}


/*
***************************************************************
��������:             DrawEventBody()                                                                               
����˵��:             �������¼��б���                                                                             
��ڲ���:             nLeft ����㣬 nTop �����                                                                   
���ڲ���:             ��                                                                                        
����ʱ��:             2011-07-24                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CPrintView::DrawEventBody1(CDC* pDC, int nLeft, int nTop,int m_nCurPage)//�����¼��б�
{
	CFont* pOldFont = pDC->SelectObject (&m_dataFont);
	int nVspace = m_rectDraw.Height() / 42;
	int nHspace = m_rectDraw.Width() / 10;
	int nStartLine = 1;

	float preDistance=0;

	TestResultInfo* pData = g_sorFileArray.sorFileList[m_nCurPage-1].pTestResultInfo;
	if (NULL == pData)
		return;

	CString strText=_T("");
	CRect textRect;
	std::vector<EventInfo> ve = pData->EventList.vEventInfo;//�ٶ�??
	int nSize=ve.size();
	for (int i=0;i<nSize;i++)//
	{
		// ���
		{
			textRect.left = nLeft;
			textRect.top = nTop + nStartLine * nVspace;
			textRect.right = nLeft + nHspace;
			textRect.bottom = nTop + (nStartLine+1) * nVspace;

			//����
			strText.Format(_T("%d"),i+1);
			if (g_currentLang == LANG::CHS)
			{
				if (0 == ve[i].nEventType)
				{
					strText += _T(" �Ƿ���");//0����Ƿ����¼�����
				}
				else if (1 == ve[i].nEventType)
				{
					strText += _T(" ����");;//1�������¼�����
				}
			}
			else if (g_currentLang == LANG::ENG)
			{
				if (0 == ve[i].nEventType)
				{
					strText += _T(" NoReflect");//0����Ƿ����¼�����
				}
				else if (1 == ve[i].nEventType)
				{
					strText += _T(" Reflect");;//1�������¼�����
				}
			}

			if (_T('S')==ve[i].cEventType)
			{
				strText+=_T("(S)");
			}
			else if (_T('E')==ve[i].cEventType)
			{
				strText+=_T("(E)");
			}
			else if (_T('A')==ve[i].cEventType)
			{
				strText+=_T("(A)");
			}
			else if (_T('F')==ve[i].cEventType)
			{
				strText+=_T("(F)");
			}
			else if (_T('O')==ve[i].cEventType)
			{
				strText+=_T("(O)");
			}

			pDC->DrawText(strText, &textRect,DT_LEFT);//����
		}
		// �¼�����
		//{
		//	textRect.left = nLeft + nHspace;
		//	textRect.top = nTop + nStartLine * nVspace;
		//	textRect.right = nLeft + 2 * nHspace;
		//	textRect.bottom = nTop + (nStartLine+1) * nVspace;

		//	//����
		//	if (0 == ve[i].nEventType)
		//	{
		//		strText = _T("��ʼ�¼�");
		//	}
		//	else if (1 == ve[i].nEventType)
		//	{
		//		strText = _T("�����¼�");
		//	}
		//	else if (2 == ve[i].nEventType)
		//	{
		//		strText = _T("�Ƿ����¼�");
		//	}
		//	else if (3 == ve[i].nEventType)
		//	{
		//		strText = _T("�����¼�");
		//	}
		//	else if (4 == ve[i].nEventType)
		//	{
		//		strText = _T("�����¼�");
		//	}
		//	else
		//	{
		//		strText = _T("--");
		//	}
		//	pDC->DrawText(strText, &textRect,DT_CENTER);//����
		//}
		// ����
		{
			textRect.left = nLeft + nHspace;
			textRect.top = nTop + nStartLine * nVspace;
			textRect.right = nLeft + 2 * nHspace;
			textRect.bottom = nTop + (nStartLine+1) * nVspace;
			float m_float_zll=0.0f;
			if (0==i)
			{
				strText.Format(_T("%.3f"),0.0f);//����
			}
			else
			{
				if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
					strText.Format(_T("%.3f"),ve[i].fLocation/1000);//����
				else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪmiles
				{
					m_float_zll=KmeterToOther(ve[i].fLocation/1000,UNIT_SHOW_MILE);
					strText.Format(_T("%.3f"),m_float_zll);//����
				}
				else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkft
				{
					m_float_zll=KmeterToOther(ve[i].fLocation/1000,UNIT_SHOW_KFEET);
					strText.Format(_T("%.3f"),m_float_zll);//����
				}
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}
		// ��
		{
			textRect.left = nLeft + 2 * nHspace;
			textRect.top = nTop + nStartLine * nVspace;
			textRect.right = nLeft + 3 * nHspace;
			textRect.bottom = nTop + (nStartLine+1) * nVspace;
			float a,b=0.0f;
			if (0==i)
			{
				strText.Format(_T("%.3f"),0.0f);//��
			}
			else
			{
				if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
					strText.Format(_T("%.3f"),(ve[i].fLocation-preDistance)/1000);//��,���¼�������
				else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪmiles
				{
					b=(ve[i].fLocation-preDistance)/1000;
					a=KmeterToOther(b,UNIT_SHOW_MILE);
					strText.Format(_T("%.3f"),a);//��,���¼�������
				}
				else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkft
				{
					b=(ve[i].fLocation-preDistance)/1000;
					a=KmeterToOther(b,UNIT_SHOW_KFEET);
					strText.Format(_T("%.3f"),a);//��,���¼�������
				}
			}
			if (0==i)
			{
				preDistance = 0.0f;
			}
			else
			{
				preDistance = ve[i].fLocation;
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}
		// ���
		{
			textRect.left = nLeft + 3 * nHspace;
			textRect.top = nTop + nStartLine * nVspace;
			textRect.right = nLeft + 4 * nHspace;
			textRect.bottom = nTop + (nStartLine+1) * nVspace;
			if (0 == ve[i].fEventloss)//Ϊ0ʱ��ʾ"-"wcq2012.07.09
			{
				strText.Format(_T("--"));//�����
			}
			else
			{
				strText.Format(_T("%.3f"), ve[i].fEventloss);//���
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}
		// �����
		{
			textRect.left = nLeft + 4 * nHspace;
			textRect.top = nTop + nStartLine * nVspace;
			textRect.right = nLeft + 5 * nHspace;
			textRect.bottom = nTop + (nStartLine+1) * nVspace;
			if (0 == ve[i].fTotalLoss)//Ϊ0ʱ��ʾ"-"wcq2012.07.09
			{
				strText.Format(_T("--"));//�����
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fTotalLoss);//�����
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}
		// б��
		{
			textRect.left = nLeft + 5 * nHspace;
			textRect.top = nTop + nStartLine * nVspace;
			textRect.right = nLeft + 6 * nHspace;
			textRect.bottom = nTop + (nStartLine+1) * nVspace;
			//б��
			if (fabs(ve[i].fLineK)>0.5)//zll 2012.10.31
			{
				strText.Format(_T("--"));
			}
			else if (0 == ve[i].fLineK)//Ϊ0ʱ��ʾ"-" zll 2012.10.31
			{
				strText.Format(_T("--"));
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fLineK);//б��
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}
		// ����
		{
			textRect.left = nLeft + 6 * nHspace;
			textRect.top = nTop + nStartLine * nVspace;
			textRect.right = nLeft + 7 * nHspace;
			textRect.bottom = nTop + (nStartLine+1) * nVspace;
			if (0 == ve[i].nEventType)//�Ƿ���
			{
				strText=_T("--");
			}
			else if (0 == ve[i].fEventHigh)//Ϊ0ʱ��ʾ"-"wcq2012.07.09
			{
				strText=_T("--");
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fEventHigh);//����
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}
	
		nStartLine++; //������1
	}
	pDC->SelectObject (pOldFont);
}

/*
***************************************************************
��������:             DrawCurveParam()                                                                               
����˵��:             �������¼��б���                                                                             
��ڲ���:             nLeft ����㣬 nTop �����                                                                   
���ڲ���:             ��                                                                                        
����ʱ��:             2011-07-24                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CPrintView::DrawCurveParam(CDC* pDC, int nLeft, int nTop)//�������߲���
{
	int nVspace = m_rectDraw.Height() / 42;
	int nHspace = m_rectDraw.Width() / 10;
	int nStartLine = 1;

	TestResultInfo* pData = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;
	if (NULL == pData)
		return;

	CFont* pOldFont = pDC->SelectObject (&m_dataFont);
	CRect textRect;
	CString strText;
	// �ļ���
	{
		textRect.left = nLeft;
		textRect.top = nTop + nVspace;
		textRect.right = m_rectDraw.right;
		textRect.bottom = nTop + 2 * nVspace;
		CString strFileName = IniUtil::ReadResString(_T("IDS_FRM_VIEW_DLG_FILENAME"));
		strText.Format(strFileName+_T("��	%s"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].filePathName);
		pDC->DrawText(strText, &textRect,DT_LEFT);//����
	}
	// ����
	{
		textRect.left = m_rectDraw.Width()/2;
		textRect.top = nTop + 2 * nVspace;
		textRect.right = textRect.left + 4 * nHspace;
		textRect.bottom = nTop + 3 * nVspace;
		CString strWaveLength = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_WAVE_LENGTH"));
		strText.Format(strWaveLength+_T("��	%dnm"), pData->WaveLength);//pData->WaveLength / 10��ΪpData->WaveLength zll 2012.11.1
		pDC->DrawText(strText, &textRect,DT_LEFT);//����
	}
	// ����
	{
		textRect.left = nLeft;
		textRect.top = nTop + 2 * nVspace;
		textRect.right = nLeft + 4 * nHspace;
		textRect.bottom = nTop + 3 * nVspace;
		CString strPulseWidth = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_PULSE_WIDTH"));
		if (pData->PulseWidth>1000||pData->PulseWidth==1000)
		{
			strText.Format(strPulseWidth+_T("��	%dus"), pData->PulseWidth/1000);
		}
		else
		{
			strText.Format(strPulseWidth+_T("��	%dns"), pData->PulseWidth);
		}
		pDC->DrawText(strText, &textRect,DT_LEFT);//����
	}

	// ������Χ
	{
		float m_Length=0.0f;
		textRect.left = m_rectDraw.Width()/2;
		textRect.top = nTop + 3 * nVspace;
		textRect.right = textRect.left + 4 * nHspace;
		textRect.bottom = nTop + 4 * nVspace;
		CString strRange=IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_RANGE"));
		if (_T("m")==g_sorFileArray.waveConfig.DistUnit)
		{
			strText.Format(strRange+_T("��	%.2fkm"), pData->Length/1000.0);
		}
		else if (_T("mi")==g_sorFileArray.waveConfig.DistUnit)
		{
			m_Length=KmeterToOther(pData->Length/1000.0,UNIT_SHOW_MILE);
			strText.Format(strRange+_T("��	%.2fmiles"), m_Length);
		}
		else if (_T("ft")==g_sorFileArray.waveConfig.DistUnit)
		{
			m_Length=KmeterToOther(pData->Length/1000.0,UNIT_SHOW_KFEET);
			strText.Format(strRange+_T("��	%.2fkft"), m_Length);
		}
		pDC->DrawText(strText, &textRect,DT_LEFT);//����
	}
	// ������
	{
		textRect.left = nLeft;
		textRect.top = nTop + 3 * nVspace;
		textRect.right = nLeft + 4 * nHspace;
		textRect.bottom = nTop + 4 * nVspace;
		CString strRefraction = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_REFRACTION"));
		strText.Format(strRefraction+_T("��	%.3f"), pData->Refraction);
		pDC->DrawText(strText, &textRect,DT_LEFT);//����
	}

	// ˥�� zll 2012.11.1 ��ʱע��
	//{
	//	textRect.left = m_rectDraw.Width()/2;
	//	textRect.top = nTop + 4 * nVspace;
	//	textRect.right = textRect.left + 4 * nHspace;
	//	textRect.bottom = nTop + 5 * nVspace;
	//	CString strAttenu = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_ATTENU"));
	//	strText.Format(strAttenu+_T("��	%.2f"), pData->Atten);
	//	pDC->DrawText(strText, &textRect,DT_LEFT);//����
	//}
	// ������
	{
		textRect.left = nLeft;
		textRect.top = nTop + 4 * nVspace;
		textRect.right = nLeft + 4 * nHspace;
		textRect.bottom = nTop + 5 * nVspace;
		CString strSample = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_SAMPLE"));
		strText.Format(strSample+_T("��	%d"), pData->DataPointsNum);
		pDC->DrawText(strText, &textRect,DT_LEFT);//����
	}

	pDC->SelectObject (pOldFont);
}

/*
***************************************************************
��������:             DrawCurve()                                                                               
����˵��:             ����������                                                                              
��ڲ���:             CDC                                                                                           
���ڲ���:             ��                                                                                        
����ʱ��:             2011-07-24                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CPrintView::DrawCurve(CDC* pDC, CRect& rect, int nCurveIndex)
{
	TestResultInfo* pData = g_sorFileArray.sorFileList[nCurveIndex].pTestResultInfo;
	if (pData == NULL ||
		pData->DataPointsNum < NUM_VALIDE_LEAST ||
		CLEAR_DATAPOINTS_NUM == pData->DataPointsNum)//û�л����ݺ���
		return;

	unsigned int i;

	//ת��Ϊ��ͼ��������
	if (NULL==pData->ptDrawData)
	{
		pData->ptDrawData=new CPoint[pData->DataPointsNum];
	}
	pData->StartPoint=0;
    pData->MaxPoint=pData->DataPointsNum;
	for(i = pData->StartPoint; i < pData->MaxPoint; i++)
	{
		pData->ptDrawData[i].x = int(rect.left + rect.Width () * (i - pData->StartPoint)/(pData->MaxPoint - pData->StartPoint));
		//pData->ptDrawData[i].y = int(rect.bottom - (((-pData->pDataPoints[i]/1000.0) - M_FSTARTDB0)/(M_FMAXDB0 - M_FSTARTDB0)) * rect.Height());
#ifdef COMPILE_SHOW_SMOOTH_DATA////��ʾʱ��ƽ���������ݣ�����ʱ��ʵ������ 2013.05.12
		pData->ptDrawData[i].y = int(rect.bottom - (((-pData->pDataPointsSmooth[i]/1000.0) - M_FSTARTDB0)/(M_FMAXDB0 - M_FSTARTDB0)) * rect.Height());
#else
		pData->ptDrawData[i].y = int(rect.bottom - (((-pData->pDataPoints[i]/1000.0) - M_FSTARTDB0)/(M_FMAXDB0 - M_FSTARTDB0)) * rect.Height() - 150);//zll 2013.6.3 -150 ʹ������y�������ƶ�
#endif
		if(pData->ptDrawData[i].x > rect.right)
			pData->ptDrawData[i].x = rect.right;
		if(pData->ptDrawData[i].y < rect.top)
			pData->ptDrawData[i].y = rect.top;
		if(pData->ptDrawData[i].y > rect.bottom)
			pData->ptDrawData[i].y = rect.bottom; 
	}

	//������
	pDC->Polyline(&pData->ptDrawData[pData->StartPoint], pData->MaxPoint - pData->StartPoint);

	//���¼������߼��·����ο� zll 2013.5.23 ��
	int fX,fY;
	for (int i=0; i<pData->EventList.vEventInfo.size(); i++)
	{		
		CRect rectLineBottom;
		CString temp;
		fX=int(rect.left + pData->EventList.vEventInfo[i].fLocation*rect.Width()/pData->Length);
		fY=int(rect.bottom - (65535-pData->pDataPoints[pData->EventList.vEventInfo[i].index])/1000.0*rect.Height()/(OTDR_TRACE_Y_MAX-OTDR_TRACE_Y_MIN) - 110);//zll 2013.6.3 -110 ʹ�¼����ʾ��y�������ƶ�
		//����ֱ��
		pDC->MoveTo(fX,fY- HIKE_UP_HEIGHT);
		pDC->LineTo(fX,fY+44+50);//zll 2013.6.3 +50 ʹ�¼����ʾ��y�������ƶ�
		//��ֱ�������������
		rectLineBottom.SetRect(fX - 8-20,fY + 34+60,fX - 8 + 16+20, fY + 34 + 18+100);
		if (i>8)//��λ��
		{
			rectLineBottom.right += 2;
		}
		pDC->Rectangle(&rectLineBottom);

		temp.Format(_T("%d"),i + 1);
		//pDC->SelectObject(&fontValue);////zll 2013.5.11 ������A��B���Աߵ���������
		rectLineBottom.DeflateRect(2,2);
		pDC->DrawText(temp,&rectLineBottom,DT_CENTER | DT_VCENTER | DT_SINGLELINE);//���ο��е���ֵ
	}
}

/*
***************************************************************
��������:             DrawCurveByReport()                                                                               
����˵��:             ����������                                                                              
��ڲ���:             CDC                                                                                           
���ڲ���:             ��                                                                                        
����ʱ��:             2011-07-24                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CPrintView::DrawCurveByReport(CDC* pDC, CRect& rect, int nCurveIndex)//zll 2013.6.3 ��
{
	TestResultInfo* pData = g_sorFileArray.sorFileList[nCurveIndex].pTestResultInfo;
	if (pData == NULL ||
		pData->DataPointsNum < NUM_VALIDE_LEAST ||
		CLEAR_DATAPOINTS_NUM == pData->DataPointsNum)//û�л����ݺ���
		return;

	unsigned int i;

	//ת��Ϊ��ͼ��������
	if (NULL==pData->ptDrawData)
	{
		pData->ptDrawData=new CPoint[pData->DataPointsNum];
	}
	pData->StartPoint=0;
	pData->MaxPoint=pData->DataPointsNum;
	for(i = pData->StartPoint; i < pData->MaxPoint; i++)
	{
		pData->ptDrawData[i].x = int(rect.left + rect.Width () * (i - pData->StartPoint)/(pData->MaxPoint - pData->StartPoint));
		//pData->ptDrawData[i].y = int(rect.bottom - (((-pData->pDataPoints[i]/1000.0) - M_FSTARTDB0)/(M_FMAXDB0 - M_FSTARTDB0)) * rect.Height());
#ifdef COMPILE_SHOW_SMOOTH_DATA////��ʾʱ��ƽ���������ݣ�����ʱ��ʵ������ 2013.05.12
		pData->ptDrawData[i].y = int(rect.bottom - (((-pData->pDataPointsSmooth[i]/1000.0) - M_FSTARTDB0)/(M_FMAXDB0 - M_FSTARTDB0)) * rect.Height());
#else
		pData->ptDrawData[i].y = int(rect.bottom - (((-pData->pDataPoints[i]/1000.0) - M_FSTARTDB0)/(M_FMAXDB0 - M_FSTARTDB0)) * rect.Height() - 150);//zll 2013.6.3 -150 ʹ������y�������ƶ�
#endif
		if(pData->ptDrawData[i].x > rect.right)
			pData->ptDrawData[i].x = rect.right;
		if(pData->ptDrawData[i].y < rect.top)
			pData->ptDrawData[i].y = rect.top;
		if(pData->ptDrawData[i].y > rect.bottom)
			pData->ptDrawData[i].y = rect.bottom; 
	}

	//������
	pDC->Polyline(&pData->ptDrawData[pData->StartPoint], pData->MaxPoint - pData->StartPoint);

	//���¼������߼��·����ο� zll 2013.5.23 ��
	int fX,fY;
	for (int i=0; i<pData->EventList.vEventInfo.size(); i++)
	{		
		CRect rectLineBottom;
		CString temp;
		fX=int(rect.left + pData->EventList.vEventInfo[i].fLocation*rect.Width()/pData->Length);
		fY=int(rect.bottom - (65535-pData->pDataPoints[pData->EventList.vEventInfo[i].index])/1000.0*rect.Height()/(OTDR_TRACE_Y_MAX-OTDR_TRACE_Y_MIN) - 150);//zll 2013.6.3 -110 ʹ�¼����ʾ��y�������ƶ�
		//����ֱ��
		pDC->MoveTo(fX,fY);
		pDC->LineTo(fX,fY+44+90);//zll 2013.6.3 +50 ʹ�¼����ʾ��y�������ƶ�
		//��ֱ�������������
		rectLineBottom.SetRect(fX - 8-20,fY + 34+100,fX - 8 + 16+20, fY + 34 + 18+140);
		if (i>8)//��λ��
		{
			rectLineBottom.right += 2;
		}
		pDC->Rectangle(&rectLineBottom);

		temp.Format(_T("%d"),i + 1);
		//pDC->SelectObject(&fontValue);////zll 2013.5.11 ������A��B���Աߵ���������
		rectLineBottom.DeflateRect(2,2);
		pDC->DrawText(temp,&rectLineBottom,DT_CENTER | DT_VCENTER | DT_SINGLELINE);//���ο��е���ֵ
	}
}

void CPrintView::InnerDraw(CDC* pDC,int m_nCurPage)
{
	if (m_bPrintCurve) //��ӡ��ǰѡ������
	{
		DrawCurCurve(pDC);
		return;
	}
	
	int nPrint = _wtoi(g_sorFileArray.reportConfig.Print);
	if (!nPrint) //�ܱ�
		DrawReport(pDC);
	else //����
	{
		int nCurveNumber = _wtoi(g_sorFileArray.reportConfig.WaveList);
		if (nCurveNumber == 0)
			Draw1Curve(pDC,m_nCurPage);
		else if (nCurveNumber == 1)
			Draw2Curve(pDC,m_nCurPage);
		else if (nCurveNumber == 2)
			Draw4Curve(pDC,m_nCurPage);
		else if (nCurveNumber == 3)
			Draw6Curve(pDC,m_nCurPage);
		else if (nCurveNumber == 4)
			Draw8Curve(pDC,m_nCurPage);
	}
}

//����դ�� wcq2012.10.31
void CPrintView::DrawGridLine(CDC* pDC, CRect& rect)
{
	if(!g_sorFileArray.waveConfig.ShowGrid)//����ʾդ��
	{
		return;
	}//eif

	CPen penGridLine;
	//penGridLine.CreatePen (PS_DOT/*����*/,1, g_sorFileArray.waveConfig.ColorGrid);//��Ϊ�����cwcq2012.10.31
	penGridLine.CreatePen (PS_DOT/*����*/, 1, RGB(0xcc,0xcc,0xcc) );//wcq2012.10.31
	CPen* pOldPen = pDC->SelectObject (&penGridLine);

	if (g_sorFileArray.waveConfig.ShowGrid)
	{
		//������̶�
		int ndx = rect.Width () / COOR_SPLIT_COUNT_H;
		int ndy = rect.Height () / COOR_SPLIT_COUNT_V;
		pDC->SelectObject (&penGridLine);
		for ( int i = 1; i< COOR_SPLIT_COUNT_H; i++)
		{	
			//������̶�
			pDC->MoveTo(rect.left + ndx * i,rect.bottom );
			pDC->LineTo(rect.left + ndx * i,rect.top );
		}

		//������̶�
		pDC->SetTextAlign(TA_LEFT | TA_TOP);
		for (int i=1; i<COOR_SPLIT_COUNT_V; i++)
		{
			pDC->MoveTo(rect.left , rect.bottom - ndy * i);
			pDC->LineTo(rect.right, rect.bottom - ndy * i);
		}
	}

	//�ָ�
	pDC->SelectObject(pOldPen);
	//�ͷ���Դ
	DeleteObject(penGridLine.GetSafeHandle());
}

CString CPrintView::right_MAX(float m_float_zll,float flo_Length)
{
	CString temp=_T("");
	if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
	{
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			temp.Format(_T("%.3fkm"),flo_Length / 1000);//wcq2012.11.02
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			temp.Format(_T("%.4fkm"),flo_Length / 1000);//wcq2012.11.02
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			temp.Format(_T("%.5fkm"),flo_Length / 1000);//wcq2012.11.02
	}
	else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪmiles
	{
		m_float_zll=KmeterToOther(flo_Length / 1000,UNIT_SHOW_MILE);
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			temp.Format(_T("%.3fmiles"),m_float_zll);//����
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			temp.Format(_T("%.4fmiles"),m_float_zll);//����
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			temp.Format(_T("%.5fmiles"),m_float_zll);//����
	}
	else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkft
	{
		m_float_zll=KmeterToOther(flo_Length / 1000,UNIT_SHOW_KFEET);
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			temp.Format(_T("%.3fkft"),m_float_zll);//����
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			temp.Format(_T("%.4fkft"),m_float_zll);//����
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			temp.Format(_T("%.5fkft"),m_float_zll);//����
	}
	return temp;
}

CString CPrintView::branch(float m_float_zll,float flo_Length)
{
	CString strDivUnit=_T("");
	if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
	{
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			strDivUnit.Format(_T("%.3fkm"),(flo_Length / 1000)/ 10);//wcq2012.11.02
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			strDivUnit.Format(_T("%.4fkm"),(flo_Length / 1000)/ 10);//wcq2012.11.02
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			strDivUnit.Format(_T("%.5fkm"),(flo_Length / 1000)/ 10);//wcq2012.11.02
	}
	else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪmiles
	{
		m_float_zll=KmeterToOther((flo_Length / 1000)/ 10,UNIT_SHOW_MILE);
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			strDivUnit.Format(_T("%.3fmiles"),m_float_zll);//����
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			strDivUnit.Format(_T("%.4fmiles"),m_float_zll);//����
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			strDivUnit.Format(_T("%.5fmiles"),m_float_zll);//����
	}
	else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkft
	{
		m_float_zll=KmeterToOther((flo_Length / 1000)/ 10,UNIT_SHOW_KFEET);
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			strDivUnit.Format(_T("%.3fkft"),m_float_zll);//����
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			strDivUnit.Format(_T("%.4fkft"),m_float_zll);//����
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			strDivUnit.Format(_T("%.5fkft"),m_float_zll);//����
	}
	return strDivUnit;
}

CString CPrintView::GetSorName(int m_CurveNum)
{
	int filePathNameLength=g_sorFileArray.sorFileList[m_CurveNum-1].filePathName.GetLength();
	int fileNameLength=g_sorFileArray.sorFileList[m_CurveNum-1].fileName.GetLength();
	CString sorName=g_sorFileArray.sorFileList[m_CurveNum-1].fileName.Mid(filePathNameLength+1,fileNameLength-(filePathNameLength+1));
	return sorName;
}