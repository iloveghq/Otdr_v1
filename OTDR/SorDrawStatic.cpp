// SorDrawStatic.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <math.h>
#include "SorDrawStatic.h"
#include "MainFrm.h"
#include "MyFont.h"
#include "EndGabor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ZOOM_VERTICAL_STANDARD 30*1000//wcq2011.09.26������ֱ��������ʱȷ��������̣����ڸ�ֵʱΪ-65~30,С��ʱΪ-55��40
#define Y_StartEndPoint 12//zll2012.8.15 ���¼������ڵ����ߵ����������¾���
#define X_StartEndPoint 5//zll2012.8.15 ���¼������ڵ������µķ������ں����������
#define CursorAB_Coefficient 1000.0//ϵ�� zll 2013.3.15
const unsigned C_zll = 299792458 ; //����еĹ���m

int g_nMaxPointFromFile=0;  //���ļ���ȡ������������
int g_iMaxFlag=-1;  //���������е�������,�����еľ��볤��0���ļ��еĴ���1

// CSorDrawStatic �Ի���
extern SorFileArray g_sorFileArray;

IMPLEMENT_DYNAMIC(CSorDrawStatic, CButton)

CSorDrawStatic::CSorDrawStatic()
{
	m_nStartPoint=0;
	m_nMaxPoint= INVALID_MAX_POINT;//ԭΪ0,wcq2012.02.20
	m_fStartDB=M_FSTARTDB0;
	m_fMaxDB=M_FMAXDB0;
	m_ptRecord=NULL;
	m_bReceiveResultOK=FALSE;
	m_iHorizonZoomNum=0; //��ʼ��Ϊ0����ʾ��ˮƽ�Ŵ���С����
	m_iVerticalZoomNum=0; //��ʼ��Ϊ0����ʾ������Ŵ����
	m_bReadFromFile=FALSE;
	m_nStartPointFromFile=0;
	m_nMaxPointFromFile=0;
	m_iCurveZoomMode = 0; //��������
	m_iTestModeForFile=0;
	m_nSelectCursor = CURSOR_SELECT::CURSOR_A; //Ĭ��ѡ��A
	m_nSelectIndex = 0;
	m_pRecvbuf = NULL;
	m_bEventCursor = FALSE;
	m_dwWaveTesting = 0;
	m_dwStartTestTime = 0;

	m_bBusy = FALSE;
	m_dwMaxPointCount=1;

	m_dwWaveSelected = INVALID_WAVELENGTH;
	m_nZoomCount=0;//���Ϊ0��δ���й��������Ŵ�������
	m_nWaveToDevice = 1550;//wcq2011.12.19
	m_nStopTestNormal = TRUE;
	m_tempInt=0;
	m_lastPageType = -1;//wcq2012.02.14
	m_bNeedHorizonZoom = FALSE;//�Ƿ���Ҫ�������ţ������������������� wcq2012.02.17
	m_lastWaveLength = 0;//��һ�β���
	m_lastDataLength = 0;//��һ�β��Ե���
	m_lastRefraction = 0;//��һ�β��Ե�������
	m_bFirstRead = TRUE;
	m_bZoomOK = FALSE; //AB���Ƿ�����϶�

	m_nMaxIndex = 0;
	m_fRefraction = 1.465f;
	m_cursorPoint.x = 0;
	m_cursorPoint.y = 0;
	m_nPointA = 0;
	m_nPointB = 0;
	m_fCoeff = 1.0;
	// initial scroll position
	m_nScrollPosVer = 0; 
	m_nScrollPosHor = 0; 
	// original scroll size
	m_nDrawHeight = 0;
	m_nDrawWidth = 0;

	m_nCurHeight = 0;
	m_nCurWidth = 0;

	m_RectSelected.SetRectEmpty();
	m_PLMouseDown.x = 0;
	m_PLMouseDown.y = 0;
}

CSorDrawStatic::~CSorDrawStatic()
{
}

BEGIN_MESSAGE_MAP(CSorDrawStatic, CButton)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void CSorDrawStatic::SetPageTitle()
{

}

//����ҳ���ı�
void CSorDrawStatic::SetCurPageText()
{
}

void CSorDrawStatic::AddEvent()
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
void CSorDrawStatic::GaborResultToEvent(TestResultInfo* pCurveData)
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



//���ļ���ȡ�¼��б��ת��Ϊʹ�õ�GaborResult�ṹ�� wcq2012.06.11
void CSorDrawStatic::EventToGaborResult(TestResultInfo* pCurveData)
{
	EventInfo eventinfo;
	int len = pCurveData->EventList.vEventInfo.size();//wcq2012.06.07
	if ((len < 0)  || (len > OTDR_EVENT_NUM_MAX))//��ֹ�������� wcq2012.07.16
	{
		return;
	}//eif
	pCurveData->m_GaborResult.Valid_Num = len;
	float fDisTwo=Get_XCoord(1,pCurveData);
	//float ddd=0;//zll 2012.12.26ע��
	for(int i=0;i<len;i++)
	{
		eventinfo = pCurveData->EventList.vEventInfo[i];
		//	pCurveData->m_GaborResult.Event_Array[i] = eventinfo.index;//�±꣬�ڼ���//???wcq2012.07.09

		//pCurveData->m_GaborResult.Reflect_Type[i] = !eventinfo.nEventType;//�¼�����//��׼����3�֣�����ֻ��ʾ���䡢�Ƿ�������cwcq2012.06.12//cwcq2012.07.09
		pCurveData->m_GaborResult.Reflect_Type[i] = eventinfo.nEventType;//�¼�����//��׼����3�֣�����ֻ��ʾ���䡢�Ƿ�������cwcq2012.06.12//wcq2012.07.09
		pCurveData->m_GaborResult.Event_Array[i] = eventinfo.index;//Get_XCoord(pCurveData->m_GaborResult..Event_Array[i],pCurveData);//�������룬��kmΪ��λ

		//�������� zll 2012.12.26ע��
		/*ddd=eventinfo.fLocation/fDisTwo;
		pCurveData->m_GaborResult.Event_Array[i]=ddd;*/

		pCurveData->m_GaborResult.Event_Loss[i] = eventinfo.fEventloss;
		pCurveData->m_GaborResult.Event_Hight[i] = eventinfo.fEventHigh ;
		//pCurveData->m_GaborResult.Line_k[i] =  fDisTwo * eventinfo.fLineK;
		pCurveData->m_GaborResult.Line_k[i] =  eventinfo.fLineK;//wcq2012.06.29 ?????
		pCurveData->m_GaborResult.Total_Loss[i] = eventinfo.fTotalLoss;
		pCurveData->m_GaborResult.Reflect = eventinfo.fReflect;
		pCurveData->m_GaborResult.Trail_End = eventinfo.fTrailEnd;
		pCurveData->m_GaborResult.Event_type[i] = eventinfo.cEventType;//wcq2012.07.09
	}//efor
}

void CSorDrawStatic::RemoveEvent()
{
	TestResultInfo* pCurveData = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;
	POSITION pos = MAIN->m_pEventFormView->m_Eventlist.GetFirstSelectedItemPosition();
	int nIndex = MAIN->m_pEventFormView->m_Eventlist.GetNextSelectedItem(pos);
	if ((nIndex < 0) || (nIndex > pCurveData->m_GaborResult.Valid_Num))
	{
		return;
	}
	int waveNum = (1550 == pCurveData->WaveLength) ? 1:0;
	__try{
		pCurveData->m_GaborResult = m_endgabor.deleteEvent(nIndex,
												waveNum,
												pCurveData->PulseWidth,
												pCurveData->pDataPoints,
												pCurveData->m_GaborResult,
												pCurveData->Refraction,//������zll
												pCurveData->Frequency,
												0.0f);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return;
	}
	GaborResultToEvent(pCurveData);////����dll�õ����ݺ�ת��Ϊ�¼��б� wcq2012.06.08
	MAIN->m_pEventFormView->FreshEventList();//ˢ�½����¼��б� 
	//
	nIndex--;
	if (nIndex < 0)
	{
		nIndex = 0;
	}
	if (MAIN->m_pEventFormView->m_Eventlist.GetItemCount() >= nIndex)
	{
		MAIN->m_pEventFormView->m_Eventlist.SetItemState(nIndex, LVIS_SELECTED, LVIS_SELECTED);
	}
	//EventCursor();//�ػ�
	InvalidateAll();// wcq2012.06.08
}

//���²��Ժ��ʼ��һЩ��������A/B��λ�� wcq2012.02.21
void CSorDrawStatic::NewTestInit(TestResultInfo* pData)
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

BOOL CSorDrawStatic::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CSorDrawStatic::OnPaint() 
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

	CalCulDispRect();
	DrawCoord(&MemDC);
	DrawCurves(&MemDC);
	//DrawLineAB(&MemDC);
	DrawCursor(&MemDC);

	//�ѻ��ƺõ�ͼ����BitBlt()"����"����Ļ��
	dc.BitBlt(0,0,ClientRect.Width()*2,ClientRect.Height() * 2,&MemDC, 0, 0,SRCCOPY); 
	MemDC.SelectObject(pOldBitMap);
	MemBitMap.DeleteObject();
	MemDC.DeleteDC();
}

//����ƶ�ʱ��Ҫ�ػ�����,�ú���ֻȷ�����ұ߽硣//wcq2011.12.26
void CSorDrawStatic::GetCursorRect(CRect* rect)//����ƶ�ʱ��Ҫ�ػ�����//wcq2011.12.26
{
	POINT ptA,ptB, ptEvent;
	int nXCoord=0;
	int nHalfWidth = m_rectCoord.right / 2;
	int lineWidth = WIDTH_CURSOR_LEFT;

	if (m_nSelectCursor == CURSOR_SELECT::CURSOR_A)//ѡ��A
	{
		nXCoord = int(m_rectCoord.left + m_rectCoord.Width () * (m_nPointA - m_nStartPoint) * 1.0/(m_nMaxPoint-m_nStartPoint));
		if (nXCoord < nHalfWidth)
		{
			rect->left = nXCoord - lineWidth*2;//���߱�����
			rect->right = nXCoord + WIDTH_CURSOR_INVALID;//����m �ı�
		}
		else if (nXCoord > nHalfWidth)
		{
			rect->right = nXCoord + lineWidth;
			rect->left = nXCoord - WIDTH_CURSOR_INVALID;
		}
		else//=�м�ʱ����Ч����Ϊ����
		{
			rect->right = nXCoord + WIDTH_CURSOR_INVALID + lineWidth + lineWidth;
			rect->left = nXCoord - WIDTH_CURSOR_INVALID;
		}
	}
	else if (m_nSelectCursor == CURSOR_SELECT::CURSOR_B) //ѡ��B
	{
		nXCoord = int(m_rectCoord.left + m_rectCoord.Width () * (m_nPointB - m_nStartPoint) * 1.0/(m_nMaxPoint-m_nStartPoint));
		if (nXCoord < nHalfWidth)
		{
			rect->left = nXCoord - lineWidth*2;//���߱�����
			rect->right = nXCoord + WIDTH_CURSOR_INVALID;//����m �ı�
		}
		else if (nXCoord > nHalfWidth)
		{
			rect->right = nXCoord + lineWidth;
			rect->left = nXCoord - WIDTH_CURSOR_INVALID;
		}
		else//=�м�ʱ����Ч����Ϊ����
		{
			rect->right = nXCoord + WIDTH_CURSOR_INVALID ;
			rect->left = nXCoord - WIDTH_CURSOR_INVALID - lineWidth*3;
		}
	}
	else if (m_nSelectCursor == CURSOR_SELECT::CURSOR_AB) //ѡ��AB
	{
		nXCoord = int(m_rectCoord.left + m_rectCoord.Width () * (m_nPointA - m_nStartPoint) * 1.0/(m_nMaxPoint-m_nStartPoint));
		int nXCoordB = int(m_rectCoord.left + m_rectCoord.Width () * (m_nPointB - m_nStartPoint) * 1.0/(m_nMaxPoint-m_nStartPoint));
		if (nXCoord < nXCoordB)//A��B���
		{
			if (nXCoord < nHalfWidth)//�ı����ұ�
			{
				rect->left = nXCoord - lineWidth;
			}
			else//�ı������
			{
				rect->left = nXCoord - WIDTH_CURSOR_INVALID;
			}//eif
			if (nXCoordB < nHalfWidth)//B,�ı����ұ�
			{
				rect->right = nXCoordB + WIDTH_CURSOR_INVALID +lineWidth;
			}
			else//�ı������
			{
				rect->right = nXCoordB +lineWidth;

			}//eif
		}
		else//B��A��
		{
			if (nXCoordB < nHalfWidth)//�ı����ұ�
			{
				rect->left = nXCoordB - lineWidth;
			}
			else//�ı������
			{
				rect->left = nXCoordB - WIDTH_CURSOR_INVALID;
			}//eif
			if (nXCoord < nHalfWidth)//A,�ı����ұ�
			{
				rect->right = nXCoord + WIDTH_CURSOR_INVALID;
			}
			else//�ı������
			{
				rect->right = nXCoord + lineWidth;

			}//eif
		}//eif //ѡ��AB
	}//eif ѡ��
}

//ˢ�»�ͼ������ͼ����������ʾ��,�������
void CSorDrawStatic::InvalidateAll()
{
	InvalidateRect(m_rectCurve,FALSE);//����
}

