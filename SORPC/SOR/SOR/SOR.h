// SOR.h : SOR DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSORApp
// �йش���ʵ�ֵ���Ϣ������� SOR.cpp
//

class CSORApp : public CWinApp
{
public:
	CSORApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
