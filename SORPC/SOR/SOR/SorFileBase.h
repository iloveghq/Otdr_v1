//������Ҫ������дsor�ļ�
//MM:May Modify��д,�����޸�
//Paras:Parameters��д
//Off:offset��д
#pragma once
#include <map>
#include <vector>
#include "SorFile.h"
using namespace std;
#define FOURINT(DATA,IT) ((DATA[IT+3]<<(8*3))+(DATA[IT+2]<<(8*2))+(DATA[IT+1]<<(8))+DATA[IT])
#define TWOINT(DATA,IT) ((DATA[IT+1]<<(8))+DATA[IT])//DATA:������������,IT����ʼλ��
#define SIZEOFWCHAR_T 2//wcq2011.10.8
#define MAX_STRING_LEN 1000//��ȡBlock���ַ���ʱ���Ը�ֵ��Ϊ��󳤶ȣ������ó�����Ϊ����2011.10.10
#define REVISION_NUM 200//����block���޶��汾��
#define C_LIGHT_SPEED 299792458.0//����//wcq2011.10.19
//#define B_INCLUDE_LNKPARAMS 1//����sor�ļ�ʱ�Ƿ����LnkParams
class CSorFileBase
{
public:
	CSorFileBase(void);
	~CSorFileBase(void);
public:
	struct MapBlock
	{
		INT getsize();//��ȡ����
		struct blockInfo
		{
			//CString ID;//����
			vector<char> ID;//����
			short RevisionNo;//�޶��汾2
			INT Size;//��С4
		};
		blockInfo mapBlock;//Map ���������Ϣ
		short blockCount;//block����
		vector<blockInfo> vBlockInfo;//block�������
	}m_mapBlock;
	struct GeneralParas//
	{
		void write(vector<BYTE>& vData);
		INT getsize();//��ȡ����
		//CString ID;//����
		vector<char> ID;//����
		//CString languageCode;//���Ա���
		//CString cableID;//���±��
		//CString fiberID;//���˱��
		//short fiberType;//��������
		//short nominalWaveLength;//�����????�磺1310
		//CString OLocation;//Originating Location��ʼλ��?????????
		//CString TLocation;//Terminating Location����λ��?????????
		//CString cableCode;//���±���?????
		//CString currentDataFlag;//����
		vector<char> languageCode;//���Ա��� �̶�2�ֽ�
		vector<char> cableID;//���±��
		vector<char> fiberID;//���˱��
		short fiberType;//��������
		short nominalWaveLength;//�����????�磺1310
		vector<char> OLocation;//Originating Location��ʼλ��?????????
		vector<char> TLocation;//Terminating Location����λ��?????????
		vector<char> cableCode;//���±���?????
		vector<char> currentDataFlag;//�����̶�2�ֽ�
		INT userOffset;//???
		INT uODistance;//user Offset Distance
		//CString operater;//operator������Ա
		vector<char> operater;//����
		//CString comment;//ע��
		vector<char> comment;//ע��
	}m_generalParas;
	struct SuppliersParas//
	{
		INT getsize();//��ȡ����
		void write(vector<BYTE>& vData);
		//CString ID;//����
		vector<char> ID;//����
		//CString name;//��������
		//CString mainframeID;//������ͺ�
		//CString mainframeSn;//��������к�
		//CString moduleID;//���ģ���
		//CString moduleSn;//�����plug-in�����к�
		//CString softVersion;//��Ӳ���汾��
		//CString other;//������Ϣ
		vector<char> name;//��������
		vector<char> mainframeID;//������ͺ�
		vector<char> mainframeSn;//��������к�
		vector<char> moduleID;//���ģ���
		vector<char> moduleSn;//�����plug-in�����к�
		vector<char> softVersion;//��Ӳ���汾��
		vector<char> other;//������Ϣ
	}m_suppliersParas;
	struct FixedParas//
	{
		INT getsize();//��ȡ����
		void write(vector<BYTE>& vData);
		//CString ID;//����
		vector<char> ID;//����
		UINT timestamp;//ʱ��
		//CString distanceUnit;//��λ km,mt,ft,kf,mi �̶�2�ֽ�
		vector<char> distanceUnit;//��λ km,mt,ft,kf,mi �̶�2�ֽ�
		short actualWavelength;//ʵ�ʲ�������
		INT acquisitionOffset;//???
		INT AODistance;//acquisition Offset Distance
		short pulseWidthNum;//Total Number of Pulse Widths Used�������
		vector<short> vPulseWidth;//Pulse Widths Used ʹ�õ�����
		vector<INT> vDataSpacing;//��ͬ����10000�����ݵ�ʱ��
		vector<INT> vDataPointsEach;//ÿ���������ݵ����,������wcq
		INT groupIndex;//�Լ����Եõ���������wcq2011.10.18
		short backscatter;
		INT avgNum;//average number
		unsigned short avgTime;//average time
		INT acquisitionRange;
		INT ARDistance;//acquisitionRange distance
		INT frontPanelOff;
		unsigned short noiseFloorLevel;
		short noiseFloorFactor;//��������ϵ��
		unsigned short powerOff;//power offset first point
		unsigned short lossThreshold;
		unsigned short reflectanceThreshod;
		unsigned short eofThreshold;//end-of-fiber threshold
		//CString traceType;
		vector<char> traceType;//��׼��˵��string��ʵ������string �̶�2�ֽڣ�����û\0
		INT windowCoor1;//window coordinates������������
		INT windowCoor2;//window coordinates
		INT windowCoor3;//window coordinates
		INT windowCoor4;//window coordinates
	}m_fixedParas;
	struct KeyEvent//
	{
		INT getsize();//��ȡ����
		void write(vector<BYTE>& vData);
		void clearEvent();//����¼��б�wcq2012.06.11
		//CString ID;//����
		vector<char> ID;//����
		short keyEventNum;//�¼����� 
		struct Event
		{
			short eventNum;//�¼����
			INT propagationTime;//���Ӻ��һ��e�¼���ʱ��
			short attenCoefficent;//˥��ϵ��
			short eventLoss;//�¼����
			INT   reflectance;//�¼�����
			//CString code;//�¼���־
			//CString lossMeasurement;
			vector<char> code;//�¼���־�̶�6�ֽ�6
			vector<char> lossMeasurement;//�̶�2�ֽ�2
			INT markerLocations[5];//2011.10.11
			//	CString comment;//ע��
			vector<char> comment;//ע��
		};
		vector<Event> vEvent;//�����¼�
		INT eteLoss;//End-to-End Loss
		INT eteMarkerStart;
		INT eteMarkerFinish;
		unsigned short returnLoss;//����
		INT RLMarkerStart;
		INT RLMarkerFinish;
	}m_keyEvent;
	struct LinkParas//
	{
		INT getsize();//��ȡ����
		void write(vector<BYTE>& vData);
		//CString ID;//����
		vector<char> ID;//����
		short totalLandmarks;//lankmarks��־����,Ĭ�Ϻ��¼��б����һ��
		struct Landmark
		{
			short num;//���
			//CString code;//����2�ֽ�
			vector<char> code;//����2�ֽڹ̶�2�ֽ�
			INT location;//
			short relatedEventNum;//��Ӧ���¼����
			INT GPSLongitude;//GPS����
			INT GPSLatitude;//GPSγ��
			short fiberCorrFactor;//
			INT   sheathEntering;
			INT   sheathLeaving;
			//	CString SLUnit;//Units of Sheath Marker Leaving Landmark
			vector<char> SLUnit;//Units of Sheath Marker Leaving Landmark 2�ֽڹ̶�2�ֽ�
			short modeFieldDiameter;
			//CString comment;
			vector<char> comment;
		};
		vector<Landmark> vLandmark;//�����¼�
	}m_linkParas;
	struct DataPoints//
	{
		INT getsize();//��ȡ����
		void write(vector<BYTE>& vData);
		//CString ID;//����
		vector<char> ID;//����
		INT dataPointNum;//���ݵ����
		short totalScaleFactors;//ʹ�õ���չϵ������
		struct DataUseScaleFactor
		{
			INT count;//���
			short scaleFactor;//����2�ֽ�
			vector<unsigned short> vData;//
		};
		vector<DataUseScaleFactor> vAllData;//��������
	}m_dataPoints;
	struct Checksum//
	{
		INT getsize();//��ȡ����
		void write(vector<BYTE>& vData);
		//CString ID;//����
		vector<char> ID;//����
		unsigned short checksum;//У����
	}m_checksum;
	