void CSorDrawStatic::DrawCursor(CDC *pDC)
{
	TestResultInfo* pCurveData = GetMaxLengthCurveData();

	int nSaveDC = pDC->SaveDC();

	POINT ptA,ptB, ptEvent;
	float nDistanceA;
	CString strTextA;

	float nDistanceB;
	CString strTextB;

	if(pCurveData)//�������ж��Ƿ��Ѿ����Ի���ع�����
	{
		ptA.x= int(m_rectCoord.left + m_rectCoord.Width () * (m_nPointA - m_nStartPoint) * 1.0/(m_nMaxPoint-m_nStartPoint));
		ptB.x= int(m_rectCoord.left + m_rectCoord.Width () * (m_nPointB - m_nStartPoint) * 1.0/(m_nMaxPoint-m_nStartPoint));

		nDistanceA=Get_XCoord(m_nPointA, pCurveData);
		nDistanceB=Get_XCoord(m_nPointB, pCurveData);

		if (ptA.x>=m_rectCoord.left && ptA.x<=m_rectCoord.right)
		{
			if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
			{
				strTextA.Format(_T("%.3f"),nDistanceA/CursorAB_Coefficient);
				strTextA=_T("A ")+strTextA+_T("km");
			}
			else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪmiles
			{
				nDistanceA=KmeterToOther(nDistanceA,UNIT_SHOW_MILE);
				strTextA.Format(_T("%.3f"),nDistanceA/CursorAB_Coefficient);
				strTextA=_T("A ")+strTextA+_T("miles");
			}
			else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkft
			{
				nDistanceA=KmeterToOther(nDistanceA,UNIT_SHOW_KFEET);
				strTextA.Format(_T("%.3f"),nDistanceA/CursorAB_Coefficient);
				strTextA=_T("A ")+strTextA+_T("kft");
			}
		}
		if (ptB.x>=m_rectCoord.left && ptB.x<=m_rectCoord.right)
		{
			if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
			{
				strTextB.Format(_T("%.3f"),nDistanceB/CursorAB_Coefficient);
				strTextB=_T("B ") +strTextB+ _T("km");
			}
			else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪmiles
			{
				nDistanceB=KmeterToOther(nDistanceB,UNIT_SHOW_MILE);
				strTextB.Format(_T("%.3f"),nDistanceB/CursorAB_Coefficient);
				strTextB=_T("B ") +strTextB+ _T("miles");
			}
			else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkft
			{
				nDistanceB=KmeterToOther(nDistanceB,UNIT_SHOW_KFEET);
				strTextB.Format(_T("%.3f"),nDistanceB/CursorAB_Coefficient);
				strTextB=_T("B ") +strTextB+ _T("kft");
			}
		}
	}
	else //��û�вɼ������ݵ����
	{
		ptA.x=m_rectCoord.left;
		ptB.x=m_rectCoord.left+m_rectCoord.Width()/5;

		nDistanceA=0;
		nDistanceB=60/5;
		//nDistanceB=60000/5;//zll 2012.10.30ע��

		if (ptA.x>=m_rectCoord.left && ptA.x<=m_rectCoord.right)
		{
			if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
			{
				strTextA.Format(_T("%.3f"),nDistanceA/CursorAB_Coefficient);
				strTextA=_T("A ")+strTextA+ _T("km");
			}
			else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪmiles
			{
				nDistanceA=KmeterToOther(nDistanceA,UNIT_SHOW_MILE);
				strTextA.Format(_T("%.3f"),nDistanceA/CursorAB_Coefficient);
				strTextA=_T("A ")+strTextA+ _T("miles");
			}
			else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkft
			{
				nDistanceA=KmeterToOther(nDistanceA,UNIT_SHOW_KFEET);
				strTextA.Format(_T("%.3f"),nDistanceA/CursorAB_Coefficient);
				strTextA=_T("A ")+strTextA+ _T("kft");
			}
		}
		if (ptB.x>=m_rectCoord.left && ptB.x<=m_rectCoord.right)
		{
			if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
			{
				strTextB.Format(_T("%.3f"),nDistanceB/CursorAB_Coefficient);
				strTextB= _T("B ")+strTextB+ _T("km");
			}
			else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪmiles
			{
				nDistanceB=KmeterToOther(nDistanceB,UNIT_SHOW_MILE);
				strTextB.Format(_T("%.3f"),nDistanceB/CursorAB_Coefficient);
				strTextB= _T("B ")+strTextB+ _T("miles");
			}
			else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkft
			{
				nDistanceB=KmeterToOther(nDistanceB,UNIT_SHOW_KFEET);
				strTextB.Format(_T("%.3f"),nDistanceB/CursorAB_Coefficient);
				strTextB= _T("B ")+strTextB+ _T("kft");
			}
		}
	}

	CPen penSelect;
	penSelect.CreatePen (PS_SOLID, 2, RGB(255,0,0));

	CPen penUnselect;
	penUnselect.CreatePen (PS_SOLID,2,RGB(255,255,255));

	CPen* pOldPen;//wcq2011.12.08
	if (m_nSelectCursor == CURSOR_SELECT::CURSOR_A)//ѡ��A
	{
		pOldPen = pDC->SelectObject(&penSelect);
		if (ptA.x>=m_rectCoord.left && ptA.x<=m_rectCoord.right)
		{
			pDC->MoveTo(ptA.x,m_rectCoord.bottom);
			pDC->LineTo(ptA.x,m_rectCoord.top+14);
		}
		pDC->SelectObject(&penUnselect);
		if (ptB.x>=m_rectCoord.left && ptB.x<=m_rectCoord.right)
		{
			pDC->MoveTo(ptB.x,m_rectCoord.bottom);
			pDC->LineTo(ptB.x,m_rectCoord.top+34);
		}
	}
	else if (m_nSelectCursor == CURSOR_SELECT::CURSOR_B) //ѡ��B
	{
		pOldPen = pDC->SelectObject(&penUnselect);
		if (ptA.x>=m_rectCoord.left && ptA.x<=m_rectCoord.right)
		{
			pDC->MoveTo(ptA.x,m_rectCoord.bottom);
			pDC->LineTo(ptA.x,m_rectCoord.top+14);
		}
		pDC->SelectObject(&penSelect);
		if (ptB.x>=m_rectCoord.left && ptB.x<=m_rectCoord.right)
		{
			pDC->MoveTo(ptB.x,m_rectCoord.bottom);
			pDC->LineTo(ptB.x,m_rectCoord.top+34);
		}
	}
	else if (m_nSelectCursor == CURSOR_SELECT::CURSOR_AB) //ѡ��AB
	{
		pOldPen = pDC->SelectObject(&penSelect);
		if (ptA.x>=m_rectCoord.left && ptA.x<=m_rectCoord.right)
		{
			pDC->MoveTo(ptA.x,m_rectCoord.bottom);
			pDC->LineTo(ptA.x,m_rectCoord.top+14);
		}

		if (ptB.x>=m_rectCoord.left && ptB.x<=m_rectCoord.right)
		{
			pDC->MoveTo(ptB.x,m_rectCoord.bottom);
			pDC->LineTo(ptB.x,m_rectCoord.top+34);
		}
	}
	else if (m_bEventCursor) //�¼����
	{
		pOldPen = pDC->SelectObject(&penSelect);
		if (ptEvent.x>=m_rectCoord.left && ptEvent.x<=m_rectCoord.right)
		{
			pDC->MoveTo(ptEvent.x,m_rectCoord.bottom);
			pDC->LineTo(ptEvent.x,m_rectCoord.top+14);
		}
	}

	CRect rtA,rtB, rtEvent;
	/////////////////////��A,B�߳�������ʱ������ʱ������ο�����ߣ��������ұ�	
	if(ptA.x < m_rectCoord.right / 2)
	{
		rtA.left=ptA.x;
		rtA.top=m_rectCoord.top;
		rtA.right=ptA.x+100;
		rtA.bottom=m_rectCoord.top+14;
	}
	else
	{
		rtA.left=ptA.x-100;
		rtA.top=m_rectCoord.top;
		rtA.right=ptA.x;
		rtA.bottom=m_rectCoord.top+14;
	}
	if(ptB.x < m_rectCoord.right / 2)
	{
		rtB.left=ptB.x;
		rtB.top=m_rectCoord.top+20;
		rtB.right=ptB.x+100;
		rtB.bottom=m_rectCoord.top+34;
	}
	else
	{
		rtB.left=ptB.x-100;
		rtB.top=m_rectCoord.top+20;
		rtB.right=ptB.x;
		rtB.bottom=m_rectCoord.top+34;
	}

	if(m_nSelectCursor == CURSOR_SELECT::CURSOR_A)//ѡ��A
	{
		pDC->SetTextColor (RGB(0,0,255));//no select cursor blue
		pDC->SetBkMode(TRANSPARENT);
		pDC->ExtTextOut(rtB.left+2,m_rectCoord.top+20,ETO_CLIPPED,rtB,strTextB,NULL);
		pDC->SetTextColor (RGB(255,0,0)); //select cursor red
		pDC->ExtTextOut(rtA.left+2,m_rectCoord.top,ETO_CLIPPED,rtA,strTextA,NULL);
	}
	else if(m_nSelectCursor == CURSOR_SELECT::CURSOR_B)//ѡ��B
	{
		pDC->SetTextColor (RGB(0,0,255)); //no select cursor blue
		pDC->SetBkMode(TRANSPARENT);
		pDC->ExtTextOut(rtA.left+2,m_rectCoord.top,ETO_CLIPPED,rtA,strTextA,NULL);
		pDC->SetTextColor (RGB(255,0,0));
		pDC->ExtTextOut(rtB.left+2,m_rectCoord.top+20,ETO_CLIPPED,rtB,strTextB,NULL);
	}
	else if(m_nSelectCursor == CURSOR_SELECT::CURSOR_AB)//ѡ��AB
	{
		pDC->SetTextColor (RGB(255,0,0));
		pDC->SetBkMode(TRANSPARENT);
		pDC->ExtTextOut(rtA.left+2,m_rectCoord.top,ETO_CLIPPED,rtA,strTextA,NULL);
		pDC->ExtTextOut(rtB.left+2,m_rectCoord.top+20,ETO_CLIPPED,rtB,strTextB,NULL);
	}
	//�ͷ���Դ
	pDC->RestoreDC(nSaveDC);//wcq2011.12.08
	penSelect.DeleteObject();
	penUnselect.DeleteObject();
}

