#include "StdAfx.h"
#include "GrandwayReg.h"
/***********************************************************
��:GrandwayReg
wcq 090730
���ܣ�ע�����ز���
˵����ʹ��ʱҪ��LPCTSTR data_set=_T("SOFTWARE\\COpticalPowerAutoTest");//��ֵ���Ƶ�COpticalPowerAutoTest��Ϊ�Լ���Ҫ������
*************************************************************/
CGrandwayReg::CGrandwayReg(void)
{
}

CGrandwayReg::~CGrandwayReg(void)
{
}
/**********************************************************
��  ��  ��:FindComm
��      ��:Ѱ��ע����еĴ���
��  ��  ��:CStrinArray,���õ��ã���������Ĵ�������
��	��  ֵ:BOOL,�д���ʱ�����棬��֮��
***********************************************************/
void CGrandwayReg::FindComm(CStringArray& arrReturn)
{
	HKEY hKey;//ָ���Ӽ��ľ��
	INT rtn;//ע�������Ƿ�ɹ���־
	rtn=RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("Hardware\\DeviceMap\\SerialComm"),NULL,KEY_READ,&hKey);//��ע����д��������Ӽ�
	if(rtn!=ERROR_SUCCESS)//��ʧ��
	{
		return;
	}

	INT i=0;//������ע�����λ��
	TCHAR portName[256],commName[256];//portName���������ƣ�
	while(1)
	{
		DWORD dwLong,dwSize;
		dwSize=sizeof(portName);
		dwLong=dwSize;
		rtn=RegEnumValue(hKey,i,portName,&dwLong,NULL,NULL,(PUCHAR)commName,&dwSize);//ö�ٴ���
		if(rtn==ERROR_SUCCESS)//�д���
		{
			arrReturn.Add((CString)commName);
		}
		if(rtn==ERROR_NO_MORE_ITEMS)//�޻����һ��
		{
			break;
		}
		i++;
	}
	RegCloseKey(hKey);//�رվ��
}
/**********************************************************
��  ��  ��:readKey
��      ��:��ע���
��  ��  ��:key:��ֵ
��	��  ֵ:��Ӧ���ַ���
***********************************************************/
CString CGrandwayReg::readKey(CString key)//��ע���
{
	HKEY hkey;
	LPCTSTR data_set=_T("SOFTWARE\\PLCSC");//��ֵ
	long ret=::RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_set,0,KEY_READ,&hkey);//�򿪼�
	if(ret!=ERROR_SUCCESS)//δ�ܴ򿪼�
	{
		//��Ӽ�ֵ
		::RegCreateKey(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\PLCSC\\"),&hkey);
		//AfxMessageBox(_T("�޷���ע���"));
		//::RegCloseKey(hkey);
		//return _T("");
		ret=::RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_set,0,KEY_READ,&hkey);//�ٴγ��Դ�
		if(ret!=ERROR_SUCCESS)
		{
			AfxMessageBox(_T("�޷���ע���"));
			::RegCloseKey(hkey);
			return _T("");
		}
	}
	BYTE data[1000];
	DWORD type=REG_SZ;
	DWORD cbdata=1000;
	ret=::RegQueryValueEx(hkey,key,NULL,&type,data,&cbdata);//��ֵ
	if(ret!=ERROR_SUCCESS)
	{
		//AfxMessageBox(_T("�޷��鿴�й�ע�������"));
		::RegCloseKey(hkey);
		writeKey(key,_T(""));////////?????????????????????????????????????????????????????
		return _T("");
	}
	::RegCloseKey(hkey);//�رռ�ֵ
	if(cbdata>0)
	{
		return CString((LPCTSTR)data);
	}
	else
	{
		return _T("");
	}
}
/**********************************************************
��  ��  ��:writeKey
��      ��:дע���
��  ��  ��:key:��ֵ,value:��Ӧ��Ҫд����ַ���
��	��  ֵ:
ע�⣺�ú����п�����readKey��ȡ���ݳ���ʱ��readKey����
***********************************************************/
void CGrandwayReg::writeKey(CString key,const CString value)//дע���
{
	HKEY hkey;
	LPCTSTR data_set=_T("SOFTWARE\\PLCSC");
	long ret=::RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_set,0,KEY_WRITE,&hkey);
	if(ret!=ERROR_SUCCESS)
	{
		if(ERROR_SUCCESS!=::RegCreateKey(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\PLCSC\\"),&hkey))//����key
		{
			AfxMessageBox(_T("�޷���ע���"));
			return;
		}//end if
	}//end if
//	char lb[1024];
	//TCHAR *va=value;
	//_tcscpy(lb,value);
	//WideCharToMultiByte(CP_OEMCP,0,(LPCTSTR)value,-1,lb,value.GetLength(),0,false);
	DWORD type=REG_SZ;
	DWORD cbdata=(lstrlen(value)+1)*sizeof(TCHAR);
	ret=::RegSetValueEx(hkey,key,NULL,type,(const BYTE*)(LPCTSTR)value,cbdata);
	if(ret!=ERROR_SUCCESS)
	{
		AfxMessageBox(_T("�޷��޸���Ӧע����ֵ"));
	}
	::RegCloseKey(hkey);
}