#pragma once
#include "atltypes.h"
#include "ReportFileSelect.h"
#include "PropStep4.h"
// PrintView.h : header file
//
#define MARGIN_LEFT_COUNT 18  //�������յĿ��ռ�ܿ�ȵļ���֮��lzy2014.8.30

/////////////////////////////////////////////////////////////////////////////
// CPrintView view

const float FONTSIZE_RATIO[5] = {1.0, 0.8, 0.6, 0.45, 0.4};  //�������ʱ�õ���������ű�����[0]��ʾ������[1]��ʾ2ͼ/ҳ��[2]��ʾ4ͼ/ҳ��[3]��ʾ6ͼ/ҳ��[4]��ʾ8ͼÿҳ


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
	void DrawCursorAB(CDC* pDC, CRect& rect);//���ƹ�� xjf2014.3.3
	CString right_MAX(float m_float_zll,float flo_Length);//����������ұ���󳤶Ⱥ���
	CString branch(float m_float_zll,float flo_Length);//��������굥λ���Ⱥ���
	void GetPrintInfo(int nCurSel); //��ȡ��ӡ��Ҫ�Ĳ���lzy 2014.8.12
	void DrawCurCurveNew(CDC* pDC, CRect rectDraw,int nCurSel = 0, BOOL bSingle = TRUE); //��ǰѡ�в��λ�ͼlzy2014.8.11
	void DrawScale(CDC *pDC, CRect &rect, float fYmax = 0, float fXmax = 0);//���ƿ̶�lzy2014.7.17
	void DrawCurve(CDC* pDC, CRect& rect, int nCurveIndex);	
	void DrawTrace(CDC *pDC, CRect rc, int nCurSel, float fontRatio = 1.0); //���ƴ��������ߣ�����AB���� lzy2014.8.26	
	void DrawTraceBmp(CDC *pDC, CRect rc, int nCurSel, float fontRatio); //���ƴ��������ߣ�������AB���ߣ�����excelר�� lzy2014.9.3

private:
	void DrawReport(CDC* pDC);
	void Draw1Curve(CDC* pDC,int m_nCurPage); //ÿҳ1������
	void Draw2Curve(CDC* pDC,int m_nCurPage); //ÿҳ2������
	void Draw4Curve(CDC* pDC,int m_nCurPage); //ÿҳ4������
	void Draw6Curve(CDC* pDC,int m_nCurPage); //ÿҳ6������
	void Draw8Curve(CDC* pDC,int m_nCurPage); //ÿҳ8������
	void Draw1CurveNew(CDC* pDC,int m_nCurPage); //ÿҳ1������lzy2014.8.28
	void Draw2CurveNew(CDC* pDC,int m_nCurPage); //ÿҳ2������lzy2014.8.28
	void Draw4CurveNew(CDC* pDC,int m_nCurPage); //ÿҳ4������lzy2014.8.28
	void Draw6CurveNew(CDC* pDC,int m_nCurPage); //ÿҳ6������lzy2014.8.28
	void Draw8CurveNew(CDC* pDC,int m_nCurPage); //ÿҳ8������lzy2014.8.28
	void DrawCurCurve(CDC* pDC); //��ǰѡ�в���
	
	void InnerDraw(CDC* pDC,int m_nCurPage); //�ڲ���ͼֻ��
	void DrawReportItem(CDC* pDC, CRect& rect, int nItemIndex); //���Ʊ�����Ŀ
	void DrawCurveItem(CDC* pDC, CRect& rect, int nItemIndex,int m_CurveNum,CString m_sorName); //����ÿ��������Ŀ
	void DrawCurveByReport(CDC* pDC, CRect& rect, int nCurveIndex);//zll 2013.6.4	
	
	CString GetSorName(int m_CurveNum);// zll 2013.6.5 ����.sor�����ļ��� 
	
	void DrawHorCoordUnit(CDC* pDC, CRect& rect, CString strText);//���ƺ����굥λ�ı�
	void DrawVerCoordUnit(CDC* pDC, CRect& rect); //�����������ı�

	void DrawEventHead(CDC* pDC, int nLeft, int nTop, CRect rectDraw);//�����¼��б��ͷlzy2014.8.28
	void DrawEventBody(CDC* pDC, int nLeft, int nTop, int nVspace, CRect rectDraw, int nCurSel);//�����¼��б�lzy2014.8.28
	void DrawEventBody1(CDC* pDC, int nLeft, int nTop,int m_nCurPage);//�����¼��б�
	void DrawCurveParam(CDC* pDC, int nLeft, int nTop); //�������߲���
	void DrawReportContent(CDC* pDC, int nLeft, int nTop); //���Ʊ�������
	
	void InitFont(CDC* pDC); //��ʼ������ lzy2014.9.3
	
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
	CFont m_FontVR;
	CFont m_fontGroup;   //���������� lzy 2014.8.11

	//lzy 2014.8.12
	BOOL m_bPrintLogo; //�Ƿ��ӡ��ά��Ȩ��Ϣ
	BOOL m_bPrintJob;  //�Ƿ��ӡJob info
	BOOL m_bPrintConfig;
	BOOL m_bPrintComment;
	BOOL m_bPrintTrace;
	BOOL m_bPrintEvent;
	
};