void CSorDrawStatic::DrawCurves(CDC *pDC) //������������
{
	int SingleorMulti=0;//�浥������
	//�ж��Ƿ�Ϊ�����߻���ģʽ
	if (!g_sorFileArray.waveConfig.MultiMode)
	{
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;
		DrawCurve(pDC, pCurve,SingleorMulti);
		return;
	}

	//�����߻���ģʽ
	for(int nCount = 0; nCount < 8; nCount++)
	{
		//
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
		SingleorMulti=nCount;//�浥������
		if (pCurve)
		{
			DrawCurve(pDC, pCurve,SingleorMulti);
		}//eif
	}//efor
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
void CSorDrawStatic::CalMaxDistance()
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
void CSorDrawStatic::DrawCurve(CDC *pDC, TestResultInfo* pData,int SingleorMulti)
{
	if (pData == NULL ||
		pData->DataPointsNum < NUM_VALIDE_LEAST ||
		pData->pDataPoints == NULL ||
		CLEAR_DATAPOINTS_NUM == pData->DataPointsNum)//û�л����ݺ���wcq2011.09.09 //0 == m_pMapCurveData[m_curABWave]->m_TestResult.DataPointsNum.�ز�ʱ������//wcq2011.12.22
		return;


	int nSaveDC=pDC->SaveDC();
	unsigned int i;
	CPen penCurve;
	penCurve.CreatePen (PS_SOLID, 1, pData->curveColor);

	CBrush brushEventPoint;
	brushEventPoint.CreateSolidBrush(CLR_YELLOW);  //�¼����ˢ����ɫ

	//ת��Ϊ��ͼ��������
	float fXConst;//����x����ʱ����Ĳ���
	float fXCoeff;//����x����ʱϵ��
	//double fXCoeff;//����x����ʱϵ��wcq2011.09.23
	float fYConst;//����y����ʱ����Ĳ���
	float fYCoeff;//����y����ʱϵ��
	float width;//���߻��������ȵ�λ��px
	unsigned int nStartPoint = pData->StartPoint;
	unsigned int nMaxPoint = pData->MaxPoint;

	if(g_sorFileArray.waveConfig.MultiMode && !pData->IsMaxLength)/*������������Ƕ༣����ʾ*/
	{
		if(0 == m_iHorizonZoomNum) 
		{
			//��ȼ��� w=( (��ǰ���߳���km/������߳���km)*��ͼ������px )*(��ͼ����/�ܹ�����)
			width = ( pData->Length / pData->MaxTestDistance ) * m_rectCoord.Width();
		}
		else
		{
			//���ݾ���ԭ��һ�� wcq2012.02.22
			width=( ((pData->MaxPoint - pData->StartPoint) * pData->DataSpacing )/ ( (m_nMaxPoint - m_nStartPoint) * pData->DataSpacing ) ) * m_rectCoord.Width();//
		}
	}
	else// if (pData->IsMaxLength)//���������,���ߵ�������ʾ
	{
		width = m_rectCoord.Width();
	}
	int left = 0,top = 0,right,bottom;//����
	right = m_rectCoord.Width();
	bottom = m_rectCoord.Height();
	//fYConst=bottom - ((-m_fStartDB)/(m_fMaxDB - m_fStartDB)) * m_rectCoord.Height() - pData->YOffset;//����y����ƫ��
	//ת��Ϊ��ͼ��������
	for(i = pData->StartPoint; i < pData->MaxPoint; i++)
	{
		pData->ptDrawData[i].x = int(m_rectCoord.left + width * (i - pData->StartPoint)/(pData->MaxPoint - pData->StartPoint));
		pData->ptDrawData[i].y = int(m_rectCoord.bottom - pData->YOffset - (((-pData->pDataPoints[i]/1000.0) - m_fStartDB)/(m_fMaxDB - m_fStartDB)) * m_rectCoord.Height());

		if(pData->ptDrawData[i].x > m_rectCoord.right)
			pData->ptDrawData[i].x = m_rectCoord.right;
		if(pData->ptDrawData[i].y < m_rectCoord.top)
			pData->ptDrawData[i].y = m_rectCoord.top;
		if(pData->ptDrawData[i].y > m_rectCoord.bottom)
			pData->ptDrawData[i].y = m_rectCoord.bottom; 
	}

	TRACE(_T("Dis:%d\n"),pData->MaxPoint-pData->StartPoint);
	//������
	pDC->SelectObject (&penCurve);
	pDC->Polyline(&pData->ptDrawData[pData->StartPoint], pData->MaxPoint - pData->StartPoint);

	//���¼���
	if (g_sorFileArray.totalCurveNumber-1==SingleorMulti)
	{
		pDC->SelectObject(&brushEventPoint);
		int nEventNum = pData->EventList.vEventInfo.size();
		for(i=0;i<nEventNum;i++)
		{
			CPen m_penBack;
			m_penBack.CreatePen (PS_SOLID,0,CLR_YELLOW);
			CPen *pOldPen = pDC->SelectObject (&m_penBack);
			EventInfo eventInfo = pData->EventList.vEventInfo[i];
			CRect rectLineBottom;//rectLineBottom:ֱ���·��ľ���wcq2011.11.16
			CString temp;
			pDC->SetBkMode(TRANSPARENT);
			if(eventInfo.index<m_nMaxPoint && eventInfo.index>=m_nStartPoint)
			{
				int nLocaltion = eventInfo.index;

				//��ֱϸ��
				pDC->MoveTo(pData->ptDrawData[nLocaltion].x,pData->ptDrawData[nLocaltion].y-Y_StartEndPoint);
				pDC->LineTo(pData->ptDrawData[nLocaltion].x,pData->ptDrawData[nLocaltion].y+Y_StartEndPoint);

				//�·�����
				rectLineBottom.SetRect(pData->ptDrawData[nLocaltion].x-X_StartEndPoint,pData->ptDrawData[nLocaltion].y+Y_StartEndPoint,pData->ptDrawData[nLocaltion].x-X_StartEndPoint + 18,pData->ptDrawData[nLocaltion].y+Y_StartEndPoint + 18);

				pDC->FillSolidRect(&rectLineBottom,CLR_YELLOW);

				if (i>8)//��λ��
				{
					rectLineBottom.right += 2;
				}
				pDC->Rectangle(rectLineBottom);//������ο�
				//rectLineBottom.left -=6;
				//rectLineBottom.right +=6;
				//rectTemp.OffsetRect(2,0);//��þ���Ч��
				temp.Format(_T("%d"),i + 1);
				pDC->DrawText(temp,&rectLineBottom,DT_CENTER | DT_VCENTER | DT_SINGLELINE);//���ο��е���ֵ

				pDC->SelectObject (pOldPen);
				m_penBack.DeleteObject();
			}
		}
	}

	//�ָ�
	pDC->RestoreDC(nSaveDC);
	//�ͷ���Դ
	penCurve.DeleteObject();
	brushEventPoint.DeleteObject();
}

////wcq2011.09.06�õ�����������ľ���,��kmΪ��λ
float CSorDrawStatic::GetMaxDistance()
{
	//��ʱֻ����һ�����ߣ��������������Ҫ����Ӧ�޸�
	DWORD dwWave = _wtoi(_T("1310"));
	TestResultInfo* pCurveData = g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo;

	float fResult=0;
	fResult=Get_XCoord(pCurveData->DataPointsNum, pCurveData);//>=Get_XCoordForFile(g_nMaxPointFromFile)
	return fResult;
}
//////////////////////////////////////////////////////////////////////////////////////////////
//��ڲ���:curveWidth:��ǰ���߻�ͼʱ������ˮƽ�����ϵĿ��,��Ϊ�����ʱ����λ������;yOffset��y�᷽���ϵ�ƫ�ƣ�yMin:y��������Сֵ;yMax:y���������ֵ;
//			rectCurve��ͼ����pData:����ָ�룬��һ����0��ʼ������ʱ�ɼ�ƫ�ƣ�dataCount�����ݵ����
//���ڲ���:
//����ʱ��:2011-09-05
//Write By:wcq
//����˵��:�������wcq 2011.09.6
//�㷨��xλ��=x��+(x-xMin)*���/(xMax-xMin)
//      yλ��=y��+(y-yMin)*�߶�/(yMax-yMin)
//      ���������Ҫ������һЩת����x��=0,��ˮƽ�����ƶ���xMin=0,����ԭ�㿪ʼ,��ƫ��
//////////////////////////////////////////////////////////////////////////////////////////////
void CSorDrawStatic::CalDrawCurveParams(int curveWidth,CRect rectCurve,int dataCount)
{

}

void CSorDrawStatic::DrawCoord(CDC *pDC)
{
	//��䱳��
	pDC->FillSolidRect(&m_rectCoord,g_sorFileArray.waveConfig.ColorBack);  //�������ڵľ�������

	TestResultInfo* pCurveData = GetMaxLengthCurveData();
	int nSaveDC = pDC->SaveDC();

	CPen penGridLine,penCoord;
	penCoord.CreatePen (PS_SOLID/*ʵ��*/,1, g_sorFileArray.waveConfig.ColorCursor);
	penGridLine.CreatePen (PS_DOT/*����*/,1, g_sorFileArray.waveConfig.ColorGrid);
	pDC->SelectObject (&penCoord);
	pDC->SelectObject(GetStockObject(NULL_BRUSH));
	if (!m_RectSelected.IsRectEmpty()) 
	{
		pDC->Rectangle(&m_RectSelected);
	}

	pDC->SelectObject (&penGridLine);

	if (g_sorFileArray.waveConfig.ShowGrid)
	{
		//������̶�
		int ndx = m_rectCoord.Width () / COOR_SPLIT_COUNT_H;
		int ndy = m_rectCoord.Height () / COOR_SPLIT_COUNT_V;
		pDC->SelectObject (&penGridLine);
		for ( int i = 1; i< COOR_SPLIT_COUNT_H; i++)
		{	
			//������̶�
			pDC->MoveTo(m_rectCoord.left + ndx * i,m_rectCoord.bottom );
			pDC->LineTo(m_rectCoord.left + ndx * i,m_rectCoord.top );
		}

		//������̶�
		pDC->SetTextAlign(TA_LEFT | TA_TOP);
		for (int i=1; i<COOR_SPLIT_COUNT_V; i++)
		{
			pDC->MoveTo(m_rectCoord.left , m_rectCoord.bottom - ndy * i);
			pDC->LineTo(m_rectCoord.right, m_rectCoord.bottom - ndy * i);
		}
	}

	//�ָ�
	pDC->RestoreDC(nSaveDC);
	//�ͷ���Դ
	penGridLine.DeleteObject();
	penCoord.DeleteObject();
}

/*
***************************************************************
��������:             Get_Xcoord()                                                                               
����˵��:             ���ɼ�������ת�������ߵĺ������                                                                              
��ڲ���:             xpoint�����ݵ�����                                                                                             
���ڲ���:             X��ת����ĺ������   ��λ  m                                                                                          
����ʱ��:             2010-06-24                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
float CSorDrawStatic::Get_XCoord(int xpoint)
{
	//1.4685�ǹ��������ʵ�ȱʡֵ��������

	float X=(float)(C_zll/(2*m_fFrequency*m_fRefraction * 1000000/*MHz*/)*xpoint);
	return X;
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
float CSorDrawStatic::Get_XCoord(unsigned int xpoint,TestResultInfo* pData)
{
	//return xpoint * pData->DataSpacing * 1.0/100000000;//wcq2011.11.25
	//return C/(5000 * pData->Refraction * 1000000/*MHz*/) * xpoint
	return C_zll/( pData->Refraction)/pData->Frequency/1000000/2* xpoint;//2012.8.3 rzg�޸Ĺ�ʽ /1000��ʾ����Ϊ��KmΪ��λ
}
//�������,��c/2nf��ʽ����
float CSorDrawStatic::Get_XCoord2nf(unsigned int xpoint,TestResultInfo* pData)
{
	return C_zll/(2*pData->Frequency * pData->Refraction * 1000000/*MHz*/) * xpoint;
}
//��֪���뷴�������±�
UINT CSorDrawStatic::Get_PosPoint(float distance,TestResultInfo* pData)
{
	return distance/(C_zll/(2*pData->Frequency * pData->Refraction * 1000000/*MHz*/));
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
void CSorDrawStatic::CalCulDispRect()
{
	//������ʾ��
	GetClientRect(m_rectCurve);
	//������ʾ��
	m_rectCoord.left=m_rectCurve.left;
	m_rectCoord.right=m_rectCurve.right;//wcq2012.02.14
	m_rectCoord.top=m_rectCurve.top;//wcq2011.12.09
	m_rectCoord.bottom=m_rectCurve.bottom;
}

/*
***************************************************************
��������:             CurveHorizonZoomIn                                                                              
����˵��:             ˮƽ�Ŵ�                                                                          
��ڲ���:             ��                                                                                      
���ڲ���:             ��                                                                                        
����ʱ��:             2011-07-25                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CSorDrawStatic::CurveHorizonZoomIn()
{
	if (m_iHorizonZoomNum < m_nZoomCount)
	{
		m_iHorizonZoomNum++;
		HorizonZooms();
		InvalidateAll();
	}
}

/*
***************************************************************
��������:             CurveHorizonZoomOut                                                                              
����˵��:             ˮƽ��С                                                                          
��ڲ���:             ��                                                                                      
���ڲ���:             ��                                                                                        
����ʱ��:             2011-07-25                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CSorDrawStatic::CurveHorizonZoomOut()
{
	if (m_iHorizonZoomNum>0)
	{
		m_iHorizonZoomNum--;
		HorizonZooms();//wcq2011.09.13
		InvalidateAll();//wcq2011.11.16
	}
}
//ˮƽ������������wcq 2011.09.13
void CSorDrawStatic::HorizonZooms()
{
	if (g_sorFileArray.totalCurveNumber == 0) //������ѡ��ʱ
		return;
	
	if (!g_sorFileArray.waveConfig.MultiMode)
	{
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;
		HorizonZoom(pCurve);
	}
	else
	{
		//���μ������������ʼ�������
		for(int nCount = 0; nCount < g_sorFileArray.totalCurveNumber; nCount++)
		{
			TestResultInfo* pCurve = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
			if (pCurve && pCurve->DataPointsNum > NUM_VALIDE_LEAST)
			{
				HorizonZoom(pCurve);
			}
		}
	}

	//�������û�ͼ�����С
	if (m_iHorizonZoomNum == 0) //������
	{
		m_nDrawWidth = m_rectCurve.Width();
	}
	else
	{
		m_nDrawWidth = m_rectCurve.Width() * (m_iHorizonZoomNum+1);
	}
	//����ˮƽ������
	SetHorScroll();
}
//����һ������wcq 2011.09.21
void CSorDrawStatic::HorizonZoom(TestResultInfo* pData)
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

//ƽ��һ������
void CSorDrawStatic::CurveHorMove(TestResultInfo* pData, int nRange, float fHorBalFactor)
{
	if(CLEAR_DATAPOINTS_NUM == pData->DataPointsNum)
	{
		return;
	}

	if(0 == m_iHorizonZoomNum) //δ����
	{
		pData->StartPoint=0;
		pData->MaxPoint=pData->DataPointsNum;
	}
	else
	{
		int nDrawPointNumber = pData->DataPointsNum/pow(2.0,m_iHorizonZoomNum-1);
		//������ʼλ��
		//pData->StartPoint= (fHorBalFactor * pData->DataPointsNum/nRange) * nDrawPointNumber;
		pData->StartPoint= pData->DataPointsNum * fHorBalFactor-nDrawPointNumber;
		if (pData->StartPoint<0)
		{
			pData->StartPoint=0;
		}
		pData->MaxPoint = pData->StartPoint + nDrawPointNumber;

		TRACE(_T("DataPointsNum:%d"),pData->DataPointsNum);
		TRACE(_T("StartPoint:%d"),pData->StartPoint);
		TRACE(_T("MaxPoint:%d"),pData->MaxPoint);


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
		//pData->FullPointCount=pointCount;
	}//eif

	//�������ú������ı�
	//MAIN->m_pWaveFormView->SetHorUnitText();
}

/*
***************************************************************
��������:             CurveVerticalZoomIn                                                                              
����˵��:             ��ֱ����Ŵ�                                                                          
��ڲ���:             ��                                                                                      
���ڲ���:             ��                                                                                        
����ʱ��:             2011-07-25                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CSorDrawStatic::CurveVerticalZoomIn()
{
	if (m_iVerticalZoomNum < ZOOM_VERTICAL_COUNT)
	{
		m_iVerticalZoomNum++;
		VerticalZoom();
		InvalidateRect(m_rectCurve,FALSE);
	}
}
/*
***************************************************************
��������:             CurveVerticalZoomOut                                                                              
����˵��:             ��ֱ������С                                                                          
��ڲ���:             ��                                                                                      
���ڲ���:             ��                                                                                        
����ʱ��:             2011-07-25                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CSorDrawStatic::CurveVerticalZoomOut()
{
	if (m_iVerticalZoomNum > 0)
	{
		m_iVerticalZoomNum--;
		VerticalZoom();
		InvalidateRect(m_rectCurve,FALSE);
	}
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
void CSorDrawStatic::VerticalZoom()
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

/*
***************************************************************
��������:             OnButtonMoveLeft                                                                              
����˵��:             ��ѡ�еı������                                                 
��ڲ���:             ��                                                                                      
���ڲ���:             ��                                                                                        
����ʱ��:             2010-06-28                                                                 
Write By:             XJF                                                                                          
***************************************************************
*/
void CSorDrawStatic::CursorMoveLeft()
{
	int moveFactor =1;//ÿ���ƶ�������//wcq2011.12.26
	//if (m_pMainDlg->checkLongClick(VK_LEFT))//wcq2011.12.26
	//{
	//	moveFactor = WIDTH_CURSOR_MOVE_LONG;
	//	//DEBUGMSG(TRUE,(_T("CursorMoveRight   is LongClick\n")));
	//}
	long l=GetTickCount();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_nSelectCursor == CURSOR_SELECT::CURSOR_A) //ѡ����A
	{
		//m_nPointA=m_nPointA - 5*(m_nMaxPoint-m_nStartPoint)/m_rectCoord.Width();//wcq2011.11.16
		//m_nPointA=m_nPointA - 1*(m_nMaxPoint-m_nStartPoint + 1)/m_rectCoord.Width();//wcq2011.11.24
		m_nPointA=m_nPointA - moveFactor*(m_nMaxPoint-m_nStartPoint + 1)/m_rectCoord.Width();////wcq2011.12.26
		//����ƶ�����Ϊ0�ˣ���ô��ÿ���ƶ�һ���㡣LDL2010-7-22����
		//if (((m_nMaxPoint-m_nStartPoint)/m_rectCoord.Width())==0)//wcq2011.11.24
		//{
		//	m_nPointA=m_nPointA+1;
		//}
		if (m_nMaxPoint-m_nStartPoint < m_rectCoord.Width())
		{
			m_nPointA --;
		}
		if (m_nPointA <0)//wcq2011.11.24
		{
			m_nPointA = 0;
		}
		if (m_nPointA>=m_nMaxPoint)////��Ϊ�����wcq2011.11.24
		{
			m_nPointA=m_nMaxPoint-1;
		}
		//if (m_nPointA>=m_pMapCurveData[m_curABWave].m_TestResult.DataPointsNum)//wcq2011.11.24
		//{
		//	m_nPointA=m_pMapCurveData[m_curABWave].m_TestResult.DataPointsNum -1;
		//}
		//InvalidateRect(m_rectCurve);//�ŵ�����wcq2011.11.16
		//�ƶ�ʱ��֤AҪ��B�����
		if (m_nPointA >= m_nPointB)
			m_nPointB = m_nPointA;
		if (m_nPointA < m_nStartPoint)//�Ƶ���ǰ��Ļ���棬����������//wcq2011.11.24
		{
			HorizonZooms();
		}
	}
	else if(m_nSelectCursor == CURSOR_SELECT::CURSOR_B) //ѡ����B
	{
		//m_nPointB=m_nPointB - 5*(m_nMaxPoint-m_nStartPoint)/m_rectCoord.Width();//wcq2011.11.16
		//m_nPointB=m_nPointB - 1*(m_nMaxPoint-m_nStartPoint + 1)/m_rectCoord.Width();//wcq2011.11.24
		m_nPointB=m_nPointB - moveFactor*(m_nMaxPoint-m_nStartPoint + 1)/m_rectCoord.Width();//wcq2011.12.26

		//����ƶ�����Ϊ0�ˣ���ô��ÿ���ƶ�һ���㡣LDL2010-7-22����
		if (m_nMaxPoint-m_nStartPoint < m_rectCoord.Width())
		{
			m_nPointB --;
		}

		if (m_nPointB <0)//wcq2011.11.24
		{
			m_nPointB = 0;
		}
		//if ((unsigned int)m_nPointB>=m_nMaxPoint)//��Ϊ�����wcq2011.11.24
		//{
		//	m_nPointB=m_nMaxPoint-1;
		//}
		//if (m_nPointB>=m_pMapCurveData[m_curABWave].m_TestResult.DataPointsNum)//wcq2011.11.24
		//{
		//	m_nPointB=m_pMapCurveData[m_curABWave].m_TestResult.DataPointsNum -1;
		//}
		//InvalidateRect(m_rectCurve);//�ŵ�����wcq2011.11.16
		//�ƶ�ʱ��֤AҪ��B�����
		if (m_nPointA >= m_nPointB)
			m_nPointA = m_nPointB;
		if (m_nPointB < m_nStartPoint)//�Ƶ���ǰ��Ļ���棬����������//wcq2011.11.24
		{
			HorizonZooms();
		}
	}
	else if(m_nSelectCursor == CURSOR_SELECT::CURSOR_AB) //ѡ����AB
	{
		//����Ѿ��ƶ����ұ߽磬�����ƶ�
		if (m_nPointA >= m_nMaxPoint ||
			m_nPointB >= m_nMaxPoint)
			return;

		//m_nPointA=m_nPointA - 5*(m_nMaxPoint-m_nStartPoint)/m_rectCoord.Width();//wcq2011.11.16
		//m_nPointB=m_nPointB - 5*(m_nMaxPoint-m_nStartPoint)/m_rectCoord.Width();//wcq2011.11.16
		//m_nPointA=m_nPointA - 1*(m_nMaxPoint-m_nStartPoint + 1)/m_rectCoord.Width();//wcq2011.11.24
		//m_nPointB=m_nPointB - 1*(m_nMaxPoint-m_nStartPoint + 1)/m_rectCoord.Width();//wcq2011.11.24
		m_nPointA=m_nPointA - moveFactor*(m_nMaxPoint-m_nStartPoint + 1)/m_rectCoord.Width();//wcq2011.12.26
		m_nPointB=m_nPointB - moveFactor*(m_nMaxPoint-m_nStartPoint + 1)/m_rectCoord.Width();//wcq2011.12.26
		//����ƶ�����Ϊ0�ˣ���ô��ÿ���ƶ�һ���㡣LDL2010-7-22����
		/*if (((m_nMaxPoint-m_nStartPoint)/m_rectCoord.Width())==0)//wcq2012.02.22
		{
		m_nPointA=m_nPointA+1;
		m_nPointB=m_nPointB+1;
		}*/
		if (m_nMaxPoint-m_nStartPoint < m_rectCoord.Width())//wcq2012.02.22
		{
			m_nPointA --;
			m_nPointB --;
		}
		if (m_nPointA <0)//wcq2011.11.24
		{
			m_nPointA = 0;
		}
		if (m_nPointB <0)//wcq2011.11.24
		{
			m_nPointB = 0;
		}
		//if (m_nPointA>=m_pMapCurveData[m_curABWave].m_TestResult.DataPointsNum)//wcq2011.11.24
		//{
		//	m_nPointA=m_pMapCurveData[m_curABWave].m_TestResult.DataPointsNum -1;
		//}
		//if (m_nPointB>=m_pMapCurveData[m_curABWave].m_TestResult.DataPointsNum)//wcq2011.11.24
		//{
		//	m_nPointB=m_pMapCurveData[m_curABWave].m_TestResult.DataPointsNum -1;
		//}
		int nCenterAB = (m_nPointB - m_nPointA)/2;
		if (nCenterAB < m_nStartPoint)//�����Ƶ���ǰ��Ļ���棬����������//wcq2011.11.24
		{
			HorizonZooms();
		}
		//InvalidateRect(m_rectCurve);//�ŵ�����wcq2011.11.16
	}
	else if(m_bEventCursor) //�¼��б�
	{
		if (m_nSelectIndex > 0)
			m_nSelectIndex --;
		else 
			m_nSelectIndex = 0;

		////if (m_nSelectIndex < m_iEventNum)//??????????????
		////	m_nPointEvent= m_CArrayEventInfoFromNode[m_nSelectIndex].nLocation;
		////else
		////	m_nPointEvent = 0;

		EventIsSelected(m_nPointEvent);
		//InvalidateRect(m_rectCurve);//�ŵ�����wcq2011.11.16
	}
	//InvalidateAll();//wcq2011.11.16
	InvalidateRect(m_rectSubInfo,FALSE);//wcq2012.03.08
	InvalidateRect(m_rectCurve,FALSE);//����
	//DEBUGMSG(true,(_T("CursorMoveLeft time:%d\n"),GetTickCount()-l));
	//InvalidateRect(m_rectSubInfo);////wcq2011.12.22
	//����Ϊ��ǰ��.wcq2011.09.23
	//if (m_bReceiveResultOK)
	//{
	//	if (m_nSelectCursor == 1) //ѡ����A
	//	{
	//		m_nPointA=m_nPointA-5*(m_nMaxPoint-m_nStartPoint)/m_rectCoord.Width();

	//		//����ƶ�����Ϊ0�ˣ���ô��ÿ���ƶ�һ���㡣LDL2010-7-22����
	//		if (((m_nMaxPoint-m_nStartPoint)/m_rectCoord.Width())==0)
	//		{
	//			m_nPointA=m_nPointA-1;
	//		}

	//		if ((int)(m_nPointA-m_nStartPoint)<=0)
	//		{
	//			m_nPointA=m_nStartPoint;
	//		}
	//		InvalidateRect(m_rectCurve);
	//		//�ƶ�ʱ��֤AҪ��B�����
	//		if (m_nPointA > m_nPointB)
	//			m_nPointB = m_nPointA;
	//	}
	//	else if(m_nSelectCursor == 2) //ѡ��B
	//	{
	//		m_nPointB=m_nPointB-5*(m_nMaxPoint-m_nStartPoint)/m_rectCoord.Width();

	//		//����ƶ�����Ϊ0�ˣ���ô��ÿ���ƶ�һ���㡣LDL2010-7-22����
	//		if (((m_nMaxPoint-m_nStartPoint)/m_rectCoord.Width())==0)
	//		{
	//			m_nPointB=m_nPointB-1;
	//		}

	//		if ((int)(m_nPointB-m_nStartPoint)<=0)
	//		{
	//			m_nPointB=m_nStartPoint;
	//		}
	//		InvalidateRect(m_rectCurve);
	//		//�ƶ�ʱ��֤AҪ��B�����
	//		if (m_nPointA > m_nPointB)
	//			m_nPointA = m_nPointB;
	//	}
	//	else if(m_nSelectCursor == 3) //ѡ��AB
	//	{
	//		//����Ѿ��ƶ�����߽磬���������ƶ�
	//		if ((int)(m_nPointA-m_nStartPoint) <= 0 ||
	//			(int)(m_nPointB-m_nStartPoint) <= 0)
	//		{
	//			return;
	//		}

	//		m_nPointA=m_nPointA-5*(m_nMaxPoint-m_nStartPoint)/m_rectCoord.Width();
	//		m_nPointB=m_nPointB-5*(m_nMaxPoint-m_nStartPoint)/m_rectCoord.Width();

	//		//����ƶ�����Ϊ0�ˣ���ô��ÿ���ƶ�һ���㡣LDL2010-7-22����
	//		if (((m_nMaxPoint-m_nStartPoint)/m_rectCoord.Width())==0)
	//		{
	//			m_nPointA=m_nPointA-1;
	//			m_nPointB=m_nPointB-1;
	//		}
	//		InvalidateRect(m_rectCurve);
	//	}
	//	else if(m_bEventCursor) //�¼��б�
	//	{
	//		if (m_nSelectIndex > 0)
	//			m_nSelectIndex --;
	//		else 
	//			m_nSelectIndex = m_iEventNum;

	//		if (m_nSelectIndex < m_iEventNum)
	//			m_nPointEvent= m_CArrayEventInfoFromNode[m_nSelectIndex].nLocation;
	//		else
	//			m_nPointEvent = 0;

	//		EventIsSelected(m_nPointEvent);
	//		InvalidateRect(m_rectCurve);
	//	}
	//}

	//if (m_bReadFromFile)
	//{
	//	if (m_nSelectCursor == 1) //ѡ����A
	//	{
	//		m_nPointA=m_nPointA-5*(m_nMaxPointFromFile-m_nStartPointFromFile)/m_rectCoord.Width();

	//		//����ƶ�����Ϊ0�ˣ���ô��ÿ���ƶ�һ���㡣LDL2010-7-22����
	//		if (((m_nMaxPointFromFile-m_nStartPointFromFile)/m_rectCoord.Width())==0)
	//		{
	//			m_nPointA=m_nPointA-1;
	//		}

	//		if ((int)(m_nPointA-m_nStartPointFromFile)<=0)
	//		{
	//			m_nPointA=m_nStartPointFromFile;
	//		}
	//		InvalidateRect(m_rectCurve);
	//		//�ƶ�ʱ��֤AҪ��B�����
	//		if (m_nPointA > m_nPointB)
	//			m_nPointB = m_nPointA;
	//	}
	//	else if(m_nSelectCursor == 2) //ѡ��B
	//	{
	//		m_nPointB=m_nPointB-5*(m_nMaxPointFromFile-m_nStartPointFromFile)/m_rectCoord.Width();

	//		//����ƶ�����Ϊ0�ˣ���ô��ÿ���ƶ�һ���㡣LDL2010-7-22����
	//		if (((m_nMaxPointFromFile-m_nStartPointFromFile)/m_rectCoord.Width())==0)
	//		{
	//			m_nPointB=m_nPointB-1;
	//		}

	//		if ((int)(m_nPointB-m_nStartPointFromFile)<=0)
	//		{
	//			m_nPointB=m_nStartPointFromFile;
	//		}
	//		InvalidateRect(m_rectCurve);
	//		//�ƶ�ʱ��֤AҪ��B�����
	//		if (m_nPointA > m_nPointB)
	//			m_nPointA = m_nPointB;
	//	}
	//	else if(m_nSelectCursor == 3) //ѡ��AB
	//	{
	//		//����Ѿ��ƶ�����߽磬���������ƶ�
	//		if ((int)(m_nPointA-m_nStartPointFromFile) <= 0 ||
	//			(int)(m_nPointB-m_nStartPointFromFile) <= 0)
	//		{
	//			return;
	//		}

	//		m_nPointA=m_nPointA-5*(m_nMaxPointFromFile-m_nStartPointFromFile)/m_rectCoord.Width();
	//		m_nPointB=m_nPointB-5*(m_nMaxPointFromFile-m_nStartPointFromFile)/m_rectCoord.Width();

	//		//����ƶ�����Ϊ0�ˣ���ô��ÿ���ƶ�һ���㡣LDL2010-7-22����
	//		if (((m_nMaxPointFromFile-m_nStartPointFromFile)/m_rectCoord.Width())==0)
	//		{
	//			m_nPointA=m_nPointA-1;
	//			m_nPointB=m_nPointB-1;
	//		}
	//		InvalidateRect(m_rectCurve);
	//	}
	//	else if(m_bEventCursor) //�¼��б�
	//	{
	//		if (m_nSelectIndex > 0)
	//			m_nSelectIndex --;
	//		else 
	//			m_nSelectIndex = m_iEventNumFromFile;

	//		if (m_nSelectIndex < m_iEventNumFromFile)
	//			m_nPointEvent= m_CArrayEventInfoFromFile[m_nSelectIndex].nLocation;
	//		else
	//			m_nPointEvent = 0;

	//		EventIsSelected(m_nPointEvent);
	//		InvalidateRect(m_rectCurve);
	//	}
	//}
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
void CSorDrawStatic::EventIsSelected(int PointEvent)
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

void CSorDrawStatic::CurvesHorMove(int nRange, float nHorBalFactor)
{
	if (g_sorFileArray.totalCurveNumber == 0)//��û����
		return;

	//�����߻���ģʽ
	if (!g_sorFileArray.waveConfig.MultiMode)
	{
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;
		CurveHorMove(pCurve, nRange, nHorBalFactor);
	}
	else
	{
		//�����߻���ģʽ
		for(int nCount = 0; nCount < g_sorFileArray.totalCurveNumber; nCount++)
		{
			TestResultInfo* pData = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
			if (!pData || pData->DataPointsNum < NUM_VALIDE_LEAST || CLEAR_DATAPOINTS_NUM == g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo->DataPointsNum)//û�л����ݺ���wcq2011.09.09 //0 == m_pMapCurveData[m_curABWave]->m_TestResult.DataPointsNum.�ز�ʱ������//wcq2011.12.22
			{
				continue;
			}
			CurveHorMove(pData, nRange, nHorBalFactor);
		}
	}
	MAIN->m_pWaveFormView->SetHorUnitText(nHorBalFactor);
	InvalidateRect(m_rectCurve,FALSE);//wcq2011.09.23
}

/*
***************************************************************
��������:             CurveVerMove
��ڲ���:             ��
���ڲ���:             ��
����ʱ��:             2011-12-14
Write By:             XJF
����˵��:             wcq ��������ƽ��ʱ��ƽ����������
ƽ�Ʒ�ʽ���Ե�ǰ����ͼ�еľ��ο�Ϊ׼���±߿������Ƶ�����ͼ������ϱ�Ե��
�ϱ߿������Ƶ�����ͼ������±�Ե���л���ƽ��ģʽʱ���ȼ�������ͼ�еľ��ο�
ƽ��ʱ���м��ͼ�������ϣ��£��ƶ�һ���ء�
***************************************************************
*/
void CSorDrawStatic::CurveVerMove(TestResultInfo* pData, int nSetp, bool bSpin)
{
	CRect rect;
	MAIN->m_pThumbFormView->GetMoveRect(&rect, pData);
	if (bSpin)
		pData->YOffset += nSetp;//ͨ����ť����λ��
	else
		pData->YOffset = nSetp;//ͨ������������λ��
}

void CSorDrawStatic::CurvesVerMove(int nSetp)
{
	if (!g_sorFileArray.waveConfig.MultiMode)
	{
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;
		CurveVerMove(pCurve, nSetp);
	}
	else
	{
		//�����߻���ģʽ
		for(int nCount = 0; nCount < g_sorFileArray.totalCurveNumber; nCount++)
		{
			TestResultInfo* pCurve = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
			if (pCurve)
			{
				CurveVerMove(pCurve, nSetp);
			}//eif
		}//efor
	}

	InvalidateAll();
}

////��ƽ�Ƶ�����ʱ����ֱ�����ƽ�ƻָ�Ϊ0;wcq2011.12.14
void CSorDrawStatic::ResetYOffset()
{
	//����
	for(int nCount = 0; nCount < g_sorFileArray.totalCurveNumber; nCount++)
	{
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
		if (pCurve)//wcq2011.12.23
		{
			pCurve->YOffset = 0;
			continue;
		}
	}
}
/*
***************************************************************
��������:             InitialEventListFromFile                                                                              
����˵��:             ���ļ����ݳ�ʼ���¼�����Ϣ                                                
��ڲ���:             ��                                                                                      
���ڲ���:             ��                                                                                        
����ʱ��:             2010-07-02                                                                
Write By:             XJF                                                                                          
***************************************************************
//*/
//void CSorDrawStatic::ReadEventListFromFile()
//{
//	m_lstEventList.DeleteAllItems();
//	int i;
//	CString str;
//	for (i=0;i<m_CArrayEventInfoFromFile.GetSize();i++)
//	{
//		str.Format(_T("%d"),i+1);
//		m_lstEventList.InsertItem(i,str,1);
//
//		if (m_CArrayEventInfoFromFile[i].nEventType==0)
//		{
//			m_lstEventList.SetItemText(i,1,_T("EventType_0"));//��ʼ�¼�
//		}
//		else if (m_CArrayEventInfoFromFile[i].nEventType==1)
//		{
//			m_lstEventList.SetItemText(i,1,_T("EventType_0")); //�����¼�
//		}
//		else if (m_CArrayEventInfoFromFile[i].nEventType==2)
//		{
//			m_lstEventList.SetItemText(i,1,_T("EventType_1")); //�Ƿ����¼�
//		}
//		else if (m_CArrayEventInfoFromFile[i].nEventType==3)
//		{
//			m_lstEventList.SetItemText(i,1,_T("EventType_1")); //�����¼�
//		}
//
//		str.Format(_T("%.2f"),Get_XCoordForFile(m_CArrayEventInfoFromFile[i].nLocation));
//		m_lstEventList.SetItemText(i,2,str);
//
//		if (m_CArrayEventInfoFromFile[i].fReflectValue==100)
//		{
//			m_lstEventList.SetItemText(i,3,_T("----"));
//		}
//		else
//		{
//			str.Format(_T("%.3f"),m_CArrayEventInfoFromFile[i].fReflectValue);
//			m_lstEventList.SetItemText(i,3,str);
//		}
//
//		if (m_CArrayEventInfoFromFile[i].fInsertloss==100)
//		{
//			m_lstEventList.SetItemText(i,4,_T("----"));
//		}
//		else
//		{
//			str.Format(_T("%.3f"),m_CArrayEventInfoFromFile[i].fInsertloss);
//			m_lstEventList.SetItemText(i,4,str);
//		}
//
//		str.Format(_T("%.3f"),m_CArrayEventInfoFromFile[i].fAttenuCoefficient);
//		m_lstEventList.SetItemText(i,5,str);
//
//		str.Format(_T("%.3f"),m_CArrayEventInfoFromFile[i].fCumulativeLoss);
//		m_lstEventList.SetItemText(i,6,str);
//
//		//////////////////////////////////////////////////////////////////////////
//		//����ʼ�¼��ͽ����¼�
//		if (m_CArrayEventInfoFromFile[i].nEventType==0)
//		{
//			m_lstEventList.SetItemText(i,4,_T("----")); //�������
//		}
//		if (m_CArrayEventInfoFromFile[i].nEventType==3)
//		{
//			m_lstEventList.SetItemText(i,4,_T("----")); //�������
//			m_lstEventList.SetItemText(i,5,_T("----")); //˥��ϵ��
//		}
//		if (m_CArrayEventInfoFromFile[i].nEventType==2)//�Ƿ����¼��ķ���ֵΪ��
//		{
//			m_lstEventList.SetItemText(i,3,_T("----")); 
//		}
//
//	}
//}
//
///*
//***************************************************************
//��������:             ReadCurveDataFromNode                                                                              
//����˵��:             ��ȡ����������                                                  
//��ڲ���:             ��                                                                                      
//���ڲ���:             ��                                                                                        
//����ʱ��:             2011-06-28                                                                 
//Write By:             XJF                                                                                          
//***************************************************************
//*/
//void CSorDrawStatic::ReadCurveDataFromNode()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	CString strPath = L"/NandFlash/OTDR/OTDR-data/sor.txt";
//	FILE *fp;
//	double fTemp;
//	char pszFileName[MAX_PATH] = {""};
//	WideCharToMultiByte(CP_OEMCP, 0, strPath, -1, pszFileName, strPath.GetLength() * 2, NULL, NULL);
//	if((fp = fopen(pszFileName,"r")) != NULL)
//	{
//		//д���ݵ�������
//		if(NULL != m_pRecvbuf)
//		{
//			delete[] m_pRecvbuf;
//		}
//		//���ļ��м���Ĭ�ϲ���������2000
//
//		CCurveData* pCurveData = m_pMapCurveData[2000];
//		if (pCurveData != NULL)
//		{
//			delete pCurveData;
//			pCurveData = NULL;
//		}
//		pCurveData = new CCurveData();
//
//		m_pMapCurveData[2000] = pCurveData;
//		int nDataLen = 0;
//		fscanf(fp,"%d",&nDataLen);
//		pCurveData->DataPointsNum = nDataLen;
//		pCurveData->MaxPoint = nDataLen;
//		pCurveData->IsMaxLength = TRUE;
//		m_fFrequency = 50;
//		m_pRecvbuf = new WORD[nDataLen];
//		WORD wTemp;
//		for (unsigned int i = 0;i < nDataLen; i++)
//		{
//			fscanf(fp,"%d",&wTemp);
//			m_pRecvbuf[i] = wTemp;
//		}
//		pCurveData->pDataPoints = m_pRecvbuf;
//		m_bReadFromFile = TRUE;
//		m_bISCurveFromFileSelected=TRUE;
//		if (m_ptRecord!=NULL)
//		{
//			delete []m_ptRecord;
//		}
//		m_ptRecord = new CPoint[pCurveData->DataPointsNum];//����ռ䱣�棬��ͼ�ĵ�
//		fclose(fp);
//	}
//}
/*

***************************************************************
��������:             OnClickListEvent                                                                              
����˵��:             ����¼����е�һ���¼��󣬱�ѡ�еı���ƶ������¼���                                              
��ڲ���:             NMHDR* pNMHDR, LRESULT* pResult                                                                                      
���ڲ���:             ��                                                                                        
����ʱ��:             2011-07-06                                                              
Write By:             XJF                                                                                          
***************************************************************
*/

void CSorDrawStatic::OnClickListEvent(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//CCurveData* pCurveData = m_pMapCurveData[m_dwWaveTesting];
	//// TODO: Add your control notification handler code here
	//POSITION pos = m_lstEventList.GetFirstSelectedItemPosition();
	//int nIndex = m_lstEventList.GetNextSelectedItem(pos);
	//if (nIndex >= 0 && nIndex < m_lstEventList.GetItemCount()-1)
	//{
	//	//m_lstEventList.SetItemState(nIndex, NULL, NULL);
	//	MoveEventList(nIndex);
	//}
	//////int i;
	//////POSITION pos=m_lstEventList.GetFirstSelectedItemPosition();
	//////while(pos)
	//////{
	//////	i=m_lstEventList.GetNextSelectedItem(pos);

	/////////	if (m_bISCurveFromFileSelected) //����ļ����߱�ѡ��//wcq2012.03.09
	/////////	{
	//////		if (m_nSelectCursor == 1) //ѡ����A��
	//////		{
	//////			m_nPointA=m_CArrayEventInfoFromFile[i].nLocation;
	//////			if (m_nPointA<g_nMaxPointFromFile)
	//////			{
	//////				HorizonZoom();
	//////				VerticalZoom();
	//////			}

	//////			if (m_nPointA>=g_nMaxPointFromFile)
	//////			{
	//////				m_nPointA=g_nMaxPointFromFile-1;
	//////			}
	//////		}
	//////		if (m_nSelectCursor == 2) //���ѡ����B��
	//////		{
	//////			m_nPointB=m_CArrayEventInfoFromFile[i].nLocation;
	//////			if (m_nPointB<g_nMaxPointFromFile)
	//////			{
	//////				HorizonZoom();
	//////				VerticalZoom();
	//////			}

	//////			if (m_nPointB>=g_nMaxPointFromFile)
	//////			{
	//////				m_nPointB=g_nMaxPointFromFile-1;
	//////			}

	//////		}
	//////		InvalidateRect(&m_rectClient,FALSE);
	//////		//InitialReturnInfo();
	//////	///}//wcq2012.03.09

	//////	////if (m_bISCurveFromNodeSelected)
	//////	////{
	//////	////	if (m_nSelectCursor == 1) //ѡ����A��
	//////	////	{
	//////	////		m_nPointA=m_CArrayEventInfoFromNode[i].nLocation;
	//////	////		if (m_nPointA<pCurveData->DataPointsNum)
	//////	////		{
	//////	////			HorizonZoom();
	//////	////			VerticalZoom();
	//////	////		}

	//////	////		if (m_nPointA>=pCurveData->DataPointsNum)
	//////	////		{
	//////	////			m_nPointA=pCurveData->DataPointsNum-1;
	//////	////		}

	//////	////	}
	//////	////	if (m_nSelectCursor == 2) //���ѡ����B��
	//////	////	{
	//////	////		m_nPointB=m_CArrayEventInfoFromNode[i].nLocation;
	//////	////		if (m_nPointB<pCurveData->DataPointsNum)
	//////	////		{
	//////	////			HorizonZoom();
	//////	////			VerticalZoom();
	//////	////		}
	//////	////		if (m_nPointB>=pCurveData->DataPointsNum)
	//////	////		{
	//////	////			m_nPointB=pCurveData->DataPointsNum-1;
	//////	////		}
	//////	////	}
	//////	////	InvalidateRect(&m_rectClient,FALSE);
	//////	////	//InitialReturnInfo();
	//////	////}
	//////}

	*pResult = 0;
}

/*
***************************************************************
��������:             InitialReturnInfo                                                                              
����˵��:             ��ʼ��������Ϣ                                              
��ڲ���:             ��                                                                                      
���ڲ���:             ��                                                                                        
����ʱ��:             2011-07-02                                                                
Write By:             XJF                                                                                          
***************************************************************
*/
//void CSorDrawStatic::InitialReturnInfo()
//{
//	//CString str;
//	////Y
//	//GetDlgItem(IDC_STATIC_X)->SetWindowText(_T("Y: 6dB/div"));
//	////X
//	//GetDlgItem(IDC_STATIC_Y)->SetWindowText(_T("X:"));
//	////����
//	//CString strTestWave;
//	//strTestWave.LoadString(m_pMainDlg->GetCurLangHandle(), IDS_TEST_WAVE);
//	//str.Format(_T("%s%d nm"),strTestWave, m_iWaveLengthFromFile);
//	//GetDlgItem(IDC_STATIC_WAVE)->SetWindowText(str);
//	////����ʱ��
//	//CString strTestTime;
//	//strTestTime.LoadString(m_pMainDlg->GetCurLangHandle(), IDS_TEST_TIME);
//	//str.Format(_T("%s%d s"),strTestTime, m_iMeasureTimeFromFile);
//	//GetDlgItem(IDC_STATIC_TIME)->SetWindowText(str);
//	////����
//	//CString strPulseWidth;
//	//strPulseWidth.LoadString(m_pMainDlg->GetCurLangHandle(), IDS_PULSE_WIDTH);
//	//str.Format(_T("%s%d ns"),strPulseWidth, m_iPulseWidthFromFile);
//	//GetDlgItem(IDC_STATIC_PULSE_WIDTH)->SetWindowText(str);
//	////����������
//	//CString strReflectConff;
//	//strReflectConff.LoadString(m_pMainDlg->GetCurLangHandle(), IDS_REFLECTION);
//	//str.Format(_T("%s%d ns"),strReflectConff, m_fRefractionFromFile);
//	//GetDlgItem(IDC_STATIC_REFLECT_COFF)->SetWindowText(str);
//	////���A��λ��
//	//int nDistanceA=Get_XCoordForFile(m_nPointA);
//	//str.Format(_T("A: %dm"),nDistanceA);
//	//GetDlgItem(IDC_STATIC_COORD_A)->SetWindowText(str);
//	////���Bλ��
//	//int nDistanceB=Get_XCoordForFile(m_nPointB);
//	//str.Format(_T("B: %dm"),nDistanceB);
//	//GetDlgItem(IDC_STATIC_COORD_B)->SetWindowText(str);
//	////���B-A����
//	//str.Format(_T("B-A: %dm"),nDistanceB - nDistanceA);
//	//GetDlgItem(IDC_STATIC_AB_DISTANCE)->SetWindowText(str);
//}

/*
***************************************************************
��������:             CalculABParaFromNode                                                                              
����˵��:             ������������AB�β���                                              
��ڲ���:             ��                                                                                      
���ڲ���:             ��                                                                                        
����ʱ��:             2011-07-02                                                                
Write By:             XJF                                                                                          
***************************************************************
//*/
//void CSorDrawStatic::CalculABParaFromNode()
//{
//	CCurveData* pCurveData = m_pMapCurveData[m_dwWaveTesting];
//
//	int RealPointA; // A��ʵ�ʵ���
//	int RealPointB; //B��ʵ�ʵ���
//	if (m_bReceiveResultOK && m_bReadFromFile)
//	{
//		if (Get_XCoord(pCurveData->DataPointsNum)>Get_XCoordForFile(g_nMaxPointFromFile))
//		{
//			m_iABSegmentLen=(long int)(fabs(Get_XCoord(m_nPointA)-Get_XCoord(m_nPointB)));//AB֮��ľ���
//			m_fABLoss=(float)(fabs(m_CArraySampleLoss[m_nPointA]-m_CArraySampleLoss[m_nPointB]));//����֮������
//			if (m_iABSegmentLen!=0)
//			{
//				m_fABSegmentAttenu=(float)(m_fABLoss/(m_iABSegmentLen/1000.0));//����֮���˥��	
//			}
//			else
//			{
//				m_fABSegmentAttenu=0;//����֮���˥��
//			}
//			//��������if7��6�ż���
//			if (m_nSelectCursor == 1) //���ѡ��A�ߣ���ʾA��������
//			{
//				//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("A��������:");
//				m_fAPointCoord=(float)m_CArraySampleLoss[m_nPointA]; //A��������
//			}
//			if (m_nSelectCursor == 2) //���ѡ��B�ߣ���ʾB��������
//			{
//				//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("B��������:");
//				m_fAPointCoord=(float)m_CArraySampleLoss[m_nPointB]; //B��������
//			}
//		}
//		else
//		{
//			//���ʱ��Ҫʹ��������������������е�ʵ�ʵ���
//			RealPointA=GetSerialNum(Get_XCoordForFile(m_nPointA));
//			RealPointB=GetSerialNum(Get_XCoordForFile(m_nPointB));
//			m_iABSegmentLen=(long int)(fabs(Get_XCoordForFile(m_nPointA)-Get_XCoordForFile(m_nPointB))); //AB��ľ���
//			if ((Get_XCoordForFile(m_nPointA)>Get_XCoord(pCurveData->DataPointsNum)) && (Get_XCoordForFile(m_nPointB)<=Get_XCoord(pCurveData->DataPointsNum)))
//			{
//				//A�˳������������ߵ�������
//				//��������if7��6�ż���
//				if (m_nSelectCursor == 1) //���ѡ��A�ߣ���ʾA��������
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("A��������:");
//					m_fAPointCoord=0; //A��������
//				}
//				if (m_nSelectCursor == 2) //���ѡ��B�ߣ���ʾB��������
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("B��������:");
//					m_fAPointCoord=(float)m_CArraySampleLoss[RealPointB]; //B��������
//				}
//				m_fABLoss=(float)fabs(m_CArraySampleLoss[RealPointB]);
//			}
//			else if ((Get_XCoordForFile(m_nPointB)>Get_XCoord(pCurveData->DataPointsNum)) && (Get_XCoordForFile(m_nPointA)<=Get_XCoord(pCurveData->DataPointsNum)))
//			{
//				//B�˳������������ߵ�������
//				//��������if7��6�ż���
//				if (m_nSelectCursor == 1) //���ѡ��A�ߣ���ʾA��������
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("A��������:");
//					m_fAPointCoord=(float)m_CArraySampleLoss[RealPointA]; //A��������
//				}
//				if (m_nSelectCursor == 2) //���ѡ��B�ߣ���ʾB��������
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("B��������:");
//					m_fAPointCoord=0; //B��������
//				}
//				m_fABLoss=(float)fabs(m_CArraySampleLoss[RealPointA]);
//			}
//			else if ((Get_XCoordForFile(m_nPointA)>Get_XCoord(pCurveData->DataPointsNum)) && (Get_XCoordForFile(m_nPointB)>Get_XCoord(pCurveData->DataPointsNum)))
//			{
//				//A��B�˶��������������ߵ�������
//				//��������if7��6�ż���
//				if (m_nSelectCursor == 1) //���ѡ��A�ߣ���ʾA��������
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("A��������:");
//					m_fAPointCoord=0; //A��������
//				}
//				if (m_nSelectCursor == 2) //���ѡ��B�ߣ���ʾB��������
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("B��������:");
//					m_fAPointCoord=0; //B��������
//				}
//				m_fABLoss=0;
//			}
//			else
//			{
//				//A��B�˶�û�г����ļ����ߵ�������
//				//��������if7��6�ż���
//				if (m_nSelectCursor == 1) //���ѡ��A�ߣ���ʾA��������
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("A��������:");
//					m_fAPointCoord=(float)m_CArraySampleLoss[RealPointA]; //A��������
//				}
//				if (m_nSelectCursor == 2) //���ѡ��B�ߣ���ʾB��������
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("B��������:");
//					m_fAPointCoord=(float)m_CArraySampleLoss[RealPointB]; //B��������
//				}
//				m_fABLoss=(float)(fabs(m_CArraySampleLoss[RealPointA]-m_CArraySampleLoss[RealPointB]));//����֮������
//			}
//
//			if (m_iABSegmentLen!=0)
//			{
//				m_fABSegmentAttenu=(float)(m_fABLoss/(m_iABSegmentLen/1000.0));
//			}
//			else
//			{
//				m_fABSegmentAttenu=0;
//			}
//		}
//	}
//
//	if(m_bReceiveResultOK)
//	{
//		m_iABSegmentLen=(long int)fabs(Get_XCoord(m_nPointA)-Get_XCoord(m_nPointB));//AB֮��ľ���
//		m_fABLoss=(float)fabs(m_CArraySampleLoss[m_nPointA]-m_CArraySampleLoss[m_nPointB]);//����֮������
//		if (m_iABSegmentLen!=0)
//		{
//			m_fABSegmentAttenu=(float)(m_fABLoss/(m_iABSegmentLen/1000.0));//����֮���˥��
//
//		}
//		else
//		{
//			m_fABSegmentAttenu=0;//����֮���˥��
//		}
//		//��������if7��6�ż���
//		if (m_nSelectCursor == 1) //���ѡ��A�ߣ���ʾA��������
//		{
//			//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("A��������:");
//			m_fAPointCoord=(float)m_CArraySampleLoss[m_nPointA]; //A��������
//		}
//		if (m_nSelectCursor == 2) //���ѡ��B�ߣ���ʾB��������
//		{
//			//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("B��������:");
//			m_fAPointCoord=(float)m_CArraySampleLoss[m_nPointB]; //B��������
//		}
//	}
//
//}

/*
***************************************************************
��������:             CalculABParaFromFile                                                                              
����˵��:             �����ļ�����AB�β���                                              
��ڲ���:             ��                                                                                      
���ڲ���:             ��                                                                                        
����ʱ��:             2011-07-02                                                                
Write By:             XJF                                                                                          
***************************************************************
//*/
//void CSorDrawStatic::CalculABParaFromFile()
//{
//	CCurveData* pCurveData = m_pMapCurveData[m_dwWaveTesting];
//	
//	int RealPointA; // A��ʵ�ʵ���
//	int RealPointB; //B��ʵ�ʵ���
//	if (m_bReceiveResultOK && m_bReadFromFile)
//	{
//		if (Get_XCoordForFile(g_nMaxPointFromFile)>Get_XCoord(pCurveData->DataPointsNum))
//		{
//			m_iABSegmentLenFromFile=(long int)fabs(Get_XCoordForFile(m_nPointA)-Get_XCoordForFile(m_nPointB));//AB֮��ľ���
//			m_fABLossFromFile=(float)fabs(m_CArraySampleLossFromFile[m_nPointA]-m_CArraySampleLossFromFile[m_nPointB]);//����֮������
//			if (m_iABSegmentLenFromFile!=0)
//			{
//				m_fABSegmentAttenuFromFile=(float)(m_fABLossFromFile/(m_iABSegmentLenFromFile/1000.0));//����֮���˥��
//
//			}
//			else
//			{
//				m_fABSegmentAttenuFromFile=0;//����֮���˥��
//			}
//			//��������if7��6�ż���
//			if (m_nSelectCursor == 1) //���ѡ��A�ߣ���ʾA��������
//			{
//				//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("A��������:");
//				m_fAPointCoordFromFile=(float)m_CArraySampleLossFromFile[m_nPointA]; //A��������
//			}
//			if (m_nSelectCursor == 2) //���ѡ��B�ߣ���ʾB��������
//			{
//				//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("B��������:");
//				m_fAPointCoordFromFile=(float)m_CArraySampleLossFromFile[m_nPointB]; //B��������
//			}
//
//		}
//		else
//		{
//			//���ʱ��Ҫʹ������������ļ������е�ʵ�ʵ���
//			RealPointA=GetSerialNumForFile(Get_XCoord(m_nPointA));
//			RealPointB=GetSerialNumForFile(Get_XCoord(m_nPointB));
//			m_iABSegmentLenFromFile=(long int)fabs(Get_XCoord(m_nPointA)-Get_XCoord(m_nPointB)); //AB��ľ���
//			if ((Get_XCoord(m_nPointA)>Get_XCoordForFile(g_nMaxPointFromFile)) && (Get_XCoord(m_nPointB)<=Get_XCoordForFile(g_nMaxPointFromFile)))
//			{
//				//A�˳������ļ����ߵ�������
//				if (m_nSelectCursor == 1) //ѡ��A��
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("A��������:");
//					m_fAPointCoordFromFile=0;
//				}
//				if (m_nSelectCursor == 2)
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("B��������:");
//					m_fAPointCoordFromFile=(float)m_CArraySampleLossFromFile[RealPointB];
//				}
//
//				m_fABLossFromFile=(float)fabs(m_CArraySampleLossFromFile[RealPointB]);
//			}
//			else if ((Get_XCoord(m_nPointB)>Get_XCoordForFile(g_nMaxPointFromFile)) && (Get_XCoord(m_nPointA)<=Get_XCoordForFile(g_nMaxPointFromFile)))
//			{
//				//B�˳������ļ����ߵ�������
//				if (m_nSelectCursor == 1)
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("A��������:");
//					m_fAPointCoordFromFile=(float)m_CArraySampleLossFromFile[RealPointA];
//				}
//				if (m_nSelectCursor == 2)
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("B��������:");
//					m_fAPointCoordFromFile=0;
//				}
//
//				m_fABLossFromFile=(float)fabs(m_CArraySampleLossFromFile[RealPointA]);
//			}
//			else if ((Get_XCoord(m_nPointA)>Get_XCoordForFile(g_nMaxPointFromFile)) && (Get_XCoord(m_nPointB)>Get_XCoordForFile(g_nMaxPointFromFile)))
//			{
//				//A��B�˶��������ļ����ߵ�������
//				if (m_nSelectCursor == 1)
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("A��������:");
//					m_fAPointCoordFromFile=0;
//				}
//				if (m_nSelectCursor == 2)
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("B��������:");
//					m_fAPointCoordFromFile=0;
//				}
//				m_fABLossFromFile=0;
//			}
//			else
//			{
//				//A��B�˶�û�г����ļ����ߵ�������
//				if (m_nSelectCursor == 1)
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("A��������:");
//					m_fAPointCoordFromFile=(float)m_CArraySampleLossFromFile[RealPointA];
//				}
//				if (m_nSelectCursor == 2)
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("B��������:");
//					m_fAPointCoordFromFile=(float)m_CArraySampleLossFromFile[RealPointB];
//				}
//
//				m_fABLossFromFile=(float)fabs(m_CArraySampleLossFromFile[RealPointA]-m_CArraySampleLossFromFile[RealPointB]);//����֮������
//			}
//
//			if (m_iABSegmentLenFromFile!=0)
//			{
//				m_fABSegmentAttenuFromFile=(float)(m_fABLossFromFile/(m_iABSegmentLenFromFile/1000.0));
//			}
//			else
//			{
//				m_fABSegmentAttenuFromFile=0;
//			}
//		}
//	}
//
//	if(m_bReadFromFile && !m_bReceiveResultOK)
//	{
//		m_iABSegmentLenFromFile=(long int)fabs(Get_XCoordForFile(m_nPointA)-Get_XCoordForFile(m_nPointB));//AB֮��ľ���
//		m_fABLossFromFile=(float)fabs(m_CArraySampleLossFromFile[m_nPointA]-m_CArraySampleLossFromFile[m_nPointB]);//����֮������
//		if (m_iABSegmentLenFromFile!=0)
//		{
//			m_fABSegmentAttenuFromFile=(float)(m_fABLossFromFile/(m_iABSegmentLenFromFile/1000.0));//����֮���˥��
//
//		}
//		else
//		{
//			m_fABSegmentAttenuFromFile=0;//����֮���˥��
//		}
//		//��������if7��6�ż���
//		if (m_nSelectCursor == 1) //���ѡ��A�ߣ���ʾA��������
//		{
//			//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("A��������:");
//			m_fAPointCoordFromFile=(float)m_CArraySampleLossFromFile[m_nPointA]; //A��������
//		}
//		if (m_nSelectCursor == 2) //���ѡ��B�ߣ���ʾB��������
//		{
//			//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("B��������:");
//			m_fAPointCoordFromFile=(float)m_CArraySampleLossFromFile[m_nPointB]; //B��������
//		}
//	}
//}
//
//

/*
***************************************************************
��������:             GetSerialNumForFile                                                                              
����˵��:             ��ȡ��������Ӧ���ļ����ߵ���                                              
��ڲ���:             ��                                                                                      
���ڲ���:             �����Ӧ����                                                                                        
����ʱ��:             2011-07-02                                                                
Write By:             XJF                                                                                          
***************************************************************
*//*
int CSorDrawStatic::GetSerialNumForFile(long int theDistance)
{
int X=(int)((2*m_fFrequencyFromFile*m_fRefractionFromFile*theDistance)/C);
return X;
}
*/

/*
***************************************************************
��������:             GetSerialNum                                                                              
����˵��:             ��ȡ��������Ӧ���������ߵ���                                              
��ڲ���:             ��                                                                                      
���ڲ���:             �����Ӧ����                                                                                        
����ʱ��:             2010-07-02                                                                
Write By:             XJF                                                                                          
***************************************************************
*/
//int CSorDrawStatic::GetSerialNum(long int theDistance)
//{
//	int X=(int)((2*m_fFrequency*m_fRefraction*theDistance)/C);
//	return X;
//}
/*

//���ص�ǰ��������ģʽ 0�����ţ�1��ƽ��
int CSorDrawStatic::GetCurCurveZoomMode() const
{
	return m_iCurveZoomMode;
}
//��������������ݣ�����DataPointsNumΪ0.//wqc2011.12.19
void CSorDrawStatic::ClearAllCurveData()
{
	for(int nCount = 0; nCount < g_sorFileArray.totalCurveNumber; nCount++)
	{
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
		if (pCurve)
		{
			//pCurve->m_TestResult.DataPointsNum = 0;//wcq2011.12.23
			pCurve->DataPointsNum = CLEAR_DATAPOINTS_NUM;//wcq2011.12.23
			pCurve->IsMaxLength = FALSE;//????wcq2011.12.23
		}
	}
}
//���õ�ǰ��������ģʽ 0�����ţ�1��ƽ��
void CSorDrawStatic::SetCurCurveZoomMode(int nCurveZoomMode)
{
	m_iCurveZoomMode = nCurveZoomMode;
}


//�¼��б�
void CSorDrawStatic::EventList()
{
	//m_bDisplayEventList = !m_bDisplayEventList;

	//CalCulDispRect(m_bDisplayEventList);
	//CRect ClientRect;
	//GetClientRect(&ClientRect);
	////InvalidateRect(&ClientRect);//wcq2011.11.16////�ŵ�����wcq2011.12.15
	////InvalidateAll();//wcq2011.11.16
	////��Ĭ��ѡ����
	//POSITION pos = m_lstEventList.GetFirstSelectedItemPosition();
	//int nIndex = m_lstEventList.GetNextSelectedItem(pos);
	//if (m_lstEventList.GetItemCount() > 0 && nIndex == -1)
	//{
	//	m_lstEventList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	//	//MoveEventList(0);	//�õ����� wcq2011.12.15
	//}//eif
	//m_lstEventList.MoveWindow(&m_rectEventArea);//ԭ����OnPaint�//wcq2011.12.15
	//m_bReDrawGrid = TRUE;//��Ϊ��С�ı��ˣ�����Ҫ�ػ�Grid.//wcq2011.12.15
	////InvalidateRect(&ClientRect);//wcq2011.12.15
	//Invalidate(FALSE);//wcq2011.12.15
}

//���߶Ա�
void CSorDrawStatic::CurveCompare(const CString strFileName)
{
	//if (strFileName == _T(""))
	//{
	//	return;
	//}
	//if (_T(".sor") !=strFileName.Right(4) && _T(".SOR") !=strFileName.Right(4))//����sor�ļ�wcq2011.12.27
	//{
	//	return;
	//}

	////���ļ��м���Ĭ�ϲ���������2000
	//m_curABWave=2000;
	//CCurveData* pCurveData = m_pMapCurveData[2000];
	//if (pCurveData != NULL)
	//{
	//	delete pCurveData;
	//	pCurveData = NULL;
	//}
	//pCurveData = new CCurveData();

	//m_pMapCurveData[2000] = pCurveData;

	//CSorFile sorFile;
	////int iiii=sorFile.readSorFile(strFileName, pCurveData->m_TestResult);
	//if(1 != sorFile.readSorFile(strFileName, pCurveData->m_TestResult))//wcq2011.12.01
	//{
	//	return;
	//}
	//pCurveData->Length = pCurveData->DataSpacing * pCurveData->DataPointsNum;//�������ߣ����ˣ����ȡ�wcq2011.12.27
	////for (int i=0;i<pCurveData->DataPointsNum;i++)
	////{
	////	DEBUGMSG(true,(_T("%d \n"),pCurveData->pDataPoints[i]));
	////}
	//if (pCurveData->DataPointsNum != 1)//��һ��(DataPointsNum��ʼΪ1)�����ݵ�����ǰ���,��������ռ�
	//{
	//	if (NULL != pCurveData->ptDrawData)
	//	{
	//		delete[] pCurveData->ptDrawData;
	//	}
	//	//if (NULL != pCurveData->pDataPoints)
	//	//{
	//	//	delete[] pCurveData->pDataPoints;
	//	//}
	//}

	//if (NULL == pCurveData->pDataCurve)
	//{
	//	pCurveData->pDataCurve= new CPoint[COUNT_POINT_MAX_MIN * 2];//ÿһ����ȡ������//wcq2011.11.23
	//}

	////pCurveData->pDataPoints= new WORD[pCurveData->DataPointsNum];
	//pCurveData->ptDrawData=new CPoint[pCurveData->DataPointsNum];
	////pCurveData->DataPointsNum = pCurveData->DataPointsNum;
	///*

	//if (0 == m_nZoomCount)//���Ϊ0��δ���й��������Ŵ�������
	//{
	//ReCalZoomParas();//��һ�������ݵ��������¼���������ز�����//wcq2011.09.23
	//}
	//pCurveData->Length = Get_XCoord2nf(pCurveData->DataPointsNum,pCurveData);//�õ����˳���2011.11.25
	//SetLowerStandard(pCurveData);
	//ReCalABLabelPos();//ÿ�μ��ػ����һ���µ���������ʱ���¼���A/B��λ��//wcq2011.09.13
	//SetLowerStandard(pCurveData);//����CCurveData�е�IsLowerStandard�������Է���������ֱ��������.wcq2011.09.28

	////DEBUGMSG(true,(_T("OnDataReady VerticalZoom HorizonZooms\n")));
	//VerticalZoom();//�õ���ֱ����Χwcq2011.09.28
	//HorizonZooms();//ˮƽ���ź���������������������ߵ�MaxPoint/StartPoint //wcq2011.09.14

	//*
	//BOOL bMaxDistance = pCurveData->IsMaxLength;//����ȷ���Ƿ�Ҫ���²������ߵ�����ͼ�����ص����߱Ȳ��Գ�������Ҫ���¼����������ߵ�����ͼ����wcq2011.12.14
	//ReCalZoomParas();//��һ�������ݵ��������¼���������ز�����//wcq2011.11.25
	////pCurveData->Length = Get_XCoord(pCurveData->DataPointsNum,pCurveData);//�õ����˳��Ȼ��ڶ�ȡ��ʱ��ֵ2011.11.25
	//ReCalABLabelPos();//ÿ�μ��ػ����һ���µ���������ʱ���¼���A/B��λ��//wcq2011.09.13
	//SetLowerStandard(pCurveData);//����CCurveData�е�IsLowerStandard�������Է���������ֱ��������.wcq2011.09.28
	//VerticalZoom();//�õ���ֱ����Χwcq2011.09.28
	////l2=::GetTickCount() -l;
	//HorizonZooms();//ˮƽ���ź���������������������ߵ�MaxPoint/StartPoint //wcq2011.09.14
	////GetThumbData(pCurveData);//ÿ�յ�һ�������ݣ������������ڻ�������ͼ�ĵ�.wcq2011-11.24
	//if (!(bMaxDistance && pCurveData->IsMaxLength))//��������������ǰ�󣬵�ǰ���ص������Ƿ�Ϊ��������߱�ʾ��һ������Ҫ���¼�����������ͼ����wcq2011.12.14
	//{
	//	GetAllThumbData();
	//}
	//else//ֻ��Ҫ���㵱ǰ���ߵ�����ͼ
	//{
	//	GetThumbData(pCurveData);//ÿ�յ�һ�������ݣ������������ڻ�������ͼ�ĵ�.wcq2011-11.24
	//}
	//if (INVALID_WAVELENGTH == m_dwWaveSelected)
	//{
	//	m_dwWaveSelected = 2000;
	//}
	//GetDataCurve(pCurveData);//wcq2011-11.24
	////InitialEventListFromNode(); //��ʼ���¼�����Ϣ
	////EventIsSelected(m_nPointA);
	////InvalidateRect(m_rectCurve);//wcq2011.09.13
	//InvalidateAll();//wcq2011.11.16
//}

//�¼����
void CSorDrawStatic::EventCursor()
{
	//if (!HasTested())//��û���Բ����ƶ�//wcq2012.01.05
	//{
	//	return;
	//}
	//m_bEventCursor = TRUE;
	//int iSelectPoint=-1;
	//POSITION pos=m_lstEventList.GetFirstSelectedItemPosition();
	//while(pos)
	//{
	//	iSelectPoint = m_lstEventList.GetNextSelectedItem(pos);
	//	break;
	//}
	//if (-1 == iSelectPoint)//��ֹδѡ�� wcq2012.03.15
	//{
	//	if ( 0==m_lstEventList.GetItemCount())
	//	{
	//		return;
	//	}
	//	iSelectPoint = 0;
	//	m_lstEventList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);//
	//}
	////m_nPointA = m_CArrayEventInfoFromFile[iSelectPoint + 1].nLocation;//c wcq 2011.12.16
	//std::vector<EventInfo> ve = m_pMapCurveData[m_dwWaveTesting]->m_TestResult.EventList.vEventInfo;//�ٶ�??//c wcq 2011.12.16
	////InvalidateRect(&m_rectCurve);
	//int eventCount = m_pMapCurveData[m_dwWaveTesting]->m_TestResult.EventList.vEventInfo.size();
	//if (0 == eventCount)//���¼���wcq2011.12.16
	//{
	//	return;
	//}
	//if (iSelectPoint == eventCount -1)//���һ��//wcq2011.12.16
	//{
	//	iSelectPoint = 0;
	//}
	//else//�������һ�����ѹ���Ƶ���һ���¼���//wcq2011.12.16
	//{
	//	iSelectPoint++;
	//}
	//m_lstEventList.SetItemState(iSelectPoint, LVIS_SELECTED, LVIS_SELECTED);
	//m_nPointA = ve[iSelectPoint].index;
	//InvalidateAll();//wcq2011.11.16
}

LRESULT CSorDrawStatic::OnDataReady(WPARAM wParam,LPARAM lParam)
{
	//	//DEBUGMSG(TRUE,(_T("OnDataReady  ****************enter \n")));
	//#ifdef TRACE_FIND_MEMORY
	//	GlobalMemoryStatus(&buffer);//wcq
	//	DEBUGMSG(TRUE,(_T("OnDataReady enter mem: %d\n"),buffer.dwTotalPhys-buffer.dwAvailPhys));
	//#endif
	//	///m_nCount++;//wcq2012.03.09
	//	long l=GetTickCount();//?????????????????
	//	m_bBusy=TRUE;
	//	INT nDataLenMsg = (int)lParam;
	//	if(0 == nDataLenMsg)
	//	{
	//		return 1;
	//	}
	//	//if(NULL != m_pRecvbuf)//2011.09.21
	//	//{
	//	//	delete[] m_pRecvbuf;
	//	//}
	//	if(NULL == m_pRecvbuf)//��һ��
	//	{
	//		m_pRecvbuf = new WORD[nDataLenMsg + COUNT_READ_DATA_OTHER];//2011.09.21//nDataLenMsg��������������wcq2011.12.05
	//#ifdef TRACE_FIND_MEMORY
	//		m_pMainDlg->SetStatusBarText(_T("OnDataReady m_pRecvbuf new") );
	//#endif
	//	}
	//	else if (nDataLenMsg>m_dwMaxPointCount)//��ǰ���,������������
	//	{
	//		if(NULL != m_pRecvbuf)//2011.09.21
	//		{
	//			delete[] m_pRecvbuf;
	//		}
	//		m_pRecvbuf = new WORD[nDataLenMsg + COUNT_READ_DATA_OTHER];//2011.09.21
	//#ifdef TRACE_FIND_MEMORY
	//		DEBUGMSG(true,(_T("OnDataReady m_pRecvbuf new\n")));
	//		//m_pMainDlg->SetStatusBarText(_T("OnDataReady m_pRecvbuf new else") );
	//#endif
	//	}
	//	//if (1310 == m_nWaveToDevice)
	//	//{
	//	//	int a=0;
	//	//}
	//	//DEBUGMSG(true,(_T("OnDataReady m_pMapCurveData size:%d  m_curABWave:%d,m_dwWaveTesting:%d\n"),m_pMapCurveData.size(),m_curABWave,m_dwWaveTesting));
	//	//��ȡ��������
	//	if (m_dwWaveTesting == 0)
	//		return -1;
	//	if (-1 == m_curABWave)
	//	{
	//		m_curABWave = m_dwWaveTesting;
	//	}
	//	CCurveData* pCurveData = m_pMapCurveData[m_dwWaveTesting];
	//	//DEBUGMSG(true,(_T("OnDataReady m_pMapCurveData size:%d  m_curABWave:%d,m_dwWaveTesting:%d\n"),m_pMapCurveData.size(),m_curABWave,m_dwWaveTesting));
	//	///*if (pCurveData != NULL)//wcq2011.09.16
	//	//{
	//	//	delete pCurveData;
	//	//	pCurveData = NULL;
	//	//}
	//	//pCurveData = new CCurveData();*/
	//
	//	if (NULL == pCurveData)//wcq2011.09.16
	//	{
	//		//delete pCurveData;
	//		//pCurveData = NULL;
	//		pCurveData = new CCurveData();//wcq2011.09.16
	//		///pCurveData->pDataPoints= new WORD[nDataLenMsg];
	//		m_pMapCurveData[m_dwWaveTesting] = pCurveData;//wcq2011.09.16
	//#ifdef TRACE_FIND_MEMORY
	//		//m_pMainDlg->SetStatusBarText(_T("OnDataReady new CCurveData") );
	//		DEBUGMSG(true,(_T("OnDataReady CCurveData new\n")));
	//#endif
	//	}
	//
	//	if (pCurveData->DataPointsNum < nDataLenMsg)//��һ��(DataPointsNum��ʼΪ1)�����ݵ�����ǰ���,��������ռ�
	//	{
	//		if (NULL != pCurveData->ptDrawData)
	//		{
	//			delete[] pCurveData->ptDrawData;
	//			pCurveData->ptDrawData=NULL;
	//		}
	//		if (NULL != pCurveData->pDataPoints)
	//		{
	//			delete[] pCurveData->pDataPoints;
	//			pCurveData->pDataPoints=NULL;
	//		}
	//		//if (NULL == pCurveData->pDataCurve)
	//		//{
	//		//	pCurveData->pDataCurve= new WORD[COUNT_POINT_MAX_MIN];//ÿһ����ȡ������//wcq2011.09.30
	//		//}
	//		//if (NULL == pCurveData->pThumbData)//wcq2011.11.25����ͼ����
	//		//{
	//		//	pCurveData->pThumbData = new CPoint[m_rectThumbnail.Width() * 2];//x��ѡ����
	//		//}
	//
	//#ifdef TRACE_FIND_MEMORY
	//		//m_pMainDlg->SetStatusBarText(_T("OnDataReady new CCurveData") );
	//		DEBUGMSG(true,(_T("OnDataReady pDataPoints new\n")));
	//#endif
	//
	//		pCurveData->ptDrawData=new CPoint[nDataLenMsg];
	//		pCurveData->pDataPoints= new WORD[nDataLenMsg];
	//		//pCurveData->ptDrawData=new CPoint[nDataLenMsg+4];//wcq2012.01.12
	//		pCurveData->DataPointsNum = nDataLenMsg;
	//	}
	//	///ASSERTMSG(_T("memery zero\n"),(NULL == m_pRecvbuf));
	//	///	DEBUGMSG(true,(_T("OnDataReady before readData test is NULL\n")));
	//	////m_pRecvbuf[0] = 0;
	//	//DEBUGMSG(true,(_T("OnDataReady before readData\n")));
	//
	//	CString ss;
	//	//if (!m_frmDriver.readData(m_pRecvbuf, nDataLenMsg ))//wcq2011.12.05
	//	if (!m_frmDriver.readData(m_pRecvbuf, nDataLenMsg  + COUNT_READ_DATA_OTHER))//wcq2011.12.05
	//		return -1; //����ʧ��
	//
	//	//wcq2012.02.07���//////////////////////////////////////////////////////
	//	m_nDataLenMsg = nDataLenMsg;
	//	::SetEvent(hEeventDealData);
	//	///ConnectDeviceToTest();//wcq2012.02.07//cwcq2012.02.21
	//	//DEBUGMSG(TRUE,(_T("OnDataReady  ****************exit \n")));
	//	return 1;
	//	//wcq2012.02.07��ӽ���//////////////////////////////////////////////////////
	//	//����Ϊ��ǰ�ģ���Ϊ�ŵ�һ���߳��ﴦ��//wcq2012.02.07
	//
	//
	//
	//
	//	////////////////////////////////////////////////////////////////////////////////////////////////
	//	//DEBUGMSG(true,(_T("OnDataReady after readData nDataLenMsg:%d\n"),nDataLenMsg));
	//	//��ȡ������ǰ��λ����
	//	pCurveData->DataType = *(m_pRecvbuf); //��������
	//	pCurveData->WaveLength = *(m_pRecvbuf + 1); //����
	//	pCurveData->PulseWidth = *(m_pRecvbuf + 2); //����
	//	pCurveData->TestMode = *(m_pRecvbuf + 3); //����ģʽ
	//	pCurveData->Frequency = *(m_pRecvbuf + 4); //����Ƶ��
	//	pCurveData->Relay = *(m_pRecvbuf + 5); //�̵���wcq2011.12.06
	//	pCurveData->Joint1 = *(m_pRecvbuf + 6); //ƴ�ӵ�1 wcq2011.12.05
	//	pCurveData->Joint2 = *(m_pRecvbuf + 7); //ƴ�ӵ�2 wcq2011.12.05
	//	pCurveData->DataSerialNum = *(m_pRecvbuf + 8); //���ݰ����
	//	pCurveData->DataPointsNum = *(m_pRecvbuf + 9); //��������
	//	//pCurveData->pDataPoints = m_pRecvbuf + 7;
	//	//memcpy(pCurveData->pDataPoints,m_pRecvbuf+9,nDataLenMsg * sizeof(WORD));//wcq2011.09.21
	//	memcpy(pCurveData->pDataPoints,m_pRecvbuf+COUNT_READ_DATA_OTHER,nDataLenMsg * sizeof(WORD));//wcq2011.12.06
	//	///	DEBUGMSG(true,(_T("On dataready time memcpy\n")));
	//	//����¼��б�wcq2011.11.15
	//	pCurveData->EventList.vEventInfo.clear();//wcq2011.11.15
	//
	//
	//#ifdef TRACE_FIND_MEMORY
	//	GlobalMemoryStatus(&buffer);//wcq
	//	DEBUGMSG(TRUE,(_T("OnDataReady after read data: %d\n"),buffer.dwTotalPhys-buffer.dwAvailPhys));
	//#endif
	//	//DEBUGMSG(true,(_T("OnDataReady after readData pCurveData->DataPointsNum:%d\n"),pCurveData->DataPointsNum));
	//	m_nMaxPoint = pCurveData->DataPointsNum; //��������
	//
	//	if (nDataLenMsg>m_dwMaxPointCount)//wcq.2011.09.23
	//	{
	//		m_dwMaxPointCount=nDataLenMsg;//wcq2011.09.21
	//	}
	//	BOOL bMaxDistance = pCurveData->IsMaxLength;//����ȷ���Ƿ�Ҫ���²������ߵ�����ͼ��������߼���ǰ���Ƿ�Ϊ��������߱�ʾ��һ��������Ҫ���¼����������ߵ�����ͼ����wcq2011.12.14
	//
	//	pCurveData->Length = Get_XCoord2nf(pCurveData->DataPointsNum,pCurveData);//�õ����˳���2011.11.25//wcq2011.12.19
	//	pCurveData->DataSpacing = Get_XCoord2nf(1,pCurveData);//
	//
	//	if (0 == m_nZoomCount)//���Ϊ0��δ���й��������Ŵ�������
	//	{
	//		//if (1310 == m_nWaveToDevice)
	//		//{
	//		//	int a=0;
	//		//}
	//		ReCalZoomParas();//��һ�������ݵ��������¼���������ز�����//wcq2011.09.23
	//	}
	//	///	DEBUGMSG(true,(_T("On dataready time Get_XCoord2nf\n")));
	//	//pCurveData->Length = Get_XCoord2nf(pCurveData->DataPointsNum,pCurveData);//�õ����˳���2011.11.25//�Ƶ�ǰ��wcq2011.12.19
	//	//pCurveData->DataSpacing = Get_XCoord2nf(1,pCurveData);//�Ƶ�ǰ��wcq2011.12.19
	//	SetLowerStandard(pCurveData);
	//	ReCalABLabelPos();//ÿ�μ��ػ����һ���µ���������ʱ���¼���A/B��λ��//wcq2011.09.13
	//	SetLowerStandard(pCurveData);//����CCurveData�е�IsLowerStandard�������Է���������ֱ��������.wcq2011.09.28
	//
	//	///	DEBUGMSG(true,(_T("On dataready time VerticalZoom\n")));
	//	//DEBUGMSG(true,(_T("OnDataReady VerticalZoom HorizonZooms\n")));
	//	VerticalZoom();//�õ���ֱ����Χwcq2011.09.28
	//	HorizonZooms();//ˮƽ���ź���������������������ߵ�MaxPoint/StartPoint //wcq2011.09.14
	//	//DEBUGMSG(true,(_T("OnDataReady VerticalZoom HorizonZooms2\n")));
	//	//l=::GetTickCount();
	//	//GetThumbData(pCurveData);//ÿ�յ�һ�������ݣ������������ڻ�������ͼ�ĵ�.wcq2011-11.10	//wcq2011.12.14
	//	if (!(bMaxDistance && pCurveData->IsMaxLength))//��������������ǰ�󣬵�ǰ���ص������Ƿ�Ϊ��������߱�ʾ��һ������Ҫ���¼�����������ͼ����wcq2011.12.14
	//	{
	//		GetAllThumbData();
	//	}
	//	else//ֻ��Ҫ���㵱ǰ���ߵ�����ͼ
	//	{
	//		GetThumbData(pCurveData);//ÿ�յ�һ�������ݣ������������ڻ�������ͼ�ĵ�.wcq2011-11.24
	//	}
	//	GetDataCurve(pCurveData);//��CurveData�е� pDataCurve������ֵ���õ��������Ļ�����������wcq2011.11.21
	//	//DEBUGMSG(true,(_T("On dataready time:%d\n"),GetTickCount() - l));
	//	//InvalidateRect(m_rectCurve);//wcq2011.09.13
	//	//InvalidateRect(m_rectThumbnail);
	//	//InvalidateRect(m_rectSubInfo);
	//	//InvalidateAll();//wcq2011.11.16
	//	m_bBusy=FALSE;
	//	//GetEventList();//ƽ���������ͨ��dll�ļ��㹫ʽ�õ��¼��б�wcq2011-12.05
	//
	//#ifdef TRACE_FIND_MEMORY
	//	GlobalMemoryStatus(&buffer);//wcq
	//	DEBUGMSG(TRUE,(_T("OnDataReady  end: %d\n"),buffer.dwTotalPhys-buffer.dwAvailPhys));
	//#endif
	//
	//	Invalidate(FALSE);//wcq2011.12.02
	//	long ll=GetTickCount()-l;//?????????????????
	//	//DEBUGMSG(TRUE,(_T("OnDataReady  end time: %d\n"),ll));
	//	if (!m_bStopTest)
	//	{
	//		//ConnectDeviceToTest(TRUE);//��ʼ����
	//		//ConnectDeviceToTest();//��ʼ����wcq2011.12.13
	//	}
	//	else//ֹͣ����,�õ��¼��б�wcq2011.11.15
	//	{
	//		long l=GetTickCount();
	//		int ll2=GetTickCount()-l;
	//		//DEBUGMSG(TRUE,(L"gorbal time(ms):%d\n",ll2));
	//	}
	//
	//return 1;
//}


//���ļ�
void CSorDrawStatic::FillCurveDataFromFile(TestResultInfo* pCurveData)
{
	if (NULL == pCurveData)
	{
		return;
	}
	pCurveData->IsLoadFromFile = 1; //���ļ�����
	pCurveData->IsMaxLength = 1;
	pCurveData->Length = pCurveData->DataSpacing * pCurveData->DataPointsNum;//�������ߣ����ˣ����ȡ�wcq2011.12.27
	if (pCurveData->DataPointsNum != 1)//��һ��(DataPointsNum��ʼΪ1)�����ݵ�����ǰ���,��������ռ�
	{
		pCurveData->MaxPoint = pCurveData->DataPointsNum;
	}

	if (NULL != pCurveData->pDataCurve)
	{
		delete[] pCurveData->pDataCurve;
		pCurveData->pDataCurve = NULL;
	}
	pCurveData->pDataCurve= new CPoint[pCurveData->DataPointsNum];

	if (NULL != pCurveData->ptDrawData)
	{
		delete[] pCurveData->ptDrawData;
		pCurveData->ptDrawData = NULL;
	}
	pCurveData->ptDrawData=new CPoint[pCurveData->DataPointsNum];

	long a=GetTickCount();
	BOOL bMaxDistance = pCurveData->IsMaxLength;//����ȷ���Ƿ�Ҫ���²������ߵ�����ͼ�����ص����߱Ȳ��Գ�������Ҫ���¼����������ߵ�����ͼ����wcq2011.12.14
	ReCalZoomParas();//��һ�������ݵ��������¼���������ز�����//wcq2011.11.25
	//pCurveData->Length = Get_XCoord(pCurveData->DataPointsNum,pCurveData);//�õ����˳��Ȼ��ڶ�ȡ��ʱ��ֵ2011.11.25
	ReCalABLabelPos();//ÿ�μ��ػ����һ���µ���������ʱ���¼���A/B��λ��//wcq2011.09.13
	SetLowerStandard(pCurveData);//����CCurveData�е�IsLowerStandard�������Է���������ֱ��������.wcq2011.09.28
	VerticalZoom();//�õ���ֱ����Χwcq2011.09.28
	//l2=::GetTickCount() -l;
	HorizonZooms();//ˮƽ���ź���������������������ߵ�MaxPoint/StartPoint //wcq2011.09.14
	NewTestInit(pCurveData);
	if (!(bMaxDistance && pCurveData->IsMaxLength))//��������������ǰ�󣬵�ǰ���ص������Ƿ�Ϊ��������߱�ʾ��һ������Ҫ���¼�����������ͼ����wcq2011.12.14
	{
		MAIN->m_pThumbFormView->GetAllThumbData();
	}
	else//ֻ��Ҫ���㵱ǰ���ߵ�����ͼ
	{
		MAIN->m_pThumbFormView->GetThumbData(pCurveData);//ÿ�յ�һ�������ݣ������������ڻ�������ͼ�ĵ�.wcq2011-11.24
	}
	if (INVALID_WAVELENGTH == m_dwWaveSelected)
	{
		m_dwWaveSelected = 2000;
	}
	InvalidateAll();//wcq2011.11.16
	TRACE(_T("222 %d\n"),GetTickCount()-a);
	a=GetTickCount();
	GetDataCurve(pCurveData);//wcq2011-11.24
	TRACE(_T("333 %d\n"),GetTickCount()-a);
}

//���¼�����������
void CSorDrawStatic::ResetCurveData()
{
	ReCalZoomParas();//��һ�������ݵ��������¼���������ز�����//wcq2011.11.25
	ReCalABLabelPos();//ÿ�μ��ػ����һ���µ���������ʱ���¼���A/B��λ��//wcq2011.09.13
	VerticalZoom();//�õ���ֱ����Χwcq2011.09.28
	HorizonZooms();//ˮƽ���ź���������������������ߵ�MaxPoint/StartPoint //wcq2011.09.14
	InvalidateAll();
}

void CSorDrawStatic::DealDataDetail()
{
	INT nDataLenMsg = m_nDataLenMsg;
	TestResultInfo* pCurveData = g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo;
	pCurveData->DataType = *(m_pRecvbuf); //��������
	pCurveData->WaveLength = *(m_pRecvbuf + 1); //����
	pCurveData->PulseWidth = *(m_pRecvbuf + 2); //����
	pCurveData->TestMode = *(m_pRecvbuf + 3); //����ģʽ
	pCurveData->Frequency = *(m_pRecvbuf + 4); //����Ƶ��
	pCurveData->Relay = *(m_pRecvbuf + 5); //�̵���wcq2011.12.06
	pCurveData->Joint1 = *(m_pRecvbuf + 6); //ƴ�ӵ�1 wcq2011.12.05
	pCurveData->Joint2 = *(m_pRecvbuf + 7); //ƴ�ӵ�2 wcq2011.12.05
	pCurveData->DataSerialNum = *(m_pRecvbuf + 8); //���ݰ����
	pCurveData->DataPointsNum = *(m_pRecvbuf + 9); //��������
	memcpy(pCurveData->pDataPoints,m_pRecvbuf+COUNT_READ_DATA_OTHER,nDataLenMsg * sizeof(WORD));
	//����¼��б�wcq2011.11.15
	pCurveData->EventList.vEventInfo.clear();//wcq2011.11.15

	if(INVALID_MAX_POINT == m_nMaxPoint)//��һ�θ�ֵ
	{
		m_nMaxPoint = pCurveData->DataPointsNum; //��������//wcq2012.02.20
	}
	if (nDataLenMsg>m_dwMaxPointCount)//wcq.2011.09.23
	{
		m_dwMaxPointCount=nDataLenMsg;//wcq2011.09.21
	}
	BOOL bMaxDistance = pCurveData->IsMaxLength;//����ȷ���Ƿ�Ҫ���²������ߵ�����ͼ��������߼���ǰ���Ƿ�Ϊ��������߱�ʾ��һ��������Ҫ���¼����������ߵ�����ͼ����wcq2011.12.14

	if (m_bFirstRead)//��һ���յ����� wcq2012.02.22
	{
		pCurveData->Length = Get_XCoord2nf(pCurveData->DataPointsNum,pCurveData);//�õ����˳���2011.11.25//wcq2011.12.19
		pCurveData->DataSpacing = Get_XCoord2nf(1,pCurveData);//
	}

	SetLowerStandard(pCurveData);
	ReCalABLabelPos();//ÿ�μ��ػ����һ���µ���������ʱ���¼���A/B��λ��//wcq2011.09.13
	SetLowerStandard(pCurveData);//����CCurveData�е�IsLowerStandard�������Է���������ֱ��������.wcq2011.09.28

	if (m_bFirstRead)//��һ���յ����� wcq2012.02.21
	{
		m_bFirstRead = FALSE;
		ReCalZoomParas();//��һ�������ݵ��������¼���������ز�����//wcq2011.09.23//wcq2012.02.22
		VerticalZoom();//�õ���ֱ����Χ
		HorizonZooms();//ˮƽ���ź���������������������ߵ�MaxPoint/StartPoint //
		NewTestInit(pCurveData);//���²��Ժ��ʼ��һЩ����
	}

	m_bBusy=FALSE;

	m_bNeedHorizonZoom = FALSE;//wcq2012.02.20

	Invalidate(FALSE);//wcq2011.12.02
}

//ÿ�յ�һ�������ݣ������������ڻ������ߵĵ�.wcq2011.11.21
void CSorDrawStatic::GetDataCurve(TestResultInfo* pCurveData)
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
//ÿ�μ��ػ����һ���µ���������ʱ���¼���A/B��λ�ã���ȷ�����������߲�������������m_curABWave.
//����и����������ߵ����������¼������Ų���
//wcq 2011.0913
void CSorDrawStatic::ReCalABLabelPos()
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

////���»س������߻ص�δ����/ƽ��״̬ 
void CSorDrawStatic::OnVKReturn()
{
	//ˮƽ���Ż�ԭ
	m_iHorizonZoomNum = 0;
	HorizonZooms();//wcq2011.09.13

	//��ֱ����ԭ
	for(int nCount = 0; nCount < g_sorFileArray.totalCurveNumber; nCount++)
	{
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
		if (pCurve)//wcq2012.01.12
		{
			pCurve->YOffset = 0;
		}//eif
	}//efor

	//��ֱ�������Ż�ԭ
	m_iVerticalZoomNum = 0;
	VerticalZoom();

	//�ػ�
	InvalidateAll();
}

/*
***************************************************************
��������:             OnLButtonDown                                                                              
����˵��:             ������꣬�ж�AB���Ƿ�ѡ�У��ǣ�m_bSelecedAB=TRUE����
������갴�µĵ㣬��Ϊ����Ŵ���ο��һ������                                                                          
��ڲ���:             UINT nFlags, CPoint point                                                                                      
���ڲ���:             ��                                                                                        
����ʱ��:             2012-06-28                                                                 
Write By:             XJF                                                                                          
***************************************************************
*/
void CSorDrawStatic::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//û������ʱֱ�ӷ���
	if (g_sorFileArray.totalCurveNumber == 0)
		return;

	if (!PtInRect(&m_rectCoord, point))
	{
		return;
	}

	if(IsLineABSeleced( point))
	{
		m_bZoomOK = FALSE;
		InvalidateRect(m_rectCurve);
		//InitialReturnInfo();  //���������A�˻�B��Ҫ������Ϣ
		EventIsSelected(point.x); //ѡ�б�˺��¼����еĶ�Ӧ�¼���ѡ��
	}
	else
	{
		//m_nSelectCursor = CURSOR_SELECT::CURSOR_NONE;
		m_bZoomOK = TRUE;
		m_PLMouseDown = point;
	}
	CButton::OnLButtonDown(nFlags, point);
}

