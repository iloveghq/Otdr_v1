#pragma once
#include "afxcmn.h"
#include "afxdtctl.h"

// CPageSystemSetting �Ի���
class CClientSocket;
class CPageSystemSetting : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageSystemSetting)

public:
	CPageSystemSetting();
	virtual ~CPageSystemSetting();
	virtual BOOL OnInitDialog();
	void SendConfigData();
	void GetConfigData();
	void SetLocaleString();

// �Ի�������
	enum { IDD = IDD_PAGE_SYSTEM_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnApply();

	DECLARE_MESSAGE_MAP()
	void Send();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar); 
	afx_msg void OnBnClickedBtnOtdr2pc();
	afx_msg void OnBnClickedBtnPc2otdr();

private:
	void RecordSetting();
	void FillSendMsg(); //��䷢����Ϣ�ı���������
	void FillGetMsg(); //���������Ϣ�ı�

public:
	CSliderCtrl m_slideTouchLight;

private:
	CString m_strSendMsg;
	CString m_strGetMsg;
	CString m_strConfig;
	CDateTimeCtrl m_date;
	CDateTimeCtrl m_time;
};
