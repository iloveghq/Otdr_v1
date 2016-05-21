// GrandWayProgress.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OTDR.h"
#include "GrandwayProgress.h"


// CGrandwayProgress �Ի���

IMPLEMENT_DYNAMIC(CGrandwayProgress, CDialog)

CGrandwayProgress::CGrandwayProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CGrandwayProgress::IDD, pParent)
{

}

CGrandwayProgress::~CGrandwayProgress()
{
}

void CGrandwayProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
}


BEGIN_MESSAGE_MAP(CGrandwayProgress, CDialog)
END_MESSAGE_MAP()


// CGrandwayProgress ��Ϣ�������

BOOL CGrandwayProgress::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_progress.SetRange(0 ,20);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CGrandwayProgress::SetPos(int nPos)
{
	m_progress.SetPos(nPos);
}