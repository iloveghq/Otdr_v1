/*�����������뱨���һЩ���ò�����Ӧ������Ҫ���޸ı���ĸ�ʽ������*
*�޸����ڣ�2014.8 *
*�޸��ߣ�lzy *
*/

#pragma once
#include "PropConfig.h"
#include "PropContent.h"
#include "MyPropertySheet.h"

// CPropStep2A �Ի���

class CPropStep2A : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropStep2A)

public:
	CPropStep2A();
	virtual ~CPropStep2A();

// �Ի�������
	enum { IDD = IDD_PROP_STEP2A };

	CPropContent m_propContent;
	CPropConfig m_propConfig;
	CPropertySheet m_prop; 
	CRect m_rcLocation;    //λ��
	
	void AdjustWin();  //��������λ��

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
};
