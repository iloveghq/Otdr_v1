#include "stdafx.h"
#include "ExcelUtil.h"
#include <afxole.h>

const short xlBorderWeightThin = 2;


CExcelUtil::CExcelUtil(void):
	m_covTrue((short)TRUE),
	m_covFalse((short)FALSE),
	m_covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR)
{
}

CExcelUtil::~CExcelUtil(void)
{
	ExitExcel();
}

/************************************************************************/
/*  param strSheet sheetname                                            */
/************************************************************************/
BOOL CExcelUtil::InitExcel(const CString& strFileName, int nSheetIndex,int SheetCount)
{
	if (!m_app.CreateDispatch(_T("Excel.Application"), NULL))
	{
		CString str_install_excel=IniUtil::ReadResString(_T("IDS_INSTALL_EXCEL"));
		::MessageBox( NULL, str_install_excel, _T("OTDR Viewer"), MB_OK | MB_ICONERROR);		
		//exit(1);  //��excel����ʱ��δԤ�Ȱ�װExcel��˴�ֱ���˳������ֽ�֮����ʹ�����ܼ������С�lzy 2014.8.20
		return FALSE;
	}
	str_Office_Edition=m_app.get_Version();
	m_strFileName = strFileName;
	//����Ϊ��ʾ
	m_app.put_Visible(FALSE);
	m_app.put_UserControl(TRUE);
	m_app.put_DisplayAlerts(FALSE);
	m_books.AttachDispatch( m_app.get_Workbooks(), TRUE );

	LPDISPATCH  lpDisp = NULL; 

	// ���ļ�
	if(_T("9.0")==str_Office_Edition||_T("10.0")==str_Office_Edition||_T("11.0")==str_Office_Edition)
	{
		lpDisp = m_books.Open(strFileName,
			m_covOptional, m_covOptional, m_covOptional, m_covOptional,
			m_covOptional, m_covOptional, m_covOptional, m_covOptional,
			m_covOptional, m_covOptional, m_covOptional, m_covOptional
			);
	}
	else if(_T("12.0")==str_Office_Edition||_T("14.0")==str_Office_Edition)
	{
		lpDisp = m_books.Open(strFileName,
			m_covOptional, m_covOptional, m_covOptional, m_covOptional,
			m_covOptional, m_covOptional, m_covOptional, m_covOptional,
			m_covOptional, m_covOptional, m_covOptional, m_covOptional, m_covOptional, m_covOptional
			);
	}
	else//��ǰû�� wcq2016.03.03
	{
		INT nVersion = _ttoi(str_Office_Edition);
		if (nVersion < 9)//�Ͱ汾
		{
			lpDisp = m_books.Open(strFileName,
				m_covOptional, m_covOptional, m_covOptional, m_covOptional,
				m_covOptional, m_covOptional, m_covOptional, m_covOptional,
				m_covOptional, m_covOptional, m_covOptional, m_covOptional);
		}
		else//�߰汾
		{
			lpDisp = m_books.Open(strFileName,
				m_covOptional, m_covOptional, m_covOptional, m_covOptional,
				m_covOptional, m_covOptional, m_covOptional, m_covOptional,
				m_covOptional, m_covOptional, m_covOptional, m_covOptional, m_covOptional, m_covOptional);
		}//eif
	}//eif
	ASSERT(lpDisp);

	// ��û��WorkBook( ������ )
	m_book.AttachDispatch(lpDisp, TRUE);
	// ��ù����� 
	m_sheets.AttachDispatch(m_book.get_Sheets(), TRUE);
	//��ȡ�����������
	int nSheetNumer = m_sheets.get_Count();
	LPDISPATCH lpActiveSheetDisp = NULL;
	CWorksheet sheet;
	for (int nIndex = nSheetNumer; nIndex >= 1; nIndex--)
	{
		LPDISPATCH lpSheetDisp = m_sheets.get_Item(_variant_t(nIndex));
		if (nIndex != nSheetIndex)
		{
			sheet.AttachDispatch(lpSheetDisp);
			sheet.Delete();
			sheet.ReleaseDispatch();
		}
		else
			lpActiveSheetDisp = lpSheetDisp;
	}

	// ���ָ���Ĺ����� 
	m_sheet.AttachDispatch(lpActiveSheetDisp, TRUE);

	for (int i=1;i<SheetCount;i++)
	{
		COleVariant vOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
		COleVariant temp;
		temp.pdispVal = m_sheets.get_Item(_variant_t(i));
		temp.vt = VT_DISPATCH;
		m_sheet.Copy(vOpt, temp);
	}
	/*m_sheet.Copy(_variant_t())
	CWorksheet m_sheetNew;
	for (int i=0;i<SheetCount;i++)
	{
		m_sheets.Copy(_variant_t((long)i+1),_variant_t((long)i+3));
	}*/

	return TRUE;
}

