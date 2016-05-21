#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include"Step4FileSelect.h"
#include "NumSpinCtrl.h"

// CPropStep4 �Ի���

class CPropStep4 : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropStep4)

public:
	CPropStep4();
	virtual ~CPropStep4();
	void SetLocalString();

	// �Ի�������
	enum { IDD = IDD_PROP_STEP4 };

public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

private:
	void InitTab();
	void RecordSetting(); //����������Ϣ

	DECLARE_MESSAGE_MAP()
	afx_msg void OnDeltaposSpinFiberNum(NMHDR *pNMHDR, LRESULT *pResult);

public:
	CNumSpinCtrl m_spinFiberNum; //о��
	CNumSpinCtrl m_spinStartFiber; // ��ʼо��
	CEdit m_editFiberNum;
	CEdit m_editStartFiber;
	CTabCtrl m_tab;
	CStep4FileSelect m_pageFileSelect;
};
