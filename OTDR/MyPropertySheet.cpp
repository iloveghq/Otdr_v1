// MyPropertySheet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OTDR.h"
#include "MyPropertySheet.h"
//#include "MainFrm.h"
//#include "OTDR.h"

// CMyPropertySheet
extern SorFileArray g_sorFileArray;

IMPLEMENT_DYNAMIC(CMyPropertySheet, CPropertySheet)

CMyPropertySheet::CMyPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	
}

CMyPropertySheet::CMyPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_propContent);
	AddPage(&m_propConfig);
	AddPage(&m_propJob);

	//m_pWnd = new CWnd;
	//m_pWnd = NULL;

	m_psh.dwFlags |= PSH_NOAPPLYNOW;   //����Ӧ�ð�ť
}

CMyPropertySheet::~CMyPropertySheet()
{
}


BEGIN_MESSAGE_MAP(CMyPropertySheet, CPropertySheet)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CMyPropertySheet ��Ϣ�������

HBRUSH CMyPropertySheet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertySheet::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	/*if(CTLCOLOR_DLG == nCtlColor)
	{
       return m_bkBrush;
	}*/
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

BOOL CMyPropertySheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	SetLocalString();

	//�޸�����ҳtab������
	TC_ITEM item;
	item.mask = TCIF_TEXT;
	item.pszText = m_strTabCotent.GetBuffer();//_T("��ӡ����");
	m_strTabCotent.ReleaseBuffer();
	GetTabControl()->SetItem(0, &item);
	item.pszText = m_strTabConfig.GetBuffer();//_T("����");
	m_strTabConfig.ReleaseBuffer();
	GetTabControl()->SetItem(1, &item);
	item.pszText = m_strTabJob.GetBuffer();//_T("����");
	m_strTabJob.ReleaseBuffer();
	GetTabControl()->SetItem(2, &item);

	GetDlgItem(IDOK)->SetWindowText(m_strBtnOK);
	GetDlgItem(IDCANCEL)->SetWindowText(m_strBtnCancel);

	//m_bkBrush = ::CreateSolidBrush(RGB(255, 0, 0));

	//��ʼ��������ҳ
	OnInitPages();

	return bResult;
}

//��ʼ��������ҳ
void CMyPropertySheet::OnInitPages(void)
{
	CString strSection, strKey, strValue;

	//��ʼ����ӡ��������ҳ
	strSection = _T("PrintSetting");
	strKey = _T("SelectLogo");
	strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	m_propContent.m_bCheckLogo = strValue == _T("0") ? 0 : 1;
	strKey = _T("SelectJob");
	strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	m_propContent.m_bCheckJob = strValue == _T("0") ? 0 : 1;
	strKey = _T("SelectConfig");
	strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	m_propContent.m_bCheckConfig = strValue == _T("0") ? 0 : 1;
	strKey = _T("SelectComment");
	strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	m_propContent.m_bCheckComment = strValue == _T("0") ? 0 : 1;
	strKey = _T("SelectTrace");
	strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	m_propContent.m_bCheckTrace = strValue == _T("0") ? 0 : 1;
	strKey = _T("SelectEvent");
	strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	m_propContent.m_bCheckEvent = strValue == _T("0") ? 0 : 1;
	m_propContent.UpdateData(FALSE);


	//��ʼ����������ҳ
	m_propConfig.OnReflash();
	//strKey = _T("OriginLocation");
	//m_propConfig.m_strStartLoc = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//strKey = _T("OriginCable");
	//m_propConfig.m_strStartCable = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//strKey = _T("OriginFiber");
	//m_propConfig.m_strStartFiber = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//strKey = _T("OriginColor");
	//m_propConfig.m_strStartColor = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//strKey = _T("EndLocation");
	//m_propConfig.m_strEndLoc = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//strKey = _T("EndCable");
	//m_propConfig.m_strEndCable = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//strKey = _T("EndFiber");
	//m_propConfig.m_strEndFiber = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//strKey = _T("EndColor");
	//m_propConfig.m_strEndColor = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//strKey = _T("SelfDefine");
	//strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//m_propConfig.m_bSelfDefine = strValue == _T("0") ? 0 : 1;
	//
	//if(!m_propConfig.m_bSelfDefine)  //����sor�ļ�������
	//{
	//	if(g_sorFileArray.totalCurveNumber > 0)
	//	{
	//		m_propConfig.m_strWavelength.Format(_T("%d"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->WaveLength);  //����
	//		m_propConfig.m_strPulse.Format(_T("%d"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->PulseWidth); //����
	//		m_propConfig.m_strRange.Format(_T("%d"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->Range); //����
	//		m_propConfig.m_strLossThreshold.Format(_T("%.3f"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->LossThreshold); //˥������
	//		m_propConfig.m_strRefThreshold.Format(_T("%.3f"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->ReflectanceThreshold); //��������
	//		m_propConfig.m_strEndThreshold.Format(_T("%.3f"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->EndOfFiberThreshold); //����ĩ������
	//		m_propConfig.m_strRBS.Format(_T("%.3f"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->BackscatterCoefficient); //���˱���ɢ��ϵ��
	//		m_propConfig.m_strIndex.Format(_T("%.6f"),g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->GroupIndex ); //������
	//		m_propConfig.m_strAverage.Format(_T("%.0f"),g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->Average ); //ƽ������

	//	}
	//}
	//else  //�Զ���
	//{	
	//	strKey = _T("Wavelength");
	//	m_propConfig.m_strWavelength = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//	strKey = _T("Pulse");
	//	m_propConfig.m_strPulse = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//	strKey = _T("LossThreshold");
	//	m_propConfig.m_strLossThreshold = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//	strKey = _T("ReflectionThreshold");
	//	m_propConfig.m_strRefThreshold = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//	strKey = _T("EndThreshold");
	//	m_propConfig.m_strEndThreshold = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//	strKey = _T("BackscatterCoeff");
	//	m_propConfig.m_strRBS = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//	strKey = _T("Index");
	//	m_propConfig.m_strIndex = IniUtil::ReadSingleConfigInfo(strSection, strKey);		
	//	strKey = _T("Average");
	//	m_propConfig.m_strAverage = IniUtil::ReadSingleConfigInfo(strSection, strKey);

	//}
	
	//��ʼ����������ҳ
	strKey = _T("Job");
	m_propJob.m_strJobID = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	strKey = _T("Contractor");
	m_propJob.m_strContractor = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	strKey = _T("Customer");
	m_propJob.m_strCustomer = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	strKey = _T("Operator");
	m_propJob.m_strOperator = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	if(g_sorFileArray.totalCurveNumber > 0)
	{
		//(CSorFileBase*)m_lpVoid
		m_propJob.m_strFile = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].fileName;//filePathName;
		CTime tm(g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->MeasureTime);
		m_propJob.m_strTestDate = tm.Format(_T("%Y-%m-%d %H:%M:%S"));
	}
	strKey = _T("Comment");
    m_propJob.m_strComment = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	
}



