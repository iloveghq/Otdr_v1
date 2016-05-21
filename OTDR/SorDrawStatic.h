// OTDRDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "EndGabor.h"
#include "Util.h"//wcq2011.09.14
#include <vector>
#include "DATASTRUCT.h"
//#include "20120801.h"
#include "Waveform.h"
#include <math.h>

//������������ʼdb,�������������ֵ//wcq2011.09.26

#define  M_FSTARTDB0 -65.63//-65.53//wcq2011.12.05
#define  M_FMAXDB0   -5.63//-9.23//wcq2011.12.05   zll 2012.10.26 ��-9.23��Ϊ-15.63   lzy2014.6.17 ��-15.63��Ϊ-5.63
#define  M_FSTARTDB1 -58.3//����������Сֵ�� > ZOOM_VERTICAL_STANDARD 30*1000ʱʹ��//wcq2011.09.26
#define  M_FMAXDB1   -8.3//wcq2011.09.26  zll 2012.10.26 ��-1��Ϊ-8.3
#define  ZOOM_VERTICAL_LEAST_RANGE 0.8//��ֱ����Ŵ������ܹ���ʾ����dB//wcq2011.09.28
#define  ZOOM_VERTICAL_PER_TIME 0.5//��ֱ����Ŵ�����//wcq2011.09.28
#define  ZOOM_VERTICAL_COUNT 47//��ֱ�������Ŵ����//wcq2011.09.28//  zll 2012.12.31�޸�
#define  ZOOM_VERTICAL_MAX 98//��ֱ����Ŵ����ʱ�ж���dB//wcq2011.11.11��
#define  EVENT_POINT_HEIGHT 30//�����߻����¼���ʱ���һ����ֱ�ߣ���ֵΪ���߸߶�//wcq2011.11.15
#define  EVENT_POINT_NUM_HEIGHT 20//�����߻����¼���ʱ���һ����ֱ�ߣ���ֵΪ��ֱ���·���1,2..��ֱ����λ��//wcq2011.11.15
#define  COUNT_POINT_MAX_MIN 686*3//��ͼ�á���������x���ϸ����������Сֵ�����,�����޸�//wcq2011.11.23
#define  COUNT_GRIDLINE_HORIZONE 10//��ͼ�á�ˮƽ������//wcq2011.12.19
#define  COUNT_GRIDLINE_VERTICAL 8//��ͼ�á���ֱ������//wcq2011.12.19

//��Ϣ
#define MESSAGE_ZOOMIN (WM_USER + 1001)//������С��Ϣwcq2011.09.30
#define MESSAGE_ZOOMOUT (WM_USER + 1002)//���߷Ŵ���Ϣwcq2011.09.30
//
#define OTDR_DATA_READY (WM_USER+2000)////wcq2011.08.11 ������Ϣ��������׼����
#define OTDR_LIGHT_CHECK (WM_USER+2001)//�������й��������Ӧ//wcq2011.12.02
#define OTDR_PARAMS_CONFIG (WM_USER+2002)//�����Բ������������Ӧ//wcq2011.12.02
//�й��� �������� ������׼�����������//wcq2011.12.05
#define OTDR_MESSAGE_CODE_LIGHT 18//�й���//wcq2011.12.05
#define OTDR_MESSAGE_CODE_PARAMS 19//��������//wcq2011.12.05
#define OTDR_MESSAGE_CODE_DATA 23//������׼����//wcq2011.12.05
//ƽ������ʱ��ǰ�����ĸ��׶Σ��й��� �������� ������׼����
#define AVG_STATE_LIGHT 1//�й���//wcq2011.12.06
#define AVG_STATE_LINK 2//����״̬ ����ʵʱ�����µ����ڲ���//wcq2011.12.07
#define AVG_STATE_PARAMS 3//��������//wcq2011.12.06
#define AVG_STATE_TESTING 4//���ݶ�ȡ ���ڲ��� //wcq2011.12.06

//
#define TIME_SHOW_LINK_PARAMS 1000//��������ǰ��ʾ����״̬�೤ʱ��//wcq2011.12.07
//
#define COUNT_READ_DATA_OTHER 10//��ȡ����ʱ���˸��������⣬�����粨�����ֶθ���.������Э�����wcq2011.12.06

//���һ�ζ��ٵ�ʱ�㱾�����ţ����ٵ��ɸ�ϵ��ȷ��
#define ZOOM_LAST_COEFF 0.6///
#define COUNT_POINT_ONE_PIX 4///�������ܶ�ʱȡ�㣬ȡ��ʱ�Կ�ȵĸ����ݱ���Ϊ�Ƿ�ȡ����ٽ��//wcq2011.09.28

