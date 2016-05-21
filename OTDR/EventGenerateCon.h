#pragma once
#include "afxcmn.h"
#include "NumSpinCtrl.h"

// CEventGenerateCon �Ի���

class CEventGenerateCon :
	public CPropertyPage
{
	DECLARE_DYNAMIC(CEventGenerateCon)

public:
	CEventGenerateCon();
	virtual ~CEventGenerateCon();

// �Ի�������
	enum { IDD = IDD_PAGE_GENERATE_CON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:

	CNumSpinCtrl m_spinPermRange;
	CNumSpinCtrl m_spinRelaDegree;
	CNumSpinCtrl m_spinBothPermRange;
	virtual BOOL OnApply();
};
