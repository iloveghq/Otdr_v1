// ReportFileSelect.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OTDR.h"
#include "ReportFileSelect.h"
#include "FolderDialog.h"

extern SorFileArray g_sorFileArray;
// CReportFileSelect �Ի���

IMPLEMENT_DYNAMIC(CReportFileSelect, CDialog)

CReportFileSelect::CReportFileSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CReportFileSelect::IDD, pParent)
{

}

CReportFileSelect::~CReportFileSelect()
{
}

void CReportFileSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILE, m_listFile);
	DDX_Control(pDX, IDC_LIST_FILE_SELECT, m_listFileSelect);
	DDX_Control(pDX, IDC_COMBO_WAVE_LENGTH, m_commboxWaveLength);
}


BEGIN_MESSAGE_MAP(CReportFileSelect, CDialog)
	ON_BN_CLICKED(IDC_BTN_FILESELECT, &CReportFileSelect::OnBnClickedBtnFileselect)
	ON_BN_CLICKED(IDC_BTN_SETTING, &CReportFileSelect::OnBnClickedBtnSetting)
	ON_BN_CLICKED(IDC_BTN_CLEARALL, &CReportFileSelect::OnBnClickedBtnClearall)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CReportFileSelect::OnBnClickedBtnClear)
	ON_BN_CLICKED(IDOK, &CReportFileSelect::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_BATCH, &CReportFileSelect::OnBnClickedBtnBatch)
	ON_BN_CLICKED(IDC_CHECK_APPLY, &CReportFileSelect::OnBnClickedCheckApply)
//	ON_BN_CLICKED(IDCANCEL, &CReportFileSelect::OnBnClickedCancel)
//ON_BN_CLICKED(IDCANCEL, &CReportFileSelect::OnBnClickedCancel)
ON_BN_CLICKED(IDCANCEL, &CReportFileSelect::OnBnClickedCancel)
END_MESSAGE_MAP()


// CReportFileSelect ��Ϣ�������

void CReportFileSelect::OnBnClickedBtnFileselect()
{
	CFolderDialog dlg(m_strFolder);
	if (IDOK == dlg.DoModal())
	{
		m_strFolder = dlg.GetPathName();
		//g_sorFileArray.reportConfig.Folder1 = m_strFolder;//zll 2013.6.13ע�� ֻ��"ѡ���ļ���"�Ĺ����������豣��·����g_sorFileArray.reportConfig.Folder1
		GetDlgItem(IDC_EDIT_STEP4_FOLDER)->SetWindowText(m_strFolder);
		PopulateFileList(); //����ļ��б�
	}

}
//����ļ��б�
void CReportFileSelect::PopulateFileList()
{
	m_listFile.DeleteAllItems();
	CFileFind tempFind;
	BOOL bFound;
	CString strSorFile;
	strSorFile.Format(_T("%s\\%s"),m_strFolder,_T("*.sor"));
	bFound=tempFind.FindFile(strSorFile);
	CString strTmp;
	int nRowIndex = 1;//�ӵ�һ�п�ʼ
	while(bFound)
	{
		bFound=tempFind.FindNextFile();
		if(!tempFind.IsDots() || 
			!tempFind.IsDirectory())
		{
			strTmp=tempFind.GetFileName();
			if(strTmp.Right(4).CompareNoCase(_T(".sor")) == 0)
			{
				m_listFile.InsertItem(nRowIndex, strTmp);
			}
		}
		nRowIndex++;
	}
	//Ĭ��ѡ�е�һ��
	if (m_listFile.GetItemCount() > 0)
	{
		m_listFile.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED); //��Ϊѡ��״̬
		CString strFileNum;
		strFileNum.Format(_T("%d"), m_listFile.GetItemCount());
		GetDlgItem(IDC_EDIT_FILE_NUM)->SetWindowText(strFileNum);
	}
	tempFind.Close();
}

