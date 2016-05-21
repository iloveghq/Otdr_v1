#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "NumSpinCtrl.h"

// CPageTestSetup �Ի���
class CClientSocket;
class CPageTestSetup : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageTestSetup)

public:
	CPageTestSetup();   // ��׼���캯��
	virtual ~CPageTestSetup();
	virtual BOOL OnInitDialog();
	void SendConfigData();
	void GetConfigData();
	void SetLocaleString();

// �Ի�������
	enum { IDD = IDD_PAGE_TEST_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnApply();

	DECLARE_MESSAGE_MAP()

private:
	void RecordSetting();
	void FillSendMsg(); //��䷢����Ϣ�ı���������
	void FillGetMsg(); //���������Ϣ�ı�

public:
	afx_msg void OnBnClickedBtnOtdr2pc();
	afx_msg void OnBnClickedBtnPc2otdr();

public:
	CEdit m_editTestTime;
	CNumSpinCtrl m_spinTestTime;
	CString m_strSendMsg;
	CString m_strGetMsg;
	CString m_strConfig;
};
