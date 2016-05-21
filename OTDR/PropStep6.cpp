// PropStep6.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OTDR.h"
#include "Util.h"
#include "PrintSheet.h"
#include "PropStep6.h"
#include "CSpreadSheet.h"
#include "XmlNode.h"
#include "XmlNodes.h"

extern SorFileArray g_sorFileArray;
extern LANG g_currentLang;
//ģ���ļ�
CString str_cannot_find_file=_T("");
CString str_check_file=_T("");
// CPropStep6 �Ի���

IMPLEMENT_DYNAMIC(CPropStep6, CPropertyPage)

CPropStep6::CPropStep6()
	: CPropertyPage(CPropStep6::IDD)
{
	m_pThread = NULL;
}

CPropStep6::~CPropStep6()
{
}

void CPropStep6::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPropStep6, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_STEP6_PREVIEW, &CPropStep6::OnBnClickedBtnStep6Preview)
	ON_BN_CLICKED(IDC_BTN_STEP6_SETTING, &CPropStep6::OnBnClickedBtnStep6Setting)
	ON_BN_CLICKED(IDC_BTN_STEP6_PRINT, &CPropStep6::OnBnClickedBtnStep6Print)
	ON_BN_CLICKED(IDC_BTN_STEP6_EXCEL, &CPropStep6::OnBnClickedBtnStep6Excel)
END_MESSAGE_MAP()


// CPropStep6 ��Ϣ�������
BOOL CPropStep6::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	//((CPropertySheet*)GetParent())->SetWizardButtons(PSWIZB_FINISH);
	((CPropSheet*)GetParent())->SetReportHeadBtn(6);
	//�ػ�ͼ������
	MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.ReDrawRect(6);
	((CPropSheet*)GetParent())->EnableNextBtn(FALSE);
	((CPropSheet*)GetParent())->EnablePrevBtn(TRUE);
	return CPropertyPage::OnSetActive();
}

BOOL CPropStep6::OnWizardFinish() 
{
	// TODO: Add your specialized code here and/or call the base class
	//UpdateData();

	return CPropertyPage::OnWizardFinish();
}
void CPropStep6::OnBnClickedBtnStep6Preview()
{
	MAIN->printType=1;//printType=1ʱ����ӡ�����������������ļ�
	//����ļ��б�
	MAIN->m_pMainSetting->m_pageMainSetting.PopulateFileList();//�ٴ�������������ļ�������ֻ��4�����߿ɴ�ӡ
	MAIN->OnPrintPreview();
}

void CPropStep6::OnBnClickedBtnStep6Setting()
{
	CPrintSheet printSheet(_T("ҳ������"));
	printSheet.DoModal();
}

void CPropStep6::OnBnClickedBtnStep6Print()
{
	MAIN->m_pMainSetting->m_pageMainSetting.PopulateFileList();//�ٴ�������������ļ�������ֻ��4�����߿ɴ�ӡ//ԭ��Ҫ��Ԥ��wcq2016.03.03
	MAIN->OnPrint();
}

void CPropStep6::OnBnClickedBtnStep6Excel()
{
	//m_pThread = AfxBeginThread(CPropStep6::ExportXlsThread,this);//pThread2	
	//��ʾ������
	m_progress.Create(IDD_DIALOG_PROGRRESS);
	m_progress.ShowWindow(SW_HIDE);
	m_progress.CenterWindow();
	ExportToExcel();
	//m_progress.SetPos(g_sorFileArray.totalCurveNumber);
	m_progress.DestroyWindow();
}

BOOL CPropStep6::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	SetLocalString();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pag	es should return FALSE
}

void CPropStep6::SetLocalString()
{
	/*                      �������                            */
	CString strOutputSetting = IniUtil::ReadResString(_T("IDS_REPORT_STEP6_OUTPUT_SETTING"));
	GetDlgItem(IDC_STATIC_STEP6_SETTING)->SetWindowText(strOutputSetting);
	//Ԥ��
	CString strPreview = IniUtil::ReadResString(_T("IDS_REPORT_STEP6_PREVIEW"));
	GetDlgItem(IDC_BTN_STEP6_PREVIEW)->SetWindowText(strPreview);
	//ҳ������
	CString strPageSetup = IniUtil::ReadResString(_T("IDS_REPORT_STEP6_PAGE_SETUP"));
	GetDlgItem(IDC_BTN_STEP6_SETTING)->SetWindowText(strPageSetup);
	/*                      ��ӡ����                            */
	CString strPrintDirect = IniUtil::ReadResString(_T("IDS_REPORT_STEP6_OUTPUT_SETTING"));
	GetDlgItem(IDC_STATIC_STEP6_SELECT)->SetWindowText(strPrintDirect);
	//��ӡ
	CString strPrint = IniUtil::ReadResString(_T("IDS_REPORT_STEP6_PRINT"));
	GetDlgItem(IDC_BTN_STEP6_PRINT)->SetWindowText(strPrint);
	//ģ���ļ�
	str_cannot_find_file=IniUtil::ReadResString(_T("IDS_CANNOT_FIND_FILE"));
	str_check_file=IniUtil::ReadResString(_T("IDS_CHECK_FILE"));
}

LRESULT CPropStep6::OnWizardBack()
{
	MAIN->ShowMainSetting(TRUE);
	return 0;
}

UINT CPropStep6::ExportXlsThread(LPVOID lparam)
{
	CPropStep6* dlg=(CPropStep6*)lparam;
	dlg->ExportToExcel();
	dlg->m_progress.SetPos(g_sorFileArray.totalCurveNumber);
	dlg->m_progress.DestroyWindow();
	return 0;
}

void CPropStep6::ExportToExcel()
{
	//����ļ��б�
	MAIN->m_pMainSetting->m_pageMainSetting.PopulateFileList();
	//���ǲ�����Ҫ����λͼ�ļ�
	int nPrint = _wtoi(g_sorFileArray.reportConfig.Print);
	if (nPrint)
	{
		MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.OutPutBmp();//����λͼ
	}//eif

	//����Excel
	ExportXlsByRpt();
	//m_progress.SetPos(15);
	if (nPrint)//ɾ��λͼ�ļ�
		MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.DeleteBmpFiles();
}