void CReportFileSelect::OnBnClickedBtnBatch()
{
	m_listFileSelect.DeleteAllItems();
	//��ȡѡ���ļ��б�
	if (!GetBatchFileSelect())
		return;
	//m_listFileSelect.SetItemText(0, 1, m_strSelectFile);
	int nSelectFileNum = m_listFile.GetSelectedCount();//zll 2012.9.18
	CString str_nSelectFileNum;
	str_nSelectFileNum.Format(_T("%d"),nSelectFileNum);
	GetDlgItem(IDC_EDIT_FILE_SELECT_NUM)->SetWindowText(str_nSelectFileNum);
	//Enable Window
	GetDlgItem(IDC_BTN_CLEARALL)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_CLEAR)->EnableWindow(TRUE);
	//Disable Window
	GetDlgItem(IDC_BTN_BATCH)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_SETTING)->EnableWindow(FALSE);
}

bool CReportFileSelect::GetBatchFileSelect() //��ȡ����ѡ���ļ�
{
	m_strSelectFiles.RemoveAll();//�����ļ�ѡ���б�
	POSITION pos = m_listFile.GetFirstSelectedItemPosition();
	int nIndex = 1;
	int nFileCount = 0;
	CString strIndex;
	//while (nIndex != LB_ERR && nFileCount < 8)
	while (nIndex != LB_ERR)//zll 2012.9.21 ȥ�� && nFileCount < 8 ��ӡʱ�������ƴ�ӡ���ߵ�����
	{
		nIndex = m_listFile.GetNextSelectedItem(pos);
		CString strSelectFile;
		strSelectFile = m_listFile.GetItemText(nIndex, 0);
		if (strSelectFile.IsEmpty())
			continue;
		strIndex.Format(_T("%d"), nFileCount + 1);
		m_strSelectFiles.Add(strSelectFile);
		m_listFileSelect.InsertItem(nFileCount, _T(""));
		m_listFileSelect.SetItemText(nFileCount, 0, strIndex);
		m_listFileSelect.SetItemText(nFileCount, 1, strSelectFile);
		nFileCount++;
	}

	return true;
}

void CReportFileSelect::OnBnClickedBtnSetting()
{
	m_strSelectFiles.RemoveAll();//�����ļ�ѡ���б�
	int nCurveNum = _wtoi(g_sorFileArray.reportConfig.NumberOfFiber);
	int nSelectFileNum = m_listFileSelect.GetItemCount();
	CString strSelectFileNum;
	strSelectFileNum.Format(_T("%d"), nSelectFileNum + 1);
	if ( nSelectFileNum >= nCurveNum)
	{
		//Disable Window
		GetDlgItem(IDC_BTN_BATCH)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SETTING)->EnableWindow(FALSE);
		return;
	}
	POSITION pos = m_listFile.GetFirstSelectedItemPosition();
	int nIndex = m_listFile.GetNextSelectedItem(pos);
	CString strSelectFile;
	strSelectFile = m_listFile.GetItemText(nIndex, 0);
	if (IsFileAlreadySelect(strSelectFile)) //�ļ��Ѿ���ѡ��
		return;
	
	if (strSelectFile.IsEmpty())
		return;

	m_strSelectFiles.Add(strSelectFile);
	m_listFileSelect.InsertItem(nSelectFileNum, _T(""));
	m_listFileSelect.SetItemText(nSelectFileNum, 0, strSelectFileNum);
	m_listFileSelect.SetItemText(nSelectFileNum, 1, strSelectFile);
	GetDlgItem(IDC_EDIT_FILE_SELECT_NUM)->SetWindowText(strSelectFileNum);
	//Enable Window
	GetDlgItem(IDC_BTN_CLEARALL)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_CLEAR)->EnableWindow(TRUE);
}

void CReportFileSelect::OnBnClickedBtnClearall()
{
	m_listFileSelect.DeleteAllItems();
	m_strSelectFiles.RemoveAll();
	GetDlgItem(IDC_EDIT_FILE_SELECT_NUM)->SetWindowText(_T("0"));//zll 2012.9.23
	//Disable Window
	GetDlgItem(IDC_BTN_CLEARALL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_CLEAR)->EnableWindow(FALSE);
	//Enable Window
	GetDlgItem(IDC_BTN_BATCH)->EnableWindow();
	GetDlgItem(IDC_BTN_SETTING)->EnableWindow(TRUE);
}

