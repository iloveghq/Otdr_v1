// GABOR.h : GABOR DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGABORApp
// �йش���ʵ�ֵ���Ϣ������� GABOR.cpp
//

class CGABORApp : public CWinApp
{
public:
	CGABORApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
