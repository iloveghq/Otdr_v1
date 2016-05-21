#pragma once

// CThumbnail ������ͼ

class CThumbnail : public CFormView
{
	DECLARE_DYNCREATE(CThumbnail)

protected:
	CThumbnail();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CThumbnail();
	void DrawThumbnail(CDC *pDC);
	void DrawThumbCoord(CDC *pDC);
	void DrawThumbCurve(CDC *pDC);
	//��ȡ����ͼ����
public:
	void GetAllThumbData();
	void GetThumbData(TestResultInfo* pTestResultInfo);
	void GetMoveRect(CRect* rectMove, TestResultInfo* pTestResultInfo);
	void SetLocalString();

public:
	enum { IDD = IDD_THUMBNAIL };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnInitialUpdate();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
	CRect m_rectThumbnail;
};
