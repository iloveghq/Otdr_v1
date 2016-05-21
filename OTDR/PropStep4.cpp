// PropStep4.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OTDR.h"
#include "PropStep4.h"
#include "MainFrm.h"

extern SorFileArray g_sorFileArray;
// CPropStep4 �Ի���

IMPLEMENT_DYNAMIC(CPropStep4, CPropertyPage)

CPropStep4::CPropStep4()
: CPropertyPage(CPropStep4::IDD)
{

}

CPropStep4::~CPropStep4()
{
}

void CPropStep4::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_STEP4_CORENUM, m_spinFiberNum);
	DDX_Control(pDX, IDC_SPIN_STEP4_START_CORENUM, m_spinStartFiber);
	DDX_Control(pDX, IDC_EDIT_STEP4_CORENUM, m_editFiberNum);
	DDX_Control(pDX, IDC_EDIT_STEP4_START_CORENUM, m_editStartFiber);
	DDX_Control(pDX, IDC_TAB_STEP4, m_tab);
}


BEGIN_MESSAGE_MAP(CPropStep4, CPropertyPage)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_STEP4_CORENUM, &CPropStep4::OnDeltaposSpinFiberNum)
END_MESSAGE_MAP()


// CPropStep4 ��Ϣ�������
BOOL CPropStep4::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	//((CPropertySheet*)GetParent())->SetWizardButtons(PSWIZB_NEXT);
	((CPropSheet*)GetParent())->SetReportHeadBtn(4);
	//�ػ�ͼ������
	MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.ReDrawRect(4);
	((CPropSheet*)GetParent())->EnableNextBtn(TRUE);
	((CPropSheet*)GetParent())->EnablePrevBtn(TRUE);
	return CPropertyPage::OnSetActive();
}

LRESULT CPropStep4::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData();
	RecordSetting();
	if (m_pageFileSelect.IsFilePosSelect() == FALSE)
	{
		CString strMsg=IniUtil::ReadResString(_T("IDS_REPORT_STEP4_CHOOSEFILES"));
		MessageBox(strMsg, _T("OTDR Viewer"), MB_OK|MB_ICONEXCLAMATION);
		return -1;
	}
	
	//��ʾ�����ô���
	MAIN->ShowMainSetting(TRUE);
	//����ļ��б�
	MAIN->m_pMainSetting->m_pageMainSetting.AddFileList();

	return 0;
}

BOOL CPropStep4::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	//��ʼ��Tabҳ
	InitTab();
	//���ñ��ػ�
	SetLocalString();
	int nNumerOfFiber = _wtoi(g_sorFileArray.reportConfig.NumberOfFiber);
	m_spinFiberNum.SetSpin(0, 1, 8, 1);
	m_spinFiberNum.SetPos(nNumerOfFiber);
	//��ʼо
	int nStartOfFiberNo = _wtoi(g_sorFileArray.reportConfig.StartOfFiberNo);
	m_spinStartFiber.SetSpin(0, 1, 8, 1);
	m_spinStartFiber.SetPos(nStartOfFiberNo);
	GetDlgItem(IDC_EDIT_STEP4_WAVENUM)->SetWindowText(g_sorFileArray.reportConfig.NumberOfWaveLength);
	GetDlgItem(IDC_EDIT_STEP4_WAVENUM)->EnableWindow(FALSE);
	//���Է���
	if (g_sorFileArray.reportConfig.MeasureDir.CompareNoCase(_T("1")))
		GetDlgItem(IDC_EDIT_STEP4_TEST_DIRECT)->SetWindowText(_T("˫��"));
	else
		GetDlgItem(IDC_EDIT_STEP4_TEST_DIRECT)->SetWindowText(_T("����"));
	GetDlgItem(IDC_EDIT_STEP4_TEST_DIRECT)->EnableWindow(FALSE);
	//�ļ�����
	CComboBox* pCommboxFileType = (CComboBox*)GetDlgItem(IDC_COMBO_FILETYPE);
	pCommboxFileType->InsertString(-1, _T("SOR(*.SOR)"));
	pCommboxFileType->SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pag	es should return FALSE
}

void CPropStep4::SetLocalString()
{
	/*                      �ļ���Ϣ                            */
	TCITEM item;
	CString strTitle = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_TAB_TITLE"));
	item.pszText=strTitle.GetBuffer();
	item.mask=TCIF_TEXT;
	m_tab.SetItem(0,&item);
	strTitle.ReleaseBuffer();

	CString strFileInfo = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILE_INFO"));
	GetDlgItem(IDC_STATIC_STEP4_FILEINFO)->SetWindowText(strFileInfo);
	//о��
	CString strFiberNumber = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FIBER_NUMBER"));
	GetDlgItem(IDC_STATIC_STEP4_CORENUM)->SetWindowText(strFiberNumber);
	//��ʼо
	CString strStartFiber = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_START_FIBER"));
	GetDlgItem(IDC_STATIC_STEP4_START_CORENUM)->SetWindowText(strStartFiber);
	//�ļ�ѡ�񷽷�
	CString strFileSelect = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILE_SELECT_METHOD"));
	GetDlgItem(IDC_STATIC_STEP4_FILEMETHOD)->SetWindowText(strFileSelect);
	//������
	CString strWaveNum = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_WAVE_COUNT"));
	GetDlgItem(IDC_STATIC_STEP4_WAVENUM)->SetWindowText(strWaveNum);
	//���Է���
	CString strTestDirect = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_TEST_DIRECTION"));
	GetDlgItem(IDC_STATIC_STEP4_TESTDIRECT)->SetWindowText(strTestDirect);
	//�ļ�����
	CString strFileFormat= IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILE_FORMAT"));
	GetDlgItem(IDC_STATIC_STEP4_FILETYPE)->SetWindowText(strFileFormat);
	m_pageFileSelect.SetLocalString();
}

void CPropStep4::InitTab()
{
	//��ʼ��m_tab�ؼ��ϵ�����ҳ��ǩ
	//����tab����
	CString strTitle = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_TAB_TITLE"));
	m_tab.InsertItem(0,strTitle);

	//��������ҳ;
	m_pageFileSelect.Create(IDD_REPORT_FILE_SELECT,GetDlgItem(IDC_TAB_STEP4));

	//��ȡTAB�Ŀͻ��˾��ο򣬴Ӷ����ø�����ҳ��TAB�ϵ�����λ��
	CRect rs;
	m_tab.GetClientRect(&rs);
	rs.top+=20;
	rs.bottom-=4;
	rs.left+=4;
	rs.right-=4;

	//��������ҳ�Ĵ�С��λ��
	m_pageFileSelect.MoveWindow(&rs);

	//Ĭ�ϵ�һҳ��ʾ
	m_pageFileSelect.ShowWindow(TRUE);
	m_tab.SetCurSel(0);

}

void CPropStep4::RecordSetting() //����������Ϣ
{
	GetDlgItem(IDC_EDIT_STEP4_CORENUM)->GetWindowText(g_sorFileArray.reportConfig.NumberOfFiber);
	GetDlgItem(IDC_EDIT_STEP4_START_CORENUM)->GetWindowText(g_sorFileArray.reportConfig.StartOfFiberNo);
}

void CPropStep4::OnDeltaposSpinFiberNum(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_EDIT_STEP4_CORENUM)->GetWindowText(g_sorFileArray.reportConfig.NumberOfFiber);
	*pResult = 0;
}