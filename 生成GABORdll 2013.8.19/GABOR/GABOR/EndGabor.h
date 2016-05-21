#pragma once
#define CLASSINDLL_CLASS_DECL __declspec(dllexport)


// CEndGabor
struct Gabor_Result     //�����ṹ��
{
	Gabor_Result()
	{
		memset(Event_Array,0,sizeof(Event_Array));
		memset(Event_Loss,0,sizeof(Event_Loss));
		memset(Reflect_Type,0,sizeof(Reflect_Type));
		memset(Event_Hight,0,sizeof(Event_Hight));
		memset(Line_k,0,sizeof(Line_k));
		memset(Total_Loss,0,sizeof(Total_Loss));
		Trail_End=10;
		Valid_Num=0;
	}
	int Reflect_Type[100];  //�¼�����	//Event Code
	int Event_Array[100];  //�¼�λ��  //Event Propagation Time
	float  Event_Loss[100];       //���//Event Loss
	float Event_Hight[100]; //�¼��㷴��dB
	float Line_k[100];//����б��
	float  Total_Loss[100];  //����ġ��ۼ����
	float  Reflect;   //����
	int Valid_Num;    //��Ч����
	float Trail_End;//β��λ��
	char Event_type[100];// event code �ڶ�������  S:��ʼ�¼� E:����ĩ�˼������¼���A���û��Լӣ� F:������в��ң�O:����������Χ�Ĺ���ĩ�� D: zll2012.08.22
};//�ṹ��Ķ����λ������

class CLASSINDLL_CLASS_DECL CEndGabor
{
public:
	CEndGabor(void);
	~CEndGabor(void);
public:
	Gabor_Result deleteEvent(int deleteNum,int wavelength,int pulsewidth,WORD input_data[],Gabor_Result sGabor_Result,float Index_N,float Sample_freaq,float Return_loss_Tybe);
	Gabor_Result insertEvent(int insertNum,int wavelength,int pulsewidth,WORD input_data[],int paramLen,Gabor_Result sGabor_Result,float Index_N,float Sample_freaq,float Return_loss_Tybe);
	Gabor_Result gabor_arithmetic(int wavelength,int pulseWidth,WORD input_data[],INT paramLen,double thresholdUpSet,double thresholdDownSet,float dB_Loss,float dB_reflex,float dB_End,float Return_loss_Tybe,float Sample_freaq,float Index_N,int link_point_number);
public:
	LPVOID m_lpVoid;
};

