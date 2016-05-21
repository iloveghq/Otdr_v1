// Eventlist.cpp : implementation file
//

#include "stdafx.h"
#include "OTDR.h"
#include <math.h>
#include "Eventlist.h"
#include "EndGabor.h"

extern SorFileArray g_sorFileArray;
extern LANG g_currentLang;

// CEventlist

IMPLEMENT_DYNCREATE(CEventlist, CFormView)

CEventlist::CEventlist()
	: CFormView(CEventlist::IDD),m_pImageList(NULL)
{

}

CEventlist::~CEventlist()
{
	if (m_pImageList)
	{
		delete m_pImageList;
		m_pImageList = NULL;
	}
}

void CEventlist::SetLocalString()
{
	//����Frame����
	CString strTitle = IniUtil::ReadResString(_T("IDS_FRM_EVENT_TITLE"));
	GetParentFrame()->SetWindowText(strTitle);

	//�����¼��б�
	SetListViewText();
}

void CEventlist::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_EVENT, m_Eventlist);
}

BEGIN_MESSAGE_MAP(CEventlist, CFormView)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_EVENT, &CEventlist::OnLvnItemchangedListEvent)
END_MESSAGE_MAP()


// CEventlist diagnostics

#ifdef _DEBUG
void CEventlist::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CEventlist::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEventlist message handlers

void CEventlist::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	/*                   ��ʼ��ImageList                                                             */
	//����λͼ�б�
	if (NULL == m_pImageList)
	{
		m_pImageList = new CImageList();
		m_pImageList->Create(16,16,ILC_COLOR32 | ILC_MASK, 1, 0);//��ʾ�ļ��л��ļ�
		m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_EVENT_NOREFLECT));
		m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_EVENT_REFLECT));

	//******��ʼ��m_Eventlist************************************************/
	m_Eventlist.SetImageList(m_pImageList, LVSIL_SMALL);
	m_Eventlist.SetWindowPos(NULL,0,0,964,400,SWP_NOMOVE|SWP_NOZORDER | SWP_NOACTIVATE);
	m_Eventlist.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);//ʹm_Eventlist������(LVS_EX_GRIDLINES)�������пɱ�����ѡ��(LVS_EX_FULLROWSELECT)
    CString strname[7];
	strname[0]=_T("�¼����");
	//strname[1]=_T("����");
	strname[1]=_T("����(Km)");
	strname[2]=_T("��(Km)");
	strname[3]=_T("���(dB)");
	strname[4]=_T("�����(dB)");
	strname[5]=_T("б��(dB/Km)");
	strname[6]=_T("����(dB)");
	for(int i=0;i<7;i++)
	{
		m_Eventlist.InsertColumn(i,strname[i],LVCFMT_LEFT,137);
	}
	}
	
	resizeControl();//�����¼��б��λ��
}

void CEventlist::SetListViewText()
{
	//���
	CString strOrder = IniUtil::ReadResString(_T("IDS_FRM_VIEW_EVENT_ORDER"));
	SetListColName(0, strOrder);//�����б���

	//����
	//CString strType = IniUtil::ReadResString(_T("IDS_FRM_VIEW_EVENT_TYPE")); 
	//SetListColName(1,strType);//�����б���

	//����
	CString strText;
	if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
		strText=_T("(km)");
	else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪmiles
		strText=_T("(miles)");
	else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkft
	    strText=_T("(kft)");
	CString strDistance = IniUtil::ReadResString(_T("IDS_FRM_VIEW_EVENT_DIST"));
	SetListColName(1,strDistance+strText);//�����б���

	//��
	CString strSegment = IniUtil::ReadResString(_T("IDS_FRM_VIEW_EVENT_SEGMENT"));
	SetListColName(2, strSegment+strText);//�����б���

	//���
	CString strLoss = IniUtil::ReadResString(_T("IDS_FRM_VIEW_EVENT_LOSS"));
	SetListColName(3, strLoss);//�����б���

	//�����
	CString strTotalLoss = IniUtil::ReadResString(_T("IDS_FRM_VIEW_EVENT_TOTAL_LOSS"));
	SetListColName(4, strTotalLoss);//�����б���

	//б��
	CString strSlope = IniUtil::ReadResString(_T("IDS_FRM_VIEW_EVENT_SLOPE"));
	SetListColName(5, strSlope);//�����б���

	//����
	CString strReflection = IniUtil::ReadResString(_T("IDS_FRM_VIEW_EVENT_REFLECTION"));
	SetListColName(6, strReflection);//�����б���
}

