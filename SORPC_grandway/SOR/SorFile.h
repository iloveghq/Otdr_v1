#pragma once
#include <vector>
#include "SORDATA.h"
#ifdef _CLASSINDLL
#define CLASSINDLL_CLASS_DECL __declspec(dllexport)
#else
#define CLASSINDLL_CLASS_DECL __declspec(dllimport)
#endif

#define FILE_SUCCESS 1//�ļ���ȷ����
#define ERROR_EMPTY_NAME 2//�ļ���Ϊ��
#define ERROR_FILE_NOTFOUND 3//δ�ܴ��ļ�
#define ERROR_CREATE_FILE 4//δ�ܴ����ļ�
#define ERROR_FORMAT 5//�ļ���ʽ��ʧ��
///////////////////////////////////////////////////////////////
//readSorFile/saveSorFile����ֵ˵����
//ERROR_EMPTY_NAME: �ļ���Ϊ��
//ERROR_FILE_NOT_FOUND: δ�ܴ��ļ�
//ERROR_CREATE_FILE:δ�ܴ����ļ�
/////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
//ʹ�÷���//wcq2012.07.24
//����:
//m_sorAssist.convert(m_testResultInfo,m_sorDATA);
//m_sorfileBase.saveSorFile(_T("e:\\a.sor"),m_sorDATA);
//
//��ȡ��
//if(m_sorfileBase.readSorFile(temp,m_sorDATA))
//{
//	m_sorAssist.convert(m_sorDATA, m_testResultInfo);
//}
//���У�m_sorAssistΪCSorAssist����,m_sorDATAΪSORDATA����,m_testResultInfoΪTestResultInfo����
///////////////////////////////////////////////////////////////////
class CLASSINDLL_CLASS_DECL CSorFile
{
public:
	CSorFile(void);
	~CSorFile(void);
public:
	/*INT readSorFile(CString filePathAndName,TestResultInfo& testResultInfo);//cwcq2012.07.23
	INT saveSorFile(CString filePathAndName,TestResultInfo testResultInfo);*///cwcq2012.07.23
	INT readSorFile(CString filePathAndName,SORDATA& sorData);//wcq2012.07.23
	INT saveSorFile(CString filePathAndName,SORDATA sorData);//wcq2012.07.23
	INT readSorFile(BYTE *pData, int nLength, SORDATA &sorData); //lzy2014.11.11ֱ�Ӷ��ڴ�sor�ļ�

public:
	LPVOID m_lpVoid;
};
