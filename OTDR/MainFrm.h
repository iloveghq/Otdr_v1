// MainFrm.h : interface of the CMainFrame class
//
#pragma once
#include "OTDRView.h"
#include "MyToolBarEx.h"
#include "Waveform.h"
#include "Eventlist.h"
#include "Thumbnail.h"
#include "Report.h"
#include "MainSetting.h"
#include "PrintView.h"
#include "ChildFrm.h"
#include "TrueColorToolBar.h"
#include "SorFile.h"
#include "Waveform.h"
#include "RemoteConnDlg.h"
#include "RemotePageSheet.h"
#include "SorFileInfo.h"
#include "SorAssist.h"
#include "SORDATA.h"
#include "SorDrawStatic.h"
#include "OtdrTrace.h"//zll2013.4.23
#include "MySmooth.h"
#include "WaveFormLeftDlg.h"
#include "PropStep2A.h"
#include "MyPropertySheet.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)

public:
	CMainFrame();
	virtual ~CMainFrame();
	void SetLocaleString();
	void PrintPageHeader(CDC* pDC, UINT nPageNumber, CPrintInfo* pInfo);
	void ShowMainSetting(BOOL bShow); //�Ƿ���ʾ�����ô���
	void ShowReport(BOOL bShow); //�Ƿ���ʾ����
	void DecodeNetData(CString data); //�յ���λ��ͨ�����紫������������Ϣ�󣬸��¶�Ӧ��������Ϣ wcq2012.05.18
	CString GetNetData(CString setType);//��λ��Ҫ��ͬ����������,�ѱ��ص�һЩ������Ϣ����ʽ���һ�� wcq2012.05.18

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);


	// Generated message map functions
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//afx_msg void OnUsbLink();
	afx_msg void OnClose();

	//File
	afx_msg void OnOpen();
	afx_msg void OnUpdateMenu(CCmdUI *pCmdUI);
	afx_msg void OnSave();
	afx_msg void OnUpdateSave(CCmdUI *pCmdUI);
	afx_msg void OnPageSetup();
	afx_msg void OnPrint();
	afx_msg void OnUpdatePrint(CCmdUI *pCmdUI);
	afx_msg void OnPrintPreview();
	afx_msg	void OnUpdatePrintPreview(CCmdUI *pCmdUI);

	//Tool
	afx_msg void OnRemoteConnect();
	afx_msg void OnRemoteSet();
	afx_msg void OnRemoteControl();
	afx_msg void OnSwitchChs();
	afx_msg void OnSwitchEng();
	afx_msg void OnSwitchPort();  //���������л�

	//Event Analy Con
	afx_msg void OnEventEditMode();
	afx_msg void OnUpdateEventEditMode(CCmdUI *pCmdUI);
	afx_msg void OnEventAutoSearch();
	afx_msg void OnUpdateEventAutoSearch(CCmdUI *pCmdUI);
	afx_msg void OnEventAnalCon();
	afx_msg void OnUpdateEventAnalCon(CCmdUI *pCmdUI);
	//View
	afx_msg void OnViewWave();
	afx_msg void OnViewThumbnail();
	afx_msg void OnViewEvent();
	afx_msg void OnSceenSet();
	afx_msg void OnOriginSetting();

	//Report Wizard
	afx_msg void OnReportWizard();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//Help
	afx_msg void OnHelpContent();

	//Print
	LRESULT OnBeginPrinting(WPARAM wParam,LPARAM lParam);
	LRESULT OnEndPrinting(WPARAM wParam,LPARAM lParam);
	LRESULT OnMyPrint(WPARAM wParam,LPARAM lParam);


private:
	void CreateRemoteConnDlg(); //�����������Ӵ���
	void CreateWaveFormFrame();//����WaveForm����
	void CreateEventListFrame();//����EventList����
	void CreateThumbnailFrame();//����Thumbnail����
	void CreateReportFrame(); //����������
	void CreateMainSettingFrame(); //���������ô���
	BOOL CreateToolBar();//����������
	BOOL CreateStatusBar();//����״̬��
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void TileViewFrame();
	void GetMDIClientSIZE(SIZE *fclient);
	COTDRView* GetSpecifyFrameView(LPCTSTR title,int viewID = -1,int type = 0);
	void OnWaveform();
	void OnEventlist();
	void InitWaveConfig();
	void InitReportConfig();    //��ʼ�����ı�������
	void InitReportConfig_EN();  //��ʼ��Ӣ�ﱨ������
	void InitReportConfig_PT();  //��ʼ���������ﱨ������
	void InitRepLayListTitle();//��ʼ��step2������Ϣ
	void InitRepLayListTitle_EN();//��ʼ��step2Ӣ����Ϣ
	void InitRepLayListTitle_PT();//��ʼ��step2������Ϣlzy2014.6.17
	void InitOTDRConfig();
	void InitPrintConfig();
	void InitEventAnalConfig();

	void RecordConfig();
	void RecordConfig_EN();	
	void RecordConfig_PT();//���������ļ�����lzy2014.6.15 	
	void LoadOneCurve(CString strFilePath,  //�ļ���ȫ·�� 
					  CString strFileName,	 //�ļ���
					  int index);//��ǰ�������ţ� ����������һ�����ߣ� lzy 2015.2.10

protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

public:
	CWaveform *m_pWaveFormView;//������ͼ
	CEventlist *m_pEventFormView;//�¼��б���ͼ
	CThumbnail *m_pThumbFormView;//����ͼ
	CReport *m_pReportFormView;//������ͼ
	CMainSetting *m_pMainSetting;//�����ô�����ͼ
	CPrintView *m_pPrintView; //��ӡ����ͼ

	CChildFrame *m_cfWaveForm;
	BOOL m_bShowWaveForm;
	CChildFrame *m_cfReport;
	CChildFrame *m_cfEventList;
	BOOL m_bShowEventList;
	CChildFrame *m_cfThumbnail;
	BOOL m_bShowThumbnail;
	CChildFrame *m_cfMainSetting;
	BOOL m_bMainSetting;

	CStatusBar m_wndStatusBar;
	CMyToolBarEx m_wndToolBar;

	BOOL m_bTilelayout;
	CMenu m_menuChs; //���Ĳ˵�
	CMenu m_menuEng; //Ӣ��
	CMenu m_menuPort; //�������Ĳ˵�

	//��Frameλ��
	CRect m_frameRect;
	CRect m_reportRect;
	CRect m_eventlistRect; //�¼��б�
	CRect m_thumbnailRect; //����ͼ
	CRect m_mainSettingRect;//�����ô���
	CRect m_formviewRect; //��ͼ��
	//Զ�����ӵ�OTDR
	CRemoteConnDlg* m_pRemoteConnDlg;
	CRemotePageSheet* m_pRemotePageSheet;

	BOOL m_bFileMenuEnable;
	BOOL m_bReportMode; //�Ƿ�Ϊ����ģʽ

	SORDATA m_sorDATA;
	CSorAssist m_sorAssist;
	CSorDrawStatic m_sorDrawStatic;

	CGrandwayReg  m_reg;//ע��������
	CUtil util;
public:
    INT printType;
	INT printFrame;//zll 2012.10.29 δ���
public:
	CMySmooth m_smooth;
	WORD* pDataSmooth;
	COLORREF colorLine;
	CWaveFormLeftDlg m_leftDlg;
	afx_msg void OnSetting();
};