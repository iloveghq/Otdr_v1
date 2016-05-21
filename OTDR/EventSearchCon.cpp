// EventSearchCon.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OTDR.h"
#include "EventSearchCon.h"

extern SorFileArray g_sorFileArray;

// CEventSearchCon �Ի���

IMPLEMENT_DYNAMIC(CEventSearchCon, CPropertyPage)

CEventSearchCon::CEventSearchCon()
	: CPropertyPage(CEventSearchCon::IDD)
{

}

CEventSearchCon::~CEventSearchCon()
{
}

void CEventSearchCon::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_BEND_TH, m_spinBendTH);
	DDX_Control(pDX, IDC_SPIN_REFLECT_TH, m_spinReflectTH);
	DDX_Control(pDX, IDC_SPIN_TERMAIL, m_spinTermail);
	DDX_Control(pDX, IDC_SPIN_SCATTER, m_spinBackScatter);
	DDX_Control(pDX, IDC_SPIN_SCATTER1550, m_spinBackScatter1550);
	DDX_Control(pDX, IDC_SPIN_SCATTER1625, m_spinBackScatter1625);
}


BEGIN_MESSAGE_MAP(CEventSearchCon, CPropertyPage)
ON_BN_CLICKED(IDC_CHECK_BEND, &CEventSearchCon::OnBnClickedCheckBend)
ON_BN_CLICKED(IDC_CHECK_RETRUN_LOSS, &CEventSearchCon::OnBnClickedCheckRetrunLoss)
ON_BN_CLICKED(IDC_CHECK_TERMAIL, &CEventSearchCon::OnBnClickedCheckTermail)
END_MESSAGE_MAP()


// CEventSearchCon ��Ϣ�������

