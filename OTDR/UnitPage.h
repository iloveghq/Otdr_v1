#pragma once

#include "resource.h"
#include "Eventlist.h"
// CUnitPage �Ի���

class CUnitPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CUnitPage)

public:
	CUnitPage();
	virtual ~CUnitPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_UNIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioUnitKm();
	afx_msg void OnBnClickedRadioUnitMile();
	afx_msg void OnBnClickedRadioUnitKf();
public:
	CWaveform *m_pWaveFormView;//������ͼ
	CEventlist *m_pEventFormView;//�¼��б���ͼ
};