/*
***************************************************************
��������:             OnLButtonUp                                                                              
����˵��:            ��굯�𣬶�����϶������в����ľ��ο�ѡ���ĵ���зŴ󣬶������׽�����ͷ�                                                  
��ڲ���:             UINT nFlags, CPoint point                                                                                      
���ڲ���:             ��                                                                                        
����ʱ��:             2012-06-28                                                                 
Write By:             XJF                                                                                          
***************************************************************
*/
void CSorDrawStatic::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (g_sorFileArray.totalCurveNumber == 0) //û������ʱ
	{
		m_RectSelected.SetRectEmpty();
		InvalidateRect(&m_rectCurve);
		return;
	}

	if (m_RectSelected.Width() > 15 && m_bZoomOK)
	{
		if (g_sorFileArray.totalCurveNumber == 0)
			return;
		TestResultInfo* pData = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;

		int iStartPoint;//����Ŵ�ǰԭʼ���������λ��
		int iMaxPoint;
		float fTemptDB;
		float fStartDB,fMaxDB;//����Ŵ�ǰԭʼ���������λ��		
		fStartDB = m_fStartDB;
		fMaxDB = m_fMaxDB;
		fTemptDB = m_fStartDB;
		iStartPoint = m_nStartPoint;
		iMaxPoint = m_nMaxPoint;

		m_nStartPoint=m_nStartPoint+(m_nMaxPoint-m_nStartPoint)*(m_RectSelected.left-m_rectCoord.left)/m_rectCoord.Width();
		m_nMaxPoint=iStartPoint+(m_nMaxPoint-iStartPoint)*(m_RectSelected.right-m_rectCoord.left)/m_rectCoord.Width();

		m_nStartPoint=m_nStartPoint<m_nMaxPoint?m_nStartPoint:m_nMaxPoint;
		if (m_nStartPoint==m_nMaxPoint) 
		{		
			m_nStartPoint=m_nMaxPoint-1;	

		}
		m_nMaxPoint=m_nMaxPoint>(unsigned int)iMaxPoint?(unsigned int)iMaxPoint:m_nMaxPoint;

		m_fStartDB=m_fStartDB+(m_fMaxDB-m_fStartDB)*(m_rectCoord.bottom-m_RectSelected.bottom)/m_rectCoord.Height();
		m_fMaxDB=fTemptDB+(m_fMaxDB-fTemptDB)*(m_rectCoord.bottom-m_RectSelected.top)/m_rectCoord.Height();

		if (int(m_fMaxDB/(m_fMaxDB-m_fStartDB))>20)
		{
			m_fMaxDB=fMaxDB;
			m_fStartDB=fStartDB;
		}
		m_iHorizonZoomNum=2;
		m_iVerticalZoomNum=2;

		//�������û�ͼ�����С
		if (m_iHorizonZoomNum == 0) //������
		{
			m_nDrawWidth = m_rectCurve.Width();
		}
		else
		{
			m_nDrawWidth = m_rectCurve.Width() * m_iHorizonZoomNum;
		}
		//����ˮƽ������
		SetHorScroll();

		//�������û�ͼ�����С
		if (m_iVerticalZoomNum == 0) //������
		{
			m_fCoeff = 1.0;
			m_nDrawHeight = m_rectCurve.Height();
		}
		else
		{
			m_fCoeff = (m_fMaxDB - m_fStartDB) / (M_FMAXDB0 - M_FSTARTDB0);
			m_nDrawHeight = m_rectCurve.Height()/m_fCoeff;
		}
		SetVerScoll();
	}
	
	InvalidateRect(&m_rectCurve);
	MAIN->m_pThumbFormView->Invalidate();//�ػ�����ͼ
	m_RectSelected.SetRectEmpty();
	//m_nSelectCursor = CURSOR_SELECT::CURSOR_NONE;//���ú�m_bIsLineSelected�غϣ�Ϊ�˷�����⣬�ʴ�û��ɾ��
	m_bZoomOK = FALSE;	
	m_PLMouseDown = NULL;
	CButton::OnLButtonUp(nFlags, point);
}