//�����б���е�������,ͨ������
BOOL CPropStep6::ExportXlsByRpt()
{
	//��ȡ����excel·��
	CString strPath = IniUtil::ReadSingleConfigInfo(_T("ExcelOut"), _T("Path"));
	//CFileDialog dlg(FALSE,_T("*.xls"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("xls (Excel��ʽ)|*.xls||"));
	CFileDialog dlg(FALSE,_T("*.xls"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("*.xls;*.xlsx (Excel)|*.xls;*.xlsx||"));//zll 2013.7.9 ͬʱ����.xlsx��׺���ļ�
	dlg.m_ofn.lpstrInitialDir=strPath;
	if(dlg.DoModal() != IDOK)
		return FALSE;

	CString strFileName = dlg.GetPathName();//·�����ļ���

	/////////////////zll 2013.7.8
	CString temp=_T("");
	temp=strFileName.Right(1);
	int a=temp.CompareNoCase(_T("x"));
	if (temp.CompareNoCase(_T("x"))==0)
	{
		strFileName=strFileName.Left(strFileName.GetLength()-1);
	}///////////////

	//��ʾ������
	m_progress.ShowWindow(SW_SHOW);
	m_progress.SetPos(1);
	//�������ò�ͬ���ز�ͬ��Excelģ��
	CString strExcelTemp;
	if (g_sorFileArray.reportConfig.Print.CompareNoCase(_T("1")) == 0)
	{
		strExcelTemp.Format(_T("%s\\Excel\\Temp1.xls"), CUtil::getExePath());//����Excelģ��
		if(g_currentLang == LANG::CHS)
		{
			m_strXmlFile.Format(_T("%s\\Excel\\Wave.xml"), CUtil::getExePath());//���Ĳ�������ģ��
		}
		else if(g_currentLang == LANG::ENG)
		{
			m_strXmlFile.Format(_T("%s\\Excel\\WaveEn.xml"), CUtil::getExePath());//Ӣ�Ĳ�������ģ�� zll 2013.6.9
		}
		else if(g_currentLang == LANG::PORT)  //�������ﲨ������ģ�� lzy 2014.7.17
		{
			m_strXmlFile.Format(_T("%s\\Excel\\WavePt.xml"), CUtil::getExePath());
		}
	}
	else
	{
		strExcelTemp.Format(_T("%s\\Excel\\Temp2.xls"), CUtil::getExePath());//�ܱ�Excelģ��
		m_strXmlFile.Format(_T("%s\\Excel\\Rpt.xml"), CUtil::getExePath());//�ܱ�����ģ��
	}

	//���ģ���ļ�xxx.xls�Ƿ����.
	if(!CUtil::isFileExist(strExcelTemp))
	{
		CString strMsg=str_cannot_find_file+strExcelTemp+str_check_file;
		MessageBox(strMsg,_T("OTDR Viewer"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
	
	CopyFile(strExcelTemp, strFileName, false);
	
	int nSheetIndex = GetSheetIndex();
	int nCurveNumber = MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.m_nCurveNumber;
	if (nCurveNumber == 0)
		nCurveNumber= 1;  //1 curve per page
	else if (nCurveNumber == 1)
		nCurveNumber= 2;  //2 curve per page
	else if (nCurveNumber == 2)
		nCurveNumber= 4;  //4 curve per page
	else if (nCurveNumber == 3)
		nCurveNumber= 6;   //6 curve per page
	else if (nCurveNumber == 4)
		nCurveNumber= 8;   //8 curve per page
	int page_sheetcount=0;
	page_sheetcount=g_sorFileArray.totalCurveNumber/nCurveNumber;
	if (0!=g_sorFileArray.totalCurveNumber%nCurveNumber)
	{
		page_sheetcount+=1;
	}
	WriteToExcel(strFileName, nSheetIndex,page_sheetcount);//zll 2012.9.25 SheetCount
	return TRUE;
}

int CPropStep6::GetSheetIndex()
{
	int nPrint = _wtoi(g_sorFileArray.reportConfig.Print);
	if (nPrint)
	{
		//����
		int nCurveNumber = MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.m_nCurveNumber;
		if (nCurveNumber == 0)
			return 1;  //1 curve per page
		else if (nCurveNumber == 1)
			return 2;  //2 curve per page
		else if (nCurveNumber == 2)
			return 3;  //4 curve per page
		else if (nCurveNumber == 3)
			return 4;   //6 curve per page
		else if (nCurveNumber == 4)
			return 5;   //8 curve per page
	}
	else
	{
		//�ܱ�
	}

	return 1;
}

void CPropStep6::WriteToExcel(CSpreadSheet* excel)
{
	BOOL bSucceed = m_xml.Open(m_strXmlFile);
	if (!bSucceed)
		return;

	excel->BeginTransaction();
	// �������
	CXmlNode nodeTitle;
	nodeTitle = m_xml.GetRoot()->GetChild(_T("P1"))->GetChild(_T("Title"));
	if (!nodeTitle.IsNull())
	{
		CString strTitle = nodeTitle.GetAttribute(_T("text"));
		CString strRow = nodeTitle.GetAttribute(_T("row"));
		int nRow = _wtoi(strRow);
		CString strCol = nodeTitle.GetAttribute(_T("col"));
		int nCol = _wtoi(strCol);
		excel->AddCell(strTitle,nCol,nRow);
	}
	//������Ŀ1-12
	for (int i = 1; i <= 12; i++)
	{
		CString strItem;
		strItem.Format(_T("Item%d"), i);
		CXmlNode nodeItem;
		nodeItem = m_xml.GetRoot()->GetChild(_T("P1"))->GetChild(strItem);
		if (!nodeItem.IsNull())
		{
			CString strText = nodeItem.GetAttribute(_T("text"));
			CString strRow = nodeItem.GetAttribute(_T("row"));
			int nRow = _wtoi(strRow);
			CString strCol = nodeItem.GetAttribute(_T("col"));
			int nCol = _wtoi(strCol);
			excel->AddCell(strText,nCol,nRow);
		}
	}
	//excel->AddHeaders(sampleArray,TRUE);
	excel->Commit();
}

void CPropStep6::WriteToExcel(const CString& fileName, int nSheetIndex,int SheetCount)
{
	BOOL bSucceed = m_xml.Open(m_strXmlFile);
	if (!bSucceed)
		return;
	//��ʼ��Excel
	
	bSucceed = m_ExcelUtil.InitExcel(fileName, nSheetIndex,SheetCount);

	if (!bSucceed)
	{
		m_ExcelUtil.ExitExcel();
		return;
	}	

	int nPrint = _wtoi(g_sorFileArray.reportConfig.Print);
	if (!nPrint) //�ܱ�
	{
		ExportReport();
	}
	else //���߲���
	{
		//����
		int nCurveNumber = MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.m_nCurveNumber;
		if (nCurveNumber == 0)
			Export1Curve();  //1 curve per page
		else if (nCurveNumber == 1)
			Export2Curve();  //2 curve per page
		else if (nCurveNumber == 2)
			Export4Curve();  //4 curve per page
		else if (nCurveNumber == 3)
			Export6Curve();   //6 curve per page
		else if (nCurveNumber == 4)
			Export8Curve();   //8 curve per page
	}
    if (_T("9.0")==m_ExcelUtil.str_Office_Edition||_T("10.0")==m_ExcelUtil.str_Office_Edition||_T("11.0")==m_ExcelUtil.str_Office_Edition)
    {
		m_ExcelUtil.Save();
	}
	if (_T("12.0")==m_ExcelUtil.str_Office_Edition||_T("14.0")==m_ExcelUtil.str_Office_Edition)
	{
		m_ExcelUtil.Save(fileName+_T("x"));//wcq2012.10.31 zll 2013.7.8 office2007���׺����Ϊ.xlsx��ʽ�ļ�
		DeleteFile(fileName);//zll 2013.7.8 ������ɶ�Ӧ��fileName.xls��׺�ļ����ڴ���ɾ����ֻ��ʾfileName.xlsx�ļ�
	}
	else//��ǰû�� wcq2016.03.03
	{
		INT nVersion = _ttoi(m_ExcelUtil.str_Office_Edition);
		if (nVersion < 9)//�Ͱ汾
		{
			m_ExcelUtil.Save();
		}
		else//�߰汾
		{
			m_ExcelUtil.Save(fileName+_T("x"));//wcq2012.10.31 zll 2013.7.8 office2007���׺����Ϊ.xlsx��ʽ�ļ�
			DeleteFile(fileName);//zll 2013.7.8 ������ɶ�Ӧ��fileName.xls��׺�ļ����ڴ���ɾ����ֻ��ʾfileName.xlsx�ļ�
		}//eif
	}//eif
	//TRACE(_T("zll2 %d"),2);

	m_ExcelUtil.ExitExcel();
}

void CPropStep6::ExportReport() //�ܱ�
{
	// �������
	CXmlNode nodeTitle;
	nodeTitle = m_xml.GetRoot()->GetChild(_T("Title"));
	if (!nodeTitle.IsNull())
	{
		CString strTitle = nodeTitle.GetAttribute(_T("text"));
		CString strRow = nodeTitle.GetAttribute(_T("row"));
		int nRow = _wtoi(strRow);
		CString strCol = nodeTitle.GetAttribute(_T("col"));
		int nCol = _wtoi(strCol);
		m_ExcelUtil.InsertItem(nRow, nCol, strTitle);
	}
	
	//������Ŀ1-12
	for (int i = 1; i <= 12; i++)
	{
		CString strItem;
		strItem.Format(_T("Item%d"), i);
		CXmlNode nodeItem;
		nodeItem = m_xml.GetRoot()->GetChild(strItem);
		if (!nodeItem.IsNull())
		{
			CString strText = nodeItem.GetAttribute(_T("text"));
			CString strRow = nodeItem.GetAttribute(_T("row"));
			int nRow = _wtoi(strRow);
			CString strCol = nodeItem.GetAttribute(_T("col"));
			int nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strText);
		}
	}
	////���뼣��
	//for (int i = 1; i <= nWaveNumber; i++)
	//{
	//	//�ж��Ѿ��������ߵ�����
	//	if (i > g_sorFileArray.totalCurveNumber)
	//		return;

	//	CString strTrace;
	//	strTrace.Format(_T("Trace%d"), i);
	//	CXmlNode nodeTrace;
	//	nodeTrace = m_xml.GetRoot()->GetChild(strPageName)->GetChild(_T("Traces"))->GetChild(strTrace);
	//	if (!nodeTrace.IsNull())
	//	{
	//		CString strBmpFile;
	//		strBmpFile.Format(_T("%s\\%s"), CUtil::getExePath(), nodeTrace.GetAttribute(_T("file")));
	//		CString strPos = nodeTrace.GetAttribute(_T("fromcell"));
	//		if (CUtil::isFileExist(strBmpFile)) //λͼ�ļ�����
	//			m_ExcelUtil.InsertPicture(strBmpFile, strPos);
	//	}

	//	//��Ӽ��߱�����Ŀ1-3
	//	for (int i = 1; i <= 3; i++)
	//	{
	//		CString strItem;
	//		strItem.Format(_T("Item%d"), i);
	//		CXmlNode nodeItem;
	//		nodeItem = nodeTrace.GetChild(strItem);
	//		if (!nodeItem.IsNull())
	//		{
	//			CString strText = nodeItem.GetAttribute(_T("text"));
	//			CString strRow = nodeItem.GetAttribute(_T("row"));
	//			int nRow = _wtoi(strRow);
	//			CString strCol = nodeItem.GetAttribute(_T("col"));
	//			int nCol = _wtoi(strCol);
	//			m_ExcelUtil.InsertItem(nRow, nCol, strText);
	//		}
	//	}
	//}

	////д���¼��б�
	//if (nWaveNumber == 1)
	//{

	//}
}

void CPropStep6::Export1Curve() //ÿҳ1������
{
	WriteDataToExcel(1, TRUE);  //�±���
}

void CPropStep6::Export2Curve() //ÿҳ2������
{
	//WriteDataToExcel(2);
	WriteDataToExcel(2, TRUE);  //�±���
}

void CPropStep6::Export4Curve() //ÿҳ4������
{
	WriteDataToExcel(4, TRUE);
}

void CPropStep6::Export6Curve() //ÿҳ6������
{
	WriteDataToExcel(6, TRUE);
}

void CPropStep6::Export8Curve() //ÿҳ8������
{
	WriteDataToExcel(8, TRUE);
}

void CPropStep6::WriteDataToExcel(int nWaveNumber) //ÿҳnWaveNumber������
{
	//���뼣��
	//for (int i = 1; i <= nWaveNumber; i++) //zll 2012.9.24ע��
	int SheetCount=1;
	for (int i = 1; i <= g_sorFileArray.totalCurveNumber; i++)//zll 2012.9.24 �ɵ�������8��������
	{
		int wave_Count=i;
		//�ж��Ѿ��������ߵ�����
		/*if (i > g_sorFileArray.totalCurveNumber)
			return;*/
		if (i-nWaveNumber>0 && i-nWaveNumber*SheetCount>0)
		{
			SheetCount++;
		}
		CString strPageName;
		strPageName.Format(_T("P%d"), nWaveNumber);

		CString strTrace;
		strTrace.Format(_T("Trace%d"), i-(SheetCount-1)*nWaveNumber);//��Ϊ����� cwcq2012.11.05
		//strTrace.Format(_T("trace%d"), i-(SheetCount-1)*nWaveNumber);//wcq2012.11.05 trace
		CXmlNode nodeTrace;
		nodeTrace = m_xml.GetRoot()->GetChild(strPageName)->GetChild(_T("Traces"))->GetChild(strTrace);
		if (!nodeTrace.IsNull())
		{
			//strTrace.Format(_T("Trace%d.bmp"), i);//��Ϊ����� cwcq2012.11.05
			strTrace.Format(_T("trace%d.bmp"), i);//wcq2012.11.05
			CString strBmpFile;
			strBmpFile.Format(_T("%s\\%s"), CUtil::getExePath(), strTrace);
			//strBmpFile.Format(_T("%s\\%s"), CUtil::getExePath(), nodeTrace.GetAttribute(_T("file")));//zllע�� 2012.9.26
			CString strPos = nodeTrace.GetAttribute(_T("fromcell"));
			//AfxMessageBox(strBmpFile);//wcq2012.11.05
			
			if (CUtil::isFileExist(strBmpFile)) //λͼ�ļ�����
			{
				m_ExcelUtil.InsertPicture(strBmpFile, strPos,SheetCount,0,0,1);//zll 2013.5.28 ����iBorder��ֵ��ʹ�����excelʱ�����б߿���߲���
				//m_ExcelUtil.InsertPicture(strBmpFile, _T("A38"),SheetCount);//��ʱ����һ�� wcq2012.11.05???????????????????
			}
		}

		// �������
		CXmlNode nodeTitle;
		nodeTitle = m_xml.GetRoot()->GetChild(strPageName)->GetChild(_T("Title"));
		if (!nodeTitle.IsNull())
		{
			//CString strTitle = nodeTitle.GetAttribute(_T("text"));
			CString strTitle = g_sorFileArray.reportConfig.Title;
			CString strRow = nodeTitle.GetAttribute(_T("row"));
			int nRow = _wtoi(strRow);
			CString strCol = nodeTitle.GetAttribute(_T("col"));
			int nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strTitle);
		}

		if (nWaveNumber != 8)//ÿҳ8������ʱ��д�������
		{
			//������Ŀ1-12
			for (int i = 1; i <= 12; i++)
			{
				CString strItem;
				strItem.Format(_T("Item%d"), i);
				CXmlNode nodeItem;
				nodeItem = m_xml.GetRoot()->GetChild(strPageName)->GetChild(strItem);
				if (!nodeItem.IsNull())
				{
					CString strText = nodeItem.GetAttribute(_T("text"));
					CString strTextVal;
					strTextVal.Format(_T("%s:  %s"),strText,GetItemValById(i));
					CString strRow = nodeItem.GetAttribute(_T("row"));
					int nRow = _wtoi(strRow);
					CString strCol = nodeItem.GetAttribute(_T("col"));
					int nCol = _wtoi(strCol);
					m_ExcelUtil.InsertItem(nRow, nCol, strTextVal);
				}
			}


			//д���¼��б� zll 2013.5.24
			WriteEventList(nWaveNumber,i);
		}

		int filePathNameLength=g_sorFileArray.sorFileList[i-1].filePathName.GetLength();
		int fileNameLength=g_sorFileArray.sorFileList[i-1].fileName.GetLength();
        CString sorName=g_sorFileArray.sorFileList[i-1].fileName.Mid(filePathNameLength+1,fileNameLength-(filePathNameLength+1));
        SetSorName(nWaveNumber,i,sorName);//����.sor�����ļ���
		SetVerUnitText(nWaveNumber,i);//����������̶��ı�
		SetHorUnitText(nWaveNumber,i);//���ú�����̶��ı�

		//��Ӽ��߱�����Ŀ1-3
		//for (int i = 1; i <= 3; i++)
		for (int i = 2; i < 3; i++)
		{
			CString strItem;
			strItem.Format(_T("Item%d"), i);
			CXmlNode nodeItem;
			nodeItem = nodeTrace.GetChild(strItem);
			if (!nodeItem.IsNull())
			{
				CString strText = nodeItem.GetAttribute(_T("text"));
				CString str_wave_Count=_T("");
				str_wave_Count.Format(_T("%d"),wave_Count);
				strText+=_T(":")+str_wave_Count;
				CString strRow = nodeItem.GetAttribute(_T("row"));
				int nRow = _wtoi(strRow);
				CString strCol = nodeItem.GetAttribute(_T("col"));
				int nCol = _wtoi(strCol);
				m_ExcelUtil.InsertItem(nRow, nCol, strText);
			}
		}
		m_progress.SetPos(i);
		TRACE(_T("zll zll %d\n"),i);
	}
}

//�±������õ�Excel����� lzy 2014.8.22
void CPropStep6::WriteDataToExcel(int nWaveNumber, BOOL bNewRpt) //ÿҳnWaveNumber������, bNewRpt=1��ʾ�±���
{
	if(!bNewRpt)//�ɱ���ֱ�ӵ����ϵĺ���
	{
		WriteDataToExcel(nWaveNumber);
		return;
	}

	int nLineOffset = 60;  //Excel�������ڱ��������ƫ��ֵ
	
	//MAIN->m_pPrintView->GetPrintInfo(g_sorFileArray.curSelection);  //��ȡ��ӡ��һЩ��Ϣ
	
	//���뼣��
	//for (int i = 1; i <= nWaveNumber; i++) //zll 2012.9.24ע��
	//int nCurveCount = g_sorFileArray.totalCurveNumber;    //lzy 2014.9.4
	int SheetCount=1;
	int j = 0;   //��ҳ�ϵڼ���ͼ
	for (int i = 1; i <= g_sorFileArray.totalCurveNumber; i++)//zll 2012.9.24 �ɵ�������8��������
	{
		if(1==nWaveNumber && i!=g_sorFileArray.curSelection+1) //������Ļ���ֻ��ӡ��ǰ��ѡ����ļ���lzy2014.9.3
			continue;
		
		if(1==nWaveNumber)
			MAIN->m_pPrintView->GetPrintInfo(g_sorFileArray.curSelection);  //��ȡ��ӡ��һЩ��Ϣ
		else
			MAIN->m_pPrintView->GetPrintInfo( i - 1 );  //��ȡ��ӡ��һЩ��Ϣ

		int wave_Count=i;
		//�ж��Ѿ��������ߵ�����
		/*if (i > g_sorFileArray.totalCurveNumber)
			return;*/
		if (i-nWaveNumber>0 && i-nWaveNumber*SheetCount>0)
		{
			SheetCount++;
		}
		CString strPageName;
		strPageName.Format(_T("P%d"), nWaveNumber);

		CString strTrace;
		//strTrace.Format(_T("Trace%d"), i-(SheetCount-1)*nWaveNumber);//��Ϊ����� cwcq2012.11.05
		strTrace.Format(_T("Trace1"));  //, i-(SheetCount-1)*nWaveNumber);  //�̶�ΪTrace1, ��Ϊÿ���ӱ������ݶ���ͬ��lzy2014.9.1

		j = (i-1) % nWaveNumber;//����ͼ֮���ƫ����
		
		CXmlNode nodeTrace;
		nodeTrace = m_xml.GetRoot()->GetChild(strPageName)->GetChild(_T("Traces"))->GetChild(strTrace);
		if (!nodeTrace.IsNull())
		{
			//strTrace.Format(_T("Trace%d.bmp"), i);//��Ϊ����� cwcq2012.11.05
			strTrace.Format(_T("trace%d.bmp"), i);//wcq2012.11.05
			CString strBmpFile;
			strBmpFile.Format(_T("%s\\%s"), CUtil::getExePath(), strTrace);
			//strBmpFile.Format(_T("%s\\%s"), CUtil::getExePath(), nodeTrace.GetAttribute(_T("file")));//zllע�� 2012.9.26
			CString strPos = nodeTrace.GetAttribute(_T("fromcell"));
			//AfxMessageBox(strBmpFile);//wcq2012.11.05
			if(i > 1)  //lzy2014.9.3
			{
				CString str = strPos.Right(2);
				int nPos = _wtoi(str) +  nLineOffset * j ; //*(i - 1) 
				strPos.Format(_T("A%d"), nPos);
			}

			if (CUtil::isFileExist(strBmpFile)) //λͼ�ļ�����
			{
				m_ExcelUtil.InsertPicture(strBmpFile, strPos,SheetCount,0,0,1);//zll 2013.5.28 ����iBorder��ֵ��ʹ�����excelʱ�����б߿���߲���
				//m_ExcelUtil.InsertPicture(strBmpFile, _T("A38"),SheetCount);//��ʱ����һ�� wcq2012.11.05???????????????????
			}
		}

		// �������
		CXmlNode nodeTitle, nodeItem, nodeSub;
		CString strItem, strHead, strTitle, strRow, strCol; 
		nodeTitle = m_xml.GetRoot()->GetChild(strPageName)->GetChild(_T("Title"));
		
		if (!nodeTitle.IsNull())
		{
			//CString strTitle = nodeTitle.GetAttribute(_T("text"));
			strTitle = g_sorFileArray.reportConfig.Title; 
			strRow = nodeTitle.GetAttribute(_T("row"));
			int nRow = _wtoi(strRow) + nLineOffset * j; //( i - 1 ) ;
			strCol = nodeTitle.GetAttribute(_T("col"));
			int nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strTitle);

			nodeItem = nodeTitle.GetChild(_T("Logo"));
			strItem = nodeItem.GetAttribute(_T("text"));
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j; //( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("PrintDate"));
			strHead = nodeItem.GetAttribute(_T("text"));
			CTime tm = CTime::GetCurrentTime();
			CString strTime = tm.Format(_T("%Y-%m-%d %H:%M:%S"));
			strItem = strHead + _T(": ") + strTime;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j; //( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);
		}

		//����Job info
		nodeTitle = m_xml.GetRoot()->GetChild(strPageName)->GetChild(_T("Job"));
		if (!nodeTitle.IsNull())
		{
			strTitle = _T("<") + nodeTitle.GetAttribute(_T("text")) + _T(">");   //����JOB���� 
			strRow = nodeTitle.GetAttribute(_T("row"));
			int nRow = _wtoi(strRow) + nLineOffset * j; //( i - 1 );
			strCol = nodeTitle.GetAttribute(_T("col"));
			int nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strTitle);

			nodeItem = nodeTitle.GetChild(_T("JobID"));  //����������
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Job.JobID;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("File"));  //����sor�ļ���
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Job.SorFileName;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("Date"));  //�������ʱ��
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Job.TestDate;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("Contractor"));  //����а���
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Job.Contractor;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("Customer"));  //�����û�
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Job.Customer;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("Operator"));  //���������
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Job.Operator;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);			
		}

		//����Configuration info
		nodeTitle = m_xml.GetRoot()->GetChild(strPageName)->GetChild(_T("Configuration"));
		if (!nodeTitle.IsNull())
		{
			strTitle = _T("<") + nodeTitle.GetAttribute(_T("text")) + _T(">");   //����Configuration���� 
			strRow = nodeTitle.GetAttribute(_T("row"));
			int nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeTitle.GetAttribute(_T("col"));
			int nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strTitle);

			nodeItem = nodeTitle.GetChild(_T("Wavelength"));  //���벨��
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.Wavelength;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("Range"));  //�������
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.Range;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("Backscatter"));  //����ɢ��ϵ��
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.BackScatter;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("Pulse"));  //����
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.Pulse;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("Average"));  //ƽ��
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.Average;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("LossThreshold"));  //�������
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.LossThreshold;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem =  nodeTitle.GetChild(_T("Origin")) ;  //���
			strItem = _T("---") + nodeItem.GetAttribute(_T("text")) + _T("---") ;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem =  nodeTitle.GetChild(_T("End")) ;  //�յ�
			strItem = _T("---") + nodeItem.GetAttribute(_T("text")) + _T("---");
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("ReflectThreshold"));  //��������
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.RefThreshold;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("OriginLocation"));  //���λ��
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.OriLocation;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("EndLocation"));  //�յ�λ��
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.EndLocation;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("EndThreshold"));  //����ĩ������
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.EndThreshold;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("OriginCable"));  //�����±��
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.OriCable;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("EndCable"));  //�յ���±��
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.EndCable;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("Index"));  //������
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.Index;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("OriginFiber"));  //�����˱��
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.OriFiber;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("EndFiber"));  //�յ���˱��
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.EndFiber;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("OriginColor"));  //��������ɫ
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.OriColor;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("EndColor"));  //�յ������ɫ
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.EndColor;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

		}

		//���Comment
		nodeTitle = m_xml.GetRoot()->GetChild(strPageName)->GetChild(_T("Comment"));
		if (!nodeTitle.IsNull())
		{
			strTitle = _T("<") + nodeTitle.GetAttribute(_T("text")) + _T(">");   //����Comment���� 
			strRow = nodeTitle.GetAttribute(_T("row"));
			int nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeTitle.GetAttribute(_T("col"));
			int nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strTitle);

			nodeItem = nodeTitle.GetChild(_T("CommentContent"));  //����ע������
			strItem =  g_sorFileArray.reportConfig.Comment;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);
		}

		//���Traceͼ
		nodeTitle = m_xml.GetRoot()->GetChild(strPageName)->GetChild(_T("Traces"));
		if (!nodeTitle.IsNull())
		{
			strTitle = _T("<") + nodeTitle.GetAttribute(_T("text")) + _T(">");   //����Trace���� 
			strRow = nodeTitle.GetAttribute(_T("row"));
			int nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeTitle.GetAttribute(_T("col"));
			int nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strTitle);

			nodeItem = nodeTitle.GetChild(_T("ItemA"));  //���A
			strItem = nodeItem.GetAttribute(_T("text"));
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeSub = nodeItem.GetChild(_T("Location")); //���A��λ��
			strItem = nodeSub.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Trace.LocationA;// + nodeSub.GetAttribute(_T("unit"));
			strRow = nodeSub.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeSub.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeSub = nodeItem.GetChild(_T("Loss")); //���A�����ֵ
			strItem = nodeSub.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Trace.LossA ; //+ nodeSub.GetAttribute(_T("unit"));
			strRow = nodeSub.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeSub.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("ItemB"));  //���B
			strItem = nodeItem.GetAttribute(_T("text"));
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeSub = nodeItem.GetChild(_T("Location")); //���B��λ��
			strItem = nodeSub.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Trace.LocationB ; //+ nodeSub.GetAttribute(_T("unit"));
			strRow = nodeSub.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeSub.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeSub = nodeItem.GetChild(_T("Loss")); //���B�����ֵ
			strItem = nodeSub.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Trace.LossB ; //+ nodeSub.GetAttribute(_T("unit"));
			strRow = nodeSub.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeSub.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("ItemAB"));  //���AB
			strItem = nodeItem.GetAttribute(_T("text"));
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeSub = nodeItem.GetChild(_T("DistanceAB")); //���AB��λ��
			strItem = nodeSub.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Trace.DistanceAB + nodeSub.GetAttribute(_T("unit"));
			strRow = nodeSub.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeSub.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeSub = nodeItem.GetChild(_T("LossAB")); //���AB�����ֵ��ֵ
			strItem = nodeSub.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Trace.LossAB + nodeSub.GetAttribute(_T("unit"));
			strRow = nodeSub.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeSub.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeSub = nodeItem.GetChild(_T("SlopeAB")); //���AB���˥��б��
			strItem = nodeSub.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Trace.SlopeAB + nodeSub.GetAttribute(_T("unit"));
			strRow = nodeSub.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeSub.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

		}

		
		//д���¼��б� zll 2013.5.24
		nodeTitle = m_xml.GetRoot()->GetChild(strPageName)->GetChild(_T("EventList"));
		if (!nodeTitle.IsNull())
		{
			nodeItem = nodeTitle.GetChild(_T("Title"));
			strTitle = _T("<") + nodeItem.GetAttribute(_T("text")) + _T(">");   //�����¼��б���� 
			strRow = nodeItem.GetAttribute(_T("row"));
			int nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			int nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strTitle);
			//WriteEventList(nWaveNumber,i, 41 + nLineOffset * ( i - 1 ));
			WriteEventList(1,i, 41 + nLineOffset * j);//( i - 1 ));   //��һ�������̶�Ϊ1��ƫ�����ɵ���������������lzy2014.9.3
		}	
	
		m_progress.SetPos(i);
	}
}

