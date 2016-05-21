#pragma once

#include "Xml.h"
#include "ExcelUtil.h"
#include "GrandwayProgress.h"

// CPropStep6 �Ի���
class CSpreadSheet;
class CPropStep6 : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropStep6)

public:
	CPropStep6();
	virtual ~CPropStep6();
	void SetLocalString();
	static UINT ExportXlsThread(LPVOID lparam);
	void ExportToExcel();
	BOOL ExportXlsByRpt();
	int GetSheetIndex();
	void WriteToExcel(CSpreadSheet* excel);
	void WriteToExcel(const CString& fileName, int nSheetIndex,int SheetCount);
public:
// �Ի�������
	enum { IDD = IDD_PROP_STEP6 };

public:
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();
	virtual LRESULT OnWizardBack();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStep6Preview();
	afx_msg void OnBnClickedBtnStep6Setting();
	afx_msg void OnBnClickedBtnStep6Print();
	afx_msg void OnBnClickedBtnStep6Excel();

private:
	void ExportReport(); //�ܱ�
	void Export1Curve(); //ÿҳ1������
	void Export2Curve(); //ÿҳ2������
	void Export4Curve(); //ÿҳ4������
	void Export6Curve(); //ÿҳ6������
	void Export8Curve(); //ÿҳ8������
	void WriteDataToExcel(int nWaveNumber);//zll 2013.5.24 ��
	void WriteDataToExcel(int nWaveNumber, BOOL bNewRpt);//lzy 2014.8.22
	void WriteEventList(int m_nWaveNumberPerPage,int m_nCurCurve, int nRow = 33);//zll 2013.5.27 ����excel��ÿҳһ������ʱ��ʾ�¼��б�����
	void WriteEventHead(int m_nWaveNumberPerPage,int m_nCurCurve, int nRow = 32);//zll 2013.5.27 �� ��ʾ�¼��б��ͷ
	void SetHorUnitText(int m_nWaveNumberPerPage,int m_nCurCurve); //zll 2013.6.11 ���ú�����̶��ı�
	void SetVerUnitText(int m_nWaveNumberPerPage,int m_nCurCurve); //zll 2013.6.11 ����������̶��ı�
	void GetHorLocation(int m_nWaveNumberPerPage,int m_nCurCurve,CString m_strStartDis,CString m_strUnitDis,CString m_strEndDis);//zll 2013.6.11 �õ�������λ�ò����̶��ı�
	void SetSorName(int m_nWaveNumberPerPage,int m_nCurCurve,CString m_sorName);//zll 2013.6.11 ����.sor�����ļ���

private:
	CGrandwayProgress m_progress; //������
	CWinThread* m_pThread;
	CXml m_xml;
	CString m_strXmlFile;
	CExcelUtil m_ExcelUtil;

public:
	CString m_strStartDis; //�����꿪ʼֵ
	CString m_strEndDis; //���������ֵ
	CString m_strUnitDis; //�����굥λ����

	CString m_strUnitDiv; //�����굥λֵ
};
