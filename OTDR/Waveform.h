#pragma once

#include "resource.h"
#include "afxwin.h"
#include "BtnST.h"
#include "afxcmn.h"
#include "Util.h"
#include "SorFile.h"
#include "WaveFormLeftDlg.h"
#include "SorDrawStatic.h"
#include "NumSpinCtrl.h"
#include "SorFileInfo.h"
#include "OtdrTrace.h"
#include "SorAssist.h"
#include "EndGabor.h"

// CWaveform form view

class CWaveform : public CFormView
{
	DECLARE_DYNCREATE(CWaveform)

protected:
	CWaveform();           // protected constructor used by dynamic creation
	virtual ~CWaveform();

public:
	enum { IDD = IDD_WAVEFORM };

public:
	void CreateLeftDlg(); //������Ի���
	void PopulateData();//�����������
	void SetLocalString();
	void OnOK();

	void SetMainSettingMode(BOOL bNoraml);
	//
	void SetOffsetVal(float fOffset);
	void SetHorUnitText(); //���ú�����̶��ı�
	void SetHorUnitText(INT m_nCurveIndex); //���ú�����̶��ı�
	void SetHorUnitText(float nHorBalFactor);//���ú������ı�
	void SetVerUnitText(); //����������̶��ı� 	
	void ChangeABStatus(BOOL bSelected);//�ı�AB���йصĿؼ���Ϣ   lzy 2015.2.10

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate();
	void OnPostEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedEventPre();
	afx_msg void OnBnClickedEventNext();
	afx_msg void OnBnClickedEventAdd();
	afx_msg void OnBnClickedEventDelete();
	afx_msg void OnBnClickedHorAmplify();
	afx_msg void OnBnClickedHorDeflate();
	afx_msg void OnBnClickedReturn();
	afx_msg void OnBnClickedVDeflate();
	afx_msg void OnBnClickedVAmplify();
	afx_msg void OnBnClickedCursorSelect();

	afx_msg void OnDeltaposSpinOffset(NMHDR *pNMHDR, LRESULT *pResult);

public:
	CWaveFormLeftDlg m_leftDlg;
	//CSorDrawStatic m_sorDrawStatic;//cwcq2013.03.29
	COtdrTrace m_sorDrawStatic;//wcq2013.03.29
	CRect m_rectSorDrawStatic;
	CRect m_rectScrollBarHor;

	//��ӡ������
	CString m_strStartDis; //�����꿪ʼֵ
	CString m_strEndDis; //���������ֵ
	CString m_strUnitDis; //�����굥λ����

	CString m_strUnitDiv; //�����굥λֵ

private:
	CButtonST m_btnCursor;
	CRect m_rectCursor;
	CButtonST m_Return;
	CRect m_rectReturn;
	CButtonST m_Auto;
	CRect m_rectAuto;
	CButtonST m_EventAdd;
	CRect m_rectEventAdd;
	CButtonST m_EventDelete;
	CRect m_rectEventDelete;
	CButtonST m_Mk1;
	CRect m_rectMk1;
	CButtonST m_Mk2;
	CRect m_rectMk2;
	CButtonST m_Clear;
	CRect m_rectClear;
	CButtonST m_hAmplify; //ˮƽ�Ŵ�
	CRect m_recthAmplify;
	CButtonST m_hDeflate; //ˮƽ��С
	CRect m_recthDeflate;
	CButtonST m_vAmplify; //��ֱ�Ŵ�
	CRect m_rectvAmplify;
	CButtonST m_vDeflate; //��ֱ��С
	CRect m_rectvDeflate;
	CButtonST m_EventNext;
	CRect m_rectEventNext;
	CButtonST m_EventPre;
	CRect m_rectEventPre;
	CStatic m_unitDivV;
	CRect m_rectUnitDivV;
	CEdit m_editOffset; //��ֱƫ��
	CRect m_rectOffset;
	CRect m_rectSpinOffset;
	CStatic m_distance_left;
	CRect m_rectdistance_left;
	CStatic m_unit_div;
	CRect m_rectunit_div;
	CStatic m_distance_right;
	CRect m_rectdistance_right;
	CRect m_rectAorB;//zll 2013.5.12
	CRect m_rectAandB;//zll 2013.5.12
	CRect m_rectABCheck;  //lzy 2015.2.11

public:
	CNumSpinCtrl m_spinOffset;//zll 2013.7.2
//////////////zll 2013.4.28
public:
	int m_nSelectCursor; //Seleced��־,1ѡ��A��2ѡ��B, 4ѡ��AB
	void GaborResultToEvent(TestResultInfo* pCurveData);
	float Get_XCoord(unsigned int xpoint, TestResultInfo* pData);//�������
//////////////zll 2013.5.7
public:
	CSorAssist m_sorAssist;
	CEndGabor m_endgabor;
public:
	void HorizonZooms();//ˮƽ������������wcq 2011.09.13
	void HorizonZoom(TestResultInfo* pData);//����һ������wcq2011.09.13
	int m_iHorizonZoomNum; //ˮƽ�Ŵ����
	int m_nDrawWidth; //��ͼ������
	void SetHorScroll(); //����ˮƽ������
	unsigned int m_nStartPoint;  //���ߺ���������
	unsigned int m_nMaxPoint;    //���ߺ�������������ֵ
	void InvalidateAll();//ˢ�»�ͼ������ͼ����������ʾ��
	int m_nMaxIndex;
	int m_nZoomCount;//���Ŵ���
	void SetABType(CString strAB);
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	void InitCurveColor();
	CButton m_AorB;//zll 2013.5.12	
	CButton m_ABCheck;// AB���Ƿ�ѡ�� lzy 2015.2.11

	CString strH_Zoom_in,strH_Zoom_out,strV_Zoom_in,strV_Zoom_out,strScaleInit,strCursorSwitch,strPreEvent,strNextEvent,
		strAddEvent,strDeleteEvent,strMK1,strMK2,strClear;
	// AB��Ҫ��ʾ
	BOOL m_bCheckSelectAB;
	afx_msg void OnBnClickedCheckAbSelect();
	
};