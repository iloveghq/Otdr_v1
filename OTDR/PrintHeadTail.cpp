// PrintHeadTail.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OTDR.h"
#include "PrintHeadTail.h"

extern SorFileArray g_sorFileArray;

// CPrintHeadTail �Ի���

IMPLEMENT_DYNAMIC(CPrintHeadTail, CPropertyPage)

CPrintHeadTail::CPrintHeadTail()
	: CPropertyPage(CPrintHeadTail::IDD)
{

}

CPrintHeadTail::~CPrintHeadTail()
{
}

void CPrintHeadTail::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_STARTNUM, m_spinStartNum);
	DDX_Control(pDX, IDC_COMBO_UPDOWN, m_comboxUpDown);
	DDX_Control(pDX, IDC_COMBO_LEFTRIGHT, m_comboxLeftRight);
	DDX_Control(pDX, IDC_SPIN_CHAPTERNUM, m_spinSectionNum);
}


BEGIN_MESSAGE_MAP(CPrintHeadTail, CPropertyPage)
END_MESSAGE_MAP()


// CPrintHeadTail ��Ϣ�������

BOOL CPrintHeadTail::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	/*               ҳ��                                                  */
	CString strPageNum = IniUtil::ReadResString(_T("IDS_PRINT_PAGENUM"));
	GetDlgItem(IDC_STATIC_PAGENUM)->SetWindowText(strPageNum);
	//����ҳ��
	CString strAddPage = IniUtil::ReadResString(_T("IDS_PRINT_ADDPAGE"));
	CButton* pBtnAddPage = (CButton*)GetDlgItem(IDC_CHECK_ADDNUM);
	pBtnAddPage->SetWindowText(strAddPage);
	if (g_sorFileArray.printConfig.AddPage.CompareNoCase(_T("1")) == 0)
	{
		pBtnAddPage->SetCheck(TRUE);
	}
	else
	{
		pBtnAddPage->SetCheck(FALSE);
	}
	//��ʼ��
	CString strStartNum = IniUtil::ReadResString(_T("IDS_PRINT_STARTNO"));
	GetDlgItem(IDC_STATIC_START_NUM)->SetWindowText(strStartNum);
	int nPageNum = _wtoi(g_sorFileArray.printConfig.PageNum);
	m_spinStartNum.SetSpin(0,1,200000,1);
	m_spinStartNum.SetPos(nPageNum);
	//��ӡλ��
	CString strPrintPos = IniUtil::ReadResString(_T("IDS_PRINT_POS"));
	GetDlgItem(IDC_STATIC_PRINT_POS)->SetWindowText(strPrintPos);
	m_comboxUpDown.InsertString(-1, _T("��"));/* �� */
	m_comboxUpDown.InsertString(-1, _T("��"));/* �� */
	int nPagePos1 = _wtoi(g_sorFileArray.printConfig.PagePos1);
	m_comboxUpDown.SetCurSel(nPagePos1);

	m_comboxLeftRight.InsertString(-1, _T("��"));/* �� */
	m_comboxLeftRight.InsertString(-1, _T("��"));/* �� */
	m_comboxLeftRight.InsertString(-1, _T("��"));/* �� */
	int nPagePos2 = _wtoi(g_sorFileArray.printConfig.PagePos2);
	m_comboxLeftRight.SetCurSel(nPagePos2);
	/*               �½ں�                                                    */
	CString strSection = IniUtil::ReadResString(_T("IDS_PRINT_SECTION"));
	GetDlgItem(IDC_STATIC_CHAPTER)->SetWindowText(strSection);
	//�����½ں�
	CString strAddSection = IniUtil::ReadResString(_T("IDS_PRINT_ADDSECTION"));
	CButton* pBtnAddSection = (CButton*)GetDlgItem(IDC_CHECK_ADDCHAPTER);
	pBtnAddSection->SetWindowText(strAddSection);
	if (g_sorFileArray.printConfig.AddSection.CompareNoCase(_T("1")) == 0)
	{
		pBtnAddSection->SetCheck(TRUE);
	}
	else
	{
		pBtnAddSection->SetCheck(FALSE);
	}
	//�½ں�
	CString strSectionNum = IniUtil::ReadResString(_T("IDS_PRINT_SECTIONNUM"));
	GetDlgItem(IDC_STATIC_CHAPTERNUM)->SetWindowText(strSectionNum);
	int nSectionNum = _wtoi(g_sorFileArray.printConfig.SectionNum);
	m_spinSectionNum.SetSpin(0,1,1000,1);
	m_spinSectionNum.SetPos(nSectionNum);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CPrintHeadTail::OnApply()
{
	// TODO: �ڴ����ר�ô����/����û���

	return CPropertyPage::OnApply();
}
