// EventGenerateCon.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OTDR.h"
#include "EventGenerateCon.h"


// CEventGenerateCon �Ի���

IMPLEMENT_DYNAMIC(CEventGenerateCon, CPropertyPage)

CEventGenerateCon::CEventGenerateCon()
	: CPropertyPage(CEventGenerateCon::IDD)
{

}

CEventGenerateCon::~CEventGenerateCon()
{
}

void CEventGenerateCon::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_PERMITRANGE, m_spinPermRange);
	DDX_Control(pDX, IDC_SPIN_LERATEFREQ, m_spinRelaDegree);
	DDX_Control(pDX, IDC_SPIN_BOTHPERMITRANGE, m_spinBothPermRange);
}


BEGIN_MESSAGE_MAP(CEventGenerateCon, CPropertyPage)
END_MESSAGE_MAP()


// CEventGenerateCon ��Ϣ�������

BOOL CEventGenerateCon::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	/*                 ���¼��Զ����ɷ���                                          */
	CString strMethod= IniUtil::ReadResString(_T("IDS_EVENT_CON_GENERATE_METHOD"));
	GetDlgItem(IDC_STATIC_GENERATE)->SetWindowText(strMethod);
	//���¼��б�������
	CString strEventList= IniUtil::ReadResString(_T("IDS_EVENT_CON_EVENTLIST"));
	CButton* pBtnEventList = (CButton*)GetDlgItem(IDC_RDO_EVENTLIST);
	pBtnEventList->SetWindowText(strEventList);
	//���Զ�����������
	CString strAutoSearch = IniUtil::ReadResString(_T("IDS_EVENT_CON_AUTOSEARCH"));
	CButton* pBtnAutoSearch = (CButton*)GetDlgItem(IDC_RDO_AUTOSEARCH);
	pBtnAutoSearch->SetWindowText(strAutoSearch);
	CString strGenMethod = IniUtil::ReadSingleConfigInfo(_T("PageSetAutoGen"), _T("GenMethod"));
	if (strGenMethod.CompareNoCase(_T("1")) == 0)
	{
		pBtnEventList->SetCheck(TRUE);
		pBtnAutoSearch->SetCheck(FALSE);
	}
	else
	{
		pBtnEventList->SetCheck(FALSE);
		pBtnAutoSearch->SetCheck(TRUE);
	}
	/*                 ��������                                          */
	CString strGenerateCon= IniUtil::ReadResString(_T("IDS_EVENT_CON_GENERATE_CON"));
	GetDlgItem(IDC_STATIC_CONDITION)->SetWindowText(strGenerateCon);
	CString strPermRangeVal = IniUtil::ReadSingleConfigInfo(_T("PageSetAutoGen"), _T("PermRange"));
	//�����������Χ
	CString strPermitRange=IniUtil::ReadResString(_T("IDS_EVENT_CON_RANGE_PERMIT"));
	GetDlgItem(IDC_STATIC_PERMITRANGE)->SetWindowText(strPermitRange);
	CString strPermRange = IniUtil::ReadSingleConfigInfo(_T("PageSetAutoGen"), _T("PermRange"));
	int nPermRange = _wtoi(strPermRange);
	m_spinPermRange.SetSpin(0,1,10,1);
	m_spinPermRange.SetPos(nPermRange);
	//��Զ���
	CString strRelativeDegree=IniUtil::ReadResString(_T("IDS_EVENT_CON_RELATIVE_DEGREE"));
	GetDlgItem(IDC_STATIC_LERATEFREQ)->SetWindowText(strRelativeDegree);
	CString strLerateFreq = IniUtil::ReadSingleConfigInfo(_T("PageSetAutoGen"), _T("LerateFreq"));
	int nLerateFreq = _wtoi(strLerateFreq);
	m_spinRelaDegree.SetSpin(0,1,100,1);
	m_spinRelaDegree.SetPos(nLerateFreq);
	//�����������(˫��)
	CString strBothPermitRange= IniUtil::ReadResString(_T("IDS_EVENT_CON_BOTHRANGE_PERMIT"));
	GetDlgItem(IDC_STATIC_BOTHPERMITRANGE)->SetWindowText(strBothPermitRange);
	CString strBothPermRange = IniUtil::ReadSingleConfigInfo(_T("PageSetAutoGen"), _T("BothPermRange"));
	int nBothPermRange = _wtoi(strBothPermRange);
	m_spinBothPermRange.SetSpin(0,1,10,1);
	m_spinBothPermRange.SetPos(nLerateFreq);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CEventGenerateCon::OnApply()
{
	/*                 ���¼��Զ����ɷ���                                          */
	CButton* pBtnEventList = (CButton*)GetDlgItem(IDC_RDO_EVENTLIST);//���¼��б�������
	CButton* pBtnAutoSearch = (CButton*)GetDlgItem(IDC_RDO_AUTOSEARCH);//���Զ�����������
	CString strAutoSearch;
	if (pBtnEventList->GetCheck())
	{
		strAutoSearch = _T("0");
	}
	else if (pBtnAutoSearch->GetCheck())
	{
		strAutoSearch = _T("1");
	}
	IniUtil::WriteSingleConfigInfo(_T("PageSetAutoGen"), _T("GenMethod"), strAutoSearch);
	/*                 ��������                                          */
	//�����������Χ
	CString strPermitRange;
	int nPermitRange = m_spinPermRange.GetPos();
	strPermitRange.Format(_T("%d"), nPermitRange);
	IniUtil::WriteSingleConfigInfo(_T("PageSetAutoGen"), _T("PermRange"), strPermitRange);
	//��Զ���
	CString strRelativeDegree;
	int nRelativeDegree = m_spinRelaDegree.GetPos();
	strRelativeDegree.Format(_T("%d"), nRelativeDegree); //��Զ���
	IniUtil::WriteSingleConfigInfo(_T("PageSetAutoGen"), _T("LerateFreq"), strRelativeDegree);
	//�����������(˫��)
	CString strBothPermitRange;
	int nBothPermitRange = m_spinBothPermRange.GetPos();
	strBothPermitRange.Format(_T("%d"), strBothPermitRange);
	IniUtil::WriteSingleConfigInfo(_T("PageSetAutoGen"), _T("BothPermRange"), strBothPermitRange);

	//�����¼���Ϣ


	return CPropertyPage::OnApply();
}