//����¼��б�lzy 2014.8.25 ��������λ�ò���
void CPropStep6::WriteEventList(int m_nWaveNumberPerPage,int m_nCurCurve, int nRow)
{
	float preDistance=0;
	WriteEventHead(m_nWaveNumberPerPage,m_nCurCurve, nRow);//�¼��б��ͷ
	int nFirst = nRow + 1;

	TestResultInfo* pData = g_sorFileArray.sorFileList[m_nCurCurve-1].pTestResultInfo;
	if (NULL == pData)
		return;

	CString strText=_T("");
	CRect textRect;
	std::vector<EventInfo> ve = pData->EventList.vEventInfo;//�ٶ�??
	int nSize=ve.size();
	for (int i=0;i<nSize;i++)//
	{
		// ���
		{
			//����
			strText.Format(_T("%d"),i+1);

			if (g_currentLang == LANG::CHS)
			{
				if (0 == ve[i].nEventType)
				{
					strText += _T(" �Ƿ���");//0����Ƿ����¼�����
				}
				else if (1 == ve[i].nEventType)
				{
					strText += _T(" ����");;//1�������¼�����
				}
			}
			else if (g_currentLang == LANG::ENG)
			{
				if (0 == ve[i].nEventType)
				{
					strText += _T(" NoReflect");//0����Ƿ����¼�����
				}
				else if (1 == ve[i].nEventType)
				{
					strText += _T(" Reflect");;//1�������¼�����
				}
			}
			

			if (_T('S')==ve[i].cEventType)
			{
				strText+=_T("(S)");
			}
			else if (_T('E')==ve[i].cEventType)
			{
				strText+=_T("(E)");
			}
			else if (_T('A')==ve[i].cEventType)
			{
				strText+=_T("(A)");
			}
			else if (_T('F')==ve[i].cEventType)
			{
				strText+=_T("(F)");
			}
			else if (_T('O')==ve[i].cEventType)
			{
				strText+=_T("(O)");
			}

			if (1==m_nWaveNumberPerPage)
			{
				m_ExcelUtil.InsertItem(nFirst+i, 1, strText);
			}
			else if (2==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(34+i, 1, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(66+i, 1, strText);
				}
			}
			else if (4==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 1, strText);
				}
				else if (2==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 8, strText);
				}
				else if (3==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 1, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 8, strText);
				}
			}
		}

		// ����
		{
			float m_float_zll=0.0f;
			if (0==i)
			{
				strText.Format(_T("%.3f"),0.0f);//����
			}
			else
			{
				if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
					strText.Format(_T("%.3f"),ve[i].fLocation/1000);//����
				else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪmiles
				{
					m_float_zll=KmeterToOther(ve[i].fLocation/1000,UNIT_SHOW_MILE);
					strText.Format(_T("%.3f"),m_float_zll);//����
				}
				else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkft
				{
					m_float_zll=KmeterToOther(ve[i].fLocation/1000,UNIT_SHOW_KFEET);
					strText.Format(_T("%.3f"),m_float_zll);//����
				}
			}
			if (1==m_nWaveNumberPerPage)
			{
				m_ExcelUtil.InsertItem(nFirst+i, 3, strText);
			}
			else if (2==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(34+i, 3, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(66+i, 3, strText);
				}
			}
			else if (4==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 2, strText);
				}
				else if (2==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 9, strText);
				}
				else if (3==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 2, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 9, strText);
				}
			}
			
		}
		// ��
		{
			float a,b=0.0f;
			if (0==i)
			{
				strText.Format(_T("%.3f"),0.0f);//��
			}
			else
			{
				if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
					strText.Format(_T("%.3f"),(ve[i].fLocation-preDistance)/1000);//��,���¼�������
				else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪmiles
				{
					b=(ve[i].fLocation-preDistance)/1000;
					a=KmeterToOther(b,UNIT_SHOW_MILE);
					strText.Format(_T("%.3f"),a);//��,���¼�������
				}
				else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkft
				{
					b=(ve[i].fLocation-preDistance)/1000;
					a=KmeterToOther(b,UNIT_SHOW_KFEET);
					strText.Format(_T("%.3f"),a);//��,���¼�������
				}
			}
			if (0==i)
			{
				preDistance = 0.0f;
			}
			else
			{
				preDistance = ve[i].fLocation;
			}

			if (1==m_nWaveNumberPerPage)
			{
				m_ExcelUtil.InsertItem(nFirst + i, 4, strText);
			}
			else if (2==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(34+i, 4, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(66+i, 4, strText);
				}
			}
			else if (4==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 3, strText);
				}
				else if (2==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 10, strText);
				}
				else if (3==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 3, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 10, strText);
				}
			}
			
		}
		// ���
		{
			if (0 == ve[i].fEventloss)//Ϊ0ʱ��ʾ"-"wcq2012.07.09
			{
				strText.Format(_T("--"));//�����
			}
			else
			{
				strText.Format(_T("%.3f"), ve[i].fEventloss);//���
			}

			if (1==m_nWaveNumberPerPage)
			{
				m_ExcelUtil.InsertItem(nFirst+i, 5, strText);
			}
			else if (2==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(34+i, 5, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(66+i, 5, strText);
				}
			}
			else if (4==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 4, strText);
				}
				else if (2==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 11, strText);
				}
				else if (3==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 4, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 11, strText);
				}
			}
			
		}
		// �����
		{
			if (0 == ve[i].fTotalLoss)//Ϊ0ʱ��ʾ"-"wcq2012.07.09
			{
				strText.Format(_T("--"));//�����
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fTotalLoss);//�����
			}

			if (1==m_nWaveNumberPerPage)
			{
				m_ExcelUtil.InsertItem(nFirst + i, 6, strText);
			}
			else if (2==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(34+i, 6, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(66+i, 6, strText);
				}
			}
			else if (4==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 5, strText);
				}
				else if (2==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 12, strText);
				}
				else if (3==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 5, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 12, strText);
				}
			}
		}
		// б��
		{
			//б��
			if (fabs(ve[i].fLineK)>0.5)//zll 2012.10.31
			{
				strText.Format(_T("--"));
			}
			else if (0 == ve[i].fLineK)//Ϊ0ʱ��ʾ"-" zll 2012.10.31
			{
				strText.Format(_T("--"));
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fLineK);//б��
			}

			if (1==m_nWaveNumberPerPage)
			{
				m_ExcelUtil.InsertItem(nFirst + i, 7, strText);
			}
			else if (2==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(34+i, 7, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(66+i, 7, strText);
				}
			}
			else if (4==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 6, strText);
				}
				else if (2==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 13, strText);
				}
				else if (3==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 6, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 13, strText);
				}
			}
		}
		// ����
		{
			if (0 == ve[i].nEventType)//�Ƿ���
			{
				strText=_T("--");
			}
			else if (0 == ve[i].fEventHigh)//Ϊ0ʱ��ʾ"-"wcq2012.07.09
			{
				strText=_T("--");
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fEventHigh);//����
			}
			if (1==m_nWaveNumberPerPage)
			{
				m_ExcelUtil.InsertItem(nFirst + i, 8, strText);
			}
			else if (2==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(34+i, 8, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(66+i, 8, strText);
				}
			}
			else if (4==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 7, strText);
				}
				else if (2==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 14, strText);
				}
				else if (3==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 7, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 14, strText);
				}
			}
		}
	}
}

