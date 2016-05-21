// PropSheet.cpp : implementation file
//

#include "stdafx.h"
#include "PropSheet.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern SorFileArray g_sorFileArray;
extern LANG g_currentLang;

/////////////////////////////////////////////////////////////////////////////
// CPropSheet

IMPLEMENT_DYNAMIC(CPropSheet, CPropertySheet)

CPropSheet::CPropSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_nAlreaySettingPageIndex = 0;

	AddPage(&m_PropStep1);  //0, ѡ��ÿҳ����������ļ���
	//AddPage(&m_PropStep2);
	//AddPage(&m_PropStep3);	//1�� ����
	//AddPage(&m_PropStep3Rpt);  //2������
	AddPage(&m_PropStep4);  //1,
	AddPage(&m_PropStep5); //2,lzy2014.9.12
	AddPage(&m_PropStep6);  //3
}

CPropSheet::CPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_nAlreaySettingPageIndex = 0;
	AddPage(&m_PropStep1);//0
	//AddPage(&m_PropStep2);
	//AddPage(&m_PropStep3);	
	//AddPage(&m_PropStep3Rpt);
	AddPage(&m_PropStep4);  //1
	AddPage(&m_PropStep5); //2,lzy2014.9.12
	AddPage(&m_PropStep6);//3
	
}

CPropSheet::~CPropSheet()
{
}

BEGIN_MESSAGE_MAP(CPropSheet, CPropertySheet)
ON_BN_CLICKED(IDC_BUTTON_QUIT, OnBtnQuit)
ON_BN_CLICKED(IDC_BUTTON_PREV, OnBtnPrev)
ON_BN_CLICKED(IDC_BUTTON_NEXT, OnBtnNext)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropSheet message handlers

BOOL CPropSheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	//�����˳���ť
	CRect rect;
	GetDlgItem(IDCANCEL)->GetWindowRect(&rect);
	ScreenToClient(rect);
	rect.OffsetRect(REPORT_WIZARD_LEFT_OFFSET, REPORT_WIZARD_BOTTOM_HEIGHT);
	GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);//hide Cancel 
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
	m_BtnQuit.Create(_T("�˳�"),
		BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP, rect, this, IDC_BUTTON_QUIT);
	m_BtnQuit.SetFont(GetFont());
	m_BtnQuit.MoveWindow(rect, TRUE);

	GetDlgItem(ID_WIZBACK)->GetWindowRect(&rect);
	ScreenToClient(rect);
	rect.OffsetRect(REPORT_WIZARD_LEFT_OFFSET, REPORT_WIZARD_BOTTOM_HEIGHT);
	GetDlgItem(ID_WIZBACK)->ShowWindow(SW_HIDE);//hide prev page Button
	GetDlgItem(ID_WIZBACK)->EnableWindow(FALSE);
	m_BtnPrev.Create(_T("��һҳ"),
		BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP, rect, this, IDC_BUTTON_PREV);
	m_BtnPrev.SetFont(GetFont());
	m_BtnPrev.EnableWindow(FALSE);
	m_BtnPrev.MoveWindow(rect,TRUE);

	GetDlgItem(ID_WIZNEXT)->GetWindowRect(&rect);
	ScreenToClient(rect);
	rect.OffsetRect(REPORT_WIZARD_LEFT_OFFSET, REPORT_WIZARD_BOTTOM_HEIGHT);
	GetDlgItem(ID_WIZNEXT)->ShowWindow(SW_HIDE);//hide next page Button
	GetDlgItem(ID_WIZNEXT)->EnableWindow(FALSE);
	m_BtnNext.Create(_T("��һҳ"),
		BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP, rect, this, IDC_BUTTON_NEXT);
	m_BtnNext.SetFont(GetFont());
	m_BtnNext.MoveWindow(rect,TRUE);
	//���ñ��ػ�
	//SetLocalString();

	
	//m_PropStep2.AdjustWin();

	return bResult;
}

