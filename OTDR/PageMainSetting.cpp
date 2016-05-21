// PageMainSetting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OTDR.h"
#include "PageMainSetting.h"
#include "Command.h"
#include "EventGenPageSheet.h"

extern SorFileArray g_sorFileArray;
// CPageMainSetting �Ի���

IMPLEMENT_DYNAMIC(CPageMainSetting, CPropertyPage)

CPageMainSetting::CPageMainSetting()
	: CPropertyPage(CPageMainSetting::IDD)
{

}

CPageMainSetting::~CPageMainSetting()
{
}

void CPageMainSetting::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STEP5_WAVE, m_fileList);
}


BEGIN_MESSAGE_MAP(CPageMainSetting, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_STEP5_OPENFILE, &CPageMainSetting::OnBnClickedBtnStep5Openfile)
	ON_BN_CLICKED(IDC_BTN_STEP5_AUTO, &CPageMainSetting::OnBnClickedBtnStep5Auto)
END_MESSAGE_MAP()


// CPageMainSetting ��Ϣ�������

BOOL CPageMainSetting::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	//��ʼ��ListFile
	InitListFile();

	SetLocalString();

	/*                      ���¼�����                            */
	GetDlgItem(IDC_BTN_STEP5_OPENFILE)->EnableWindow(FALSE);
	CButton* pBtnFilePos = (CButton*)GetDlgItem(IDC_RADIO_STEP5_POS);
	pBtnFilePos->SetCheck(TRUE);
	//����
	GetDlgItem(IDC_RADIO_STEP5_NEG)->EnableWindow(FALSE);

	m_potdrTrace=&MAIN->m_pWaveFormView->m_sorDrawStatic;

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CPageMainSetting::SetLocalString()
{
	/*                      ���¼�����                            */
	CString strMasterSetting = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_MASTEREVENT_SETTING"));
	GetDlgItem(IDC_STATIC_STEP5_TITLE)->SetWindowText(strMasterSetting);
	//���ļ�
	CString strOpenFile = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_OPENFILE"));
	GetDlgItem(IDC_BTN_STEP5_OPENFILE)->SetWindowText(strOpenFile);
	//�ļ���
	CString strFileName = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_FILENAME"));
	GetDlgItem(IDC_STATIC_STEP5_FILENAME)->SetWindowText(strFileName);
	//�Զ�����
	CString strAuto = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_AUTO_GENERATE"));
	GetDlgItem(IDC_BTN_STEP5_AUTO)->SetWindowText(strAuto);
	/*                      �����ļ�                            */
	CString strWaveFile = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_WAVEFILE"));
	GetDlgItem(IDC_STATIC_STEP5_WAVEFILE)->SetWindowText(strWaveFile);
	//����
	CString strDirect = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_DIRECT"));
	GetDlgItem(IDC_STATIC_STEP5_DIRECT)->SetWindowText(strDirect);
	//����
	CString strPos = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_DIRECT_POS"));
	CButton* pBtnFilePos = (CButton*)GetDlgItem(IDC_RADIO_STEP5_POS);
	pBtnFilePos->SetWindowText(strPos);
	//����
	CString strNeg = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_DIRECT_NEG"));
	GetDlgItem(IDC_RADIO_STEP5_NEG)->SetWindowText(strNeg);
	//������
	CString strWavePos = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_WAVE_POS"));
	GetDlgItem(IDC_STATIC_STEP5_POS_WAVE)->SetWindowText(strWavePos);
	//������
	CString strWaveNeg = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_WAVE_NEG"));
	GetDlgItem(IDC_STATIC_STEP5_NEG_WAVE)->SetWindowText(strWaveNeg);
	/*                      �༭ģʽ                            */
	CString strEditMode= IniUtil::ReadResString(_T("IDS_REPORT_STEP5_EDIT_MODE"));
	GetDlgItem(IDC_STATIC_STEP5_EDITMODE)->SetWindowText(strEditMode);
	//�༭�¼�
	CString strEventEdit = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_EVENT_MODE"));
	GetDlgItem(IDC_RADIO_STEP5_EDITEVENT)->SetWindowText(strEventEdit);
	//�༭����ı��
	CString strAllLossMark = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_EDIT_ALLLOSS_MARK"));
	GetDlgItem(IDC_RADIO_STEP5_LOSSMARK)->SetWindowText(strAllLossMark);
	//�༭������α���
	CString strWaveScale = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_WAVE_SCALE"));
	GetDlgItem(IDC_RADIO_STEP5_OUTPUT_WAVE)->SetWindowText(strWaveScale);
	//�洢���ļ�
	CString strMaster = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_MASTER"));
	GetDlgItem(IDC_BTN_STEP5_STORAGE)->SetWindowText(strMaster);
}

