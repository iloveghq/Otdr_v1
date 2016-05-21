// ReportView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OTDR.h"
#include "ReportView.h"


// CReportView �Ի���

IMPLEMENT_DYNAMIC(CReportView, CDialog)

CReportView::CReportView(CWnd* pParent /*=NULL*/)
	: CDialog(CReportView::IDD, pParent)
{

}

CReportView::~CReportView()
{
}

void CReportView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_IMG, m_ImgStatic);
	DDX_Control(pDX, IDC_BUTTON_DRAW, m_DrawButton);
}


BEGIN_MESSAGE_MAP(CReportView, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CReportView ��Ϣ�������

BOOL CReportView::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ImgStatic.SetBkColor(RGB(172, 168, 153));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CReportView::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (NULL == m_ImgStatic.GetSafeHwnd() ||
		NULL == m_DrawButton.GetSafeHwnd())
		return;

	//����Static
	CRect clientRect;
	GetWindowRect(clientRect);
	ScreenToClient(clientRect);
	clientRect.DeflateRect(8, 8, 8, 8);
	m_ImgStatic.MoveWindow(clientRect);
	//��ͼ��ť
	clientRect.DeflateRect(80, 8, 80, 8);
	m_DrawButton.MoveWindow(clientRect);
}
