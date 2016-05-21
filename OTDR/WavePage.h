#pragma once
#include "resource.h"
#include "afxwin.h"
#include "DrawStaticWave.h"

// CWavePage �Ի���

class CWavePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CWavePage)

public:
	CWavePage();   // ��׼���캯��
	virtual ~CWavePage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_WAVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioAvgprocShow();
	afx_msg void OnBnClickedRadioAvgprocHide();
	afx_msg void OnBnClickedRadioCursorCrossline();
	afx_msg void OnBnClickedRadioCursorLine();
	afx_msg void OnBnClickedRadioGirdShow();
	afx_msg void OnBnClickedRadioGridHide();
	afx_msg void OnBnClickedRadioDoubleCursorShow();
	afx_msg void OnBnClickedRadioDoubleCursorHide();

private:
	bool m_bAvgProc; //�Ƿ���ƽ����������
	CRect m_drawRect; //���������С
public:
	CDrawStaticWave m_drawStatic;
};
