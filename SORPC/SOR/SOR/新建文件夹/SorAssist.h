#pragma once
//#include "CurveData.h"
#include "SORDATA.h"
#include "math.h"
//sor������
//��Ҫ�ǽ���sor.dll���ò���ת��
//wcq2012.07.23
class CSorAssist
{
public:
	CSorAssist(void);
	~CSorAssist(void);
public:
	void convert(SORDATA sorData, TestResultInfo& testResult);//SORDATAת��ΪTestResultInfo
	void convert(TestResultInfo testResult, SORDATA& sorData);//TestResultInfoת��ΪSORDATA
};
