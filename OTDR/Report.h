#pragma once

#include "ReportWizard.h"
#include "ReportView.h"

// CReport ������ͼ

class CReport : public CFormView
{
	DECLARE_DYNCREATE(CReport)

protected:
	CReport();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CReport();
	void CreateWizardDlg();
	void CreateReportViewDlg();

public:
	enum { IDD = IDD_REPORT };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	void SetLocalString();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnInitialUpdate();

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
	CReportWizard m_ReportWizardDlg;
	CReportView m_ReportViewDlg;
};


