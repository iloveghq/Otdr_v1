#include <vector>
//���Խ���ṹ��
struct EventInfo //�¼�����Ϣ�ṹ��
{
	unsigned int index;//λ����������ĵڼ�����
	unsigned int nEventType;//�¼�����0,��ʼ�¼���1�����¼���2�Ƿ����¼���3�����¼���4�����¼�
	float fLocation;//�¼�λ��,(C/(2nf))*index,���ݵڼ������õ�����//2011.11.24//gabor_dll�ṹ����û����ֶ� cwcq2012.06.11
	float fEventloss;//���//2011.11.24
	float fEventHigh;////�¼��㷴��dB//2011.11.24 ���� wcq2012.06.11 
	float fLineK;////����б��//2011.11.24
	float fTotalLoss;////����ġ��ۼ����//2011.11.24
	float fReflect;//û���õ�
	float fTrailEnd;////β��λ�� 
	char  cEventType;//sor��׼ event code �ڶ������� �¼������ɵĻ����û���ӻ���β���¼���   A���û��Լӣ�M: E:����ĩ�ˣ�F:������в��ң�O:����������Χ�Ĺ���ĩ�� D: wcq2012.06.11
};
struct EventInfoList //�¼�����Ϣ�б�
{
	std::vector<EventInfo> vEventInfo; //
	//unsigned int EventNum; //
};
struct TestResultInfo 
{
	TestResultInfo()//wcq 2011.11.1
	{
		pDataPoints = NULL;
		pDataCurve = NULL;
		ptDrawData = NULL;
		pThumbData = NULL;
		dataCurveCount = 0;//wcq 2011.11.21
		IsMaxLength = FALSE;//�����ж��Ƿ���Ҫ�ػ�����ͼwcq2011.12.14
	}
	unsigned long int DataPointsNum; //���������
	short ScaleFactor;//����ϵ����ÿ�����ݵ㶼�˸�ϵ��
	int YOffset;//��ֱ����ƫ�ƣ�����ƽ��
	WORD* pDataPoints; //������ֵ ������1000����ֵ,ԭʼ����
	//WORD* pDataCurve; //����ȡ�������������
	CPoint* pDataCurve; //����ȡ������������� x:������ԭ���������е��±꣬y:�����Ӧ�Ĺ���ֵ��ԭʼδ����1000��wcq2011.11.23
	CPoint*  pThumbData;//����ͼ���ݡ����ڻ�������ͼwcq2011.11.10
	CPoint* ptDrawData; //��ͼ�����,ÿ�����Ӧһ����ͼ����ָ��,ԭ������ΪCOTDRPage����m_ptRecord wcq2011.09.21
	//long int Frequency; //Ƶ��//Ϊ�˺�������˾��sor�ļ�ͬ������Ϊ�����cwcq2012.06.12
	//float Frequency; //Ƶ��wcq2012.06.12
	long int Range; //������Χ
	long int PulseWidth; //������
	long int WaveLength; //���Ĳ���
	long int MeasureTime; //����ʱ��
	long int ChainLength; //����
	int dataCurveCount;//pDataCurve�����,��һ��ΪCOUNT_POINT_MAX_MIN��wcq2011.11.23
	float Frequency; //Ƶ��wcq2012.06.12
	float Refraction; //������
	float ChainLoss; //����ģ�������1000��
	float ChainAttenu; //��˥����������1000��
	float MaxTestDistance; //��������������
	float Length; //���߳���(���˳���)wcq2011.11.10
	//INT DataSpacing;//for each pulse width used:one-way time(units of 100 psec) for 10000 data points//wcq2011.10.19���Ϊ��������//wcq2011.11.30
	float DataSpacing;//for each pulse width used:one-way time(units of 100 psec) for 10000 data points//wcq2011.10.19���Ϊ��������//wcq2011.11.30
	WORD DataType; //�������ͣ� ������8888��������4444
	WORD TestMode; //����ģʽ��ƽ������2222��
	WORD Relay;//�̵���//wcq2011.12.06
	WORD Joint1;//ƴ�ӵ�1//wcq2011.12.05
	WORD Joint2;//ƴ�ӵ�2//wcq2011.12.05
	WORD DataSerialNum; //���ݰ����
	unsigned int StartPoint;  //���ߺ���������
	unsigned int MaxPoint;    //���ߺ�������������ֵ

