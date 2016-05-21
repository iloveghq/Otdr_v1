#include "StdAfx.h"
#include "Command.h"
#include "FileSaveCommand.h"
#include "CommandManager.h"
#include "SorFileInfo.h"
#include "MainFrm.h"

extern SorFileArray g_sorFileArray;

CFileSaveCommand::CFileSaveCommand(CCommandManager* pCommandManager) : Command
	(pCommandManager)
{
	m_CommandID = Commands::FILE_SAVE;
}

CFileSaveCommand::~CFileSaveCommand(void)
{
}

void CFileSaveCommand::Execute()
{
	CString strMsg;

	CFileDialog dlg(FALSE,_T("*.SOR"),NULL,OFN_HIDEREADONLY,_T("SOR (*.SOR)|*.SOR||"));
	if(dlg.DoModal() != IDOK)
		return;

	CString strFilePath=dlg.GetPathName();//�õ��ļ���
	CString strFileName=dlg.GetFileName();//·�����ļ���
	
	TestResultInfo* pFileInfo = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;

	//�����ļ�
	CSorFile sorFile;
	if (1 == sorFile.saveSorFile(strFileName, *pFileInfo))
	{
	}
	else//�������д�����ʾ????????????????????????
	{

	}
}