void CEventlist::SetListColName(int nColIndex, const CString& colName)
{
	LVCOLUMN col;
	col.mask= LVCF_TEXT;
	col.pszText= (LPWSTR)(LPCWSTR)colName;
	col.cchTextMax= 256;

	m_Eventlist.SetColumn(nColIndex, &col);
}

void CEventlist::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	resizeControl();
}
void CEventlist::resizeControl()
{
	if(this->GetDlgItem(IDC_LIST_EVENT)->GetSafeHwnd()==NULL)
	{
		return;
	}
	CRect rect,rectlistview;
	GetWindowRect(&rect);
	this->GetDlgItem(IDC_LIST_EVENT)->GetWindowRect(&rectlistview);
	ScreenToClient(&rect);
	ScreenToClient(&rectlistview);
	this->GetDlgItem(IDC_LIST_EVENT)->SetWindowPos(NULL, 0, 0, rect.Width()-4, rect.bottom-rectlistview.top-2, SWP_NOMOVE);
}

//ѡ���˲�ͬ�����ߺ���ʾ��Ӧ���ߵ��¼��б�
void CEventlist::FreshEventList()
{
	CString strText = _T("");
	if (g_sorFileArray.totalCurveNumber == 0)
	{
		//�����ͷ
		if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
			strText=_T("(km)");
		else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪmiles
			strText=_T("(miles)");
		else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkft
			strText=_T("(kft)");
		CString strDistance = IniUtil::ReadResString(_T("IDS_FRM_VIEW_EVENT_DIST"));
		SetListColName(1,strDistance+strText);//�����б���

		//�α�ͷ
		CString strSegment = IniUtil::ReadResString(_T("IDS_FRM_VIEW_EVENT_SEGMENT"));
		SetListColName(2, strSegment+strText);//�����б���

		m_Eventlist.DeleteAllItems();
		return;
	}
	std::vector<EventInfo> ve = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->EventList.vEventInfo;//�ٶ�??
	int nSize=ve.size();
	//
	float preDistance=0;
	INT index = 0;
	INT curSele = 0;//�б��ѡ���к�
	m_Eventlist.DeleteAllItems();
	for (int i=0;i< nSize;i++)//
	{		LV_ITEM lvi;
		lvi.mask = LVIF_TEXT| LVIF_IMAGE;
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.stateMask = 0xFFFF;     // get all state flags
		lvi.cchTextMax = 50;
		lvi.pszText = (LPWSTR)(LPCTSTR)strText;
		//����
		if (0 == ve[i].nEventType)
		{
			lvi.iImage = 0;//0����Ƿ����¼�����
		}
		else if (1 == ve[i].nEventType)
		{
			lvi.iImage = 1;//1�������¼�����,zll 2013.12.25�޸ģ�ԭ���ķ���
		}
		strText.Format(_T("%d"), i+1);
		m_Eventlist.InsertItem(&lvi);
		//���
		if (_T('S')==ve[i].cEventType)
		{
			m_Eventlist.SetItemText(i, 0, _T("(S)")+strText);
		}
		else if (_T('E')==ve[i].cEventType)
		{
			m_Eventlist.SetItemText(i, 0, _T("(E)")+strText);
		}
		else if (_T('A')==ve[i].cEventType)
		{
			m_Eventlist.SetItemText(i, 0, _T("(A)")+strText);
		}
		else if (_T('F')==ve[i].cEventType)
		{
			m_Eventlist.SetItemText(i, 0, _T("(F)")+strText);
		}
		else if (_T('O')==ve[i].cEventType)
		{
			m_Eventlist.SetItemText(i, 0, _T("(O)")+strText);
		}
		else
		{
			m_Eventlist.SetItemText(i, 0, _T("    ")+strText);
		}
		//m_Eventlist.SetItemText(i, 1, strText);

		//�����ͷ
		if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
			strText=_T("(km)");
		else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪmiles
			strText=_T("(miles)");
		else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkft
			strText=_T("(kft)");
		CString strDistance = IniUtil::ReadResString(_T("IDS_FRM_VIEW_EVENT_DIST"));
		SetListColName(1,strDistance+strText);//�����б���

		//�α�ͷ
		CString strSegment = IniUtil::ReadResString(_T("IDS_FRM_VIEW_EVENT_SEGMENT"));
		SetListColName(2, strSegment+strText);//�����б���

		//����
		float m_float_zll=0.0f;
		if (0==i)
		{
			if(3==g_sorFileArray.waveConfig.UnitDistFig)
				strText.Format(_T("%.3f"),0.0f);//����
			else if(4==g_sorFileArray.waveConfig.UnitDistFig)
				strText.Format(_T("%.4f"),0.0f);//����
			else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			    strText.Format(_T("%.5f"),0.0f);//����
		}
		else
		{
			if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
			{
				if(3==g_sorFileArray.waveConfig.UnitDistFig)
					strText.Format(_T("%.3f"),ve[i].fLocation/1000);//����
				else if(4==g_sorFileArray.waveConfig.UnitDistFig)
					strText.Format(_T("%.4f"),ve[i].fLocation/1000);//����
				else if(5==g_sorFileArray.waveConfig.UnitDistFig)
				    strText.Format(_T("%.5f"),ve[i].fLocation/1000);//����
			}
			else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪmiles
			{
				m_float_zll=KmeterToOther(ve[i].fLocation/1000,UNIT_SHOW_MILE);
				if(3==g_sorFileArray.waveConfig.UnitDistFig)
					strText.Format(_T("%.3f"),m_float_zll);//����
				else if(4==g_sorFileArray.waveConfig.UnitDistFig)
					strText.Format(_T("%.4f"),m_float_zll);//����
				else if(5==g_sorFileArray.waveConfig.UnitDistFig)
				    strText.Format(_T("%.5f"),m_float_zll);//����
			}
			else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkft
			{
				m_float_zll=KmeterToOther(ve[i].fLocation/1000,UNIT_SHOW_KFEET);
				if(3==g_sorFileArray.waveConfig.UnitDistFig)
					strText.Format(_T("%.3f"),m_float_zll);//����
				else if(4==g_sorFileArray.waveConfig.UnitDistFig)
					strText.Format(_T("%.4f"),m_float_zll);//����
				else if(5==g_sorFileArray.waveConfig.UnitDistFig)
				    strText.Format(_T("%.5f"),m_float_zll);//����
			}
		}
		m_Eventlist.SetItemText(i, 1, strText);
		//��,���¼�������
		float a,b=0.0f;
		if (0==i)
		{
			if(3==g_sorFileArray.waveConfig.UnitDistFig)
				strText.Format(_T("%.3f"),0.0f);//��
			else if(4==g_sorFileArray.waveConfig.UnitDistFig)
				strText.Format(_T("%.4f"),0.0f);//��
			else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			    strText.Format(_T("%.5f"),0.0f);//��
		}
		else
		{
			if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
			{
				if(3==g_sorFileArray.waveConfig.UnitDistFig)
					strText.Format(_T("%.3f"),(ve[i].fLocation-preDistance)/1000);//��,���¼�������
				else if(4==g_sorFileArray.waveConfig.UnitDistFig)
					strText.Format(_T("%.4f"),(ve[i].fLocation-preDistance)/1000);//��,���¼�������
				else if(5==g_sorFileArray.waveConfig.UnitDistFig)
				    strText.Format(_T("%.5f"),(ve[i].fLocation-preDistance)/1000);//��,���¼�������
			}
			else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪmiles
			{
				b=(ve[i].fLocation-preDistance)/1000;
				a=KmeterToOther(b,UNIT_SHOW_MILE);
				if(3==g_sorFileArray.waveConfig.UnitDistFig)
					strText.Format(_T("%.3f"),a);//��,���¼�������
				else if(4==g_sorFileArray.waveConfig.UnitDistFig)
					strText.Format(_T("%.4f"),a);//��,���¼�������
				else if(5==g_sorFileArray.waveConfig.UnitDistFig)
				    strText.Format(_T("%.5f"),a);//��,���¼�������
			}
			else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkft
			{
				b=(ve[i].fLocation-preDistance)/1000;
				a=KmeterToOther(b,UNIT_SHOW_KFEET);
				if(3==g_sorFileArray.waveConfig.UnitDistFig)
					strText.Format(_T("%.3f"),a);//��,���¼�������
				else if(4==g_sorFileArray.waveConfig.UnitDistFig)
					strText.Format(_T("%.4f"),a);//��,���¼�������
				else if(5==g_sorFileArray.waveConfig.UnitDistFig)
				    strText.Format(_T("%.5f"),a);//��,���¼�������
			}
		}
		m_Eventlist.SetItemText(i, 2, strText);
		if (0==i)
		{
			preDistance = 0.0f;
		}
		else
		{
			preDistance = ve[i].fLocation;
		}
		//���
		if (0 == ve[i].fEventloss)//Ϊ0ʱ��ʾ"-"wcq2012.07.09
		{
			strText.Format(_T("--"));//�����
		}
		else
		{
			strText.Format(_T("%.3f"), ve[i].fEventloss);//���
		}
		m_Eventlist.SetItemText(i, 3, strText);
		//�����
		if (0 == ve[i].fTotalLoss)//Ϊ0ʱ��ʾ"-"wcq2012.07.09
		{
			strText.Format(_T("--"));//�����
		}
		else
		{
			strText.Format(_T("%.3f"),ve[i].fTotalLoss);//�����
		}
		m_Eventlist.SetItemText(i, 4, strText);
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
		m_Eventlist.SetItemText(i,5,strText);
		//����
		if (0 == ve[i].nEventType)//����
		{
			m_Eventlist.SetItemText(i,6,_T("--"));
		}
		else if (0 == ve[i].fEventHigh)//Ϊ0ʱ��ʾ"-"wcq2012.07.09
		{
			m_Eventlist.SetItemText(i,6,_T("--"));
		}
		else
		{
			strText.Format(_T("%.3f"),ve[i].fEventHigh);//����
			m_Eventlist.SetItemText(i, 6, strText);
		}
		

		index++;
	}
	//���õ�ǰѡ����
	m_Eventlist.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
}

