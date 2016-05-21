#pragma once
#include "atltypes.h"
#include "ReportFileSelect.h"
#include "PropStep4.h"
// PrintView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrintView view

class CPrintView : public CView
{
public:
	CPrintView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPrintView)

// Attributes
public:
	BOOL	m_bPrint;
	BOOL	m_bPrintCurve;
	CRect m_rectDraw;//���ƾ��������С
// Operations
public:
	void OnMyPrint();
	void OnMyPrintPreview();
	void PrintPageHeader(CDC* pDC, UINT nPageNumber, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
	void DrawGridLine(CDC* pDC, CRect& rect);//����դ�� wcq2012.10.31
	CString right_MAX(float m_float_zll,float flo_Length);//����������ұ���󳤶Ⱥ���
	CString branch(float m_float_zll,float flo_Length);//��������굥λ���Ⱥ���

private:
	void DrawReport(CDC* pDC);
	void Draw1Curve(CDC* pDC,int m_nCurPage); //ÿҳ1������
	void Draw2Curve(CDC* pDC,int m_nCurPage); //ÿҳ2������
	void Draw4Curve(CDC* pDC,int m_nCurPage); //ÿҳ4������
	void Draw6Curve(CDC* pDC,int m_nCurPage); //ÿҳ6������
	void Draw8Curve(CDC* pDC,int m_nCurPage); //ÿҳ8������
	void DrawCurCurve(CDC* pDC); //��ǰѡ�в���
	void InnerDraw(CDC* pDC,int m_nCurPage); //�ڲ���ͼֻ��
	void DrawReportItem(CDC* pDC, CRect& rect, int nItemIndex); //���Ʊ�����Ŀ
	void DrawCurveItem(CDC* pDC, CRect& rect, int nItemIndex,int m_CurveNum,CString m_sorName); //����ÿ��������Ŀ
	void DrawCurve(CDC* pDC, CRect& rect, int nCurveIndex);
	void DrawCurveByReport(CDC* pDC, CRect& rect, int nCurveIndex);//zll 2013.6.4
	CString GetSorName(int m_CurveNum);// zll 2013.6.5 ����.sor�����ļ��� 
	
	void DrawHorCoordUnit(CDC* pDC, CRect& rect, CString strText);//���ƺ����굥λ�ı�
	void DrawVerCoordUnit(CDC* pDC, CRect& rect); //�����������ı�

	void DrawEventHead(CDC* pDC, int nLeft, int nTop);//�����¼��б��ͷ
	void DrawEventBody(CDC* pDC, int nLeft, int nTop);//�����¼��б�
	void DrawEventBody1(CDC* pDC, int nLeft, int nTop,int m_nCurPage);//�����¼��б�
	void DrawCurveParam(CDC* pDC, int nLeft, int nTop); //�������߲���
	void DrawReportContent(CDC* pDC, int nLeft, int nTop); //���Ʊ�������
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrintView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPrintView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
public:
	//{{AFX_MSG(CPrintView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	// Print 
	BOOL m_bPrintData;
	BOOL m_bPrintImage;
	BOOL m_bPrintHead;
	UINT m_cxWidth;
	UINT m_cxOffset;
	UINT m_nLinesPerPage;
	UINT m_PageCount;
	UINT m_cyPrinter;
	CFont m_ListFont;
	CFont m_fontPrinter;
	CFont m_dataFont;
};