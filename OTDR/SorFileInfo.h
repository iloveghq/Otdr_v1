#pragma once

#include "VARIABLE.h"
#include "WaveConfig.h"
#include "ReportConfig.h"
#include "OTDRConfig.h"
#include "PrintConfig.h"
#include "EventAnalConfig.h"
#include "OtdrTrace.h"

#define  M_FSTARTDB0 -65.63//-65.53//wcq2011.12.05
#define  M_FMAXDB0   -5.63//-9.23//wcq2011.12.05   zll 2012.10.26 ��-9.23��Ϊ-15.63 lzy 2014.6.17��-15.63��Ϊ-5.63
#define  M_FSTARTDB1 -58.3//����������Сֵ�� > ZOOM_VERTICAL_STANDARD 30*1000ʱʹ��//wcq2011.09.26
#define  M_FMAXDB1   -8.3//wcq2011.09.26  zll 2012.10.26 ��-1��Ϊ-8.3
#define  ZOOM_VERTICAL_LEAST_RANGE 0.8//��ֱ����Ŵ������ܹ���ʾ����dB//wcq2011.09.28
#define  ZOOM_VERTICAL_PER_TIME 0.5//��ֱ����Ŵ�����//wcq2011.09.28
#define  ZOOM_VERTICAL_COUNT 47//��ֱ�������Ŵ����//wcq2011.09.28 //  zll 2012.12.31�޸�
#define  ZOOM_VERTICAL_MAX 98//��ֱ����Ŵ����ʱ�ж���dB//wcq2011.11.11��
#define  EVENT_POINT_HEIGHT 100//�����߻����¼���ʱ���һ����ֱ�ߣ���ֵΪ���߸߶�//wcq2011.11.15
#define  EVENT_POINT_NUM_HEIGHT 20//�����߻����¼���ʱ���һ����ֱ�ߣ���ֵΪ��ֱ���·���1,2..��ֱ����λ��//wcq2011.11.15
#define  COUNT_POINT_MAX_MIN 686*3//��ͼ�á���������x���ϸ����������Сֵ�����,�����޸�//wcq2011.11.23
#define  COUNT_GRIDLINE_HORIZONE 10//��ͼ�á�ˮƽ������//wcq2011.12.19
#define  COUNT_GRIDLINE_VERTICAL 8//��ͼ�á���ֱ������//wcq2011.12.19

COLORREF GetCurveColor(int nCurveIndex); //��ȡ��ǰ������ɫ

//����SOR����
struct SorFileInfo 
{
	SorFileInfo()
	{
		bValid = false;
		pTestResultInfo = new TestResultInfo();
	}

	~SorFileInfo()
	{
		if (pTestResultInfo)
		{
			delete pTestResultInfo;
			pTestResultInfo = NULL;
		}
	}

	void Reset()
	{
		if (pTestResultInfo)
		{
			delete pTestResultInfo;
			pTestResultInfo = new TestResultInfo();
		}
		bValid = FALSE;
		fileName.Empty();
		filePathName.Empty();
	}
	
	BOOL bValid;//δ����/���س���/ɾ����Ϊfalse
	float fOffset;
	CString fileName;
	CString filePathName;
	TestResultInfo* pTestResultInfo;
};
//����SOR����
struct SorFileArray
{
	//�ƶ������е�����Ԫ��
	void Move(int nForm, int nTo)
	{
		//�ж�Խ��
		if (nForm >= MAX_CURVE_NUM ||
			nTo < 0)
		//if (nTo < 0)//zll 2012.9.21 
		{
			return;
		}
		//��Ա��ֵ
		sorFileList[nTo].bValid = sorFileList[nForm].bValid;
		sorFileList[nTo].fOffset = sorFileList[nForm].fOffset;
		sorFileList[nTo].fileName = sorFileList[nForm].fileName;
		sorFileList[nTo].filePathName = sorFileList[nForm].filePathName;
		sorFileList[nTo].pTestResultInfo = sorFileList[nForm].pTestResultInfo;
		if (NULL != sorFileList[nTo].pTestResultInfo)
			sorFileList[nTo].pTestResultInfo->curveColor = GetCurveColor(nForm);

		//�ÿ�ԭ����
		sorFileList[nForm].pTestResultInfo = new TestResultInfo();
		sorFileList[nForm].bValid = FALSE;
		sorFileList[nForm].fileName.Empty();
		sorFileList[nForm].filePathName.Empty();
	}
	
	//�������
	void Reset()
	{
		//���SorFileInfo
		for (int nIndex = 0; nIndex < totalCurveNumber; nIndex++)
		{
			sorFileList[nIndex].Reset();
		}
		//��������
		curSelection = 0;
		totalCurveNumber = 0;
		maxDistCurveIndex = 0;
	}

	//ɾ����ǰѡ������
	void RemoveCurSel()
	{
		//���SorFileInfo
		sorFileList[curSelection].Reset();

		//��������˳��
		for (int nIndex = 0;nIndex < totalCurveNumber; nIndex++)
		{
			if (sorFileList[nIndex].bValid /*��Ч����*/)
				continue;
			else //�����Ѿ���ɾ��,ǰ������
			{
				if (sorFileList[nIndex+1].bValid)
					Move(nIndex+1, nIndex);
			}
		}

		if (curSelection > 0)
			curSelection = curSelection - 1;
		totalCurveNumber = totalCurveNumber - 1;

		maxDistCurveIndex = 0;
	}
	
	SorFileInfo sorFileList[MAX_CURVE_NUM]; //������8������ zll 2012.9.24
	int curSelection;//�ļ��б�ǰѡ���ĸ�
	int totalCurveNumber; //������������
	int maxDistCurveIndex; //�����������
	WaveConfig waveConfig;
	ReportConfig reportConfig;
	OTDRConfig otdrConfig;
	PrintConfig printConfig;
	EventAnalConfig eventAnalConfig;
};

//��������ʱ���ҵ�һ��δ���ص������±�
int GetNextCurveNum(SorFileArray& sorFileArray);
BOOL OpenCurveFile(const CString& strFile);//�������ļ�
CString GetItemValById(int nIndex); //��ȡItem��ֵ
BOOL IsFileAlreadyOpen(CString& strFileName);
//��λת��
BOOL IsUnitKm(); //��鵱ǰ�ǲ�����kmΪ��λ
float MeterToOther(float values,CString strUnit);//��ת��Ϊfeet �� mile
float KmeterToOther(float values,CString strUnit);//ǧ��ת��ΪKfeet �� mile
CString UnitConvert(CString configUnit);
