#pragma once

struct EventAnalConfig
{
	float BendTH;//˥������
	float ReflectTH; //��������
	float Termial; //����ĩ��
	float BackScatter;//����ɢ������ 1310nm
	float BackScatter1550;//����ɢ������ 1550nm //zll 2012.10.16
	float BackScatter1625;//����ɢ������ 1550nm //zll 2013.5.10 ��
	float Refraction;//������ 1310nm
	float Refraction1550;//������ 1550nm //zll 2012.10.16
	float Refraction1625;//������ 1625nm //zll 2013.5.10 ��

	float Reflect; 
};