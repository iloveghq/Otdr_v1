#pragma once


// CPropContent �Ի���

class CPropContent : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropContent)

public:
	CPropContent();
	virtual ~CPropContent();

// �Ի�������
	enum { IDD = IDD_PROP_STEP2_CONTENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	// �Ƿ��ӡgrandway logo
	BOOL m_bCheckLogo;
	// �Ƿ��ӡ������Ϣ
	BOOL m_bCheckJob;
	// �Ƿ��ӡ������Ϣ
	BOOL m_bCheckConfig;
	// �Ƿ��ӡע����Ϣ
	BOOL m_bCheckComment;
	// �Ƿ��ӡ����
	BOOL m_bCheckTrace;
	// �Ƿ��ӡ�¼�
	BOOL m_bCheckEvent;

	void SetLocalString();

private:
	CString m_strSelect;
	CString m_strLogo;
	CString m_strJob;
	CString m_strCfg;
	CString m_strCmt;
	CString m_strTrace;
	CString m_strEvent;
public:
	virtual BOOL OnInitDialog();
};