void CPageMainSetting::InitListFile()
{
	//******��ʼ��m_fileList
	m_fileList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CString strOrder=_T("No");
	CString strName=_T("�ļ���");
	m_fileList.InsertColumn(0,strOrder,LVCFMT_LEFT,30);
	m_fileList.InsertColumn(1,strName,LVCFMT_LEFT,130);
}

void CPageMainSetting::PopulateFileList()
{
	g_sorFileArray.Reset();
	//����ļ��б�
	int item_Count=MAIN->m_pReportFormView->m_ReportWizardDlg.m_pPropSheet->m_PropStep4.m_pageFileSelect.m_fileSelectDlg.m_strSelectFiles.GetCount();
	if (0!=item_Count)
	{
		m_fileList.DeleteAllItems();
	}
	for (int i=0; i < item_Count; i++)
	{
		CString strFileIndex;
		strFileIndex.Format(_T("%d"), i+1);
		CString strFile;
		strFile = MAIN->m_pReportFormView->m_ReportWizardDlg.m_pPropSheet->m_PropStep4.m_pageFileSelect.m_fileSelectDlg.m_strSelectFiles.GetAt(i);
		m_fileList.InsertItem(i, _T(""));
		m_fileList.SetItemText(i, 0, strFileIndex);
		m_fileList.SetItemText(i, 1, strFile);
		//��ʾ����
		OpenCurveFile(strFile);
	}
}

void CPageMainSetting::AddFileList()
{
	//����ļ��б�
	int item_Count=MAIN->m_pReportFormView->m_ReportWizardDlg.m_pPropSheet->m_PropStep4.m_pageFileSelect.m_fileSelectDlg.m_strSelectFiles.GetCount();
	if (0!=item_Count)
	{
		m_fileList.DeleteAllItems();
	}
	m_potdrTrace->m_vAllData.clear();//zll 2013.6.6 �� ��ʱ���m_vAllData�����ݣ�����m_vAllData����һֱ���ӣ����û�������ͬһ�����ߡ���һҳ������һҳ�������������ߴ򿪵�����ɫΪ��ɫ�����
	for (int i=0; i < MAIN->m_pReportFormView->m_ReportWizardDlg.m_pPropSheet->m_PropStep4.m_pageFileSelect.m_fileSelectDlg.m_strSelectFiles.GetCount(); i++)
	{
		CString strFileIndex;
		strFileIndex.Format(_T("%d"), i+1);
		CString strFile;
		strFile = MAIN->m_pReportFormView->m_ReportWizardDlg.m_pPropSheet->m_PropStep4.m_pageFileSelect.m_fileSelectDlg.m_strSelectFiles.GetAt(i);
		m_fileList.InsertItem(i, _T(""));
		m_fileList.SetItemText(i, 0, strFileIndex);
		m_fileList.SetItemText(i, 1, strFile);
		//����ʾ8������ 
		if (i<8)
		{
			OpenCurveFile(strFile);//�򿪲���ʾ����
		}
		else
			continue;//��9�����߿�ʼ���ô򿪲���ʾ�������˷�ʱ�� zll 2013.6.8
	}
	//_CrtDumpMemoryLeaks();//�����ڴ�й¶
}

void CPageMainSetting::OnBnClickedBtnStep5Openfile()
{
	MAIN->OnOpen();
}

void CPageMainSetting::OnBnClickedBtnStep5Auto()
{
	CEventGenPageSheet configSheet(_T("����"));
	configSheet.DoModal();
}
