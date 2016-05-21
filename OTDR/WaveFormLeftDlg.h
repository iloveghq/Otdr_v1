#pragma once
#include "afxcmn.h"
#include "Util.h"
#include "afxwin.h"

// CWaveFormLeftDlg �Ի���
class CMainFrame;

class CWaveFormLeftDlg : public CDialog
{
	DECLARE_DYNAMIC(CWaveFormLeftDlg)

public:
	CWaveFormLeftDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWaveFormLeftDlg();
	void SetLocalString();
	void GetColorBitmap(COLORREF color, CBitmap &bmp);
	void ResetImageList();

	// �Ի�������
	enum { IDD = IDD_WAVEFORM_LEFT };

public:
	void PopulateData();//�������
	void FillParams(); //������߲���
	void OnOK();
	void OnCancel();

private:
	void FillFileList();//����ļ��б��

	void SetControlText(int nControlID, LPCTSTR strVal);
	void SetListColName(int nColIndex, const CString& colName);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickCheckMultiDisplay();
	afx_msg void OnLvnItemchangedListFile(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickListFile(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnWaveInfo();
	afx_msg void OnSorFileClose();

	CListCtrl m_list;
	CImageList m_ImageList;
	CUtil m_util;
	CButton m_checkMultiMode;
	CMainFrame* m_pMainFrm;
	int m_nCurFileSelectIndex;

public:
	INT m_listItem;//zll 2013.5.7
	INT m_listCurrentItem;
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
