// Thumbnail.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OTDR.h"
#include "Thumbnail.h"
#include "SorFileInfo.h"
#include "SorDrawStatic.h"
#include "MainFrm.h"

extern SorFileArray g_sorFileArray;
// CThumbnail

IMPLEMENT_DYNCREATE(CThumbnail, CFormView)

CThumbnail::CThumbnail()
	: CFormView(CThumbnail::IDD)
{
}

CThumbnail::~CThumbnail()
{
}

void CThumbnail::SetLocalString()
{
	CString strTitle = IniUtil::ReadResString(_T("IDS_FRM_THUMBNAIL_TITLE"));
	GetParentFrame()->SetWindowText(strTitle);
}

void CThumbnail::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CThumbnail, CFormView)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CThumbnail ���

#ifdef _DEBUG
void CThumbnail::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CThumbnail::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CThumbnail ��Ϣ�������

void CThumbnail::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
}

void CThumbnail::OnPaint()
{
	CPaintDC dc(this);

	//ʹ��˫���弼����ͼ
	GetClientRect(&m_rectThumbnail);

	CRect InvalidRect;
		dc.GetClipBox(&InvalidRect); //��ȡ��Ч��,����Ҫ�ػ������

	CDC MemDC;
	CBitmap MemBitMap;
	CBitmap *pOldBitMap;

	//����һ����dc���ݵ��ڴ��ڴ��豸����
	if (MemDC.CreateCompatibleDC(&dc))
	{
		//����һ����dc���ݵ�λͼ����СΪ�����ͻ���
		if (MemBitMap.CreateCompatibleBitmap(&dc,m_rectThumbnail.Width(),m_rectThumbnail.Height()))
		{
			//��λͼѡ���ڴ滷��
			pOldBitMap=MemDC.SelectObject(&MemBitMap);

			//ʹMemDC�������ͻ���������Ч��
			MemDC.SelectClipRgn(NULL);
			//�� "����" ��ȡ����Ч����ʹ�ڴ滷����dc��ȡ����Ч�����
			MemDC.IntersectClipRect(InvalidRect);

			//�ñ���ɫ��λͼ����ɾ�
			MemDC.FillSolidRect(&m_rectThumbnail, RGB(236,233,216));
		}
	}
	DrawThumbnail(&MemDC);

	//�ѻ��ƺõ�ͼ����BitBlt()"����"����Ļ��
	dc.BitBlt(0,0,m_rectThumbnail.Width(),m_rectThumbnail.Height(),&MemDC, 0, 0,SRCCOPY); 
	MemDC.SelectObject(pOldBitMap);
	MemBitMap.DeleteObject();
	MemDC.DeleteDC();
}

/*
***************************************************************
��������:             DrawThumbnail()                                                                               
����˵��:             ���Ʋ�����ʾ��                                                                              
��ڲ���:             CDC                                                                         
���ڲ���:             ��                                                                         
����ʱ��:             2012-04-02                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CThumbnail::DrawThumbnail(CDC *pDC)
{
	DrawThumbCoord(pDC);
	DrawThumbCurve(pDC);

}
/*
***************************************************************
��������:             DrawThumbCoord()                                                                               
����˵��:             ��������ͼ����                                                                              
��ڲ���:             CDC                                                                         
���ڲ���:             ��                                                                         
����ʱ��:             2011-07-24                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CThumbnail::DrawThumbCoord(CDC *pDC)
{
	//��䱳��
	pDC->FillSolidRect(&m_rectThumbnail,g_sorFileArray.waveConfig.ColorBack);  //�������ڵľ�������
	if (g_sorFileArray.totalCurveNumber == 0)
		return;

	//��������ϵ
	TestResultInfo* pTestResultInfo = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;
	int nSaveDC = pDC->SaveDC();

	//zll 2013.7.3ע��
	/*CPen penThumbZoom;
	penThumbZoom.CreatePen (PS_SOLID, 1, RGB(0,255,0));*/

	CBrush brushThumbBG;
	brushThumbBG.CreateSolidBrush(g_sorFileArray.waveConfig.ColorBack);
	pDC->SelectObject(&brushThumbBG);//

	 //����ͼ�ľ�������
	pDC->Rectangle(&CRect(0, 0, m_rectThumbnail.Width(), m_rectThumbnail.Height()));
	if (NULL == pTestResultInfo->pThumbData ||
		pTestResultInfo->DataPointsNum < NUM_VALIDE_LEAST)
	{

	}
	//zll 2013.7.3ע��
	//pDC->SelectObject (&penThumbZoom);
	//CRect rect;
	//GetMoveRect(&rect, pTestResultInfo);//�õ����ž��ο�//2013.5.10 ��
	//pDC->SelectStockObject(NULL_BRUSH);
	//pDC->Rectangle(&rect);
	
	//�ָ�
	pDC->RestoreDC(nSaveDC);
	//ɾ����Դ
	//penThumbZoom.DeleteObject();//zll 2013.7.3ע��
	brushThumbBG.DeleteObject();
}

