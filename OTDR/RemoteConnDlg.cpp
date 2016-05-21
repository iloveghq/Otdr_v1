// RemoteConnDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OTDR.h"
#include "RemoteConnDlg.h"
#include "ClientSocket.h"

// CRemoteConnDlg �Ի���

IMPLEMENT_DYNAMIC(CRemoteConnDlg, CDialog)
extern LANG g_currentLang;

CString str_Remoteconnect = _T("");
CString str_Remotedisconnect = _T("");
CString str_Remotedisconnect_check = _T("");
CString str_Remote_notice = _T("");


CRemoteConnDlg::CRemoteConnDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRemoteConnDlg::IDD, pParent)
	, m_nPort(0)
	, m_dwIP(0)
	, m_pClientSocket(NULL)
{

}

CRemoteConnDlg::~CRemoteConnDlg()
{
}

void CRemoteConnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nPort);
	DDX_IPAddress(pDX, IDC_IPADDRESS, m_dwIP);
}


BEGIN_MESSAGE_MAP(CRemoteConnDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CRemoteConnDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT, &CRemoteConnDlg::OnBnClickedBtnDisconnect)
	ON_MESSAGE(SOCK_ONRECEIVE, OnReceive)
	ON_MESSAGE(SOCK_ONCLOSE, OnDisconnect)
END_MESSAGE_MAP()


// CRemoteConnDlg ��Ϣ�������

void CRemoteConnDlg::OnBnClickedBtnConnect()
{
	GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(FALSE);
	int sockType;
	UINT port;

	//If it's already connected: disconnect.
	if(m_pClientSocket->GetSocketState() > CCESocket::CREATED)
	{
		m_pClientSocket->Disconnect();
		if(g_currentLang == LANG::CHS)
			MAIN->m_wndStatusBar.GetStatusBarCtrl().SetText(str_Remoteconnect,1,0);//����״̬���ı�
		else if(g_currentLang == LANG::ENG)
			MAIN->m_wndStatusBar.GetStatusBarCtrl().SetText(str_Remoteconnect,1,0);//����״̬���ı�
		return;
	}

	UpdateData();
	//��ȡ��IP��ַ
	m_strIP.Format(_T("%d.%d.%d.%d"),
		HIBYTE(HIWORD(m_dwIP)),
		LOBYTE(HIWORD(m_dwIP)),
		HIBYTE(LOWORD(m_dwIP)),
		LOBYTE(LOWORD(m_dwIP)));

	if(m_pClientSocket->Create(SOCK_STREAM))
	{
		if(m_pClientSocket->Connect(m_strIP, m_nPort))
		{
			CString strSend = _T("Connect Succeed");
			GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(TRUE);
			//m_pClientSocket->SendLine(strSend);
			if(g_currentLang == LANG::CHS)
				MAIN->m_wndStatusBar.GetStatusBarCtrl().SetText(str_Remoteconnect,1,0);//����״̬���ı�
			else if(g_currentLang == LANG::ENG)
				MAIN->m_wndStatusBar.GetStatusBarCtrl().SetText(str_Remoteconnect,1,0);//����״̬���ı�
			RecordSetting();
		}
		else
		{
			m_pClientSocket->Disconnect();
			GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(TRUE);
			MAIN->m_wndStatusBar.GetStatusBarCtrl().SetText(str_Remotedisconnect,1,0);//����״̬���ı�
			//CString strMsg = _T("��������ʧ�ܣ������������ã�");
			MessageBox(str_Remotedisconnect_check, str_Remote_notice, MB_OK|MB_ICONEXCLAMATION);
		}
	}
	//���ش���
	ShowWindow(SW_HIDE);
}

void CRemoteConnDlg::OnBnClickedBtnDisconnect()
{
	m_pClientSocket->Disconnect();
	//���ش���
	ShowWindow(SW_HIDE);
	if(g_currentLang == LANG::CHS)
		MAIN->m_wndStatusBar.GetStatusBarCtrl().SetText(str_Remotedisconnect,1,0);//����״̬���ı�
	else if(g_currentLang == LANG::ENG)
		MAIN->m_wndStatusBar.GetStatusBarCtrl().SetText(str_Remotedisconnect,1,0);//����״̬���ı�
}

BOOL CRemoteConnDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//����Socket
	m_pClientSocket = new CClientSocket(this);
	m_pClientSocket->SetEolFormat(CCESocket::EOL_CR);

	//���ñ��ػ�
	SetLocaleString();
	//��ȡ������Ϣ
	m_strIP = IniUtil::ReadSingleConfigInfo(_T("RemoteSetting"), _T("Ip"));
	char pszIP[MAX_PATH] = {""};
	WideCharToMultiByte(CP_OEMCP, 0, m_strIP, -1, pszIP, m_strIP.GetLength() , NULL, NULL);
	m_dwIP = ntohl(inet_addr(pszIP));

	CString strPort = IniUtil::ReadSingleConfigInfo(_T("RemoteSetting"), _T("Port"));
	m_nPort = _wtoi(strPort);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CRemoteConnDlg::RecordSetting()
{
	m_strIP.Format(_T("%d.%d.%d.%d"),
		HIBYTE(HIWORD(m_dwIP)),
		LOBYTE(HIWORD(m_dwIP)),
		HIBYTE(LOWORD(m_dwIP)),
		LOBYTE(LOWORD(m_dwIP)));
	//����IP
	IniUtil::WriteSingleConfigInfo(_T("RemoteSetting"), _T("Ip"), m_strIP);
	//����˿�
	CString strPort;
	strPort.Format(_T("%d"), m_nPort);
	IniUtil::WriteSingleConfigInfo(_T("RemoteSetting"), _T("Port"), strPort);
}


void CRemoteConnDlg::SetLocaleString()
{
	//���öԻ������
	CString strTitle = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_TITLE"));
	SetWindowText(strTitle);

	//����IP��ַ
	CString strIP = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_IP"));
	GetDlgItem(IDC_STATIC_IPADDR)->SetWindowText(strIP);

	//���ö˿ں�
	CString strPort = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_PORT"));
	GetDlgItem(IDC_STATIC_PORT)->SetWindowText(strPort);

	//�������� ��ť
	CString strConnect = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_CONNECT"));
	GetDlgItem(IDC_BTN_CONNECT)->SetWindowText(strConnect);

	//���öϿ� ��ť
	CString strDisConnect = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_DISCONNECT"));
	GetDlgItem(IDC_BTN_DISCONNECT)->SetWindowText(strDisConnect);

	str_Remoteconnect = IniUtil::ReadResString(_T("IDS_REMOTE_CONNECT"));
	str_Remotedisconnect = IniUtil::ReadResString(_T("IDS_REMOTE_DISCONNECT"));
	str_Remotedisconnect_check = IniUtil::ReadResString(_T("IDS_REMOTE_DISCONNECT_CHECK"));
	str_Remote_notice = IniUtil::ReadResString(_T("IDS_REMOTE_NOTICE"));
}

LRESULT CRemoteConnDlg::OnReceive(WPARAM wParam, LPARAM lParam)
{
	if (!m_pClientSocket)
		return -1;
	CString str, temp;
	while(m_pClientSocket->GetDataSize() > 0 && m_pClientSocket->ReadString(temp))
		str += temp;

	MAIN->DecodeNetData(str);//������������

	return 0;
}

LRESULT CRemoteConnDlg::OnDisconnect(WPARAM wParam, LPARAM lParam)
{
	MAIN->m_wndStatusBar.GetStatusBarCtrl().SetText(str_Remotedisconnect,1,0);//����״̬���ı�
	return 0;
}