#define LASER_TIMER 1
#define AVG_TEST_TIMER 2
#define STATE_PARAMS_TIMER 3 //��������ǰ����ʾһС������״̬ wcq2011.12.07
#define NUM_VALIDE_LEAST 100//��CurveData�е����ݸ���<��ֵʱ�����ԣ������л���wcq2011.09.13

#define CLEAR_DATAPOINTS_NUM 1//���²��Ժ��������ʱ����Ӧ������pDataPointsNumֵ����Ϊ0���׵��³�0,��ʾ���.wcq2011.12.23

#define WIDTH_CURSOR_LEFT 36//����ƶ�ʱ�������ȡ�wcq2011.12.26
#define WIDTH_CURSOR_INVALID 136//����ƶ�ʱ�������λ�ú��������������ȡ�����ȷ���ػ�����wcq2011.12.26
#define WIDTH_CURSOR_MOVE_LONG 20//��곤��ÿ���ƶ����� wcq2011.12.27
#define WIDTH_CURVE_MOVE_LONG 16//����ƽ�� ����ÿ���ƶ����� wcq2011.12.27
#define WIDTH_OTDR_BG_MAIN_TOP 5//�������䱳���߶�wcq2012.02.13
#define INVALID_WAVELENGTH -1//��Ч��δ����ʱĬ�ϣ�����
#define INVALID_MAX_POINT 0//��Ч��δ����ʱĬ�ϣ��������ݵ����wcq2012.02.20
#define WIDTH_CURVE_SPAN 4//��ͼ����ͱ߾���//wcq2012.02.14
#define WIDTH_CLIENT 700//�ͻ������//wcq2012.02.14
#define HEIGHT_CLIENT (480-28)//�ͻ������//wcq2012.02.14

////#define TRACE_PAINT_TIME 1//��Ϊ�˷������ʱ�����ͼʱ��2011.12.15
//#define TRACE_FIND_MEMORY 1////��Ϊ�˷������ʱ�����ڴ�й©��ء�2011.12.15
#define  TIMER_TEMP 12//����������ʱ�õ�

#define  OTDR_EVENT_NUM_MAX 100//�¼������
typedef struct EventPtStruct
{
	unsigned int nLocation;//�¼�λ��
	int nEventType;//�¼�����0,��ʼ�¼���1�����¼���2�Ƿ����¼���4�����¼�
	float fReflectValue;//����ֵ
	float fInsertloss;//�������
	float fAttenuCoefficient;//˥��ϵ��
	float fCumulativeLoss;//�ۼ����*/

}EventPtInfo;

class CMainFrame;

// CSorDrawStatic �Ի���
class CSorDrawStatic : public CButton
{
	DECLARE_DYNAMIC(CSorDrawStatic)
	// ����
public:
	CSorDrawStatic();	// ��׼���캯��
	virtual ~CSorDrawStatic();
	void FillCurveDataFromFile(TestResultInfo* pCurve);
	void EventIsSelected(int PointAOrB);
	void AddEvent();
	void RemoveEvent();
	int GetCurTestMode() const;//��ȡ��ǰ����ģʽ
	void SetCurTestMode(const int iTestMode); //���õ�ǰ����ģʽwcq2011.12.06
	void SetWaveToDevice(const int iTestMode); //���õ�ǰ���͵��豸�Ĳ���(m_nWaveToDevice:1550/1310) wcq2011.12.19
	//Draw Curve
	void DrawCursor(CDC *pDC);
	void DrawCurve(CDC *pDC, TestResultInfo* pTestResultInfo,int SingleorMulti);//���Ƶ�������
	void DrawCurves(CDC *pDC); //������������
	void CalMaxDistance();//����������������ľ��벢�����������߽ṹ��Ӧ����wcq 2011.09.09

	float Get_XCoord(int xpoint);
	float Get_XCoord(unsigned int xpoint, TestResultInfo* pData);//�������
	float Get_XCoord2nf(unsigned int xpoint, TestResultInfo* pData);//�������,��c/2nf��ʽ����
	UINT  Get_PosPoint(float distance,TestResultInfo* pData);//wqc2012.02.21//��֪���뷴�������±�
	float GetMaxDistance();////wcq2011.09.06�õ�������������ľ���,��kmΪ��λ????????????????
	///float GetTestDistance(const int iPointNumber) const;//wcq2012.03.09
	void DrawCoord(CDC *pDC);
	void CalCulDispRect();
	void ReCalABLabelPos();//ÿ�μ��ػ����һ���µ���������ʱ���¼���A/B��λ�ã���������m_curABWave.wcq 2011.0913
	void ResetCurveData(); //���¼�������
	//Load Curve Data