void CExcelUtil::Save()
{
	//�������ݷ�ֹ���֡���һ�������������С������⣬ע��ʹ�����´�����#include <afxole.h>
	AfxOleGetMessageFilter()->EnableBusyDialog(FALSE);
	AfxOleGetMessageFilter()->SetBusyReply(SERVERCALL_RETRYLATER);
	AfxOleGetMessageFilter()->EnableNotRespondingDialog(TRUE);
	AfxOleGetMessageFilter()->SetMessagePendingDelay(-1);
	//���� 
	m_book.Save();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//�������saveas�����excel2007���� wcq2012.10.31
void CExcelUtil::Save(CString filePathFull)
{
	//�������ݷ�ֹ���֡���һ�������������С������⣬ע��ʹ�����´�����#include <afxole.h>
	AfxOleGetMessageFilter()->EnableBusyDialog(FALSE);
	AfxOleGetMessageFilter()->SetBusyReply(SERVERCALL_RETRYLATER);
	AfxOleGetMessageFilter()->EnableNotRespondingDialog(TRUE);
	AfxOleGetMessageFilter()->SetMessagePendingDelay(-1);

	//���� 
	long NewFileFormat = 51;//NewFileFormat=51�����office2007��ʽ, NewFileFormat=56�����office2003��ʽ 
	//CString strSuffix = filePathFull.Mid(filePathFull.ReverseFind(_T('.')));
	//if (0 == strSuffix.CompareNoCase(_T(".xlsx")))//Excel2007
	//{
	//	NewFileFormat = 56;
	//}
	m_book.SaveAs(_variant_t(filePathFull), _variant_t((long)NewFileFormat), vtMissing, vtMissing, vtMissing, 
		vtMissing, 0, vtMissing, vtMissing, vtMissing,
		vtMissing, vtMissing);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void CExcelUtil::ExitExcel()
{
	//�ͷ���Դ
	m_sheet.ReleaseDispatch();
	m_sheets.ReleaseDispatch();
	m_book.Close(m_covOptional,COleVariant(m_strFileName),m_covOptional);
	m_book.ReleaseDispatch ();
	m_books.Close();
	m_books.ReleaseDispatch ();
	m_app.Quit();
	m_app.ReleaseDispatch();
}

void CExcelUtil::CombineCell(CString cell1,CString cell2)
{
	CRange range;
	range = m_sheet.get_Range(COleVariant(cell1), COleVariant(cell2));
	//	range.put_MergeCells(_variant_t("ad"));
}

void CExcelUtil::SetBorder(CString cell1,CString cell2,long borderIndex,int borderWeight)
{	//all=6;left=7;top=8;bottom=9;right=10;midColumn=11;midRow=12;
	if(borderIndex<6 || borderIndex>12)
		return;

	CRange range;
	range = m_sheet.get_Range(COleVariant(cell1), COleVariant(cell1));
	CBorders borders;

	borders = range.get_Borders();
	short xlBorderWeightThin = borderWeight;
	if(borderIndex==6)
		borders.put_Weight(COleVariant(xlBorderWeightThin));
	else
	{
		CBorder border;
		border = borders.get_Item(borderIndex);
		border.put_Weight(COleVariant(xlBorderWeightThin));	
		border.ReleaseDispatch();
	}
	borders.ReleaseDispatch();
	range.ReleaseDispatch();
}

void CExcelUtil::InsertItem(CString strPos,CString strValue)
{
	CRange range;
	range = m_sheet.get_Range(COleVariant(strPos), COleVariant(strPos));
	range.put_Value2(COleVariant(strValue));
	range.ReleaseDispatch();
}

void CExcelUtil::InsertItem(int cellRow,int cellColumn,CString strValue)
{
	CRange range;
	range.AttachDispatch(m_sheet.get_Cells(),true); 
	range.put_Item(_variant_t((long)cellRow),_variant_t((long)cellColumn),_variant_t(strValue)); 
	range.ReleaseDispatch();  
}

void CExcelUtil::InsertItem(CString cell1,CString cell2,COleSafeArray *saRet)
{
	CRange range;
	range = m_sheet.get_Range(COleVariant(cell1), COleVariant(cell2));
	range.put_Value2(COleVariant(saRet));
}

LRESULT CExcelUtil::InsertPicture(CString picPath,CString strPos,int SheetCount,int picWidth,int picHeight,int iBorder)
{
	CWorksheet sheet;
	CRange range;
	CPictures pics;
	CPicture pic;
	CBorder border;

	// �õ�����ͼƬ��λ��
	m_sheet=m_sheets.get_Item(_variant_t(SheetCount));
	range = m_sheet.get_Range(COleVariant(strPos), COleVariant(strPos));
	//range = m_sheet.get_Range(COleVariant(_T("A12")), COleVariant(_T("M38")));//��ʱ����һ�� wcq2012.11.05???????????????????
	m_sheet.Activate();
	range.Select();
	//CShapes shapes = m_sheet.get_Shapes();
	//shapes.AddPicture(picPath,false,true,0,0,100,100);

	pics = m_sheet.Pictures(m_covOptional);
	
	pic = pics.Insert(picPath, m_covOptional);//����ͼƬ

	if(picWidth!=0)	//�Զ���ͼƬ��
		pic.put_Width(picWidth);
	if(picHeight!=0)	//�Զ���ͼƬ��
		pic.put_Height(picHeight);

	//���office2007 �������� ����ͼƬ���뵽Excel����λ�ô���
	pic.put_Left((double)range.get_Left().dblVal);//wcq2012.11.06
	pic.put_Top((double)range.get_Top().dblVal);//wcq2012.11.06

	if(iBorder!=0)	//�Ƿ�ӱ߿�
	{
		border = pic.get_Border ();
		short xlBorderWeightThin = iBorder;
		border.put_Weight(COleVariant(xlBorderWeightThin));
	}

	range.Show ();
	// ��� cell height/width
	VARIANT varCellH = range.get_Height();
	double cellH = varCellH.dblVal;
	VARIANT varCellW = range.get_Width();
	double cellW = varCellW.dblVal;

	range.ReleaseDispatch();
	sheet.ReleaseDispatch(); 
	border.ReleaseDispatch();
	//���ͼƬ�ߣ�����ռcell��������Ϊ����ֵ
	double w = pic.get_Width ();
	double h = pic.get_Height ();
	int cells_Row = (int) (h/cellH);
	int cells_Column = int(w/cellW);
	LRESULT  lp = MAKEWPARAM(cells_Column,cells_Row);
	return lp;
}
