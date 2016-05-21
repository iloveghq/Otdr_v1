#pragma once

#include "CApplication.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CRange.h"
#include "CWorksheets.h"
#include "CPicture.h"
#include "CPictures.h"
#include "CBorder.h"
#include "CBorders.h"

class CExcelUtil
{
public:
	CExcelUtil(void);
	~CExcelUtil(void);
	BOOL InitExcel(const CString& strFileName, int nSheetIndex,int SheetCount);
	void Save();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Save(CString filePathFull);//�������saveas�����excel2007���� wcq2012.10.31
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	void ExitExcel();
	void CombineCell(CString cell1,CString cell2);
	void SetBorder(CString cell1,CString cell2,long borderIndex,int borderWeight);

	void FillSafeArray(CString strText, int iRow, int iCol,COleSafeArray* sa);
	void InsertItem(CString cell1,CString cell2,COleSafeArray* saRet);
	void InsertItem(CString strPos,CString strValue);	//�������ݣ�strPosΪExcel�е�λ�ã���A1��B2��
	void InsertItem(int cellRow,int cellColumn,CString strValue);	//��������
	//����ͼƬʱ������ֵֵΪͼƬ��С����Excel����ռ�ĵ�Ԫ�񣬵�λΪcolumn����λΪrow��
	LRESULT InsertPicture(CString picPath,CString strPos,int SheetCount=0,int picWidth=0,int picHeight=0,int iBorder=0);

	public:
	// spreadsheet variables
	CApplication	m_app;
	CWorkbook		m_book;
	CWorkbooks		m_books;
	CWorksheet		m_sheet;
	CWorksheets		m_sheets;
	COleVariant		m_covTrue;
	COleVariant		m_covFalse;
	COleVariant		m_covOptional;

	//excel �ļ���
	CString m_strFileName;
	CString str_Office_Edition;
};