	void EventList();
	void EventCursor();
	//Cursor Operator
	void CursorMoveLeft();
	void CursorMoveRight();
	//Curve Operate
	void CurveHorizonZoomIn();
	void HorizonZooms();//ˮƽ������������wcq 2011.09.13
	void HorizonZoom(TestResultInfo* pData);//����һ������wcq2011.09.13
	void CurveHorizonZoomOut();
	void CurveVerticalZoomIn();
	void CurveVerticalZoomOut();
	void VerticalZoom();
	void CurveCompare(const CString strFileName);
	void CurvesHorMove(int nRange, float nHorBalFactor);//ˮƽ�ƶ�����
	void CurveHorMove(TestResultInfo* pData, int nRange, float nHorBalFactor);
	void CurvesVerMove(int nDelta);//��ֱ�ƶ�����
	void CurveVerMove(TestResultInfo* pData, int nDelta, bool bSpin = false/*�Ƿ���ͨ����ť����*/);
	//�¼�ѡ���л�
	void EventSelectUp();
	void EventSelectDown();
	BOOL IsLineABSeleced(CPoint point);

	virtual void SetPageTitle();
	virtual void SetCurPageText();

	//���õ�ǰ��������ģʽ 0�����ţ�1��ƽ��
	void SetCurCurveZoomMode(int nCurveZoomMode);
	void CalDrawCurveParams(int curveWidth,CRect rectCurve,int dataCount);
	TestResultInfo* GetMaxLengthCurveData();//��ȡ��������������
	int GetZoomCount(int maxDistanceDataCount);////�õ��������Ŵ���wcq 2011.09.20
	int GetLastZoomCount(int countPerTime,int dataCount,int zoomCount);////�õ������������һ�����ŵ����ݵ����wcq 2011.09.21
	void GetLastZoomCounts();////�õ����������������һ�����ŵ����ݵ����wcq 2011.09.23
	//int GetLastZoomCount(int countPerTime,int dataCount,int zoomCount);////�õ��ڼ�������wcq 2011.09.21
	void CalFullPointCount();//������������δ����ʱ������ͼ����ĵ��� wcq 2011.09.21
	void ReCalZoomParas();//��һ�λ��и��������������ݵ��������¼���������ز��������������GetZoomCount,CalFullPointCount,GetLastZoomCount.wcq2011.09.23
	void SetLowerStandard(TestResultInfo* pData);//����CCurveData�е�IsLowerStandard�������Է���������ֱ��������.wcq2011.09.26
	BOOL IsLowerStandardAll();//�鲼�����������ݣ����Ƿ�������һ��IsLowerStandardΪ��.wcq2011.09.26
	void CalVerticalRange(int zoomNum,float fStandPosDb);//������ֱ��������ʱ���ֵ��Сֵ.����m_fStartDB,m_fMaxDB.wcq2011.09.26
	void MoveEventList(INT nIndex);//ѡ�����¼��б�����»�������;wcq2011-11.15
	void InvalidateAll();//ˢ�»�ͼ������ͼ����������ʾ��
	void GetDataCurve(TestResultInfo* pData);//��CurveData�е� pDataCurve������ֵ���õ��������Ļ�����������wcq2011.11.23
	int  GetLEvent(int realPulseWidth);//��������õ�LEvent(�¼��б��㷨��һ������).2011.12.06
	void ResetYOffset();////��ƽ�Ƶ�����ʱ����ֱ�����ƽ�ƻָ�Ϊ0;wcq2011.12.14
	void ClearAllCurveData();//��������������ݣ�����DataPointsNumΪ0.//wqc2011.12.19
	void OnVKReturn();////���»س������߻ص�δ����/ƽ��״̬ wcq2012.01.12
	void NewTestInit(TestResultInfo* pData);//���²��Ժ��ʼ��һЩ��������A/B��λ�� wcq2012.02.21
	void StartTest(int iTestMode);//AVG/REAL time���Թ�ͬ��Ӧ�¼� wcq2012.02.21
	//��ͼ���wcq2011.12.09
	void GetCursorRect(CRect *rect);//����ƶ�ʱ��Ҫ�ػ�����//wcq2011.12.26
	UINT DealData();// wcq2012.02.07
	void DealDataDetail();// wcq2012.02.07