LRESULT CPropSheet::WizardBack()//ǰһҳ��
{
	int nPrint = _wtoi(g_sorFileArray.reportConfig.Print); //�Ƿ�Ϊ����
	int nActivePage = GetActiveIndex();
	switch (nActivePage)
	{
	case REPORT_PAGE::PROP_SETTING:
		break;	
	
	case REPORT_PAGE::PROP_FILE_SELECT:
		{
			SetActivePage(PROP_SETTING);
			break;
		}
	
	case PROP_CONFIG:
		{
			//SetActivePage(PROP_FILE_SELECT);
			MAIN->ShowMainSetting(TRUE);
			break;
		}
	
	case REPORT_PAGE::PROP_OUTPUT:
		{
			//MAIN->ShowMainSetting(TRUE);
			SetActivePage(PROP_CONFIG);
			break;
		}
	}

	return 0;
}

LRESULT CPropSheet::WizardNext()//��һҳ��
{
	CPropertyPage* pPage = GetActivePage();
	int nPrint = _wtoi(g_sorFileArray.reportConfig.Print); //�Ƿ�Ϊ����

	int nActivePage = GetActiveIndex();
	switch (nActivePage)
	{
	case REPORT_PAGE::PROP_SETTING:
		{
			if (0!=g_sorFileArray.totalCurveNumber)
			{
				g_sorFileArray.Reset();//zll 2012.9.29 
			}
			pPage->OnWizardNext();
			SetActivePage(PROP_FILE_SELECT);
			break;
		}

	case REPORT_PAGE::PROP_FILE_SELECT://�޸Ļ����һЩ�������ݣ�lzy2014.9.12
		{
			if (0!=g_sorFileArray.totalCurveNumber)
			{
				g_sorFileArray.Reset();//zll 2012.9.29 
			}
			//if(-1 != pPage->OnWizardNext())
				//SetActivePage(PROP_CONFIG);
			pPage->OnWizardNext();
			break;
		}
	case REPORT_PAGE::PROP_CONFIG:  //��ʾ���ߣ�lzy2014.9.12
		{
			if (0!=g_sorFileArray.totalCurveNumber)
			{
				g_sorFileArray.Reset();//zll 2012.9.29 
			}
			
			//SetActivePage(PROP_FILE_SELECT);
			//pPage = GetActivePage();
			pPage->OnWizardNext();
			break;
		}
	case REPORT_PAGE::PROP_OUTPUT:
		break;
	//case REPORT_PAGE::PROP_WAVE_ITEM:
	//	{
	//		//if (nPrint == 1) //����
	//		//	SetActivePage(PROP_WAVE_SETTING);
	//		//else //�ܱ�
	//		//	SetActivePage(PROP_RPT_SETTING);
	//		//pPage->OnWizardNext();
	//		if (0!=g_sorFileArray.totalCurveNumber)
	//		{
	//			g_sorFileArray.Reset();//zll 2012.9.29 
	//		}
	//		pPage->OnWizardNext();//zll,2012.9.4
	//		SetActivePage(PROP_FILE_SELECT);			
	//		break;
	//	}
	//case REPORT_PAGE::PROP_WAVE_SETTING:
	//	{
	//		if (0!=g_sorFileArray.totalCurveNumber)
	//		{
	//			g_sorFileArray.Reset();//zll 2012.9.29 
	//		}
	//		pPage->OnWizardNext();
	//		SetActivePage(PROP_FILE_SELECT);
	//		break;
	//	}
	//case REPORT_PAGE::PROP_RPT_SETTING:
	//	{
	//		if (0!=g_sorFileArray.totalCurveNumber)
	//		{
	//			g_sorFileArray.Reset();//zll 2012.9.29 
	//		}
	//		pPage->OnWizardNext();
	//		SetActivePage(PROP_FILE_SELECT);
	//		break;
	//	}

	
	}

	return 0;
}