void CPropStep6::WriteEventHead(int m_nWaveNumberPerPage,int m_nCurCurve, int nRow)
{
	//���Ʊ�����
	LV_COLUMN lvColumn;
	TCHAR uin[50];
	lvColumn.pszText=uin;
	lvColumn.cchTextMax=50;
	lvColumn.mask=LVCF_TEXT;
	CString header=_T("");
	int nLen = MAIN->m_pEventFormView->m_Eventlist.GetHeaderCtrl()->GetItemCount();//�õ�����
	//��ͷ����
	if (nLen > 8) //���9��
	{
		nLen = 8;
	}
	CRect textRect;
	for(int i=0;i<nLen;i++)
	{
		if(MAIN->m_pEventFormView->m_Eventlist.GetColumn(i,&lvColumn))
		{
			header=lvColumn.pszText;
			if (1==m_nWaveNumberPerPage)
			{
				if (0==i)
				{
					m_ExcelUtil.InsertItem(nRow, i+1, header);
				}
				else if (0!=i)
				{
					m_ExcelUtil.InsertItem(nRow, i+2, header);
				}
			}
			else if (2==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					if (0==i)
					{
						m_ExcelUtil.InsertItem(33, i+1, header);
					}
					else if (0!=i)
					{
						m_ExcelUtil.InsertItem(33, i+2, header);
					}
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					if (0==i)
					{
						m_ExcelUtil.InsertItem(65, i+1, header);
					}
					else if (0!=i)
					{
						m_ExcelUtil.InsertItem(65, i+2, header);
					}
				}
			}
			else if (4==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(29, i+1, header);
				}
				else if (2==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(62, i+1, header);
				}
				else if (3==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(29, i+8, header);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(62, i+8, header);
				}
			}
		}
	}
}