	void SetHorScroll(); //����ˮƽ������
	void SetVerScoll(); //������ֱ������
	//����dll�õ����ݺ�ת��Ϊ�¼��б�
	void GaborResultToEvent(TestResultInfo* pCurveData);
	//���ļ���ȡ�¼��б��ת��Ϊʹ�õ�GaborResult�ṹ�� wcq2012.06.11
	void EventToGaborResult(TestResultInfo* pCurveData);

public:
	// ʵ��
protected:

public:

	CRect m_rectCurve; //�������ߵľ�������
	CRect m_rectCoord; //���������ھ�������
	CRect m_rectSubInfo; //AB������ʾ����wcq2011.11.16
	long int m_iStartLocation; //���λ�ã���λΪ(m)
	long int m_iMaxLocation; //ĩ��λ�ã���λ(m)
	unsigned int m_nStartPoint;  //���ߺ���������
	unsigned int m_nMaxPoint;    //���ߺ�������������ֵ
	CPoint *m_ptRecord; //��ͼ�����
	float m_fStartDB;    //������������ʼdb
	float m_fMaxDB;      //�������������ֵ
	float m_fFrequency;  //Ƶ��
	CArray<double,double > m_CArraySampleLoss;//�洢���紫�����Ĳ��������
	BOOL m_bReceiveResultOK; //�����������

	int m_nSelectCursor; //Seleced��־,1ѡ��A��2ѡ��B, 4ѡ��AB
	BOOL m_bEventCursor; //�Ƿ�Ϊ�¼����
	int m_nPointA;//A�ߵ�λ�õ㣬������ ������Ϊ�޷�����,  //ʼ��������������Ϊ׼wcq
	int m_nPointB;//B�ߵ�λ�õ㣬������  //ʼ��������������Ϊ׼wcq
	int m_point_minus;//AB��֮�����Ծ��룬zll 2013.1.6
	int m_nPointEvent; //��ǰ�¼��㣬 ������
	int m_nSelectIndex; //ѡ�е��¼�������


	int m_iHorizonZoomNum; //ˮƽ�Ŵ����
	int m_iVerticalZoomNum; //��ֱ�Ŵ����

	BOOL m_bReadFromFile; //���ļ���ȡ������
	unsigned int m_nStartPointFromFile;  //���ߺ���������
	unsigned int m_nMaxPointFromFile;    //���ߺ�������������ֵ
	///CArray<double,double > m_CArraySampleLossFromFile;//�洢�ļ���ȡ�Ĳ��������//wcq2012.03.09
	//	int m_nEventNumFromFile; //���ļ���ȡ���¼�����
	CArray<EventPtInfo,EventPtInfo> m_CArrayEventInfoFromFile; //���ļ���ȡ���¼���Ϣ
	float m_fRefraction;  //������
	unsigned long int	m_iEventNum; //�¼���Ŀ
	CArray<EventPtInfo,EventPtInfo> m_CArrayEventInfoFromNode; //��������յ��¼�����

	int m_iCurveZoomMode; //��������ģʽ 0�����ţ�1��ƽ��
	int m_iTestModeForFile; //�ļ����ߵĲ���ģʽ
	int m_iCtrlTime; //���ƽ�����ʱ��


	//dwssstesting
	DWORD m_dwWaveTesting;//��ǰ���ڲ��ԵĲ��� ����Ϊ1310��1550 wcq 2011.09.14
	CUtil m_util;//wcq 2011.09.14
	BOOL m_bBusy;//�Ƿ��յ���Ϣ�����ڴ������� wcq 2011.09.15
	DWORD m_dwMaxPointCount;//��������������,�����ڻ�������ʱʹ�� wcq 2011.09.15
	DWORD m_dwWaveSelected;//ѡ������������ wcq 2011.09.16
	int m_nZoomCount;//���Ŵ���
	//���ڲ��Ե���һ�׶�,���ݸñ���������ʾ��Ϣ �й��� ����״̬(�м�) �������� ���ڲ���/����״̬ (����״̬��ʵʱ����ʱ���������)
	COLORREF m_clrCurrent; //�м��������ɫ//wcq2011.12.07
	//CRect m_rectMoveThumb; //��������ƽ��ʱȷ��ƽ�ƽ���λ��//2011.12.14
	int m_nWaveToDevice;//��ǰ����ʱҪ�����豸�Ĳ���,˫����ƽ������ʱ�Ȳ���1550���ٲ�1310.//2011.12.19
	BOOL m_nStopTestNormal;//�Ƿ�����˳��������ԣ�˫����ʱ�Ȳ���1550���ٲ�1310���ڼ��п��������°��²��Ե��½���//2011.12.19
	BOOL m_bRedrawByCursor;//�ػ�ԭ�������ڹ���ƶ�.wcq2011.12.23
	int m_nDataLenMsg;//��Ϣ�а��������ݸ���wcq2012.02.07
	INT m_lastPageType;//��ǰ��ҳ�����ͣ����ڰ��¡����߶Աȡ��˳��󷵻ص���ǰ���档wcq2012.02.14
	/////�Ƿ���Ҫ���¼���A/Bλ��////////////////////////////////////////////////////////////
	BOOL m_bNeedHorizonZoom;//�Ƿ���Ҫ�������ţ������������������� wcq2012.02.17
	INT m_lastWaveLength;//��һ�β��� wcq2012.02.20
	INT m_lastDataLength;//��һ�β��Ե��� wcq2012.02.20
	float m_lastRefraction;//��һ�β��Ե������� wcq2012.02.20
	/////////////////////////////////////////////////////////////////
	BOOL m_bFirstRead;//���β����Ƿ��һ���յ����� wcq2012.02.21
	int m_nMaxIndex;