	BOOL IsMaxLength; //�Ƿ��������
	BOOL IsLowerStandard;//wcq2011.09.26������ZOOM_VERTICAL_STANDARD�Ƚ϶�Ϊtrue��false.������ֱ��������ʱȷ���������
	BOOL IsLoadFromFile;//�Ƿ���ļ���������������//wcq2011.10.19
	int FullPointCount;//��������ʱΪ���ٸ���ʱ����ռ��������ͼ����,����ȷ�����߿��wcq2011.09.16
	int FullPointCountMin;//����δ����,Ϊ���ٸ���ʱ����ռ��������ͼ����,����ȷ�����߿��,FullPointCountΪ�����ı�����wcq2011.09.21
	int LastZoomPointCount;//��������ʱ���һ�����Ź����ٵ㣬��ǰ������ŵĵ�ĸ�����һ��һ����wcq2011.09.21
	unsigned int ThumbDrawXCount;//����ͼ����ʱx����ֵ���ֵ�������Ϊ����ͼ������wcq2011.11.10
	EventInfoList EventList;//�¼��б�
};




////#include <vector>
////struct EventInfo //�¼�����Ϣ�ṹ��
////{
////	unsigned long int ulLocation;//�¼�λ��
////	long int lEventType;//�¼�����0,��ʼ�¼���1�����¼���2�Ƿ����¼���3�����¼���4�����¼�
////	float lReflectValue;//����ֵ
////	float lInsertloss;//�������
////	float lAttenuCoefficient;//˥��ϵ��
////	float lCumulativeLoss;//�ۼ����*/
////};
////
////struct EventInfoList//�¼�����Ϣ�б�
////{
////	std::vector<EventInfo> vEventInfo; //��������յ��¼�����
////};
////struct TestResultInfo 
////{
////	BOOL IsMaxLength; //�Ƿ��������
////	BOOL IsLowerStandard;//wcq2011.09.26������ZOOM_VERTICAL_STANDARD�Ƚ϶�Ϊtrue��false.������ֱ��������ʱȷ���������
////	BOOL IsLoadFromFile;//�Ƿ���ļ���������������//wcq2011.10.19
////	WORD DataType; //�������ͣ� ������8888��������4444
////	WORD TestMode; //����ģʽ��ƽ������2222��
////	WORD DataSerialNum; //���ݰ����
////	WORD* pDataPoints; //������ֵ ������1000����ֵ
////	WORD* pDataCurve; //����ȡ�������������
////	short ScaleFactor;//����ϵ����ÿ�����ݵ㶼�˸�ϵ��
////	int YOffset;//��ֱ����ƫ�ƣ�����ƽ��
////	unsigned int StartPoint;  //���ߺ���������
////	unsigned int MaxPoint;    //���ߺ�������������ֵ
////	int FullPointCount;//��������ʱΪ���ٸ���ʱ����ռ��������ͼ����,����ȷ�����߿��wcq2011.09.16
////	int FullPointCountMin;//����δ����,Ϊ���ٸ���ʱ����ռ��������ͼ����,����ȷ�����߿��,FullPointCountΪ�����ı�����wcq2011.09.21
////	int LastZoomPointCount;//��������ʱ���һ�����Ź����ٵ㣬��ǰ������ŵĵ�ĸ�����һ��һ����wcq2011.09.21
////	INT DataSpacing;//for each pulse width used:one-way time(units of 100 psec) for 10000 data points//wcq2011.10.19���Ϊ��������
////	float Refraction; //������
////	float ChainLoss; //����ģ�������1000��
////	float ChainAttenu; //��˥����������1000��
////	float MaxTestDistance; //��������������
////	unsigned long int DataPointsNum; //���������
////	long int Frequency; //Ƶ��
////	long int Range; //������Χ
////	long int PulseWidth; //������
////	long int WaveLength; //���Ĳ���
////	long int MeasureTiem; //����ʱ��
////	long int ChainLength; //����
////	CPoint* ptDrawData; //��ͼ�����,ÿ�����Ӧһ����ͼ����ָ��,ԭ������ΪCOTDRPage����m_ptRecord wcq2011.09.21
////	EventInfoList EventList;//�¼��б�
////};