void CReportFileSelect::OnBnClickedBtnClear()
{
	POSITION pos = m_listFileSelect.GetFirstSelectedItemPosition();
	int nIndex = m_listFileSelect.GetNextSelectedItem(pos);
	if (nIndex == LB_ERR)
		return;

	m_strSelectFiles.RemoveAt(nIndex);
	m_listFileSelect.DeleteItem(nIndex);
	int int_rows_FileSelect=m_listFileSelect.GetItemCount();
	CString str_rows_FileSelect;
	str_rows_FileSelect.Format(_T("%d"),int_rows_FileSelect);
	GetDlgItem(IDC_EDIT_FILE_SELECT_NUM)->SetWindowText(str_rows_FileSelect);//zll 2012.9.23
    for (int i=0;i<int_rows_FileSelect;i++)
    {
		CString str_i;
		str_i.Format(_T("%d"),i+1);
		m_listFileSelect.SetItemText(i,0,str_i);
    }
	if (m_listFileSelect.GetItemCount() == 0)
	{
		//Disable Window
		GetDlgItem(IDC_BTN_CLEARALL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CLEAR)->EnableWindow(FALSE);
		//Enable Window
		GetDlgItem(IDC_BTN_BATCH)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_SETTING)->EnableWindow(TRUE);
	}
}

