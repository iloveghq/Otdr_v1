#pragma once


// CPropStep5 �Ի���

class CPropStep5 : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropStep5)

public:
	CPropStep5();
	virtual ~CPropStep5();

// �Ի�������
	enum { IDD = IDD_PROP_STEP5 };

public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