	template<typename To,typename From>
	void vchar2string(To& t,From& f)
	{
		t.assign(f.begin(),f.end());
	}


public:
	vector< vector<BYTE> > m_vUnknownParas;//��������OTDR�豸�����Զ����Block.wcq2011.10.11
	CString m_errInfo;//������ʾ
	//INT m_parsePos;//����ʱ�����ֽ�λ��,��һ��¼��ʼλ��

public:
	//
	void writeMapBlock(vector<BYTE>& vData);
	void writeData(vector<BYTE>& vData);
	void init();//��ʼ��һЩ��������һЩ������Ĭ��ֵ���ղ���������ݱ���ʱ��Ҫ��ʼ��
	BOOL checkSize(int fileLen);//��ȡ�ļ�ʱ������block��С֮���Ƿ�����ļ��ܴ�С
	BOOL parseData(BYTE data[],int len);//���ļ����������block����������漸��read����
	//////��д
	unsigned short crc16(byte d[], int len);//CRC-CCITT 2011.10.8
	/////*INT readSorFile(CString filePathAndName,TestResultInfo& testResultInfo);//cwcq2012.07.23//���м����漸�ж��ŵ����� cwcq2012.10.30
	////BOOL setReturnData(TestResultInfo& testResultInfo);//������������ݸ�������Ĳ���
	////INT saveSorFile(CString filePathAndName,TestResultInfo testResultInfo);*///cwcq2012.07.23
	//////void dealTestInfo(TestResultInfo testResultInfo);//��������ʱ�����������Ĳ���תΪ���Ա//cwcq2012.07.23
	////INT readSorFile(CString filePathAndName,SORDATA& sorData);//wcq2012.07.23
	////BOOL setReturnData(SORDATA& sorData);//������������ݸ�������Ĳ���//wcq2012.07.23
	////void deleteDataOnError();//�����س���ʱ��ɾ������ wcq2012.08.24
	////INT saveSorFile(CString filePathAndName,SORDATA sorData);//wcq2012.07.23
	////void convertDataToSor(SORDATA sorData);//��������ʱ�����������Ĳ���תΪ���Ա//wcq2012.07.23
	//
	int readString(BYTE data[],vector<char>& vChar);
	//��ȡ��׼MapBlock
	INT readMap(BYTE data[]);//��ȡMapBlock
	INT readGeneralParas(BYTE data[]);//��ȡGeneralParas Block
	INT readSuppliersParas(BYTE data[]);//��ȡSuppliersParas Block
	INT readFixedParas(BYTE data[]);//��ȡFxdParams Block
	INT readKeyEvents(BYTE data[]);//��ȡKeyEvent Block
	INT readLinkParas(BYTE data[]);//��ȡLinkParas 
	INT readDataPts(BYTE data[]);//��ȡDataPts Block
	INT readCKsum(BYTE data[]);//��ȡCKsum Block
	//��ȡδ֪���Ǳ�׼���Զ���ģ� Block
	//INT readUnknownParas(BYTE data[],vector<char> nextBlockID,int startPos,int maxLen);//
	//�����Ҫ�Զ���block�����ڴ����

	CString readString(BYTE data[],int startPos);


////////////////////////////////////////////////////////////////////////////////////////////	
////////////////////////////////////////////////////////////////////////////////////////////	
////////////////////////////////////////////////////////////////////////////////////////////	
	//��ȡ wcq2012.10.30
	INT readSorFile(CString filePathAndName,SORDATA& sorData);//wcq2012.07.23
	BOOL convertToReturnData(SORDATA& sorData);//������������ݸ�������Ĳ���//wcq2012.07.23//ԭΪsetReturnData cwcq2012.10.30
	void deleteDataOnError();//�����س���ʱ��ɾ������ wcq2012.08.24

	//����
	INT saveSorFile(CString filePathAndName,SORDATA sorData);//wcq2012.07.23
	void convertDataToSor(SORDATA sorData);//��������ʱ�����������Ĳ���תΪ���Ա//wcq2012.07.23
};