void CPropSheet::OnBtnQuit()
{
	CMainFrame *pMainFrm = MAIN;
	if (pMainFrm)
	{
		//if(g_currentLang == LANG::CHS) 
		//	RecordConfig();
		//else
		//	RecordConfig_EN();//����������Ϣn
        //m_Step4FileSelect.m_listFilePos.DeleteAllItems();
		//m_fileSelectDlg.m_strSelectFiles.RemoveAll(); //zll 2012.10.9
		SetActivePage(PROP_SETTING);
		//����ļ�ѡ��
		g_sorFileArray.Reset();
		//�����¼��б�
		MAIN->m_pEventFormView->FreshEventList();
		//�������ñ���ͷ��ť
		MAIN->m_pReportFormView->m_ReportWizardDlg.m_ReportHeaderDlg.SetReportHeadBtn(1);
		//����WaveForm
		MAIN->m_pWaveFormView->m_leftDlg.PopulateData();
		MAIN->m_pThumbFormView->Invalidate(); //redraw thumbnail
		pMainFrm->ShowReport(FALSE); //���ر�����
		/////zll 2013.5.12
		MAIN->m_pWaveFormView->m_sorDrawStatic.m_vAllData.clear();
		MAIN->m_pWaveFormView->m_sorDrawStatic.DrawCurves(&MAIN->m_pWaveFormView->m_sorDrawStatic.m_dcCurve,-1);
		MAIN->m_pWaveFormView->m_sorDrawStatic.Invalidate();

		MAIN->m_pWaveFormView->GetDlgItem(IDC_EVENT_ADD)->ShowWindow(TRUE);//zll 2013.6.5 ��
		MAIN->m_pWaveFormView->GetDlgItem(IDC_EVENT_DELETE)->ShowWindow(TRUE);//zll 2013.6.5 ��

		//zll 2013.6.9 �˳������򵼺󣬱�������򿪵������ļ��е�·��
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay"), _T("Folder1"),g_sorFileArray.reportConfig.Folder1);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay_EN"), _T("Folder1"),g_sorFileArray.reportConfig.Folder1);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay_PT"), _T("Folder1"),g_sorFileArray.reportConfig.Folder1);
	}
}

void CPropSheet::OnBtnNext()
{
	WizardNext();
}

void CPropSheet::OnBtnPrev()
{
	WizardBack();
}

void CPropSheet::SetLocalString()
{
	//���ñ���
	CString strTitle = IniUtil::ReadResString(_T("IDS_REPORT_WIZARD_TITLE"));
	SetTitle(strTitle);

	//��һҳ
	CString strPrev = IniUtil::ReadResString(_T("IDS_REPORT_WIZARD_PREV"));
	m_BtnPrev.SetWindowText(strPrev);

	//��һҳ
	CString strNext = IniUtil::ReadResString(_T("IDS_REPORT_WIZARD_NEXT"));
	m_BtnNext.SetWindowText(strNext);

	//�ر�
	CString strQuit = IniUtil::ReadResString(_T("IDS_REPORT_WIZARD_QUIT"));
	GetDlgItem(IDC_BUTTON_QUIT)->SetWindowText(strQuit);

	//����Page���ػ�
	if (m_PropStep1.GetSafeHwnd())
		m_PropStep1.SetLocalString();
	if (m_PropStep2.GetSafeHwnd())
		//m_PropStep2.AdjustWin();
		m_PropStep2.SetLocalString();
	if (m_PropStep3.GetSafeHwnd())
		m_PropStep3.SetLocalString();
	if (m_PropStep3Rpt.GetSafeHwnd())
		m_PropStep3Rpt.SetLocalString();
	if (m_PropStep4.GetSafeHwnd())
		m_PropStep4.SetLocalString();
	if (m_PropStep5.GetSafeHwnd())
		m_PropStep5.SetLocalString();
	if (m_PropStep6.GetSafeHwnd())
		m_PropStep6.SetLocalString();
	/*if (m_pageFileSelect.GetSafeHwnd())
	    m_pageFileSelect.SetLocalString();*/
	//if (m_Step4FileSelect.GetSafeHwnd())
	//m_Step4FileSelect.SetLocalString();
}

BOOL CPropSheet::EnablePrevBtn(BOOL bEnable)
{
	if (NULL == m_BtnPrev.GetSafeHwnd())
		return FALSE;

	return m_BtnPrev.EnableWindow(bEnable);
}

BOOL CPropSheet::EnableNextBtn(BOOL bEnable)
{
	if (NULL == m_BtnNext.GetSafeHwnd())
		return FALSE;

	return m_BtnNext.EnableWindow(bEnable);
}

void CPropSheet::SetReportHeadBtn(int nCurPageIndex)//���ñ���ͷ��ť��ʾ
{
	if (nCurPageIndex > m_nAlreaySettingPageIndex)
	{
		m_nAlreaySettingPageIndex = nCurPageIndex;
		MAIN->m_pReportFormView->m_ReportWizardDlg.m_ReportHeaderDlg.SetReportHeadBtn(m_nAlreaySettingPageIndex);
	}
}