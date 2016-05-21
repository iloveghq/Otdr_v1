#include "StdAfx.h"
#include "Util.h"

CUtil::CUtil(void)
{
	m_newFhpCommName=_T("");
	m_standFhpCommName=_T("");
}

CUtil::~CUtil(void)
{
}
//�ж϶˿��ǲ�����һ��array
BOOL CUtil::isInArray(CStringArray& strArray,CString commname)
{
	INT it=strArray.GetSize()-1;
	for(int i=it;i>-1;i--)
	{
		if(commname==strArray.GetAt(i))
		{
			return TRUE;
		}
	}
	return FALSE;
}
/*****************************************************************************************************
��  ��  ��:GetTimeWaste
��      ��:��������ͨ��GetTickCount()��õ�ʱ�����ַ�����ʾ
��	��  ֵ:CString,ʱ������ַ���
******************************************************************************************************/
CString CUtil::GetTimeWaste(LONG longbegintime,LONG longendtime)//��ʱ
{
	CString str,str1;
	LONG t;
	t=longendtime-longbegintime;
	t%=3600000*60;
	str1.Format(_T(":%d"),t/3600000);
	str+=str1;
	t%=3600000;
	str1.Format(_T(":%d"),t/60000);
	str+=str1;
	t%=60000;
	str1.Format(_T(":%d"),t/1000);
	str+=str1;
	return str;
}
////////////////////////////////////////////////////////////////////////////////////
//��  ��  ��:getCurTimeStr
//��      ��:�õ���ǰʱ����ַ�����ʾ��bContainHour���Ƿ����ʱ:��:��
//��  ��  ֵ:bContainHour:TRUE,��������"2011-04-15 10:16:30",bContainHour:FALSE,��������"2011-04-15"
////////////////////////////////////////////////////////////////////////////////////
CString CUtil::getCurTimeStr(BOOL bContainHour)
{
	CString temp;
	CTime time=CTime::GetCurrentTime();////ʱ��
	if(bContainHour)
	{
		temp=time.Format(_T("%Y-%m-%d %H:%M:%S"));
	}
	else
	{
		temp=time.Format(_T("%Y-%m-%d"));
	}//eif
	return temp;
}
/*****************************************************************************************************
��  ��  ��:splitString
��      ��:�ֽ��ַ���
��	��  ֵ:
******************************************************************************************************/
INT CUtil::splitString(CString str,char split,CStringArray& strArray)
{
	strArray.RemoveAll();
	CString strTemp=str;
	int nIndex=0;
	while(1)
	{
		nIndex=strTemp.Find(split);
		if(nIndex>=0)
		{
			strArray.Add(strTemp.Left(nIndex));
			strTemp=strTemp.Right(strTemp.GetLength()-nIndex-1);
		}
		else
		{
			break;
		}
	}
	strArray.Add(strTemp);
	return strArray.GetSize();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
//����:�����Զ���1
//����  ֵ:���кź�lastλ��һ��õ����ַ���
///////////////////////////////////////////////////////////////////////////////////////////////////////
CString CUtil::getAddSerial(CString str,INT last)
{
	INT len=str.GetLength();
	if(len<last)//���кų��Ⱥ����λһ�����С���򲻴���
	{
		return str;
	}
	CString left=str.Left(str.GetLength()-last);
	CString arr[5]={_T("%.4d"),_T("%.5d"),_T("%.6d"),_T("%.7d"),_T("%.8d")};
	CString temp2;
	temp2.Format(arr[last-4],_ttoi(str.Right(last))+1);
	return left+temp2;
}

/*****************************************************************************************************
��  ��  ��:GetTimeWaste
��      ��://��ȡ�ı��ļ�ʱΪʱstdiofile���ܴ���������Ҫ��ת������
��	��  ֵ:
******************************************************************************************************/
void CUtil::changeLocaleToChs()
{
	old_locale=_strdup(setlocale(LC_CTYPE,NULL));
	setlocale(LC_CTYPE,"chs");
}
/*****************************************************************************************************
��  ��  ��:recoverLoacle()
��      ��:ǰ�溯���ķ�����
��	��  ֵ:
******************************************************************************************************/
void CUtil::recoverLoacle()
{
	setlocale(LC_CTYPE,old_locale);
	free(old_locale);
}
/*****************************************************************************************************
��  ��  ��:getExePath()
��      ��://��ȡ�ļ�����·��
��	��  ֵ:
******************************************************************************************************/
CString CUtil::getExePath()
{
	CString sPath;
	GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	sPath.ReleaseBuffer();
	int nPos;
	nPos=sPath.ReverseFind('\\');
	sPath=sPath.Left(nPos);
	return sPath;
}
/*****************************************************************************************************
��  ��  ��:isFileExist
��      ��:;//�鿴�ļ��Ƿ����
��	��  ֵ:
******************************************************************************************************/
BOOL CUtil::isFileExist(CString filename)
{
	CString strFile;
	CFileFind finder;
	//return finder.FindFile(filename);//��Ϊ����� cwcq2012.11.05
	BOOL bFind = finder.FindFile(filename);//wcq2012.11.05
	finder.Close();//wcq2012.11.05
	return bFind;//wcq2012.11.05
}
/*****************************************************************************************************
��  ��  ��:openFolder
��      ��:���ļ���
��	��  ֵ:
******************************************************************************************************/
void CUtil::openFolder(CString filepath)
{
	ShellExecute(NULL,NULL,filepath,NULL,NULL,SW_SHOW);
}
/*****************************************************************************************************
��  ��  ��:setListToReport
��      ��:�޸�list��ʽΪ������ʽ
��  ��  ��:list:CListCtrl
��	��  ֵ:
˵      ������Ҫ����������viewΪreport
******************************************************************************************************/
void CUtil::setListToReport(CListCtrl &list)
{
	DWORD dwStyle=::GetWindowLong(list.m_hWnd,GWL_STYLE);
	SetWindowLong(list.m_hWnd,GWL_STYLE,dwStyle|LVS_SHOWSELALWAYS|LVS_REPORT|LVS_NOLABELWRAP);
	ListView_SetExtendedListViewStyle(list.m_hWnd,LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

}
/*****************************************************************************************************
��  ��  ��:setListToReport
��      ��:�޸�list��ʽΪ������ʽ,����checkbox��ʽ
��  ��  ��:list:CListCtrl
��	��  ֵ:
˵      ������Ҫ����������viewΪreport
******************************************************************************************************/
void CUtil::setListToReportCheck(CListCtrl &list)
{
	DWORD dwStyle=::GetWindowLong(list.m_hWnd,GWL_STYLE);
	SetWindowLong(list.m_hWnd,GWL_STYLE,dwStyle|LVS_SHOWSELALWAYS|LVS_REPORT|LVS_NOLABELWRAP);
	ListView_SetExtendedListViewStyle(list.m_hWnd,LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);

}
/*****************************************************************
** ������:GetPath
** �� ��: ��
** �� ��: CString strPath
**        strPath�ǿ�, ��ʾ�û�ѡ����ļ���·��
**        strPathΪ��, ��ʾ�û�����ˡ�ȡ��������ȡ��ѡ��
** ��������:��ʾ��ѡ���ļ��С��Ի������û�ѡ���ļ��� ������磺"E:\xxx�����ļ���\plc"
****************************************************************/
CString CUtil::getPath(HWND hwndOwner)
{
	CString strPath =_T("");
	BROWSEINFO bInfo;
	ZeroMemory(&bInfo, sizeof(bInfo));
	bInfo.hwndOwner = hwndOwner;
	bInfo.lpszTitle = _T("��ѡ��·��: ");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS;    

	LPITEMIDLIST lpDlist; //�������淵����Ϣ��IDList
	lpDlist = SHBrowseForFolder(&bInfo) ; //��ʾѡ��Ի���
	if(lpDlist != NULL)  //�û�����ȷ����ť
	{
		TCHAR chPath[255]; //�����洢·�����ַ���
		SHGetPathFromIDList(lpDlist, chPath);//����Ŀ��ʶ�б�ת�����ַ���
		strPath = chPath; //��TCHAR���͵��ַ���ת��ΪCString���͵��ַ���
	}
	return strPath;
}
/*****************************************************************************************************
��  ��  ��:isFloat
��      ��:;//�ж�һ���������Ƿ�Ϊ�����ĸ�����
��  ��  ��:f:������
��	��  ֵ:����"-1.#J"
˵      ����
******************************************************************************************************/
BOOL CUtil::isFloat(FLOAT f)
{
	CString temp;
	temp.Format(_T("%.2f"),f);
	if(temp.Find(_T("-1.#"))!=-1)//
	{
		return FALSE;
	}
	return TRUE;
}

//����������Դ����Ϊ�ļ�
//saveAsFileName:�����ļ�Ϊ�����ƣ�resID����ԴID,folder:�ļ���;bSubPathOfExe:�Ƿ����Ӧ�ó����ļ����£�true:��
BOOL CUtil::saveBinResourceAsFile(CString saveAsFileName,UINT resID,CString folder,BOOL bSubPathOfExe)
{
	//
	HRSRC hsrc=::FindResource(NULL,MAKEINTRESOURCE(resID),TEXT("BIN"));//��λ��Դ
	if(NULL==hsrc)
	{
		return FALSE;
	}//end if

	DWORD dwSize=::SizeofResource(NULL,hsrc);//��ȡ��Դ��С
	if(0==dwSize)
	{
		return FALSE;
	}//end if

	HGLOBAL hGlobal=::LoadResource(NULL,hsrc);//������Դ
	if(NULL==hGlobal)
	{
		return FALSE;
	}//end if

	LPBYTE p;
	p=(LPBYTE)GlobalAlloc(GPTR,dwSize);//�����ڴ�
	if(NULL==p)
	{
		//AfxMessageBox(_T("�ڴ����ʧ��"));
		return FALSE;
	}//end if

	::CopyMemory((LPVOID)p,(LPCVOID)::LockResource(hGlobal),dwSize);//�������ݵ�p
	GlobalUnlock(hGlobal);
	CString path;
	//�����ļ�·��
	if(bSubPathOfExe)//����Ӧ�ó����ļ�����
	{
		if(""==folder)//ֱ�ӷ���Ӧ�ó����ļ�����
		{
			path.Format(_T("%s\\%s"),getExePath(),saveAsFileName);
		}
		else
		{
			path.Format(_T("%s\\%s\\%s"),getExePath(),folder,saveAsFileName);
		}//end if
	}
	else//�����Լ�ѡ����ļ�����
	{
		if(""==folder)
		{
			path.Format(_T("%s"),saveAsFileName);//��ʱ�����ļ��к��ļ���
		}
		else
		{
			path.Format(_T("%s\\%s"),folder,saveAsFileName);
		}//end if
	}//end if
	CFile file(path,CFile::modeCreate|CFile::modeWrite);
	file.Write(p,dwSize);//д��
	file.Close();//�ر�
	//�ͷ���Դ
	UnlockResource(hGlobal);
	FreeResource(hGlobal);
	::GlobalFree((HGLOBAL)p);
	return TRUE;
}
//��ע������ʼ��������ѡ��(check.radio)
void CUtil::initMultiSelect(CDialog* dlg,INT startID,INT totalItem,CString regKey)
{
	CString temp=m_reg.readKey(regKey);
	CStringArray strarr;
	if(""!=temp)
	{
		splitString(temp,',',strarr);
		if(strarr.GetSize()>=totalItem)
		{
			for(int i=0;i<totalItem;i++)
			{
				if("1"==strarr.GetAt(i))
				{
					((CButton*)dlg->GetDlgItem(startID+i))->SetCheck(TRUE);
				}
				else
				{
					((CButton*)dlg->GetDlgItem(startID+i))->SetCheck(0);
				}
			}//end for
		}//end if
	}//end if
}//einitMultiSelect

//����������ѡ��ע�����(check.radio)
void CUtil::saveMultiSelect(CDialog* dlg,INT startID,INT totalItem,CString regKey)
{
	CString temp=_T("");
	for(int i=0;i<totalItem;i++)
	{
		if(((CButton*)dlg->GetDlgItem(startID+i))->GetCheck())
		{
			temp+=_T("1,");
		}
		else
		{
			temp+=_T("0,");
		}
	}//end for
	m_reg.writeKey(regKey,temp);
}

//��ע������ʼ��������ѡ��(check.radio)
void CUtil::initMultiEdit(CDialog* dlg,INT startID,INT totalItem,CString regKey,CString defaultValues)
{
	CString temp=m_reg.readKey(regKey);
	CStringArray strarr;
	if(""!=temp)
	{
		splitString(temp,',',strarr);
		if(strarr.GetSize()>=totalItem)
		{
			for(int i=0;i<totalItem;i++)
			{
				dlg->GetDlgItem(startID+i)->SetWindowTextW(strarr.GetAt(i));
			}//end for
		}//end if
	}
	else if(""!=defaultValues)//δ�������������Ĭ��ֵ
	{
		splitString(defaultValues,',',strarr);
		if(strarr.GetSize()>=totalItem)
		{
			for(int i=0;i<totalItem;i++)
			{
				dlg->GetDlgItem(startID+i)->SetWindowTextW(strarr.GetAt(i));
			}//end for
		}//end if
	}
	//end if
}//einitMultiSelect

//�õ����������checkbox,radioѡ�����ĸ�������ƫ�Ƶ�һ��λ��
INT CUtil::getMultiSelect(CDialog* dlg,UINT startID,INT totalItem)
{
	INT sele=0;
	for(int i=0;i<totalItem;i++)
	{
		if(((CButton*)dlg->GetDlgItem(startID+i))->GetCheck())
		{
			sele=i;
			break;
		}
	}//end for
	return sele;
}
//����������ѡ��ע�����(check.radio)
void CUtil::saveMultiEdit(CDialog* dlg,INT startID,INT totalItem,CString regKey)
{
	CString temp=_T(""),values=_T("");
	for(int i=0;i<totalItem;i++)
	{
		dlg->GetDlgItem(startID+i)->GetWindowTextW(temp);
		values+=temp+_T(",");;
	}//end for
	m_reg.writeKey(regKey,values);
}
//������ѡ���¼��еõ�ѡ����ı�
CString CUtil::getComboSelText(CComboBox* comboBox)
{
	CString temp;
	if(-1==comboBox->GetCurSel())
	{
		comboBox->GetWindowText(temp);
	}
	else
	{
		comboBox->GetLBText(comboBox->GetCurSel(),temp);
	}//end if
	return temp;
}
//��������
void CUtil::ReStart(BOOL bNormal) 
{ 
	PROCESS_INFORMATION   info; 
	STARTUPINFO startup; 
	TCHAR szPath[128]; 
	TCHAR *szCmdLine; 

	GetModuleFileName(AfxGetApp()-> m_hInstance,szPath,sizeof(szPath)); 
	szCmdLine= GetCommandLine(); 
	GetStartupInfo(&startup); 

	BOOL bSucc=CreateProcess(szPath,szCmdLine,NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS,NULL, NULL,&startup,&info); 

	if(bNormal && bSucc) 
	{ 
		CWnd *pWnd= AfxGetMainWnd(); 

		if(pWnd != NULL) 
		{ 
			pWnd->PostMessage(WM_CLOSE,0,0); 
		} 
		else 
			ExitProcess(-1); 
	} 
	else 
		ExitProcess(-1); 
} 
void CUtil::setEditFloat(CDialog* dlg,UINT ID,FLOAT f,INT precision)
{
	CString temp;
	if(2==precision)
	{
		temp.Format(_T("%.2f"),f);
	}
	else
	{
		temp.Format(_T("%.3f"),f);
	}
	dlg->GetDlgItem(ID)->SetWindowTextW(temp);
}
//char*ת��ΪCString
//#include <afxpriv.h>
CString CUtil::pCharToCString(char* pchar)
{
	///USES_CONVERSION;
	///return A2T(pchar);	
	//����char *�����С�����ֽ�Ϊ��λ��һ������ռ�����ֽ�
	int charLen = strlen(pchar);
	//������ֽ��ַ��Ĵ�С�����ַ����㡣
	int len = MultiByteToWideChar(CP_ACP,0,pchar,charLen,NULL,0);
	//Ϊ���ֽ��ַ���������ռ䣬�����СΪ���ֽڼ���Ķ��ֽ��ַ���С
	TCHAR *buf = new TCHAR[len + 1];
	//���ֽڱ���ת���ɿ��ֽڱ���
	///MultiByteToWideChar(CP_ACP,0,pchar,charLen,buf,len);
	MultiByteToWideChar(CP_ACP,0,pchar,charLen,buf,len);
	char m_pchar_zll;
	for(int i=0;i<len;i++)
	{
		m_pchar_zll=pchar[i];
	}
	buf[len] = '\0'; //����ַ�����β��ע�ⲻ��len+1
	//��TCHAR����ת��ΪCString
	CString pWideChar;
	pWideChar.Append(buf);
	//ɾ��������
	delete []buf;
	return pWideChar;
}
//char*ת��ΪCString
//#include <afxpriv.h>
CString CUtil::pTCharToCString(TCHAR* pchar)
{
	///USES_CONVERSION;
	///return A2T(pchar);	
	//����char *�����С�����ֽ�Ϊ��λ��һ������ռ�����ֽ�
	int charLen = wcslen(pchar);
	//������ֽ��ַ��Ĵ�С�����ַ����㡣
	int len = WideCharToMultiByte(CP_ACP,0,pchar,charLen,NULL,0,NULL,NULL);;//MultiByteToWideChar(CP_ACP,0,pchar,charLen,NULL,0);
	//Ϊ���ֽ��ַ���������ռ䣬�����СΪ���ֽڼ���Ķ��ֽ��ַ���С
	char *buf = new char[len + 1];
	//���ֽڱ���ת���ɿ��ֽڱ���
	///MultiByteToWideChar(CP_ACP,0,pchar,charLen,buf,len);
	//MultiByteToWideChar(CP_ACP,0,pchar,charLen,buf,len);
	CString ss;
	WideCharToMultiByte(CP_ACP,0,pchar,charLen,buf,len,NULL,NULL);;//MultiByteToWideChar(CP_ACP,0,pchar,charLen,NULL,0);
	buf[len] = '\0'; //����ַ�����β��ע�ⲻ��len+1
	//��TCHAR����ת��ΪCString
	CString pWideChar(buf);
	//pWideChar.Append(buf);
	//ɾ��������
	//delete []buf;
	//return ss;
	return pWideChar;
}
//////////////////////////////////////////////////////////////////
//CStringת��Ϊchar*
//����ʵ���ֽڳ��ȣ�ʹ��ʱpchar�иó��Ⱥ���ַ���Ч
//////////////////////////////////////////////////////////////////
INT CUtil::CStringToPChar(CString str, char* pchar)
{
	///USES_CONVERSION;
	///pchar=T2A(str);
	///CString str = _T("D:\\У����Ŀ\\QQ.bmp");
	//ע�⣺����n��len��ֵ��С��ͬ,n�ǰ��ַ�����ģ�len�ǰ��ֽڼ����
	int n = str.GetLength();     // n = 14, len = 18
	//��ȡ���ֽ��ַ��Ĵ�С����С�ǰ��ֽڼ����
	int len = WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),NULL,0,NULL,NULL);
	//Ϊ���ֽ��ַ���������ռ䣬�����СΪ���ֽڼ���Ŀ��ֽ��ֽڴ�С
	///char * pFileName = new char[len+1];   //���ֽ�Ϊ��λ
	//���ֽڱ���ת���ɶ��ֽڱ���
	WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),pchar,len,NULL,NULL);
	pchar[len+1] = '\0';   //���ֽ��ַ���'\0'����
	return len;
}