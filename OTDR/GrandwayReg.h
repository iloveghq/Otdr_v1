#pragma once
/***********************************************************
��:GrandwayReg
wcq 090730
���ܣ�ע�����ز���
*************************************************************/
class CGrandwayReg
{
public:
	CGrandwayReg(void);
public:
	~CGrandwayReg(void);
public:	
	void FindComm(CStringArray& arrReturn);//Ѱ��ע����еĴ���
	CString readKey(CString key);//��ע���
	void writeKey(CString key,CString value);//дע���
};
