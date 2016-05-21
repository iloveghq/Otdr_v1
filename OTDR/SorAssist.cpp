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
#ifdef COMPILE_SHOW_SMOOTH_DATA////��ʾʱ��ƽ���������ݣ�����ʱ��ʵ������ wcq2013.04.26
			if (NULL == testResultInfo.pDataPointsSmooth)
			{
				testResultInfo.pDataPointsSmooth = new WORD[sorData.vAllData[0].nCount];
			}
#endif
		}
		else//��������
		{
			if (testResultInfo.DataPointsNum < sorData.vAllData[0].nCount)//�������ݳ��ȱȽ�Ҫ��ֵ����
			{
				delete[] testResultInfo.pDataPoints;
				testResultInfo.pDataPoints = new WORD[sorData.vAllData[0].nCount];
#ifdef COMPILE_SHOW_SMOOTH_DATA////��ʾʱ��ƽ���������ݣ�����ʱ��ʵ������ wcq2013.04.26
				delete[] testResultInfo.pDataPointsSmooth;
				testResultInfo.pDataPointsSmooth = new WORD[sorData.vAllData[0].nCount];
#endif
			}//eif
			memcpy(testResultInfo.pDataPoints,&(sorData.vAllData[0].vData[0]),sorData.vAllData[0].nCount * 2);
		}//eif
		testResultInfo.DataPointsNum = sorData.vAllData[0].nCount;

		TRACE(_T("sorAssist.cpp No.100 data=%d;last data=%d\n"), sorData.vAllData[0].vData[100],  sorData.vAllData[0].vData[testResultInfo.DataPointsNum-1]);  //lzy2014.7.16

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
	testResultInfo.Length=sorData.Length;
	///*CString temp;
	//temp.Format(_T("convert testResultInfo.DataPointsNum:%d testResultInfo.Length:%f  testResultInfo.DataSpacing:%f"),testResultInfo.DataPointsNum,testResultInfo.Length,testResultInfo.DataSpacing );
	//AfxMessageBox(temp);*/
	////sorData.Length = testResultInfo.WaveLength;//???????????????????

	//�¼��б�
	int nTemp = sorData.vEvent.size();
	float fTempLocation = pow(10.0,-10) * C_LIGHT_SPEED / (testResultInfo.Refraction );
	float fTempIndex = (2 *testResultInfo.Frequency)/10000;
	testResultInfo.EventList.vEventInfo.clear();//�¼��б������� wcq2012.08.13
	FLOAT fTemp;
	for(int i=0; i<nTemp; i++)
	{	
		EventInfo eventInfo;


		if (sorData.vEvent[i].code.size()>5)//ʵ�ʳ���Ӧ��Ϊ6
		{
			eventInfo.nEventType = sorData.vEvent[i].code[0] - '0';//�¼�����
			eventInfo.cEventType = sorData.vEvent[i].code[1];//�¼������ɵĻ����û���ӻ���β���¼���
		}//eif
	
		//eventInfo.fLocation = sorData.vEvent[i].propagationTime * pow(10.0,-10) * C_LIGHT_SPEED / (testResultInfo.Refraction ) ;//λ��//cwcq2012.07.24
		///eventInfo.fLocation = sorData.vEvent[i].propagationTime * fTempLocation ;//λ��//�����湫ʽ�򻯶��� wcq2012.07.24
		eventInfo.fLocation = sorData.vEvent[i].propagationTime *( C_LIGHT_SPEED/ (testResultInfo.Refraction * pow(10.0,10))) ;//λ�� wcq2013.11.06
		//eventInfo.index = sorData.vEvent[i].propagationTime * (2 *testResultInfo.Frequency)/10000;//����/����(km/ps) * ������ λ��//cwcq2012.07.24
		///eventInfo.index = sorData.vEvent[i].propagationTime * fTempIndex;//����/����(km/ps) * ������ λ��//�����湫ʽ�򻯶��� wcq2012.07.24// cwcq2013.11.06
		fTemp = eventInfo.fLocation / testResultInfo.DataSpacing - 1;
		if (((INT)fTemp) != ((INT)(fTemp + 0.1)))//����ʱ������Ա�1Сwcq2013.11.20
		{
			fTemp += 0.1;
			eventInfo.index = fTemp;
			eventInfo.fLocation = testResultInfo.DataSpacing * (eventInfo.index + 1);//�����±����¼������
		}
		else
		{
			eventInfo.index = fTemp;
		}
		/////eventInfo.index = eventInfo.fLocation / testResultInfo.DataSpacing - 1;//  wcq2013.11.06//��Ϊ�����cwcq2013.11.20
		///float fff=eventInfo.fLocation / testResultInfo.DataSpacing - 1;
		//��ֹ������Χ
		if (eventInfo.index <0)
		{
			eventInfo.index =0;
		}
		else if (eventInfo.index >= testResultInfo.DataPointsNum)
		{
			//eventInfo.index = testResultInfo.DataPointsNum;//cwcq2013.11.06
			eventInfo.index = testResultInfo.DataPointsNum - 1;//wcq2013.11.06
		}//eif
		///*CString temp;
		//temp.Format(_T("convert i:%d eventInfo.index:%d indnew:%f  eventInfo.fLocation:%f sorData.vEvent[i].propagationTime:%d"),i,eventInfo.index,fff,eventInfo.fLocation,sorData.vEvent[i].propagationTime );
		//AfxMessageBox(temp);*/

		//eventInfo.index = (sorData.vEvent[i].propagationTime * (2 *testResultInfo.Frequency)/10000);//����/����(km/ps) * ������ λ��
		eventInfo.fLineK = sorData.vEvent[i].attenCoefficent /1000.0;//б��/˥������ dB/km 350=0.35
		eventInfo.fEventloss =  sorData.vEvent[i].eventLoss / 1000.0;////���
		eventInfo.fEventHigh = sorData.vEvent[i].reflectance / 1000.0;//����

		////if (0 == i)//�������� cwcq2013.11.12
		////{
		////	eventInfo.fTotalLoss = eventInfo.fEventloss;
		////}
		////else
		////{
		////	//ei.fTotalLoss = testResultInfo.EventList.vEventInfo[i-1].fTotalLoss/*ǰһ�¼������*/ + ei.fEventloss/*��ǰ�¼����*/ + (ei.fLineK/1000) *(ei.fLocation - testResultInfo.EventList.vEventInfo[i-1].fLocation);
		////	eventInfo.fTotalLoss = testResultInfo.EventList.vEventInfo[i-1].fTotalLoss/*ǰһ�¼������*/ + eventInfo.fEventloss/*��ǰ�¼����*/ + (eventInfo.fLineK/1000) *(eventInfo.fLocation - testResultInfo.EventList.vEventInfo[i-1].fLocation);
		////}//eif
		eventInfo.fReflect = 0;//�������û���õ�
		eventInfo.fTrailEnd =0;////�������û���õ�C/(2nf))*index
		//eventInfo.nEventType = sorData.vEvent[i].code[0] - '0';
		//eventInfo.cEventType = sorData.vEvent[i].code[1];
		testResultInfo.EventList.vEventInfo.push_back(eventInfo);
	}//efor

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//��������� wcq2013.11.12
	//1.���¼��б�ת��ΪGabor_Result
	Gabor_Result gaborResult;
	EventInfo eventinfo;
	for(int i=0;i<nTemp;i++)
	{
		eventinfo = testResultInfo.EventList.vEventInfo[i];
		
		gaborResult.Reflect_Type[i] = eventinfo.nEventType;//�¼�����//��׼����3�֣�����ֻ��ʾ���䡢�Ƿ�������
		//gaborResult.Event_Array[i] = eventinfo.fLocation / testResultInfo.DataSpacing;//cwcq2013.11.19Get_XCoord(gaborResult..Event_Array[i],pCurveData);//�������룬��kmΪ��λ
		gaborResult.Event_Array[i] = eventinfo.index;//wcq2013.11.19Get_XCoord(gaborResult..Event_Array[i],pCurveData);//�������룬��kmΪ��λ
		gaborResult.Event_Loss[i] = eventinfo.fEventloss;
		gaborResult.Event_Hight[i] = eventinfo.fEventHigh ;
		gaborResult.Line_k[i] =  eventinfo.fLineK;//
		//gaborResult.Total_Loss[i] = eventinfo.fTotalLoss;
		gaborResult.Reflect = eventinfo.fReflect;
		gaborResult.Trail_End = eventinfo.fTrailEnd;
		gaborResult.Event_type[i] = eventinfo.cEventType;//
	}//efor
	//2.�õ������
	//Gabor_Result Acount_Total_Loss(int wavelength,int pulsewidth,WORD input_data[],Gabor_Result sGabor_Result,float Index_N/*������*/,float Sample_freaq)
	gaborResult.Valid_Num = nTemp;
	gaborResult = m_endgabor.Acount_Total_Loss(testResultInfo.WaveLength, testResultInfo.PulseWidth, testResultInfo.pDataPoints, gaborResult, testResultInfo.Refraction, testResultInfo.Frequency);
	for(int i=0;i<nTemp;i++)
	{
		testResultInfo.EventList.vEventInfo[i].fTotalLoss = gaborResult.Total_Loss[i];
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
	testResultInfo.MeasureTime = sorData.TestTimestamp;  //����������ʱ��lzy2014.8.8
	testResultInfo.Average = sorData.AverageCount;//ƽ��������lzy2014.8.12
	testResultInfo.GroupIndex = sorData.GroupIndex * 0.00001;  //������ lzy2014.8.13
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
//TestResultInfoת��ΪSORDATA
void CSorAssist::convert(TestResultInfo* testResultInfo, SORDATA& sorData)
{
	//wince��ֻ��һ������ϵ�������е����ݶ���ԭʼ����
	sorData.vAllData.clear();
	Data data;
	data.nCount = testResultInfo->DataPointsNum;
	//data.scaleFactor = testResultInfo.ScaleFactor;//û��ϵ�� *1000???????????
	data.scaleFactor = 1000;//û��ϵ�� *1000???????????
	data.vData.assign(testResultInfo->pDataPoints, testResultInfo->pDataPoints + data.nCount);
	sorData.vAllData.push_back(data);

	//
	sorData.Refraction = testResultInfo->Refraction;
	sorData.PulseWidth = testResultInfo->PulseWidth;
	sorData.WaveLength = testResultInfo->WaveLength;
	sorData.Frequency = testResultInfo->Frequency;
	////sorData.Length = testResultInfo.WaveLength;//???????????????????
	
	//�¼��б�wcq2012.06.11
	sorData.vEvent.clear();
	std::string pos="9999";
	int nTemp = testResultInfo->EventList.vEventInfo.size();
	for(int i=0; i<nTemp; i++)
	{
		Event event;		
		//event.propagationTime = (testResultInfo.EventList.vEventInfo[i].index /(2 *testResultInfo.Frequency))*10000;//����/����(km/ps) * ������ λ��. ��Ϊ�����wcq2013.07.17
		///event.propagationTime = ((testResultInfo.EventList.vEventInfo[i].index + 1) /(2 *testResultInfo.Frequency))*10000;//����/����(km/ps) * ������ λ�� �޸�ǰ����־������2m��� wcq2013.07.17//cwcq2013.11.06
		///event.propagationTime = testResultInfo.EventList.vEventInfo[i].fLocation * ((testResultInfo.Refraction *pow(10.0,10)/ C_LIGHT_SPEED));//(����*������/����)*10ʮ�η� wcq2013.11.06//�����������淽�����һ��cwcq2013.11.20
		event.propagationTime = (testResultInfo->EventList.vEventInfo[i].fLocation * (testResultInfo->Refraction *pow(10.0,10)))/ C_LIGHT_SPEED;//(����*������/����)*10ʮ�η� wcq2013.11.20
		////float ffff = testResultInfo.EventList.vEventInfo[i].fLocation * ((testResultInfo.Refraction *pow(10.0,10)/ C_LIGHT_SPEED));//wcq2013.11.20
		////float ffff2 = ffff *( C_LIGHT_SPEED/ (testResultInfo.Refraction * pow(10.0,10)));
		//event.attenCoefficent = testResultInfo->EventList.vEventInfo[i].fLineK * 1000;//б��(˥������) dB/km 350=0.35//cwcq2013.11.06
		event.attenCoefficent = (testResultInfo->EventList.vEventInfo[i].fLineK + 0.0001) * 1000;//б��(˥������) dB/km 350=0.35 //�����������봦��,���Ӻ���ʹ������ٴ򿪳���һ��ƫ�� wcq2013.11.06
		//event.eventLoss = testResultInfo->EventList.vEventInfo[i].fEventloss * 1000;////���//cwcq2013.11.06
		event.eventLoss = (testResultInfo->EventList.vEventInfo[i].fEventloss + 0.0001) * 1000;////��� //�����������봦��,���Ӻ���ʹ������ٴ򿪳���һ��ƫ�� wcq2013.11.06
		//event.reflectance = testResultInfo->EventList.vEventInfo[i].fEventHigh * 1000;//����//cwcq2013.11.06
		event.reflectance = (testResultInfo->EventList.vEventInfo[i].fEventHigh + 0.0001) * 1000;//���� //�����������봦��,���Ӻ���ʹ������ٴ򿪳���һ��ƫ�� wcq2013.11.06
		event.code.push_back(testResultInfo->EventList.vEventInfo[i].nEventType + '0');//wcq2012.07.09
		event.code.push_back(testResultInfo->EventList.vEventInfo[i].cEventType);//
		///*CString temp;
		//float ff = event.propagationTime *( C_LIGHT_SPEED/ (testResultInfo.Refraction * pow(10.0,10)));
		//temp.Format(_T("convert 1018 i:%d event.propagationTime:%d  use old:%f sorData.Length:%f  old location:%f tosave location:%f  nw:%f"),i,event.propagationTime,ffff,sorData.Length,testResultInfo.EventList.vEventInfo[i].fLocation,ff,ffff2);
		//AfxMessageBox(temp);*/
		event.code.insert(event.code.end(), pos.begin(), pos.end());//��ʹ�� landmark
		vchar2string(event.lossMeasurement,std::string("LS"));//���㷨 �����õ���4�㷨
		sorData.vEvent.push_back(event);
	}//efor
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//������� wcq2012.10.30
	sorData.BackscatterCoefficient = testResultInfo->BackscatterCoefficient;
	sorData.LossThreshold = testResultInfo->LossThreshold;
	sorData.ReflectanceThreshold = testResultInfo->ReflectanceThreshold;
	sorData.EndOfFiberThreshold = testResultInfo->EndOfFiberThreshold;
	sorData.Range = testResultInfo->Length;//��������Ϊ������Χ����λ��m
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


}

//��sor�ļ�����������ת��Ϊotdrtrace���� zll2013.04.23
//colorLine:������ɫ
void CSorAssist::AddSordataToOtdrTrace(SORDATA sorData, COtdrTrace* pOtdrTrace, WORD* pDataSmooth, COLORREF colorLine)
{
	TestResultInfo testResultInfo;
	convert(sorData, testResultInfo);

	std::vector<FLOAT> record;//
	record.clear();//�������Record��¼λ��
#ifdef COMPILE_SHOW_SMOOTH_DATA////��ʾʱ��ƽ���������ݣ�����ʱ��ʵ������pDataPointsSmooth wcq2013.04.26
	std::vector<FLOAT> recordSmooth;//
	recordSmooth.clear();//�������Record��¼λ��
#endif
	int factorCount = sorData.vAllData.size();//ϵ������
	int countEachFactor;
	INT nDataCount=0;
	//�����������ݷŵ�m_record��
	for (int i=0;i<factorCount;i++)
	{
		std::vector<unsigned short>& data = sorData.vAllData[i].vData;
		countEachFactor=data.size();
		nDataCount = countEachFactor;
		for (int j=0;j<countEachFactor;j++)
		{
			record.push_back((65535-(int)data[j])/1000.0);
		}//efor
	}//efor

#ifdef COMPILE_SHOW_SMOOTH_DATA////��ʾʱ��ƽ���������ݣ�����ʱ��ʵ������pDataPointsSmooth wcq2013.04.26
	for (int i=0;i<nDataCount;i++)
	{
		recordSmooth.push_back((65535-(int)pDataSmooth[i])/1000.0);
	}//efor
#endif


	CGraphElement element;
	element.m_nPointsCount = testResultInfo.DataPointsNum;
	element.m_fLength = testResultInfo.Length;//zll 2013.4.23��Ϊ����
	element.m_fDataSpacing = testResultInfo.DataSpacing;
	element.m_LineColor = colorLine;
	element.m_pData = new FLOAT[element.m_nPointsCount];
	INT nCount = element.m_nPointsCount;
#ifdef COMPILE_SHOW_SMOOTH_DATA////��ʾʱ��ƽ���������ݣ�����ʱ��ʵ������pDataPointsSmooth wcq2013.04.26
	element.m_pDataSmooth = new FLOAT[element.m_nPointsCount];
	memcpy(element.m_pDataSmooth, &recordSmooth[0], nCount * sizeof(FLOAT));
#endif
	memcpy(element.m_pData, &record[0], nCount * sizeof(FLOAT));
	SetEvents(testResultInfo, &element);
	element.m_bShowEvent = pOtdrTrace->m_bShowEventList;//wcq2015.05.06
	pOtdrTrace->m_vAllData.push_back(element);
	pOtdrTrace->SetMaxDistance(element.m_fLength);
	pOtdrTrace->SetScrollRangeX();//
	pOtdrTrace->SetRange(0, element.m_fLength, 0, 65.535);//
	pOtdrTrace->OnInitAB();  //lzy2014.9.10
    pOtdrTrace->SetCurrentSeleTrace(pOtdrTrace->m_vAllData.size()-1);
	
	pOtdrTrace->DrawCurves(&pOtdrTrace->m_dcCurve,-1);
	//
	record.clear();
	//delete[] testResultInfo.pDataPoints;
}

//���¼��б�ת��Ϊ������ʾ�õ��¼��б�//zll2013.4.23
void CSorAssist::SetEvents(TestResultInfo& testResultInfo, CGraphElement* graphElement)
{
	graphElement->m_vAllEvents.clear();
	CGraphElement::TraceEvent te;
	float fTemp;
	for (int i=0; i< testResultInfo.EventList.vEventInfo.size(); i++)
	{
		fTemp = testResultInfo.DataSpacing * testResultInfo.EventList.vEventInfo[i].index;
		//TRACE(_T("i:%d index:%d distance:%.4f distance by Dataspacing:%.4f\n"),i,testResultInfo.EventList.vEventInfo[i].index, testResultInfo.DataSpacing*testResultInfo.EventList.vEventInfo[i].index,fTemp);
		te.m_nIndex = testResultInfo.EventList.vEventInfo[i].index;
		te.m_distance = testResultInfo.EventList.vEventInfo[i].fLocation;
		//TRACE(_T("distance:%.4f\n"),te.m_distance);
		graphElement->m_vAllEvents.push_back(te);
	}//eif
}

//��sor�ļ�����������ת��Ϊotdrtrace���� zll2013.5.8
//colorLine:������ɫ
void CSorAssist::CloseCurvedata(int m_listItem,SORDATA sorData, COtdrTrace* pOtdrTrace, COLORREF colorLine)
{
	int temp=-1;
	std::vector<CGraphElement>::iterator it=pOtdrTrace->m_vAllData.begin();
	for (;it!=pOtdrTrace->m_vAllData.end();it++)
	{
		temp++;
		if (m_listItem==temp)
		{
			pOtdrTrace->m_vAllData.erase(it);
			break;
		}
	}
	pOtdrTrace->DrawCurves(&pOtdrTrace->m_dcCurve,-1);
	pOtdrTrace->Invalidate(FALSE);//ˢ��
}

//zll 2013.6.18 ִ���Զ�����ʱ����sor�ļ�����������ת��Ϊotdrtrace����
void CSorAssist::ReEventAutoSearch(TestResultInfo& testResultInfo, SORDATA sorData, COtdrTrace* pOtdrTrace, WORD* pDataSmooth, COLORREF colorLine,int m_listItem)
{
	convert(&testResultInfo,sorData);//ע��������AddSordataToOtdrTrace(,,,)������convert�ǲ�ͬ��
	std::vector<FLOAT> record;//
	record.clear();//�������Record��¼λ��
#ifdef COMPILE_SHOW_SMOOTH_DATA////��ʾʱ��ƽ���������ݣ�����ʱ��ʵ������pDataPointsSmooth wcq2013.04.26
	std::vector<FLOAT> recordSmooth;//
	recordSmooth.clear();//�������Record��¼λ��
#endif
	int factorCount = sorData.vAllData.size();//ϵ������
	int countEachFactor;
	INT nDataCount=0;
	//�����������ݷŵ�m_record��
	for (int i=0;i<factorCount;i++)
	{
		std::vector<unsigned short>& data = sorData.vAllData[i].vData;
		countEachFactor=data.size();
		nDataCount = countEachFactor;
		for (int j=0;j<countEachFactor;j++)
		{
			record.push_back((65535-(int)data[j])/1000.0);
		}//efor
	}//efor

#ifdef COMPILE_SHOW_SMOOTH_DATA////��ʾʱ��ƽ���������ݣ�����ʱ��ʵ������pDataPointsSmooth wcq2013.04.26
	for (int i=0;i<nDataCount;i++)
	{
		recordSmooth.push_back((65535-(int)pDataSmooth[i])/1000.0);
	}//efor
#endif


	CGraphElement element;
	element.m_nPointsCount = testResultInfo.DataPointsNum;
	element.m_fLength = testResultInfo.Length;//zll 2013.4.23��Ϊ����
	element.m_fDataSpacing = testResultInfo.DataSpacing;
	element.m_LineColor = colorLine;
	element.m_pData = new FLOAT[element.m_nPointsCount];
	INT nCount = element.m_nPointsCount;
#ifdef COMPILE_SHOW_SMOOTH_DATA////��ʾʱ��ƽ���������ݣ�����ʱ��ʵ������pDataPointsSmooth wcq2013.04.26
	element.m_pDataSmooth = new FLOAT[element.m_nPointsCount];
	memcpy(element.m_pDataSmooth, &recordSmooth[0], nCount * sizeof(FLOAT));
#endif
	memcpy(element.m_pData, &record[0], nCount * sizeof(FLOAT));
	SetEvents(testResultInfo, &element);
	int temp=-1;
	std::vector<CGraphElement>::iterator it=pOtdrTrace->m_vAllData.begin();
	for (;it<=pOtdrTrace->m_vAllData.end();it++)//zll 2013.6.18 ע����it<=pOtdrTrace->m_vAllData.end()������it!=pOtdrTrace->m_vAllData.end()
	{
		temp++;
		if (m_listItem==temp)
		{
			pOtdrTrace->m_vAllData.insert(it,element);
			break;
		}
	}
	//pOtdrTrace->m_vAllData.push_back(element);
	//pOtdrTrace->SetMaxDistance(element.m_fLength);
	//pOtdrTrace->SetScrollRangeX();//
	//pOtdrTrace->SetRange(0, element.m_fLength, 0, 65.535);//
	pOtdrTrace->SetCurrentSeleTrace(pOtdrTrace->m_vAllData.size()-1);
	//pOtdrTrace->DrawCurves(&pOtdrTrace->m_dcCurve);
	//
	record.clear();
}

//��ȡ����Y������ֵlzy2014.06.23
int CSorAssist::GetTraceYmax(SORDATA &sorData)
{
	int factorCount = sorData.vAllData.size();//ϵ������
	int countEachFactor;
	INT nDataCount=0;
	int nValue = 0;
	int nMax = 0;
	//�����������ݷŵ�m_record��
	for (int i=0;i<factorCount;i++)
	{
		std::vector<unsigned short>& data = sorData.vAllData[i].vData;
		countEachFactor=data.size();
		nDataCount = countEachFactor;
		for (int j=0;j<countEachFactor;j++)
		{
			nValue = (int)((65535-(int)data[j])/1000.0);
			nMax = nMax < nValue ? nValue : nMax;

		}
	}

	return nMax;
	
}