void CPropStep6::SetHorUnitText(int m_nWaveNumberPerPage,int m_nCurCurve)
{
	if (g_sorFileArray.totalCurveNumber == 0) //û�����߼���
		return;

	//����ѡ������
	TestResultInfo* pData = g_sorFileArray.sorFileList[m_nCurCurve-1].pTestResultInfo;
	if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
	{
		//���ú������ı� ��ʼ����
		float fStartVal = MAIN->m_pWaveFormView->m_sorDrawStatic.m_fRangeX[OTDR_TRACE_MIN]/1000;//zll 2013.5.10
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.3fkm"), fStartVal);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.4fkm"), fStartVal);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.5fkm"), fStartVal);

		//���ú������ı� ��������
		float fEndVal = pData->Length/1000;//zll 2013.5.10
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.3fkm"), fEndVal);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.4fkm"), fEndVal);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.5fkm"), fEndVal);

		//���ú������ı� ��λ����
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.3fkm/div"), (fEndVal-fStartVal)/10);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.4fkm/div"), (fEndVal-fStartVal)/10);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.5fkm/div"), (fEndVal-fStartVal)/10);

		GetHorLocation(m_nWaveNumberPerPage,m_nCurCurve,m_strStartDis,m_strUnitDis,m_strEndDis);
	}
	else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪmiles
	{
		//���ú������ı� ��ʼ����
		float fStartVal = MAIN->m_pWaveFormView->m_sorDrawStatic.m_fRangeX[OTDR_TRACE_MIN]/1000;//zll 2013.5.10
		fStartVal=KmeterToOther(fStartVal,UNIT_SHOW_MILE);
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.3fmiles"), fStartVal);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.4fmiles"), fStartVal);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.5fmiles"), fStartVal);

		//���ú������ı� ��������
		float fEndVal = pData->Length/1000;//zll 2013.5.10
		fEndVal=KmeterToOther(fEndVal,UNIT_SHOW_MILE);
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.3fmiles"), fEndVal);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.4fmiles"), fEndVal);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.5fmiles"), fEndVal);

		//���ú������ı� ��λ����
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.3fmiles/div"), (fEndVal-fStartVal)/10);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.4fmiles/div"), (fEndVal-fStartVal)/10);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.5fmiles/div"), (fEndVal-fStartVal)/10);

		GetHorLocation(m_nWaveNumberPerPage,m_nCurCurve,m_strStartDis,m_strUnitDis,m_strEndDis);
	}
	else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkft
	{
		//���ú������ı� ��ʼ����
		float fStartVal = MAIN->m_pWaveFormView->m_sorDrawStatic.m_fRangeX[OTDR_TRACE_MIN]/1000;//zll 2013.5.10
		fStartVal=KmeterToOther(fStartVal,UNIT_SHOW_KFEET);
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.3fkft"), fStartVal);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.4fkft"), fStartVal);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.5fkft"), fStartVal);

		//���ú������ı� ��������
		float fEndVal = pData->Length/1000;//zll 2013.5.10
		fEndVal=KmeterToOther(fEndVal,UNIT_SHOW_KFEET);
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.3fkft"), fEndVal);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.4fkft"), fEndVal);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.5fkft"), fEndVal);

		//���ú������ı� ��λ����
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.3fkft/div"), (fEndVal-fStartVal)/10);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.4fkft/div"), (fEndVal-fStartVal)/10);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.5fkft/div"), (fEndVal-fStartVal)/10);

		GetHorLocation(m_nWaveNumberPerPage,m_nCurCurve,m_strStartDis,m_strUnitDis,m_strEndDis);
	}
}
void CPropStep6::SetVerUnitText(int m_nWaveNumberPerPage,int m_nCurCurve)
{
	if (g_sorFileArray.totalCurveNumber == 0) //û�����߼���
		return;

	//�����������ı� ��λ����
	float fUnitDiv = (MAIN->m_pWaveFormView->m_sorDrawStatic.m_fRangeY[OTDR_TRACE_MAX] - MAIN->m_pWaveFormView->m_sorDrawStatic.m_fRangeY[OTDR_TRACE_MIN]) / COUNT_GRIDLINE_VERTICAL;
	if(1==g_sorFileArray.waveConfig.UnitDbFig)
		m_strUnitDiv.Format(_T("%.1f"), fUnitDiv);
	else if(2==g_sorFileArray.waveConfig.UnitDbFig)
		m_strUnitDiv.Format(_T("%.2f"), fUnitDiv);
	else if(3==g_sorFileArray.waveConfig.UnitDbFig)
		m_strUnitDiv.Format(_T("%.3f"), fUnitDiv);

	if (1==m_nWaveNumberPerPage)
	{
		m_ExcelUtil.InsertItem(13, 1, m_strUnitDiv);
		m_ExcelUtil.InsertItem(14, 1, _T("dB/Div"));
	}
	else if (2==m_nWaveNumberPerPage)
	{
		if (1==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(13, 1, m_strUnitDiv);
			m_ExcelUtil.InsertItem(14, 1, _T("dB/Div"));
		}
		else if (0==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(45, 1, m_strUnitDiv);
			m_ExcelUtil.InsertItem(46, 1, _T("dB/Div"));
		}
	}
	else if (4==m_nWaveNumberPerPage)
	{
		if (1==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(11, 1, m_strUnitDiv+_T("dB/Div"));
		}
		else if (2==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(11, 8, m_strUnitDiv+_T("dB/Div"));
		}
		else if (3==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(44, 1, m_strUnitDiv+_T("dB/Div"));
		}
		else if (0==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(44, 8, m_strUnitDiv+_T("dB/Div"));
		}
	}
	else if (6==m_nWaveNumberPerPage)
	{
		if (1==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(11, 1, m_strUnitDiv+_T("dB/Div"));
		}
		else if (2==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(11, 8, m_strUnitDiv+_T("dB/Div"));
		}
		else if (3==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(33, 1, m_strUnitDiv+_T("dB/Div"));
		}
		else if (4==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(33, 8, m_strUnitDiv+_T("dB/Div"));
		}
		else if (5==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(55, 1, m_strUnitDiv+_T("dB/Div"));
		}
		else if (0==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(55, 8, m_strUnitDiv+_T("dB/Div"));
		}
	}
	else if (8==m_nWaveNumberPerPage)
	{
		if (1==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(4, 1, m_strUnitDiv+_T("dB/Div"));
		}
		else if (2==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(4, 8, m_strUnitDiv+_T("dB/Div"));
		}
		else if (3==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(25, 1, m_strUnitDiv+_T("dB/Div"));
		}
		else if (4==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(25, 8, m_strUnitDiv+_T("dB/Div"));
		}
		else if (5==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(46, 1, m_strUnitDiv+_T("dB/Div"));
		}
		else if (6==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(46, 8, m_strUnitDiv+_T("dB/Div"));
		}
		else if (7==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(67, 1, m_strUnitDiv+_T("dB/Div"));
		}
		else if (0==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(67, 8, m_strUnitDiv+_T("dB/Div"));
		}
	}
}

