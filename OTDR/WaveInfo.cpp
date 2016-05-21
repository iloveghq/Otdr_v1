// WaveInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OTDR.h"
#include "WaveInfo.h"


// CWaveInfo �Ի���

IMPLEMENT_DYNAMIC(CWaveInfo, CDialog)

CWaveInfo::CWaveInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CWaveInfo::IDD, pParent)
{

}

CWaveInfo::~CWaveInfo()
{
}

void CWaveInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWaveInfo, CDialog)
END_MESSAGE_MAP()


// CWaveInfo ��Ϣ�������

BOOL CWaveInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	//���öԻ������
	CString strTitle = IniUtil::ReadResString(_T("IDS_WAVEINFO"));
	SetWindowText(strTitle);
	//��Ӧ��
	CString strManufacture = IniUtil::ReadResString(_T("IDS_WAVEINFO_MANUFACTURE"));
	GetDlgItem(IDC_STATIC_MANUFACTURE)->SetWindowText(strManufacture);
	//������
	CString strOperator = IniUtil::ReadResString(_T("IDS_WAVINFO_OPERATOR"));
	GetDlgItem(IDC_STATIC_OPERATOR)->SetWindowText(strOperator);
	//�º�
	CString strLineNum = IniUtil::ReadResString(_T("IDS_WAVEINFO_LINE_NUM"));
	GetDlgItem(IDC_STATIC_NUM)->SetWindowText(strLineNum);
	//�˺�
	CString strFiberNum = IniUtil::ReadResString(_T("IDS_WAVINFO_FIBER_NUM"));
	GetDlgItem(IDC_STATIC_FIBERNUM)->SetWindowText(strFiberNum);
	//��ʼλ��
	CString strStartPos = IniUtil::ReadResString(_T("IDS_WAVEINFO_SRARTPOS"));
	GetDlgItem(IDC_STATIC_STARTPOS)->SetWindowText(strStartPos);
	//��ֹλ��
	CString strEndPos = IniUtil::ReadResString(_T("IDS_WAVEINFO_ENDPOS"));
	GetDlgItem(IDC_STATIC_ENDPOS)->SetWindowText(strEndPos);
	//ȷ����ť
	CString strOk = IniUtil::ReadResString(_T("IDS_WAVEINFO_OK"));
	GetDlgItem(IDOK)->SetWindowText(strOk);
	//ȡ����ť
	CString strCancel = IniUtil::ReadResString(_T("IDS_WAVEINFO_CANCEL"));
	GetDlgItem(IDCANCEL)->SetWindowText(strCancel);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
