// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "OTDR.h"
#include "MainFrm.h"
#include "Eventlist.h"
#include "Thumbnail.h"
#include "Report.h"
#include "Command.h"
#include "PrintSheet.h"
#include "ConfigSheet.h"
#include "EventSearchPageSheet.h"

#pragma comment(lib,"SOR.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define HELP_FILE_CHS _T("OTDR_CH.chm")   //2014.3.20 lzy
#define HELP_FILE_ENG _T("OTDR_EN.chm")   //2014.3.20 lzy

SorFileArray g_sorFileArray;
extern LANG g_currentLang;
CString str_OkorCancel=_T("");
CString str_max_curve=_T("");
CString str_open_file_error=_T("");
CString str_open_data_invalid=_T("");
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_SIZE()

	//File Menu 
	ON_COMMAND(ID_OPEN, &CMainFrame::OnOpen)
	ON_UPDATE_COMMAND_UI(ID_OPEN, &CMainFrame::OnUpdateMenu)
	ON_COMMAND(ID_SAVE, &CMainFrame::OnSave)
	ON_UPDATE_COMMAND_UI(ID_SAVE, &CMainFrame::OnUpdateSave)
	ON_COMMAND(ID_PAGE_SETUP, &CMainFrame::OnPageSetup)
	ON_UPDATE_COMMAND_UI(ID_PAGE_SETUP, &CMainFrame::OnUpdateMenu)
	ON_COMMAND(ID_PRINT, &CMainFrame::OnPrint)
	ON_UPDATE_COMMAND_UI(ID_PRINT, &CMainFrame::OnUpdatePrint)
	ON_COMMAND(ID_PRINT_PREVIEW, &CMainFrame::OnPrintPreview)
	ON_UPDATE_COMMAND_UI(ID_PRINT_PREVIEW, &CMainFrame::OnUpdatePrintPreview)

	//Tool Menu 
	ON_COMMAND(ID_REMOTE_CONNECT, &CMainFrame::OnRemoteConnect)
	ON_UPDATE_COMMAND_UI(ID_REMOTE_CONNECT, &CMainFrame::OnUpdateMenu)
	ON_COMMAND(ID_REMOTE_SET, &CMainFrame::OnRemoteSet)
	ON_UPDATE_COMMAND_UI(ID_REMOTE_SET, &CMainFrame::OnUpdateMenu)
	ON_COMMAND(ID_REMOTE_CONTROL, &CMainFrame::OnRemoteControl)
	ON_UPDATE_COMMAND_UI(ID_REMOTE_CONTROL, &CMainFrame::OnUpdateMenu)
	ON_COMMAND(ID_MENU_LANG_CHS, &CMainFrame::OnSwitchChs)
	ON_UPDATE_COMMAND_UI(ID_MENU_LANG_CHS, &CMainFrame::OnUpdateMenu)
	ON_COMMAND(ID_MENU_LANG_ENG, &CMainFrame::OnSwitchEng)
	ON_UPDATE_COMMAND_UI(ID_MENU_LANG_ENG, &CMainFrame::OnUpdateMenu)
	ON_COMMAND(ID_MENU_LANG_PORT, &CMainFrame::OnSwitchPort)
	ON_UPDATE_COMMAND_UI(ID_MENU_LANG_PORT, &CMainFrame::OnUpdateMenu)

	//View Menu
	ON_COMMAND(ID_VIEW_WAVE, &CMainFrame::OnViewWave)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WAVE, &CMainFrame::OnUpdateMenu)
	ON_COMMAND(ID_VIEW_THUMBNAIL, &CMainFrame::OnViewThumbnail)
	ON_UPDATE_COMMAND_UI(ID_VIEW_THUMBNAIL, &CMainFrame::OnUpdateMenu)
	ON_COMMAND(ID_VIEW_EVENTLIST, &CMainFrame::OnViewEvent)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EVENTLIST, &CMainFrame::OnUpdateMenu)
	ON_COMMAND(ID_WAVESCREEN_SET, &CMainFrame::OnSceenSet)
	ON_UPDATE_COMMAND_UI(ID_WAVESCREEN_SET, &CMainFrame::OnUpdateMenu)
	ON_COMMAND(ID_ORIGIN_SETTING, &CMainFrame::OnOriginSetting)
	ON_UPDATE_COMMAND_UI(ID_ORIGIN_SETTING, &CMainFrame::OnUpdateMenu)

	//Event Analyse Menu
	ON_COMMAND(ID_EVENT_EDIT_MODE, &CMainFrame::OnEventEditMode)
	ON_UPDATE_COMMAND_UI(ID_EVENT_EDIT_MODE, &CMainFrame::OnUpdateEventEditMode)
	ON_COMMAND(ID_EVENT_AUTO_SEARCH, &CMainFrame::OnEventAutoSearch)
	ON_UPDATE_COMMAND_UI(ID_EVENT_AUTO_SEARCH, &CMainFrame::OnUpdateEventAutoSearch)
	ON_COMMAND(ID_CON_SETTING, &CMainFrame::OnEventAnalCon)
	ON_UPDATE_COMMAND_UI(ID_CON_SETTING, &CMainFrame::OnUpdateEventAnalCon)

	//Report Menu
	ON_COMMAND(ID_REPORT_WIZARD, &CMainFrame::OnReportWizard)
	ON_UPDATE_COMMAND_UI(ID_REPORT_WIZARD, &CMainFrame::OnUpdateMenu)

	//Help Menu
	ON_COMMAND(ID_HELP_CONTENT, &CMainFrame::OnHelpContent)
	ON_COMMAND(ID_SETTING, &CMainFrame::OnSetting)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CONNECTSTATUS,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame(): 
		m_pRemoteConnDlg(NULL),
		m_bTilelayout(FALSE),
		m_bShowThumbnail(TRUE),
		m_bShowWaveForm(TRUE),
		m_bShowEventList(TRUE),
		m_bMainSetting(TRUE),
		m_bFileMenuEnable(FALSE),
		m_bReportMode(FALSE),
		m_cfWaveForm(NULL),
		m_cfEventList(NULL),
		m_cfThumbnail(NULL),
		m_cfReport(NULL),
		m_cfMainSetting(NULL),
		m_pWaveFormView(NULL),
		m_pEventFormView(NULL),
		m_pReportFormView(NULL),
		m_pThumbFormView(NULL),
		m_pMainSetting(NULL),
		m_pPrintView(NULL),
		m_pRemotePageSheet(NULL)
	{
		m_bAutoMenuEnable = FALSE;
		printType=0;
		printFrame=1;
		InitWaveConfig(); //初始化WaveConfig
		InitOTDRConfig(); //初始化OTDRConfig
		InitPrintConfig(); //初始化PrintConfig
		InitEventAnalConfig(); //初始化EventAnalConfig
	}

	CMainFrame::~CMainFrame()
	{
		if (m_pRemoteConnDlg != NULL)
			delete m_pRemoteConnDlg;
		if (m_pRemotePageSheet != NULL)
			delete m_pRemotePageSheet;
		
	}
	
	void CMainFrame::SetLocaleString()
	{
		/*   菜单项本地化                                      */
		if (m_menuChs.m_hMenu != NULL)
			m_menuChs.DestroyMenu();
		m_menuChs.LoadMenu(IDR_OTDRTYPE); //中文

		if (m_menuEng.m_hMenu != NULL)
			m_menuEng.DestroyMenu();
		m_menuEng.LoadMenu(IDR_OTDRTYPE_EN); //英文
		//CMenu *subMenu = m_menuEng.GetSubMenu(2);
		//subMenu->RemoveMenu(2, MF_BYPOSITION); //lzy 2014.8.15

		//lzy 2014.06.06
		if (m_menuPort.m_hMenu != NULL)
			m_menuPort.DestroyMenu();
		m_menuPort.LoadMenu(IDR_OTDRTYPE_PT); //葡萄牙语

		CToolBarCtrl &bb=m_wndToolBar.GetToolBarCtrl();
		CToolTipCtrl* aa=bb.GetToolTips();


		if(g_currentLang == LANG::CHS) 
		{
			SetMenu(&m_menuChs);

			//更改CToolBar类各按钮的Tip中文提示信息
			aa->UpdateTipText(_T("打开文件"),&m_wndToolBar,ID_OPEN);
			aa->UpdateTipText(_T("保存轨迹文件"),&m_wndToolBar,ID_SAVE);
			aa->UpdateTipText(_T("打印"),&m_wndToolBar,ID_PRINT);
			aa->UpdateTipText(_T("远程连接到设备"),&m_wndToolBar,ID_REMOTE_CONNECT);
			aa->UpdateTipText(_T("远程设置"),&m_wndToolBar,ID_REMOTE_SET);
			aa->UpdateTipText(_T("关于"),&m_wndToolBar,ID_APP_ABOUT);

			InitReportConfig();
			InitRepLayListTitle();//初始化ReportConfig
		}
		else if(g_currentLang == LANG::ENG)
		{ 
			SetMenu(&m_menuEng);

			//更改CToolBar类各按钮的Tip英文提示信息
			aa->UpdateTipText(_T("Open File"),&m_wndToolBar,ID_OPEN);
			aa->UpdateTipText(_T("Save File"),&m_wndToolBar,ID_SAVE);
			aa->UpdateTipText(_T("Print"),&m_wndToolBar,ID_PRINT);
			aa->UpdateTipText(_T("Remote Connection"),&m_wndToolBar,ID_REMOTE_CONNECT);
			aa->UpdateTipText(_T("Remote Setting"),&m_wndToolBar,ID_REMOTE_SET);
			aa->UpdateTipText(_T("About"),&m_wndToolBar,ID_APP_ABOUT);

			InitReportConfig_EN();
			InitRepLayListTitle_EN();//初始化ReportConfig_EN
		}
		else if(g_currentLang == LANG::PORT)    //lzy 2014.06.06
		{ 
			SetMenu(&m_menuPort);

			//更改CToolBar类各按钮的Tip英文提示信息
			aa->UpdateTipText(_T("Arquivo Aberto"),&m_wndToolBar,ID_OPEN);
			aa->UpdateTipText(_T("Salvar Arquivo"),&m_wndToolBar,ID_SAVE);
			aa->UpdateTipText(_T("Print"),&m_wndToolBar,ID_PRINT);
			aa->UpdateTipText(_T("Conectar Remoto"),&m_wndToolBar,ID_REMOTE_CONNECT);
			aa->UpdateTipText(_T("Configuração Remoto"),&m_wndToolBar,ID_REMOTE_SET);
			aa->UpdateTipText(_T("Sobre"),&m_wndToolBar,ID_APP_ABOUT);

			InitReportConfig_PT();
			InitRepLayListTitle_PT();//初始化ReportConfig_EN
		}


		//没有曲线加载时
		if (g_sorFileArray.totalCurveNumber == 0)
			m_bFileMenuEnable = FALSE;

		/*   标题本地化                                      */

		/*   子框架本地化                                      */
		m_pWaveFormView->SetLocalString();
		m_pEventFormView->SetLocalString();
		m_pThumbFormView->SetLocalString();
		m_pReportFormView->SetLocalString();
		m_pRemoteConnDlg->SetLocaleString();
		m_pMainSetting->SetLocalString();
		str_OkorCancel=IniUtil::ReadResString(_T("IDS_OKORCANCEL"));
		str_max_curve=IniUtil::ReadResString(_T("IDS_MAX_CURVE"));
		str_open_file_error=IniUtil::ReadResString(_T("IDS_OPEN_FILE_ERROR"));
		str_open_data_invalid=IniUtil::ReadResString(_T("IDS_OPEN_DATA_INVALID"));
	}

	int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
			return -1;

		//获取窗口大小
		this->GetClientRect(&m_frameRect);
		CreateRemoteConnDlg();//创建远程连接窗口
		CreateWaveFormFrame();//创建WaveForm窗口
		CreateEventListFrame();//创建EventList窗口
		CreateThumbnailFrame(); //创建Thumbnail窗口
		CreateReportFrame(); //创建Report窗口
		CreateMainSettingFrame(); //创建MainSetting窗口
		CreateToolBar(); //创建工具栏
		CreateStatusBar(); //创建工具栏

		return 0;
	}

	void CMainFrame::InitWaveConfig()
	{
		CString strTrace1 = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Trace1"));
		g_sorFileArray.waveConfig.ColorCurve1 = _wtol(strTrace1);
		g_sorFileArray.sorFileList[0].pTestResultInfo->curveColor = g_sorFileArray.waveConfig.ColorCurve1;

		CString strTrace2 = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Trace2"));
		g_sorFileArray.waveConfig.ColorCurve2  = _wtol(strTrace2);
		g_sorFileArray.sorFileList[1].pTestResultInfo->curveColor = g_sorFileArray.waveConfig.ColorCurve2;

		CString strTrace3 = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Trace3"));
		g_sorFileArray.waveConfig.ColorCurve3  = _wtol(strTrace3);
		g_sorFileArray.sorFileList[2].pTestResultInfo->curveColor = g_sorFileArray.waveConfig.ColorCurve3;

		CString strTrace4 = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Trace4"));
		g_sorFileArray.waveConfig.ColorCurve4  = _wtol(strTrace4);
		g_sorFileArray.sorFileList[3].pTestResultInfo->curveColor = g_sorFileArray.waveConfig.ColorCurve4;

		CString strTrace5 = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Trace5"));
		g_sorFileArray.waveConfig.ColorCurve5  = _wtol(strTrace5);
		g_sorFileArray.sorFileList[4].pTestResultInfo->curveColor = g_sorFileArray.waveConfig.ColorCurve5;

		CString strTrace6 = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Trace6"));
		g_sorFileArray.waveConfig.ColorCurve6  = _wtol(strTrace6);
		g_sorFileArray.sorFileList[5].pTestResultInfo->curveColor = g_sorFileArray.waveConfig.ColorCurve6;

		CString strTrace7 = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Trace7"));
		g_sorFileArray.waveConfig.ColorCurve7  = _wtol(strTrace7);
		g_sorFileArray.sorFileList[6].pTestResultInfo->curveColor = g_sorFileArray.waveConfig.ColorCurve7;

		CString strTrace8 = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Trace8"));
		g_sorFileArray.waveConfig.ColorCurve8  = _wtol(strTrace8);
		g_sorFileArray.sorFileList[7].pTestResultInfo->curveColor = g_sorFileArray.waveConfig.ColorCurve8;

		CString strBack = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Back"));
		g_sorFileArray.waveConfig.ColorBack = _wtol(strBack);

		CString strGrid = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Grid"));
		g_sorFileArray.waveConfig.ColorGrid = _wtol(strGrid);

		CString strEventMarker = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("EventMarker"));
		g_sorFileArray.waveConfig.ColorEvent = _wtol(strEventMarker);

		CString strCursor = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Cursor"));
		g_sorFileArray.waveConfig.ColorCursor = _wtol(strCursor);

		CString strShowOrNot = IniUtil::ReadSingleConfigInfo(_T("Condition"), _T("Grid"));
		if (strShowOrNot.CompareNoCase(_T("0")) == 0)
			g_sorFileArray.waveConfig.ShowGrid = TRUE;
		else
			g_sorFileArray.waveConfig.ShowGrid = FALSE;

		strShowOrNot = IniUtil::ReadSingleConfigInfo(_T("Condition"), _T("Cursor"));
		if (strShowOrNot.CompareNoCase(_T("0")) == 0)
			g_sorFileArray.waveConfig.ShowCursor = TRUE;
		else
			g_sorFileArray.waveConfig.ShowCursor = FALSE;

		strShowOrNot = IniUtil::ReadSingleConfigInfo(_T("Condition"), _T("Ghost"));
		if (strShowOrNot.CompareNoCase(_T("0")) == 0)
			g_sorFileArray.waveConfig.ShowDoubleCursor = TRUE;
		else
			g_sorFileArray.waveConfig.ShowDoubleCursor = FALSE;

		CString strMultiMode = IniUtil::ReadSingleConfigInfo(_T("FileList"), _T("MultiMode"));
		if (strMultiMode.CompareNoCase(_T("0")) == 0)
			g_sorFileArray.waveConfig.MultiMode = FALSE;
		else
			g_sorFileArray.waveConfig.MultiMode = TRUE;

		//UnitPage 
		CString strDbUnit = IniUtil::ReadSingleConfigInfo(_T("Unit"), _T("Figure"));
		g_sorFileArray.waveConfig.UnitDbFig = _wtoi(strDbUnit);

		CString strUnit = IniUtil::ReadSingleConfigInfo(_T("Unit"), _T("DFigure"));
		g_sorFileArray.waveConfig.UnitDistFig = _wtoi(strUnit);

		g_sorFileArray.waveConfig.DistUnit =  IniUtil::ReadSingleConfigInfo(_T("Unit"), _T("Distance"));
	}

	void CMainFrame::InitReportConfig()
	{
		//step1
		g_sorFileArray.reportConfig.ListFormat=IniUtil::ReadSingleConfigInfo(_T("RepLayListLay"), _T("ListFormat"));
		g_sorFileArray.reportConfig.Print=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay"), _T("Print"));
		g_sorFileArray.reportConfig.Direction=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay"), _T("Direction"));
		g_sorFileArray.reportConfig.WaveList=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay"), _T("WaveList"));
		//step2
		g_sorFileArray.reportConfig.Title=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Heading"));
		g_sorFileArray.reportConfig.Item1=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Item1"));
		g_sorFileArray.reportConfig.Item1Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Edit1"));
		g_sorFileArray.reportConfig.Item2=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Item2"));
		g_sorFileArray.reportConfig.Item2Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Edit2"));
		g_sorFileArray.reportConfig.Item3=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Item3"));
		g_sorFileArray.reportConfig.Item3Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Edit3"));
		g_sorFileArray.reportConfig.Item4=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Item4"));
		g_sorFileArray.reportConfig.Item4Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Edit4"));
		g_sorFileArray.reportConfig.Item5=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Item5"));
		g_sorFileArray.reportConfig.Item5Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Edit5"));
		g_sorFileArray.reportConfig.Item6=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Item6"));
		g_sorFileArray.reportConfig.Item6Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Edit6"));
		g_sorFileArray.reportConfig.Item7=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Item7"));
		g_sorFileArray.reportConfig.Item7Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Edit7"));
		g_sorFileArray.reportConfig.Item8=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Item8"));
		g_sorFileArray.reportConfig.Item8Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Edit8"));
		g_sorFileArray.reportConfig.Item9=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Item9"));
		g_sorFileArray.reportConfig.Item9Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Edit9"));
		g_sorFileArray.reportConfig.Item10=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Item10"));
		g_sorFileArray.reportConfig.Item10Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Edit10"));
		g_sorFileArray.reportConfig.Item11=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Item11"));
		g_sorFileArray.reportConfig.Item11Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Edit11"));
		g_sorFileArray.reportConfig.Item12=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Item12"));
		g_sorFileArray.reportConfig.Item12Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Edit12"));

		g_sorFileArray.reportConfig.Combox1=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox1"));
		g_sorFileArray.reportConfig.Combox2=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox2"));
		g_sorFileArray.reportConfig.Combox3=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox3"));
		g_sorFileArray.reportConfig.Combox4=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox4"));
		g_sorFileArray.reportConfig.Combox5=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox5"));
		g_sorFileArray.reportConfig.Combox6=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox6"));
		g_sorFileArray.reportConfig.Combox7=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox7"));
		g_sorFileArray.reportConfig.Combox8=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox8"));
		g_sorFileArray.reportConfig.Combox9=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox9"));
		g_sorFileArray.reportConfig.Combox10=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox10"));
		g_sorFileArray.reportConfig.Combox11=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox11"));
		g_sorFileArray.reportConfig.Combox12=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox12"));
		g_sorFileArray.reportConfig.Combox13=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox13"));
		g_sorFileArray.reportConfig.Combox14=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox14"));
		g_sorFileArray.reportConfig.Combox15=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox15"));
		g_sorFileArray.reportConfig.Combox16=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox16"));
		//Step3
		g_sorFileArray.reportConfig.NumberOfWaveLength=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay"), _T("NumberOfWaveLength"));
		g_sorFileArray.reportConfig.MeasureDir=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay"), _T("MeasureDir"));
		g_sorFileArray.reportConfig.OutputSort=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay"), _T("OutputSort"));
		g_sorFileArray.reportConfig.EventSort=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay"), _T("EventSort"));
		g_sorFileArray.reportConfig.UpDownSort=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay"), _T("UpDownSort"));
		g_sorFileArray.reportConfig.FileFormat=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay"), _T("FileFormat"));
		g_sorFileArray.reportConfig.NumberOfFiber=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay"), _T("NumberOfFiber"));
		g_sorFileArray.reportConfig.StartOfFiberNo=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay"), _T("StartOfFiberNo"));

		g_sorFileArray.reportConfig.FirstTime=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader"), _T("FirstTime"));
		g_sorFileArray.reportConfig.Step3Item1=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader"), _T("Item1"));
		g_sorFileArray.reportConfig.Step3Item2=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader"), _T("Item2"));
		g_sorFileArray.reportConfig.Step3Item3=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader"), _T("Item3"));
		g_sorFileArray.reportConfig.Step3Item4=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader"), _T("Item4"));
		g_sorFileArray.reportConfig.Step3Item5=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader"), _T("Item5"));
		g_sorFileArray.reportConfig.Step3Item6=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader"), _T("Item6"));
		g_sorFileArray.reportConfig.Step3Edit1=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader"), _T("Edit1"));
		g_sorFileArray.reportConfig.Step3Edit2=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader"), _T("Edit2"));
		g_sorFileArray.reportConfig.Step3Edit3=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader"), _T("Edit3"));
		g_sorFileArray.reportConfig.Step3Edit4=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader"), _T("Edit4"));
		g_sorFileArray.reportConfig.Step3Edit5=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader"), _T("Edit5"));
		g_sorFileArray.reportConfig.Step3Edit6=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader"), _T("Edit6"));
		g_sorFileArray.reportConfig.Step3ComboBox1=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader"), _T("ComboBox1"));
		g_sorFileArray.reportConfig.Step3ComboBox2=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader"), _T("ComboBox2"));
		g_sorFileArray.reportConfig.Step3ComboBox3=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader"), _T("ComboBox3"));
		g_sorFileArray.reportConfig.Step3ComboBox4=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader"), _T("ComboBox4"));
		g_sorFileArray.reportConfig.Step3ComboBox5=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader"), _T("ComboBox5"));
		g_sorFileArray.reportConfig.Step3ComboBox6=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader"), _T("ComboBox6"));
		g_sorFileArray.reportConfig.Step3ComboBox7=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader"), _T("ComboBox7"));
		g_sorFileArray.reportConfig.Step3ComboBox8=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader"), _T("ComboBox8"));
		g_sorFileArray.reportConfig.Step3ComboBox9=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader"), _T("ComboBox9"));
		g_sorFileArray.reportConfig.Step3ComboBox10=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader"), _T("ComboBox10"));
		//Step3 Rpt[RepLayListTotalList] [RepLayListLay]
		g_sorFileArray.reportConfig.Folder1=IniUtil::ReadSingleConfigInfo(_T("RepLayListTotalList"), _T("Add"));
		g_sorFileArray.reportConfig.Folder2=IniUtil::ReadSingleConfigInfo(_T("RepLayListLay"), _T("TotalLossOnOff"));
		g_sorFileArray.reportConfig.SetOptionSort=IniUtil::ReadSingleConfigInfo(_T("RepLayListLay"), _T("AddAbnormality"));
		//Step4
		g_sorFileArray.reportConfig.Folder1=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay"), _T("Folder1"));
		g_sorFileArray.reportConfig.Folder2=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay"), _T("Folder2"));
		g_sorFileArray.reportConfig.SetOptionSort=IniUtil::ReadSingleConfigInfo(_T("RepLayFileSelect"), _T("SetOptionSort"));
		g_sorFileArray.reportConfig.BeAdapted=IniUtil::ReadSingleConfigInfo(_T("FileSelOption"), _T("BeAdapted"));
		g_sorFileArray.reportConfig.WLIndex=IniUtil::ReadSingleConfigInfo(_T("FileSelOption"), _T("WLIndex"));
		
		//以下为新报表多语言版文字相关内容 lzy 2014.8.18
		g_sorFileArray.reportConfig.Job.strJobTitle=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("JobTitle"));
		g_sorFileArray.reportConfig.Job.strJobID=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("JobID"));
		g_sorFileArray.reportConfig.Job.strContractor=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("JobContractor"));
		g_sorFileArray.reportConfig.Job.strCustomer=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("JobCustomer"));
		g_sorFileArray.reportConfig.Job.strSorFileName=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("JobFile"));
		g_sorFileArray.reportConfig.Job.strTestDate=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("JobDate"));
		g_sorFileArray.reportConfig.Job.strOperator=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("JobOperator"));
		g_sorFileArray.reportConfig.Job.strComment=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("JobComment"));
	   	g_sorFileArray.reportConfig.Job.strEventTitle=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("EventTitle"));
		g_sorFileArray.reportConfig.Configuration.strTitle=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("ConfigTitle"));
		g_sorFileArray.reportConfig.Configuration.strOrigin=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("ConfigOrigin"));
		g_sorFileArray.reportConfig.Configuration.strOriLocation=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("ConfigOriginLocation"));
		g_sorFileArray.reportConfig.Configuration.strOriCable=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("ConfigOriginCable"));
		g_sorFileArray.reportConfig.Configuration.strOriFiber=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("ConfigOriginFiber"));
		g_sorFileArray.reportConfig.Configuration.strOriColor=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("ConfigOriginColor"));
		g_sorFileArray.reportConfig.Configuration.strEnd=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("ConfigEnd"));
		g_sorFileArray.reportConfig.Configuration.strEndLocation=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("ConfigEndLocation"));
		g_sorFileArray.reportConfig.Configuration.strEndCable=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("ConfigEndCable"));
		g_sorFileArray.reportConfig.Configuration.strEndFiber=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("ConfigEndFiber"));
		g_sorFileArray.reportConfig.Configuration.strEndColor=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("ConfigEndColor"));
		g_sorFileArray.reportConfig.Configuration.strWavelength=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("ConfigWave"));
		g_sorFileArray.reportConfig.Configuration.strIndex=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("ConfigIndex"));
		g_sorFileArray.reportConfig.Configuration.strPulse=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("ConfigPulse"));
		g_sorFileArray.reportConfig.Configuration.strRange=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("ConfigRange"));
		g_sorFileArray.reportConfig.Configuration.strAverage=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("ConfigAverage"));
		g_sorFileArray.reportConfig.Configuration.strResolution=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("ConfigResolution"));
		g_sorFileArray.reportConfig.Configuration.strBackScatter=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("ConfigRBS"));
		g_sorFileArray.reportConfig.Configuration.strLossThreshold=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("ConfigLossTH"));
		g_sorFileArray.reportConfig.Configuration.strRefThreshold=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("ConfigReflectionTH"));
		g_sorFileArray.reportConfig.Configuration.strEndThreshold=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("ConfigEndFiberTH"));
		g_sorFileArray.reportConfig.Trace.strTitle=IniUtil::ReadSingleConfigInfo(_T("NewReport_CH"), _T("TraceTitle"));


	}

	void CMainFrame::InitRepLayListTitle()
	{
		//step2
		g_sorFileArray.reportConfig.Title=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Heading"));
		g_sorFileArray.reportConfig.Item1=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Item1"));
		g_sorFileArray.reportConfig.Item1Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Edit1"));
		g_sorFileArray.reportConfig.Item2=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Item2"));
		g_sorFileArray.reportConfig.Item2Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Edit2"));
		g_sorFileArray.reportConfig.Item3=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Item3"));
		g_sorFileArray.reportConfig.Item3Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Edit3"));
		g_sorFileArray.reportConfig.Item4=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Item4"));
		g_sorFileArray.reportConfig.Item4Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Edit4"));
		g_sorFileArray.reportConfig.Item5=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Item5"));
		g_sorFileArray.reportConfig.Item5Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Edit5"));
		g_sorFileArray.reportConfig.Item6=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Item6"));
		g_sorFileArray.reportConfig.Item6Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Edit6"));
		g_sorFileArray.reportConfig.Item7=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Item7"));
		g_sorFileArray.reportConfig.Item7Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Edit7"));
		g_sorFileArray.reportConfig.Item8=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Item8"));
		g_sorFileArray.reportConfig.Item8Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Edit8"));
		g_sorFileArray.reportConfig.Item9=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Item9"));
		g_sorFileArray.reportConfig.Item9Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Edit9"));
		g_sorFileArray.reportConfig.Item10=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Item10"));
		g_sorFileArray.reportConfig.Item10Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Edit10"));
		g_sorFileArray.reportConfig.Item11=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Item11"));
		g_sorFileArray.reportConfig.Item11Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Edit11"));
		g_sorFileArray.reportConfig.Item12=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Item12"));
		g_sorFileArray.reportConfig.Item12Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("Edit12"));

		g_sorFileArray.reportConfig.Combox1=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox1"));
		g_sorFileArray.reportConfig.Combox2=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox2"));
		g_sorFileArray.reportConfig.Combox3=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox3"));
		g_sorFileArray.reportConfig.Combox4=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox4"));
		g_sorFileArray.reportConfig.Combox5=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox5"));
		g_sorFileArray.reportConfig.Combox6=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox6"));
		g_sorFileArray.reportConfig.Combox7=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox7"));
		g_sorFileArray.reportConfig.Combox8=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox8"));
		g_sorFileArray.reportConfig.Combox9=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox9"));
		g_sorFileArray.reportConfig.Combox10=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox10"));
		g_sorFileArray.reportConfig.Combox11=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox11"));
		g_sorFileArray.reportConfig.Combox12=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox12"));
		g_sorFileArray.reportConfig.Combox13=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox13"));
		g_sorFileArray.reportConfig.Combox14=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox14"));
		g_sorFileArray.reportConfig.Combox15=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox15"));
		g_sorFileArray.reportConfig.Combox16=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox16"));

	}

	void CMainFrame::InitReportConfig_EN()
	{
		//step1
		g_sorFileArray.reportConfig.ListFormat=IniUtil::ReadSingleConfigInfo(_T("RepLayListLay_EN"), _T("ListFormat"));
		g_sorFileArray.reportConfig.Print=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay_EN"), _T("Print"));
		g_sorFileArray.reportConfig.Direction=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay_EN"), _T("Direction"));
		g_sorFileArray.reportConfig.WaveList=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay"), _T("WaveList"));
		//step2
		g_sorFileArray.reportConfig.Title=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Heading"));
		g_sorFileArray.reportConfig.Item1=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item1"));
		g_sorFileArray.reportConfig.Item1Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit1"));
		g_sorFileArray.reportConfig.Item2=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item2"));
		g_sorFileArray.reportConfig.Item2Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit2"));
		g_sorFileArray.reportConfig.Item3=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item3"));
		g_sorFileArray.reportConfig.Item3Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit3"));
		g_sorFileArray.reportConfig.Item4=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item4"));
		g_sorFileArray.reportConfig.Item4Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit4"));
		g_sorFileArray.reportConfig.Item5=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item5"));
		g_sorFileArray.reportConfig.Item5Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit5"));
		g_sorFileArray.reportConfig.Item6=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item6"));
		g_sorFileArray.reportConfig.Item6Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit6"));
		g_sorFileArray.reportConfig.Item7=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item7"));
		g_sorFileArray.reportConfig.Item7Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit7"));
		g_sorFileArray.reportConfig.Item8=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item8"));
		g_sorFileArray.reportConfig.Item8Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit8"));
		g_sorFileArray.reportConfig.Item9=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item9"));
		g_sorFileArray.reportConfig.Item9Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit9"));
		g_sorFileArray.reportConfig.Item10=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item10"));
		g_sorFileArray.reportConfig.Item10Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit10"));
		g_sorFileArray.reportConfig.Item11=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item11"));
		g_sorFileArray.reportConfig.Item11Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit11"));
		g_sorFileArray.reportConfig.Item12=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item12"));
		g_sorFileArray.reportConfig.Item12Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit12"));

		g_sorFileArray.reportConfig.Combox1=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox1"));
		g_sorFileArray.reportConfig.Combox2=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox2"));
		g_sorFileArray.reportConfig.Combox3=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox3"));
		g_sorFileArray.reportConfig.Combox4=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox4"));
		g_sorFileArray.reportConfig.Combox5=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox5"));
		g_sorFileArray.reportConfig.Combox6=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox6"));
		g_sorFileArray.reportConfig.Combox7=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox7"));
		g_sorFileArray.reportConfig.Combox8=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox8"));
		g_sorFileArray.reportConfig.Combox9=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox9"));
		g_sorFileArray.reportConfig.Combox10=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox10"));
		g_sorFileArray.reportConfig.Combox11=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox11"));
		g_sorFileArray.reportConfig.Combox12=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox12"));
		g_sorFileArray.reportConfig.Combox13=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox13"));
		g_sorFileArray.reportConfig.Combox14=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox14"));
		g_sorFileArray.reportConfig.Combox15=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox15"));
		g_sorFileArray.reportConfig.Combox16=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox16"));
		//Step3
		g_sorFileArray.reportConfig.NumberOfWaveLength=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay_EN"), _T("NumberOfWaveLength"));
		g_sorFileArray.reportConfig.MeasureDir=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay_EN"), _T("MeasureDir"));
		g_sorFileArray.reportConfig.OutputSort=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay_EN"), _T("OutputSort"));
		g_sorFileArray.reportConfig.EventSort=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay_EN"), _T("EventSort"));
		g_sorFileArray.reportConfig.UpDownSort=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay_EN"), _T("UpDownSort"));
		g_sorFileArray.reportConfig.FileFormat=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay_EN"), _T("FileFormat"));
		g_sorFileArray.reportConfig.NumberOfFiber=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay_EN"), _T("NumberOfFiber"));
		g_sorFileArray.reportConfig.StartOfFiberNo=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay_EN"), _T("StartOfFiberNo"));
		
		g_sorFileArray.reportConfig.FirstTime=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_EN"), _T("FirstTime"));
		g_sorFileArray.reportConfig.Step3Item1=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_EN"), _T("Item1"));
		g_sorFileArray.reportConfig.Step3Item2=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_EN"), _T("Item2"));
		g_sorFileArray.reportConfig.Step3Item3=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_EN"), _T("Item3"));
		g_sorFileArray.reportConfig.Step3Item4=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_EN"), _T("Item4"));
		g_sorFileArray.reportConfig.Step3Item5=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_EN"), _T("Item5"));
		g_sorFileArray.reportConfig.Step3Item6=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_EN"), _T("Item6"));
		g_sorFileArray.reportConfig.Step3Edit1=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_EN"), _T("Edit1"));
		g_sorFileArray.reportConfig.Step3Edit2=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_EN"), _T("Edit2"));
		g_sorFileArray.reportConfig.Step3Edit3=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_EN"), _T("Edit3"));
		g_sorFileArray.reportConfig.Step3Edit4=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_EN"), _T("Edit4"));
		g_sorFileArray.reportConfig.Step3Edit5=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_EN"), _T("Edit5"));
		g_sorFileArray.reportConfig.Step3Edit6=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_EN"), _T("Edit6"));
		g_sorFileArray.reportConfig.Step3ComboBox1=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_EN"), _T("ComboBox1"));
		g_sorFileArray.reportConfig.Step3ComboBox2=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_EN"), _T("ComboBox2"));
		g_sorFileArray.reportConfig.Step3ComboBox3=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_EN"), _T("ComboBox3"));
		g_sorFileArray.reportConfig.Step3ComboBox4=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_EN"), _T("ComboBox4"));
		g_sorFileArray.reportConfig.Step3ComboBox5=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_EN"), _T("ComboBox5"));
		g_sorFileArray.reportConfig.Step3ComboBox6=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_EN"), _T("ComboBox6"));
		g_sorFileArray.reportConfig.Step3ComboBox7=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_EN"), _T("ComboBox7"));
		g_sorFileArray.reportConfig.Step3ComboBox8=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_EN"), _T("ComboBox8"));
		g_sorFileArray.reportConfig.Step3ComboBox9=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_EN"), _T("ComboBox9"));
		g_sorFileArray.reportConfig.Step3ComboBox10=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_EN"), _T("ComboBox10"));
		//Step3 Rpt[RepLayListTotalList] [RepLayListLay]
		g_sorFileArray.reportConfig.Folder1=IniUtil::ReadSingleConfigInfo(_T("RepLayListTotalList"), _T("Add"));
		g_sorFileArray.reportConfig.Folder2=IniUtil::ReadSingleConfigInfo(_T("RepLayListLay_EN"), _T("TotalLossOnOff"));
		g_sorFileArray.reportConfig.SetOptionSort=IniUtil::ReadSingleConfigInfo(_T("RepLayListLay_EN"), _T("AddAbnormality"));
		//Step4
		g_sorFileArray.reportConfig.Folder1=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay_EN"), _T("Folder1"));
		g_sorFileArray.reportConfig.Folder2=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay_EN"), _T("Folder2"));
		g_sorFileArray.reportConfig.SetOptionSort=IniUtil::ReadSingleConfigInfo(_T("RepLayFileSelect"), _T("SetOptionSort"));
		g_sorFileArray.reportConfig.BeAdapted=IniUtil::ReadSingleConfigInfo(_T("FileSelOption"), _T("BeAdapted"));
		g_sorFileArray.reportConfig.WLIndex=IniUtil::ReadSingleConfigInfo(_T("FileSelOption"), _T("WLIndex"));

		//以下为新报表多语言版文字相关内容 lzy 2014.8.18
		g_sorFileArray.reportConfig.Job.strJobTitle=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("JobTitle"));
		g_sorFileArray.reportConfig.Job.strJobID=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("JobID"));
		g_sorFileArray.reportConfig.Job.strContractor=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("JobContractor"));
		g_sorFileArray.reportConfig.Job.strCustomer=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("JobCustomer"));
		g_sorFileArray.reportConfig.Job.strSorFileName=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("JobFile"));
		g_sorFileArray.reportConfig.Job.strTestDate=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("JobDate"));
		g_sorFileArray.reportConfig.Job.strOperator=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("JobOperator"));
		g_sorFileArray.reportConfig.Job.strComment=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("JobComment"));
	   	g_sorFileArray.reportConfig.Job.strEventTitle=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("EventTitle"));
		g_sorFileArray.reportConfig.Configuration.strTitle=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("ConfigTitle"));
		g_sorFileArray.reportConfig.Configuration.strOrigin=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("ConfigOrigin"));
		g_sorFileArray.reportConfig.Configuration.strOriLocation=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("ConfigOriginLocation"));
		g_sorFileArray.reportConfig.Configuration.strOriCable=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("ConfigOriginCable"));
		g_sorFileArray.reportConfig.Configuration.strOriFiber=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("ConfigOriginFiber"));
		g_sorFileArray.reportConfig.Configuration.strOriColor=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("ConfigOriginColor"));
		g_sorFileArray.reportConfig.Configuration.strEnd=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("ConfigEnd"));
		g_sorFileArray.reportConfig.Configuration.strEndLocation=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("ConfigEndLocation"));
		g_sorFileArray.reportConfig.Configuration.strEndCable=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("ConfigEndCable"));
		g_sorFileArray.reportConfig.Configuration.strEndFiber=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("ConfigEndFiber"));
		g_sorFileArray.reportConfig.Configuration.strEndColor=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("ConfigEndColor"));
		g_sorFileArray.reportConfig.Configuration.strWavelength=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("ConfigWave"));
		g_sorFileArray.reportConfig.Configuration.strIndex=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("ConfigIndex"));
		g_sorFileArray.reportConfig.Configuration.strPulse=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("ConfigPulse"));
		g_sorFileArray.reportConfig.Configuration.strRange=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("ConfigRange"));
		g_sorFileArray.reportConfig.Configuration.strAverage=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("ConfigAverage"));
		g_sorFileArray.reportConfig.Configuration.strResolution=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("ConfigResolution"));
		g_sorFileArray.reportConfig.Configuration.strBackScatter=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("ConfigRBS"));
		g_sorFileArray.reportConfig.Configuration.strLossThreshold=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("ConfigLossTH"));
		g_sorFileArray.reportConfig.Configuration.strRefThreshold=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("ConfigReflectionTH"));
		g_sorFileArray.reportConfig.Configuration.strEndThreshold=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("ConfigEndFiberTH"));
		g_sorFileArray.reportConfig.Trace.strTitle=IniUtil::ReadSingleConfigInfo(_T("NewReport_EN"), _T("TraceTitle"));

	}

	void CMainFrame::InitReportConfig_PT()
	{
		//step1
		g_sorFileArray.reportConfig.ListFormat=IniUtil::ReadSingleConfigInfo(_T("RepLayListLay_PT"), _T("ListFormat"));
		g_sorFileArray.reportConfig.Print=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay_PT"), _T("Print"));
		g_sorFileArray.reportConfig.Direction=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay_PT"), _T("Direction"));
		g_sorFileArray.reportConfig.WaveList=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay"), _T("WaveList"));           //?
		//step2
		g_sorFileArray.reportConfig.Title=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Heading"));
		g_sorFileArray.reportConfig.Item1=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item1"));
		g_sorFileArray.reportConfig.Item1Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit1"));
		g_sorFileArray.reportConfig.Item2=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item2"));
		g_sorFileArray.reportConfig.Item2Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit2"));
		g_sorFileArray.reportConfig.Item3=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item3"));
		g_sorFileArray.reportConfig.Item3Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit3"));
		g_sorFileArray.reportConfig.Item4=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item4"));
		g_sorFileArray.reportConfig.Item4Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit4"));
		g_sorFileArray.reportConfig.Item5=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item5"));
		g_sorFileArray.reportConfig.Item5Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit5"));
		g_sorFileArray.reportConfig.Item6=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item6"));
		g_sorFileArray.reportConfig.Item6Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit6"));
		g_sorFileArray.reportConfig.Item7=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item7"));
		g_sorFileArray.reportConfig.Item7Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit7"));
		g_sorFileArray.reportConfig.Item8=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item8"));
		g_sorFileArray.reportConfig.Item8Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit8"));
		g_sorFileArray.reportConfig.Item9=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item9"));
		g_sorFileArray.reportConfig.Item9Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit9"));
		g_sorFileArray.reportConfig.Item10=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item10"));
		g_sorFileArray.reportConfig.Item10Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit10"));
		g_sorFileArray.reportConfig.Item11=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item11"));
		g_sorFileArray.reportConfig.Item11Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit11"));
		g_sorFileArray.reportConfig.Item12=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item12"));
		g_sorFileArray.reportConfig.Item12Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit12"));

		g_sorFileArray.reportConfig.Combox1=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox1"));
		g_sorFileArray.reportConfig.Combox2=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox2"));
		g_sorFileArray.reportConfig.Combox3=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox3"));
		g_sorFileArray.reportConfig.Combox4=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox4"));
		g_sorFileArray.reportConfig.Combox5=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox5"));
		g_sorFileArray.reportConfig.Combox6=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox6"));
		g_sorFileArray.reportConfig.Combox7=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox7"));
		g_sorFileArray.reportConfig.Combox8=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox8"));
		g_sorFileArray.reportConfig.Combox9=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox9"));
		g_sorFileArray.reportConfig.Combox10=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox10"));
		g_sorFileArray.reportConfig.Combox11=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox11"));
		g_sorFileArray.reportConfig.Combox12=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox12"));
		g_sorFileArray.reportConfig.Combox13=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox13"));
		g_sorFileArray.reportConfig.Combox14=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox14"));
		g_sorFileArray.reportConfig.Combox15=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox15"));
		g_sorFileArray.reportConfig.Combox16=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox16"));
		//Step3
		g_sorFileArray.reportConfig.NumberOfWaveLength=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay_PT"), _T("NumberOfWaveLength"));
		g_sorFileArray.reportConfig.MeasureDir=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay_PT"), _T("MeasureDir"));
		g_sorFileArray.reportConfig.OutputSort=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay_PT"), _T("OutputSort"));
		g_sorFileArray.reportConfig.EventSort=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay_PT"), _T("EventSort"));
		g_sorFileArray.reportConfig.UpDownSort=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay_PT"), _T("UpDownSort"));
		g_sorFileArray.reportConfig.FileFormat=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay_PT"), _T("FileFormat"));
		g_sorFileArray.reportConfig.NumberOfFiber=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay_PT"), _T("NumberOfFiber"));
		g_sorFileArray.reportConfig.StartOfFiberNo=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay_PT"), _T("StartOfFiberNo"));

		g_sorFileArray.reportConfig.FirstTime=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_PT"), _T("FirstTime"));
		g_sorFileArray.reportConfig.Step3Item1=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_PT"), _T("Item1"));
		g_sorFileArray.reportConfig.Step3Item2=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_PT"), _T("Item2"));
		g_sorFileArray.reportConfig.Step3Item3=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_PT"), _T("Item3"));
		g_sorFileArray.reportConfig.Step3Item4=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_PT"), _T("Item4"));
		g_sorFileArray.reportConfig.Step3Item5=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_PT"), _T("Item5"));
		g_sorFileArray.reportConfig.Step3Item6=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_PT"), _T("Item6"));
		g_sorFileArray.reportConfig.Step3Edit1=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_PT"), _T("Edit1"));
		g_sorFileArray.reportConfig.Step3Edit2=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_PT"), _T("Edit2"));
		g_sorFileArray.reportConfig.Step3Edit3=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_PT"), _T("Edit3"));
		g_sorFileArray.reportConfig.Step3Edit4=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_PT"), _T("Edit4"));
		g_sorFileArray.reportConfig.Step3Edit5=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_PT"), _T("Edit5"));
		g_sorFileArray.reportConfig.Step3Edit6=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_PT"), _T("Edit6"));
		g_sorFileArray.reportConfig.Step3ComboBox1=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_PT"), _T("ComboBox1"));
		g_sorFileArray.reportConfig.Step3ComboBox2=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_PT"), _T("ComboBox2"));
		g_sorFileArray.reportConfig.Step3ComboBox3=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_PT"), _T("ComboBox3"));
		g_sorFileArray.reportConfig.Step3ComboBox4=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_PT"), _T("ComboBox4"));
		g_sorFileArray.reportConfig.Step3ComboBox5=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_PT"), _T("ComboBox5"));
		g_sorFileArray.reportConfig.Step3ComboBox6=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_PT"), _T("ComboBox6"));
		g_sorFileArray.reportConfig.Step3ComboBox7=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_PT"), _T("ComboBox7"));
		g_sorFileArray.reportConfig.Step3ComboBox8=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_PT"), _T("ComboBox8"));
		g_sorFileArray.reportConfig.Step3ComboBox9=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_PT"), _T("ComboBox9"));
		g_sorFileArray.reportConfig.Step3ComboBox10=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveHeader_PT"), _T("ComboBox10"));
		//Step3 Rpt[RepLayListTotalList] [RepLayListLay]
		g_sorFileArray.reportConfig.Folder1=IniUtil::ReadSingleConfigInfo(_T("RepLayListTotalList"), _T("Add"));
		g_sorFileArray.reportConfig.Folder2=IniUtil::ReadSingleConfigInfo(_T("RepLayListLay_PT"), _T("TotalLossOnOff"));
		g_sorFileArray.reportConfig.SetOptionSort=IniUtil::ReadSingleConfigInfo(_T("RepLayListLay_PT"), _T("AddAbnormality"));
		//Step4
		g_sorFileArray.reportConfig.Folder1=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay_PT"), _T("Folder1"));
		g_sorFileArray.reportConfig.Folder2=IniUtil::ReadSingleConfigInfo(_T("RepLayWaveLay_PT"), _T("Folder2"));
		g_sorFileArray.reportConfig.SetOptionSort=IniUtil::ReadSingleConfigInfo(_T("RepLayFileSelect"), _T("SetOptionSort"));
		g_sorFileArray.reportConfig.BeAdapted=IniUtil::ReadSingleConfigInfo(_T("FileSelOption"), _T("BeAdapted"));
		g_sorFileArray.reportConfig.WLIndex=IniUtil::ReadSingleConfigInfo(_T("FileSelOption"), _T("WLIndex"));

		//以下为新报表多语言版文字相关内容 lzy 2014.8.18
		g_sorFileArray.reportConfig.Job.strJobTitle=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("JobTitle"));
		g_sorFileArray.reportConfig.Job.strJobID=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("JobID"));
		g_sorFileArray.reportConfig.Job.strContractor=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("JobContractor"));
		g_sorFileArray.reportConfig.Job.strCustomer=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("JobCustomer"));
		g_sorFileArray.reportConfig.Job.strSorFileName=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("JobFile"));
		g_sorFileArray.reportConfig.Job.strTestDate=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("JobDate"));
		g_sorFileArray.reportConfig.Job.strOperator=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("JobOperator"));
		g_sorFileArray.reportConfig.Job.strComment=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("JobComment"));
	   	g_sorFileArray.reportConfig.Job.strEventTitle=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("EventTitle"));
		g_sorFileArray.reportConfig.Configuration.strTitle=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("ConfigTitle"));
		g_sorFileArray.reportConfig.Configuration.strOrigin=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("ConfigOrigin"));
		g_sorFileArray.reportConfig.Configuration.strOriLocation=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("ConfigOriginLocation"));
		g_sorFileArray.reportConfig.Configuration.strOriCable=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("ConfigOriginCable"));
		g_sorFileArray.reportConfig.Configuration.strOriFiber=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("ConfigOriginFiber"));
		g_sorFileArray.reportConfig.Configuration.strOriColor=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("ConfigOriginColor"));
		g_sorFileArray.reportConfig.Configuration.strEnd=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("ConfigEnd"));
		g_sorFileArray.reportConfig.Configuration.strEndLocation=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("ConfigEndLocation"));
		g_sorFileArray.reportConfig.Configuration.strEndCable=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("ConfigEndCable"));
		g_sorFileArray.reportConfig.Configuration.strEndFiber=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("ConfigEndFiber"));
		g_sorFileArray.reportConfig.Configuration.strEndColor=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("ConfigEndColor"));
		g_sorFileArray.reportConfig.Configuration.strWavelength=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("ConfigWave"));
		g_sorFileArray.reportConfig.Configuration.strIndex=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("ConfigIndex"));
		g_sorFileArray.reportConfig.Configuration.strPulse=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("ConfigPulse"));
		g_sorFileArray.reportConfig.Configuration.strRange=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("ConfigRange"));
		g_sorFileArray.reportConfig.Configuration.strAverage=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("ConfigAverage"));
		g_sorFileArray.reportConfig.Configuration.strResolution=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("ConfigResolution"));
		g_sorFileArray.reportConfig.Configuration.strBackScatter=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("ConfigRBS"));
		g_sorFileArray.reportConfig.Configuration.strLossThreshold=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("ConfigLossTH"));
		g_sorFileArray.reportConfig.Configuration.strRefThreshold=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("ConfigReflectionTH"));
		g_sorFileArray.reportConfig.Configuration.strEndThreshold=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("ConfigEndFiberTH"));
		g_sorFileArray.reportConfig.Trace.strTitle=IniUtil::ReadSingleConfigInfo(_T("NewReport_PT"), _T("TraceTitle"));

	}

	void CMainFrame::InitRepLayListTitle_EN()
	{
		//step2
		g_sorFileArray.reportConfig.Title=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Heading"));
		g_sorFileArray.reportConfig.Item1=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item1"));
		g_sorFileArray.reportConfig.Item1Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit1"));
		g_sorFileArray.reportConfig.Item2=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item2"));
		g_sorFileArray.reportConfig.Item2Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit2"));
		g_sorFileArray.reportConfig.Item3=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item3"));
		g_sorFileArray.reportConfig.Item3Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit3"));
		g_sorFileArray.reportConfig.Item4=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item4"));
		g_sorFileArray.reportConfig.Item4Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit4"));
		g_sorFileArray.reportConfig.Item5=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item5"));
		g_sorFileArray.reportConfig.Item5Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit5"));
		g_sorFileArray.reportConfig.Item6=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item6"));
		g_sorFileArray.reportConfig.Item6Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit6"));
		g_sorFileArray.reportConfig.Item7=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item7"));
		g_sorFileArray.reportConfig.Item7Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit7"));
		g_sorFileArray.reportConfig.Item8=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item8"));
		g_sorFileArray.reportConfig.Item8Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit8"));
		g_sorFileArray.reportConfig.Item9=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item9"));
		g_sorFileArray.reportConfig.Item9Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit9"));
		g_sorFileArray.reportConfig.Item10=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item10"));
		g_sorFileArray.reportConfig.Item10Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit10"));
		g_sorFileArray.reportConfig.Item11=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item11"));
		g_sorFileArray.reportConfig.Item11Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit11"));
		g_sorFileArray.reportConfig.Item12=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item12"));
		g_sorFileArray.reportConfig.Item12Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit12"));

		g_sorFileArray.reportConfig.Combox1=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox1"));
		g_sorFileArray.reportConfig.Combox2=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox2"));
		g_sorFileArray.reportConfig.Combox3=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox3"));
		g_sorFileArray.reportConfig.Combox4=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox4"));
		g_sorFileArray.reportConfig.Combox5=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox5"));
		g_sorFileArray.reportConfig.Combox6=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox6"));
		g_sorFileArray.reportConfig.Combox7=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox7"));
		g_sorFileArray.reportConfig.Combox8=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox8"));
		g_sorFileArray.reportConfig.Combox9=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox9"));
		g_sorFileArray.reportConfig.Combox10=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox10"));
		g_sorFileArray.reportConfig.Combox11=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox11"));
		g_sorFileArray.reportConfig.Combox12=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox12"));
		g_sorFileArray.reportConfig.Combox13=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox13"));
		g_sorFileArray.reportConfig.Combox14=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox14"));
		g_sorFileArray.reportConfig.Combox15=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox15"));
		g_sorFileArray.reportConfig.Combox16=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox16"));
		
	}


	void CMainFrame::InitRepLayListTitle_PT()
	{
		//step2
		g_sorFileArray.reportConfig.Title=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Heading"));
		g_sorFileArray.reportConfig.Item1=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item1"));
		g_sorFileArray.reportConfig.Item1Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit1"));
		g_sorFileArray.reportConfig.Item2=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item2"));
		g_sorFileArray.reportConfig.Item2Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit2"));
		g_sorFileArray.reportConfig.Item3=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item3"));
		g_sorFileArray.reportConfig.Item3Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit3"));
		g_sorFileArray.reportConfig.Item4=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item4"));
		g_sorFileArray.reportConfig.Item4Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit4"));
		g_sorFileArray.reportConfig.Item5=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item5"));
		g_sorFileArray.reportConfig.Item5Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit5"));
		g_sorFileArray.reportConfig.Item6=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item6"));
		g_sorFileArray.reportConfig.Item6Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit6"));
		g_sorFileArray.reportConfig.Item7=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item7"));
		g_sorFileArray.reportConfig.Item7Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit7"));
		g_sorFileArray.reportConfig.Item8=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item8"));
		g_sorFileArray.reportConfig.Item8Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit8"));
		g_sorFileArray.reportConfig.Item9=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item9"));
		g_sorFileArray.reportConfig.Item9Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit9"));
		g_sorFileArray.reportConfig.Item10=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item10"));
		g_sorFileArray.reportConfig.Item10Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit10"));
		g_sorFileArray.reportConfig.Item11=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item11"));
		g_sorFileArray.reportConfig.Item11Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit11"));
		g_sorFileArray.reportConfig.Item12=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item12"));
		g_sorFileArray.reportConfig.Item12Val=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit12"));

		g_sorFileArray.reportConfig.Combox1=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox1"));
		g_sorFileArray.reportConfig.Combox2=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox2"));
		g_sorFileArray.reportConfig.Combox3=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox3"));
		g_sorFileArray.reportConfig.Combox4=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox4"));
		g_sorFileArray.reportConfig.Combox5=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox5"));
		g_sorFileArray.reportConfig.Combox6=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox6"));
		g_sorFileArray.reportConfig.Combox7=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox7"));
		g_sorFileArray.reportConfig.Combox8=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox8"));
		g_sorFileArray.reportConfig.Combox9=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox9"));
		g_sorFileArray.reportConfig.Combox10=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox10"));
		g_sorFileArray.reportConfig.Combox11=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox11"));
		g_sorFileArray.reportConfig.Combox12=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox12"));
		g_sorFileArray.reportConfig.Combox13=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox13"));
		g_sorFileArray.reportConfig.Combox14=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox14"));
		g_sorFileArray.reportConfig.Combox15=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox15"));
		g_sorFileArray.reportConfig.Combox16=IniUtil::ReadSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox16"));
	}


	void CMainFrame::InitOTDRConfig()
	{
		//System Config
		g_sorFileArray.otdrConfig.strLang=IniUtil::ReadSingleConfigInfo(_T("OTDR"), _T("Language"));
		g_sorFileArray.otdrConfig.strLightLevel=IniUtil::ReadSingleConfigInfo(_T("OTDR"), _T("LightLevel"));
		g_sorFileArray.otdrConfig.strAutoPowerOff=IniUtil::ReadSingleConfigInfo(_T("OTDR"), _T("AutoPowerOff"));
		//g_sorFileArray.otdrConfig.strDate=IniUtil::ReadSingleConfigInfo(_T("OTDR"), _T("Date"));
		//g_sorFileArray.otdrConfig.strTime=IniUtil::ReadSingleConfigInfo(_T("OTDR"), _T("Time"));
		g_sorFileArray.otdrConfig.strTouchSreeenStatus=IniUtil::ReadSingleConfigInfo(_T("OTDR"), _T("TouchSreeenStatus"));
		//Test Config
		g_sorFileArray.otdrConfig.strTestWave=IniUtil::ReadSingleConfigInfo(_T("OTDR"), _T("TestWave"));
		g_sorFileArray.otdrConfig.strTestTime=IniUtil::ReadSingleConfigInfo(_T("OTDR"), _T("TestTime"));
		g_sorFileArray.otdrConfig.strTestMode=IniUtil::ReadSingleConfigInfo(_T("OTDR"), _T("TestMode"));
		g_sorFileArray.otdrConfig.strTestRange=IniUtil::ReadSingleConfigInfo(_T("OTDR"), _T("TestRange"));
		g_sorFileArray.otdrConfig.strUnit=IniUtil::ReadSingleConfigInfo(_T("OTDR"), _T("TestUnit"));
		g_sorFileArray.otdrConfig.strPluseWidth=IniUtil::ReadSingleConfigInfo(_T("OTDR"), _T("PluseWidth"));
		g_sorFileArray.otdrConfig.strResolution=IniUtil::ReadSingleConfigInfo(_T("OTDR"), _T("Resolution"));
		g_sorFileArray.otdrConfig.strAutoSave=IniUtil::ReadSingleConfigInfo(_T("OTDR"), _T("AutoSave"));
	}
	
	void CMainFrame::InitPrintConfig()
	{
		//RepLayPageMargin
		g_sorFileArray.printConfig.Left=IniUtil::ReadSingleConfigInfo(_T("RepLayPageMargin"), _T("Left"));
		g_sorFileArray.printConfig.Right=IniUtil::ReadSingleConfigInfo(_T("RepLayPageMargin"), _T("Right"));
		g_sorFileArray.printConfig.Top=IniUtil::ReadSingleConfigInfo(_T("RepLayPageMargin"), _T("Top"));
		g_sorFileArray.printConfig.Bottom=IniUtil::ReadSingleConfigInfo(_T("RepLayPageMargin"), _T("Bottom"));
		//RepLayPageNumber
		g_sorFileArray.printConfig.AddPage=IniUtil::ReadSingleConfigInfo(_T("RepLayPageNumber"), _T("AddPage"));
		g_sorFileArray.printConfig.PageNum=IniUtil::ReadSingleConfigInfo(_T("RepLayPageNumber"), _T("PageNum"));
		g_sorFileArray.printConfig.PagePos1=IniUtil::ReadSingleConfigInfo(_T("RepLayPageNumber"), _T("PagePos1"));
		g_sorFileArray.printConfig.PagePos2=IniUtil::ReadSingleConfigInfo(_T("RepLayPageNumber"), _T("PagePos2"));
		g_sorFileArray.printConfig.AddSection=IniUtil::ReadSingleConfigInfo(_T("RepLayPageNumber"), _T("AddSection"));
		g_sorFileArray.printConfig.SectionNum=IniUtil::ReadSingleConfigInfo(_T("RepLayPageNumber"), _T("SectionNum"));
	}

	void CMainFrame::InitEventAnalConfig()
	{
		CString strBendTH = IniUtil::ReadSingleConfigInfo(_T("EventAnal"), _T("BendTH"));
		g_sorFileArray.eventAnalConfig.BendTH= _wtof(strBendTH);

		CString strReflectTH =IniUtil::ReadSingleConfigInfo(_T("EventAnal"), _T("ReflectTH"));
		g_sorFileArray.eventAnalConfig.ReflectTH = _wtof(strReflectTH);

		CString strTermial = IniUtil::ReadSingleConfigInfo(_T("EventAnal"), _T("Termial"));
		g_sorFileArray.eventAnalConfig.Termial = _wtof(strTermial);

		CString strBackScatter = IniUtil::ReadSingleConfigInfo(_T("EventAnal"), _T("BackScatter"));
		g_sorFileArray.eventAnalConfig.BackScatter = _wtof(strBackScatter);

		CString strBackScatter1550 = IniUtil::ReadSingleConfigInfo(_T("EventAnal"), _T("BackScatter1550"));
		g_sorFileArray.eventAnalConfig.BackScatter1550 = _wtof(strBackScatter1550);

		CString strBackScatter1625 = IniUtil::ReadSingleConfigInfo(_T("EventAnal"), _T("BackScatter1625"));//zll 2013.5.10 晚
		g_sorFileArray.eventAnalConfig.BackScatter1625 = _wtof(strBackScatter1625);

		CString strRefraction = IniUtil::ReadSingleConfigInfo(_T("EventAnal"), _T("Refraction"));
		g_sorFileArray.eventAnalConfig.Refraction = _wtof(strRefraction);

		CString strRefraction1550 = IniUtil::ReadSingleConfigInfo(_T("EventAnal"), _T("Refraction1550"));
		g_sorFileArray.eventAnalConfig.Refraction1550 = _wtof(strRefraction1550);

		CString strRefraction1625 = IniUtil::ReadSingleConfigInfo(_T("EventAnal"), _T("Refraction1625"));//zll 2013.5.10 晚
		g_sorFileArray.eventAnalConfig.Refraction1625 = _wtof(strRefraction1625);
	}

	BOOL CMainFrame::CreateToolBar()
	{
		if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT|TBSTYLE_TOOLTIPS, WS_CHILD | WS_VISIBLE | CBRS_TOP
			| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
			!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
		{
			TRACE0("Failed to create toolbar\n");
			return FALSE;      // fail to create
		}

		m_wndToolBar.LoadTrueColorToolBar(32,IDB_TOOLBAR_NORMAL,IDB_TOOLBAR_NORMAL,IDB_TOOLBAR_DISABLE);//加载自制的工具条

		//************设置工具条尺寸
		CRect rc(0,0,0,0);
		CSize sizeMax(0,0);
		CToolBarCtrl& bar=m_wndToolBar.GetToolBarCtrl();
		for(int nIndex=bar.GetButtonCount()-1;nIndex>=0;nIndex--)
		{
			bar.GetItemRect(nIndex,rc);

			rc.NormalizeRect();
			sizeMax.cx=__max(rc.Size().cx,sizeMax.cx);
			sizeMax.cy=__max(rc.Size().cy,sizeMax.cy);
		}
		m_wndToolBar.SetSizes(sizeMax,CSize(32,32));

		// TODO: Delete these three lines if you don't want the toolbar to be dockable
		m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
		EnableDocking(CBRS_ALIGN_ANY);
		DockControlBar(&m_wndToolBar);

		return TRUE;

	}

	BOOL CMainFrame::CreateStatusBar()
	{
		//if (!m_wndStatusBar.Create(this) ||
		//	!m_wndStatusBar.SetIndicators(indicators,
		//	sizeof(indicators)/sizeof(UINT)))
		//{
		//	TRACE0("Failed to create status bar\n");
		//	return FALSE;      // fail to create
		//}//以上 zll 2012.10.24注释 修改为需要的 如下
		if (!m_wndStatusBar.Create(this))
		{
			TRACE0("Failed to create status bar\n");
			return FALSE;      // fail to create
		}
		int int_count[2]={800,-1};
		m_wndStatusBar.GetStatusBarCtrl().SetParts(2,int_count);
		m_wndStatusBar.GetStatusBarCtrl().SetText(_T("Status"),1,0);

		return TRUE;
	}

	BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
	{
		cs.style |=WS_MAXIMIZE;
		cs.x=0;
		cs.y=0;
		cs.cx=::GetSystemMetrics(SM_CXSCREEN);
		cs.cy=::GetSystemMetrics(SM_CYSCREEN)-30;
		cs.style &=~FWS_ADDTOTITLE;//不要-XYZ这样的标题

		if( !CMDIFrameWnd::PreCreateWindow(cs) )
			return FALSE;
		// TODO: Modify the Window class or styles here by modifying
		//  the CREATESTRUCT cs

		return TRUE;
	}


	// CMainFrame diagnostics

