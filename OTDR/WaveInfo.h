#pragma once


// CWaveInfo �Ի���

class CWaveInfo : public CDialog
{
	DECLARE_DYNAMIC(CWaveInfo)

public:
	CWaveInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWaveInfo();

// �Ի�������
	enum { IDD = IDD_DIALOG_WAVEINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