/*
***************************************************************
��������:             IsLineABSeleced                                                                              
����˵��:             ������꣬�����A��B�߸���,AB�߱�ѡ�в�����TRUE�����򷵻�FALSE                                                                       
��ڲ���:             UINT nFlags, CPoint point                                                                                      
���ڲ���:             ��                                                                                        
����ʱ��:             2012-06-28                                                                 
Write By:             XJF                                                                                          
***************************************************************
*/
BOOL CSorDrawStatic::IsLineABSeleced(CPoint point)
{
	if (!PtInRect(&m_rectCoord, point))
	{
		return FALSE;
	}

	TestResultInfo* pData =  g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo;
	//ȷ��m_nPointA ��m_nPoint��Խ��
	if (m_nPointA > pData->DataPointsNum-1)
	{
		m_nPointA = pData->DataPointsNum-1;
	}
	if (m_nPointB > pData->DataPointsNum-1)
	{
		m_nPointB = pData->DataPointsNum-1;
	}
	int SpaceAR = pData->ptDrawData[m_nPointA].x+10;
	int SpaceAL = pData->ptDrawData[m_nPointA].x-10;
	int SpaceBR = pData->ptDrawData[m_nPointB].x+10;
	int SpaceBL = pData->ptDrawData[m_nPointB].x-10;

	/*ѡ��A*/ 

	if ((point.x < SpaceAR && point.x > SpaceAL) &&
		!(point.x < SpaceBR && point.x > SpaceBL))
	{
		//m_nSelectCursor = CURSOR_SELECT::CURSOR_A; //��ʾA�߱�ѡ��
		return TRUE;
	}
	/*ѡ��B*/ 
	else if (!(point.x < SpaceAR && point.x > SpaceAL)&&
		(point.x < SpaceBR && point.x > SpaceBL))
	{		
		//m_nSelectCursor = CURSOR_SELECT::CURSOR_B;//��ʾB�߱�ѡ��
		return TRUE;
	}
	/*ͬʱѡ��AB*/ 
	else if((point.x < SpaceAR && point.x > SpaceAL)&&
		(point.x < SpaceBR && point.x > SpaceBL))
	{
		/*�ƶ�������AB�����ʱ�������ǰѡ����A*/ 
		//m_nSelectCursor = CURSOR_SELECT::CURSOR_AB;//��ʾAB�߱�ͬʱѡ��
		return TRUE;
	}
	return FALSE;
}

