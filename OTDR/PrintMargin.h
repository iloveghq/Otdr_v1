#pragma once
#include "afxcmn.h"
#include "NumSpinCtrl.h"

// CPrintMargin �Ի���

class CPrintMargin : public CPropertyPage
{
	DECLARE_DYNAMIC(CPrintMargin)

public:
	CPrintMargin();
	virtual ~CPrintMargin();

// �Ի�������
	enum { IDD = IDD_PAGE_PRINT_MARGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();

private:
	CNumSpinCtrl m_spinLeft;
	CNumSpinCtrl m_spinRight;
	CNumSpinCtrl m_spinTop;
	CNumSpinCtrl m_spinBottom;
};
