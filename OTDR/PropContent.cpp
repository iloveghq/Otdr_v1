// PropContent.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OTDR.h"
#include "PropContent.h"


// CPropContent �Ի���

IMPLEMENT_DYNAMIC(CPropContent, CPropertyPage)

CPropContent::CPropContent()
	: CPropertyPage(CPropContent::IDD)
	, m_bCheckLogo(FALSE)
	, m_bCheckJob(FALSE)
	, m_bCheckConfig(FALSE)
	, m_bCheckComment(FALSE)
	, m_bCheckTrace(FALSE)
	, m_bCheckEvent(FALSE)
	, m_strSelect(_T(""))
{
	SetLocalString();
}

CPropContent::~CPropContent()
{
}

void CPropContent::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_PROP_CONTENT_LOGO, m_bCheckLogo);
	DDX_Check(pDX, IDC_CHECK_PROP_CONTENT_JOB, m_bCheckJob);
	DDX_Check(pDX, IDC_CHECK_PROP_CONTENT_CONFIG, m_bCheckConfig);
	DDX_Check(pDX, IDC_CHECK_PROP_CONTENT_COMMENT, m_bCheckComment);
	DDX_Check(pDX, IDC_CHECK_PROP_CONTENT_TRACE, m_bCheckTrace);
	DDX_Check(pDX, IDC_CHECK_PROP_CONTENT_EVENT, m_bCheckEvent);
	DDX_Text(pDX, IDS_CNT_SELECT, m_strSelect);
}


BEGIN_MESSAGE_MAP(CPropContent, CPropertyPage)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CPropContent ��Ϣ�������

BOOL CPropContent::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	//return CPropertyPage::OnEraseBkgnd(pDC);
}

void CPropContent::SetLocalString()
{
	//����ע��
	m_strSelect = IniUtil::ReadResString(_T("IDS_CNT_SELECT"));
	m_strLogo = IniUtil::ReadResString(_T("IDC_CHECK_PROP_CONTENT_LOGO"));
	m_strJob = IniUtil::ReadResString(_T("IDC_CHECK_PROP_CONTENT_JOB"));
	m_strCfg = IniUtil::ReadResString(_T("IDC_CHECK_PROP_CONTENT_CONFIG"));
	m_strCmt = IniUtil::ReadResString(_T("IDC_CHECK_PROP_CONTENT_COMMENT"));
	m_strTrace = IniUtil::ReadResString(_T("IDC_CHECK_PROP_CONTENT_TRACE"));
	m_strEvent = IniUtil::ReadResString(_T("IDC_CHECK_PROP_CONTENT_EVENT"));
}
BOOL CPropContent::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	GetDlgItem(IDC_CHECK_PROP_CONTENT_LOGO)->SetWindowText(m_strLogo);
	GetDlgItem(IDC_CHECK_PROP_CONTENT_JOB)->SetWindowText(m_strJob);
	GetDlgItem(IDC_CHECK_PROP_CONTENT_CONFIG)->SetWindowText(m_strCfg);
	GetDlgItem(IDC_CHECK_PROP_CONTENT_COMMENT)->SetWindowText(m_strCmt);
	GetDlgItem(IDC_CHECK_PROP_CONTENT_TRACE)->SetWindowText(m_strTrace);
	GetDlgItem(IDC_CHECK_PROP_CONTENT_EVENT)->SetWindowText(m_strEvent);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