//�����ߺ�ͨ��dll�ļ��㹫ʽ�õ��¼��б�
TestResultInfo* CEventlist::GetEventList()
{
	//return;//��ʱ������???????????????????????????????????????????????
	TestResultInfo* pCurveData = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;

	long l= GetTickCount();
	int waveNum = (1550 == pCurveData->WaveLength) ? 1:0; 
	float flo_BackScatter,flo_Refraction=0.0f;
	if (1310 == pCurveData->WaveLength)
	{
		flo_BackScatter=g_sorFileArray.eventAnalConfig.BackScatter;
		flo_Refraction=g_sorFileArray.eventAnalConfig.Refraction;
	}
	else if (1550 == pCurveData->WaveLength)
	{
		flo_BackScatter=g_sorFileArray.eventAnalConfig.BackScatter1550;
		flo_Refraction=g_sorFileArray.eventAnalConfig.Refraction1550;
	}
	else if (1625 == pCurveData->WaveLength)//zll,2013.5.10 ��
	{
		flo_BackScatter=g_sorFileArray.eventAnalConfig.BackScatter1625;
		flo_Refraction=g_sorFileArray.eventAnalConfig.Refraction1625;
	}
	__try{//zll 2013.6.8 �� ����Ƿ�gabor_arithmetic�㷨������
		pCurveData->m_GaborResult = m_endgabor.gabor_arithmetic(pCurveData->WaveLength,//����
			                            pCurveData->PulseWidth, //����			 
										pCurveData->pDataPoints, //���Ե�����
										pCurveData->DataPointsNum, //����
										2.08, //����Ҷ�仯��ֵ���ޡ�Ĭ��Ϊ2.08
										2.08, //����Ҷ�仯��ֵ����.Ĭ��Ϊ2.08
										g_sorFileArray.eventAnalConfig.BendTH, //˥���ж����� ����
										g_sorFileArray.eventAnalConfig.ReflectTH, //�����ж�����
										g_sorFileArray.eventAnalConfig.Termial, //dB_End��ĩ���ж�����Ĭ��Ϊ3
										flo_BackScatter, //����ɢ��ϵ��
										pCurveData->Frequency,
										flo_Refraction,//������,zll
										0);//0Ϊ����ƴ�ӵ㣬�ݶ�Ϊ0,zll//����DLL����������¼��б�β��λ�õ�
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return pCurveData;
	}
	pCurveData->EventList.vEventInfo.clear();
	EventInfo eventinfo;
	int len = pCurveData->m_GaborResult.Valid_Num;//wcq2012.06.07
	//float fDisTwo = MAIN->m_pWaveFormView->m_sorDrawStatic.Get_XCoord(1,pCurveData);//*1000��ת��Ϊ��ȷ�Ĺ����� zll 2012.10.16cwcq2013.03.29
	float fDisTwo = MAIN->m_pWaveFormView->m_sorDrawStatic.Get_XCoord(1,pCurveData);//zll 2013.6.7
	for(int i=0;i<len;i++)
	{
		if (0==fDisTwo)
		{
			int ff=0;
		}//eif
		eventinfo.index = pCurveData->m_GaborResult.Event_Array[i];//�±꣬�ڼ���
		eventinfo.nEventType = pCurveData->m_GaborResult.Reflect_Type[i];//�¼����� //zll 2013.6.8 ȥ��"��"
		//eventinfo.nEventType = !pCurveData->m_GaborResult.Reflect_Type[i];//�¼�����
		eventinfo.cEventType=pCurveData->m_GaborResult.Event_type[i];
		eventinfo.fLocation = fDisTwo * pCurveData->m_GaborResult.Event_Array[i];//�������룬��kmΪ��λ
		eventinfo.fEventloss = pCurveData->m_GaborResult.Event_Loss[i];
		eventinfo.fEventHigh = pCurveData->m_GaborResult.Event_Hight[i];
		eventinfo.fLineK = pCurveData->m_GaborResult.Line_k[i];//zll 2013.6.8 ȥ��fDisTwo
		//eventinfo.fLineK = pCurveData->m_GaborResult.Line_k[i] / fDisTwo;
		eventinfo.fTotalLoss = pCurveData->m_GaborResult.Total_Loss[i];
		eventinfo.fReflect =pCurveData->m_GaborResult.Reflect;
		eventinfo.fTrailEnd = pCurveData->m_GaborResult.Trail_End;
		pCurveData->EventList.vEventInfo.push_back(eventinfo);
		//MAIN->m_pWaveFormView->m_sorDrawStatic.m_vAllData[i].index=eventinfo.index;//zll 2013.6.14
		//MAIN->m_pWaveFormView->m_sorDrawStatic.m_vAllData[i].fLocation=eventinfo.fLocation;//zll 2013.6.14
	}
	return pCurveData;
}

void CEventlist::OnLvnItemchangedListEvent(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	POSITION pos = m_Eventlist.GetFirstSelectedItemPosition();
	int nIndex = m_Eventlist.GetNextSelectedItem(pos);
	if (nIndex != LB_ERR)
	{
		//MAIN->m_pWaveFormView->m_sorDrawStatic.MoveEventList(nIndex);// cwcq2013.03.29
		MAIN->m_pWaveFormView->m_sorDrawStatic.MoveEventList(nIndex);
	}

	*pResult = 0;
}
