#define _AFXEXT
#include "StdAfx.h"
#include "SorFileBase.h"
void writeShort(vector<BYTE>& vData,short nToWrite);
void writeInt(vector<BYTE>& vData,INT nToWrite);
void writeString(vector<BYTE>& vData,vector<char> vChar);//д���ַ�������'\0'
void writeString(vector<BYTE>& vData,vector<char> vChar,int len);//д��̶������ַ�������'\0'

CSorFileBase::CSorFileBase(void)
{
}

CSorFileBase::~CSorFileBase(void)
{
}
//��ʼ��һЩ��������һЩ������Ĭ��ֵ���ղ���������ݱ���ʱ��Ҫ��ʼ��
void CSorFileBase::init()
{
	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	//Map 
	memset(&m_mapBlock,0,sizeof(MapBlock));
	vchar2string(m_mapBlock.mapBlock.ID,string("Map"));
	m_mapBlock.mapBlock.RevisionNo = REVISION_NUM;

	MapBlock::blockInfo bInfo;
	const int paramsCount = 6;//������LnkParams
	string paras[paramsCount]={"GenParams","SupParams","FxdParams","KeyEvents",/*"LnkParams",*/"DataPts","Cksum"};
	for (int i=0;i<paramsCount;i++)
	{
		vchar2string(bInfo.ID,paras[i]);
		bInfo.RevisionNo = REVISION_NUM;
		m_mapBlock.vBlockInfo.push_back(bInfo);
	}
	m_mapBlock.blockCount = paramsCount +1 ;//����map����

	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	//GenParams/GeneralParas
	memset(&m_generalParas,0,sizeof(GeneralParas));
	vchar2string(m_generalParas.ID,string("GenParams"));
	vchar2string(m_generalParas.languageCode,string("CH"));
	//m_generalParas.languageCode.push_back('C');
	//m_generalParas.languageCode.push_back('H');
	vchar2string(m_generalParas.currentDataFlag,string("BC"));

	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	//SupParams/SuppliersParas
	memset(&m_suppliersParas,0,sizeof(SuppliersParas));
	vchar2string(m_suppliersParas.ID,string("SupParams"));
	vchar2string(m_suppliersParas.name,string("Grandway"));

	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	//FxdParams/FixedParas
	memset(&m_fixedParas,0,sizeof(FixedParas));
	vchar2string(m_fixedParas.ID,string("FxdParams"));
	m_fixedParas.vPulseWidth.push_back(10000);//Ĭ��10us
	m_fixedParas.vDataSpacing.push_back(0);
	m_fixedParas.vDataPointsEach.push_back(0);
	vchar2string(m_fixedParas.distanceUnit,string("km"));
	vchar2string(m_fixedParas.traceType,string("ST"));
	m_fixedParas.pulseWidthNum=1;//�������Ϊ1
	m_fixedParas.windowCoor1 = 0;
	m_fixedParas.windowCoor2 = 0;
	m_fixedParas.windowCoor3 = 0;
	m_fixedParas.windowCoor4 = 0;

	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	//KeyEvents
	memset(&m_keyEvent,0,sizeof(KeyEvent));
	vchar2string(m_keyEvent.ID,string("KeyEvents"));
	m_keyEvent.keyEventNum = 0;//�¼������Ĭ��Ϊ0

#ifdef B_INCLUDE_LNKPARAMS
	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	//LnkParams/linkParams
	memset(&m_linkParas,0,sizeof(LinkParas));
	vchar2string(m_linkParas.ID,string("LnkParams"));
#endif

	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	//DataPts
	memset(&m_dataPoints,0,sizeof(DataPoints));
	vchar2string(m_dataPoints.ID,string("DataPts"));
	m_dataPoints.dataPointNum=0;//
	m_dataPoints.totalScaleFactors=1;
	DataPoints::DataUseScaleFactor dus;
	dus.scaleFactor = 1000;
	m_dataPoints.vAllData.push_back(dus);

	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	//Cksum
	memset(&m_checksum,0,sizeof(Checksum));
	vchar2string(m_checksum.ID,string("Cksum"));
}