	MEMORYSTATUS buffer;//����ʱ��������ڴ��õ�wcq

public:
	struct LastTestInfo//wcq2012.02.21
	{//��Ҫ��¼���ϴβ���ʱ��һЩ����
		LastTestInfo()
		{
			m_distanceA = 0;
			m_distanceB = 0;
		}
		FLOAT m_distanceA;//A����ԭ�����
		FLOAT m_distanceB;//B����ԭ�����
	}m_lastTestInfo;
	struct CurTestParams//wcq2012.02.21
	{//��ǰ����(�Ӱ���"start"������"stop")ʱ�Ĳ���
		INT nSecondTestTime; //����ʱ��
		INT nTestMode; //����ģʽ��0Ϊ�ֶ���1Ϊ�Զ�
		//DWORD dwDataType;//�������ͣ�5858
		DWORD dwWaveLength;//�������粨��Ϊ1310����ʾΪ����1310��
		DWORD dwPulseWidth;//����������20ns ����ʾΪ����20���Զ����ò���50000��
		DWORD dwDistance;//���Ծ��룺����Ծ���300m����ʾΪ300������Ծ���200KM����ʾΪ200000���Զ����ò���500000��
		//DWORD dwModel;//����ģʽ��ƽ������2222��ʵʱ����ģʽ5555�����ò���4444���й���1111��
		DWORD dwLimit;//ĩ�����ޣ�Ӧ������й���ĩ�����޵�ֵ��
		FLOAT fReflect1310;//1310�����ʡ�
		FLOAT fReflect1550;//�����ʡ�
	}m_curTestParas;

	int m_tempInt;
	float m_fCoeff; //��ֱ����ϵ��
	
	// actual scroll position
	int m_nScrollPosVer;
	int m_nScrollPosHor;
	// actual dialog height
	int m_nCurHeight;
	int m_nDrawHeight; //��ͼ����߶�(�������߶�)
	// actual dialog width
	int m_nCurWidth;
	int m_nDrawWidth; //��ͼ������
	BOOL m_bZoomOK; //�϶�������ʱ���û��ѡ��AB��ֵΪ�棬��ʾ��ѡ������Ŵ󣬷���Ϊ�٣���ʾû�зŴ�����
private:
	///CString m_strCurveNameFromFile; //���߶Աȣ����ڴ��豸��ѡȡ���ļ�//wcq2012.03.09
	//struct TestResultInfo Result;
	///int CountLen;//wcq2012.03.09
	WORD* m_pRecvbuf; //�������ݻ�����
	///int recvlen; //�������ݳ���//wcq2012.03.09
	//��ʼƽ��ʱ��
	DWORD m_dwStartTestTime;
	///int m_nCount;//wcq2012.03.09
	POINT m_cursorPoint;
	CPoint m_PLMouseDown;//���������µ�λ��;
	CRect m_RectSelected;//��ʶѡ�еķŴ�����

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnDataReady(WPARAM wParam,LPARAM lParam);//wcq2011.08.11 �յ�������Ϣ�����ж�ȡ

	afx_msg LRESULT OnDataParamsConfig(WPARAM wParam,LPARAM lParam);//wcq2011.12.02 �յ���������������Ϣ

	afx_msg void OnClickListEvent(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
public:
	CEndGabor m_endgabor;//zll 2013.8.19
};
