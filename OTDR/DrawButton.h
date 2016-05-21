#pragma once
#include "afxwin.h"
#include "PrintView.h"

class CDrawButton : public CButton
{
public:
	CDrawButton(void);
	~CDrawButton(void);
	void Draw(unsigned int nStep, unsigned int nCurveNumber);//���ⲿ����
	void SetDrawMode(BOOL bReportDraw); //���û���ģʽ��TRUE���Ʊ���FALSE���Ʋ���
	void Setlayout(BOOL bHorLayout); //���ô�ӡ���з�ʽ
	void SetListFormat(int nListFormat); //0:A-1,1:A-2,2:A-3,3:B-1
	void ReDrawRect(int nStep/*��ǰ����*/); //�ػ������
	void OutPutBmp();
	void SaveCurveToLocal(int nCurveIndex);//��trace
	void DeleteBmpFiles(); //ɾ��λͼ�ļ�

private:
	void DrawReport(CDC* pDC);
	void Draw1Curve(CDC* pDC); //ÿҳ1������
	void Draw1CurveTitle(CDC* pDC); //�����α������
	void Draw2Curve(CDC* pDC); //ÿҳ2������
	void Draw2CurveTitle(CDC* pDC); //�����α������
	void Draw4Curve(CDC* pDC); //ÿҳ4������
	void Draw4CurveTitle(CDC* pDC); //�����α������
	void Draw6Curve(CDC* pDC); //ÿҳ6������
	void Draw6CurveTitle(CDC* pDC); //�����α������
	void Draw8Curve(CDC* pDC); //ÿҳ8������
	void Draw8CurveTitle(CDC* pDC); //�����α������
	void InnerDraw(CDC* pDC); //�ڲ���ͼֻ��
	void DrawCurveTitleRect(CDC* pDC); //���Ʊ����
	void DrawLittlesCurveTitleRect(CDC* pDC); //����С�����
	void DrawRptTitleRect(CDC* pDC); //�����ܱ�����
	void DrawReportFailRect(CDC* pDC); //�����ܱ�ϸ񲻺ϸ��
	void DrawReportRptRect(CDC* pDC); //�����ܱ��ܽ���
	void DrawCurve(CDC* pDC, CRect& rect, int nCurveIndex); //��������

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

public:
	CRect m_rectDraw;//���ƾ��������С
	BOOL m_bReportDraw;//�Ƿ�Ϊ�����ܱ�TRUE�ǣ�FALSE�����Ʋ���
	unsigned int m_nStep; //��ǰ����
	unsigned int m_nCurveNumber; //ÿҳ���θ���
	unsigned int m_nListFormat; //�б��ʽ
	BOOL m_bHorLayout; //��ӡ���ַ�ʽ���Ƿ�Ϊ����
public:
	CPrintView m_PrintView;
};