////������������ݸ�������Ĳ���
//BOOL CSorFileBase::setReturnData(TestResultInfo& testResultInfo)
//{
//	//
//	if (NULL !=testResultInfo.pDataPoints)//�����ԭ�еĸ�������
//	{
//		delete[] testResultInfo.pDataPoints;//
//		testResultInfo.pDataPoints= NULL;
//	}
//	testResultInfo.DataPointsNum = m_dataPoints.dataPointNum;//���ݵ����
//	if (0 != m_dataPoints.dataPointNum)
//	{
//		testResultInfo.pDataPoints = new WORD[m_dataPoints.vAllData[0].count];//ֻ����ֻ��һ��������������????????????????????????
//		memcpy(testResultInfo.pDataPoints,&(m_dataPoints.vAllData[0].vData[0]),m_dataPoints.vAllData[0].count * 2);////memcpy���ֽڣ�����Ҫ��2
//		//if (1000 != m_dataPoints.vAllData[0].scaleFactor)//���ϵ����һ�����������Ӧ��ϵ��//wcq2011.11.30 �Ȳ�����????
//		//{
//		//	int count=m_dataPoints.vAllData[0].count;
//		//	float factor = m_dataPoints.vAllData[0].scaleFactor * 1.0/1000;
//		//	for(int i=0;i<count;i++)
//		//	{
//		//		testResultInfo.pDataPoints[i] = testResultInfo.pDataPoints[i] * factor;
//		//	}
//		//}
//	}
//	//����ϵ��wcq2011.10.21
//	testResultInfo.ScaleFactor = m_dataPoints.vAllData[0].scaleFactor;
//
//	//�����������
//	float fC=299792458.0;
//	float fReflect=m_fixedParas.groupIndex /100000.0;//������
//	float numEachTime=m_fixedParas.vDataSpacing[0]/100000000000000.0;//�����ʱ��//wcq2011.11.25
//	//DEBUGMSG(true,(_T("numEachTime :%.2f\n"),numEachTime));
//	numEachTime = C_LIGHT_SPEED * (numEachTime) / fReflect;//ÿ����� =�����ʱ�� * ���ڹ����ٶ�//wcq2011.11.25
//	//DEBUGMSG(true,(_T("numEachTime 2:%.2f\n"),numEachTime));
//	//testResultInfo.Length =C_LIGHT_SPEED * (numEachTime) * m_fixedParas.vDataPointsEach[0] /fReflect;//
//	testResultInfo.Length = numEachTime * m_fixedParas.vDataPointsEach[0];//ÿ����� * ����//wcq2011.11.25
//	////DataSpacing:for each pulse width used:one-way time(units of 100 psec) for 10000 data points//wcq2011.10.19���Ϊ��������
//	//������ ��������
//	testResultInfo.Refraction = fReflect;
//	//testResultInfo.DataSpacing = numEachTime;
//	testResultInfo.DataSpacing = numEachTime;//wcq2011.11.25//m_fixedParas.vDataSpacing[0];//2011.10.21
//	//DEBUGMSG(true,(_T("numEachTime :%.2f   DataSpacing :%.2f\n"),testResultInfo.DataSpacing,testResultInfo.DataSpacing));
//	//����
//	//testResultInfo.WaveLength = m_fixedParas.actualWavelength;
//	testResultInfo.WaveLength = m_fixedParas.actualWavelength/10;//wcq2012.02.07
//
//	//����
//	testResultInfo.PulseWidth = m_fixedParas.vPulseWidth[0];
//
//	//
//	//����Ƶ��
//	//testResultInfo.Frequency = (1/numEachTime)*1000000;
//	testResultInfo.Frequency = 100000000.0 / (m_fixedParas.vDataSpacing[0] * 2);//wcq2012.06.12
//
//	//�¼��б� wcq2012.06.11
//	testResultInfo.EventList.vEventInfo.clear();
//	int count = m_keyEvent.vEvent.size();
//	if (count > 0)//����ļ����У���ֵ
//	{
//		for (int i=0;i<count;i++)
//		{
//			EventInfo ei;
//			if (m_keyEvent.vEvent[i].code.size()>5)//ʵ�ʳ���Ӧ��Ϊ6
//			{
//				ei.nEventType = m_keyEvent.vEvent[i].code[0] - '0';//�¼�����
//				ei.cEventType = m_keyEvent.vEvent[i].code[1];//�¼������ɵĻ����û���ӻ���β���¼���
//			}//eif
//			///ei.fLocation = (m_keyEvent.vEvent[i].propagationTime / (testResultInfo.Refraction  * pow(10.0,-10))) * C_LIGHT_SPEED;//λ��
//
//			ei.fLocation = m_keyEvent.vEvent[i].propagationTime * pow(10.0,-10) * C_LIGHT_SPEED / (testResultInfo.Refraction ) ;//λ��
//			//event.propagationTime = (testResultInfo.EventList.vEventInfo[i].fLocation * pow(10.0,-10)/C_LIGHT_SPEED ) * testResultInfo.Refraction;//����/����(km/ps) * ������ λ��
//			ei.index = (ei.fLocation / testResultInfo.Length)*testResultInfo.DataPointsNum;//�������������±�,����û�м���?????????????????????????????????????
//			//��ֹ������Χ
//			if (ei.index <0)
//			{
//				ei.index =0;
//			}
//			else if (ei.index > testResultInfo.DataPointsNum)
//			{
//				ei.index = testResultInfo.DataPointsNum;//
//			}//eif
//			ei.fEventloss = m_keyEvent.vEvent[i].eventLoss / 1000.0;//���
//			ei.fEventHigh = m_keyEvent.vEvent[i].reflectance /1000.0;//����dB��otdr�����¼��б�
//			ei.fLineK = m_keyEvent.vEvent[i].attenCoefficent / 1000.0;//б��
//			//ei.fTotalLoss = m_keyEvent.vEvent[i]//�����????????????????????
//			if (0 == i)
//			{
//				ei.fTotalLoss = ei.fEventloss;
//			}
//			else
//			{
//				ei.fTotalLoss = testResultInfo.EventList.vEventInfo[i-1].fTotalLoss/*ǰһ�¼������*/ + ei.fEventloss/*��ǰ�¼����*/ + (ei.fLineK/1000) *(ei.fLocation - testResultInfo.EventList.vEventInfo[i-1].fLocation);
//			}//eif
//			ei.fReflect = 0;//�������û���õ�
//			ei.fTrailEnd =0;////�������û���õ�C/(2nf))*index
//			testResultInfo.EventList.vEventInfo.push_back(ei);
//		}//efor
//	}//eif count > 0
//	return TRUE;
//}
////////////////////////////////////////////////////////////////////////////////////
//����˵��://���ļ����������block����������漸��read����
//��ڲ���:data:�ļ�����;len���ļ�����
//���ڲ���:
//����ʱ��:2011.10.11
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
BOOL CSorFileBase::parseData(BYTE fileData[],int len)
{
	//�������OTDR�豸�����Զ����Block����
	for(int i=0;i<m_vUnknownParas.size();i++)//����Block���
	{
		m_vUnknownParas[i].clear();
	}
	m_vUnknownParas.clear();//�������
	int index=0;//�ļ��±�
	//string temp;
	char* pChar;
	int IDLen=0;
	if(readMap(fileData))//�鿴�Ƿ�Ϊ.sor�ļ���ʽ������ǣ���ȡMap���ݿ�
	{
		if (!checkSize(len))//�ļ���С�͸���Block֮�Ͳ�һ��������Ϊ�ļ���Ч,��ֹ�ļ���ȡ��bug.
		{
			//delete[] fileData;//ɾ����ʱ����2011.12.01
			return FALSE;
		}

		index += m_mapBlock.mapBlock.Size;//����map Block�Ĵ�С
		int count=m_mapBlock.blockCount-1;//�ܹ��ж��ٸ�Block,��ȥMap Block����
		for (int i=0;i<count;i++)
		{
			//vchar2string(temp,m_mapBlock.vBlockInfo[i].ID);
			IDLen=m_mapBlock.vBlockInfo[i].ID.size();
			pChar=&m_mapBlock.vBlockInfo[i].ID[0];
			if (0 == strncmp(pChar,"GenParams",IDLen))//
			{
				readGeneralParas(fileData + index);
			}
			else if (0 == strncmp(pChar,"SupParams",IDLen))
			{
				readSuppliersParas(fileData + index);
			}
			else if (0 == strncmp(pChar,"FxdParams",IDLen))
			{
				readFixedParas(fileData + index);
			}
			else if (0 == strncmp(pChar,"KeyEvents",IDLen))
			{
				readKeyEvents(fileData + index);
			}
			else if (0 == strncmp(pChar,"LnkParams",IDLen))
			{
				readLinkParas(fileData + index);
			}
			else if (0 == strncmp(pChar,"DataPts",IDLen))
			{
				readDataPts(fileData + index);
			}
			else if (0 == strncmp(pChar,"Cksum",IDLen) || 0 == strncmp(pChar,"CKsum",IDLen))//��׼ӦΪCksum,���ǵ������õ���CKsum
			{
				readCKsum(fileData + index);
			}
			else
			{
				vector<BYTE> vParams(fileData + index,fileData + index + m_mapBlock.vBlockInfo[i].Size);
				m_vUnknownParas.push_back(vParams);
			}////eif
			index += m_mapBlock.vBlockInfo[i].Size;
		}//efor
	}//eif
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////
//����˵��:��ȡMapBlock��
//��ڲ���:data:BYTE����;
//���ڲ���:
//����ʱ��:2011.7.8
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
INT CSorFileBase::readMap(BYTE data[])//
{
	vchar2string(m_mapBlock.mapBlock.ID,string("Map"));
	int blockLen=0;//block����
	//Map��־
	//if(_T("Map")!=readString(data,0))//��Ϊ�ж�
	if('M'!=data[0] || 'a'!=data[1] || 'p'!=data[2] || '\0'!=data[3])
	{
		return FALSE;
	}
	//
	//m_parsePos=4;//�޶��汾����ʼλ��
	blockLen=4;
	m_mapBlock.mapBlock.RevisionNo=TWOINT(data,blockLen);
	//m_parsePos=6;//������ʼλ��
	blockLen=6;
	m_mapBlock.mapBlock.Size=FOURINT(data,blockLen);
	//m_parsePos=10;//Block������ʼλ��
	blockLen=10;//Block������ʼλ��
	INT count=TWOINT(data,blockLen);//Block����
	m_mapBlock.blockCount=count;
	count--;//��ȥMap Block����
	MapBlock::blockInfo block;
	//m_parsePos=12;//��һ��blockλ��
	blockLen=12;//��һ��blockλ��
	m_mapBlock.vBlockInfo.clear();//�����ǰ�����ݿ��¼
	int strLen=0;
	for (int i=0;i<count;i++)
	{//int readString(BYTE data[],vector<char> vChar)
		//block.ID=readString(data,m_parsePos);//����
		strLen=readString(data+blockLen,block.ID);
		//m_parsePos+=block.ID.GetLength()+1;//�޶��汾����ʼλ��
		blockLen+=strLen+1;//�޶��汾����ʼλ��
		//block.RevisionNo=TWOINT(data,m_parsePos);
		block.RevisionNo=TWOINT(data,blockLen);
		//m_parsePos+=2;//������ʼλ��
		blockLen+=2;//������ʼλ��
		//block.Size=FOURINT(data,m_parsePos);
		block.Size=FOURINT(data,blockLen);
		//m_parsePos+=4;//
		blockLen+=4;//
		m_mapBlock.vBlockInfo.push_back(block);
	}
	return blockLen;
}

////////////////////////////////////////////////////////////////////////////////////
//����˵��:��ȡGeneralParas Block������ͨ��7.8
//��ڲ���:data:BYTE����
//���ڲ���:
//����ʱ��:2011.7.8
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
INT CSorFileBase::readGeneralParas(BYTE data[])
{
	vchar2string(m_generalParas.ID,string("GenParams"));
	int blockLen=0;//block����
	int strLen=0;
	blockLen = 10;//���Ա�����ʼλ��,����GenParams\0
	m_generalParas.languageCode.clear();
	m_generalParas.languageCode.push_back(data[blockLen]);
	m_generalParas.languageCode.push_back(data[blockLen+1]);
	//m_parsePos+=2;//���±����ʼλ��
	blockLen += 2;//���±����ʼλ��

	//m_generalParas.cableID=readString(data,m_parsePos);//���±��
	strLen=readString(data + blockLen,m_generalParas.cableID);
	blockLen += strLen+1;//���˱����ʼλ��

	//m_generalParas.fiberID=readString(data,m_parsePos);//���˱��
	strLen=readString(data + blockLen,m_generalParas.fiberID);
	//m_parsePos+=m_generalParas.fiberID.GetLength()+1;//���˱����ʼλ��
	blockLen += strLen+1;//���˱����ʼλ��

	//m_generalParas.fiberType=TWOINT(data,m_parsePos);//��������
	m_generalParas.fiberType=TWOINT(data,blockLen);//��������

	//m_parsePos+=2;//�������ʼλ��
	blockLen+=2;//�������ʼλ��
	m_generalParas.nominalWaveLength=TWOINT(data,blockLen);//�����

	//m_parsePos+=2;//��ʼλ��
	blockLen+=2;//��ʼλ��
	//m_generalParas.OLocation=readString(data,m_parsePos);//Originating Location
	strLen=readString(data + blockLen,m_generalParas.OLocation);

	//m_parsePos+=m_generalParas.OLocation.GetLength()+1;//��ʼλ��
	blockLen += strLen+1;//��ʼλ��
	//m_generalParas.TLocation=readString(data,m_parsePos);//Terminating Location
	strLen=readString(data + blockLen,m_generalParas.TLocation);

	//m_parsePos+=m_generalParas.TLocation.GetLength()+1;//��ʼλ��
	blockLen += strLen+1;//��ʼλ��
	//m_generalParas.cableCode=readString(data,m_parsePos);//
	strLen=readString(data + blockLen,m_generalParas.cableCode);

	//m_parsePos+=m_generalParas.cableCode.GetLength()+1;//��ʼλ��
	blockLen += strLen+1;//��ʼλ��
	//temp.Format(_T("%C%C"),data[m_parsePos],data[m_parsePos+1]);
	//m_generalParas.currentDataFlag=temp;//Terminating Location
	m_generalParas.currentDataFlag.clear();
	m_generalParas.currentDataFlag.push_back(data[blockLen]);
	m_generalParas.currentDataFlag.push_back(data[blockLen+1]);

	//m_parsePos+=2;//��ʼλ��
	blockLen += 2;//��ʼλ��
	//m_generalParas.userOffset=FOURINT(data,m_parsePos);//
	m_generalParas.userOffset=FOURINT(data,blockLen);//

	//m_parsePos+=4;//��ʼλ��
	blockLen += 4;//��ʼλ��
	m_generalParas.uODistance=FOURINT(data,blockLen);//user Offset Distance

	//m_parsePos+=4;//������Ա��ʼλ��
	blockLen += 4;//��ʼλ��
	//m_generalParas.operater=readString(data,m_parsePos);//������Ա
	strLen=readString(data + blockLen,m_generalParas.operater);

	//m_parsePos+=m_generalParas.operater.GetLength()+1;//ע����ʼλ��
	blockLen += strLen+1;//��ʼλ��
	//m_generalParas.comment=readString(data,m_parsePos);//ע��
	strLen=readString(data + blockLen,m_generalParas.comment);

	//m_parsePos+=m_generalParas.comment.GetLength()+1;//��һ��¼��ʼλ��
	blockLen += strLen+1;//��ʼλ��

	return blockLen;
}

////////////////////////////////////////////////////////////////////////////////////
//����˵��:��ȡSuppliersParas Block������ͨ��7.8
//��ڲ���:data:BYTE����
//���ڲ���:
//����ʱ��:2011.7.8
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
INT CSorFileBase::readSuppliersParas(BYTE data[])
{
	vchar2string(m_suppliersParas.ID,string("SupParams"));
	int blockLen=0;//block����
	int strLen=0;
	blockLen+=10;//����������ʼλ��,����SupParams\0
	strLen=readString(data + blockLen,m_suppliersParas.name);

	blockLen+=strLen+1;//������ͺ���ʼλ��
	strLen=readString(data + blockLen,m_suppliersParas.mainframeID);//������ͺ�

	blockLen+=strLen+1;//��������к���ʼλ��
	strLen=readString(data + blockLen,m_suppliersParas.mainframeSn);//��������к�

	blockLen+=strLen+1;//���ģ�����ʼλ��
	strLen=readString(data + blockLen,m_suppliersParas.moduleID);//���ģ���

	blockLen+=strLen+1;//�����plug-in�����к���ʼλ��
	strLen=readString(data + blockLen,m_suppliersParas.moduleSn);//�����plug-in�����к�

	blockLen+=strLen+1;//��Ӳ���汾����ʼλ��
	strLen=readString(data + blockLen,m_suppliersParas.softVersion);//��Ӳ���汾��

	blockLen+=strLen+1;//������Ϣ��ʼλ��
	strLen=readString(data + blockLen,m_suppliersParas.other);//������Ϣ

	blockLen+=strLen+1;//��һ��¼��ʼλ��
	return blockLen;
	//m_parsePos+=10;//����������ʼλ��,����SupParams\0
	//m_suppliersParas.name=readString(data,m_parsePos);//��������

	//m_parsePos+=m_suppliersParas.name.GetLength()+1;//������ͺ���ʼλ��
	//m_suppliersParas.mainframeID=readString(data,m_parsePos);//������ͺ�

	//m_parsePos+=m_suppliersParas.mainframeID.GetLength()+1;//��������к���ʼλ��
	//m_suppliersParas.mainframeSn=readString(data,m_parsePos);//��������к�

	//m_parsePos+=m_suppliersParas.mainframeSn.GetLength()+1;//���ģ�����ʼλ��
	//m_suppliersParas.moduleID=readString(data,m_parsePos);//���ģ���

	//m_parsePos+=m_suppliersParas.moduleID.GetLength()+1;//�����plug-in�����к���ʼλ��
	//m_suppliersParas.moduleSn=readString(data,m_parsePos);//�����plug-in�����к�

	//m_parsePos+=m_suppliersParas.moduleSn.GetLength()+1;//��Ӳ���汾����ʼλ��
	//m_suppliersParas.softVersion=readString(data,m_parsePos);//��Ӳ���汾��

	//m_parsePos+=m_suppliersParas.softVersion.GetLength()+1;//������Ϣ��ʼλ��
	//m_suppliersParas.other=readString(data,m_parsePos);//������Ϣ

	//m_parsePos+=m_suppliersParas.other.GetLength()+1;//��һ��¼��ʼλ��

	//return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
//����˵��:��ȡFxdParams Block��
//��ڲ���:data:BYTE����
//���ڲ���:
//����ʱ��:2011.7.8
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
INT CSorFileBase::readFixedParas(BYTE data[])
{
	vchar2string(m_fixedParas.ID,string("FxdParams"));
	int blockLen=0;//block����
	int strLen=0;
	blockLen+=10;//ʱ����ʼλ��,����FxdParams\0
	m_fixedParas.timestamp=FOURINT(data,blockLen);//

	blockLen+=4;//��λ��ʼλ��
	m_fixedParas.distanceUnit.clear();
	m_fixedParas.distanceUnit.push_back(data[blockLen]);
	m_fixedParas.distanceUnit.push_back(data[blockLen+1]);

	blockLen+=2;//ʵ�ʲ���������ʼλ��
	m_fixedParas.actualWavelength=TWOINT(data,blockLen);//

	blockLen+=2;//acquisition Offset������ʼλ��
	m_fixedParas.acquisitionOffset=FOURINT(data,blockLen);//

	blockLen+=4;//acquisition Offset Distance��ʼλ��
	m_fixedParas.AODistance=FOURINT(data,blockLen);//

	blockLen+=4;//���������ʼλ��
	int itemp=TWOINT(data,blockLen);
	m_fixedParas.pulseWidthNum=itemp;//

	blockLen+=2;//��ʼλ��
	m_fixedParas.vPulseWidth.clear();
	for(int i=0;i<itemp;i++)
	{
		m_fixedParas.vPulseWidth.push_back(TWOINT(data,blockLen));
		blockLen+=2;//��һ��λ��
	}

	//blockLen+=2;//��������ѭ���˼ӹ�
	//��ͬ����10000�����ݵ�ʱ����ʼλ��
	m_fixedParas.vDataSpacing.clear();
	for(int i=0;i<itemp;i++)
	{
		m_fixedParas.vDataSpacing.push_back(FOURINT(data,blockLen));
		blockLen+=4;//��һ��λ��
	}

	//blockLen+=4;//��������ѭ���˼ӹ�
	//ÿ���������ݵ���� ��ʼλ��
	m_fixedParas.vDataPointsEach.clear();
	for(int i=0;i<itemp;i++)
	{
		m_fixedParas.vDataPointsEach.push_back(FOURINT(data,blockLen));
		blockLen+=4;//��һ��λ��
	}

	//blockLen+=4;//group Index��������ѭ���мӹ�.��ʼλ��
	m_fixedParas.groupIndex=FOURINT(data,blockLen);

	blockLen+=4;//backscatter ��ʼλ��
	m_fixedParas.backscatter=TWOINT(data,blockLen);

	blockLen+=2;//average number��ʼλ��
	m_fixedParas.avgNum=FOURINT(data,blockLen);

	blockLen+=4;//average time ��ʼλ��
	m_fixedParas.avgTime=TWOINT(data,blockLen);

	blockLen+=2;//acquisitionRange��ʼλ��
	m_fixedParas.acquisitionRange=FOURINT(data,blockLen);

	blockLen+=4;//acquisitionRange distance��ʼλ��
	m_fixedParas.ARDistance=FOURINT(data,blockLen);

	blockLen+=4;//front Panel Offset��ʼλ��
	m_fixedParas.frontPanelOff=FOURINT(data,blockLen);

	blockLen+=4;//noise Floor Level ��ʼλ��
	m_fixedParas.noiseFloorLevel=TWOINT(data,blockLen);

	blockLen+=2;//��������ϵ�� ��ʼλ��
	m_fixedParas.noiseFloorFactor=TWOINT(data,blockLen);

	blockLen+=2;//power offset first point��ʼλ��
	m_fixedParas.powerOff=TWOINT(data,blockLen);

	blockLen+=2;//loss Threshold��ʼλ��
	m_fixedParas.lossThreshold=TWOINT(data,blockLen);

	blockLen+=2;//��������ʼλ��
	m_fixedParas.reflectanceThreshod=TWOINT(data,blockLen);

	blockLen+=2;//end-of-fiber threshold��ʼλ��
	m_fixedParas.eofThreshold=TWOINT(data,blockLen);

	blockLen+=2;//trace Type׷��������ʼλ��,��׼��˵��string��ʵ������string �̶�2�ֽڣ�����û\0
	//m_fixedParas.traceType=readString(data,blockLen);//
	m_fixedParas.traceType.clear();
	m_fixedParas.traceType.push_back(data[blockLen]);
	m_fixedParas.traceType.push_back(data[blockLen+1]);

	blockLen+=2;//window coordinates��ʼλ��
	m_fixedParas.windowCoor1=FOURINT(data,blockLen);
	blockLen+=4;//
	m_fixedParas.windowCoor2=FOURINT(data,blockLen);
	blockLen+=4;//
	m_fixedParas.windowCoor3=FOURINT(data,blockLen);
	blockLen+=4;//
	m_fixedParas.windowCoor4=FOURINT(data,blockLen);


	blockLen+=4;//��һ��¼��ʼλ��

	return blockLen;

	//CString temp;
	//m_parsePos+=10;//ʱ����ʼλ��,����FxdParams\0
	//m_fixedParas.timestamp=FOURINT(data,m_parsePos);//

	//m_parsePos+=4;//��λ��ʼλ��
	//temp.Format(_T("%C%C"),data[m_parsePos],data[m_parsePos+1]);
	//m_fixedParas.distanceUnit=temp;//��λ

	//m_parsePos+=2;//ʵ�ʲ���������ʼλ��
	//m_fixedParas.actualWavelength=TWOINT(data,m_parsePos);//

	//m_parsePos+=2;//acquisition Offset������ʼλ��
	//m_fixedParas.acquisitionOffset=FOURINT(data,m_parsePos);//

	//m_parsePos+=4;//acquisition Offset Distance��ʼλ��
	//m_fixedParas.AODistance=FOURINT(data,m_parsePos);//

	//m_parsePos+=4;//���������ʼλ��
	//int itemp=TWOINT(data,m_parsePos);
	//m_fixedParas.pulseWidthNum=itemp;//

	//m_parsePos+=2;//��ʼλ��
	//m_fixedParas.vPulseWidth.clear();
	//for(int i=0;i<itemp;i++)
	//{
	//	m_fixedParas.vPulseWidth.push_back(TWOINT(data,m_parsePos));
	//	m_parsePos+=2;//��һ��λ��
	//}

	////m_parsePos+=2;//��������ѭ���˼ӹ�
	////��ͬ����10000�����ݵ�ʱ����ʼλ��
	//m_fixedParas.vDataSpacing.clear();
	//for(int i=0;i<itemp;i++)
	//{
	//	m_fixedParas.vDataSpacing.push_back(FOURINT(data,m_parsePos));
	//	m_parsePos+=4;//��һ��λ��
	//}

	////m_parsePos+=4;//��������ѭ���˼ӹ�
	////ÿ���������ݵ���� ��ʼλ��
	//m_fixedParas.vDataPointsEach.clear();
	//for(int i=0;i<itemp;i++)
	//{
	//	m_fixedParas.vDataPointsEach.push_back(FOURINT(data,m_parsePos));
	//	m_parsePos+=4;//��һ��λ��
	//}

	////m_parsePos+=4;//group Index��������ѭ���мӹ�.��ʼλ��
	//m_fixedParas.groupIndex=FOURINT(data,m_parsePos);

	//m_parsePos+=4;//backscatter ��ʼλ��
	//m_fixedParas.backscatter=TWOINT(data,m_parsePos);

	//m_parsePos+=2;//average number��ʼλ��
	//m_fixedParas.avgNum=FOURINT(data,m_parsePos);

	//m_parsePos+=4;//average time ��ʼλ��
	//m_fixedParas.avgTime=TWOINT(data,m_parsePos);

	//m_parsePos+=2;//acquisitionRange��ʼλ��
	//m_fixedParas.acquisitionRange=FOURINT(data,m_parsePos);

	//m_parsePos+=4;//acquisitionRange distance��ʼλ��
	//m_fixedParas.ARDistance=FOURINT(data,m_parsePos);

	//m_parsePos+=4;//front Panel Offset��ʼλ��
	//m_fixedParas.frontPanelOff=FOURINT(data,m_parsePos);

	//m_parsePos+=4;//noise Floor Level ��ʼλ��
	//m_fixedParas.noiseFloorLevel=TWOINT(data,m_parsePos);

	//m_parsePos+=2;//��������ϵ�� ��ʼλ��
	//m_fixedParas.noiseFloorFactor=TWOINT(data,m_parsePos);

	//m_parsePos+=2;//power offset first point��ʼλ��
	//m_fixedParas.powerOff=TWOINT(data,m_parsePos);

	//m_parsePos+=2;//loss Threshold��ʼλ��
	//m_fixedParas.lossThreshold=TWOINT(data,m_parsePos);

	//m_parsePos+=2;//��������ʼλ��
	//m_fixedParas.reflectanceThreshod=TWOINT(data,m_parsePos);

	//m_parsePos+=2;//end-of-fiber threshold��ʼλ��
	//m_fixedParas.eofThreshold=TWOINT(data,m_parsePos);

	//m_parsePos+=2;//trace Type׷��������ʼλ��
	//m_fixedParas.traceType=readString(data,m_parsePos);//

	//m_parsePos+=m_fixedParas.traceType.GetLength()+1;//window coordinates��ʼλ��
	//m_fixedParas.windowCoor1=FOURINT(data,m_parsePos);
	//m_parsePos+=4;//
	//m_fixedParas.windowCoor2=FOURINT(data,m_parsePos);
	//m_parsePos+=4;//
	//m_fixedParas.windowCoor3=FOURINT(data,m_parsePos);
	//m_parsePos+=4;//
	//m_fixedParas.windowCoor4=FOURINT(data,m_parsePos);


	//m_parsePos+=4;//��һ��¼��ʼλ��

	//return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
//����˵��:��ȡreadKeyEvents Block����û����?????????????????????????????????????????????
//��ڲ���:data:BYTE����
//���ڲ���:
//����ʱ��:2011.7.8
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
INT CSorFileBase::readKeyEvents(BYTE data[])
{
	vchar2string(m_keyEvent.ID,string("KeyEvents"));
	int blockLen=0;//block����
	int strLen=0;
	blockLen+=10;//�¼�������ʼλ��,����KeyEvents\0
	int count=TWOINT(data,blockLen);
	m_keyEvent.keyEventNum=count;//

	KeyEvent::Event event;
	blockLen+=2;//��һ���¼���ʼλ��
	m_keyEvent.vEvent.clear();
	for (int i=0;i<count;i++)
	{
		event.eventNum=TWOINT(data,blockLen);//���

		blockLen+=2;//���Ӻ��һ��e�¼���ʱ����ʼλ��
		event.propagationTime=FOURINT(data,blockLen);//

		blockLen+=4;//˥��ϵ�� ��ʼλ��
		event.attenCoefficent=TWOINT(data,blockLen);//

		blockLen+=2;//�¼���� ��ʼλ��
		event.eventLoss=TWOINT(data,blockLen);//

		blockLen+=2;//�¼����� ��ʼλ��
		event.reflectance=FOURINT(data,blockLen);//

		blockLen+=4;//�¼���־ ��ʼλ��
		//temp.Format(_T("%C%C%C%C%C%C"),data[blockLen],data[blockLen+1],data[blockLen+2],data[blockLen+3],data[blockLen+4],data[blockLen+5]);
		event.code.clear();
		for (int i=0;i<6;i++)//6�ֽ�
		{
			event.code.push_back(data[blockLen+i]);
		}

		blockLen+=6;//loss Measurement Technique��ʼλ��
		//temp.Format(_T("%C%C"),data[blockLen],data[blockLen+1]);//2�ֽ�
		event.lossMeasurement.clear();
		event.lossMeasurement.push_back(data[blockLen]);
		event.lossMeasurement.push_back(data[blockLen+1]);
		//event.lossMeasurement=temp;

		blockLen+=2;//marker Locations��ʼλ��
		for (int j=0;j<5;j++)
		{
			event.markerLocations[j]=FOURINT(data,blockLen);//
			blockLen+=4;//ע�� ��ʼλ��
		}

		//event.comment=readString(data,blockLen);//
		strLen=readString(data + blockLen,event.comment);//

		blockLen+=strLen+1;//��һ��¼��ʼλ��

		m_keyEvent.vEvent.push_back(event);
	}//efor


	///blockLen+=2;//�Ѽӹ�End-to-End Loss ��ʼλ��
	m_keyEvent.eteLoss=FOURINT(data,blockLen);//

	blockLen+=4;//End-to-End Marker Positions Start ��ʼλ��
	m_keyEvent.eteMarkerStart=FOURINT(data,blockLen);//

	blockLen+=4;//End-to-End Marker Positions Finish ��ʼλ��
	m_keyEvent.eteMarkerFinish=FOURINT(data,blockLen);//

	blockLen+=4;//���� ��ʼλ��
	m_keyEvent.returnLoss=TWOINT(data,blockLen);//

	blockLen+=2;//�����־��� ��ʼλ��
	m_keyEvent.RLMarkerStart=FOURINT(data,blockLen);//

	blockLen+=4;//�����־������ ��ʼλ��
	m_keyEvent.RLMarkerFinish=FOURINT(data,blockLen);//

	blockLen+=4;//��һ��¼��ʼλ��

	return blockLen;

	//m_parsePos+=10;//�¼�������ʼλ��,����LnkParams\0
	//int count=TWOINT(data,m_parsePos);
	//m_keyEvent.keyEventNum=count;//

	//KeyEvent::Event event;
	//m_parsePos+=2;//��һ���¼���ʼλ��
	//CString temp;
	//m_keyEvent.vEvent.clear();
	//for (int i=0;i<count;i++)
	//{
	//	event.eventNum=TWOINT(data,m_parsePos);//���

	//	m_parsePos+=2;//���Ӻ��һ��e�¼���ʱ����ʼλ��
	//	event.propagationTime=FOURINT(data,m_parsePos);//

	//	m_parsePos+=4;//˥��ϵ�� ��ʼλ��
	//	event.attenCoefficent=TWOINT(data,m_parsePos);//

	//	m_parsePos+=2;//�¼���� ��ʼλ��
	//	event.eventLoss=TWOINT(data,m_parsePos);//

	//	m_parsePos+=2;//�¼����� ��ʼλ��
	//	event.reflectance=FOURINT(data,m_parsePos);//

	//	m_parsePos+=4;//�¼���־ ��ʼλ��
	//	temp.Format(_T("%C%C%C%C%C%C"),data[m_parsePos],data[m_parsePos+1],data[m_parsePos+2],data[m_parsePos+3],data[m_parsePos+4],data[m_parsePos+5]);
	//	event.code=temp;//6�ֽ�

	//	m_parsePos+=6;//loss Measurement Technique��ʼλ��
	//	temp.Format(_T("%C%C"),data[m_parsePos],data[m_parsePos+1]);//2�ֽ�
	//	event.lossMeasurement=temp;

	//	m_parsePos+=2;//marker Locations��ʼλ��
	//	event.markerLocations=FOURINT(data,m_parsePos);//

	//	m_parsePos+=4;//ע�� ��ʼλ��
	//	event.comment=readString(data,m_parsePos);//

	//	m_parsePos+=event.comment.GetLength()+1;//��һ��¼��ʼλ��

	//	m_keyEvent.vEvent.push_back(event);
	//}//efor


	/////m_parsePos+=2;//�Ѽӹ�End-to-End Loss ��ʼλ��
	//m_keyEvent.eteLoss=FOURINT(data,m_parsePos);//

	//m_parsePos+=4;//End-to-End Marker Positions Start ��ʼλ��
	//m_keyEvent.eteMarkerStart=FOURINT(data,m_parsePos);//

	//m_parsePos+=4;//End-to-End Marker Positions Finish ��ʼλ��
	//m_keyEvent.eteMarkerFinish=FOURINT(data,m_parsePos);//

	//m_parsePos+=4;//���� ��ʼλ��
	//m_keyEvent.eteLoss=TWOINT(data,m_parsePos);//

	//m_parsePos+=2;//�����־��� ��ʼλ��
	//m_keyEvent.RLMarkerStart=FOURINT(data,m_parsePos);//

	//m_parsePos+=4;//�����־������ ��ʼλ��
	//m_keyEvent.RLMarkerFinish=FOURINT(data,m_parsePos);//

	//m_parsePos+=4;//��һ��¼��ʼλ��

	//return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
//����˵��:��ȡLinkParas Block����û����?????????????????????????????????????????????
//��ڲ���:data:BYTE����
//���ڲ���:
//����ʱ��:2011.10.8
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
BOOL CSorFileBase::readLinkParas(BYTE data[])
{
	vchar2string(m_linkParas.ID,string("LnkParams"));
	int blockLen=0;//block����
	int strLen=0;
	blockLen+=10;//�¼�������ʼλ��,����LnkParams\0
	int count=TWOINT(data,blockLen);
	m_linkParas.totalLandmarks=count;//�ر����

	LinkParas::Landmark landmark;
	blockLen+=2;//��һ���¼���ʼλ��,�����ر����
	CString temp;
	m_linkParas.vLandmark.clear();
	for (int i=0;i<count;i++)
	{
		landmark.num=TWOINT(data,blockLen);//���

		blockLen+=2;//�ر������
		//temp.Format(_T("%C%C"),data[blockLen],data[blockLen+1]);
		//landmark.code=temp;//2�ֽ�
		landmark.code.clear();
		landmark.code.push_back(data[blockLen]);
		landmark.code.push_back(data[blockLen+1]);

		blockLen+=2;//location 4�ֽ�
		landmark.location=FOURINT(data,blockLen);//


		blockLen+=4;//related Event Number��Ӧ�¼����
		landmark.relatedEventNum=TWOINT(data,blockLen);//

		blockLen+=2;//GPS Information GPS��Ϣ 4�ֽ�//GPS����
		landmark.GPSLongitude=FOURINT(data,blockLen);//

		blockLen+=4;//GPSγ��
		landmark.GPSLatitude=FOURINT(data,blockLen);//

		blockLen+=4;//Fiber Correction Factor Lead-in Fiber  2�ֽ�
		landmark.fiberCorrFactor = TWOINT(data,blockLen);//

		blockLen+=2;//Sheath Marker Entering LandMark 4�ֽ�
		landmark.sheathEntering=FOURINT(data,blockLen);//

		blockLen+=4;////Sheath Marker Leaving LandMark 4�ֽ� ·�����Ĺ��������ϵ�����
		landmark.sheathLeaving=FOURINT(data,blockLen);//

		blockLen+=4;//���������ϱ�ʾ����ĵ�λ 2�ֽ� feet �� meters
		//temp.Format(_T("%C%C"),data[blockLen],data[blockLen+1]);
		//landmark.SLUnit=temp;//
		landmark.SLUnit.clear();
		landmark.SLUnit.push_back(data[blockLen]);
		landmark.SLUnit.push_back(data[blockLen+1]);

		blockLen+=2;//Mode Field Diameter Leaving LandMark �Ŵ��� 2�ֽ�
		landmark.modeFieldDiameter=TWOINT(data,blockLen);//

		blockLen+=2;//Commentע�� string
		//landmark.comment=readString(data,blockLen);//
		strLen=readString(data + blockLen,landmark.comment);//

		blockLen+=strLen+1;//��һ��¼��ʼλ��
		m_linkParas.vLandmark.push_back(landmark);
	}//efor

	return blockLen;

	//m_parsePos+=10;//�¼�������ʼλ��,����KeyEvents\0
	//int count=TWOINT(data,m_parsePos);
	//m_linkParas.totalLandmarks=count;//�ر����

	//LinkParas::Landmark landmark;
	//m_parsePos+=2;//��һ���¼���ʼλ��,�����ر����
	//CString temp;
	//m_linkParas.vLandmark.clear();
	//for (int i=0;i<count;i++)
	//{
	//	landmark.num=TWOINT(data,m_parsePos);//���

	//	m_parsePos+=2;//�ر������
	//	temp.Format(_T("%C%C"),data[m_parsePos],data[m_parsePos+1]);
	//	landmark.code=temp;//2�ֽ�

	//	m_parsePos+=2;//location 4�ֽ�
	//	landmark.location=FOURINT(data,m_parsePos);//


	//	m_parsePos+=4;//related Event Number��Ӧ�¼����
	//	landmark.relatedEventNum=TWOINT(data,m_parsePos);//

	//	m_parsePos+=2;//GPS Information GPS��Ϣ 4�ֽ�//GPS����
	//	landmark.GPSLongitude=FOURINT(data,m_parsePos);//

	//	m_parsePos+=4;//GPSγ��
	//	landmark.GPSLatitude=FOURINT(data,m_parsePos);//

	//	m_parsePos+=4;//Fiber Correction Factor Lead-in Fiber  2�ֽ�
	//	landmark.fiberCorrFactor = TWOINT(data,m_parsePos);//

	//	m_parsePos+=2;//Sheath Marker Entering LandMark 4�ֽ�
	//	landmark.sheathEntering=FOURINT(data,m_parsePos);//

	//	m_parsePos+=4;////Sheath Marker Leaving LandMark 4�ֽ� ·�����Ĺ��������ϵ�����
	//	landmark.sheathLeaving=FOURINT(data,m_parsePos);//

	//	m_parsePos+=4;//���������ϱ�ʾ����ĵ�λ 2�ֽ� feet �� meters
	//	temp.Format(_T("%C%C"),data[m_parsePos],data[m_parsePos+1]);
	//	landmark.SLUnit=temp;//

	//	m_parsePos+=2;//Mode Field Diameter Leaving LandMark �Ŵ��� 2�ֽ�
	//	landmark.modeFieldDiameter=FOURINT(data,m_parsePos);//

	//	m_parsePos+=2;//Commentע�� string
	//	landmark.comment=readString(data,m_parsePos);//

	//	m_parsePos+=landmark.comment.GetLength()+1;//��һ��¼��ʼλ��
	//	m_linkParas.vLandmark.push_back(landmark);
	//}//efor

	//return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
//����˵��:��ȡDataPts Block������ͨ��7.8
//��ڲ���:data:BYTE����
//���ڲ���:
//����ʱ��:2011.7.8
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
INT CSorFileBase::readDataPts(BYTE data[])
{
	vchar2string(m_dataPoints.ID,string("DataPts"));
	int blockLen=0;//block����
	//int strLen=0;
	blockLen+=8;//�¼�������ʼλ��,����DataPts\0
	int count=FOURINT(data,blockLen);
	m_dataPoints.dataPointNum=count;//

	blockLen+=4;//ʹ�õ���չϵ������ ��ʼλ��
	count=TWOINT(data,blockLen);//
	m_dataPoints.totalScaleFactors=count;

	///int itemp;
	blockLen+=2;//��һ����չϵ���������-�ܸ��� ��ʼλ��
	DataPoints::DataUseScaleFactor dataFactor;
	int len;
	FLOAT factor;
	m_dataPoints.vAllData.clear();
	unsigned short* pUnShort;
	for (int i=0;i<count;i++)
	{
		len=FOURINT(data,blockLen);
		dataFactor.count=len;//ʹ�õ�ǰϵ���������ܸ���

		blockLen += 4;//ϵ�� ��ʼλ��
		dataFactor.scaleFactor=TWOINT(data,blockLen);//1000=1.0
		////factor=dataFactor.scaleFactor/1000.0;//1000==1.0

		blockLen += 2;//�������� ��ʼλ��
		dataFactor.vData.clear();
		//for (int j=0;j<len;j++)//ԭ������������ȡwcq2011.10.21
		//{
		//	////dataFactor.vData.push_back(TWOINT(data,blockLen)*factor);
		//	dataFactor.vData.push_back(TWOINT(data,blockLen));

		//	blockLen+=2;//��һ������ ��ʼλ��
		//}//efor
		pUnShort = (unsigned short*)(data+blockLen);
		dataFactor.vData.assign(pUnShort,pUnShort+len);
		blockLen += len * 2;
		m_dataPoints.vAllData.push_back(dataFactor);
	}//efor

	////blockLen+=2;//��һ��¼��ʼλ��,����ǰ��ѭ���ӹ�

	return blockLen;

	//m_parsePos+=8;//�¼�������ʼλ��,����DataPts\0
	//int count=FOURINT(data,m_parsePos);
	//m_dataPoints.dataPointNum=count;//

	//m_parsePos+=4;//ʹ�õ���չϵ������ ��ʼλ��
	//count=TWOINT(data,m_parsePos);//
	//m_dataPoints.totalScaleFactors=count;

	/////int itemp;
	//m_parsePos+=2;//��һ����չϵ���������-�ܸ��� ��ʼλ��
	//DataPoints::DataUseScaleFactor dataFactor;
	//int len;
	//FLOAT factor;
	//m_dataPoints.vAllData.clear();
	//for (int i=0;i<count;i++)
	//{
	//	len=FOURINT(data,m_parsePos);
	//	dataFactor.count=len;//ʹ�õ�ǰϵ���������ܸ���

	//	m_parsePos+=4;//ϵ�� ��ʼλ��
	//	dataFactor.scaleFactor=TWOINT(data,m_parsePos);//1000=1.0
	//	factor=dataFactor.scaleFactor/1000.0;//1000==1.0

	//	m_parsePos+=2;//�������� ��ʼλ��
	//	dataFactor.vData.clear();
	//	for (int j=0;j<len;j++)
	//	{
	//		dataFactor.vData.push_back(TWOINT(data,m_parsePos)*factor);
	//		
	//		m_parsePos+=2;//��һ������ ��ʼλ��
	//	}//efor
	//	m_dataPoints.vAllData.push_back(dataFactor);
	//}//efor

	//////m_parsePos+=2;//��һ��¼��ʼλ��,����ǰ��ѭ���ӹ�

	//return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
//����˵��:��ȡCksum Block��У���롣�����ʼ��ȡ������ͨ��7.8
//��ڲ���:data:BYTE����,length:�ļ��ֽ���
//���ڲ���:
//����ʱ��:2011.7.8
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
INT CSorFileBase::readCKsum(BYTE data[])
{
	vchar2string(m_checksum.ID,string("Cksum"));
	int blockLen=0;//block����
	blockLen+=6;//У���� ��ʼλ��
	m_checksum.checksum=TWOINT(data,blockLen);//

	return blockLen;

	//m_parsePos=length-8;//�¼�������ʼλ��,CKsum\0��ʼλ��.CKsum�����ļ���������Դ�����ȡ��
	//CString temp;
	//temp=readString(data,m_parsePos);//
	//if("CKsum"!=temp)
	//{
	//	return FALSE;
	//}

	//m_parsePos+=6;//У���� ��ʼλ��
	//m_checksum.checksum=TWOINT(data,m_parsePos);//

	//return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////
//����˵��:��ȡδ֪Block
//��ڲ���:data:BYTE����,nextBlockID:��һ��Block��ID��startPos:��ʼλ�ã�maxLen�ļ���󳤶�
//���ڲ���:Block����
//����ʱ��:2011.10.10
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
////INT CSorFileBase::readUnknownParas(BYTE data[],vector<char> nextBlockID,int startPos,int maxLen)
////{
////	int blockLen=0;//block����
////	int remainLen =maxLen - startPos;//
////	int strLen=nextBlockID.size();
////	//bool bContinue=true;
////	int j;
////	while(blockLen< remainLen)
////	{
////		if (nextBlockID[0] == data[blockLen])//����һ��block ID�ĵ�һ���ַ�һ��
////		{
////			for (j=0;j<strLen;j++)
////			{
////				if (nextBlockID[j] != data[blockLen + j])
////				{
////					break;
////				}
////			}
////			if (j == strLen)////�����ַ���һ����������һ��block
////			{
////				blockLen--;
////				break;
////			}			
////		}
////		blockLen++;
////	}//eofwhile
////
////	//���浱ǰBlock��m_vUnknownParas
////	vector<BYTE> vcParams(data,data+blockLen-1);
////	m_vUnknownParas.push_back(vcParams);
////
////	return blockLen;
////}
////////////////////////////////////////////////////////////////////////////////////
//����˵��://��ȡ�ļ�ʱ������block��С֮���Ƿ�����ļ��ܴ�С
//��ڲ���:fileLen:�ļ��ܴ�С
//���ڲ���:
//����ʱ��:2011.10.11
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
BOOL CSorFileBase::checkSize(int fileLen)
{
	int len=m_mapBlock.mapBlock.Size;
	int count=m_mapBlock.blockCount-1;//�ܹ��ж��ٸ�Block,��ȥMap Block����
	for (int i=0;i<count;i++)
	{
		len += m_mapBlock.vBlockInfo[i].Size;
	}
	if (len != fileLen)
	{
		return FALSE;
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////
//����˵��:��ȡ.sor�ļ�,û���ǳ����ļ�λ�á���1000��Ϊ����ַ�������.ת�������Ƿ���Ż�**MM**
//��ڲ���:data:BYTE����;startPos:��ʼλ��
//���ڲ���:�������ַ���
//����ʱ��:2011.7.8
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
CString CSorFileBase::readString(BYTE data[],int startPos)
{
	CString temp=_T(""),temp2;
	int itemp=0;
	while('\0'!=data[startPos+itemp] && itemp<1000)//��1000��Ϊ����ַ�������
	{
		temp2.Format(_T("%C"),data[startPos+itemp]);
		temp+=temp2;
		itemp++;
	}
	return temp;
}
////////////////////////////////////////////////////////////////////////////////////
//����˵��:
//��ڲ���:vChar:��ŷ��ؽ��,data:���ܼ���ƫ�ơ�vChar:�����ַ���
//���ڲ���:���ٸ��ֽ�
//����ʱ��:2011.10.9
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
int CSorFileBase::readString(BYTE data[],vector<char>& vChar)
{
	int itemp=0;
	vChar.clear();
	while('\0'!=data[itemp] && itemp<MAX_STRING_LEN)//��1000��Ϊ����ַ�������
	{
		vChar.push_back(data[itemp]);
		itemp++;
	}
	return vChar.size();
}
unsigned short CSorFileBase::crc16(byte d[], int len)
{
	byte b = 0;
	unsigned short crc = 0xffff;
	int i, j;
	for(i=0; i<len; i++)
	{
		for(j=0; j<8; j++)
		{
			b = ((d[i]<<j)&0x80) ^ ((crc&0x8000)>>8);
			crc<<=1;
			if(b!=0)crc^=0x1021;
		}
	}
	crc = ~crc;
	return crc;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//��ȡ����////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//��Ҫ�����ڱ����µ��ļ�������Ǽ��أ��򲻻�ı䣬Ҳ�������¼���.???????????????????????????
INT CSorFileBase::MapBlock::getsize()
{
	INT len;
	//len=4 + 6;//Map����
	//len+= 2;//Block����
	//len+= (10 + 6)*4;//GenParams/SupParams/FxdParams/KeyEvents/
	/////len+= 10 + 6;//LnkParams��ʱ����
	//len+= 8 + 6;//DataPts
	/////Special Propritary��ʱ����
	//len+= 6 + 6;//Cksum

	len=102;//��������Block��Ҫ���¼���.???????????????????????????
	return len;
}
//��Ҫ�����ڱ����µ��ļ�������Ǽ��أ��򲻻�ı䣬Ҳ�������¼���
INT CSorFileBase::GeneralParas::getsize()
{
	INT len = 10;//:GenParams����
	len += 16;//��������
	len += cableID.size()+1;
	len += fiberID.size()+1;
	len += OLocation.size()+1;
	len += TLocation.size()+1;
	len += cableCode.size()+1;
	len += operater.size()+1;
	len += comment.size()+1;
	return len;
}
INT CSorFileBase::SuppliersParas::getsize()
{
	INT len = 10;//:SupParams����
	len += name.size()+1;
	len += mainframeID.size()+1;
	len += mainframeSn.size()+1;
	len += moduleID.size()+1;
	len += moduleSn.size()+1;
	len += softVersion.size()+1;
	len += other.size()+1;
	return len;
}
INT CSorFileBase::FixedParas::getsize()
{
	INT len = 10;//:FixedParas����
	//len += 74;//
	len += 72;//
	len += pulseWidthNum * 10;
	return len;
}
INT CSorFileBase::KeyEvent::getsize()
{
	INT len = 10;//KeyEvents����
	len += 24;//
	
	for (int i=0;i<keyEventNum;i++)
	{
		len += 42 + vEvent[i].comment.size()+1;
	}
	return len;
}
INT CSorFileBase::LinkParas::getsize()
{
	INT len = 10;//KeyEvents����
	len += 2;//

	for (int i=0;i<totalLandmarks;i++)
	{
		len += 32 + vLandmark[i].comment.size()+1;
	}
	return len;
}
INT CSorFileBase::DataPoints::getsize()
{
	INT len = 8;//KeyEvents����
	len += 6;//dataPointNum + totalScaleFactors

	//for (int i=0;i<totalScaleFactors;i++)
	for (int i=0;i<vAllData.size();i++)
	{
		len += 6 + vAllData[i].count * 2;
	}
	return len;
}
INT CSorFileBase::Checksum::getsize()
{
	return 8;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//����////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//

void writeShort(vector<BYTE>& vData,short nToWrite)
{
	vData.push_back((unsigned char)nToWrite);
	nToWrite >>= 8;
	vData.push_back((unsigned char)nToWrite);
}
void writeInt(vector<BYTE>& vData,INT nToWrite)
{
	for(int i=0;i<4;i++)
	{
		vData.push_back((unsigned char)nToWrite);
		nToWrite >>= 8;
	}
}
//д���ַ�������'\0'
void writeString(vector<BYTE>& vData,vector<char> vChar)
{
	int len= vChar.size();
	for(int i=0;i<len;i++)
	{
		vData.push_back(vChar[i]);
	}
	vData.push_back('\0');
}
//д��̶������ַ���������'\0'
void writeString(vector<BYTE>& vData,vector<char> vChar,int len)
{
	if (vChar.size()<len)//��ֹ����wcq2011.10.21
	{
		while(vChar.size()<len)
		{
			vChar.push_back('0');
		}
	}
	for(int i=0;i<len;i++)
	{
		vData.push_back(vChar[i]);
	}
}
//��Ҫ�����ڱ����µ��ļ�������Ǽ��أ��򲻻�ı䣬Ҳ�������¼���.???????????????????????????
void CSorFileBase::writeMapBlock(vector<BYTE>& vData)
{
	INT len;
	writeString(vData,m_mapBlock.mapBlock.ID);//4
	writeShort(vData,m_mapBlock.mapBlock.RevisionNo);//2 REVISION_NUM
	writeInt(vData,1);//��ռ��λ�ȻỹҪ�޸�//4
	int nLen=m_mapBlock.vBlockInfo.size();
	writeShort(vData,m_mapBlock.vBlockInfo.size()+1);//����Map����
	len=12;//ǰ�漸���ĺ�
	int nLenTemp=0;
	char* pChar;
	int IDLen=0;
	for(int i=0;i<nLen;i++)//����block��С�п��ܱ仯������Ҫ���¼���
	{
		writeString(vData,m_mapBlock.vBlockInfo[i].ID);//4
		writeShort(vData,m_mapBlock.vBlockInfo[i].RevisionNo);//2

		IDLen=m_mapBlock.vBlockInfo[i].ID.size();
		pChar=&m_mapBlock.vBlockInfo[i].ID[0];
		if (0 == strncmp(pChar,"GenParams",IDLen))//
		{
			nLenTemp = m_generalParas.getsize();
		}
		else if (0 == strncmp(pChar,"SupParams",IDLen))
		{
			nLenTemp = m_suppliersParas.getsize();
		}
		else if (0 == strncmp(pChar,"FxdParams",IDLen))
		{
			nLenTemp = m_fixedParas.getsize();
		}
		else if (0 == strncmp(pChar,"KeyEvents",IDLen))
		{
			nLenTemp = m_keyEvent.getsize();
		}
		else if (0 == strncmp(pChar,"LnkParams",IDLen))
		{
			nLenTemp = m_linkParas.getsize();
		}
		else if (0 == strncmp(pChar,"DataPts",IDLen))
		{
			nLenTemp = m_dataPoints.getsize();
		}
		else if (0 == strncmp(pChar,"Cksum",IDLen) || 0 == strncmp(pChar,"CKsum",IDLen))//��׼ӦΪCksum,���ǵ������õ���CKsum
		{
			nLenTemp = m_checksum.getsize();
		}
		else
		{
			nLenTemp = m_mapBlock.vBlockInfo[i].Size;
		}////eif
		writeInt(vData,nLenTemp);//
		len += 6 + m_mapBlock.vBlockInfo[i].ID.size()+1;
	}//efor
	
	//map block��С���¸�ֵ	
	for(int i=0;i<4;i++)
	{
		vData[6+i] = (unsigned char)len;
		len >>= 8;
	}
}//д��GeneralParas Block
void CSorFileBase::GeneralParas::write(vector<BYTE>& vData)
{
	writeString(vData,ID);//
	writeString(vData,languageCode,2);//
	writeString(vData,cableID);//
	writeString(vData,fiberID);//
	writeShort(vData,fiberType);//
	writeShort(vData,nominalWaveLength);//

	writeString(vData,OLocation);//
	writeString(vData,TLocation);//
	writeString(vData,cableCode);//
	writeString(vData,currentDataFlag,2);//

	writeInt(vData,userOffset);//
	writeInt(vData,uODistance);//

	writeString(vData,operater);//
	writeString(vData,comment);//
}
//д��SuppliersParas Block
void CSorFileBase::SuppliersParas::write(vector<BYTE>& vData)
{
	writeString(vData,ID);//
	writeString(vData,name);//
	writeString(vData,mainframeID);//
	writeString(vData,mainframeSn);//
	writeString(vData,moduleID);//
	writeString(vData,moduleSn);//
	writeString(vData,softVersion);//
	writeString(vData,other);//
}
//д��FxdParams Block
void CSorFileBase::FixedParas::write(vector<BYTE>& vData)
{
	writeString(vData,ID);//
	writeInt(vData,timestamp);//
	writeString(vData,distanceUnit,2);//
	writeShort(vData,actualWavelength);//
	writeInt(vData,acquisitionOffset);//
	writeInt(vData,AODistance);//

	writeShort(vData,pulseWidthNum);//

	//for(int i=0;i<pulseWidthNum;i++)//��Ϊ����� cwcq2012.08.24
	for(int i=0;i<vPulseWidth.size();i++)//wcq2012.08.24
	{
		writeShort(vData,vPulseWidth[i]);//
		writeInt(vData,vDataSpacing[i]);//
		writeInt(vData,vDataPointsEach[i]);//
	}

	writeInt(vData,groupIndex);//
	writeShort(vData,backscatter);//
	writeInt(vData,avgNum);//
	writeShort(vData,avgTime);//

	writeInt(vData,acquisitionRange);//
	writeInt(vData,ARDistance);//
	writeInt(vData,frontPanelOff);//

	writeShort(vData,noiseFloorLevel);//
	writeShort(vData,noiseFloorFactor);//
	writeShort(vData,powerOff);//
	writeShort(vData,lossThreshold);//
	writeShort(vData,reflectanceThreshod);//
	writeShort(vData,eofThreshold);//

	writeString(vData,traceType,2);//

	writeInt(vData,windowCoor1);//
	writeInt(vData,windowCoor2);//
	writeInt(vData,windowCoor3);//
	writeInt(vData,windowCoor4);//
}
//д��KeyEvent Block
void CSorFileBase::KeyEvent::write(vector<BYTE>& vData)
{
	writeString(vData,ID);//
	writeShort(vData,keyEventNum);//
	//for (int i=0;i<keyEventNum;i++)//��Ϊ����� cwcq2012.08.24
	for (int i=0; i<vEvent.size(); i++)//wcq2012.08.24
	{
		writeShort(vData,vEvent[i].eventNum);//
		writeInt(vData,vEvent[i].propagationTime);//
		writeShort(vData,vEvent[i].attenCoefficent);//
		writeShort(vData,vEvent[i].eventLoss);//
		writeInt(vData,vEvent[i].reflectance);//
		writeString(vData,vEvent[i].code,6);//
		writeString(vData,vEvent[i].lossMeasurement,2);//
		for (int j=0;j<5;j++)
		{
			writeInt(vData,vEvent[i].markerLocations[j]);//
		}
		writeString(vData,vEvent[i].comment);//
	}
	writeInt(vData,eteLoss);//
	writeInt(vData,eteMarkerStart);//
	writeInt(vData,eteMarkerFinish);//
	writeShort(vData,returnLoss);//
	writeInt(vData,RLMarkerStart);//
	writeInt(vData,RLMarkerFinish);//
}
//����¼��б�wcq2012.06.11
void CSorFileBase::KeyEvent::clearEvent()
{
	keyEventNum = 0;
	for (int i=0;i<vEvent.size();i++)
	{
		vEvent[i].code.clear();
		vEvent[i].lossMeasurement.clear();
		vEvent[i].comment.clear();
	}
	vEvent.clear();
}
//д��LinkParas 
void CSorFileBase::LinkParas::write(vector<BYTE>& vData)
{
	writeString(vData,ID);//
	writeShort(vData,totalLandmarks);//
	//for (int i=0;i<totalLandmarks;i++)//��Ϊ����� cwcq2012.08.24
	for (int i=0;i<vLandmark.size();i++)//wcq2012.08.24
	{
		writeShort(vData,vLandmark[i].num);//
		writeString(vData,vLandmark[i].code,2);//
		writeInt(vData,vLandmark[i].location);//
		writeShort(vData,vLandmark[i].relatedEventNum);//

		writeInt(vData,vLandmark[i].GPSLongitude);//
		writeInt(vData,vLandmark[i].GPSLatitude);//
		writeShort(vData,vLandmark[i].fiberCorrFactor);//
		writeInt(vData,vLandmark[i].sheathEntering);//
		writeInt(vData,vLandmark[i].sheathLeaving);//

		writeString(vData,vLandmark[i].SLUnit,2);//

		writeShort(vData,vLandmark[i].modeFieldDiameter);//
		writeString(vData,vLandmark[i].comment);//
	}
}
//д��DataPts Block
void CSorFileBase::DataPoints::write(vector<BYTE>& vData)
{
	writeString(vData,ID);//
	writeInt(vData,dataPointNum);//
	writeShort(vData,totalScaleFactors);//
	int num;
	unsigned short sValue;
	BYTE* pB;
	int count = vAllData.size();
	//for (int i=0;i<totalScaleFactors;i++)//��Ϊ����� cwcq2012.08.24
	for (int i=0;i<count;i++)//wcq2012.08.24
	{
		writeInt(vData,vAllData[i].count);//
		writeShort(vData,vAllData[i].scaleFactor);//
		/*num = vAllData[i].count;
		for (int j=0;j<num;j++)
		{
			sValue = vAllData[i].vData[j];
			vData.push_back((unsigned char)sValue);
			sValue >>= 8;
			vData.push_back((unsigned char)sValue);
		}*/
		pB=(BYTE*)&vAllData[i].vData[0];
		vData.insert(vData.end(),pB,pB + vAllData[i].count * 2);
	}
}
//д��Cksum Block
void CSorFileBase::Checksum::write(vector<BYTE>& vData)
{
	writeString(vData,ID);//
	writeShort(vData,checksum);//
}
void CSorFileBase::writeData(vector<BYTE>& vData)
{
	writeMapBlock(vData);//Map
	int nLenTemp=0;
	char* pChar;
	int IDLen=0;
	int nLen=m_mapBlock.vBlockInfo.size();
	int indexUnkown=0;
	for(int i=0;i<nLen;i++)//����block��С�п��ܱ仯������Ҫ���¼���
	{
		IDLen=m_mapBlock.vBlockInfo[i].ID.size();
		pChar=&m_mapBlock.vBlockInfo[i].ID[0];
		if (0 == strncmp(pChar,"GenParams",IDLen))//
		{
			m_generalParas.write(vData);
		}
		else if (0 == strncmp(pChar,"SupParams",IDLen))
		{
			m_suppliersParas.write(vData);
		}
		else if (0 == strncmp(pChar,"FxdParams",IDLen))
		{
			m_fixedParas.write(vData);
		}
		else if (0 == strncmp(pChar,"KeyEvents",IDLen))
		{
			m_keyEvent.write(vData);
		}
		else if (0 == strncmp(pChar,"LnkParams",IDLen))
		{
			m_linkParas.write(vData);
		}
		else if (0 == strncmp(pChar,"DataPts",IDLen))
		{
			m_dataPoints.write(vData);
		}
		else if (0 == strncmp(pChar,"Cksum",IDLen) || 0 == strncmp(pChar,"CKsum",IDLen))//��׼ӦΪCksum,���ǵ������õ���CKsum
		{
			m_checksum.write(vData);
		}
		else//�Զ���Block
		{
			vData.insert(vData.end(),m_vUnknownParas[indexUnkown].begin(),m_vUnknownParas[indexUnkown].end());
			indexUnkown++;
		}////eif
	}//efor
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��д
////////////////////////////////////////////////////////////////////////////////////
//����˵��:��ȡ.sor�ļ�����ȡʱ�����Զ�������ݿ�
//��ڲ���:filePathAndName:�ļ���������·��
//���ڲ���:
//����ʱ��:2011.7.8
//Write By:wcq
////////////////////////////////////////////////////////////////////////////////////
//INT CSorFileBase::readSorFile(CString filePathAndName,TestResultInfo& testResultInfo)
INT CSorFileBase::readSorFile(CString filePathAndName,SORDATA& sorData)
{
	if(""==filePathAndName)//
	{
		//m_errInfo=_T("�ļ���Ϊ��");
		return ERROR_EMPTY_NAME;
	}//eif

	//
	CFile file;
	if(!file.Open(filePathAndName,CFile::modeReadWrite))//��
	{
		//m_errInfo.Format(_T("δ�ܴ��ļ�%s "),filePathAndName);
		return ERROR_FILE_NOT_FOUND;
	}

	//
	int len=file.GetLength();//�ļ�����
	//������ʱ����������ݣ������е�����һ�ζ����ڴ棬�ٶ��Ƿ���,���Ƿ�ռ���ڴ�.???????????????**�����޸�**
	BYTE* fileData=new BYTE[len];
	/////
	file.Read(fileData,len);//��ȡ������Ϣ
	file.Close();//�ر�

	//
	if(parseData(fileData,len))//����2011.12.01
	{
		//setReturnData(testResultInfo);
		convertToReturnData(sorData);
	}
	else
	{
		delete[] fileData;//ɾ����ʱ����
		deleteDataOnError();//�����س���ʱ��ɾ������ wcq2012.08.24
		init();// wcq2012.08.24
		return ERROR_FORMAT;
	}

	delete[] fileData;//ɾ����ʱ����
	return FILE_SUCCESS;
}
//��ȡʱ�ѱ�׼�ļ������ز���ת��Ϊ��ʾ������Ҫ�����ݣ���һЩlong integer ���͵�����ת��Ϊfloat��
//�����溯������

//��ȡ�ڴ�sorFile���ݣ�lzy2014.11.11
INT CSorFileBase::readSorFile(BYTE *pData, int nLength, SORDATA &sorData)
{
	if(parseData(pData,nLength))//����2011.12.01
	{
		//setReturnData(testResultInfo);
		convertToReturnData(sorData);
	}
	else
	{
		deleteDataOnError();//�����س���ʱ��ɾ������ wcq2012.08.24
		init();// wcq2012.08.24
		return ERROR_FORMAT;
	}

	return FILE_SUCCESS;

}
BOOL CSorFileBase::convertToReturnData(SORDATA& sorData)
{
	//���ݵ㴦��
	//sorData.vAllData = m_dataPoints.vAllData;
	int nTemp = m_dataPoints.vAllData.size();
	Data data;
	sorData.vAllData.clear();
	for (int i=0;i< m_dataPoints.vAllData.size(); i++)
	{
		data.nCount = m_dataPoints.vAllData[i].count;
		data.scaleFactor = m_dataPoints.vAllData[i].scaleFactor;
		data.vData.assign(m_dataPoints.vAllData[i].vData.begin(), m_dataPoints.vAllData[i].vData.end());
		sorData.vAllData.push_back(data);
	}//efor
	//

	//���¼���ֻ����ֻʹ��һ����������Σ���������ж����������Σ�����Ҫ�޸� wcq2012.07.23 ??????????????????????????????????????
	//�����������
	float fC=299792458.0;
	float fReflect=m_fixedParas.groupIndex /100000.0;//������
	//float numEachTime=m_fixedParas.vDataSpacing[0]/100000000000000.0;//�����ʱ��//wcq2011.11.25//��Ϊ����� cwcq2012.08.24
	float numEachTime =0;//wcq2012.08.24
	if (m_fixedParas.vDataSpacing.size() > 0)//ԭ��û�м��ж� cwcq2012.08.24
	{
		numEachTime=m_fixedParas.vDataSpacing[0]/100000000000000.0;//�����ʱ��//wcq2011.11.25
	}//eif
	numEachTime = C_LIGHT_SPEED * (numEachTime) / fReflect;//ÿ����� =�����ʱ�� * ���ڹ����ٶ�//wcq2011.11.25
	//sorData.Length = numEachTime * m_fixedParas.vDataPointsEach[0];//ÿ����� * ����//wcq2011.11.25//��Ϊ����� cwcq2012.08.24
	if (m_fixedParas.vDataPointsEach.size() > 0)//ԭ��û�м��ж� cwcq2012.08.24
	{
		sorData.Length = numEachTime * m_fixedParas.vDataPointsEach[0];//ÿ����� * ����////wcq2012.08.24
	}//eif
	//������ ��������
	sorData.Refraction = fReflect;
	sorData.DataSpacing = numEachTime;//
	//����
	sorData.WaveLength = m_fixedParas.actualWavelength/10;//wcq2012.02.07
	//����
	//sorData.PulseWidth = m_fixedParas.vPulseWidth[0];//��Ϊ����� cwcq2012.08.24
	if (m_fixedParas.vPulseWidth.size() > 0)//ԭ��û�м��ж� cwcq2012.08.24
	{
		sorData.PulseWidth = m_fixedParas.vPulseWidth[0];//wcq2012.08.24
	}//eif
	//����Ƶ��
	if (m_fixedParas.vDataSpacing.size() > 0)//ԭ��û�м��ж� cwcq2012.08.24
	{
		sorData.Frequency = 100000000.0 / (m_fixedParas.vDataSpacing[0] * 2);//wcq2012.06.12 cwcq2012.08.24
	}//eif

	//�¼��б� 
	//sorData.vEvent = m_keyEvent.vEvent;
	Event event;
	nTemp = m_keyEvent.vEvent.size();
	sorData.vEvent.clear();
	for (int i = 0;i < nTemp; i++)
	{
		event.propagationTime = m_keyEvent.vEvent[i].propagationTime;
		event.attenCoefficent = m_keyEvent.vEvent[i].attenCoefficent;
		event.eventLoss = m_keyEvent.vEvent[i].eventLoss;
		event.reflectance = m_keyEvent.vEvent[i].reflectance;
		event.code.assign(m_keyEvent.vEvent[i].code.begin(),m_keyEvent.vEvent[i].code.end());
		event.lossMeasurement.assign(m_keyEvent.vEvent[i].lossMeasurement.begin(),m_keyEvent.vEvent[i].lossMeasurement.end());
		event.comment.assign(m_keyEvent.vEvent[i].comment.begin(),m_keyEvent.vEvent[i].comment.end());
		sorData.vEvent.push_back(event);
	}//efor

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//������� wcq2012.10.30
	sorData.BackscatterCoefficient = m_fixedParas.backscatter / 10.0;//ɢ��ϵ�� 800 = 80.0 wcq2012.10.30
	sorData.LossThreshold = m_fixedParas.lossThreshold / 1000.0;//˥������ 200 = 0.200dB wcq2012.10.30
	sorData.ReflectanceThreshold = m_fixedParas.reflectanceThreshod /(- 1000.0);//�������� 40000 = -40.000 dB wcq2012.10.30
	sorData.EndOfFiberThreshold = m_fixedParas.eofThreshold / 1000.0;//����ĩ������ 3000 = 3.000dB wcq2012.10.30
	sorData.Range = m_fixedParas.ARDistance * 1000 / 10;//km --> m wcq2012.10.30 80.544 = 805
	sorData.TestTimestamp = m_fixedParas.timestamp;   //������ʱ�̣�lzy2014.8.8
	sorData.AverageCount = m_fixedParas.avgNum; //ƽ��������lzy2014.8.8
	sorData.GroupIndex = m_fixedParas.groupIndex; //������ 2014.8.13
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//���س������Ƶ� wcq2013.07.08
	std::string stdTemp;
	vchar2string(stdTemp, m_suppliersParas.name);
	CString temp;//
	temp.Format(_T("name=%s"), CString(stdTemp.c_str()));
	sorData.vParams.push_back(temp);

	return TRUE;
}
//�����س���ʱ��ɾ������,��Ҫ��������ݵ� wcq2012.08.24
void CSorFileBase::deleteDataOnError()
{
	if (m_dataPoints.vAllData.size() > 0)
	{
		for (int i=0; i<m_dataPoints.vAllData.size(); i++)
		{
			m_dataPoints.vAllData[i].vData.clear();
		}//eif
	}//eif
}


//����sor�ļ�
//INT CSorFileBase::saveSorFile(CString filePathAndName,TestResultInfo testResultInfo)//cwcq2012.07.23
INT CSorFileBase::saveSorFile(CString filePathAndName,SORDATA sorData)//wcq2012.07.23
{
	if(""==filePathAndName)//
	{
		//m_errInfo=_T("�ļ���Ϊ��");
		return ERROR_EMPTY_NAME;
	}//eif

	//
	CFile file;
	if(!file.Open(filePathAndName,CFile::modeWrite | CFile::modeCreate))//�����ļ�������Ѵ��ڣ���ɾ��
	{
		//m_errInfo.Format(_T("δ�ܴ����ļ�%s "),filePathAndName);
		return ERROR_CREATE_FILE;
	}

	//
	if (0==m_mapBlock.vBlockInfo.size())//δ��ʼ������û�о��������ļ��ٱ��棬����ֱ�ӱ����������
	{
		init();
	}
	//dealTestInfo(testResultInfo);//���������Ĳ���תΪ��Ӧ�����Ա
	convertDataToSor(sorData);//���������Ĳ���תΪ��Ӧ�����Ա
	
	vector<BYTE> vData;
	writeData(vData);//��������Ա����д��vData����

	//
	file.Write(&vData[0],vData.size());//д��
	file.Close();//�ر�

	return TRUE;
}
////��������ʱ�����������Ĳ���תΪ���Ա//wcq2012.07.23
void CSorFileBase::convertDataToSor(SORDATA sorData)
{
	int nTemp=0;
	DataPoints::DataUseScaleFactor dus;
	m_dataPoints.vAllData.clear();// = sorData.vAllData;
	for (int i=0;i< sorData.vAllData.size(); i++)
	{
		dus.count = sorData.vAllData[i].nCount;
		dus.scaleFactor = sorData.vAllData[i].scaleFactor;
		dus.vData.assign(sorData.vAllData[i].vData.begin(),sorData.vAllData[i].vData.end());
		m_dataPoints.vAllData.push_back(dus);
		nTemp += sorData.vAllData[i].nCount;
	}//efor
	m_dataPoints.dataPointNum = nTemp;//���ݵ����
	
	//������
	m_fixedParas.groupIndex = sorData.Refraction * 100000;

	CTime ct;//(st);
	time_t lt= ct.GetCurrentTime().GetTime(); 
	m_fixedParas.timestamp = lt;//wcq2012.02.10
	m_fixedParas.actualWavelength = sorData.WaveLength * 10;//wcq2012.02.07
	m_generalParas.nominalWaveLength = sorData.WaveLength;//wcq2012.02.10
	
	//����ֻ����������ж�����᲻׼ȷ?????????????????????????????????????????????????????????????
	if (m_fixedParas.vDataSpacing.size() > 0)//ԭ��û�м��ж� cwcq2012.08.24
	{
		m_fixedParas.vDataSpacing[0] = 100000000/(2*sorData.Frequency);//wcq2012.06.12
	}//eif
	if (m_fixedParas.vPulseWidth.size() > 0)//ԭ��û�м��ж� cwcq2012.08.24
	{
		m_fixedParas.vPulseWidth[0] = sorData.PulseWidth;
	}//eif
	if (m_fixedParas.vDataPointsEach.size() > 0)//ԭ��û�м��ж� cwcq2012.08.24
	{
		m_fixedParas.vDataPointsEach[0]= nTemp;
	}//eif

	//�¼��б�wcq2012.06.11
	m_keyEvent.keyEventNum = sorData.vEvent.size();

	KeyEvent::Event event;
	nTemp = sorData.vEvent.size();
	m_keyEvent.vEvent.clear();
	for (int i=0; i< nTemp;i++)
	{
		event.eventNum = i;
		event.propagationTime = sorData.vEvent[i].propagationTime;
		event.attenCoefficent = sorData.vEvent[i].attenCoefficent;
		event.eventLoss = sorData.vEvent[i].eventLoss;
		event.reflectance = sorData.vEvent[i].reflectance;
		event.code.assign(sorData.vEvent[i].code.begin(),sorData.vEvent[i].code.end());
		event.lossMeasurement.assign(sorData.vEvent[i].lossMeasurement.begin(),sorData.vEvent[i].lossMeasurement.end());
		event.comment.assign(sorData.vEvent[i].comment.begin(),sorData.vEvent[i].comment.end());
		for(int j=0;j<5;j++)
		{
			event.markerLocations[j]  = 0;
		}//efor
		m_keyEvent.vEvent.push_back(event);
	}//efor

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//������� wcq2012.10.30
	m_fixedParas.backscatter = sorData.BackscatterCoefficient * 10;
	m_fixedParas.lossThreshold = sorData.LossThreshold * 1000;
	m_fixedParas.reflectanceThreshod = sorData.ReflectanceThreshold * (-1000);
	m_fixedParas.eofThreshold = sorData.EndOfFiberThreshold * 1000;
	m_fixedParas.acquisitionRange = 0;//��acquisitionRange distance����ʾ����,����ʹ��acquisitionRange distance
	m_fixedParas.ARDistance = (sorData.Range + 50) * 10 / 1000;//���ǵ��ļ���ʹ��km,����ת���� 80.544 = 805
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}


//void CSorFileBase::dealTestInfo(TestResultInfo testResultInfo)//cwcq2012.07.23
//{
//	m_dataPoints.dataPointNum = testResultInfo.DataPointsNum;//���ݵ����
//	//�����������
//	for (int i=0;i<m_dataPoints.totalScaleFactors;i++)
//	{
//		m_dataPoints.vAllData[i].vData.clear();
//	}
//	//"����"����
//	m_dataPoints.vAllData.clear();
//	m_dataPoints.totalScaleFactors = 1;
//	DataPoints::DataUseScaleFactor dus;
//	dus.count = testResultInfo.DataPointsNum;
//	dus.scaleFactor =testResultInfo.ScaleFactor;//����ϵ��Ϊ1
//	dus.vData.assign(testResultInfo.pDataPoints,testResultInfo.pDataPoints + testResultInfo.DataPointsNum);
//	//dus.vData.resize(testResultInfo.DataPointsNum);
//	//memcpy(&(dus.vData[0]),testResultInfo.pDataPoints,testResultInfo.DataPointsNum);
//	m_dataPoints.vAllData.push_back(dus);
//
//	//
//	//short fRefraction = testResultInfo.Refraction * 100000;
//	m_fixedParas.groupIndex = testResultInfo.Refraction * 100000;
//	//for (int i=0;i<m_dataPoints.vAllData.size();i++)
//	//{
//	//	m_dataPoints.vAllData[i].scaleFactor = fRefraction;
//	//}
//	//m_fixedParas.actualWavelength = testResultInfo.WaveLength;
//	CTime ct;//(st);
//	time_t lt= ct.GetCurrentTime().GetTime(); 
//	m_fixedParas.timestamp = lt;//wcq2012.02.10
//	m_fixedParas.actualWavelength = testResultInfo.WaveLength * 10;//wcq2012.02.07
//	m_generalParas.nominalWaveLength = testResultInfo.WaveLength;//wcq2012.02.10
//	//m_fixedParas.vDataSpacing[0] = testResultInfo.DataSpacing;//wcq2011.12.01testResultInfo.DataSpacing�������mΪ��λ
//	//m_fixedParas.vDataSpacing[0] =testResultInfo.DataSpacing * (m_fixedParas.groupIndex /C_LIGHT_SPEED )* 100000000000000.0;//wcq2011.12.01
//	//m_fixedParas.vDataSpacing[0] =testResultInfo.DataSpacing * testResultInfo.Refraction * ( 100000000000000.0/C_LIGHT_SPEED );//wcq2011.12.01
//	m_fixedParas.vDataSpacing[0] = 100000000/(2*testResultInfo.Frequency);//wcq2012.06.12
//	m_fixedParas.vPulseWidth[0] = testResultInfo.PulseWidth;
//	m_fixedParas.vDataPointsEach[0]= testResultInfo.DataPointsNum;
//
//	//�¼��б�wcq2012.06.11
//	//���
//	m_keyEvent.clearEvent();
//	m_keyEvent.keyEventNum = testResultInfo.EventList.vEventInfo.size();//�¼�����
//	string pos="9999";
//	//float fPTimeCoeff = testResultInfo.Refraction * pow(10.0,-10) /C_LIGHT_SPEED
//	//int eventType = 0;//wcq2012.07.07
//	for(int i=0; i<m_keyEvent.keyEventNum; i++)
//	{
//		KeyEvent::Event event;		
//		event.eventNum = i;////�¼����
//		event.propagationTime = (testResultInfo.EventList.vEventInfo[i].index /(2 *testResultInfo.Frequency))*10000;//����/����(km/ps) * ������ λ��
//		//event.propagationTime = (testResultInfo.EventList.vEventInfo[i].fLocation * pow(10.0,-10)/C_LIGHT_SPEED ) * testResultInfo.Refraction;//����/����(km/ps) * ������ λ��
//		event.attenCoefficent = testResultInfo.EventList.vEventInfo[i].fLineK * 1000;//б��/˥������ dB/km 350=0.35
//		event.eventLoss = testResultInfo.EventList.vEventInfo[i].fEventloss * 1000;////���
//		event.reflectance = testResultInfo.EventList.vEventInfo[i].fEventHigh * 1000;//����
//		//event.code //�¼�����????????????
//		//event.code.push_back(testResultInfo.EventList.vEventInfo[i].nEventType + '0');//���ǵ�ֻ�зǷ���ͷ����¼�,���͵�û��//��Ϊ�����
//		////if (0 == testResultInfo.EventList.vEventInfo[i].nEventType)//cwcq2012.07.09
//		////{
//		////	event.code.push_back('0');
//		////}
//		////else//1������
//		////{
//		////	event.code.push_back('1');
//		////}//eif
//		event.code.push_back(testResultInfo.EventList.vEventInfo[i].nEventType + '0');//wcq2012.07.09
//		event.code.push_back(testResultInfo.EventList.vEventInfo[i].cEventType);//
//		event.code.insert(event.code.end(), pos.begin(), pos.end());//��ʹ�� landmark
//		vchar2string(event.lossMeasurement,string("LS"));//���㷨 �����õ���4�㷨
//		///event.markerLocations//�㷨ÿ��λ�ã�����
//		///event.comment.push_back('\0');//ע�ͣ�û�в�����ӣ���ӻ����wcq2012.06.12
//		m_keyEvent.vEvent.push_back(event);
//	}//efor
//}
//
