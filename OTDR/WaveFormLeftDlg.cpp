// WaveFormLeftDlg.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "OTDR.h"
#include "WaveFormLeftDlg.h"
#include "MainFrm.h"
#include "SorFileInfo.h"
#include "WaveInfo.h"

extern SorFileArray g_sorFileArray;
extern LANG g_currentLang;
// CWaveFormLeftDlg �Ի���

IMPLEMENT_DYNAMIC(CWaveFormLeftDlg, CDialog)

CWaveFormLeftDlg::CWaveFormLeftDlg(CWnd* pParent /*=NULL*/)
: CDialog(CWaveFormLeftDlg::IDD, pParent),m_pMainFrm(NULL),m_nCurFileSelectIndex(LB_ERR)
{
}

CWaveFormLeftDlg::~CWaveFormLeftDlg()
{
}

void CWaveFormLeftDlg::SetListColName(int nColIndex, const CString& colName)
{
	LVCOLUMN col;
	col.mask= LVCF_TEXT;
	col.pszText= (LPWSTR)(LPCWSTR)colName;
	col.cchTextMax= 256;

	m_list.SetColumn(nColIndex, &col);
}

void CWaveFormLeftDlg::SetLocalString()
{
	//�ನ����ʾ
	CString strMulWave = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_MUL_WAVE"));
	GetDlgItem(IDC_CHECK_MULTI_DISPLAY)->SetWindowText(strMulWave);
	/*                     ����listctrl���ػ�                                */
	//�ļ���
	CString strFileName = IniUtil::ReadResString(_T("IDS_FRM_VIEW_DLG_FILENAME"));
	SetListColName(1, strFileName);//�����б���

	/*                                     ��������                           */
	CString strTestCon = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_TEST_CON"));
	GetDlgItem(IDC_STATIC_TEST_CON)->SetWindowText(strTestCon);

	//����
	CString strWaveLength = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_WAVE_LENGTH"));
	GetDlgItem(IDC_STATIC_WAVELENGTH)->SetWindowText(strWaveLength);

	//����
	CString strPulseWidth = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_PULSE_WIDTH"));
	GetDlgItem(IDC_STATIC_PULSEWIDTH)->SetWindowText(strPulseWidth);

	//������Χ
	CString strRange = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_RANGE"));
	GetDlgItem(IDC_STATIC_RANGE)->SetWindowText(strRange);

	//������
	CString strRefraction = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_REFRACTION"));
	GetDlgItem(IDC_STATIC_REFRACTION)->SetWindowText(strRefraction);

	////˥��
	//CString strAttenu = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_ATTENU"));
	//GetDlgItem(IDC_STATIC_ATTENU)->SetWindowText(strAttenu);

	////ƽ��
	//CString strAvg = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_AVG"));
	//GetDlgItem(IDC_STATIC_AVG)->SetWindowText(strAvg);

	//������
	CString strSample = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_SAMPLE"));
	GetDlgItem(IDC_STATIC_SAMPLE)->SetWindowText(strSample);

	///*                                     ���                           */
	//CString strMark = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_MARK"));
	//GetDlgItem(IDC_STATIC_MARK)->SetWindowText(strMark);

	////������
	//CString strCursorDist = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_CURSOR_DIST"));
	//GetDlgItem(IDC_STATIC_CURSOR_DIST)->SetWindowText(strCursorDist);

	////�۽����
	//CString strSpliceLoss = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_SPLICE_LOSS"));
	//GetDlgItem(IDC_STATIC_SPLICE_LOSS)->SetWindowText(strSpliceLoss);

	////�ز����
	//CString strReturnLoss = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_RETURN_LOSS"));
	//GetDlgItem(IDC_STATIC_RETRUN_LOSS)->SetWindowText(strReturnLoss);

	////���[1-2]
	//CString strMark12 = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_MARK12"));
	//GetDlgItem(IDC_STATIC_MARK12)->SetWindowText(strMark12);

	////���[1-2]
	//CString strLoss12 = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_LOSS"));
	//GetDlgItem(IDC_STATIC_LOSS12)->SetWindowText(strLoss12);

	////����[1-2]
	//CString strDist12 = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_DIST"));
	//GetDlgItem(IDC_STATIC_DIST12)->SetWindowText(strDist12);

	////˥������[1-2]
	//CString strSlope12 = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_SLOPE"));
	//GetDlgItem(IDC_STATIC_SLOPE12)->SetWindowText(strSlope12);

	////���[2-3]
	//CString strMark23 = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_MARK23"));
	//GetDlgItem(IDC_STATIC_MARK23)->SetWindowText(strMark23);

	////���[2-3]
	//CString strLoss23 = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_LOSS"));
	//GetDlgItem(IDC_STATIC_LOSS23)->SetWindowText(strLoss23);

	////����[2-3]
	//CString strDist23 = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_DIST"));
	//GetDlgItem(IDC_STATIC_DIST23)->SetWindowText(strDist23);

	////˥������[2-3]
	//CString strSlope23 = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_SLOPE"));
	//GetDlgItem(IDC_STATIC_SLOPE23)->SetWindowText(strSlope23);
}

void CWaveFormLeftDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILE, m_list);
	DDX_Control(pDX, IDC_CHECK_MULTI_DISPLAY, m_checkMultiMode);
}


BEGIN_MESSAGE_MAP(CWaveFormLeftDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CWaveFormLeftDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CWaveFormLeftDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_MULTI_DISPLAY, &CWaveFormLeftDlg::OnBnClickCheckMultiDisplay)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FILE, &CWaveFormLeftDlg::OnLvnItemchangedListFile)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_FILE, &CWaveFormLeftDlg::OnNMRclickListFile)
	//SOR File Menu 
	ON_COMMAND(ID_SORFILE_WAVEINFO, &CWaveFormLeftDlg::OnWaveInfo)
	ON_COMMAND(ID_SORFILE_CLOSE, &CWaveFormLeftDlg::OnSorFileClose)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CWaveFormLeftDlg ��Ϣ�������

void CWaveFormLeftDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CWaveFormLeftDlg::OnOK()
{

}

void CWaveFormLeftDlg::OnCancel()
{

}

void CWaveFormLeftDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CWaveFormLeftDlg::OnBnClickCheckMultiDisplay()
{
	COtdrTrace* otdrTrace=&MAIN->m_pWaveFormView->m_sorDrawStatic;
	int nCheck = m_checkMultiMode.GetCheck();
	if (nCheck == 0)
	{
		g_sorFileArray.waveConfig.MultiMode = FALSE;
		for(int i=0;i<otdrTrace->m_vAllData.size();i++)
		{
			if (otdrTrace->m_nCurveIndex!=i)
			{
				otdrTrace->SetShowCurve(i,FALSE);
			}
		}		
	}
	else
	{
		g_sorFileArray.waveConfig.MultiMode = TRUE;
		for(int i=0;i<otdrTrace->m_vAllData.size();i++)
		{
			otdrTrace->SetShowCurve(i,TRUE);
		}
	}


	otdrTrace->ReCalMaxDistance();
	otdrTrace->DrawCurves(&otdrTrace->m_dcCurve,otdrTrace->m_nCurveIndex);
	otdrTrace->Invalidate();
	CString strMultiMode;
	strMultiMode.Format(_T("%d"), nCheck);
	IniUtil::WriteSingleConfigInfo(_T("FileList"), _T("MultiMode"), strMultiMode);
	//m_pMainFrm->m_pWaveFormView->m_sorDrawStatic.ResetCurveData(); //���¼�����������//cwcq2013.03.29
}

void CWaveFormLeftDlg::OnWaveInfo()
{
	CWaveInfo dlg(this);
	if(dlg.DoModal() != IDOK)
	{
		//�������θ�����Ϣ
	}
}

