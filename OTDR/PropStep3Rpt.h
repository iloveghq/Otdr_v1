#pragma once

// CPropStep3Rpt �Ի���

class CPropStep3Rpt : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropStep3Rpt)

public:
	CPropStep3Rpt();
	virtual ~CPropStep3Rpt();
	void SetLocalString();
	void RecordSetting();

// �Ի�������
	enum { IDD = IDD_PROP_STEP3_RPT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