BOOL CEventSearchCon::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	SetLocaleString();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CEventSearchCon::SetLocaleString()
{
	//��ֵ
	CString strThreshold = IniUtil::ReadResString(_T("IDS_EVENT_CON_THRESHOLD"));
	GetDlgItem(IDC_STATIC_TH)->SetWindowText(strThreshold);

	//˥���ж����� ����
	CString strBend = IniUtil::ReadResString(_T("IDS_EVENT_CON_BEND_TH"));
	GetDlgItem(IDC_STATIC_BEND)->SetWindowText(strBend);
	m_spinBendTH.SetSpin(2,0.01,9.99,0.01);
	m_spinBendTH.SetPos(g_sorFileArray.eventAnalConfig.BendTH);
	pBtnBend = (CButton*)GetDlgItem(IDC_CHECK_BEND);
	pEditBend = (CEdit*)GetDlgItem(IDC_EDIT_BEND_TH);
	pSpinBend = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_BEND_TH);
	if (0.0f==g_sorFileArray.eventAnalConfig.BendTH)
	{
		pBtnBend->SetCheck(TRUE);
		pEditBend->EnableWindow(FALSE);
		pSpinBend->EnableWindow(FALSE);
	}
	else
	{
		pBtnBend->SetCheck(FALSE);
		pEditBend->EnableWindow(TRUE);
		pSpinBend->EnableWindow(TRUE);
	}

	//��������
	CString strReflection = IniUtil::ReadResString(_T("IDS_EVENT_CON_REFLECT_TH"));
	GetDlgItem(IDC_STATIC_RETRUN_LOSS)->SetWindowText(strReflection);
	m_spinReflectTH.SetSpin(2,0.01,32.00,0.01);
	m_spinReflectTH.SetPos(g_sorFileArray.eventAnalConfig.ReflectTH);
	pBtnReturnLoss = (CButton*)GetDlgItem(IDC_CHECK_RETRUN_LOSS);
	pEditReturnLoss = (CEdit*)GetDlgItem(IDC_EDIT_RETURNLOSS);
	pSpinReturnLoss = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_REFLECT_TH);
	if (0.0f==g_sorFileArray.eventAnalConfig.ReflectTH)
	{
		pBtnReturnLoss->SetCheck(TRUE);
		pEditReturnLoss->EnableWindow(FALSE);
		pSpinReturnLoss->EnableWindow(FALSE);
	}
	else
	{
		pBtnReturnLoss->SetCheck(FALSE);
		pEditReturnLoss->EnableWindow(TRUE);
		pSpinReturnLoss->EnableWindow(TRUE);
	}

	//����ĩ��
	CString strTermial = IniUtil::ReadResString(_T("IDS_EVENT_CON_OPTICAL_TERMIAL"));
	GetDlgItem(IDC_STATIC_TERMAIL)->SetWindowText(strTermial);
	m_spinTermail.SetSpin(0,3,15,1);
	m_spinTermail.SetPos(g_sorFileArray.eventAnalConfig.Termial);
	pBtnTerminal = (CButton*)GetDlgItem(IDC_CHECK_TERMAIL);
	pEditTerminal = (CEdit*)GetDlgItem(IDC_EDIT_TERMAIL);
	pSpinTerminal = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_TERMAIL);
	if (0.0f==g_sorFileArray.eventAnalConfig.Termial)
	{
		pBtnTerminal->SetCheck(TRUE);
		pEditTerminal->EnableWindow(FALSE);
		pSpinTerminal->EnableWindow(FALSE);
	}
	else
	{
		pBtnTerminal->SetCheck(FALSE);
		pEditTerminal->EnableWindow(TRUE);
		pSpinTerminal->EnableWindow(TRUE);
	}

	//ɢ������
	CString strBackScatter = IniUtil::ReadResString(_T("IDS_EVENT_CON_BACK_SCATTER"));
	GetDlgItem(IDC_STATIC_SCATTER)->SetWindowText(strBackScatter);

	//if(1310==g_sorFileArray.sorFileList->pTestResultInfo->WaveLength)
	{
		m_spinBackScatter.SetSpin(2,-81.50,-52.0,00.01);
		m_spinBackScatter.SetPos(g_sorFileArray.eventAnalConfig.BackScatter);
	}
	//else if(1550==g_sorFileArray.sorFileList->pTestResultInfo->WaveLength)
	{
		m_spinBackScatter1550.SetSpin(2,-79.00,-52.0,00.01);
		m_spinBackScatter1550.SetPos(g_sorFileArray.eventAnalConfig.BackScatter1550);
	}
	{//zll 2013.5.10 ��
		m_spinBackScatter1625.SetSpin(2,-79.00,-52.0,00.01);
		m_spinBackScatter1625.SetPos(g_sorFileArray.eventAnalConfig.BackScatter1625);
	}

	//ɢ��ϵ��
	/*CString strScatter=IniUtil::ReadResString(_T("IDS_EVENT_CON_SCATTER"));
	GetDlgItem(IDC_STATIC_COEFFICIENT)->SetWindowText(strScatter);*/
}
BOOL CEventSearchCon::OnApply()
{
	/*                                          �����¼���������                                        */
	//��������
	if (1==pBtnBend->GetCheck())
	{
		IniUtil::WriteSingleConfigInfo(_T("EventAnal"), _T("BendTH"), _T("0"));
		pBtnBend->SetCheck(TRUE);
	}
	else if (0==pBtnBend->GetCheck())
	{
		g_sorFileArray.eventAnalConfig.BendTH = m_spinBendTH.GetPos();
		CString strBendTH;
		strBendTH.Format(_T("%.2f"), g_sorFileArray.eventAnalConfig.BendTH);
		IniUtil::WriteSingleConfigInfo(_T("EventAnal"), _T("BendTH"), strBendTH);
	}
	
	
	//��������
	if (1==pBtnReturnLoss->GetCheck())
	{
		IniUtil::WriteSingleConfigInfo(_T("EventAnal"), _T("ReflectTH"),_T("0"));
		pBtnReturnLoss->SetCheck(TRUE);
	}
	else if (0==pBtnReturnLoss->GetCheck())
	{
		g_sorFileArray.eventAnalConfig.ReflectTH = m_spinReflectTH.GetPos();
		CString strReflectTH;
		strReflectTH.Format(_T("%.2f"), g_sorFileArray.eventAnalConfig.ReflectTH);
		IniUtil::WriteSingleConfigInfo(_T("EventAnal"), _T("ReflectTH"),strReflectTH);
	}
	
	
	//����ĩ��
	if (1==pBtnTerminal->GetCheck())
	{
		IniUtil::WriteSingleConfigInfo(_T("EventAnal"), _T("Termial"), _T("0"));
		pBtnTerminal->SetCheck(TRUE);
	}
	else if (0==pBtnTerminal->GetCheck())
	{
		g_sorFileArray.eventAnalConfig.Termial = m_spinTermail.GetPos();
		CString strTermial;
		strTermial.Format(_T("%.2f"), g_sorFileArray.eventAnalConfig.Termial);
		IniUtil::WriteSingleConfigInfo(_T("EventAnal"), _T("Termial"), strTermial);
	}
	

	//����ɢ������1310nm
	//if (1310==g_sorFileArray.sorFileList->pTestResultInfo->WaveLength)
	{
		g_sorFileArray.eventAnalConfig.BackScatter = m_spinBackScatter.GetPos();
		CString strBackScatter;
		strBackScatter.Format(_T("%.2f"), g_sorFileArray.eventAnalConfig.BackScatter);
		IniUtil::WriteSingleConfigInfo(_T("EventAnal"), _T("BackScatter"),strBackScatter);
	}
	//else if (1550==g_sorFileArray.sorFileList->pTestResultInfo->WaveLength)//����ɢ������1550nm
	{
		g_sorFileArray.eventAnalConfig.BackScatter1550 = m_spinBackScatter1550.GetPos();
		CString strBackScatter1550;
		strBackScatter1550.Format(_T("%.2f"), g_sorFileArray.eventAnalConfig.BackScatter1550);
		IniUtil::WriteSingleConfigInfo(_T("EventAnal"), _T("BackScatter1550"),strBackScatter1550);
	}
	//zll 2013.5.10 ��
	{
		g_sorFileArray.eventAnalConfig.BackScatter1625 = m_spinBackScatter1625.GetPos();
		CString strBackScatter1625;
		strBackScatter1625.Format(_T("%.2f"), g_sorFileArray.eventAnalConfig.BackScatter1625);
		IniUtil::WriteSingleConfigInfo(_T("EventAnal"), _T("BackScatter1625"),strBackScatter1625);
	}

	return CPropertyPage::OnApply();
}

