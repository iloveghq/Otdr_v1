#pragma once

extern SorFileArray g_sorFileArray;
// CPropConfig �Ի���

class CPropConfig : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropConfig)

public:
	CPropConfig();
	virtual ~CPropConfig();

// �Ի�������
	enum { IDD = IDD_PROP_STEP2_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	// ���λ�ã�EDIT�ؼ�����
	CString m_strStartLoc;
	// �����º�
	CString m_strStartCable;
	// �����˺�
	CString m_strStartFiber;
	// ��������ɫ
	CString m_strStartColor;
	// �յ�λ�ã�EDIT�ؼ�����
	CString m_strEndLoc;
	// �յ���º�
	CString m_strEndCable;
	// �յ���˺�
	CString m_strEndFiber;
	// �յ������ɫ
	CString m_strEndColor;
	// ����
	CString m_strWavelength;
	// ������
	CString m_strIndex;
	// ����
	CString m_strPulse;
	// ������
	CString m_strResolution;
	// ����ɢ������
	CString m_strRBS;
	// ��Χ
	CString m_strRange;
	// ˥������
	CString m_strLossThreshold;
	// ��������
	CString m_strRefThreshold;
	// ����ĩ������
	CString m_strEndThreshold;
	//ƽ������
	CString m_strAverage;

	afx_msg void OnClear();
	afx_msg void OnCheckManual();  //�Զ�������

	void SetLocalString();
	CString m_strStLink;
	CString m_strStOrigin;
	CString m_strStEnd;
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
	CString m_strStEndTh;
	CString m_strStBack;
	CString m_strStIndex;
	CString m_strStAverage;
	// �Զ���
	BOOL m_bSelfDefine;
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();

	void OnReflash();
	void OnSave();
};
