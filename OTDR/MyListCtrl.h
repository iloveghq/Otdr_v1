#pragma once
#include "afxcmn.h"
#include "MyFont.h"
#define WIDTH_RADIO 16//��ѡͼ����wcq2012.03.01
#define RADIO_HEIGHT_LINE 20//��ѡͼ��߶�wcq2012.03.01
#define COLOR_BG_GROUP CLR_GBX_BG_T2//��ѡͼ����wcq2012.03.01
#define COLOR_TEXT_RADIO RGB(0,0,0)//��ѡͼ����wcq2012.03.01
#define COLOR_TEXT_RADIO_DISABLE RGB(192,192,192)//��ѡͼ����wcq2012.03.01
/////////////////////////////////////////////////////////////////////////////////////
//ʹ�÷���
//m_imageList.Create(16, RADIO_HEIGHT_LINE, ILC_COLOR4,10,10 );   
//myList.SetImageList(   &m_imageList,   LVSIL_SMALL   );
//myList.SetItemImg(IDB_RADIO_NORMAL,IDB_RADIO_SELE,IDB_RADIO_DISABLE_NORMAL,IDB_RADIO_DISABLE_SELE);
//////////////////////////////////////////////////////////////////////////////////////
class CMyListCtrl : public CListCtrl
{
public:
    CMyListCtrl(void);
    ~CMyListCtrl(void);

	void SetItemImg(UINT idNormal,UINT idSele,UINT idDisableNormal,UINT iDisableSele);
	INT GetSeleIndex();//ѡ���±��0��ʼwcq2012.03.01
	void SetSeleIndex(int nIndex);//wcq2012.03.01
protected:
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

    DECLARE_MESSAGE_MAP()
protected:
    virtual void PreSubclassWindow();
    //}}AFX_VIRTUAL
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    void RepaintSelectedItems();

	HBITMAP     m_bmpNormal;//����
	HBITMAP     m_bmpSele;
	HBITMAP     m_bmpDisNormal;//disable normal
	HBITMAP     m_bmpDisSele;// disable select
	//CFont m_font;//��Ϊȫ������wcq2012.03.07
	CFont* m_font;//wcq2012.03.07
	//INT m_height;//�߶�
private:
    void Init();
    void PaintBk(CDC *pDC, CRect& rect);
    void AssignImg(UINT id, HBITMAP* hBMP);
public:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void MeasureItem ( LPMEASUREITEMSTRUCT lpMeasureItemStruct );
};