void CEventSearchCon::OnBnClickedCheckBend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (0==pBtnBend->GetCheck())
	{
		pEditBend->EnableWindow(TRUE);
		pSpinBend->EnableWindow(TRUE);
	}
	else if (1==pBtnBend->GetCheck())
	{
		pEditBend->EnableWindow(FALSE);
		pSpinBend->EnableWindow(FALSE);
	}
}

void CEventSearchCon::OnBnClickedCheckRetrunLoss()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (0==pBtnReturnLoss->GetCheck())
	{
		pEditReturnLoss->EnableWindow(TRUE);
		pSpinReturnLoss->EnableWindow(TRUE);
	}
	else if (1==pBtnReturnLoss->GetCheck())
	{
		pEditReturnLoss->EnableWindow(FALSE);
		pSpinReturnLoss->EnableWindow(FALSE);
	}
}

void CEventSearchCon::OnBnClickedCheckTermail()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (0==pBtnTerminal->GetCheck())
	{
		pEditTerminal->EnableWindow(TRUE);
		pSpinTerminal->EnableWindow(TRUE);
	}
	else if (1==pBtnTerminal->GetCheck())
	{
		pEditTerminal->EnableWindow(FALSE);
		pSpinTerminal->EnableWindow(FALSE);
	}
}
