// PageSystemSetting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OTDR.h"
#include "PageSystemSetting.h"
#include "RemotePageSheet.h"
#include "ClientSocket.h"

extern SorFileArray g_sorFileArray;
CString str_net_connect=_T("");

// CPageSystemSetting �Ի���

IMPLEMENT_DYNAMIC(CPageSystemSetting, CPropertyPage)

CPageSystemSetting::CPageSystemSetting()
	: CPropertyPage(CPageSystemSetting::IDD)
{

}

CPageSystemSetting::~CPageSystemSetting()
{
}

void CPageSystemSetting::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_BRIGHT, m_slideTouchLight);
	DDX_Control(pDX, IDC_DP_DATE, m_date);
	DDX_Control(pDX, IDC_DP_TIME, m_time);
}


BEGIN_MESSAGE_MAP(CPageSystemSetting, CPropertyPage)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BTN_OTDR2PC, &CPageSystemSetting::OnBnClickedBtnOtdr2pc)
	ON_BN_CLICKED(IDC_BTN_PC2OTDR, &CPageSystemSetting::OnBnClickedBtnPc2otdr)
END_MESSAGE_MAP()

void CPageSystemSetting::SetLocaleString()
{
	/*                       ����                                           */
	CString strLang = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_SYSTEM_LANG"));
	GetDlgItem(IDC_STATIC_LANG_SETTING)->SetWindowText(strLang);

	//Ӣ��
	CString strEng = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_SYSTEM_ENG"));
	CButton *pBtnEng = (CButton*)GetDlgItem(IDC_RADIO_ENG);
	pBtnEng->SetWindowText(strEng);
	//����
	CString strChs = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_SYSTEM_CHS"));
	CButton *pBtnChs = (CButton*)GetDlgItem(IDC_RADIO_CHS);
	pBtnChs->SetWindowText(strChs);
	CString strLanguage = g_sorFileArray.otdrConfig.strLang;
	if (strLanguage.CompareNoCase(_T("0")) == 0)
	{
		pBtnChs->SendMessage(BM_CLICK, 0, 0);
	}
	else
	{
		pBtnEng->SendMessage(BM_CLICK, 0, 0);
	}
	
	/*                              �Զ��ػ�                                                  */
	CString strAutoPowerOff = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_SYSTEM_AUTOPOWEROFF"));
	CButton *pBtnAutoPowerOff = (CButton*)GetDlgItem(IDC_STATIC_AUTO_POWEROFF);
	pBtnAutoPowerOff->SetWindowText(strAutoPowerOff);
	//���ػ�
	CString strNever = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_SYSTEM_AUTOPOWEROFF_NEVER"));
	CButton *pBtnNever = (CButton*)GetDlgItem(IDC_RADIO_NEVER);
	pBtnNever->SetWindowText(strNever);
	CButton *pBtn1Min = (CButton*)GetDlgItem(IDC_RADIO_1MIN);
	CButton *pBtn3Min = (CButton*)GetDlgItem(IDC_RADIO_3MIN);
	CButton *pBtn5Min = (CButton*)GetDlgItem(IDC_RADIO_5MIN);
	CButton *pBtn10Min = (CButton*)GetDlgItem(IDC_RADIO_10MIN);
	CButton *pBtn20Min = (CButton*)GetDlgItem(IDC_RADIO_20MIN);
	CButton *pBtn30Min = (CButton*)GetDlgItem(IDC_RADIO_30MIN);
	CButton *pBtn60Min = (CButton*)GetDlgItem(IDC_RADIO_60MIN);
	CString strAutoPower = g_sorFileArray.otdrConfig.strAutoPowerOff;
	if (strAutoPower.CompareNoCase(_T("Never")) == 0)
	{
		pBtnNever->SendMessage(BM_CLICK, 0, 0);
	}
	else if (strAutoPower.CompareNoCase(_T("1")) == 0)
	{
		pBtn1Min->SendMessage(BM_CLICK, 0, 0);
	}
	else if (strAutoPower.CompareNoCase(_T("3")) == 0)
	{
		pBtn3Min->SendMessage(BM_CLICK, 0, 0);
	}
	else if (strAutoPower.CompareNoCase(_T("5")) == 0)
	{
		pBtn5Min->SendMessage(BM_CLICK, 0, 0);
	}
	else if (strAutoPower.CompareNoCase(_T("10")) == 0)
	{
		pBtn10Min->SendMessage(BM_CLICK, 0, 0);
	}
	else if (strAutoPower.CompareNoCase(_T("20")) == 0)
	{
		pBtn20Min->SendMessage(BM_CLICK, 0, 0);
	}
	else if (strAutoPower.CompareNoCase(_T("30")) == 0)
	{
		pBtn30Min->SendMessage(BM_CLICK, 0, 0);
	}
	else if (strAutoPower.CompareNoCase(_T("60")) == 0)
	{
		pBtn60Min->SendMessage(BM_CLICK, 0, 0);
	}
	//����
	CString strBrighten = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_SYSTEM_BRIGHTEN"));
	GetDlgItem(IDC_STATIC_BRIGHT)->SetWindowText(strBrighten);
	int nPos = _wtoi(g_sorFileArray.otdrConfig.strLightLevel);
	m_slideTouchLight.SetRange(15 ,100);
	m_slideTouchLight.SetPos(115 - nPos);
	//����
	CString strDate = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_SYSTEM_DATE"));
	GetDlgItem(IDC_STATIC_DATE)->SetWindowText(strDate);

	//ʱ��
	CString strTime = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_SYSTEM_TIME"));
	GetDlgItem(IDC_STATIC_TIME)->SetWindowText(strTime);
	//������
	CString strTouchScreen = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_SYSTEM_TOUCH"));
	GetDlgItem(IDC_STATIC_TOUCH)->SetWindowText(strTouchScreen);
	CString strTouchStartUp = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_SYSTEM_TOUCH_STARTUP"));
	CButton *pBtnTouchEnable = (CButton*)GetDlgItem(IDC_RADIO_TOUCH_ENABLE);
	pBtnTouchEnable->SetWindowText(strTouchStartUp);
	CString strTouchShutdown = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_SYSTEM_TOUCH_SHUTDOWN"));
	CButton *pBtnTouchDisnable = (CButton*)GetDlgItem(IDC_RADIO_TOUCH_DISABLE);
	pBtnTouchDisnable->SetWindowText(strTouchShutdown);
	CString strTouch = g_sorFileArray.otdrConfig.strTouchSreeenStatus;
	if (strTouch.CompareNoCase(_T("1")) == 0)
	{
		pBtnTouchDisnable->SendMessage(BM_CLICK, 0, 0);
	}
	else if (strTouch.CompareNoCase(_T("0")) == 0)
	{
		pBtnTouchEnable->SendMessage(BM_CLICK, 0, 0);
	}
	str_net_connect=IniUtil::ReadResString(_T("IDS_NET_CONNECT"));
}