//ƽ������ʱ�õ�������ͼ�б�ʾ���ų̶ȵľ��ο�(m_rectMoveThumb).2011.12.14//zll 2013.5.10 �� ��Ҫ�޸�
void CThumbnail::GetMoveRect(CRect* rectMove, TestResultInfo* pTestResultInfo)
{
	if (pTestResultInfo->DataPointsNum < NUM_VALIDE_LEAST)//wcq2011.12.22//wcq2011.12.23
	{
		return;
	}
    /////zll 2013.5.11
	INT nWidth;//���ο��
	nWidth = m_rectThumbnail.Width();//���ο��
	INT nHeight;//���ο��
	nHeight = m_rectThumbnail.Height();//���ο��
	CGraphElement* element=MAIN->m_pWaveFormView->m_sorDrawStatic.GetSeleCurve();
	COtdrTrace* pOtdrTrace=&MAIN->m_pWaveFormView->m_sorDrawStatic;
	int left = 0;
	int top  = 0;
	int right = m_rectThumbnail.Width();
	int bottom  = m_rectThumbnail.Height();
	left=pOtdrTrace->m_fRangeX[OTDR_TRACE_MIN]*nWidth/element->m_fLength;
	right=pOtdrTrace->m_fRangeX[OTDR_TRACE_MAX]*nWidth/element->m_fLength;
	top=nHeight-pOtdrTrace->m_fRangeY[OTDR_TRACE_MIN]*nHeight/g_nTraceYmax;
	bottom=nHeight-pOtdrTrace->m_fRangeY[OTDR_TRACE_MAX]*nHeight/g_nTraceYmax;
	rectMove->left=left;
    rectMove->right=right;
	rectMove->bottom=bottom;
	rectMove->top=top;

	////if (0 == MAIN->m_pWaveFormView->m_sorDrawStatic.m_iHorizonZoomNum && //cwcq2013.03.29
	////	0 == MAIN->m_pWaveFormView->m_sorDrawStatic.m_iVerticalZoomNum)//ˮƽ��ֱ��û����
	////{
	////	rectMove->SetRect(0, 0, m_rectThumbnail.Width(),m_rectThumbnail.Height());
	////}
	////else
	////{
	////	float fLeftRightCoeff;//���ұ���
	////	INT nWidth;//���ο��
	////	INT nCenter;//�������ĵ�
	////	//x��
	////	int m_nPointLabel; //m_nSelectCursor:Seleced��־,1ѡ��A��2ѡ��B, 3ѡ��AB
	////	if (CURSOR_SELECT::CURSOR_A == MAIN->m_pWaveFormView->m_sorDrawStatic.m_nSelectCursor) //ѡ��A
	////	{
	////		m_nPointLabel = MAIN->m_pWaveFormView->m_sorDrawStatic.m_nPointA;
	////	}	
	////	else if (CURSOR_SELECT::CURSOR_B == MAIN->m_pWaveFormView->m_sorDrawStatic.m_nSelectCursor)
	////	{
	////		m_nPointLabel = MAIN->m_pWaveFormView->m_sorDrawStatic.m_nPointB;
	////	}
	////	else//AB
	////	{
	////		m_nPointLabel=(MAIN->m_pWaveFormView->m_sorDrawStatic.m_nPointA + MAIN->m_pWaveFormView->m_sorDrawStatic.m_nPointB)/2;
	////	}//eif
	////	//��ֹԽ��
	////	if (m_nPointLabel > pTestResultInfo->DataPointsNum)
	////	{
	////		m_nPointLabel = pTestResultInfo->DataPointsNum - 1;
	////	}
	////	int left = 0;//wqc2011.12.13
	////	int top  = 0;//wqc2011.12.13
	////	int right = m_rectThumbnail.Width();//wqc2011.12.13
	////	int bottom  = m_rectThumbnail.Height();//wqc2011.12.13
	////	if (0 == MAIN->m_pWaveFormView->m_sorDrawStatic.m_iHorizonZoomNum)
	////	{
	////		rectMove->left = left;//wqc2011.12.13
	////		rectMove->right = right;//wqc2011.12.13
	////	}
	////	else//ˮƽ������
	////	{
	////		fLeftRightCoeff = 0;//���ұ���
	////		nWidth = m_rectThumbnail.Width();//���ο��
	////		nCenter= nWidth / 2;//�������ĵ�

	////		fLeftRightCoeff = m_nPointLabel * 1.0 / pTestResultInfo->DataPointsNum;//����ʱ���ұ��������Ҳ�һ��
	////		nCenter = m_rectThumbnail.Width() * fLeftRightCoeff;//�������ĵ�
	////		nWidth = (MAIN->m_pWaveFormView->m_sorDrawStatic.m_nZoomCount - 
	////			MAIN->m_pWaveFormView->m_sorDrawStatic.m_iHorizonZoomNum) * 
	////			nWidth / MAIN->m_pWaveFormView->m_sorDrawStatic.m_nZoomCount;//���ο��
	////		rectMove->left = left + nCenter - nWidth * fLeftRightCoeff;//wqc2011.12.13
	////		if ( 0 == nWidth)//��ֹ�治��������
	////		{
	////			nWidth++;
	////			nWidth++;
	////		}
	////		rectMove->right = rectMove->left + nWidth;
	////	}

	////	//y��
	////	if (0 == MAIN->m_pWaveFormView->m_sorDrawStatic.m_iVerticalZoomNum)
	////	{
	////		rectMove->top = top;//wqc2011.12.13
	////		rectMove->bottom = bottom;//wqc2011.12.13
	////	}
	////	else
	////	{
	////		fLeftRightCoeff= (-pTestResultInfo->pDataPoints[m_nPointLabel]/1000.0 - 
	////			MAIN->m_pWaveFormView->m_sorDrawStatic.m_fStartDB) * 
	////			1.0/(MAIN->m_pWaveFormView->m_sorDrawStatic.m_fMaxDB - 
	////			MAIN->m_pWaveFormView->m_sorDrawStatic.m_fStartDB);//����ʱ���ұ��������Ҳ�һ��
	////		nCenter = m_rectThumbnail.Height() * (1 - fLeftRightCoeff);//fLeftRightCoeff;
	////		nWidth = (ZOOM_VERTICAL_COUNT - 
	////			MAIN->m_pWaveFormView->m_sorDrawStatic.m_iVerticalZoomNum)*m_rectThumbnail.Height()/ZOOM_VERTICAL_COUNT;//���θ߶�
	////		rectMove->top = top + nCenter - nWidth * (1 - fLeftRightCoeff);//wqc2011.12.13
	////		if ( 0 == nWidth)//��ֹ�治��������
	////		{
	////			nWidth++;
	////		}
	////		rectMove->bottom = rectMove->top + nWidth;
	////	}//eif
	////}////eif

	//////������ֱ�����ƽ��wcq2011.12.14
	//////int yOffset = (m_rectThumbnail.Height() / m_rectCoord.Height())*m_pMapCurveData[m_curABWave]->m_TestResult.YOffset;
	////int yOffset = (m_rectThumbnail.Height() * pTestResultInfo->YOffset / 
	////	MAIN->m_pWaveFormView->m_sorDrawStatic.m_rectCoord.Height());
	////rectMove->OffsetRect(0, yOffset);
}

