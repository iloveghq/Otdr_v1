#pragma once
#include "afxcmn.h"
#include "NumSpinCtrl.h"

// CEventAnalCon �Ի���

class CEventAnalCon : public CPropertyPage
{
	DECLARE_DYNAMIC(CEventAnalCon)

public:
	CEventAnalCon();
	virtual ~CEventAnalCon();
	void SetLocaleString();

// �Ի�������
	enum { IDD = IDD_PAGE_ANAL_CON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnApply();
	CNumSpinCtrl m_spinReflect;
public:
	CNumSpinCtrl m_spinReflect1550;
	CNumSpinCtrl m_spinReflect1625;
};