void CPageSystemSetting::RecordSetting()
{
	/*                ����                                       */
	CButton *pBtnEng = (CButton*)GetDlgItem(IDC_RADIO_ENG);
	if (pBtnEng->GetCheck())
	{
		g_sorFileArray.otdrConfig.strLang = _T("1");
	}
	else
	{
		g_sorFileArray.otdrConfig.strLang = _T("0");
	}
	IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("Language"), g_sorFileArray.otdrConfig.strLang);
	/*                              �Զ��ػ�                                                  */
	CButton *pBtnNever = (CButton*)GetDlgItem(IDC_RADIO_NEVER);
	CButton *pBtn1Min = (CButton*)GetDlgItem(IDC_RADIO_1MIN);
	CButton *pBtn3Min = (CButton*)GetDlgItem(IDC_RADIO_3MIN);
	CButton *pBtn5Min = (CButton*)GetDlgItem(IDC_RADIO_5MIN);
	CButton *pBtn10Min = (CButton*)GetDlgItem(IDC_RADIO_10MIN);
	CButton *pBtn20Min = (CButton*)GetDlgItem(IDC_RADIO_20MIN);
	CButton *pBtn30Min = (CButton*)GetDlgItem(IDC_RADIO_30MIN);
	CButton *pBtn60Min = (CButton*)GetDlgItem(IDC_RADIO_60MIN);
	if (pBtnNever->GetCheck())
	{
		g_sorFileArray.otdrConfig.strAutoPowerOff = _T("Never");
	}
	else if (pBtn1Min->GetCheck())
	{
		g_sorFileArray.otdrConfig.strAutoPowerOff = _T("1");
	}
	else if (pBtn3Min->GetCheck())
	{
		g_sorFileArray.otdrConfig.strAutoPowerOff = _T("3");
	}
	else if (pBtn5Min->GetCheck())
	{
		g_sorFileArray.otdrConfig.strAutoPowerOff = _T("5");
	}
	else if (pBtn10Min->GetCheck())
	{
		g_sorFileArray.otdrConfig.strAutoPowerOff = _T("10");
	}
	else if (pBtn20Min->GetCheck())
	{
		g_sorFileArray.otdrConfig.strAutoPowerOff = _T("20");
	}
	else if (pBtn30Min->GetCheck())
	{
		g_sorFileArray.otdrConfig.strAutoPowerOff = _T("30");
	}
	else if (pBtn60Min->GetCheck())
	{
		g_sorFileArray.otdrConfig.strAutoPowerOff = _T("60");
	}
	IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("AutoPowerOff"), g_sorFileArray.otdrConfig.strAutoPowerOff);
	/*                                ����                                                 */
	IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("LightLevel"), g_sorFileArray.otdrConfig.strLightLevel);
	/*                                   ʱ��                                                              */
	SYSTEMTIME sysDate;
	m_date.GetTime(&sysDate);
	SYSTEMTIME sysTime;
	m_time.GetTime(&sysTime);
	sysDate.wHour = sysTime.wHour;
	sysDate.wMinute = sysTime.wMinute;
	sysDate.wSecond = sysTime.wSecond;
	SetLocalTime(&sysDate);
	CTime time(sysDate);
	g_sorFileArray.otdrConfig.strDateTime  = time.Format(_T("%Y-%m-%d %H:%M:%S"));//ת��ʱ���ʽ
	/*                                ������                                                                */
	CButton *pBtnTouchEnable = (CButton*)GetDlgItem(IDC_RADIO_TOUCH_ENABLE);
	CButton *pBtnTouchDisnable = (CButton*)GetDlgItem(IDC_RADIO_TOUCH_DISABLE);
	if (pBtnTouchEnable->GetCheck())
	{
		g_sorFileArray.otdrConfig.strTouchSreeenStatus = _T("0");
	}
	else if (pBtnTouchDisnable->GetCheck())
	{
		g_sorFileArray.otdrConfig.strTouchSreeenStatus = _T("1");
	}
}

