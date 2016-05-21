#include "StdAfx.h"
#include "SorAssist.h"
#define C_LIGHT_SPEED 299792458.0//����//wcq2011.10.19
template<typename To,typename From>
void vchar2string(To& t,From& f)
{
	t.assign(f.begin(),f.end());
}
CSorAssist::CSorAssist(void)
{
}

CSorAssist::~CSorAssist(void)
{
}
//SORDATAת��ΪTestResultInfo
void CSorAssist::convert(SORDATA sorData, TestResultInfo& testResultInfo)
{
	if (sorData.vAllData.size() > 0)//������Ч����
	{
		if (NULL == testResultInfo.pDataPoints)
		{
			testResultInfo.pDataPoints = new WORD[sorData.vAllData[0].nCount];
			memcpy(testResultInfo.pDataPoints,&(sorData.vAllData[0].vData[0]),sorData.vAllData[0].nCount * 2);
		}
		else//��������
		{
			if (testResultInfo.DataPointsNum < sorData.vAllData[0].nCount)//�������ݳ��ȱȽ�Ҫ��ֵ����
			{
				delete[] testResultInfo.pDataPoints;
				testResultInfo.pDataPoints = new WORD[sorData.vAllData[0].nCount];
			}//eif
			memcpy(testResultInfo.pDataPoints,&(sorData.vAllData[0].vData[0]),sorData.vAllData[0].nCount * 2);
		}//eif
		testResultInfo.DataPointsNum = sorData.vAllData[0].nCount;

		////if (1000 != sorData.vAllData[0].scaleFactor)//ϵ������1000,ϵ����Ϊ1��������Ҫ��һ��ϵ��?????????????????
		////{
		////	float fTemp = sorData.vAllData[0].scaleFactor / 1000.0;
		////	for (int i=0; i < testResultInfo.DataPointsNum; i++)
		////	{
		////		testResultInfo.pDataPoints[i] *= fTemp;
		////	}//efor
		////}//eif
	}//eif

	//
	testResultInfo.Refraction = sorData.Refraction;
	testResultInfo.PulseWidth = sorData.PulseWidth;
	testResultInfo.WaveLength = sorData.WaveLength;
	testResultInfo.Frequency = sorData.Frequency;
	testResultInfo.DataSpacing = sorData.DataSpacing;

	////sorData.Length = testResultInfo.WaveLength;//???????????????????

	//�¼��б�
	int nTemp = sorData.vEvent.size();
	float fTempLocation = pow(10.0,-10) * C_LIGHT_SPEED / (testResultInfo.Refraction );
	float fTempIndex = (2 *testResultInfo.Frequency)/10000;
	testResultInfo.EventList.vEventInfo.clear();//�¼��б������� wcq2012.08.13
	for(int i=0; i<nTemp; i++)
	{	
		EventInfo eventInfo;


		if (sorData.vEvent[i].code.size()>5)//ʵ�ʳ���Ӧ��Ϊ6
		{
			eventInfo.nEventType = sorData.vEvent[i].code[0] - '0';//�¼�����
			eventInfo.cEventType = sorData.vEvent[i].code[1];//�¼������ɵĻ����û���ӻ���β���¼���
		}//eif
	
		//eventInfo.fLocation = sorData.vEvent[i].propagationTime * pow(10.0,-10) * C_LIGHT_SPEED / (testResultInfo.Refraction ) ;//λ��//cwcq2012.07.24
		eventInfo.fLocation = sorData.vEvent[i].propagationTime * fTempLocation ;//λ��//�����湫ʽ�򻯶��� wcq2012.07.24
		//eventInfo.index = sorData.vEvent[i].propagationTime * (2 *testResultInfo.Frequency)/10000;//����/����(km/ps) * ������ λ��//cwcq2012.07.24
		eventInfo.index = sorData.vEvent[i].propagationTime * fTempIndex;//����/����(km/ps) * ������ λ��//�����湫ʽ�򻯶��� wcq2012.07.24
		//��ֹ������Χ
		if (eventInfo.index <0)
		{
			eventInfo.index =0;
		}
		else if (eventInfo.index > testResultInfo.DataPointsNum)
		{
			eventInfo.index = testResultInfo.DataPointsNum;//
		}//eif


		//eventInfo.index = (sorData.vEvent[i].propagationTime * (2 *testResultInfo.Frequency)/10000);//����/����(km/ps) * ������ λ��
		eventInfo.fLineK = sorData.vEvent[i].attenCoefficent /1000.0;//б��/˥������ dB/km 350=0.35
		eventInfo.fEventloss =  sorData.vEvent[i].eventLoss / 1000.0;////���
		eventInfo.fEventHigh = sorData.vEvent[i].reflectance / 1000.0;//����

		if (0 == i)
		{
			eventInfo.fTotalLoss = eventInfo.fEventloss;
		}
		else
		{
			//ei.fTotalLoss = testResultInfo.EventList.vEventInfo[i-1].fTotalLoss/*ǰһ�¼������*/ + ei.fEventloss/*��ǰ�¼����*/ + (ei.fLineK/1000) *(ei.fLocation - testResultInfo.EventList.vEventInfo[i-1].fLocation);
			eventInfo.fTotalLoss = testResultInfo.EventList.vEventInfo[i-1].fTotalLoss/*ǰһ�¼������*/ + eventInfo.fEventloss/*��ǰ�¼����*/ + (eventInfo.fLineK/1000) *(eventInfo.fLocation - testResultInfo.EventList.vEventInfo[i-1].fLocation);
		}//eif
		eventInfo.fReflect = 0;//�������û���õ�
		eventInfo.fTrailEnd =0;////�������û���õ�C/(2nf))*index
		//eventInfo.nEventType = sorData.vEvent[i].code[0] - '0';
		//eventInfo.cEventType = sorData.vEvent[i].code[1];
		testResultInfo.EventList.vEventInfo.push_back(eventInfo);
	}//efor
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//������� wcq2012.10.30
	testResultInfo.BackscatterCoefficient = sorData.BackscatterCoefficient;
	testResultInfo.LossThreshold = sorData.LossThreshold;
	testResultInfo.ReflectanceThreshold = sorData.ReflectanceThreshold;
	testResultInfo.EndOfFiberThreshold = sorData.EndOfFiberThreshold;
	testResultInfo.Range = sorData.Range;//��������Ϊ������Χ����λ��m
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
//TestResultInfoת��ΪSORDATA
void CSorAssist::convert(TestResultInfo testResultInfo, SORDATA& sorData)
{
	//wince��ֻ��һ������ϵ�������е����ݶ���ԭʼ����
	sorData.vAllData.clear();
	Data data;
	data.nCount = testResultInfo.DataPointsNum;
	//data.scaleFactor = testResultInfo.ScaleFactor;//û��ϵ�� *1000???????????
	data.scaleFactor = 1000;//û��ϵ�� *1000???????????
	data.vData.assign(testResultInfo.pDataPoints, testResultInfo.pDataPoints + data.nCount);
	sorData.vAllData.push_back(data);

	//
	sorData.Refraction = testResultInfo.Refraction;
	sorData.PulseWidth = testResultInfo.PulseWidth;
	sorData.WaveLength = testResultInfo.WaveLength;
	sorData.Frequency = testResultInfo.Frequency;
	////sorData.Length = testResultInfo.WaveLength;//???????????????????
	
	//�¼��б�wcq2012.06.11
	sorData.vEvent.clear();
	std::string pos="9999";
	int nTemp = testResultInfo.EventList.vEventInfo.size();
	for(int i=0; i<nTemp; i++)
	{
		Event event;		
		event.propagationTime = (testResultInfo.EventList.vEventInfo[i].index /(2 *testResultInfo.Frequency))*10000;//����/����(km/ps) * ������ λ��
		event.attenCoefficent = testResultInfo.EventList.vEventInfo[i].fLineK * 1000;//б��/˥������ dB/km 350=0.35
		event.eventLoss = testResultInfo.EventList.vEventInfo[i].fEventloss * 1000;////���
		event.reflectance = testResultInfo.EventList.vEventInfo[i].fEventHigh * 1000;//����
		event.code.push_back(testResultInfo.EventList.vEventInfo[i].nEventType + '0');//wcq2012.07.09
		event.code.push_back(testResultInfo.EventList.vEventInfo[i].cEventType);//
		event.code.insert(event.code.end(), pos.begin(), pos.end());//��ʹ�� landmark
		vchar2string(event.lossMeasurement,std::string("LS"));//���㷨 �����õ���4�㷨
		sorData.vEvent.push_back(event);
	}//efor
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//������� wcq2012.10.30
	sorData.BackscatterCoefficient = testResultInfo.BackscatterCoefficient;
	sorData.LossThreshold = testResultInfo.LossThreshold;
	sorData.ReflectanceThreshold = testResultInfo.ReflectanceThreshold;
	sorData.EndOfFiberThreshold = testResultInfo.EndOfFiberThreshold;
	sorData.Range = testResultInfo.Length;//��������Ϊ������Χ����λ��m
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