void CWaveFormLeftDlg::OnSorFileClose()
{
	//ɾ����ǰѡ�����ļ�
	g_sorFileArray.RemoveCurSel();
	//���õ�ǰfilelist
	FillFileList();
	//�����¼��б�
	MAIN->m_pEventFormView->FreshEventList();
	//�ػ�ͼ����
	//m_pMainFrm->m_pWaveFormView->m_sorDrawStatic.Invalidate();//cwcq2013.03.29
	m_pMainFrm->m_pThumbFormView->Invalidate();

	/********///zll,2012.9.13 ʹ���ñ����򵼴�����ʱ�������ߺ������굥λ�ı����¼��б�����ͼ����ʾ
	int index = GetNextCurveNum(g_sorFileArray);
	m_pMainFrm->m_pWaveFormView->PopulateData();
	//����¼��б�
	m_pMainFrm->m_pEventFormView->FreshEventList();
	//�����������
	//m_pMainFrm->m_pWaveFormView->m_sorDrawStatic.FillCurveDataFromFile(g_sorFileArray.sorFileList[index].pTestResultInfo);//cwcq2013.03.29
	//m_pMainFrm->m_pWaveFormView->m_sorDrawStatic.Invalidate();//redraw//cwcq2013.03.29
	m_pMainFrm->m_sorAssist.CloseCurvedata(m_listCurrentItem,m_pMainFrm->m_sorDATA,&m_pMainFrm->m_pWaveFormView->m_sorDrawStatic);//zll 2013.5.7
	m_pMainFrm->m_pThumbFormView->Invalidate(); //redraw thumbnail

	//���ú����굥λ�ı�
	m_pMainFrm->m_pWaveFormView->SetHorUnitText();
	//���������굥λ�ı�
	m_pMainFrm->m_pWaveFormView->SetVerUnitText();
	/********///zll,2012.9.13 ʹ���ñ����򵼴�����ʱ�������ߺ������굥λ�ı����¼��б�����ͼ����ʾ
    m_pMainFrm->m_pWaveFormView->m_sorDrawStatic.TraceCountChanged();

	//�����ǰ����û�����ߣ���ӡ�ͱ���˵��ҵ�
	if (g_sorFileArray.totalCurveNumber == 0)
	{
		MAIN->m_bFileMenuEnable = FALSE;
	}
	else
	{
		//���¼�����������
		//MAIN->m_pWaveFormView->m_sorDrawStatic.ResetCurveData();//cwcq2013.03.29
	}
}


BOOL CWaveFormLeftDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ResetImageList();

	//��ʼ���ļ��б�
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CString strNo=_T("No");
	CString strName=_T("�ļ���");
	m_list.InsertColumn(0,strNo,LVCFMT_LEFT,50);
	m_list.InsertColumn(1,strName,LVCFMT_LEFT,176);

	//�����Ƿ�Ϊ��������ʾģʽ

	m_checkMultiMode.SetCheck(g_sorFileArray.waveConfig.MultiMode);
	m_listItem=-1;//zll 2013.5.7

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CWaveFormLeftDlg::ResetImageList()
{
	m_ImageList.DeleteImageList();
	//����λͼ�б�
	m_ImageList.Create(16,16,ILC_COLOR32 | ILC_MASK, 1, 0);//��ʾ�ļ��л��ļ�
	m_list.SetImageList(&m_ImageList, LVSIL_SMALL);

	CBitmap bitmap1;
	GetColorBitmap(g_sorFileArray.waveConfig.ColorCurve1, bitmap1);
	m_ImageList.Add(&bitmap1, RGB(0, 0, 0));

	CBitmap bitmap2;
	GetColorBitmap(g_sorFileArray.waveConfig.ColorCurve2, bitmap2);
	m_ImageList.Add(&bitmap2, RGB(0, 0, 0));

	CBitmap bitmap3;
	GetColorBitmap(g_sorFileArray.waveConfig.ColorCurve3, bitmap3);
	m_ImageList.Add(&bitmap3, RGB(0, 0, 0));

	CBitmap bitmap4;
	GetColorBitmap(g_sorFileArray.waveConfig.ColorCurve4, bitmap4);
	m_ImageList.Add(&bitmap4, RGB(0, 0, 0));

	CBitmap bitmap5;
	GetColorBitmap(g_sorFileArray.waveConfig.ColorCurve5, bitmap5);
	m_ImageList.Add(&bitmap5, RGB(0, 0, 0));

	CBitmap bitmap6;
	GetColorBitmap(g_sorFileArray.waveConfig.ColorCurve6, bitmap6);
	m_ImageList.Add(&bitmap6, RGB(0, 0, 0));

	CBitmap bitmap7;
	GetColorBitmap(g_sorFileArray.waveConfig.ColorCurve7, bitmap7);
	m_ImageList.Add(&bitmap7, RGB(0, 0, 0));

	CBitmap bitmap8;
	GetColorBitmap(g_sorFileArray.waveConfig.ColorCurve8, bitmap8);
	m_ImageList.Add(&bitmap8, RGB(0, 0, 0));
}

void CWaveFormLeftDlg::GetColorBitmap(COLORREF color, CBitmap &bmp)
{
	CPaintDC dc(this);
	BITMAP BitMap;
	CDC MemDC;
	CBitmap MemBitMap;
	CBitmap *pOldBitMap;
	CBrush* pOldBrush;
	CRect rect;
	rect.SetRect(0, 0, 15, 15);
	CRect InvalidRect;
	dc.GetClipBox(&InvalidRect); //��ȡ��Ч��,����Ҫ�ػ������

	if (MemDC.CreateCompatibleDC(&dc))
	{
		//����һ����dc���ݵ�λͼ����СΪ16 x 15 
		if (MemBitMap.LoadBitmap(IDB_BITMAP_WAVE_COLOR))
		{
			//��λͼѡ���ڴ滷��
			pOldBitMap = MemDC.SelectObject(&MemBitMap);;
			//�ñ���ɫ���λͼ
			MemDC.FillSolidRect(&rect, color);
		}

		bmp.Attach(MemBitMap.Detach());
		MemDC.SelectObject(pOldBitMap);
		MemDC.DeleteDC();
	}
}

