#pragma once
#include "afxwin.h"


// CItemSettingDlg �Ի���

class CItemSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CItemSettingDlg)

public:
	CItemSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CItemSettingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_EDIT_ITEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSetting();
	afx_msg void OnBnClickedBtnClearall();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnLbnSelchangeListSelect();
	afx_msg void OnLbnDblclkListSelect();

private:
	CListBox m_list;
	CListBox m_listSelect;
};