void CPageSystemSetting::OnVScroll(UINT nSBCode,UINT nPos,CScrollBar *pScrollBar)
{
	CPropertyPage::OnVScroll(nSBCode,nPos,pScrollBar);
	//��λ��ת��Ϊ15-100֮��
	int nCurPos = 115 - m_slideTouchLight.GetPos();
	g_sorFileArray.otdrConfig.strLightLevel.Format(_T("%d"), nCurPos);
}

// CPageSystemSetting ��Ϣ�������

BOOL CPageSystemSetting::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	SetLocaleString();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CPageSystemSetting::OnBnClickedBtnOtdr2pc()
{
	FillGetMsg();
	GetConfigData();
}

void CPageSystemSetting::OnBnClickedBtnPc2otdr()
{
	RecordSetting(); //��������
	FillSendMsg();
	SendConfigData();
}

BOOL CPageSystemSetting::OnApply()
{
	// TODO: �ڴ����ר�ô����/����û���
	RecordSetting();
	return CPropertyPage::OnApply();
}

void CPageSystemSetting::Send()
{
	//UpdateData();
	//m_pClientSocket->SendLine(m_sSend);
}

void CPageSystemSetting::FillGetMsg()
{
	m_strGetMsg.Empty(); //����ַ���
	//��ȡ��������
	m_strGetMsg = NET_TYPE_SYSTEM_GET;
}

void CPageSystemSetting::FillSendMsg()//��䷢����Ϣ�ı�
{
	m_strSendMsg.Empty(); //����ַ���
	//����
	CString strCommand; //������������
	strCommand.Format(_T("1"));
	m_strSendMsg += strCommand;
	//ϵͳ������
	CString strLang; //����
	strLang.Format(_T("Language=%s,"), g_sorFileArray.otdrConfig.strLang);
	m_strSendMsg += strLang;

	CString strAutoPoweOff; //�Զ��ػ�
	strAutoPoweOff.Format(_T("AutoPowerOff=%s,"), g_sorFileArray.otdrConfig.strAutoPowerOff);
	m_strSendMsg += strAutoPoweOff;

	CString strBright; //����
	strBright.Format(_T("LightLevel=%s,"), g_sorFileArray.otdrConfig.strLightLevel);
	m_strSendMsg += strBright;

	//CString strTestDate; //����
	//strTestDate.Format(_T("Date=%s,"), g_sorFileArray.otdrConfig.strDate);
	//m_strSendMsg += strTestDate;

	CString strDateTime; //����ʱ��
	strDateTime.Format(_T("DateTime=%s,"), g_sorFileArray.otdrConfig.strDateTime);
	m_strSendMsg += strDateTime;

	CString strTouchStatus; //������
	strTouchStatus.Format(_T("TouchSreeenStatus=%s,"), g_sorFileArray.otdrConfig.strTouchSreeenStatus);
	m_strSendMsg += strTouchStatus;
}

void CPageSystemSetting::SendConfigData()
{
	CClientSocket* pClientSocket = MAIN->m_pRemoteConnDlg->m_pClientSocket;
	if (!pClientSocket)
		return;

	int nSocketStatus = pClientSocket->GetSocketState();
	if (nSocketStatus <= CCESocket::CREATED)
	{
		MessageBox(str_net_connect, _T("OTDR Viewer"), MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	pClientSocket->SendLine(m_strSendMsg);
}

void CPageSystemSetting::GetConfigData()
{
	CClientSocket* pClientSocket = MAIN->m_pRemoteConnDlg->m_pClientSocket;
	if (!pClientSocket)
		return;
	int nSocketStatus = pClientSocket->GetSocketState();
	if (nSocketStatus <= CCESocket::CREATED)
	{
		MessageBox(str_net_connect, _T("OTDR Viewer"), MB_OK|MB_ICONEXCLAMATION);
		return;
	}

	//���ͻ�ȡ������Ϣ����
	pClientSocket->SendLine(m_strGetMsg);
}
