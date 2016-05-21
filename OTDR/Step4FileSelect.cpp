// Step4FileSelect.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OTDR.h"
#include "Step4FileSelect.h"

extern SorFileArray g_sorFileArray;

// CStep4FileSelect �Ի���

IMPLEMENT_DYNAMIC(CStep4FileSelect, CPropertyPage)

CStep4FileSelect::CStep4FileSelect()
	: CPropertyPage(CStep4FileSelect::IDD)
{

}

CStep4FileSelect::~CStep4FileSelect()
{
}

void CStep4FileSelect::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STEP4, m_listFilePos);
	DDX_Control(pDX, IDC_LIST_STEP4_FLIP, m_listFileNeg);
}


BEGIN_MESSAGE_MAP(CStep4FileSelect, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_STEP4_FILESELECT_POS, &CStep4FileSelect::OnBnClickedBtnStep4FileselectPos)
	ON_BN_CLICKED(IDC_BTN_STEP4_FILESELECT_NEG, &CStep4FileSelect::OnBnClickedBtnStep4FileselectNeg)
END_MESSAGE_MAP()


// CStep4FileSelect ��Ϣ�������
BOOL CStep4FileSelect::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	//��ʼ���ļ��б�
	InitListFilePos();
	InitListFileNeg();
	//���ñ��ػ�
	SetLocalString();

	GetDlgItem(IDC_EDIT_STEP4_FOLDER_POS)->SetWindowText(g_sorFileArray.reportConfig.Folder1);
	GetDlgItem(IDC_EDIT_STEP4_FOLDER_NEG)->SetWindowText(g_sorFileArray.reportConfig.Folder2);
	if (g_sorFileArray.reportConfig.MeasureDir.CompareNoCase(_T("1")) == 1)//˫��
	{
		GetDlgItem(IDC_EDIT_STEP4_FOLDER_NEG)->EnableWindow(TRUE);
		GetDlgItem(IDC_LIST_STEP4_FLIP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_STEP4_FILESELECT_NEG)->EnableWindow(TRUE);
	}
	else//����
	{
		GetDlgItem(IDC_EDIT_STEP4_FOLDER_NEG)->EnableWindow(FALSE);
		GetDlgItem(IDC_LIST_STEP4_FLIP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_STEP4_FILESELECT_NEG)->EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pag	es should return FALSE
}

void CStep4FileSelect::SetLocalString()
{
	/*                      �ļ�ѡ��                            */
	//����
	CString strFileSelectPos = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILE_SELECT_POS"));
	GetDlgItem(IDC_STATIC_STEP4_FILESELECT_POS)->SetWindowText(strFileSelectPos);
	GetDlgItem(IDC_EDIT_STEP4_FOLDER_POS)->SetWindowText(g_sorFileArray.reportConfig.Folder1);
	//����
	CString strFileSelectNeg = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILE_SELECT_NEG"));
	GetDlgItem(IDC_STATIC_STEP4_FILESELECT_NEG)->SetWindowText(strFileSelectNeg);
	GetDlgItem(IDC_EDIT_STEP4_FOLDER_NEG)->SetWindowText(g_sorFileArray.reportConfig.Folder2);
	if (g_sorFileArray.reportConfig.MeasureDir.CompareNoCase(_T("1")) == 1)//˫��
	{
		GetDlgItem(IDC_EDIT_STEP4_FOLDER_NEG)->EnableWindow(TRUE);
		GetDlgItem(IDC_LIST_STEP4_FLIP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_STEP4_FILESELECT_NEG)->EnableWindow(TRUE);
	}
	else//����
	{
		GetDlgItem(IDC_EDIT_STEP4_FOLDER_NEG)->EnableWindow(FALSE);
		GetDlgItem(IDC_LIST_STEP4_FLIP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_STEP4_FILESELECT_NEG)->EnableWindow(FALSE);
	}
	//�ļ���
	CString strFolder = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILE_FOLDER"));
	GetDlgItem(IDC_STATIC_STEP4_FOLDER_POS)->SetWindowText(strFolder);
	GetDlgItem(IDC_STATIC_STEP4_FOLDER_NEG)->SetWindowText(strFolder);
	//�ļ�ѡ��
	CString strFileSelect = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILE_SELECT"));
	GetDlgItem(IDC_BTN_STEP4_FILESELECT_POS)->SetWindowText(strFileSelect);
	GetDlgItem(IDC_BTN_STEP4_FILESELECT_NEG)->SetWindowText(strFileSelect);

	//ListCtrl
	SetListViewText(m_listFileNeg);
	SetListViewText(m_listFilePos);
}

void CStep4FileSelect::SetListViewText(CListCtrl& listCtrl)
{
	//���
	CString strOrder = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILESELECT_ORDER"));
	SetListColName(listCtrl, 0, strOrder);//�����б���

	//�ļ���
	CString strFileName = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILESELECT_FILENAME")); 
	SetListColName(listCtrl,1,strFileName);//�����б���
}

void CStep4FileSelect::SetListColName(CListCtrl& listCtrl, int nColIndex, const CString& colName)
{
	LVCOLUMN col;
	col.mask= LVCF_TEXT;
	col.pszText= (LPWSTR)(LPCWSTR)colName;
	col.cchTextMax= 256;

	listCtrl.SetColumn(nColIndex, &col);
}

void CStep4FileSelect::InitListFilePos()
{
	//******��ʼ��m_listFilePos
	m_listFilePos.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CString strOrder=_T("���");
	CString strName=_T("�ļ���");
	m_listFilePos.InsertColumn(0,strOrder,LVCFMT_LEFT,100);
	m_listFilePos.InsertColumn(1,strName,LVCFMT_LEFT,250);
}

void CStep4FileSelect::InitListFileNeg()
{
	//******��ʼ��m_listFileNeg
	m_listFileNeg.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CString strOrder=_T("���");
	CString strName=_T("�ļ���");
	m_listFileNeg.InsertColumn(0,strOrder,LVCFMT_LEFT,100);
	m_listFileNeg.InsertColumn(1,strName,LVCFMT_LEFT,250);
}

//����ļ��б�
void CStep4FileSelect::PopulateFileList()
{
	m_listFilePos.DeleteAllItems();
	//g_sorFileArray.Reset();//��������ϴ�����������,zll 2012.9.22
	for (int i=0; i < m_fileSelectDlg.m_strSelectFiles.GetCount(); i++)
	{
		CString strFileIndex;
		strFileIndex.Format(_T("%d"), i+1);
		CString strFile;
		strFile = m_fileSelectDlg.m_strSelectFiles.GetAt(i);
		m_listFilePos.InsertItem(i, _T(""));
		m_listFilePos.SetItemText(i, 0, strFileIndex);
		m_listFilePos.SetItemText(i, 1, strFile);
	}
}

void CStep4FileSelect::OnBnClickedBtnStep4FileselectPos()
{
	//m_fileSelectDlg.m_strSelectFiles.RemoveAll();//�����ļ�ѡ���б�
	if(IDOK == m_fileSelectDlg.DoModal())
	{
		GetDlgItem(IDC_EDIT_STEP4_FOLDER_POS)->SetWindowText(g_sorFileArray.reportConfig.Folder1);
		PopulateFileList();
	}
}

void CStep4FileSelect::OnBnClickedBtnStep4FileselectNeg()
{
	m_fileSelectDlg.DoModal();
}

BOOL CStep4FileSelect::IsFilePosSelect() //�Ƿ��Ѿ�ѡ���ļ�
{
	if(m_listFilePos.GetItemCount() > 0)
		return TRUE;
	else
		return FALSE;
}