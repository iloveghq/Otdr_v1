#pragma once
#include "afxwin.h"
#include "ReportFileSelect.h"

// CStep4FileSelect �Ի���

class CStep4FileSelect : public CPropertyPage
{
	DECLARE_DYNAMIC(CStep4FileSelect)

public:
	CStep4FileSelect();
	virtual ~CStep4FileSelect();
	void InitListFilePos();
	void InitListFileNeg();
	void PopulateFileList();
	BOOL IsFilePosSelect(); //�Ƿ��Ѿ�ѡ���ļ�
	void SetListViewText(CListCtrl& listCtrl);
	void SetListColName(CListCtrl& listCtrl, int nColIndex, const CString& colName);
	void SetLocalString();

// �Ի�������
	enum { IDD = IDD_REPORT_FILE_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStep4FileselectPos();
	afx_msg void OnBnClickedBtnStep4FileselectNeg();

public:
	CReportFileSelect m_fileSelectDlg;

public:
	CListCtrl m_listFilePos;
	CListCtrl m_listFileNeg;
};