BOOL CMyPropertySheet::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	
	return CPropertySheet::PreTranslateMessage(pMsg);
}

//�������ݵ�config.ini�Լ���ӡҪ�漰�����ݽṹ��
void CMyPropertySheet::SaveSetting()
{
	CString strSection, strKey, strValue;
    strSection = _T("PrintSetting");

	//�����ӡ��������ҳ
	strKey = _T("SelectLogo");
	strValue.Format(_T("%d"), m_propContent.m_bCheckLogo);
	IniUtil::WriteSingleConfigInfo(strSection, strKey, strValue);
	strKey = _T("SelectJob");
	strValue.Format(_T("%d"), m_propContent.m_bCheckJob);
	IniUtil::WriteSingleConfigInfo(strSection, strKey, strValue);
	strKey = _T("SelectConfig");
	strValue.Format(_T("%d"), m_propContent.m_bCheckConfig);
	IniUtil::WriteSingleConfigInfo(strSection, strKey, strValue);
	strKey = _T("SelectComment");
	strValue.Format(_T("%d"), m_propContent.m_bCheckComment);
	IniUtil::WriteSingleConfigInfo(strSection, strKey, strValue);
	strKey = _T("SelectTrace");
	strValue.Format(_T("%d"), m_propContent.m_bCheckTrace);
	IniUtil::WriteSingleConfigInfo(strSection, strKey, strValue);
	strKey = _T("SelectEvent");
	strValue.Format(_T("%d"), m_propContent.m_bCheckEvent);
	IniUtil::WriteSingleConfigInfo(strSection, strKey, strValue);

	//������������ҳ
	m_propConfig.OnSave();
	

	//������������ҳ
	strKey = _T("Job");
	IniUtil::WriteSingleConfigInfo(strSection, strKey, m_propJob.m_strJobID);
	strKey = _T("Contractor");
	IniUtil::WriteSingleConfigInfo(strSection, strKey, m_propJob.m_strContractor);	
	strKey = _T("Customer");
	IniUtil::WriteSingleConfigInfo(strSection, strKey, m_propJob.m_strCustomer);	
	strKey = _T("Operator");
	IniUtil::WriteSingleConfigInfo(strSection, strKey, m_propJob.m_strOperator);	
	strKey = _T("Comment");
	IniUtil::WriteSingleConfigInfo(strSection, strKey, m_propJob.m_strComment);	

	
}

void CMyPropertySheet::SetLocalString()
{
	m_strTabCotent = IniUtil::ReadResString(_T("IDS_SETTING_TAB_CONTENT"));
	m_strTabConfig = IniUtil::ReadResString(_T("IDS_SETTING_TAB_CONFIG"));
	m_strTabJob = IniUtil::ReadResString(_T("IDS_SETTING_TAB_JOB"));
	m_strBtnOK = IniUtil::ReadResString(_T("IDS_SETTING_BTN_OK"));
	m_strBtnCancel = IniUtil::ReadResString(_T("IDS_SETTING_BTN_CANCEL"));
}
