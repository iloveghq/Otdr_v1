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
	long int Range; //������Χ
	long int PulseWidth; //������
	long int WaveLength; //���Ĳ���
	float Frequency; //Ƶ��
	float Refraction; //������
	float Length; //���߳���(���˳���),
	float DataSpacing;//for each pulse width used:one-way time(units of 100 psec) for 10000 data points//wcq2011.10.19���Ϊ��������//wcq2011.11.30

	std::vector<Event> vEvent; //
};