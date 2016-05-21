#pragma once
#include "afxwin.h"
#include "ColorStatic.h"
#include "DrawButton.h"

// CReportView �Ի���

class CReportView : public CDialog
{
	DECLARE_DYNAMIC(CReportView)

public:
	CReportView(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CReportView();
	virtual BOOL OnInitDialog();

// �Ի�������
	enum { IDD = IDD_REPORT_VIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CDrawButton m_DrawButton;
	CColorStatic m_ImgStatic;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