//ˢ���б�� 
void CWaveFormLeftDlg::PopulateData()
{
	FillFileList();
	FillParams();
	//return 1;
}

//����ļ��б�
void CWaveFormLeftDlg::FillFileList()
{
	m_list.DeleteAllItems();
	INT index = 0;
	INT curSele = 0;//�б��ѡ���к�
	for(int i = 0;i < g_sorFileArray.totalCurveNumber; i++)
	{
		CString strFileName = g_sorFileArray.sorFileList[i].fileName;
		CString strIndex;

		LV_ITEM lvi;
		lvi.mask = LVIF_TEXT| LVIF_IMAGE;
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.stateMask = 0xFFFF;     // get all state flags
		lvi.iImage = i;
		lvi.cchTextMax = strFileName.GetLength();
		lvi.pszText = (LPWSTR)(LPCTSTR)strFileName;
		strIndex.Format(_T("%d"), i+1);
		if (g_sorFileArray.sorFileList[i].bValid)
		{
			m_list.InsertItem(&lvi);
			m_list.SetItemText(i, 0, strIndex);
			m_list.SetItemText(i, 1, strFileName);
		}
		if (g_sorFileArray.curSelection == i)
		{
			curSele = index;
		}
		index++;
	}

	m_list.SendMessage(LB_SETCURSEL,index,0);//��Ϊѡ��״̬
}

//������߲���
void CWaveFormLeftDlg::FillParams()
{
	TestResultInfo* pTestResultInfo = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;
	if(!pTestResultInfo)
		return;
	//��������**********************************************************//
	//����
	CString strWaveLength;
	int dWaveLength = pTestResultInfo->WaveLength;
	strWaveLength.Format(_T("%d"), dWaveLength);
	SetControlText(IDC_STATIC_WAVELENGTH_VAL, strWaveLength);
	//���� zll 2012.10.30
	CString strPulseWidth;
	if (pTestResultInfo->PulseWidth>1000||pTestResultInfo->PulseWidth==1000)
	{
		strPulseWidth.Format(_T("%dus"), pTestResultInfo->PulseWidth/1000);
	}
	else
	{
		strPulseWidth.Format(_T("%dns"), pTestResultInfo->PulseWidth);
	}
	SetControlText(IDC_STATIC_PULSEWIDTH_VAL, strPulseWidth);
	//������Χ	zll 2012.10.30
	CString strRange;
	float m_Range=0.0f;
	if (_T("m")==g_sorFileArray.waveConfig.DistUnit)
	{
		strRange.Format(_T("%.2fkm"), pTestResultInfo->Range/1000.0);
	}
	else if (_T("mi")==g_sorFileArray.waveConfig.DistUnit)
	{
		m_Range=KmeterToOther(pTestResultInfo->Range/1000.0,UNIT_SHOW_MILE);
		strRange.Format(_T("%.2fmiles"), m_Range);
	}
	else if (_T("ft")==g_sorFileArray.waveConfig.DistUnit)
	{
		m_Range=KmeterToOther(pTestResultInfo->Range/1000.0,UNIT_SHOW_KFEET);
		strRange.Format(_T("%.2fkft"), m_Range);
	}
	SetControlText(IDC_STATIC_RANGE_VAL, strRange);
	//������
	CString strRefraction;
	strRefraction.Format(_T("%.3f"), pTestResultInfo->Refraction);
	SetControlText(IDC_STATIC_REFRACTION_VAL, strRefraction);
	//˥��
	CString strAttenu;
	strAttenu.Format(_T("%2f"), pTestResultInfo->Atten);
	SetControlText(IDC_STATIC_ATTENU_VAL, strAttenu);
	//ƽ��
	CString strAvg;
	strAvg.Format(_T("%2f"), pTestResultInfo->Average);
	SetControlText(IDC_STATIC_AVG_VAL, strAvg);
	//������
	CString strSampleCount;
	strSampleCount.Format(_T("%d"), pTestResultInfo->DataPointsNum);
	SetControlText(IDC_STATIC_SAMPLE_COUNT, strSampleCount);

	//���************************************************************//
	//������
	SetControlText(IDC_STATIC_CURSOR_DIST_VAL, L"1510");
	//�۽����
	SetControlText(IDC_STATIC_SPLICE_LOSS_VAL, L"1510");
	//�ز����
	SetControlText(IDC_STATIC_RETRUN_LOSS_VAL, L"1510");
	//���[1-2] ���
	SetControlText(IDC_STATIC_MARK12_VAL, L"1510");
	//���[1-2] ����
	SetControlText(IDC_STATIC_DIST12_VAL, L"1510");
	//���[1-2] ˥������
	SetControlText(IDC_STATIC_SLOPE12_VAL, L"1510");
}

