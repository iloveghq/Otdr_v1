#include <vector>
#include "EndGabor.h"
struct EventInfo //�¼�����Ϣ�ṹ��
{
	unsigned int index;//λ����������ĵڼ�����
	unsigned int nEventType;//�¼����ͣ�0�����¼���1�Ƿ����¼�
	float fLocation;//�¼�λ��,(C/(2nf))*index,���ݵڼ������õ�����//2011.11.24
	float fEventloss;//���//2011.11.24
	float fEventHigh;////�¼��㷴��dB//2011.11.24
	float fLineK;////����б��//2011.11.24
	float fTotalLoss;////����ġ��ۼ����//2011.11.24
	float fReflect;//����ֵ
	float fTrailEnd;////β��λ��
	char  cEventType;// event code �ڶ�������  S:��ʼ�¼� E:����ĩ�˼������¼���A���û��Լӣ� F:������в��ң�O:����������Χ�Ĺ���ĩ�� D: wcq2012.06.11
};

struct EventInfoList//�¼�����Ϣ�б�
{
	std::vector<EventInfo> vEventInfo; //��������յ��¼�����
};
//struct DataScales//wcq20111101
//{
//	short scaleFactor;//����2�ֽ�
//	std::vector<unsigned short> vData;//
//};
//std::vector<DataScales> vAllData;//��������
struct TestResultInfo 
{
	TestResultInfo()//wcq 2011.11.1
	{
		IsMaxLength = FALSE;
		IsLowerStandard = FALSE;
		IsLoadFromFile = TRUE;
		DataType = 8888; //�������ͣ� ������8888��������4444
		TestMode = 2222; //����ģʽ��ƽ������2222��
		DataSerialNum = 0; //���ݰ����
		pDataPoints = NULL; //������ֵ ������1000����ֵ
		pDataCurve = NULL; //����ȡ�������������
		ScaleFactor = 1;//����ϵ����ÿ�����ݵ㶼�˸�ϵ��
		YOffset = 0;//��ֱ����ƫ�ƣ�����ƽ��
		StartPoint = 0;  //���ߺ���������
		MaxPoint = 0;    //���ߺ�������������ֵ
		FullPointCount = 0;//��������ʱΪ���ٸ���ʱ����ռ��������ͼ����,����ȷ�����߿��wcq2011.09.16
		FullPointCountMin = 0;//����δ����,Ϊ���ٸ���ʱ����ռ��������ͼ����,����ȷ�����߿��,FullPointCountΪ�����ı�����wcq2011.09.21
		LastZoomPointCount = 0;//��������ʱ���һ�����Ź����ٵ㣬��ǰ������ŵĵ�ĸ�����һ��һ����wcq2011.09.21
		DataSpacing = 0;//for each pulse width used:one-way time(units of 100 psec) for 10000 data points//wcq2011.10.19���Ϊ��������
		Refraction = 0.0f; //������
		ChainLoss = 0.0f; //����ģ�������1000��
		ChainAttenu = 0.0f; //��˥����������1000��
		MaxTestDistance = 0.0f; //��������������
		DataPointsNum = 0; //���������
		Frequency = 0; //Ƶ��
		Range = 0; //������Χ
		PulseWidth = 0; //������
		WaveLength = 0; //���Ĳ���
		MeasureTime = 0; //����ʱ��
		ChainLength = 0; //����
		ptDrawData = NULL; //��ͼ�����,ÿ�����Ӧһ����ͼ����ָ��,ԭ������ΪCOTDRPage����m_ptRecord wcq2011.09.21

		//
		Atten = 0.0f;//˥��
		Average = 0.0f;//ƽ��
		SampleCount = 0;//������
		FusionLoss = 0.0f;//�۽����
		ReturnLoss = 0.0f;//�ز����

		//��������
		Relay = 0;//�̵���//wcq2011.12.06
		Joint1 = 0;//ƴ�ӵ�1//wcq2011.12.05
		Joint2 = 0;//ƴ�ӵ�2//wcq2011.12.05
		Length = 0; //���߳���(���˳���)
		ThumbDrawXCount = 0;//����ͼ����ʱx����ֵ���ֵ�������Ϊ����ͼ������
		dataCurveCount = 0;//pDataCurve�����,��һ��ΪCOUNT_POINT_MAX_MIN��wcq2011.11.23
		curveColor = RGB(0,0,255); //������ɫ
		pThumbData = NULL;//����ͼ����
//��ʾʱ��ƽ���������ݣ�����ʱ��ʵ������ wcq2013.04.26
		pDataPointsSmooth = NULL;

	}
	~TestResultInfo()//wcq 2011.11.1
	{
		if (NULL != pDataPoints)
		{
			delete[] pDataPoints;
			pDataPoints = NULL;
		}
		if (NULL != pDataCurve)
		{
			delete[] pDataCurve;
			pDataCurve = NULL;
		}
		if (NULL != ptDrawData)
		{
			//delete[] ptDrawData;
			ptDrawData = NULL;
		}
		if (NULL != pThumbData)
		{
			delete[] pThumbData;
			pThumbData = NULL;
		}
		EventList.vEventInfo.clear();
//��ʾʱ��ƽ���������ݣ�����ʱ��ʵ������ wcq2013.04.26
		//m_TestResult.pDataPointsSmooth=NULL;
		if (NULL != pDataPointsSmooth)
		{
			delete[] pDataPointsSmooth;
			pDataPointsSmooth=NULL;
		}

	}
	BOOL IsMaxLength; //�Ƿ��������
	BOOL IsLowerStandard;//wcq2011.09.26������ZOOM_VERTICAL_STANDARD�Ƚ϶�Ϊtrue��false.������ֱ��������ʱȷ���������
	BOOL IsLoadFromFile;//�Ƿ���ļ���������������//wcq2011.10.19
	WORD DataType; //�������ͣ� ������8888��������4444
	WORD TestMode; //����ģʽ��ƽ������2222��
	WORD DataSerialNum; //���ݰ����
	WORD* pDataPoints; //������ֵ ������1000����ֵ
	CPoint* pDataCurve; //����ȡ�������������
	short ScaleFactor;//����ϵ����ÿ�����ݵ㶼�˸�ϵ��
	int YOffset;//��ֱ����ƫ�ƣ�����ƽ��
	int StartPoint;  //���ߺ���������
	int MaxPoint;    //���ߺ�������������ֵ
	int FullPointCount;//��������ʱΪ���ٸ���ʱ����ռ��������ͼ����,����ȷ�����߿��wcq2011.09.16
	int FullPointCountMin;//����δ����,Ϊ���ٸ���ʱ����ռ��������ͼ����,����ȷ�����߿��,FullPointCountΪ�����ı�����wcq2011.09.21
	int LastZoomPointCount;//��������ʱ���һ�����Ź����ٵ㣬��ǰ������ŵĵ�ĸ�����һ��һ����wcq2011.09.21
	float DataSpacing;//for each pulse width used:one-way time(units of 100 psec) for 10000 data points//wcq2011.10.19���Ϊ��������
	float Refraction; //������1310nm
	float Refraction1550; //������1550nm//zll 2012.9.18
	float GroupIndex;   //������lzy 2014.8.13
	float ChainLoss; //����ģ�������1000��
	float ChainAttenu; //��˥����������1000��
	float MaxTestDistance; //��������������
	unsigned long int DataPointsNum; //���������
	float Frequency; //Ƶ��
	long int Range; //������Χ
	long int PulseWidth; //������
	long int WaveLength; //���Ĳ���
	long int MeasureTime; //����ʱ��
	long int ChainLength; //����
	CPoint* ptDrawData; //��ͼ�����,ÿ�����Ӧһ����ͼ����ָ��,ԭ������ΪCOTDRPage����m_ptRecord wcq2011.09.21
	EventInfoList EventList;//�¼��б�
//��ʾʱ��ƽ���������ݣ�����ʱ��ʵ������
	WORD* pDataPointsSmooth; //������ֵ ������1000����ֵ,ԭʼ���ݾ���ƽ������������


	//
	float Atten;//˥��
	float Average;//ƽ��
	int   SampleCount;//������
	float FusionLoss;//�۽����
	float ReturnLoss;//�ز����

	//��������
	WORD Relay;//�̵���//wcq2011.12.06
	WORD Joint1;//ƴ�ӵ�1//wcq2011.12.05
	WORD Joint2;//ƴ�ӵ�2//wcq2011.12.05
	float Length; //���߳���(���˳���)
	unsigned int ThumbDrawXCount;//����ͼ����ʱx����ֵ���ֵ�������Ϊ����ͼ������
	int dataCurveCount;//pDataCurve�����,��һ��ΪCOUNT_POINT_MAX_MIN��wcq2011.11.23
	COLORREF curveColor; //������ɫ
	CPoint*  pThumbData;//����ͼ����

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//������� wcq2012.10.30
	float BackscatterCoefficient;//ɢ��ϵ�� wcq2012.10.30
	float LossThreshold;//˥������ wcq2012.10.30
	float ReflectanceThreshold;//�������� wcq2012.10.30
	float EndOfFiberThreshold;//����ĩ������ wcq2012.10.30
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	Gabor_Result m_GaborResult;//�¼��б�
};
