#pragma once
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
//��дsor�ļ�ʱ�����봫������
//Ϊʹ�޸ĳ����ж�Ӧ�Ĳ���ʱ����Ӱ�쵽sor.dll�޸ģ����Ե����ó���
//wcq2012.07.23
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
//�¼�
struct Event
{
	INT propagationTime;//���Ӻ��һ��e�¼���ʱ��
	short attenCoefficent;//˥��ϵ��
	short eventLoss;//�¼����
	INT   reflectance;//�¼�����
	std::vector<char> code;//�¼���־�̶�6�ֽ�6
	std::vector<char> lossMeasurement;//�̶�2�ֽ�2
	std::vector<char> comment;//ע��
};
//����
struct Data
{
	INT nCount;//���
	short scaleFactor;//����2�ֽ� ϵ��
	std::vector<unsigned short> vData;//����
};
struct SORDATA
{
	std::vector<Data> vAllData;//��������
	long int Range; //������Χ sor���Ӧ Acquisition Range Distance(Acquisition Range����ʱ���ʾ����) ,��λ:m
	long int PulseWidth; //������
	long int WaveLength; //���Ĳ���
	float Frequency; //Ƶ��
	float Refraction; //������
	float Length; //���߳���(���˳���),
	float DataSpacing;//for each pulse width used:one-way time(units of 100 psec) for 10000 data points//wcq2011.10.19���Ϊ��������//wcq2011.11.30

	////wcq2012.10.30
	float BackscatterCoefficient;//ɢ��ϵ�� wcq2012.10.30
	float LossThreshold;//˥������ wcq2012.10.30
	float ReflectanceThreshold;//�������� wcq2012.10.30
	float EndOfFiberThreshold;//����ĩ������ wcq2012.10.30
	time_t TestTimestamp;    //������ʱ���lzy2014.8.8
	UINT AverageCount;   //ƽ���Ĵ���lzy2014.8.8

	std::vector<Event> vEvent; //
};