BOOL CReportFileSelect::OnInitDialog()
{
	CDialog::OnInitDialog();
	//��ʼ���ļ��б�
	InitListFile();
	InitListFileSelect();
	//����ļ��б�
	m_strFolder = g_sorFileArray.reportConfig.Folder1;
	PopulateFileList();
	/*                      ѡ���ļ�                            */
	//����
	CString strFileSelect = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILESELECT_DLG_TITLE"));
	SetWindowText(strFileSelect);
	//ȷ��
	CString strFileOk = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_OK"));
	GetDlgItem(IDOK)->SetWindowText(strFileOk);
	//ȡ��
	CString strFileCancel = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_CANCEL"));
	GetDlgItem(IDCANCEL)->SetWindowText(strFileCancel);
	//�ļ�����
	CString strFolderName = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILESELECT_DLG_FOLDER"));
	GetDlgItem(IDC_STATIC_FOLDER)->SetWindowText(strFolderName);
	GetDlgItem(IDC_EDIT_STEP4_FOLDER)->SetWindowText(g_sorFileArray.reportConfig.Folder1);
	//ѡ���ļ���
	CString strFolderSelect = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILESELECT_DLG_SELECTFOLDER"));
	GetDlgItem(IDC_BTN_FILESELECT)->SetWindowText(strFolderSelect);
	//�ļ�����
	CString strFileType = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILESELECT_DLG_FILETYPE"));
	GetDlgItem(IDC_STATIC_FILETYPE)->SetWindowText(strFileType);
	GetDlgItem(IDC_EDIT_FILE_TYPE)->SetWindowText(_T("SOR(*.SOR)"));
	//����1
	CString strWave1 = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILESELECT_DLG_WAVE1"));
	GetDlgItem(IDC_STATIC_WAVE1)->SetWindowText(strWave1);
	//������
	CString strBatch = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILESELECT_DLG_BATCH"));
	GetDlgItem(IDC_BTN_BATCH)->SetWindowText(strBatch);
	//����
	CString strSetting = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILESELECT_DLG_SETTING"));
	GetDlgItem(IDC_BTN_SETTING)->SetWindowText(strSetting);
	//�������
	CString strCleraAll = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILESELECT_DLG_CLEARALL"));
	GetDlgItem(IDC_BTN_CLEARALL)->SetWindowText(strCleraAll);
	GetDlgItem(IDC_BTN_CLEARALL)->EnableWindow(FALSE);
	//���
	CString strClear = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILESELECT_DLG_CLEAR"));
	GetDlgItem(IDC_BTN_CLEAR)->SetWindowText(strClear);
	GetDlgItem(IDC_BTN_CLEAR)->EnableWindow(FALSE);
	//�ļ�����
	CString strFileNum = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILESELECT_DLG_FILENUM"));
	GetDlgItem(IDC_STATIC_FILENUM)->SetWindowText(strFileNum);
	GetDlgItem(IDC_STATIC_FILE_NUM_SELECT)->SetWindowText(strFileNum);
	/*                         ѡ��                                                         */
	CString strFileOption = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILESELECT_DLG_OPTION"));
	GetDlgItem(IDC_STATIC_SELECT)->SetWindowText(strFileOption);
	//�ļ�����
	CString strFileClass = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILESELECT_DLG_FILECLASS"));
	GetDlgItem(IDC_STATIC_FILE_TYPE)->SetWindowText(strFileClass);
	//����
	CString strAsc = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILESELECT_DLG_ASC"));
	CButton* pBtnAsc = (CButton*)GetDlgItem(IDC_RDO_ASC);
	pBtnAsc->SetWindowText(strAsc);
	//����
	CString strDesc = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILESELECT_DLG_DESC"));
	CButton* pBtnDesc = (CButton*)GetDlgItem(IDC_RDO_DES);
	pBtnDesc->SetWindowText(strDesc);
	if (g_sorFileArray.reportConfig.SetOptionSort.CompareNoCase(_T("1")))
	{
		pBtnDesc->SetCheck(TRUE);
		pBtnAsc->SetCheck(FALSE);
	}
	else
	{
		pBtnDesc->SetCheck(FALSE);
		pBtnAsc->SetCheck(TRUE);
	}
	//�ļ�������
	CString strFilter = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILESELECT_DLG_FILTER"));
	GetDlgItem(IDC_STATIC_FILE_FILTER)->SetWindowText(strFilter);
	//Ӧ��
	CString strApply = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILESELECT_DLG_APPLY"));
	GetDlgItem(IDC_CHECK_APPLY)->SetWindowText(strApply);
	//����
	CString strWave = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILESELECT_DLG_WAVELENGTH"));
	GetDlgItem(IDC_STATIC_WAVELENGTH)->SetWindowText(strWave);
	m_commboxWaveLength.InsertString(-1, _T("850nm"));
	m_commboxWaveLength.InsertString(-1, _T("1300nm"));
	m_commboxWaveLength.InsertString(-1, _T("1310nm"));
	m_commboxWaveLength.InsertString(-1, _T("1550nm"));
	m_commboxWaveLength.InsertString(-1, _T("1650nm"));
	int nWaveLength = _wtoi(g_sorFileArray.reportConfig.WLIndex);
	m_commboxWaveLength.SetCurSel(nWaveLength);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CReportFileSelect::InitListFile()
{
	//******��ʼ��m_listFilePos
	m_listFile.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CString strName= IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILESELECT_FILENAME")); 
	m_listFile.InsertColumn(0,strName,LVCFMT_LEFT,200);
}

void CReportFileSelect::InitListFileSelect()
{
	//******��ʼ��m_listFileNeg
	m_listFileSelect.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CString strOrder= IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILESELECT_ORDER"));
	CString strName= IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILESELECT_FILENAME")); 
	m_listFileSelect.InsertColumn(0,strOrder,LVCFMT_LEFT,50);
	m_listFileSelect.InsertColumn(1,strName,LVCFMT_LEFT,250);
}

void CReportFileSelect::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_sorFileArray.reportConfig.Folder1 = m_strFolder;//zll 2013.6.13 ��ȷ��ѡ��ǰ�ļ��вű��浽g_sorFileArray.reportConfig.Folder1
	g_sorFileArray.Reset();//��������ϴ�����������zll 2012.9.22
	OnOK();
}

void CReportFileSelect::OnBnClickedCheckApply()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_APPLY);
	if (pBtn->GetCheck())
	{
		GetDlgItem(IDC_COMBO_WAVE_LENGTH)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_COMBO_WAVE_LENGTH)->EnableWindow(FALSE);
	}
}

BOOL CReportFileSelect::IsFileAlreadySelect(const CString& strFile)//�ж��ļ��Ƿ��Ѿ�ѡ��
{
	for(int i=0; i < m_strSelectFiles.GetCount(); i++)
	{
		CString strTmp = m_strSelectFiles.ElementAt(i);
		if(0 == strTmp.CompareNoCase(strFile))
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CReportFileSelect::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int b=m_strSelectFiles.GetCount();
	int a=m_listFileSelect.GetItemCount();
	if (0!=a)//zll 2012.10.8
	{
		if (0!=g_sorFileArray.totalCurveNumber)
		{
			MAIN->m_pReportFormView->m_ReportWizardDlg.m_pPropSheet->m_PropStep4.m_pageFileSelect.m_fileSelectDlg.m_strSelectFiles.RemoveAll();//zll 2012.9.28 ��ֹ�����ú���ȡ������m_strSelectFiles�Լ�������
		}
	}
	OnCancel();
}
