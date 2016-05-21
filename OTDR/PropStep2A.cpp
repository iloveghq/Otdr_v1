// PropStep2A.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OTDR.h"
#include "PropStep2A.h"


// CPropStep2A �Ի���

IMPLEMENT_DYNAMIC(CPropStep2A, CPropertyPage)

CPropStep2A::CPropStep2A()
	: CPropertyPage(CPropStep2A::IDD)
{
	
}

CPropStep2A::~CPropStep2A()
{
}

void CPropStep2A::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPropStep2A, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CPropStep2A::OnBnClickedOk)
END_MESSAGE_MAP()


// CPropStep2A ��Ϣ�������

BOOL CPropStep2A::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_prop.AddPage(&m_propContent);
	m_prop.AddPage(&m_propConfig);
	/*m_prop.m_psh.dwFlags &= ~PSH_HASHELP;	
	m_prop.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_propContent.m_psp.dwFlags &=  ~PSH_HASHELP;
	m_propConfig.m_psp.dwFlags &=  ~PSH_HASHELP;*/
	
	//�޸ĳߴ�
	CRect rc;
	GetWindowRect(rc);
	m_prop.Create(this, WS_CHILD | WS_VISIBLE, WS_EX_STATICEDGE);

	m_prop.SetWindowPos(NULL, 0, 0, rc.Width() * 11 / 12, rc.Height() * 11 / 12, SWP_SHOWWINDOW);
	//m_prop.DoModal();
	
	//�޸�����ҳtab������
	TC_ITEM item;
	item.mask = TCIF_TEXT;
	item.pszText = _T("����");
	m_prop.GetTabControl()->SetItem(0, &item);
	item.pszText = _T("����");
	m_prop.GetTabControl()->SetItem(1, &item);

	//m_prop
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//��������λ��
void CPropStep2A::AdjustWin()
{
	MoveWindow(m_rcLocation);
}

HBRUSH CPropStep2A::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void CPropStep2A::OnBnClickedOk()
{
	OnOK();
}
