#ifndef CONST_HEAD_FILE
#define CONST_HEAD_FILE
#endif

#ifdef CONST_HEAD_FILE
//��ʼ����///////////////////////////////////////////////////////////////////////////////
#include "mainfrm.h"
#include "OTDR.h"
extern COTDRApp theApp;
// ���峣�� //////////////////////////////////////////////////////////////////////

#define CONST_INITEVERYBODY // ��ʼ��ʱ����̶����ݵ�list�ؼ�
#define CONST_ID_LENGTH 5 //id����󳤶�
#define LPMAIN ((CMainFrame *)::AfxGetApp ()->m_pMainWnd) //���mainָ��
#define LISTBOX_MAX 1024 //�б����������
#define CONST_TEST //������Ա�־
#define LEFT_TOOLBAR WM_USER + 100	//��߹������¼���Ϣ







// ������ //////////////////////////////////////////////////////////////////////

//����:��ʾ������Ϣ�Լ�һЩ��־
#define CONST_WARNINGLOG(str,type) \
{	\
	(LPMAIN->m_list[1].AddString ((CString)str)); \
	(LPMAIN->m_list[1].SendMessage(WM_VSCROLL,SB_BOTTOM,0)); \
	(((int)type == 1)?(NULL):(theApp.logcount++)); \
	(LPMAIN->WatchListFull(&LPMAIN->m_list[1])); \
}







// �ؼ�ID //////////////////////////////////////////////////////////////////

#define ID_DOCKWND1 WM_USER + 150
#define ID_DOCKWND2 WM_USER + 151
#define ID_TABCTRL1 WM_USER + 152
#define ID_TABCTRL2 WM_USER + 153
#define ID_LISTBOX1 WM_USER + 154
#define ID_LISTBOX2 WM_USER + 155
#define ID_LISTBOX3 WM_USER + 156



//����///////////////////////////////////////////////////////////////////////////////////
#endif