void CWaveFormLeftDlg::SetControlText(int nControlID, LPCTSTR strVal)
{
	CWnd* pWnd = GetDlgItem(nControlID);
	if (!pWnd)
		return;

	CString strUnitText;
	pWnd->GetWindowText(strUnitText);
	CString strTempUnitText;
	int nPos = strUnitText.ReverseFind(' ');
	if (nPos != -1)
		strTempUnitText = strUnitText.Right(strUnitText.GetLength() - nPos);

	CString strValText;
	strValText.Format(_T("%s%s"), strVal, strTempUnitText);
	pWnd->SetWindowText(strValText);
}

void CWaveFormLeftDlg::OnLvnItemchangedListFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	m_pMainFrm = MAIN;
	if (NULL == m_pMainFrm)
		return;

	if (pNMLV->uChanged==LVIF_STATE)
	{
		POSITION pos = m_list.GetFirstSelectedItemPosition();
		int nIndex = m_list.GetNextSelectedItem(pos);
		if (nIndex != LB_ERR)
		{
			//if(nIndex == g_sorFileArray.curSelection)  //lzy2014.9.9
				//return;

			g_sorFileArray.curSelection = nIndex;
			MAIN->m_pWaveFormView->m_spinOffset.SetPos(-(m_pMainFrm->m_pWaveFormView->m_sorDrawStatic.m_vAllData[nIndex].m_nOffsetY));//zll 2013.7.2
			m_pMainFrm->m_pWaveFormView->m_sorDrawStatic.SetCurrentSeleTrace(nIndex);
			
			//m_pMainFrm->m_pWaveFormView->m_sorDrawStatic.Invalidate();//cwcq2013.03.29
			m_pMainFrm->m_pThumbFormView->Invalidate();
			
			//��������������
			FillParams();
			//ˢ���¼��б�
			MAIN->m_pEventFormView->FreshEventList();
			m_pMainFrm->m_pWaveFormView->m_sorDrawStatic.SetRange(0, m_pMainFrm->m_pWaveFormView->m_sorDrawStatic.m_vAllData[nIndex].m_fLength, 0, 65.535);// zll 2013.7.1 ÿ��ĳ���� �����߾Ͳ���������ʾ����
		
			m_pMainFrm->m_pWaveFormView->m_sorDrawStatic.OnInitAB();  //lzy2014.9.9

		}

		//zll 2013.5.7
		NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
		m_listItem=pNMListView->iItem;
	}

	*pResult = 0;
}

void CWaveFormLeftDlg::OnNMRclickListFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != LB_ERR)
	{
		m_nCurFileSelectIndex = pNMListView->iItem;
		DWORD dwPos = GetMessagePos();
		CPoint point( LOWORD(dwPos), HIWORD(dwPos) );

		CMenu menu;
		if (g_currentLang==LANG::CHS)
		{
			VERIFY( menu.LoadMenu(IDR_MENU_SORFILE));
		}
		else if (g_currentLang==LANG::ENG)
		{
			VERIFY( menu.LoadMenu(IDR_MENU_SORFILE_EN));
		}
		else if (g_currentLang==LANG::PORT)  //lzy 2014.06.06
		{
			VERIFY( menu.LoadMenu(IDR_MENU_SORFILE_PT));
		}
		
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT( popup != NULL );
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}

	*pResult = 0;

	POSITION pos=m_list.GetFirstSelectedItemPosition();
	m_listCurrentItem=m_list.GetNextSelectedItem(pos);
}

void CWaveFormLeftDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//UINT count;
	//TCHAR filePath[200];
	//count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	//if(count)
	//{
	//	for(int i=0; i<count; i++)
	//	{
	//		int pathLen = ::DragQueryFile(hDropInfo, i, filePath, sizeof(filePath));
	//	}
	//}

	//DragFinish(hDropInfo);	
	
	CDialog::OnDropFiles(hDropInfo);

}
