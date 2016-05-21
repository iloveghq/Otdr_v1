#pragma once


// CPropStep5New �Ի���

class CPropStep5New : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropStep5New)

public:
	CPropStep5New();
	virtual ~CPropStep5New();

// �Ի�������
	enum { IDD = IDD_PROP_STEP5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual LRESULT OnWizardBack();
	virtual LRESULT OnWizardNext();
	virtual BOOL OnSetActive();

	void SetLocalString();
	virtual BOOL OnInitDialog();

private:	
	CString m_strGroupJob;//����
	CString m_strGroupConfig;//����
	CString m_strGroupSelect;  //��ӡѡ��
	
	CString m_strStInfo;  
	CString m_strStID;
	CString m_strStContractor; 
	CString m_strStCustomer;
	CString m_strStOperator;
	CString m_strStFile;
	CString m_strStTime ;
	CString m_strStComment;
	//CString m_strBtnClear ;

	//����ע��
	CString m_strStLink;
	CString m_strStOrigin;
	CString m_strStEnd ;
	CString m_strStLocation;
	CString m_strStCable;
	CString m_strStFiber;
	CString m_strStColor;
	CString m_strStClear;
	CString m_strStMeasurement; 
	CString m_strStWave;
	CString m_strStPulse;
	CString m_strStRange;
	CString m_strStLossTh; 
	CString m_strStRefTh; 
	CString m_strStEndTh ;
	CString m_strStBack ;
	CString m_strStIndex; 
	CString m_strStAverage ;

	//ѡ���ӡ������
	CString m_strSelect; 
	CString m_strLogo;
	CString m_strJob;
	CString m_strCfg;
	CString m_strCmt;
	CString m_strTrace ;
	CString m_strEvent;
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

	CString m_strStartLoc;
	CString m_strStartCable;
	CString m_strStartFiber ;
	CString m_strStartColor ;
	CString m_strEndLoc ;
	CString m_strEndCable ;
	CString m_strEndFiber;
	CString m_strEndColor ;
	CString m_strWavelength;  //����
	CString m_strPulse; //����
	CString m_strRange; //����
	CString m_strLossThreshold; //˥������
	CString m_strRefThreshold; //��������
	CString m_strEndThreshold; //����ĩ������
	CString m_strRBS; //����ĩ������
	CString m_strIndex;
	CString m_strAverage;  //ƽ������

	CString m_strJobID ; //������
	CString m_strContractor ;//�а���
	CString m_strCustomer;//�ͻ�
	CString m_strOperator ;//����Ա
	CString m_strFile ;   //�ļ�
	CString m_strTestDate;//��������
    CString m_strComment;//ע��

public:
	afx_msg void OnBtnStep5Modify();
	afx_msg void OnPaint();
};