/*
***************************************************************
��������:             OnMouseMove                                                                              
����˵��:             �϶���꣬�����AB�߱�ѡ�У����ƶ�AB�ߣ���������һ�����ο���굯�����
����ʱ���Ծ��ο��еĵ���зŴ�                                                             
��ڲ���:             UINT nFlags, CPoint point                                                                                      
���ڲ���:             ��                                                                                        
����ʱ��:             2012-06-28                                                                 
Write By:             XJF                                                                                          
***************************************************************
*/
void CSorDrawStatic::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (g_sorFileArray.totalCurveNumber == 0) //û�����ߴ�
		return;

	if (nFlags == MK_LBUTTON && PtInRect(&m_rectCoord,point))
	{
		if (!m_bZoomOK)  //���AB�߿����϶�
		{
			if (m_nSelectCursor == CURSOR_SELECT::CURSOR_A) //���ѡ��A��
			{
				m_nPointA = m_nStartPoint+(point.x - m_rectCoord.left)*(m_nMaxPoint - m_nStartPoint) / m_rectCoord.Width();
				EventIsSelected(m_nPointA);
				InvalidateRect(m_rectCoord);
				m_point_minus=(m_nPointB-m_nPointA);//AB��֮�����Ծ��룬zll 2013.1.6
			}
			else if (m_nSelectCursor == CURSOR_SELECT::CURSOR_B)/*���ѡ��B��*/ 
			{
				m_nPointB = m_nStartPoint + (point.x - m_rectCoord.left)*(m_nMaxPoint - m_nStartPoint) / m_rectCoord.Width();

				EventIsSelected(m_nPointA);
				InvalidateRect(m_rectCoord);
				m_point_minus=(m_nPointB-m_nPointA);//AB��֮�����Ծ��룬zll 2013.1.6
			}
			else if (m_nSelectCursor == CURSOR_SELECT::CURSOR_AB)/*���ѡ��AB��*/ 
			{
				m_nPointA = m_nStartPoint+(point.x - m_rectCoord.left)*(m_nMaxPoint - m_nStartPoint) / m_rectCoord.Width();
				//m_nPointBֵ�����AB��֮�����Ծ���m_point_minus��zll 2013.1.6
				m_nPointB = m_nStartPoint + m_point_minus + (point.x - m_rectCoord.left)*(m_nMaxPoint - m_nStartPoint) / m_rectCoord.Width();
				InvalidateRect(m_rectCoord);
			}
		}

		else //AB�����϶����ֲ��Ŵ�
		{
			//SetCapture();
			long iTempMx = m_PLMouseDown.x;
			long iTempPx = point.x;
			long iTempMy = m_PLMouseDown.y;
			long iTempPy = point.y;
			point.x = point.x < m_rectCoord.right ? point.x : m_rectCoord.right;
			point.x = point.x > m_rectCoord.left ? point.x : m_rectCoord.left;
			point.y = point.y < m_rectCoord.bottom ? point.y : m_rectCoord.bottom;
			point.y = point.y > m_rectCoord.top ? point.y : m_rectCoord.top;


			if (iTempMx < iTempPx)//��������ƶ�
			{
				if (iTempMy < iTempPy)//to right and down
				{			      
					CRect RectTemp(m_PLMouseDown, point);
					m_RectSelected = RectTemp;

				}
				else//to right and up
				{	
					CRect RectTemp(m_PLMouseDown.x, point.y, point.x, m_PLMouseDown.y);
					m_RectSelected = RectTemp;
				}
			}
			else//��������ƶ�
			{
				if (iTempMy < iTempPy)//����
				{
					CRect RectTemp(point.x, m_PLMouseDown.y, m_PLMouseDown.x, point.y);
					m_RectSelected = RectTemp;
				}
				else//left and up
				{
					CRect RectTemp(point, m_PLMouseDown);
					m_RectSelected = RectTemp;
				}

			} 
			InvalidateRect(m_rectCoord);
		}	
	}

	CButton::OnMouseMove(nFlags, point);
}