void CPropStep6::GetHorLocation(int m_nWaveNumberPerPage,int m_nCurCurve,CString m_strStartDis,CString m_strUnitDis,CString m_strEndDis)
{
	if (1==m_nWaveNumberPerPage)
	{
		m_ExcelUtil.InsertItem(29, 2, m_strStartDis);
		m_ExcelUtil.InsertItem(29, 5, m_strUnitDis);
		m_ExcelUtil.InsertItem(29, 8, m_strEndDis);
	}
	else if (2==m_nWaveNumberPerPage)
	{
		if (1==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(31, 2, m_strStartDis);
			m_ExcelUtil.InsertItem(31, 5, m_strUnitDis);
			m_ExcelUtil.InsertItem(31, 8, m_strEndDis);
		}
		else if (0==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(63, 2, m_strStartDis);
			m_ExcelUtil.InsertItem(63, 5, m_strUnitDis);
			m_ExcelUtil.InsertItem(63, 8, m_strEndDis);
		}
	}
	else if (4==m_nWaveNumberPerPage)
	{
		if (1==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(27, 1, m_strStartDis);
			m_ExcelUtil.InsertItem(27, 3, m_strUnitDis);
			m_ExcelUtil.InsertItem(27, 6, m_strEndDis);
		}
		else if (2==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(27, 8, m_strStartDis);
			m_ExcelUtil.InsertItem(27, 10, m_strUnitDis);
			m_ExcelUtil.InsertItem(27, 13, m_strEndDis);
		}
		else if (3==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(60, 1, m_strStartDis);
			m_ExcelUtil.InsertItem(60, 3, m_strUnitDis);
			m_ExcelUtil.InsertItem(60, 6, m_strEndDis);
		}
		else if (0==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(60, 8, m_strStartDis);
			m_ExcelUtil.InsertItem(60, 10, m_strUnitDis);
			m_ExcelUtil.InsertItem(60, 13, m_strEndDis);
		}
	}
	else if (6==m_nWaveNumberPerPage)
	{
		if (1==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(28, 1, m_strStartDis);
			m_ExcelUtil.InsertItem(28, 3, m_strUnitDis);
			m_ExcelUtil.InsertItem(28, 6, m_strEndDis);
		}
		else if (2==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(28, 8, m_strStartDis);
			m_ExcelUtil.InsertItem(28, 10, m_strUnitDis);
			m_ExcelUtil.InsertItem(28, 13, m_strEndDis);
		}
		else if (3==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(50, 1, m_strStartDis);
			m_ExcelUtil.InsertItem(50, 3, m_strUnitDis);
			m_ExcelUtil.InsertItem(50, 6, m_strEndDis);
		}
		else if (4==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(50, 8, m_strStartDis);
			m_ExcelUtil.InsertItem(50, 10, m_strUnitDis);
			m_ExcelUtil.InsertItem(50, 13, m_strEndDis);
		}
		else if (5==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(72, 1, m_strStartDis);
			m_ExcelUtil.InsertItem(72, 3, m_strUnitDis);
			m_ExcelUtil.InsertItem(72, 6, m_strEndDis);
		}
		else if (0==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(72, 8, m_strStartDis);
			m_ExcelUtil.InsertItem(72, 10, m_strUnitDis);
			m_ExcelUtil.InsertItem(72, 13, m_strEndDis);
		}
	}
	else if (8==m_nWaveNumberPerPage)
	{
		if (1==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(20, 1, m_strStartDis);
			m_ExcelUtil.InsertItem(20, 3, m_strUnitDis);
			m_ExcelUtil.InsertItem(20, 6, m_strEndDis);
		}
		else if (2==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(20, 8, m_strStartDis);
			m_ExcelUtil.InsertItem(20, 10, m_strUnitDis);
			m_ExcelUtil.InsertItem(20, 13, m_strEndDis);
		}
		else if (3==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(41, 1, m_strStartDis);
			m_ExcelUtil.InsertItem(41, 3, m_strUnitDis);
			m_ExcelUtil.InsertItem(41, 6, m_strEndDis);
		}
		else if (4==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(41, 8, m_strStartDis);
			m_ExcelUtil.InsertItem(41, 10, m_strUnitDis);
			m_ExcelUtil.InsertItem(41, 13, m_strEndDis);
		}
		else if (5==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(62, 1, m_strStartDis);
			m_ExcelUtil.InsertItem(62, 3, m_strUnitDis);
			m_ExcelUtil.InsertItem(62, 6, m_strEndDis);
		}
		else if (6==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(62, 8, m_strStartDis);
			m_ExcelUtil.InsertItem(62, 10, m_strUnitDis);
			m_ExcelUtil.InsertItem(62, 13, m_strEndDis);
		}
		else if (7==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(83, 1, m_strStartDis);
			m_ExcelUtil.InsertItem(83, 3, m_strUnitDis);
			m_ExcelUtil.InsertItem(83, 6, m_strEndDis);
		}
		else if (0==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(83, 8, m_strStartDis);
			m_ExcelUtil.InsertItem(83, 10, m_strUnitDis);
			m_ExcelUtil.InsertItem(83, 13, m_strEndDis);
		}
	}
}

