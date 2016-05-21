// MainSetting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OTDR.h"
#include "MainSetting.h"

extern SorFileArray g_sorFileArray;
// CMainSetting

IMPLEMENT_DYNCREATE(CMainSetting, CFormView)

CMainSetting::CMainSetting()
	: CFormView(CMainSetting::IDD)
{

}

CMainSetting::~CMainSetting()
{
}

void CMainSetting::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_STEP3, m_btnStep3);
	DDX_Control(pDX, IDC_BTN_STEP4, m_btnStep4);
	DDX_Control(pDX, IDC_BTN_STEP5, m_btnStep5);
	DDX_Control(pDX, IDC_TAB_MAINSETTING, m_tab);
}

BEGIN_MESSAGE_MAP(CMainSetting, CFormView)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_PREV, &CMainSetting::OnBnClickedBtnPrev)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CMainSetting::OnBnClickedBtnNext)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CMainSetting::OnBnClickedBtnCancel)
	ON_BN_CLICKED(IDC_BTN_STEP3, &CMainSetting::OnBnClickedBtnStep3)
	ON_BN_CLICKED(IDC_BTN_STEP4, &CMainSetting::OnBnClickedBtnStep4)
	ON_BN_CLICKED(IDC_BTN_STEP5, &CMainSetting::OnBnClickedBtnStep5)
END_MESSAGE_MAP()


// CMainSetting ���

#ifdef _DEBUG
void CMainSetting::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMainSetting::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMainSetting ��Ϣ�������

BOOL CMainSetting::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CMainSetting::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	//���ð�ťλͼ
	m_btnStep3.SetIcon(IDI_WIZARD_STEP3U,32,32,IDI_WIZARD_STEP3D,32,32);
	m_btnStep4.SetIcon(IDI_WIZARD_STEP4U,32,32,IDI_WIZARD_STEP4D,32,32);
	m_btnStep5.SetIcon(IDI_WIZARD_STEP5U,32,32,IDI_WIZARD_STEP5U,32,32);
	//��ʼ��Tabҳ
	InitTab();
}

void CMainSetting::SetLocalString()
{
	//���ñ���
	CString strTitle = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_TAB_TITLE"));
	TCITEM item;
	item.pszText=strTitle.GetBuffer();
	item.mask=TCIF_TEXT;
	m_tab.SetItem(0,&item);
	strTitle.ReleaseBuffer();
	//��һҳ
	CString strPrev = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_PREV"));
	GetDlgItem(IDC_BTN_PREV)->SetWindowText(strPrev);
	//��һҳ
	CString strNext = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_NEXT"));
	GetDlgItem(IDC_BTN_NEXT)->SetWindowText(strNext);
	//ȡ��
	CString strCancel = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_CANCEL"));
	GetDlgItem(IDC_BTN_CANCEL)->SetWindowText(strCancel);

	m_pageMainSetting.SetLocalString();
}

void CMainSetting::InitTab()
{
	//��������ҳ;
	if (NULL == m_pageMainSetting.GetSafeHwnd())
	{
		m_pageMainSetting.Create(IDD_MAIN_SETTING, &m_tab);
		//����tab����
		//���ñ���
		CString strTitle = _T("");
		m_tab.InsertItem(0,strTitle);
	}
}

void CMainSetting::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	//��ȡTAB�Ŀͻ��˾��ο򣬴Ӷ����ø�����ҳ��TAB�ϵ�����λ��
	if(m_tab.GetSafeHwnd() == NULL ||
		m_pageMainSetting.GetSafeHwnd() == NULL)
		return;

	CRect rs;
	m_tab.GetClientRect(&rs);
	rs.top+=20;
	rs.bottom-=4;
	rs.left+=4;
	rs.right-=4;

	//��������ҳ�Ĵ�С��λ��
	m_pageMainSetting.MoveWindow(&rs);

	//Ĭ�ϵ�һҳ��ʾ
	m_pageMainSetting.ShowWindow(TRUE);
	m_tab.SetCurSel(0);
}

void CMainSetting::OnBnClickedBtnPrev()
{
	MAIN->ShowMainSetting(FALSE);
	MAIN->m_pReportFormView->m_ReportWizardDlg.m_pPropSheet->SetActivePage(REPORT_PAGE::PROP_FILE_SELECT);
	//MAIN->m_pReportFormView->m_ReportWizardDlg.m_pPropSheet->SetActivePage(REPORT_PAGE::PROP_CONFIG);

}

void CMainSetting::OnBnClickedBtnNext()
{
	MAIN->ShowMainSetting(FALSE);
	//MAIN->m_pReportFormView->m_ReportWizardDlg.m_pPropSheet->SetActivePage(REPORT_PAGE::PROP_OUTPUT);
	MAIN->m_pReportFormView->m_ReportWizardDlg.m_pPropSheet->SetActivePage(REPORT_PAGE::PROP_CONFIG);

}

void CMainSetting::OnBnClickedBtnCancel()
{
	//ȡ��
	MAIN->ShowMainSetting(FALSE);
	MAIN->m_pReportFormView->m_ReportWizardDlg.m_pPropSheet->OnBtnQuit();
}

void CMainSetting::OnBnClickedBtnStep3()
{
	MAIN->ShowMainSetting(FALSE);
	MAIN->m_pReportFormView->m_ReportWizardDlg.m_pPropSheet->SetActivePage(REPORT_PAGE::PROP_FILE_SELECT);
}

void CMainSetting::OnBnClickedBtnStep4()
{
}

void CMainSetting::OnBnClickedBtnStep5()
{
	MAIN->ShowMainSetting(FALSE);
	MAIN->m_pReportFormView->m_ReportWizardDlg.m_pPropSheet->SetActivePage(REPORT_PAGE::PROP_OUTPUT);
}
