#pragma once
#include "afxcmn.h"


// CGrandwayProgress �Ի���

class CGrandwayProgress : public CDialog
{
	DECLARE_DYNAMIC(CGrandwayProgress)

public:
	CGrandwayProgress(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGrandwayProgress();
	void SetPos(int nPos);

// �Ի�������
	enum { IDD = IDD_DIALOG_PROGRRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CProgressCtrl m_progress;
};
