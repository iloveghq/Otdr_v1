#include "StdAfx.h"
#include "Command.h"
#include "LangSwitchCommand.h"
#include "CommandManager.h"
#include "SorFileInfo.h"
#include "MainFrm.h"

extern LANG g_currentLang;

CLangSwitchCommand::CLangSwitchCommand(CCommandManager* pCommandManager) : Command
	(pCommandManager)
{
	m_CommandID = Commands::LANG_SWITCH;
}

CLangSwitchCommand::~CLangSwitchCommand(void)
{
}

void CLangSwitchCommand::Execute()
{
	//�޸�Frame����
	CMainFrame *pMain = MAIN;
	if (pMain)
	{
		pMain->SetLocaleString();

		//������������
		IniUtil::SaveLangSetting();
	}
}
