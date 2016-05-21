#pragma once


// CPropJob �Ի���

class CPropJob : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropJob)

public:
	CPropJob();
	virtual ~CPropJob();

// �Ի�������
	enum { IDD = IDD_PROP_STEP2_JOB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	// ������,EDIT�ؼ�����
	CString m_strJobID;
	// �а��̣�EDIT�ؼ�����
	CString m_strContractor;
	// �û���EDIT�ؼ�����
	CString m_strCustomer;
	// �����ߣ�EDIT�ؼ�����
	CString m_strOperator;
	// �ļ�����EDIT�ؼ�����
	CString m_strFile;
	// ����ʱ�䣬EDIT�ؼ�����
	CString m_strTestDate;
	// ע�ͣ�EDIT�ؼ�����
	CString m_strComment;
	afx_msg void OnBnClickedBtnJobClear();

private:
	CString m_strStInfo;  //Job info static text
	CString m_strStID;   //text of job index
	CString m_strStContractor;
	CString m_strStCustomer;
	CString m_strStOperator;
	CString m_strStFile;
	CString m_strStTime;
	CString m_strStComment;
	CString m_strBtnClear;

	void SetLocalString();

public:
	virtual BOOL OnInitDialog();
};