/*
***************************************************************
��������:             DrawThumbCurve()                                                                               
����˵��:             ��������ͼ����                                                                              
��ڲ���:             CDC                                                                         
���ڲ���:             ��                                                                         
����ʱ��:             2012-04-02                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CThumbnail::DrawThumbCurve(CDC *pDC)
{	
	//��ǰѡ������
	TestResultInfo* pTestResultInfo = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;
	if (NULL == pTestResultInfo)
		return;

	int nSaveDC=pDC->SaveDC();
	unsigned int i;
	CPen penCurve;
	penCurve.CreatePen (PS_SOLID, 1, pTestResultInfo->curveColor);
	pDC->SelectObject(&penCurve);
	pDC->Polyline(pTestResultInfo->pThumbData, pTestResultInfo->ThumbDrawXCount);

	//int index = pTestResultInfo->ThumbDrawXCount > 0 ? pTestResultInfo->ThumbDrawXCount : 0;
	//if(index >0)
		//TRACE(_T("last point value = %d\n"), (*(pTestResultInfo->pThumbData+index-1)).y);

    ////////////////zll 2013.7.3 ���ž��ο��������ͼ������ ���Ի�һЩ
	CPen penThumbZoom;
	penThumbZoom.CreatePen (PS_SOLID, 1, RGB(0,255,0));
	pDC->SelectObject (&penThumbZoom);
	CRect rect;
	GetMoveRect(&rect, pTestResultInfo);//�õ����ž��ο�//2013.5.10 ��
	pDC->SelectStockObject(NULL_BRUSH);
	pDC->Rectangle(&rect);  
	////////////////

	//�ָ�
	pDC->RestoreDC(nSaveDC);
	penCurve.DeleteObject();
	penThumbZoom.DeleteObject();
}

/*
***************************************************************
��������:             GetAllThumbData
��ڲ���:             
���ڲ���:             
����ʱ��:             2011-12-14
Write By:             xjf
����˵��:             ���»�ȡ�������ߵ�����ͼ����
***************************************************************
*/
void CThumbnail::GetAllThumbData()
{
	//����
	for(int nCount = 0; nCount < g_sorFileArray.totalCurveNumber; nCount++)
	{
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
		if (!pCurve)
		{
			continue;
		}//eif
		GetThumbData(pCurve);
	}//efor
}

