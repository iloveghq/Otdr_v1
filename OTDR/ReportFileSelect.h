#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CReportFileSelect �Ի���

class CReportFileSelect : public CDialog
{
	DECLARE_DYNAMIC(CReportFileSelect)

public:
	CReportFileSelect(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CReportFileSelect();
	void InitListFile();
	void InitListFileSelect();
	void PopulateFileList();
	BOOL IsFileAlreadySelect(const CString& strFile);//�ж��ļ��Ƿ��Ѿ�ѡ��
	bool GetBatchFileSelect();

// �Ի�������
	enum { IDD = IDD_DIALOG_FILE_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBtnFileselect();
	afx_msg void OnBnClickedBtnBatch();
	afx_msg void OnBnClickedBtnSetting();
	afx_msg void OnBnClickedBtnClearall();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckApply();

public:
	CStringArray m_strSelectFiles;//ѡ����ļ�

private:
	CListCtrl m_listFile;
	CListCtrl m_listFileSelect;
	CString m_strFolder;//�ļ���·��
	CComboBox m_commboxWaveLength;
//	afx_msg void OnBnClickedCancel();
//	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCancel();
};
