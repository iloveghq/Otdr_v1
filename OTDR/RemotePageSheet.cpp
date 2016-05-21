#include "StdAfx.h"
#include "RemotePageSheet.h"
#include "Util.h"
#include "ClientSocket.h"

extern SorFileArray g_sorFileArray;

IMPLEMENT_DYNAMIC(CRemotePageSheet, CPropertySheet)

CRemotePageSheet::CRemotePageSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
: CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_PageSystemSetting);
	AddPage(&m_PageTestSetup);
}

CRemotePageSheet::~CRemotePageSheet(void)
{

}

BEGIN_MESSAGE_MAP(CRemotePageSheet, CPropertySheet)
	ON_MESSAGE(SOCK_ONRECEIVE_DATA_READY, OnSocketDataReady)
END_MESSAGE_MAP()

BOOL CRemotePageSheet::OnInitDialog()
{
	CPropertySheet::OnInitDialog();
	//����Help��ť
	GetDlgItem(IDHELP)->ShowWindow(SW_HIDE);

	//�ƶ�OK��CANCEL �� Applyλ��
	CRect helpRect;
	CRect applyRect;
	CRect cancelRect;
	GetDlgItem(IDHELP)->GetWindowRect(&helpRect);
	GetDlgItem(ID_APPLY_NOW)->GetWindowRect(&applyRect);
	GetDlgItem(IDCANCEL)->GetWindowRect(&cancelRect);
	ScreenToClient(helpRect);
	GetDlgItem(ID_APPLY_NOW)->MoveWindow(helpRect);
	ScreenToClient(applyRect);
	GetDlgItem(IDCANCEL)->MoveWindow(&applyRect);
	ScreenToClient(cancelRect);
	GetDlgItem(IDOK)->MoveWindow(&cancelRect);

	//ȷ����ť
	CString strOK = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_OK"));
	GetDlgItem(IDOK)->SetWindowText(strOK);

	//ȡ����ť
	CString strCancel = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_CANCEL"));
	GetDlgItem(IDCANCEL)->SetWindowText(strCancel);

	//���ñ���
	CString strTitle = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_SYSTEM_SETTING"));
	SetWindowText(strTitle);

	//ϵͳ���ñ���
	strTitle = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_SYSTEM_TITLE"));
	SetPageTitle(0, strTitle);

	//�������ñ���
	strTitle = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_TEST_TITLE"));
	SetPageTitle(1, strTitle);

	return TRUE;
}

BOOL CRemotePageSheet::SetPageTitle (int nPage, const CString& strText)
{
	CTabCtrl* pTab = GetTabControl();
	ASSERT (pTab);

	TC_ITEM ti;
	ti.mask = TCIF_TEXT;
	ti.pszText = (LPWSTR)(LPCTSTR)strText;
	VERIFY (pTab->SetItem (nPage, &ti));

	return TRUE;
} 


/* /��¼�û�������Ϣ��Config.ini
* @param wParam  ������Ϣ����
* @param lParam ��Ϣ�Ƿ�ı�
*/
LRESULT CRemotePageSheet::OnRecordConfigChange(WPARAM wParam, LPARAM lParam)
{
	
	return 1;
}

bool CRemotePageSheet::CheckInputValid(const CString& strVal)
{


	return true;
}

LRESULT CRemotePageSheet::OnEnableApply(WPARAM wParam, LPARAM lParam)
{
	GetDlgItem(ID_APPLY_NOW)->EnableWindow(TRUE);
	GetDlgItem(IDOK)->EnableWindow(TRUE);

	return 0;
}

LRESULT CRemotePageSheet::OnSocketDataReady(WPARAM wParam, LPARAM lParam)
{
	int nCode = (int)wParam;
	if (nCode == 1) //ϵͳ����
	{
		m_PageSystemSetting.SetLocaleString();
	}
	else if(nCode == 2) //��������
	{
		m_PageTestSetup.SetLocaleString();
	}
	return 0;
} 
