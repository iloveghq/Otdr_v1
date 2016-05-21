// EventListPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EventListPage.h"
#include "ConfigSheet.h"
#include <winsock2.h>

IMPLEMENT_DYNAMIC(CEventListPage, CPropertyPage)

CEventListPage::CEventListPage()
	: CPropertyPage(CEventListPage::IDD)
{

}

CEventListPage::~CEventListPage()
{
}

void CEventListPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

BOOL CEventListPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	/*               ��Ŀ��ʾ������                                                  */
	CString strItemShowOrNot = IniUtil::ReadResString(_T("IDS_SCREEN_EVENTLIST_ITEMSHOWORNOT"));
	GetDlgItem(IDC_STATIC_EVENTLIST)->SetWindowText(strItemShowOrNot);

	//�ܾ���
	CString strDisTotal = IniUtil::ReadSingleConfigInfo(_T("Event"), _T("V_DisTotal")); 
	CButton* pBtn = (CButton*) GetDlgItem(IDC_CHECK_TOTAL_DIST);
	if (pBtn)
	{
		if (strDisTotal.CompareNoCase(_T("1")) == 0)
			pBtn->SetCheck(TRUE);
		strDisTotal = IniUtil::ReadResString(_T("IDS_SCREEN_TOTAL_DIST"));
		pBtn->SetWindowText(strDisTotal);
	}

	//�������
	CString strDisSection = IniUtil::ReadSingleConfigInfo(_T("Event"), _T("V_DisSection"));
	pBtn = (CButton*) GetDlgItem(IDC_CHECK_SECTION_DIST);
	if (pBtn)
	{
		if (strDisSection.CompareNoCase(_T("1")) == 0)
			pBtn->SetCheck(TRUE);
		strDisSection = IniUtil::ReadResString(_T("IDS_SCREEN_SECTION_DIST"));
		pBtn->SetWindowText(strDisSection);

	}

	//�۽����
	CString strConLoss = IniUtil::ReadSingleConfigInfo(_T("Event"), _T("V_ConLoss"));
	pBtn = (CButton*) GetDlgItem(IDC_CHECK_FUSION_LOSS);
	if (pBtn)
	{
		if (strConLoss.CompareNoCase(_T("1")) == 0)
			pBtn->SetCheck(TRUE);
		strConLoss = IniUtil::ReadResString(_T("IDS_SCREEN_FUSION_LOSS"));
		pBtn->SetWindowText(strConLoss);
	}

	//�ز����
	CString strRefLoss = IniUtil::ReadSingleConfigInfo(_T("Event"), _T("V_RefLoss"));
	pBtn = (CButton*) GetDlgItem(IDC_CHECK_RETURN_LOSS);
	if (pBtn)
	{
		if (strRefLoss.CompareNoCase(_T("1")) == 0)
			pBtn->SetCheck(TRUE);
		strRefLoss = IniUtil::ReadResString(_T("IDS_SCREEN_RETURN_LOSS"));
		pBtn->SetWindowText(strRefLoss);
	}

	//������
	CString strRefQua = IniUtil::ReadSingleConfigInfo(_T("Event"), _T("V_RefQua"));
	pBtn = (CButton*) GetDlgItem(IDC_CHECK_REFLECTION_VAL);
	if (pBtn)
	{
		if (strRefQua.CompareNoCase(_T("1")) == 0)
			pBtn->SetCheck(TRUE);
		strRefQua = IniUtil::ReadResString(_T("IDS_SCREEN_REFLECTION_VAL"));
		pBtn->SetWindowText(strRefQua);
	}

	//�ۼ����
	CString strTotalLoss = IniUtil::ReadSingleConfigInfo(_T("Event"), _T("V_TotalLoss"));
	pBtn = (CButton*) GetDlgItem(IDC_CHECK_CUMULATIVE_LOSS);
	if (pBtn)
	{
		if (strTotalLoss.CompareNoCase(_T("1")) == 0)
			pBtn->SetCheck(TRUE);
		strTotalLoss = IniUtil::ReadResString(_T("IDS_SCREEN_CUMULATIVE_LOSS"));
		pBtn->SetWindowText(strTotalLoss);
	}

	//˥������
	CString strUnitLoss = IniUtil::ReadSingleConfigInfo(_T("Event"), _T("V_UnitLoss"));
	pBtn = (CButton*) GetDlgItem(IDC_CHECK_ATTENU_COEFF);
	if (pBtn)
	{
		if (strUnitLoss.CompareNoCase(_T("1")) == 0)
			pBtn->SetCheck(TRUE);
		strUnitLoss = IniUtil::ReadResString(_T("IDS_SCREEN_ATTENU_COEFF"));
		pBtn->SetWindowText(strUnitLoss);
	}

	//�¼�����
	CString strKind = IniUtil::ReadSingleConfigInfo(_T("Event"), _T("V_Kind"));
	pBtn = (CButton*) GetDlgItem(IDC_CHECK_EVENT_TYPE);
	if (pBtn)
	{
		if (strKind.CompareNoCase(_T("1")) == 0)
			pBtn->SetCheck(TRUE);
		strKind = IniUtil::ReadResString(_T("IDS_SCREEN_EVENT_TYPE"));
		pBtn->SetWindowText(strKind);
	}

	//������
	CString strRefCoef = IniUtil::ReadSingleConfigInfo(_T("Event"), _T("V_RefCoef"));
	 pBtn = (CButton*) GetDlgItem(IDC_CHECK_REFRACTION);
	 if (pBtn)
	 {
		 if (strRefCoef.CompareNoCase(_T("1")) == 0)
			 pBtn->SetCheck(TRUE);
		 strRefCoef = IniUtil::ReadResString(_T("IDS_SCREEN_REFRACTION"));
		 pBtn->SetWindowText(strRefCoef);
	 }

	//ע��
	CString strComment = IniUtil::ReadSingleConfigInfo(_T("Event"), _T("V_Comment"));
	pBtn = (CButton*) GetDlgItem(IDC_CHECK_REMARK);
	if (pBtn)
	{
		if (strComment.CompareNoCase(_T("1")) == 0)
			pBtn->SetCheck(TRUE);
		strComment = IniUtil::ReadResString(_T("IDS_SCREEN_REMARK"));
		pBtn->SetWindowText(strComment);
	}

	//ȱʡ����
	CString strDefault = IniUtil::ReadResString(_T("IDS_SCREEN_DEFAULT"));
	pBtn = (CButton*) GetDlgItem(IDC_BTN_DEFAULT_SETTING);
	if (pBtn)	
		pBtn->SetWindowText(strDefault);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// CEventListPage ��Ϣ�������
BEGIN_MESSAGE_MAP(CEventListPage, CPropertyPage)

END_MESSAGE_MAP()


// CEventListPage ��Ϣ�������
BOOL CEventListPage::OnApply()
{
	/*                ��Ŀ��ʾ������                                       */
	//�ܾ���
	CString strDisTotal;
	CButton* pBtn = (CButton*) GetDlgItem(IDC_CHECK_TOTAL_DIST);
	strDisTotal.Format(_T("%d"), pBtn->GetCheck());
	IniUtil::WriteSingleConfigInfo(_T("Event"), _T("V_DisTotal"), strDisTotal);

	//�������
	CString strDisSection;
	pBtn = (CButton*) GetDlgItem(IDC_CHECK_SECTION_DIST);
	strDisSection.Format(_T("%d"), pBtn->GetCheck());
	IniUtil::WriteSingleConfigInfo(_T("Event"), _T("V_DisSection"), strDisSection);

	//�۽����
	CString strConLoss;
	pBtn = (CButton*) GetDlgItem(IDC_CHECK_FUSION_LOSS);
	strConLoss.Format(_T("%d"), pBtn->GetCheck());
	IniUtil::WriteSingleConfigInfo(_T("Event"), _T("V_ConLoss"), strConLoss);

	//�ز����
	CString strRefLoss;
	pBtn = (CButton*) GetDlgItem(IDC_CHECK_RETURN_LOSS);
	strRefLoss.Format(_T("%d"), pBtn->GetCheck());
	IniUtil::WriteSingleConfigInfo(_T("Event"), _T("V_RefLoss"), strRefLoss);

	//������
	CString strRefQua;
	pBtn = (CButton*) GetDlgItem(IDC_CHECK_REFLECTION_VAL);
	strRefQua.Format(_T("%d"), pBtn->GetCheck());
	IniUtil::WriteSingleConfigInfo(_T("Event"), _T("V_RefQua"), strRefQua);

	//�ۼ����
	CString strTotalLoss;
	pBtn = (CButton*) GetDlgItem(IDC_CHECK_CUMULATIVE_LOSS);
	strTotalLoss.Format(_T("%d"), pBtn->GetCheck());
	IniUtil::WriteSingleConfigInfo(_T("Event"), _T("V_TotalLoss"), strTotalLoss);

	//˥������
	CString strUnitLoss;
	pBtn = (CButton*) GetDlgItem(IDC_CHECK_ATTENU_COEFF);
	strUnitLoss.Format(_T("%d"), pBtn->GetCheck());
	IniUtil::WriteSingleConfigInfo(_T("Event"), _T("V_UnitLoss"), strUnitLoss);

	//�¼�����
	CString strKind;
	pBtn = (CButton*) GetDlgItem(IDC_CHECK_EVENT_TYPE);
	strKind.Format(_T("%d"), pBtn->GetCheck());
	IniUtil::WriteSingleConfigInfo(_T("Event"), _T("V_Kind"), strKind);

	//������
	CString strRefCoef;
	pBtn = (CButton*) GetDlgItem(IDC_CHECK_REFRACTION);
	strRefCoef.Format(_T("%d"), pBtn->GetCheck());
	IniUtil::WriteSingleConfigInfo(_T("Event"), _T("V_RefCoef"), strRefCoef);

	//ע��
	CString strComment;
	pBtn = (CButton*) GetDlgItem(IDC_CHECK_REMARK);
	strComment.Format(_T("%d"), pBtn->GetCheck());
	IniUtil::WriteSingleConfigInfo(_T("Event"), _T("V_Comment"), strComment);

	return TRUE;
}