TestResultInfo* CSorDrawStatic::GetMaxLengthCurveData()//��ȡ��������������
{
	if (g_sorFileArray.totalCurveNumber == 0)//û������
		return NULL;

	return g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo;
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
int CSorDrawStatic::GetZoomCount(int maxDistanceDataCount)
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
/*
***************************************************************
��ڲ���:
���ڲ���:
����ʱ��:2011.09.23
Write By:wcq 
����˵��:�õ����������������һ�����ŵ����ݵ����
***************************************************************
*/
void CSorDrawStatic::GetLastZoomCounts()
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
int CSorDrawStatic::GetLastZoomCount(int countPerTime,int dataCount,int zoomCount)
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
//������������δ����ʱ������ͼ����ĵ��� wcq 2011.09.21
void CSorDrawStatic::CalFullPointCount()
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
//��һ�λ��и��������������ݵ��������¼���������ز��������������GetZoomCount,CalFullPointCount,GetLastZoomCount.wcq2011.09.23
void CSorDrawStatic::ReCalZoomParas()
{
	CalMaxDistance();//�ȼ���������������ľ��벢�����������߽ṹ��Ӧ����
	m_nZoomCount=GetZoomCount(g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo->DataPointsNum);//�õ����Ŵ���
	CalFullPointCount();//������������δ����ʱ������ͼ����ĵ���
	GetLastZoomCounts();//�õ����������������һ�����ŵ����ݵ����
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
void CSorDrawStatic::SetLowerStandard(TestResultInfo* pData)
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
��ڲ���:
���ڲ���:
����ʱ��:2011.09.26
Write By:wcq 
����˵��:�鲼�����������ݣ����Ƿ�������IsLowerStandardΪ��
***************************************************************
*/
BOOL CSorDrawStatic::IsLowerStandardAll()
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
void CSorDrawStatic::CalVerticalRange(int zoomNum,float fStandPosDb)
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

void CSorDrawStatic::EventSelectUp()
{
	POSITION pos = MAIN->m_pEventFormView->m_Eventlist.GetFirstSelectedItemPosition();
	int nIndex = MAIN->m_pEventFormView->m_Eventlist.GetNextSelectedItem(pos);
	if (nIndex > 0)
	{
		nIndex--;
		MoveEventList(nIndex);
		//����ѡ����
		MAIN->m_pEventFormView->m_Eventlist.SetItemState(nIndex, LVIS_SELECTED, LVIS_SELECTED);
	}
}

void CSorDrawStatic::EventSelectDown()
{
	POSITION pos = MAIN->m_pEventFormView->m_Eventlist.GetFirstSelectedItemPosition();
	int nIndex =  MAIN->m_pEventFormView->m_Eventlist.GetNextSelectedItem(pos);
	if (nIndex >= 0 && nIndex < MAIN->m_pEventFormView->m_Eventlist.GetItemCount()-1)
	{
		nIndex++;
		MoveEventList(nIndex);
		//����ѡ����
		MAIN->m_pEventFormView->m_Eventlist.SetItemState(nIndex, LVIS_SELECTED, LVIS_SELECTED);
	}
	else
	{
		//�õ�һ��Ϊѡ����
		if (nIndex > 0)
			MAIN->m_pEventFormView->m_Eventlist.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	}
}
//ѡ�����¼��б�����»�������;wcq2011-11.15
//nIndex:��0��ʼ��Ӧ���ߵڼ����¼���
void CSorDrawStatic::MoveEventList(INT nIndex)
{
	TestResultInfo* pCurveData = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;
	if (1 == m_nSelectCursor) //ѡ��A
	{
		m_nPointA = pCurveData->EventList.vEventInfo[nIndex].index;
	}	
	else if (2 == m_nSelectCursor)////ѡ��B
	{
		m_nPointB = pCurveData->EventList.vEventInfo[nIndex].index;
	}
	else//AB
	{
		m_nPointA = pCurveData->EventList.vEventInfo[nIndex].index;
		m_nPointB = m_nPointA;
	}//eif
	HorizonZooms();//
	InvalidateAll();//wcq2011.11.16
}

void CSorDrawStatic::OnSize(UINT nType, int cx, int cy)
{
	CButton::OnSize(nType, cx, cy);

	m_nCurWidth = cx;
	m_nCurHeight = cy;
}

void CSorDrawStatic::SetHorScroll() //����ˮƽ������
{
	//ˮƽ������
	SCROLLINFO siHor;
	siHor.cbSize = sizeof(SCROLLINFO);
	siHor.fMask = SIF_ALL; 
	siHor.nMin = 0;
	siHor.nMax = m_nDrawWidth-1;//��ô����windows������Ĺ�����λ������Ϊsi.nmax-si.npage+1,������si.nmax
	siHor.nPage = m_rectCurve.Width();
	siHor.nPos = 0;

	SetScrollInfo(SB_HORZ, &siHor, TRUE);
}

void CSorDrawStatic::SetVerScoll() //������ֱ������
{
	SCROLLINFO siVer;
	siVer.cbSize = sizeof(SCROLLINFO);
	siVer.fMask = SIF_ALL; 
	siVer.nMin = 0;
	siVer.nMax = m_nDrawHeight;
	siVer.nPage = m_rectCurve.Height();
	siVer.nPos = 0;
	SetScrollInfo(SB_VERT, &siVer, TRUE);
}

void CSorDrawStatic::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nDelta;
	int nMaxPos = m_nDrawHeight - m_nCurHeight;

	switch (nSBCode)
	{
	case SB_LINEDOWN:
		if (m_nScrollPosVer >= nMaxPos)
			return;

		nDelta = min(max(nMaxPos/20,5),nMaxPos-m_nScrollPosVer);
		break;

	case SB_LINEUP:
		if (m_nScrollPosVer <= 0)
			return;
		nDelta = -min(max(nMaxPos/20,5),m_nScrollPosVer);
		break;
	case SB_PAGEDOWN:
		if (m_nScrollPosVer >= nMaxPos)
			return;
		nDelta = min(max(nMaxPos/10,5),nMaxPos-m_nScrollPosVer);
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		nDelta = (int)nPos - m_nScrollPosVer;
		break;

	case SB_PAGEUP:
		if (m_nScrollPosVer <= 0)
			return;
		nDelta = -min(max(nMaxPos/10,5),m_nScrollPosVer);
		break;

	default:
		return;
	}
	m_nScrollPosVer += nDelta;
	SetScrollPos(SB_VERT,m_nScrollPosVer,TRUE);
	//�����ƶ�����
	SCROLLINFO siVer;
	siVer.cbSize = sizeof(SCROLLINFO);

	GetScrollInfo(SB_VERT,&siVer);
	int nRange = siVer.nMax - siVer.nMin;
	float fVerBalFactor = siVer.nPos * 1.0 / siVer.nPage; //����ϵ����
	nDelta = nRange * fVerBalFactor;
	CurvesVerMove(nDelta);
	CButton::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CSorDrawStatic::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nDelta;
	int nMaxPos = m_nDrawWidth - m_nCurWidth;

	switch (nSBCode)
	{
	case SB_LINELEFT:
		if (m_nScrollPosHor <= 0)
			return;

		nDelta = -min(max(nMaxPos/20,5),m_nScrollPosHor);
		break;

	case SB_LINERIGHT:
		if (m_nScrollPosHor >= nMaxPos)
			return;

		nDelta = min(max(nMaxPos/20,5),nMaxPos-m_nScrollPosHor);
		break;
	case SB_PAGEDOWN:
		if (m_nScrollPosHor <= 0)
			return;
		nDelta = -min(max(nMaxPos/10,5),nMaxPos-m_nScrollPosHor);
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		nDelta = (int)nPos - m_nScrollPosHor;
		TRACE(_T("Delta:%d \n"),nDelta);
		break;

	case SB_PAGEUP:
		if (m_nScrollPosHor >= nMaxPos)
			return;
		nDelta = min(max(nMaxPos/10,5),m_nScrollPosHor);
		break;

	default:
		return;
	}
	m_nScrollPosHor += nDelta;
	TRACE(_T("Delta:%d \n"),m_nScrollPosHor);
	SetScrollPos(SB_HORZ,m_nScrollPosHor,TRUE);

	SCROLLINFO siHor;
	siHor.cbSize = sizeof(SCROLLINFO);

	GetScrollInfo(SB_HORZ,&siHor);
	int nRange = siHor.nMax - siHor.nMin;
	//float fHorBalFactor = siHor.nPos * 1.0 / (siHor.nMax-siHor.nMin); //����ϵ����
	float fHorBalFactor = abs(m_nScrollPosHor) * 1.0 / (m_nCurWidth * m_iHorizonZoomNum); //����ϵ�� 2013.3.14 ϯ���û�ȥ���޸ĵ�
	TRACE(_T("nMin:%d \n"),siHor.nMin);
	TRACE(_T("nMax:%d \n"),siHor.nMax);
	TRACE(_T("nPos:%d \n"),siHor.nPos);
	TRACE(_T("nPage:%d \n"),siHor.nPage);
	TRACE(_T("fHorBalFactor:%f \n"),fHorBalFactor);
	CurvesHorMove(nRange, fHorBalFactor);
	CButton::OnHScroll(nSBCode, nPos, pScrollBar);
}