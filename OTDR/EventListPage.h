#pragma once
#include "resource.h"
#include "afxwin.h"

// CEventListPage �Ի���

class CEventListPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CEventListPage)

public:
	CEventListPage();
	virtual ~CEventListPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_EVENTLIST };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();

	DECLARE_MESSAGE_MAP()

};