#ifdef _DEBUG
	void CMainFrame::AssertValid() const
	{
		CMDIFrameWnd::AssertValid();
	}

	void CMainFrame::Dump(CDumpContext& dc) const
	{
		CMDIFrameWnd::Dump(dc);
	}

#endif //_DEBUG


	// CMainFrame message handlers

	void CMainFrame::OnPaint() 
	{
		CPaintDC dc(this); // device context for painting
		TileViewFrame();//自定义平铺框架布局//wcq
	}

	void CMainFrame::OnRemoteConnect()
	{
		if (m_pRemoteConnDlg)
		{
			m_pRemoteConnDlg->ShowWindow(SW_SHOW);
			m_pRemoteConnDlg->CenterWindow();
		}
	}



	void CMainFrame::OnRemoteSet()
	{
		if (NULL == m_pRemotePageSheet)
			m_pRemotePageSheet = new CRemotePageSheet(_T("报表向导"), this);
		m_pRemotePageSheet->DoModal();
	}

	void CMainFrame::OnRemoteControl()
	{
		
	}

	void CMainFrame::OnSwitchChs()
	{
		g_currentLang = LANG::CHS;

		SetLocaleString();
		//保存语言设置
		IniUtil::SaveLangSetting();
	}

	void CMainFrame::OnSwitchEng()
	{
		g_currentLang = LANG::ENG;

		SetLocaleString();
		//保存语言设置
		IniUtil::SaveLangSetting();
	}

	void CMainFrame::OnSwitchPort()
	{
		g_currentLang = LANG::PORT;

		SetLocaleString();
		//保存语言设置
		IniUtil::SaveLangSetting();
	}

	void CMainFrame::OnEventEditMode()
	{

	}

	void CMainFrame::OnUpdateEventEditMode(CCmdUI *pCmdUI)
	{
		if(!m_bReportMode && m_bFileMenuEnable)
			pCmdUI-> Enable(TRUE);
		else
			pCmdUI-> Enable(FALSE);
	}

	void CMainFrame::OnEventAutoSearch()
	{
		//m_pWaveFormView->m_sorDrawStatic.m_vAllData.clear();
		int temp=-1;
		std::vector<CGraphElement>::iterator it=m_pWaveFormView->m_sorDrawStatic.m_vAllData.begin();
		for (;it!=m_pWaveFormView->m_sorDrawStatic.m_vAllData.end();it++)
		{
			temp++;
			if (g_sorFileArray.curSelection==temp)
			{
				m_pWaveFormView->m_sorDrawStatic.m_vAllData.erase(it);
				break;
			}
		}
		TestResultInfo* pTestResultInfo;
		pTestResultInfo=m_pEventFormView->GetEventList();
		m_sorAssist.ReEventAutoSearch(*pTestResultInfo,m_sorDATA,&m_pWaveFormView->m_sorDrawStatic,pDataSmooth,colorLine,g_sorFileArray.curSelection);//zll 2013.6.14
		m_pWaveFormView->Invalidate();// 重绘曲线
		m_pEventFormView->FreshEventList();//刷新事件列表
	}

	void CMainFrame::OnUpdateEventAutoSearch(CCmdUI *pCmdUI)
	{
		if(!m_bReportMode && m_bFileMenuEnable)
			pCmdUI-> Enable(TRUE);
		else
			pCmdUI-> Enable(FALSE);
	}

	void CMainFrame::OnEventAnalCon()
	{
		CEventSearchPageSheet configSheet(_T("设置"));
		configSheet.DoModal();
	}

	void CMainFrame::OnUpdateEventAnalCon(CCmdUI *pCmdUI)
	{
		if(!m_bReportMode && m_bFileMenuEnable)
			pCmdUI-> Enable(TRUE);
		else
			pCmdUI-> Enable(FALSE);
	}

	void CMainFrame::OnViewWave()
	{
		//隐藏或显示波形视图
		m_bShowWaveForm = !m_bShowWaveForm;
		m_cfWaveForm->ShowWindow(m_bShowWaveForm);
		if (m_bShowWaveForm)
			GetMenu()->CheckMenuItem(ID_VIEW_WAVE, MF_BYCOMMAND|MF_CHECKED);
		else 
			GetMenu()->CheckMenuItem(ID_VIEW_WAVE, MF_BYCOMMAND|MF_UNCHECKED);
	}

	void CMainFrame::OnViewEvent()
	{
		//隐藏或显示事件列表视图
		m_bShowEventList = !m_bShowEventList;
		m_cfEventList->ShowWindow(m_bShowEventList);
		if (m_bShowEventList)
			GetMenu()->CheckMenuItem(ID_VIEW_EVENTLIST, MF_BYCOMMAND|MF_CHECKED);
		else 
			GetMenu()->CheckMenuItem(ID_VIEW_EVENTLIST, MF_BYCOMMAND|MF_UNCHECKED);
	}


	/*void CMainFrame::SetAllowLayout(BOOL Allow)
	{
	Istilelayout=Allow;
	}*/

	/*void CMainFrame::OnUsbLink()
	{
	}*/

	void CMainFrame::OnWaveform()
	{

	}

	void CMainFrame::OnReportWizard()
	{
		//删除当前选择文件
		if (g_sorFileArray.totalCurveNumber > 0)
		{
			CString str_delete_editedview=IniUtil::ReadResString(_T("IDS_DELETE_EDITEDVIEW"));
			int nRet = MessageBox(str_delete_editedview, _T("OTDR Viewer"),  MB_OKCANCEL|MB_ICONQUESTION);
			if (nRet != IDOK)
				return;

			/////zll 2013.5.12
            m_pWaveFormView->m_sorDrawStatic.m_vAllData.clear();
			m_pWaveFormView->m_sorDrawStatic.DrawCurves(&m_pWaveFormView->m_sorDrawStatic.m_dcCurve,-1);
			m_pWaveFormView->m_sorDrawStatic.Invalidate();

			g_sorFileArray.Reset();
			MAIN->m_pWaveFormView->m_sorDrawStatic.m_vAllData.clear();//zll 2013.7.4 开始报表向导时必须清空m_vAllData
			//刷新事件列表
			m_pEventFormView->FreshEventList();
			//清空文件列表
			m_pWaveFormView->PopulateData();
		}

		
		if(g_currentLang == LANG::CHS) 
		{
			InitReportConfig();
			InitRepLayListTitle();
		}
		else if(g_currentLang == LANG::ENG)
		{
			InitReportConfig_EN();
		    InitRepLayListTitle_EN();//初始化ReportConfig_EN
		}
		else if(g_currentLang == LANG::PORT)
		{
			InitReportConfig_PT();
			InitRepLayListTitle_PT();//初始化ReportConfig_PT
		}

		m_bFileMenuEnable = FALSE;
		m_bReportMode = TRUE;
		//显示报表
		ShowReport(TRUE);
	}

	void CMainFrame::OnHelpContent()
	{
		//判断中英文
		CString strHelpFile;
		if (g_currentLang == LANG::CHS)
		{
			strHelpFile = HELP_FILE_CHS;
		}
		else if (g_currentLang == LANG::ENG)//ZLL 2012.7.5 LANG::CHS改为LANG::ENG
		{
			strHelpFile = HELP_FILE_ENG;
		}
		else if(g_currentLang == LANG::PORT)  //lzy 2014.6.15 葡语版帮助文档暂时没有，用英文代替
		{
			strHelpFile = HELP_FILE_ENG;
		}
		CString strFullFile;
		strFullFile.Format(_T("%s\\%s"),CUtil::getExePath(), strHelpFile);
		BOOL bExist = CUtil::isFileExist(strFullFile);
		if(bExist)
			::ShellExecute(NULL,_T("open"), strFullFile, NULL, NULL, SW_SHOW);
	}

	void CMainFrame::OnSceenSet()
	{
		CConfigSheet configSheet(_T("设置"));
		configSheet.DoModal();
	}

	void CMainFrame::OnOriginSetting()
	{
		m_bShowWaveForm = SW_SHOWNORMAL;
		CRect rect;
		m_cfWaveForm->GetWindowRect(&rect);
		m_cfWaveForm->MoveWindow(&m_formviewRect,TRUE);
		m_cfWaveForm->ShowWindow(m_bShowWaveForm);
		//正常显示waveform
		m_pWaveFormView->SetMainSettingMode(TRUE);
		GetMenu()->CheckMenuItem(ID_VIEW_WAVE, MF_BYCOMMAND|MF_CHECKED);

		m_bShowEventList = SW_SHOWNORMAL;
		m_cfEventList->MoveWindow(&m_eventlistRect,TRUE);
		m_cfEventList->ShowWindow(m_bShowEventList);
		GetMenu()->CheckMenuItem(ID_VIEW_EVENTLIST, MF_BYCOMMAND|MF_CHECKED);

		m_bShowThumbnail = SW_SHOWNORMAL;
		m_cfThumbnail->MoveWindow(&m_thumbnailRect,TRUE);
		m_cfThumbnail->ShowWindow(m_bShowThumbnail);
		GetMenu()->CheckMenuItem(ID_VIEW_THUMBNAIL, MF_BYCOMMAND|MF_CHECKED);
	}

	void CMainFrame::OnViewThumbnail()
	{
		//隐藏或显示缩略图
		m_bShowThumbnail = !m_bShowThumbnail;
		m_cfThumbnail->ShowWindow(m_bShowThumbnail);
		if (m_bShowThumbnail)
			GetMenu()->CheckMenuItem(ID_VIEW_THUMBNAIL, MF_BYCOMMAND|MF_CHECKED);
		else 
			GetMenu()->CheckMenuItem(ID_VIEW_THUMBNAIL, MF_BYCOMMAND|MF_UNCHECKED);

		CMenu* pMenu = GetMenu();
	}

	void CMainFrame::OnUpdateMenu(CCmdUI *pCmdUI)
	{	
		/////////// zll 2013.6.19 防止按下ID_OPEN后ID_SAVE和ID_PRINT的Tip信息消失
		CToolBarCtrl &bb=m_wndToolBar.GetToolBarCtrl();
		CToolTipCtrl* aa=bb.GetToolTips();
		if(g_currentLang == LANG::CHS) 
		{
			//更改CToolBar类各按钮的Tip中文提示信息
			aa->UpdateTipText(_T("保存轨迹文件"),&m_wndToolBar,ID_SAVE);
			aa->UpdateTipText(_T("打印"),&m_wndToolBar,ID_PRINT);
		}
		else if(g_currentLang == LANG::ENG)
		{
			//更改CToolBar类各按钮的Tip英文提示信息
			aa->UpdateTipText(_T("Save File"),&m_wndToolBar,ID_SAVE);
			aa->UpdateTipText(_T("Print"),&m_wndToolBar,ID_PRINT);
		}

		else if(g_currentLang == LANG::PORT)
		{
			aa->UpdateTipText(_T("Salve o arquivo"),&m_wndToolBar,ID_SAVE);
			aa->UpdateTipText(_T("Impressão"),&m_wndToolBar,ID_PRINT);
		}
		
		/////////////

		if(!m_bReportMode)
			pCmdUI-> Enable(TRUE);
		else
			pCmdUI-> Enable(FALSE);
	}

	void CMainFrame::OnEventlist()
	{
		/*if(cf!=NULL)
		{
		cf->MDIActivate();
		return;
		}*/
		////cf=new CChildFrame();
		////CCreateContext cc;
		////cc.m_pNewViewClass=RUNTIME_CLASS(CEventlist);
		////if(!cf->LoadFrame(IDR_OTDRTYPE,WS_MAXIMIZE|WS_OVERLAPPEDWINDOW,this,&cc))
		////	return;
		////cf->MoveWindow(0,365,1024,260,TRUE);
		////cf->ShowWindow(1);
		//cf->InitialUpdateFrame(NULL,true);
	}
	//打开文件
	void CMainFrame::OnOpen()
	{
		CString strMsg;
		int index=-1;
		if (g_sorFileArray.totalCurveNumber<=7)
		{
			index = GetNextCurveNum(g_sorFileArray);
		}
		if (g_sorFileArray.totalCurveNumber>7)
		{
			if (LB_ERR == index)//全部都加载过了
			{
				MessageBox(str_max_curve, _T("OTDR Viewer"), MB_OK|MB_ICONEXCLAMATION);
				return;
			}
		}

		CFileDialog dlg(TRUE,_T("*.SOR"), NULL,OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, _T("SOR (*.SOR)|*.SOR||"));  //可多选，lzy2014.9.17
		//CFileDialog dlg(TRUE,_T("*.SOR"), NULL,OFN_HIDEREADONLY , _T("SOR (*.SOR)|*.SOR||"));  //单选

		CString path;
		path=m_reg.readKey(REPORT_RPT_PATH_REG);
		if(""==path)//首次打开曲线，则路径指向软件安装所在的路径 zll 2012.10.30
		{
			path=util.getExePath()+REPORT_RPT_DEFAULT_PATH;
		}
		dlg.m_ofn.lpstrInitialDir=path;
		
		if(dlg.DoModal() == IDOK)  //打开所有选中的曲线，最多8条		  
		{
			CString strFileName, strFullPath;
			CStringArray strArrPath, strArrFile;
			int nPos = -1;
			POSITION pos = dlg.GetStartPosition();
			//读取多个文件
			while(pos != NULL)
			{
				strFullPath = dlg.GetNextPathName(pos);
				nPos = strFullPath.ReverseFind(_T('\\'));
				strFileName = strFullPath.Right(strFullPath.GetLength() - nPos - 1);
				strArrFile.Add(strFileName);
				strArrPath.Add(strFullPath);
			}
			
			//加载所有文件
			for(int i=0; i<strArrFile.GetCount(); i++)
			{				
				if(i+index > 7)
				{
					break;
				}
				LoadOneCurve(strArrPath[i], strArrFile[i], i+index);				
			}
		}
		else
		{ 			
			return;
		} 
		
		//保存一下选择的路径
		if (path!=dlg.m_ofn.lpstrFile)
			m_reg.writeKey(REPORT_RPT_PATH_REG,dlg.m_ofn.lpstrFile);//软件安装路径与当前打开曲线的路径不同，则保存当前路径
		else
			m_reg.writeKey(REPORT_RPT_PATH_REG,path);////软件安装路径与当前打开曲线的路径相同，则保存软件安装路径
			
		//CString strFilePath=dlg.GetPathName();//得到文件名
		//CString strFileName=dlg.GetFileName();//路径加文件名
		////判断文件是否已经加载
		//if (IsFileAlreadyOpen(strFilePath))
		//{	
		//	return;
		//}
		////读取文件
		//CSorFile sorFile;
		//
		//INT nRes = 0;
		//TestResultInfo* pTestResultInfo;
		//try
		//{
		//	//std::vector<Data>::size_type x = m_sorDATA.vAllData.size();
		//	nRes = sorFile.readSorFile(strFileName, m_sorDATA);
		//	m_sorAssist.convert(m_sorDATA,*g_sorFileArray.sorFileList[index].pTestResultInfo);
		//	m_sorDrawStatic.EventToGaborResult(g_sorFileArray.sorFileList[index].pTestResultInfo); 
		//	pTestResultInfo=g_sorFileArray.sorFileList[index].pTestResultInfo;
		//	g_sorFileArray.sorFileList[index].pTestResultInfo->DataSpacing = m_sorDrawStatic.Get_XCoord2nf(1,g_sorFileArray.sorFileList[index].pTestResultInfo);
		//}
		//catch (CException* e)
		//{
		//	nRes = 0;
		//}
		//if(NULL==m_sorDATA.vAllData.size())
		//{
		//	MessageBox(str_open_data_invalid, _T("OTDR Viewer"), MB_OK|MB_ICONEXCLAMATION);
		//	return;
		//}
		//if (1 == nRes)
		//{
		//	g_sorFileArray.sorFileList[index].bValid = TRUE;
		//	g_sorFileArray.sorFileList[index].fileName = strFileName;
		//	g_sorFileArray.sorFileList[index].filePathName = strFilePath;
		//	g_sorFileArray.totalCurveNumber++;
		//	//设置曲线颜色
		//	g_sorFileArray.sorFileList[index].pTestResultInfo->curveColor = GetCurveColor(g_sorFileArray.totalCurveNumber);
		//	g_sorFileArray.curSelection = index;//设为当前选中行号
		//}
		//else//出错，进行错误提示????????????????????????
		//{
		//	MessageBox(str_open_file_error, _T("OTDR Viewer"), MB_OK|MB_ICONEXCLAMATION);
		//	return; //
		//}

		////以下判断一下数据的最大值，来修改Y轴的范围,lzy2014.6.23
		//int iYmax = m_sorAssist.GetTraceYmax(m_sorDATA);
		//int iChange = abs(iYmax * 1.2 - g_nTraceYmax) / 5.0;    //超出几个单位的5dB		
		//if(!index) //第一条曲线，需要作缩小范围的修改
		//{
		//	if(iYmax * 1.2 > g_nTraceYmax)
		//		g_nTraceYmax += (1 + iChange) * 5;
		//	else
		//		g_nTraceYmax -= iChange * 5;
		//}
		//else  //其余的曲线如果范围扩大则修改
		//{
		//	if(iYmax * 1.2 > g_nTraceYmax)
		//		g_nTraceYmax += (1 + iChange) * 5;
		//}
		//if(g_nTraceYmax < OTDR_TRACE_Y_MAX)
		//	g_nTraceYmax = OTDR_TRACE_Y_MAX;   //设定最小范围
		//		
		//m_pWaveFormView->PopulateData();
		////填充事件列表
		//m_pEventFormView->FreshEventList();
		////填充曲线数据
		//m_sorAssist.AddSordataToOtdrTrace(m_sorDATA,&m_pWaveFormView->m_sorDrawStatic);//zll 2013.5.7	

		///////////////////////////zll 2013.7.1
		//if (g_sorFileArray.waveConfig.MultiMode == FALSE)
		//{
		//	for(int i=0;i<m_pWaveFormView->m_sorDrawStatic.m_vAllData.size();i++)
		//	{
		//		if (m_pWaveFormView->m_sorDrawStatic.m_nCurveIndex!=i)
		//		{
		//			m_pWaveFormView->m_sorDrawStatic.SetShowCurve(i,FALSE);
		//		}
		//	}		
		//}
		//else
		//{
		//	for(int i=0;i<m_pWaveFormView->m_sorDrawStatic.m_vAllData.size();i++)
		//	{
		//		m_pWaveFormView->m_sorDrawStatic.SetShowCurve(i,TRUE);
		//	}
		//}
		/////////////////////////////////////

		//m_pWaveFormView->m_sorDrawStatic.ReCalMaxDistance();
		//m_pWaveFormView->m_sorDrawStatic.SetMaxDistance(m_pWaveFormView->m_sorDrawStatic.GetMaxDistance());
		//MAIN->m_pWaveFormView->m_spinOffset.SetPos(0.0f);//zll 2013.7.2
		//InvalidateRect(FALSE);
		//m_pThumbFormView->GetAllThumbData();//zll 2013.5.10 晚
		//m_pThumbFormView->Invalidate(); //redraw thumbnail

		////设置横坐标单位文本
		//m_pWaveFormView->SetHorUnitText();
		////设置纵坐标单位文本
		//m_pWaveFormView->SetVerUnitText();

		////激活保存和打印菜单
		//m_bFileMenuEnable = TRUE;
	}

	//加载一条曲线， lzy 2015.2.10
	void CMainFrame::LoadOneCurve(CString strFilePath, CString strFileName, int index)
	{
		//CString strFilePath=dlg.GetPathName();//得到文件名
		//CString strFileName=dlg.GetFileName();//路径加文件名
		//判断文件是否已经加载
		if (IsFileAlreadyOpen(strFilePath))
		{	
			return;
		}
		//读取文件
		CSorFile sorFile;

		INT nRes = 0;
		TestResultInfo* pTestResultInfo;
		try
		{
			//std::vector<Data>::size_type x = m_sorDATA.vAllData.size();
			nRes = sorFile.readSorFile(strFileName, m_sorDATA);//
			m_sorAssist.convert(m_sorDATA,*g_sorFileArray.sorFileList[index].pTestResultInfo);
			m_sorDrawStatic.EventToGaborResult(g_sorFileArray.sorFileList[index].pTestResultInfo); 
			pTestResultInfo=g_sorFileArray.sorFileList[index].pTestResultInfo;
			g_sorFileArray.sorFileList[index].pTestResultInfo->DataSpacing = m_sorDrawStatic.Get_XCoord2nf(1,g_sorFileArray.sorFileList[index].pTestResultInfo);
		}
		catch (CException* e)
		{
			nRes = 0;
		}
		if(NULL==m_sorDATA.vAllData.size())
		{
			MessageBox(str_open_data_invalid, _T("OTDR Viewer"), MB_OK|MB_ICONEXCLAMATION);
			return;
		}
		if (1 == nRes)
		{
			g_sorFileArray.sorFileList[index].bValid = TRUE;
			g_sorFileArray.sorFileList[index].fileName = strFileName;
			g_sorFileArray.sorFileList[index].filePathName = strFilePath;
			g_sorFileArray.totalCurveNumber++;
			//设置曲线颜色
			g_sorFileArray.sorFileList[index].pTestResultInfo->curveColor = GetCurveColor(g_sorFileArray.totalCurveNumber);
			g_sorFileArray.curSelection = index;//设为当前选中行号
		}
		else//出错，进行错误提示????????????????????????
		{
			MessageBox(str_open_file_error, _T("OTDR Viewer"), MB_OK|MB_ICONEXCLAMATION);
			return; //
		}

		//以下判断一下数据的最大值，来修改Y轴的范围,lzy2014.6.23
		int iYmax = m_sorAssist.GetTraceYmax(m_sorDATA);
		int iChange = abs(iYmax * 1.2 - g_nTraceYmax) / 5.0;    //超出几个单位的5dB		
		if(!index) //第一条曲线，需要作缩小范围的修改
		{
			if(iYmax * 1.2 > g_nTraceYmax)
				g_nTraceYmax += (1 + iChange) * 5;
			else
				g_nTraceYmax -= iChange * 5;
		}
		else  //其余的曲线如果范围扩大则修改
		{
			if(iYmax * 1.2 > g_nTraceYmax)
				g_nTraceYmax += (1 + iChange) * 5;
		}
		if(g_nTraceYmax < OTDR_TRACE_Y_MAX)
			g_nTraceYmax = OTDR_TRACE_Y_MAX;   //设定最小范围

		m_pWaveFormView->PopulateData();
		//填充事件列表
		m_pEventFormView->FreshEventList();
		//填充曲线数据
		m_sorAssist.AddSordataToOtdrTrace(m_sorDATA,&m_pWaveFormView->m_sorDrawStatic);//zll 2013.5.7	

		/////////////////////////zll 2013.7.1
		if (g_sorFileArray.waveConfig.MultiMode == FALSE)
		{
			for(int i=0;i<m_pWaveFormView->m_sorDrawStatic.m_vAllData.size();i++)
			{
				if (m_pWaveFormView->m_sorDrawStatic.m_nCurveIndex!=i)
				{
					m_pWaveFormView->m_sorDrawStatic.SetShowCurve(i,FALSE);
				}
			}		
		}
		else
		{
			for(int i=0;i<m_pWaveFormView->m_sorDrawStatic.m_vAllData.size();i++)
			{
				m_pWaveFormView->m_sorDrawStatic.SetShowCurve(i,TRUE);
			}
		}
		///////////////////////////////////

		m_pWaveFormView->m_sorDrawStatic.ReCalMaxDistance();
		m_pWaveFormView->m_sorDrawStatic.SetMaxDistance(m_pWaveFormView->m_sorDrawStatic.GetMaxDistance());
		MAIN->m_pWaveFormView->m_spinOffset.SetPos(0.0f);//zll 2013.7.2
		InvalidateRect(FALSE);
		m_pThumbFormView->GetAllThumbData();//zll 2013.5.10 晚
		m_pThumbFormView->Invalidate(); //redraw thumbnail

		//设置横坐标单位文本
		m_pWaveFormView->SetHorUnitText();
		//设置纵坐标单位文本
		m_pWaveFormView->SetVerUnitText();

		//激活保存和打印菜单
		m_bFileMenuEnable = TRUE;
	}

	void CMainFrame::OnSave()
	{
		CFileDialog dlg(FALSE,_T("*.SOR"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("SOR (*.SOR)|*.SOR||"));
		if(dlg.DoModal() != IDOK)
			return;

		CString strFilePath=dlg.GetPathName();//得到文件名
		CString strFileName=dlg.GetFileName();//路径加文件名

		TestResultInfo* pFileInfo = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;

		//保存文件
		CSorFile sorFile;
		m_sorAssist.convert(pFileInfo,m_sorDATA);
		if (1 == sorFile.saveSorFile(strFileName,m_sorDATA))
		{
		}
		else//出错，进行错误提示????????????????????????
		{

		}
	}

	void CMainFrame::OnUpdateSave(CCmdUI *pCmdUI)
	{
		if(!m_bReportMode && m_bFileMenuEnable)
			pCmdUI-> Enable(TRUE);
		else
			pCmdUI-> Enable(FALSE);
	}

	void CMainFrame::OnPageSetup()
	{
		CString strPageSet=IniUtil::ReadResString(_T("IDS_PRINT_SETUP"));
		CPrintSheet printSheet(strPageSet);
		printSheet.DoModal();
	}

	void CMainFrame::OnPrint()
	{
		//Command* pCommand =  m_commandManager.GetCommandByID(Commands::FILE_PRINT);
		//if (pCommand)
		//	pCommand->Execute();
		{
			if(m_pPrintView)
			{
				m_pPrintView->OnMyPrint();
			}
		}
	}

	void CMainFrame::OnUpdatePrint(CCmdUI *pCmdUI)
	{
		if(!m_bReportMode && m_bFileMenuEnable)
			pCmdUI-> Enable(TRUE);
		else
			pCmdUI-> Enable(FALSE);
	}

	void CMainFrame::OnPrintPreview()
	{
		if (m_pPrintView)
		{
			m_pPrintView->OnMyPrintPreview();
		}
	}

	void CMainFrame::OnUpdatePrintPreview(CCmdUI *pCmdUI)
	{
		if(!m_bReportMode && m_bFileMenuEnable)
			pCmdUI-> Enable(TRUE);
		else
			pCmdUI-> Enable(FALSE);
	}

	void CMainFrame::OnClose()
	{
		int nRet = 0;
		nRet = MessageBox(str_OkorCancel, _T("OTDR Viewer"),  MB_OKCANCEL|MB_ICONQUESTION);
		if (nRet == IDOK)
		{
			if(g_currentLang == LANG::CHS) 
				RecordConfig();
			else if(g_currentLang == LANG::ENG)
			    RecordConfig_EN();//保存配置信息
			else if(g_currentLang == LANG::PORT)
				RecordConfig_PT();
			CMDIFrameWnd::OnClose();
		}
	}

	void CMainFrame::RecordConfig()
	{
		//保存设置信息
		/*                                 OTDR设备信息                                                 */
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("Language"),g_sorFileArray.otdrConfig.strLang);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("LightLevel"),g_sorFileArray.otdrConfig.strLightLevel);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("AutoPowerOff"),g_sorFileArray.otdrConfig.strAutoPowerOff	);
		//IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("Date"),g_sorFileArray.otdrConfig.strDate);
		//IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("Time"),g_sorFileArray.otdrConfig.strTime);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("TouchSreeenStatus"),g_sorFileArray.otdrConfig.strTouchSreeenStatus);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("TestWave"),g_sorFileArray.otdrConfig.strTestWave);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("TestTime"),g_sorFileArray.otdrConfig.strTestTime);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("TestMode"),g_sorFileArray.otdrConfig.strTestMode);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("TestRange"),g_sorFileArray.otdrConfig.strTestRange);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("TestUnit"),g_sorFileArray.otdrConfig.strUnit);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("PluseWidth"),g_sorFileArray.otdrConfig.strPluseWidth);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("strTestRange"),g_sorFileArray.otdrConfig.strTestRange);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("Resolution"),g_sorFileArray.otdrConfig.strResolution);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("AutoSave"),g_sorFileArray.otdrConfig.strAutoSave);

		/*                                          报表配置数据                                        */
		//step1
		IniUtil::WriteSingleConfigInfo(_T("RepLayListLay"), _T("ListFormat"),g_sorFileArray.reportConfig.ListFormat);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay"), _T("Print"),g_sorFileArray.reportConfig.Print);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay"), _T("Direction"),g_sorFileArray.reportConfig.Direction);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay"), _T("WaveList"),g_sorFileArray.reportConfig.WaveList);
		//step2
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("Heading"),g_sorFileArray.reportConfig.Title);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("Item1"),g_sorFileArray.reportConfig.Item1);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("Edit1"),g_sorFileArray.reportConfig.Item1Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("Item2"),g_sorFileArray.reportConfig.Item2);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("Edit2"),g_sorFileArray.reportConfig.Item2Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("Item3"),g_sorFileArray.reportConfig.Item3);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("Edit3"),g_sorFileArray.reportConfig.Item3Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("Item4"),g_sorFileArray.reportConfig.Item4);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("Edit4"),g_sorFileArray.reportConfig.Item4Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("Item5"),g_sorFileArray.reportConfig.Item5);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("Edit5"),g_sorFileArray.reportConfig.Item5Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("Item6"),g_sorFileArray.reportConfig.Item6);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("Edit6"),g_sorFileArray.reportConfig.Item6Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("Item7"),g_sorFileArray.reportConfig.Item7);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("Edit7"),g_sorFileArray.reportConfig.Item7Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("Item8"),g_sorFileArray.reportConfig.Item8);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("Edit8"),g_sorFileArray.reportConfig.Item8Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("Item9"),g_sorFileArray.reportConfig.Item9);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("Edit9"),g_sorFileArray.reportConfig.Item9Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("Item10"),g_sorFileArray.reportConfig.Item10);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("Edit10"),g_sorFileArray.reportConfig.Item10Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("Item11"),g_sorFileArray.reportConfig.Item11);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("Edit11"),g_sorFileArray.reportConfig.Item11Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("Item12"),g_sorFileArray.reportConfig.Item12);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("Edit12"),g_sorFileArray.reportConfig.Item12Val);

		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox1"),g_sorFileArray.reportConfig.Combox1);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox2"),g_sorFileArray.reportConfig.Combox2);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox3"),g_sorFileArray.reportConfig.Combox3);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox4"),g_sorFileArray.reportConfig.Combox4);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox5"),g_sorFileArray.reportConfig.Combox5);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox6"),g_sorFileArray.reportConfig.Combox6);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox7"),g_sorFileArray.reportConfig.Combox7);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox8"),g_sorFileArray.reportConfig.Combox8);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox9"),g_sorFileArray.reportConfig.Combox9);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox10"),g_sorFileArray.reportConfig.Combox10);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox11"),g_sorFileArray.reportConfig.Combox11);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox12"),g_sorFileArray.reportConfig.Combox12);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox13"),g_sorFileArray.reportConfig.Combox13);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox14"),g_sorFileArray.reportConfig.Combox14);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox15"),g_sorFileArray.reportConfig.Combox15);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle"), _T("ComboBox16"),g_sorFileArray.reportConfig.Combox16);
		//Step3
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay"), _T("NumberOfWaveLength"),g_sorFileArray.reportConfig.NumberOfWaveLength);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay"), _T("MeasureDir"),g_sorFileArray.reportConfig.MeasureDir);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay"), _T("OutputSort"),g_sorFileArray.reportConfig.OutputSort);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay"), _T("EventSort"),g_sorFileArray.reportConfig.EventSort);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay"), _T("UpDownSort"),g_sorFileArray.reportConfig.UpDownSort);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay"), _T("FileFormat"),g_sorFileArray.reportConfig.FileFormat);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay"), _T("NumberOfFiber"),g_sorFileArray.reportConfig.NumberOfFiber);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay"), _T("StartOfFiberNo"),g_sorFileArray.reportConfig.StartOfFiberNo);

		IniUtil::WriteSingleConfigInfo(_T("RepLayListTotalList"), _T("Add"), g_sorFileArray.reportConfig.Add);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListLay"), _T("TotalLossOnOff"), g_sorFileArray.reportConfig.TotalLossOnOff);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListLay"), _T("AddAbnormality"), g_sorFileArray.reportConfig.AddAbnormality);
		//Step4
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay"), _T("Folder1"),g_sorFileArray.reportConfig.Folder1);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay"), _T("Folder2"),g_sorFileArray.reportConfig.Folder2);
		IniUtil::WriteSingleConfigInfo(_T("RepLayFileSelect"), _T("SetOptionSort"),g_sorFileArray.reportConfig.SetOptionSort);
		IniUtil::WriteSingleConfigInfo(_T("FileSelOption"), _T("BeAdapted"),g_sorFileArray.reportConfig.BeAdapted);
		IniUtil::WriteSingleConfigInfo(_T("FileSelOption"), _T("WLIndex"),g_sorFileArray.reportConfig.WLIndex);
	}

	void CMainFrame::RecordConfig_EN()
	{
		//保存设置信息
		/*                                 OTDR设备信息                                                 */
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("Language"),g_sorFileArray.otdrConfig.strLang);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("LightLevel"),g_sorFileArray.otdrConfig.strLightLevel);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("AutoPowerOff"),g_sorFileArray.otdrConfig.strAutoPowerOff	);
		//IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("Date"),g_sorFileArray.otdrConfig.strDate);
		//IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("Time"),g_sorFileArray.otdrConfig.strTime);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("TouchSreeenStatus"),g_sorFileArray.otdrConfig.strTouchSreeenStatus);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("TestWave"),g_sorFileArray.otdrConfig.strTestWave);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("TestTime"),g_sorFileArray.otdrConfig.strTestTime);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("TestMode"),g_sorFileArray.otdrConfig.strTestMode);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("TestRange"),g_sorFileArray.otdrConfig.strTestRange);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("TestUnit"),g_sorFileArray.otdrConfig.strUnit);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("PluseWidth"),g_sorFileArray.otdrConfig.strPluseWidth);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("strTestRange"),g_sorFileArray.otdrConfig.strTestRange);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("Resolution"),g_sorFileArray.otdrConfig.strResolution);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("AutoSave"),g_sorFileArray.otdrConfig.strAutoSave);

		/*                                          报表配置数据                                        */
		//step1
		IniUtil::WriteSingleConfigInfo(_T("RepLayListLay_EN"), _T("ListFormat"),g_sorFileArray.reportConfig.ListFormat);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay_EN"), _T("Print"),g_sorFileArray.reportConfig.Print);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay_EN"), _T("Direction"),g_sorFileArray.reportConfig.Direction);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay"), _T("WaveList"),g_sorFileArray.reportConfig.WaveList);
		//step2
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Heading"),g_sorFileArray.reportConfig.Title);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item1"),g_sorFileArray.reportConfig.Item1);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit1"),g_sorFileArray.reportConfig.Item1Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item2"),g_sorFileArray.reportConfig.Item2);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit2"),g_sorFileArray.reportConfig.Item2Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item3"),g_sorFileArray.reportConfig.Item3);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit3"),g_sorFileArray.reportConfig.Item3Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item4"),g_sorFileArray.reportConfig.Item4);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit4"),g_sorFileArray.reportConfig.Item4Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item5"),g_sorFileArray.reportConfig.Item5);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit5"),g_sorFileArray.reportConfig.Item5Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item6"),g_sorFileArray.reportConfig.Item6);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit6"),g_sorFileArray.reportConfig.Item6Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item7"),g_sorFileArray.reportConfig.Item7);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit7"),g_sorFileArray.reportConfig.Item7Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item8"),g_sorFileArray.reportConfig.Item8);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit8"),g_sorFileArray.reportConfig.Item8Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item9"),g_sorFileArray.reportConfig.Item9);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit9"),g_sorFileArray.reportConfig.Item9Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item10"),g_sorFileArray.reportConfig.Item10);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit10"),g_sorFileArray.reportConfig.Item10Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item11"),g_sorFileArray.reportConfig.Item11);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit11"),g_sorFileArray.reportConfig.Item11Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Item12"),g_sorFileArray.reportConfig.Item12);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("Edit12"),g_sorFileArray.reportConfig.Item12Val);

		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox1"),g_sorFileArray.reportConfig.Combox1);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox2"),g_sorFileArray.reportConfig.Combox2);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox3"),g_sorFileArray.reportConfig.Combox3);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox4"),g_sorFileArray.reportConfig.Combox4);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox5"),g_sorFileArray.reportConfig.Combox5);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox6"),g_sorFileArray.reportConfig.Combox6);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox7"),g_sorFileArray.reportConfig.Combox7);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox8"),g_sorFileArray.reportConfig.Combox8);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox9"),g_sorFileArray.reportConfig.Combox9);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox10"),g_sorFileArray.reportConfig.Combox10);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox11"),g_sorFileArray.reportConfig.Combox11);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox12"),g_sorFileArray.reportConfig.Combox12);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox13"),g_sorFileArray.reportConfig.Combox13);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox14"),g_sorFileArray.reportConfig.Combox14);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox15"),g_sorFileArray.reportConfig.Combox15);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_EN"), _T("ComboBox16"),g_sorFileArray.reportConfig.Combox16);
		//Step3
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay_EN"), _T("NumberOfWaveLength"),g_sorFileArray.reportConfig.NumberOfWaveLength);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay_EN"), _T("MeasureDir"),g_sorFileArray.reportConfig.MeasureDir);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay_EN"), _T("OutputSort"),g_sorFileArray.reportConfig.OutputSort);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay_EN"), _T("EventSort"),g_sorFileArray.reportConfig.EventSort);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay_EN"), _T("UpDownSort"),g_sorFileArray.reportConfig.UpDownSort);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay_EN"), _T("FileFormat"),g_sorFileArray.reportConfig.FileFormat);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay_EN"), _T("NumberOfFiber"),g_sorFileArray.reportConfig.NumberOfFiber);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay_EN"), _T("StartOfFiberNo"),g_sorFileArray.reportConfig.StartOfFiberNo);
		
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTotalList"), _T("Add"), g_sorFileArray.reportConfig.Add);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListLay_EN"), _T("TotalLossOnOff"), g_sorFileArray.reportConfig.TotalLossOnOff);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListLay_EN"), _T("AddAbnormality"), g_sorFileArray.reportConfig.AddAbnormality);
		//Step4
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay_EN"), _T("Folder1"),g_sorFileArray.reportConfig.Folder1);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay_EN"), _T("Folder2"),g_sorFileArray.reportConfig.Folder2);
		IniUtil::WriteSingleConfigInfo(_T("RepLayFileSelect"), _T("SetOptionSort"),g_sorFileArray.reportConfig.SetOptionSort);
		IniUtil::WriteSingleConfigInfo(_T("FileSelOption"), _T("BeAdapted"),g_sorFileArray.reportConfig.BeAdapted);
		IniUtil::WriteSingleConfigInfo(_T("FileSelOption"), _T("WLIndex"),g_sorFileArray.reportConfig.WLIndex);
	}

	//葡语配置文件保存
	void CMainFrame::RecordConfig_PT()
	{
		//保存设置信息
		/*                                 OTDR设备信息                                                 */
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("Language"),g_sorFileArray.otdrConfig.strLang);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("LightLevel"),g_sorFileArray.otdrConfig.strLightLevel);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("AutoPowerOff"),g_sorFileArray.otdrConfig.strAutoPowerOff	);
		//IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("Date"),g_sorFileArray.otdrConfig.strDate);
		//IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("Time"),g_sorFileArray.otdrConfig.strTime);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("TouchSreeenStatus"),g_sorFileArray.otdrConfig.strTouchSreeenStatus);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("TestWave"),g_sorFileArray.otdrConfig.strTestWave);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("TestTime"),g_sorFileArray.otdrConfig.strTestTime);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("TestMode"),g_sorFileArray.otdrConfig.strTestMode);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("TestRange"),g_sorFileArray.otdrConfig.strTestRange);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("TestUnit"),g_sorFileArray.otdrConfig.strUnit);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("PluseWidth"),g_sorFileArray.otdrConfig.strPluseWidth);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("strTestRange"),g_sorFileArray.otdrConfig.strTestRange);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("Resolution"),g_sorFileArray.otdrConfig.strResolution);
		IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("AutoSave"),g_sorFileArray.otdrConfig.strAutoSave);

		/*                                          报表配置数据                                        */
		//step1
		IniUtil::WriteSingleConfigInfo(_T("RepLayListLay_PT"), _T("ListFormat"),g_sorFileArray.reportConfig.ListFormat);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay_PT"), _T("Print"),g_sorFileArray.reportConfig.Print);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay_PT"), _T("Direction"),g_sorFileArray.reportConfig.Direction);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay"), _T("WaveList"),g_sorFileArray.reportConfig.WaveList);
		//step2
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Heading"),g_sorFileArray.reportConfig.Title);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item1"),g_sorFileArray.reportConfig.Item1);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit1"),g_sorFileArray.reportConfig.Item1Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item2"),g_sorFileArray.reportConfig.Item2);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit2"),g_sorFileArray.reportConfig.Item2Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item3"),g_sorFileArray.reportConfig.Item3);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit3"),g_sorFileArray.reportConfig.Item3Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item4"),g_sorFileArray.reportConfig.Item4);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit4"),g_sorFileArray.reportConfig.Item4Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item5"),g_sorFileArray.reportConfig.Item5);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit5"),g_sorFileArray.reportConfig.Item5Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item6"),g_sorFileArray.reportConfig.Item6);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit6"),g_sorFileArray.reportConfig.Item6Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item7"),g_sorFileArray.reportConfig.Item7);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit7"),g_sorFileArray.reportConfig.Item7Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item8"),g_sorFileArray.reportConfig.Item8);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit8"),g_sorFileArray.reportConfig.Item8Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item9"),g_sorFileArray.reportConfig.Item9);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit9"),g_sorFileArray.reportConfig.Item9Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item10"),g_sorFileArray.reportConfig.Item10);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit10"),g_sorFileArray.reportConfig.Item10Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item11"),g_sorFileArray.reportConfig.Item11);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit11"),g_sorFileArray.reportConfig.Item11Val);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Item12"),g_sorFileArray.reportConfig.Item12);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("Edit12"),g_sorFileArray.reportConfig.Item12Val);

		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox1"),g_sorFileArray.reportConfig.Combox1);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox2"),g_sorFileArray.reportConfig.Combox2);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox3"),g_sorFileArray.reportConfig.Combox3);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox4"),g_sorFileArray.reportConfig.Combox4);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox5"),g_sorFileArray.reportConfig.Combox5);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox6"),g_sorFileArray.reportConfig.Combox6);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox7"),g_sorFileArray.reportConfig.Combox7);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox8"),g_sorFileArray.reportConfig.Combox8);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox9"),g_sorFileArray.reportConfig.Combox9);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox10"),g_sorFileArray.reportConfig.Combox10);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox11"),g_sorFileArray.reportConfig.Combox11);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox12"),g_sorFileArray.reportConfig.Combox12);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox13"),g_sorFileArray.reportConfig.Combox13);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox14"),g_sorFileArray.reportConfig.Combox14);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox15"),g_sorFileArray.reportConfig.Combox15);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListTitle_PT"), _T("ComboBox16"),g_sorFileArray.reportConfig.Combox16);
		//Step3
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay_PT"), _T("NumberOfWaveLength"),g_sorFileArray.reportConfig.NumberOfWaveLength);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay_PT"), _T("MeasureDir"),g_sorFileArray.reportConfig.MeasureDir);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay_PT"), _T("OutputSort"),g_sorFileArray.reportConfig.OutputSort);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay_PT"), _T("EventSort"),g_sorFileArray.reportConfig.EventSort);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay_PT"), _T("UpDownSort"),g_sorFileArray.reportConfig.UpDownSort);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay_PT"), _T("FileFormat"),g_sorFileArray.reportConfig.FileFormat);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay_PT"), _T("NumberOfFiber"),g_sorFileArray.reportConfig.NumberOfFiber);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay_PT"), _T("StartOfFiberNo"),g_sorFileArray.reportConfig.StartOfFiberNo);

		IniUtil::WriteSingleConfigInfo(_T("RepLayListTotalList"), _T("Add"), g_sorFileArray.reportConfig.Add);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListLay_PT"), _T("TotalLossOnOff"), g_sorFileArray.reportConfig.TotalLossOnOff);
		IniUtil::WriteSingleConfigInfo(_T("RepLayListLay_PT"), _T("AddAbnormality"), g_sorFileArray.reportConfig.AddAbnormality);
		//Step4
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay_PT"), _T("Folder1"),g_sorFileArray.reportConfig.Folder1);
		IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay_PT"), _T("Folder2"),g_sorFileArray.reportConfig.Folder2);
		IniUtil::WriteSingleConfigInfo(_T("RepLayFileSelect"), _T("SetOptionSort"),g_sorFileArray.reportConfig.SetOptionSort);
		IniUtil::WriteSingleConfigInfo(_T("FileSelOption"), _T("BeAdapted"),g_sorFileArray.reportConfig.BeAdapted);
		IniUtil::WriteSingleConfigInfo(_T("FileSelOption"), _T("WLIndex"),g_sorFileArray.reportConfig.WLIndex);
	}

	//创建网络连接对话框
	void CMainFrame::CreateRemoteConnDlg()
	{
		if (!m_pRemoteConnDlg)
		{
			m_pRemoteConnDlg = new CRemoteConnDlg();
			m_pRemoteConnDlg->Create(IDD_DIALOG_CONNECT, this);
		}
	}

	//创建WaveForm窗口
	void CMainFrame::CreateWaveFormFrame()
	{
		if(!m_cfWaveForm)
			m_cfWaveForm = new CChildFrame();

		CCreateContext cc;
		cc.m_pNewViewClass = RUNTIME_CLASS(CWaveform);
		if(!m_cfWaveForm->LoadFrame(IDR_OTDRTYPE,WS_MAXIMIZE|WS_OVERLAPPEDWINDOW,this,&cc))
			return;

		CMenu *pSysMenu = m_cfWaveForm->GetSystemMenu(FALSE);
		VERIFY(pSysMenu->RemoveMenu(SC_CLOSE, MF_BYCOMMAND));

		m_formviewRect.SetRect(0, 0, m_frameRect.Width(), DEFAULT_WAVEFORM_HEIGHT);
		m_cfWaveForm->MoveWindow(m_formviewRect,TRUE);
		m_cfWaveForm->ShowWindow(SW_SHOWNORMAL);
		m_cfWaveForm->InitialUpdateFrame(NULL,TRUE);
		m_pWaveFormView = (CWaveform *)m_cfWaveForm->GetActiveView();
	}

	//创建EventList窗口
	void CMainFrame::CreateEventListFrame()
	{
		if(!m_cfEventList)
			m_cfEventList=new CChildFrame();

		CCreateContext cc;
		cc.m_pNewViewClass=RUNTIME_CLASS(CEventlist);
		if(!m_cfEventList->LoadFrame(IDR_OTDRTYPE,WS_MAXIMIZE|WS_OVERLAPPEDWINDOW,this,&cc))
			return;

		CMenu *pSysMenu = m_cfEventList->GetSystemMenu(FALSE); 
		VERIFY(pSysMenu->RemoveMenu(SC_CLOSE, MF_BYCOMMAND)); 

		m_eventlistRect.SetRect(0, DEFAULT_WAVEFORM_HEIGHT, m_frameRect.Width() * 3 / 4, m_frameRect.bottom - DEFAULT_EVENTLIST_DIST);//设置子窗口位置及大小
		m_cfEventList->MoveWindow(&m_eventlistRect,TRUE);//设置子窗口位置及大小
		m_cfEventList->ShowWindow(SW_SHOWNORMAL);

		m_cfEventList->InitialUpdateFrame(NULL,TRUE);
		m_pEventFormView = (CEventlist *)m_cfEventList->GetActiveView();
	}

	//创建ThumbNail窗口
	void CMainFrame::CreateThumbnailFrame()
	{
		if(!m_cfThumbnail)
			m_cfThumbnail = new CChildFrame();

		CCreateContext cc;
		cc.m_pNewViewClass=RUNTIME_CLASS(CThumbnail);
		if(!m_cfThumbnail->LoadFrame(IDR_OTDRTYPE,WS_MAXIMIZE|WS_OVERLAPPEDWINDOW,this,&cc))
			return;
		//disable system close
		CMenu *pSysMenu = m_cfThumbnail->GetSystemMenu(FALSE); 
		VERIFY(pSysMenu->RemoveMenu(SC_CLOSE, MF_BYCOMMAND)); 

		m_thumbnailRect.SetRect(m_frameRect.Width() * 3 / 4, DEFAULT_WAVEFORM_HEIGHT, m_frameRect.Width(), m_frameRect.bottom - DEFAULT_EVENTLIST_DIST);
		m_cfThumbnail->MoveWindow(&m_thumbnailRect,TRUE);//设置子窗口位置及大小
		m_cfThumbnail->ShowWindow(SW_SHOWNORMAL);

		m_cfThumbnail->InitialUpdateFrame(NULL,TRUE);
		m_pThumbFormView = (CThumbnail *)m_cfThumbnail->GetActiveView();
	}

	void CMainFrame::CreateReportFrame() //创建报表窗口
	{
		if(!m_cfReport)
			m_cfReport = new CChildFrame();

		CCreateContext cc;
		cc.m_pNewViewClass=RUNTIME_CLASS(CReport);
		if(!m_cfReport->LoadFrame(IDR_OTDRTYPE,WS_MAXIMIZE|WS_OVERLAPPEDWINDOW,this,&cc))
			return;
		//disable system close
		CMenu *pSysMenu = m_cfReport->GetSystemMenu(FALSE); 
		VERIFY(pSysMenu->RemoveMenu(SC_CLOSE, MF_BYCOMMAND)); 

		m_cfReport->InitialUpdateFrame(NULL,TRUE);
		m_pReportFormView = (CReport *)m_cfReport->GetActiveView();
		
		m_reportRect.SetRect(0, 0, m_frameRect.Width(), m_frameRect.bottom - DEFAULT_STATUSBAR_HEIGHT);//设置子窗口位置及大小
		m_cfReport->MoveWindow(&m_frameRect,TRUE);//设置子窗口位置及大小
		m_cfReport->ShowWindow(SW_HIDE);
	}

	void CMainFrame::CreateMainSettingFrame()
	{
		if(!m_cfMainSetting)
			m_cfMainSetting = new CChildFrame();

		CCreateContext cc;
		cc.m_pNewViewClass=RUNTIME_CLASS(CMainSetting);
		if(!m_cfMainSetting->LoadFrame(IDR_OTDRTYPE,WS_MAXIMIZE|WS_OVERLAPPEDWINDOW,this,&cc))
			return;
		//disable system close
		CMenu *pSysMenu = m_cfMainSetting->GetSystemMenu(FALSE); 
		VERIFY(pSysMenu->RemoveMenu(SC_CLOSE, MF_BYCOMMAND)); 

		m_cfMainSetting->InitialUpdateFrame(NULL,TRUE);
		m_pMainSetting = (CMainSetting *)m_cfMainSetting->GetActiveView();

		m_mainSettingRect.SetRect(0, m_frameRect.top, DEFAULT_MAIN_SETTING_WIDTH, m_frameRect.bottom-DEFAULT_EVENTLIST_DIST);
		m_cfMainSetting->MoveWindow(&m_mainSettingRect,TRUE);//设置子窗口位置及大小
		m_cfMainSetting->ShowWindow(SW_HIDE);
	}

	void CMainFrame::ShowMainSetting(BOOL bShow)
	{
		//bShow 显示主设置对话框，隐藏报表
		if (bShow)
		{
			m_cfReport->ShowWindow(SW_SHOWNORMAL);
			m_cfReport->ShowWindow(SW_HIDE);
			m_bMainSetting = TRUE;
			m_cfMainSetting->ShowWindow(SW_SHOW);
			m_bShowWaveForm = TRUE;
			m_cfWaveForm->ShowWindow(SW_SHOW);
			m_bShowEventList = TRUE;
			m_cfEventList->ShowWindow(SW_SHOW);
			m_bShowThumbnail = TRUE;
			m_cfThumbnail->ShowWindow(SW_SHOW);
			//调整波形窗口大小
			CRect rectWave;
			rectWave.SetRect(DEFAULT_MAIN_SETTING_WIDTH, m_frameRect.top, m_frameRect.right, DEFAULT_WAVEFORM_HEIGHT);
			m_cfWaveForm->MoveWindow(&rectWave, TRUE);

			/********///zll,2012.9.13 使得用报表向导打开曲线时，有曲线横纵坐标单位文本、事件列表及缩略图的显示
			int index = GetNextCurveNum(g_sorFileArray);
			m_pWaveFormView->PopulateData();
			//填充事件列表
			m_pEventFormView->FreshEventList();
			//填充曲线数据
			////m_pWaveFormView->m_sorDrawStatic.FillCurveDataFromFile(g_sorFileArray.sorFileList[index].pTestResultInfo); cwcq2013.03.29
			//m_pWaveFormView->m_sorDrawStatic.Invalidate();//redraw cwcq2013.03.29
			m_pThumbFormView->Invalidate(); //redraw thumbnail

			//设置横坐标单位文本
			m_pWaveFormView->SetHorUnitText();
			//设置纵坐标单位文本
			m_pWaveFormView->SetVerUnitText();
			/********///zll,2012.9.13 使得用报表向导打开曲线时，有曲线横纵坐标单位文本、事件列表及缩略图的显示

			m_pWaveFormView->SetMainSettingMode(FALSE);
			//调整事件列表窗口大小
			m_cfEventList->MoveWindow(m_eventlistRect.left+DEFAULT_MAIN_SETTING_WIDTH,
				m_eventlistRect.top,
				m_eventlistRect.Width()- DEFAULT_MAIN_SETTING_WIDTH,
				m_eventlistRect.Height(), TRUE);
		}
		else
		{
			m_bMainSetting = FALSE;
			m_cfMainSetting->ShowWindow(SW_HIDE);
			m_bShowWaveForm = FALSE;
			m_cfWaveForm->ShowWindow(SW_HIDE);
			m_bShowEventList = FALSE;
			m_cfEventList->ShowWindow(SW_HIDE);
			m_bShowThumbnail = FALSE;
			m_cfThumbnail->ShowWindow(SW_HIDE);
			m_cfReport->ShowWindow(SW_SHOWMAXIMIZED);
			//调整波形窗口大小
			CRect rectWave;
			rectWave.SetRect(DEFAULT_MAIN_SETTING_WIDTH, m_frameRect.top, m_frameRect.right, DEFAULT_WAVEFORM_HEIGHT);
			m_cfWaveForm->MoveWindow(&rectWave, TRUE);
			m_pWaveFormView->SetMainSettingMode(FALSE);
			//调整事件列表窗口大小
			m_cfEventList->MoveWindow(&m_eventlistRect, TRUE);
		}
	}

	void CMainFrame::ShowReport(BOOL bShow)
	{
		//bShow TRUE 隐藏主设置对话框，显示报表
		if (bShow)
		{
			m_bReportMode = TRUE;
			m_pPrintView->m_bPrintCurve = FALSE;//打印报表
			m_bShowWaveForm = FALSE;
			m_cfWaveForm->ShowWindow(SW_HIDE);
			m_bShowEventList = FALSE;
			m_cfEventList->ShowWindow(SW_HIDE);
			m_bShowThumbnail = FALSE;
			m_cfThumbnail->ShowWindow(SW_HIDE);
			//m_cfReport->ShowWindow(SW_NORMAL);//显示报表
			m_cfReport->ShowWindow(SW_SHOWMAXIMIZED);//显示报表
		}
		else
		{
			m_bReportMode = FALSE;
			m_bFileMenuEnable = FALSE; //无迹线d打开
			m_pPrintView->m_bPrintCurve = TRUE; //打印曲线
			m_cfReport->ShowWindow(SW_SHOWNORMAL);
			m_cfReport->ShowWindow(SW_HIDE);
			//m_pMainSetting->ShowWindow(SW_HIDE); //隐藏主设置窗口
			//恢复初始设置
			OnOriginSetting();
		}
	}

	BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle , CWnd* pParentWnd , CCreateContext* pContext)
	{
		BOOL bRet = CMDIFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext);

		//设置本地化
		IniUtil::ReadLangSetting();
		SetLocaleString();		

		//设置菜单状态
		m_bFileMenuEnable = FALSE;

		return bRet;
	}

	BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
	{
		m_pPrintView = new CPrintView();
		m_pPrintView->Create(NULL,NULL,WS_CHILD | WS_VISIBLE, CRect(0,0,0,0), this, AFX_IDW_PANE_LAST, pContext);

		return CMDIFrameWnd::OnCreateClient(lpcs, pContext);
	}

	void CMainFrame::TileViewFrame()
	{
		if(m_bTilelayout==FALSE)
		{
			SIZE fclient={0};
			COTDRView* lpView=NULL;

			GetMDIClientSIZE((SIZE*)&fclient);

			if(fclient.cx<=245||fclient.cy<=102)
				return;

			int hpos=fclient.cx-8;//框架的宽度
			int vpos=fclient.cy/2;//框架的高度

			//获取指定框架标题的视图指标,设置WaveForm框架的大小及位置,zll
			if((lpView = GetSpecifyFrameView((LPCTSTR)_T("Waveform"))) != NULL)
			{
				if(lpView->GetParentFrame()->IsIconic())
					lpView->GetParentFrame()->ShowWindow(SW_RESTORE);
				lpView->GetParentFrame()->MoveWindow(0,0,hpos,vpos*3/2,TRUE);
				//lpView->GetParentFrame()->MoveWindow(0,0,hpos,vpos*3/2,TRUE);
				lpView->GetParentFrame()->ShowWindow(SW_SHOWNORMAL);
			}
			//获取指定框架标题的视图指标,设置EventList框架的大小及位置,zll
			if((lpView = GetSpecifyFrameView((LPCTSTR)_T("EventList"))) != NULL)
			{
				if(lpView->GetParentFrame()->IsIconic())
					lpView->GetParentFrame()->ShowWindow (SW_RESTORE);
				lpView->GetParentFrame()->MoveWindow(0,vpos*3/2,hpos,vpos/2,TRUE);
				//lpView->GetParentFrame()->MoveWindow(0,vpos*3/2,hpos,vpos/2,TRUE);
				lpView->GetParentFrame()->ShowWindow (SW_SHOWNORMAL);
			}
		}
		//如果左边停靠窗口没有被手动拖拉就进行程序自动设定合适宽高
		//if(!m_dockwnd1)
	}

	void CMainFrame::GetMDIClientSIZE(SIZE *fclient)
	{
		//获取MDIClient window 大小尺寸
		int ix,iy;
		int iwidth,iheight;
		tagRECT rc={0};
		ix=iy=iwidth=iheight=0;

		LPMAIN->GetClientRect(&rc);
		fclient->cx=rc.right+rc.left;
		fclient->cy=rc.top+rc.bottom;

		if(fclient->cx<=10||fclient->cy<= 10)
			return;

		if(LPMAIN->m_wndToolBar.IsWindowVisible())
		{
			LPMAIN->m_wndToolBar.GetWindowRect(&rc);
			ScreenToClient(&rc);
			fclient->cy-=rc.top+rc.bottom-2;
		}

		if(LPMAIN->m_wndStatusBar.IsWindowVisible())
		{
			m_wndStatusBar.GetClientRect(&rc);
			fclient->cy-=rc.top+rc.bottom;
		}

		fclient->cx+=4;
		fclient->cy+=2;
	}

	COTDRView* CMainFrame::GetSpecifyFrameView(LPCTSTR title, int viewID, int type)
	{
		COTDRView* lpView=NULL;

		if(type>0&&viewID<0)
		{
			//#ifdef CONST_TEST
			//CONST_WARNINGLOG(_T("TEST###:获取视图的参数错误 type > 0 && viewID < 0"),0);
			//#endif
			return lpView;
		}
		else
			if(type == 0&& lstrlen(title) <= 0)
			{
				//#ifdef CONST_TEST
				//CONST_WARNINGLOG("TEST###:获取视图的参数错误 type == 0 && lstrlen(title) <= 0",0);
				//#endif
				return lpView;
			}
			CWinApp* lpApp=AfxGetApp();
			POSITION pos1=lpApp->GetFirstDocTemplatePosition();

			while(pos1)
			{
				CDocTemplate* lptemp1=(CDocTemplate*)lpApp->GetNextDocTemplate(pos1);
				POSITION pos2=lptemp1->GetFirstDocPosition();

				while(pos2)
				{
					CDocument *pDoc;

					if((pDoc=lptemp1->GetNextDoc(pos2)) != NULL)
					{
						POSITION pos3;

						if(type==0)
						{
							//如果type = 0 就按框架标题寻找指定视图
							if(pDoc->GetTitle().CompareNoCase(title)==0)
							{
								//标题相同就取视图指针
								pos3=pDoc->GetFirstViewPosition();

								while(pos3)
								{
									if((lpView=(COTDRView*)pDoc->GetNextView(pos3))!=NULL)
										break;
								}
							}
						}
						else
						{
							//如果type = 1 就按视图id寻找指定视图
							pos3=pDoc->GetFirstViewPosition();
							COTDRView* lptmpView=NULL;

							while(pos3)
							{
								if((lptmpView=(COTDRView*)pDoc->GetNextView(pos3))!=NULL)
									//if(lptmpView->GetMyID()==viewID)
								{//id相等就将指针给lpView
									lpView=lptmpView;
									break;
								}
							}
						}
					}
				}
			}
			return lpView;
	}

	void CMainFrame::OnSize(UINT nType, int cx, int cy)
	{
		CMDIFrameWnd::OnSize(nType, cx, cy);
		if (NULL == m_cfEventList->GetSafeHwnd() ||
			NULL == m_cfMainSetting->GetSafeHwnd() ||
			NULL == m_cfReport->GetSafeHwnd() ||
			NULL == m_cfThumbnail->GetSafeHwnd())
			return;
		
		//获取窗口大小
		GetClientRect(&m_frameRect);
		//事件列表
		m_eventlistRect.SetRect(0, m_frameRect.Height() * 2/3, m_frameRect.Width() * 3/4, m_frameRect.bottom - DEFAULT_EVENTLIST_DIST);//设置子窗口位置及大小
		m_cfEventList->MoveWindow(&m_eventlistRect,TRUE);//设置子窗口位置及大小
		//主设置
		m_mainSettingRect.SetRect(0, m_frameRect.top, DEFAULT_MAIN_SETTING_WIDTH, m_frameRect.bottom-DEFAULT_EVENTLIST_DIST);
		m_cfMainSetting->MoveWindow(&m_mainSettingRect,TRUE);//设置子窗口位置及大小
		//报表
		//m_cfReport->MoveWindow(&m_frameRect,TRUE);//设置子窗口位置及大小
		//m_cfReport->ShowWindow(SW_NORMAL);
		if (m_bReportMode)
			m_cfReport->ShowWindow(SW_SHOWMAXIMIZED);
		//缩略图
		m_thumbnailRect.SetRect(m_frameRect.Width() * 3/4, m_frameRect.Height() * 2/3, m_frameRect.Width(), m_frameRect.bottom - DEFAULT_EVENTLIST_DIST);
		m_cfThumbnail->MoveWindow(&m_thumbnailRect,TRUE);//设置子窗口位置及大小
		//图像显示区
		m_formviewRect.SetRect(0, 0, m_frameRect.Width(), m_frameRect.Height() * 2/3);
		m_cfWaveForm->MoveWindow(m_formviewRect,TRUE);
	}

	////////////////////////////////////////////////////////////////////////////////////
	//收到上位机通过网络传过来的配置信息后，更新对应的配置信息 wcq2012.05.18
	//格式:1AutoSave=1,TestMode=0,TestTime=5,...
	//1:系统设置 2:测试设置
	////////////////////////////////////////////////////////////////////////////////////
	void CMainFrame::DecodeNetData(CString data)
	{
		if (data.GetLength()<4)
		{
			return;
		}//eif
		CStringArray strArr;
		CStringArray strArrTemp;
		CString temp;
		CString itemValue;//对应的值
		CString valideData;//
		valideData = data.Right(data.GetLength()-1);
		int count;
		CUtil::splitString(valideData,',',strArr);//把各项分开
		count = strArr.GetSize();
		if (NET_TYPE_SYSTEM == data.Left(1))//系统设置
		{
			for (int i=0;i<count;i++)//遍历每一项
			{
				CUtil::splitString(strArr[i],'=',strArrTemp);
				if (strArrTemp.GetSize()<2)//格式不对
				{
					continue;
				}//eif
				temp = strArrTemp[0];//
				itemValue = strArrTemp[1];//
				//各个字符串都是从OTDR.ini里的一致,如果OTDR.ini和config.h相应变量的命名作了修改，这里也要作修改
				if ("Language" == temp)//语言选择
				{
					if ("1" == itemValue)
					{
						g_sorFileArray.otdrConfig.strLang = itemValue;
					}
					else
					{
						g_sorFileArray.otdrConfig.strLang = _T("0");
					}
				}
				else if ("TouchSreeenStatus" == temp)//触摸屏状态
				{
					if ("1" == itemValue)
					{
						g_sorFileArray.otdrConfig.strTouchSreeenStatus= itemValue;
					}
					else
					{
						g_sorFileArray.otdrConfig.strTouchSreeenStatus = _T("0");
					}
				}
				else if ("AutoPowerOff" == temp)//自动关机
				{
					g_sorFileArray.otdrConfig.strAutoPowerOff = itemValue;
				}
				else if ("LightLevel" == temp)//背光亮度调节
				{
					g_sorFileArray.otdrConfig.strLightLevel = itemValue;
				}//eif
			}//efor
			::PostMessage(m_pRemotePageSheet->GetSafeHwnd(),SOCK_ONRECEIVE_DATA_READY, 1, 0);
		}
		else if (NET_TYPE_SETUP == data.Left(1))//测试设置
		{
			for (int i=0;i<count;i++)//遍历每一项
			{
				CUtil::splitString(strArr[i],'=',strArrTemp);
				if (strArrTemp.GetSize()<2)//格式不对
				{
					continue;
				}//eif
				temp = strArrTemp[0];//
				itemValue = strArrTemp[1];//
				//各个字符串都是从OTDR.ini里的一致,如果OTDR.ini和config.h相应变量的命名作了修改，这里也要作修改
				if ("TestWave" == temp)//测量波长
				{
					g_sorFileArray.otdrConfig.strTestWave = itemValue;
				}
				else if ("TestMode" == temp)//测量模式
				{
					if ("1" == itemValue)
					{
						g_sorFileArray.otdrConfig.strTestMode = itemValue;
					}
					else
					{
						g_sorFileArray.otdrConfig.strTestMode = _T("0");
					}//eif
				}
				else if ("TestTime" == temp)//测量时间
				{
					g_sorFileArray.otdrConfig.strTestTime = itemValue;
				}
				else if ("AutoSave" == temp)//自动保存
				{
					if ("1" == itemValue)
					{
						g_sorFileArray.otdrConfig.strAutoSave = itemValue;
					}
					else
					{
						g_sorFileArray.otdrConfig.strAutoSave = _T("0");
					}//eif
				}//eif
				else if ("Resolution" == temp)//分辨率
				{
					if ("1" == itemValue)
					{
						g_sorFileArray.otdrConfig.strResolution = itemValue;
					}
					else
					{
						g_sorFileArray.otdrConfig.strResolution = _T("0");
					}//eif
				}
				else if ("TestRange" == temp)//测量范围
				{
					g_sorFileArray.otdrConfig.strTestRange = itemValue;
				}
				else if ("TestUnit" == temp)//单位
				{
					g_sorFileArray.otdrConfig.strUnit = itemValue;
				}
				else if ("PluseWidth" == temp)//脉宽选择
				{
					g_sorFileArray.otdrConfig.strPluseWidth = itemValue;
				}//eif
			}//efor
			::PostMessage(m_pRemotePageSheet->GetSafeHwnd(),SOCK_ONRECEIVE_DATA_READY, 2, 0);
		}//eif
	}

	//上位机要求同步本地数据,把本地的一些配置信息按格式组合一下 wcq2012.05.18
	CString CMainFrame::GetNetData(CString setType)
	{
		CString re = setType;
		if (NET_TYPE_SYSTEM == setType)//系统设置
		{
			re += _T("Language=") + g_sorFileArray.otdrConfig.strLang + _T(",");////语言选择
			re += _T("TouchSreeenStatus=") + g_sorFileArray.otdrConfig.strTouchSreeenStatus + _T(",");////触摸屏状态
			re += _T("AutoPowerOff=") + g_sorFileArray.otdrConfig.strAutoPowerOff + _T(",");////自动关机
			re += _T("LightLevel=") + g_sorFileArray.otdrConfig.strLightLevel + _T(",");////背光亮度调节
		}
		else if (NET_TYPE_SETUP == setType)//测试设置
		{
			re += _T("TestWave=") + g_sorFileArray.otdrConfig.strTestWave + _T(",");////测量波长
			re += _T("TestMode=") + g_sorFileArray.otdrConfig.strTestMode + _T(",");////测量模式
			re += _T("TestTime=") + g_sorFileArray.otdrConfig.strTestTime + _T(",");////测量时间
			re += _T("AutoSave=") + g_sorFileArray.otdrConfig.strAutoSave + _T(",");////自动保存
			re += _T("Resolution=") + g_sorFileArray.otdrConfig.strAutoSave + _T(",");////分辨率
			re += _T("TestRange=") + g_sorFileArray.otdrConfig.strTestRange + _T(",");////测量范围
			re += _T("TestUnit=") + g_sorFileArray.otdrConfig.strUnit + _T(",");////单位
			re += _T("PluseWidth=") + g_sorFileArray.otdrConfig.strPluseWidth + _T(",");////脉宽选择
		}//eif
		return re;
	}
	
	//菜单选项，设置测量的参数等,lzy2014.8.6
	void CMainFrame::OnSetting()
	{		
		CString strTitle = IniUtil::ReadResString(_T("IDS_SETTING_TITLE"));
		CMyPropertySheet myProp(strTitle);
		
		//myProp.m_propConfig.SetLocalString();  //根据所选的语言设定静态文本
		if(myProp.DoModal() == IDOK)
		{
			myProp.SaveSetting(); 
						
		}
		else
		{
			//AfxMessageBox(_T("cancel button pushed"));
		}
	}
