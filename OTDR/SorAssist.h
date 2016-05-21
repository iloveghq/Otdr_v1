#pragma once
//#include "CurveData.h"
#include "EndGabor.h"
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
	void convert(TestResultInfo* testResult, SORDATA& sorData);//TestResultInfoת��ΪSORDATA,zll 2012.8.20�޸�TestResultInfoΪTestResultInfo*
	//void AddSordataToOtdrTrace(SORDATA sorData, COtdrTrace* pOtdrTrace, COLORREF colorLine=RGB(0, 0, 255));//��sor�ļ�����������ת��Ϊotdrtrace���� zll2013.04.23
	void AddSordataToOtdrTrace(SORDATA sorData, COtdrTrace* pOtdrTrace, WORD* pDataSmooth=NULL, COLORREF colorLine=RGB(0, 0, 255));//��sor�ļ�����������ת��Ϊotdrtrace���� zll2013.04.23
	void SetEvents(TestResultInfo& testResultInfo, CGraphElement* graphElement);//���¼��б�ת��Ϊ������ʾ�õ��¼��б�zll2013.04.23
	void CloseCurvedata(int m_listItem,SORDATA sorData, COtdrTrace* pOtdrTrace, COLORREF colorLine=RGB(0, 0, 255));//��sor�ļ�����������ת��Ϊotdrtrace���� zll2013.04.23
	void ReEventAutoSearch(TestResultInfo& testResultInfo, SORDATA sorData, COtdrTrace* pOtdrTrace, WORD* pDataSmooth, COLORREF colorLine,int m_listItem);//zll 2013.6.18 ִ���Զ�����ʱ����sor�ļ�����������ת��Ϊotdrtrace����
	int GetTraceYmax(SORDATA &sorData);  //��ȡһ�����ߵ����ֵ��lzy2014.06.24
public:
	CEndGabor m_endgabor;//zll 2013.8.19
};
