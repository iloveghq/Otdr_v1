#include "StdAfx.h"
#include "PrintSheet.h"

IMPLEMENT_DYNAMIC(CPrintSheet, CPropertySheet)

CPrintSheet::CPrintSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
: CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_PrintHeadTail);
	AddPage(&m_PrintMargin);
}

CPrintSheet::~CPrintSheet(void)
{

}

BEGIN_MESSAGE_MAP(CPrintSheet, CPropertySheet)
	//ON_MESSAGE(WM_NOTIFY_RECORD_CHANGE, &CPrintSheet::OnRecordConfigChange)
	//ON_MESSAGE(WM_NOTIFY_ENABLE_APPLY, &CPrintSheet::OnEnableApply)
END_MESSAGE_MAP()

BOOL CPrintSheet::OnInitDialog()
{
	CPropertySheet::OnInitDialog();
	//ҳü/ҳ�ţ����ñ���
	CString strPageTitle=IniUtil::ReadResString(_T("IDS_PRINT_HEADERTAIL"));
    SetPageTitle(0,strPageTitle);

	strPageTitle=IniUtil::ReadResString(_T("IDS_PRINT_MARGIN_SETTING"));
	SetPageTitle(1,strPageTitle);
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

	//���ñ���
	CString strTitle;
	//strTitle.LoadString(IDS_SETTING);
	//SetWindowText(strTitle);

	return TRUE;
}

/* /��¼�û�������Ϣ��Config.ini
* @param wParam  ������Ϣ����
* @param lParam ��Ϣ�Ƿ�ı�
*/
LRESULT CPrintSheet::OnRecordConfigChange(WPARAM wParam, LPARAM lParam)
{
	
	return 1;
}

bool CPrintSheet::CheckInputValid(const CString& strVal)
{
	return true;
}

LRESULT CPrintSheet::OnEnableApply(WPARAM wParam, LPARAM lParam)
{
	GetDlgItem(ID_APPLY_NOW)->EnableWindow(TRUE);
	GetDlgItem(IDOK)->EnableWindow(TRUE);

	return 0;
}

BOOL CPrintSheet::SetPageTitle (int nPage, const CString& strText)
{
	CTabCtrl* pTab = GetTabControl();
	ASSERT (pTab);

	TC_ITEM ti;
	ti.mask = TCIF_TEXT;
	ti.pszText = (LPWSTR)(LPCTSTR)strText;
	VERIFY (pTab->SetItem (nPage, &ti));

	return TRUE;
} 