void CPropStep6::SetSorName(int m_nWaveNumberPerPage,int m_nCurCurve,CString m_sorName)
{
	if (1==m_nWaveNumberPerPage)
	{
		m_ExcelUtil.InsertItem(9, 1, m_sorName);
	}
	if (2==m_nWaveNumberPerPage)
	{
		if (1==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(9, 1, m_sorName);
		}
		else if (0==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(41, 1, m_sorName);
		}
	}
	if (4==m_nWaveNumberPerPage)
	{
		if (1==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(9, 1, m_sorName);
		}
		else if (2==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(9, 8, m_sorName);
		}
		else if (3==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(42, 1, m_sorName);
		}
		else if (0==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(42, 8, m_sorName);
		}
	}
	if (6==m_nWaveNumberPerPage)
	{
		if (1==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(9, 1, m_sorName);
		}
		else if (2==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(9, 8, m_sorName);
		}
		else if (3==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(31, 1, m_sorName);
		}
		else if (4==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(31, 8, m_sorName);
		}
		else if (5==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(53, 1, m_sorName);
		}
		else if (0==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(53, 8, m_sorName);
		}
	}
	if (8==m_nWaveNumberPerPage)
	{
		if (1==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(2, 1, m_sorName);
		}
		else if (2==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(2, 8, m_sorName);
		}
		else if (3==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(23, 1, m_sorName);
		}
		else if (4==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(23, 8, m_sorName);
		}
		else if (5==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(44, 1, m_sorName);
		}
		else if (6==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(44, 8, m_sorName);
		}
		else if (7==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(65, 1, m_sorName);
		}
		else if (0==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(65, 8, m_sorName);
		}
	}
}