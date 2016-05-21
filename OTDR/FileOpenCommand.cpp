#include "StdAfx.h"
#include "Command.h"
#include "FileOpenCommand.h"
#include "CommandManager.h"
#include "SorFileInfo.h"
#include "MainFrm.h"

extern SorFileArray g_sorFileArray;

CFileOpenCommand::CFileOpenCommand(CCommandManager* pCommandManager) : Command
	(pCommandManager)
{
	m_CommandID = Commands::FILE_OPEN;
}

CFileOpenCommand::~CFileOpenCommand(void)
{
}

void CFileOpenCommand::Execute()
{
	CString strMsg;
	int index = GetNextCurveNum(g_sorFileArray);
	if (LB_ERR == index)//ȫ�������ع���
	{
		strMsg.Format(_T("������%d������.      "), MAX_CURVE_NUM);
		MessageBox(NULL, strMsg, _T("��ʾ"), MB_OK|MB_ICONEXCLAMATION);
		return;
	}

	CFileDialog dlg(TRUE,_T("*.SOR"),NULL,OFN_HIDEREADONLY,_T("SOR (*.SOR)|*.SOR||"));
	if(dlg.DoModal() != IDOK)
		return;

	CString strFilePath=dlg.GetPathName();//�õ��ļ���
	CString strFileName=dlg.GetFileName();//·�����ļ���
	//�ж��ļ��Ƿ��Ѿ�����
	if (IsFileAlreadyOpen(strFilePath))
	{	
		return;
	}
	//��ȡ�ļ�
	CSorFile sorFile;
	if (1 == sorFile.readSorFile(strFileName, *(g_sorFileArray.sorFileList[index].pTestResultInfo)))
	{
		g_sorFileArray.sorFileList[index].bValid = TRUE;
		g_sorFileArray.sorFileList[index].fileName = strFileName;
		g_sorFileArray.sorFileList[index].filePathName = strFilePath;
		g_sorFileArray.totalCurveNumber++;
		//����������ɫ
		g_sorFileArray.sorFileList[index].pTestResultInfo->curveColor = GetCurveColor(g_sorFileArray.totalCurveNumber);
		g_sorFileArray.curSelection = index;//��Ϊ��ǰѡ���к�
	}
	else//�������д�����ʾ????????????????????????
	{
		CString strMsg = _T("���ļ�����");
		MessageBox(NULL, strMsg, _T("��ʾ"), MB_OK|MB_ICONEXCLAMATION);
		return; //
	}
	if (MAIN)
	{
		MAIN->m_pWaveFormView->PopulateData();
		//�����������
		MAIN->m_pWaveFormView->m_sorDrawStatic.FillCurveDataFromFile(g_sorFileArray.sorFileList[index].pTestResultInfo);
		MAIN->m_pWaveFormView->m_sorDrawStatic.Invalidate();//redraw
		MAIN->m_pThumbFormView->Invalidate(); //redraw thumbnail

		//���ú����굥λ�ı�
		MAIN->m_pWaveFormView->SetHorUnitText();
		//���������굥λ�ı�
		MAIN->m_pWaveFormView->SetVerUnitText();

		//�����ʹ�ӡ�˵�
		MAIN->m_bFileMenuEnable = TRUE;
	}
}