//ÿ�յ�һ�������ݣ������������ڻ�������ͼ�ĵ�.
void CThumbnail::GetThumbData(TestResultInfo* pTestResultInfo)
{
	if (pTestResultInfo->DataPointsNum < NUM_VALIDE_LEAST)//û�л����ݺ���
		return;

	if (pTestResultInfo->pThumbData)//����ͼ����
	{
		delete[] pTestResultInfo->pThumbData;
		pTestResultInfo->pThumbData = NULL;
	}
	pTestResultInfo->pThumbData = new CPoint[m_rectThumbnail.Width() * 2];//x��ѡ����

	float fXCoeff;//����x����ʱϵ��
	float fYConst;//����y����ʱ����Ĳ���
	float fYCoeff;//����y����ʱϵ��
	float width;//���߻��������ȵ�λ��px
	unsigned int nStartPoint = 0;
	unsigned int nMaxPoint = pTestResultInfo->DataPointsNum;//���е�
	//if (pTestResultInfo->IsMaxLength)//�Ǿ��������
	//{
		width = m_rectThumbnail.Width();
	//}
	//else/*���������*/
	//{
	//	width = m_rectThumbnail.Width() * pTestResultInfo->Length / pTestResultInfo->MaxTestDistance;
	//}

	float MaxDB,StartDB;
	//if (IsLowerStandardAll())//
	{
		StartDB = M_FSTARTDB0;
		MaxDB = M_FMAXDB0;
	}
/*	else
	{
		StartDB = M_FSTARTDB1;
		MaxDB = M_FMAXDB1;
	}*/
	int top,left;//,right,bottom;//��Ϊ����ͼ����ʱ���Ȼ��Ƶ�һ��λͼ����������Ͻ���Ϊ��ʼ��wcq2011.12.13
	left = 0;//wcq2011.12.13
	top = 0;//wcq2011.12.13

	fYConst = top + m_rectThumbnail.Height()+ ( StartDB/( MaxDB - StartDB ) )*m_rectThumbnail.Height();//����y����ʱ����Ĳ���//wcq2011.12.13
	//fYCoeff = m_rectThumbnail.Height() /((MaxDB - StartDB) * 1000);//����y����ʱϵ��
	//fYCoeff = m_rectThumbnail.Height()*9/10 /((MaxDB - StartDB) * 1000);//����y����ʱϵ�� zll 2013.6.19 ������С��9/10
	fYCoeff = m_rectThumbnail.Height() /((MaxDB - StartDB) * 1000);//����y����ʱϵ�� lzy2014.7.16


	//WORD * pword = pTestResultInfo->pDataPoints;
#ifdef COMPILE_SHOW_SMOOTH_DATA////��ʾʱ��ƽ���������ݣ�����ʱ��ʵ������
	WORD * pword = pTestResultInfo->pDataPointsSmooth;
#else
	WORD * pword = pTestResultInfo->pDataPoints;
#endif
	CPoint * pPoint = pTestResultInfo->pThumbData;
	float fStepCoeff = pTestResultInfo->DataPointsNum / width;
	int index = 0;
	int maxIndex, minIndex;//ȡ��ʱĳһ���ض�Ӧ�������Сֵ������������±�.wcq2011.09.29
	int wMax, wMin;//ĳһ���ض�Ӧ�������Сֵ���ֵ
	int step = 0;
	int lastStepPoint = 0;
	int maxLen = width;//m_rectThumbnail.Width();//
	int i;
	//int left = m_rectThumbnail.left;//����ǰ������ wcq2011.12.13
	pTestResultInfo->ThumbDrawXCount = maxLen * 2;//������ļ����޹أ���Ҫ�����ڻ�������ͼ��*2:ÿһ��ȡ����//wcq2011.12.08
	for(i=0; i< maxLen; i++)
	{
		maxIndex = lastStepPoint;//Ĭ��ÿһ���ص�һ��
		minIndex = lastStepPoint;//Ĭ��ÿһ���ص�һ��
		wMax = pword[lastStepPoint];//Ĭ��ÿһ���ص�һ��
		wMin = pword[lastStepPoint];//Ĭ��ÿһ���ص�һ��
		step =(i+1)*fStepCoeff;//���ᳬ���������??????
		//DEBUGMSG(true,(_T("step:%d\n"),step));
		for(int j=lastStepPoint+1;j<step;j++)
		{
			if (j==nMaxPoint)
			{
				break;
			}
			if (pword[j] > wMax)
			{
				wMax = pword[j];
				maxIndex=j;
			}
			else if (pword[j] < wMin)
			{
				wMin = pword[j];
				minIndex=j;
			}
		}
		if (minIndex < maxIndex)//�����±��Ⱥ���Ӧ�����Сֵ��ӵ�����
		{
			//����
			pPoint[index].x = i + left;//m_rectThumbnail.left + maxIndex*fXCoeff;//wcq2011.12.13
			pPoint[index].y = fYConst+pword[minIndex]*fYCoeff;//wcq2011.12.13
			index++;
			//��С��
			pPoint[index].x = i + left;//m_rectThumbnail.left + minIndex*fXCoeff;
			pPoint[index].y = fYConst+pword[maxIndex]*fYCoeff;//wcq2011.12.13
		}
		else
		{//wcq2011.12.13
			//����
			pPoint[index].x = i + left;//m_rectThumbnail.left + maxIndex*fXCoeff;//wcq2011.12.13
			pPoint[index].y = fYConst+pword[maxIndex]*fYCoeff;//wcq2011.12.13
			index++;
			//��С��
			pPoint[index].x = i + left;//m_rectThumbnail.left + minIndex*fXCoeff;
			pPoint[index].y = fYConst+pword[minIndex]*fYCoeff;//wcq2011.12.13

		}
		index++;
		//
		lastStepPoint= step;//���汾�ε����һ��
	}//endfor
}
void CThumbnail::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	//���¼����ͼ����
	GetClientRect(&m_rectThumbnail);
	//���»�ȡthumbnail����
	GetAllThumbData();
}
