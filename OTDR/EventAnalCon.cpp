// EventAnalCon.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OTDR.h"
#include "EventAnalCon.h"

extern SorFileArray g_sorFileArray;
// CEventAnalCon �Ի���

IMPLEMENT_DYNAMIC(CEventAnalCon, CPropertyPage)

CEventAnalCon::CEventAnalCon()
	: CPropertyPage(CEventAnalCon::IDD)
{

}

CEventAnalCon::~CEventAnalCon()
{
}

void CEventAnalCon::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_REFRACTION, m_spinReflect);
	DDX_Control(pDX, IDC_SPIN_REFRACTION2, m_spinReflect1550);
	DDX_Control(pDX, IDC_SPIN_REFRACTION4, m_spinReflect1625);
}


BEGIN_MESSAGE_MAP(CEventAnalCon, CPropertyPage)
END_MESSAGE_MAP()


// CEventAnalCon ��Ϣ�������

BOOL CEventAnalCon::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	SetLocaleString();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CEventAnalCon::SetLocaleString()
{
	//������
	CString strReflect= IniUtil::ReadResString(_T("IDS_EVENT_CON_REFLECTION"));
	GetDlgItem(IDC_STATIC_REFRACTION)->SetWindowText(strReflect);
	m_spinReflect.SetSpin(4,1.4000,1.6000,0.0001);
	//m_spinReflect.SetPos(g_sorFileArray.eventAnalConfig.Reflect);
	m_spinReflect.SetPos(g_sorFileArray.eventAnalConfig.Refraction);//zll,2012.9.18 ӦΪ�����ʣ�ԭ���Ƿ���

	m_spinReflect1550.SetSpin(4,1.4000,1.6000,0.0001);
	m_spinReflect1550.SetPos(g_sorFileArray.eventAnalConfig.Refraction1550);//zll,2012.9.18

	m_spinReflect1625.SetSpin(4,1.0000,2.0000,0.0001);
	m_spinReflect1625.SetPos(g_sorFileArray.eventAnalConfig.Refraction1625);//zll,2013.5.10 ��

	//�˲���
	CString strFilter = IniUtil::ReadResString(_T("IDS_EVENT_CON_FILTER"));
	GetDlgItem(IDC_STATIC_FILTER)->SetWindowText(strFilter);
}

BOOL CEventAnalCon::OnApply()
{
	//������1310nm
	g_sorFileArray.eventAnalConfig.Refraction = m_spinReflect.GetPos();
	CString strRefraction;
	strRefraction.Format(_T("%.4f"), g_sorFileArray.eventAnalConfig.Refraction);
	IniUtil::WriteSingleConfigInfo(_T("EventAnal"), _T("Refraction"), strRefraction);

	//������1550nm
	g_sorFileArray.eventAnalConfig.Refraction1550 = m_spinReflect1550.GetPos();
	CString strRefraction1550;
	strRefraction1550.Format(_T("%.4f"), g_sorFileArray.eventAnalConfig.Refraction1550);
	IniUtil::WriteSingleConfigInfo(_T("EventAnal"), _T("Refraction1550"), strRefraction1550);

	//������1625nm//zll 2013.5.10 ��
	g_sorFileArray.eventAnalConfig.Refraction1625 = m_spinReflect1625.GetPos();
	CString strRefraction1625;
	strRefraction1625.Format(_T("%.4f"), g_sorFileArray.eventAnalConfig.Refraction1625);
	IniUtil::WriteSingleConfigInfo(_T("EventAnal"), _T("Refraction1625"), strRefraction1625);

	return CPropertyPage::OnApply();
}