// PageTestSetup.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OTDR.h"
#include "PageTestSetup.h"
#include "RemotePageSheet.h"
#include "ClientSocket.h"

extern SorFileArray g_sorFileArray;
CString str_net_connect1=_T("");

// CPageTestSetup �Ի���

IMPLEMENT_DYNAMIC(CPageTestSetup, CPropertyPage)

CPageTestSetup::CPageTestSetup()
	: CPropertyPage(CPageTestSetup::IDD)
{

}

CPageTestSetup::~CPageTestSetup()
{
}

void CPageTestSetup::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MEASURE_TIME, m_editTestTime);
	DDX_Control(pDX, IDC_SPIN_MEASURE_TIME, m_spinTestTime);
}


BEGIN_MESSAGE_MAP(CPageTestSetup, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_OTDR2PC, &CPageTestSetup::OnBnClickedBtnOtdr2pc)
	ON_BN_CLICKED(IDC_BTN_PC2OTDR, &CPageTestSetup::OnBnClickedBtnPc2otdr)
END_MESSAGE_MAP()


// CPageTestSetup ��Ϣ�������

BOOL CPageTestSetup::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	SetLocaleString();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CPageTestSetup::SetLocaleString()
{
	/*                           ���Բ���                                     */
	CString strWave = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_TEST_WAVE"));
	GetDlgItem(IDC_STATIC_TEST_WAVE)->SetWindowText(strWave);

	//˫����
	CString strWaveDual = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_TEST_WAVE_DUAL"));
	CButton *pBtn2Wave = (CButton*)GetDlgItem(IDC_RADIO_WAVE_DOUBLE);
	pBtn2Wave->SetWindowText(strWaveDual);
	CButton *pBtn1310Wave = (CButton*)GetDlgItem(IDC_RADIO_WAVE_1310);
	CButton *pBtn1550Wave = (CButton*)GetDlgItem(IDC_RADIO_WAVE_1550);
	if ((g_sorFileArray.otdrConfig.strTestWave.CompareNoCase(_T("1310")) == 0))
	{
		pBtn1310Wave->SendMessage(BM_CLICK, 0, 0);
	}
	else if ((g_sorFileArray.otdrConfig.strTestWave.CompareNoCase(_T("1550")) == 0))
	{
		pBtn1550Wave->SendMessage(BM_CLICK, 0, 0);
	}
	else if ((g_sorFileArray.otdrConfig.strTestWave.CompareNoCase(_T("2000")) == 0))
	{
		pBtn2Wave->SendMessage(BM_CLICK, 0, 0);
	}

	/*                                        �ֱ���                                    */
	CString strResolution = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_TEST_RESOLUTION"));
	GetDlgItem(IDC_STATIC_TEST_RESOLUTION)->SetWindowText(strResolution);
	//��׼
	CString strNoraml = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_TEST_RESOLUTION_NORMAL"));
	CButton *pBtnStandard = (CButton*)GetDlgItem(IDC_RADIO_RESOLUTION_STANDARD);
	pBtnStandard->SetWindowText(strNoraml);
	//��
	CString strHigh = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_TEST_RESOLUTION_HIGH"));
	CButton *pBtnHigh = (CButton*)GetDlgItem(IDC_RADIO_RESOLUTION_HIGH);
	pBtnHigh->SetWindowText(strHigh);
	if (g_sorFileArray.otdrConfig.strResolution.CompareNoCase(_T("0")) == 0)
	{
		pBtnStandard->SendMessage(BM_CLICK, 0, 0);
	}
	else 
	{
		pBtnHigh->SendMessage(BM_CLICK, 0, 0);
	}
	/*                                        �Զ�����                                    */
	CString strAutoSave = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_TEST_AUTOSAVE"));
	GetDlgItem(IDC_STATIC_AUTO_SAVE)->SetWindowText(strAutoSave);
	//��
	CString strSaveYes = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_TEST_AUTOSAVE_YES"));
	CButton *pBtnSaveYes = (CButton*)GetDlgItem(IDC_RADIO_AUTO_SAVE_YES);
	pBtnSaveYes->SetWindowText(strSaveYes);
	//��
	CString strSaveNo = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_TEST_AUTOSAVE_NO"));
	CButton *pBtnSaveNo = (CButton*)GetDlgItem(IDC_RADIO_AUTO_SAVE_NO);
	pBtnSaveNo->SetWindowText(strSaveNo);
	if (g_sorFileArray.otdrConfig.strAutoSave.CompareNoCase(_T("1")) == 0)
	{
		pBtnSaveYes->SendMessage(BM_CLICK, 0, 0);
	}
	else 
	{
		pBtnSaveNo->SendMessage(BM_CLICK, 0, 0);
	}
	/*                                   ����ʱ��                                      */
	CString strTime = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_TEST_TIME"));
	GetDlgItem(IDC_STATIC_TEST_TIME)->SetWindowText(strTime);
	int nTestTime = _wtoi(g_sorFileArray.otdrConfig.strTestTime);
	m_spinTestTime.SetSpin(0,0,100,1);
	m_spinTestTime.SetPos(nTestTime);

	/*                         ����ģʽ                                                */
	CString strTestMode = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_TEST_MODE"));
	GetDlgItem(IDC_STATIC_TEST_MODE)->SetWindowText(strTestMode);
	//�ֶ�
	CString strTestModeManul = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_TEST_MODE_MANUL"));
	CButton *pBtnManual = (CButton*)GetDlgItem(IDC_RADIO_MODE_MANUAL);
	pBtnManual->SetWindowText(strTestModeManul);
	//�Զ�
	CString strTestModeAuto = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_TEST_MODE_AUTO"));
	CButton *pBtnAuto = (CButton*)GetDlgItem(IDC_RADIO_MODE_AUTO);
	pBtnAuto->SetWindowText(strTestModeAuto);
	if ((g_sorFileArray.otdrConfig.strTestMode.CompareNoCase(_T("0")) == 0))
	{
		pBtnManual->SendMessage(BM_CLICK, 0, 0);
	}
	else if ((g_sorFileArray.otdrConfig.strTestMode.CompareNoCase(_T("1")) == 0))
	{
		pBtnAuto->SendMessage(BM_CLICK, 0, 0);
	}
	/*                                       ������Χ                             */
	CString strRange = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_TEST_RANGE"));
	GetDlgItem(IDC_STATIC_TEST_RANGE)->SetWindowText(strRange);
	CButton *pBtn500m= (CButton*)GetDlgItem(IDC_RADIO_SCOPE_500M);
	CButton *pBtn2km= (CButton*)GetDlgItem(IDC_RADIO_SCOPE_2KM);
	CButton *pBtn5km= (CButton*)GetDlgItem(IDC_RADIO_SCOPE_5KM);
	CButton *pBtn10km= (CButton*)GetDlgItem(IDC_RADIO_SCOPE_10KM);
	CButton *pBtn20km= (CButton*)GetDlgItem(IDC_RADIO_SCOPE_20KM);
	CButton *pBtn40km= (CButton*)GetDlgItem(IDC_RADIO_SCOPE_40KM);
	CButton *pBtn80km= (CButton*)GetDlgItem(IDC_RADIO_SCOPE_80KM);
	CButton *pBtn120km= (CButton*)GetDlgItem(IDC_RADIO_SCOPE_120KM);
	CButton *pBtn160km= (CButton*)GetDlgItem(IDC_RADIO_SCOPE_160KM);
	//ä������
	CString strBlink= IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_TEST_RANGE_BLINK"));
	CButton *pBtnBlink = (CButton*)GetDlgItem(IDC_RADIO_RANGE_BLINK);
	pBtnBlink->SetWindowText(strBlink);
	//�Զ�����
	CString strRangeAuto = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_TEST_RANGE_AUTO"));
	CButton *pBtnRangeAuto= (CButton*)GetDlgItem(IDC_RADIO_RANGE_AUTO);
	pBtnRangeAuto->SetWindowText(strRangeAuto);
	if ((g_sorFileArray.otdrConfig.strTestRange.CompareNoCase(_T("500")) == 0))
	{
		pBtn500m->SendMessage(BM_CLICK, 0, 0);
	}
	else if ((g_sorFileArray.otdrConfig.strTestRange.CompareNoCase(_T("2000")) == 0))
	{
		pBtn2km->SendMessage(BM_CLICK, 0, 0);
	}
	else if ((g_sorFileArray.otdrConfig.strTestRange.CompareNoCase(_T("5000")) == 0))
	{
		pBtn5km->SendMessage(BM_CLICK, 0, 0);
	}
	else if ((g_sorFileArray.otdrConfig.strTestRange.CompareNoCase(_T("10000")) == 0))
	{
		pBtn10km->SendMessage(BM_CLICK, 0, 0);
	}
	else if ((g_sorFileArray.otdrConfig.strTestRange.CompareNoCase(_T("20000")) == 0))
	{
		pBtn20km->SendMessage(BM_CLICK, 0, 0);
	}
	else if ((g_sorFileArray.otdrConfig.strTestRange.CompareNoCase(_T("40000")) == 0))
	{
		pBtn40km->SendMessage(BM_CLICK, 0, 0);
	}
	else if ((g_sorFileArray.otdrConfig.strTestRange.CompareNoCase(_T("80000")) == 0))
	{
		pBtn80km->SendMessage(BM_CLICK, 0, 0);
	}
	else if ((g_sorFileArray.otdrConfig.strTestRange.CompareNoCase(_T("120000")) == 0))
	{
		pBtn120km->SendMessage(BM_CLICK, 0, 0);
	}
	else if ((g_sorFileArray.otdrConfig.strTestMode.CompareNoCase(_T("160000")) == 0))
	{
		pBtn160km->SendMessage(BM_CLICK, 0, 0);
	}
	else if ((g_sorFileArray.otdrConfig.strTestRange.CompareNoCase(_T("50000")) == 0))
	{
		pBtnBlink->SendMessage(BM_CLICK, 0, 0);
	}
	else if ((g_sorFileArray.otdrConfig.strTestRange.CompareNoCase(_T("10")) == 0))
	{
		pBtnRangeAuto->SendMessage(BM_CLICK, 0, 0);
	}
	/*                                    ��λ                                 */
	CString strUnit = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_TEST_UNIT"));
	GetDlgItem(IDC_STATIC_TEST_UNIT)->SetWindowText(strUnit);
	CButton *pBtnUnitKm= (CButton*)GetDlgItem(IDC_RADIO_UNIT_KM);
	CButton *pBtnUnitKfeet= (CButton*)GetDlgItem(IDC_RADIO_UNIT_KFEET);
	CButton *pBtnUnitMiles= (CButton*)GetDlgItem(IDC_RADIO_UNIT_MILES);
	if ((g_sorFileArray.otdrConfig.strUnit.CompareNoCase(_T("m"))) == 0)
	{
		pBtnUnitKm->SendMessage(BM_CLICK, 0, 0);
	}
	else if ((g_sorFileArray.otdrConfig.strUnit.CompareNoCase(_T("ft"))) == 0)
	{
		pBtnUnitKfeet->SendMessage(BM_CLICK, 0, 0);
	}
	else if ((g_sorFileArray.otdrConfig.strUnit.CompareNoCase(_T("mi"))) == 0)
	{
		pBtnUnitMiles->SendMessage(BM_CLICK, 0, 0);
	}

	//����ѡ��
	CString strPulseWidth = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_TEST_PULSE_WIDTH"));
	CButton *pBtnPulseWidth= (CButton*)GetDlgItem(IDC_STATIC_PULSE_WIDTH_SELECT);
	GetDlgItem(IDC_STATIC_PULSE_WIDTH_SELECT)->SetWindowText(strPulseWidth);

	//�Զ�����
	CString strPulseAuto = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_TEST_PULSE_WIDTH_AUTO"));
	CButton *pBtnPulseAuto= (CButton*)GetDlgItem(IDC_RADIO_WIDTH_AUTO);
	pBtnPulseAuto->SetWindowText(strPulseAuto);
	CButton *pBtn3ns= (CButton*)GetDlgItem(IDC_RADIO_WIDTH_3NS);
	CButton *pBtn5ns= (CButton*)GetDlgItem(IDC_RADIO_WIDTH_5NS);
	CButton *pBtn10ns= (CButton*)GetDlgItem(IDC_RADIO_WIDTH_10NS);
	CButton *pBtn20ns= (CButton*)GetDlgItem(IDC_RADIO_WIDTH_20NS);
	CButton *pBtn50ns= (CButton*)GetDlgItem(IDC_RADIO_WIDTH_50NS);
	CButton *pBtn100ns= (CButton*)GetDlgItem(IDC_RADIO_WIDTH_100NS);
	CButton *pBtn200ns= (CButton*)GetDlgItem(IDC_RADIO_WIDTH_200NS);
	CButton *pBtn500ns= (CButton*)GetDlgItem(IDC_RADIO_WIDTH_500NS);
	CButton *pBtn1000ns= (CButton*)GetDlgItem(IDC_RADIO_WIDTH_1000NS);
	CButton *pBtn2000ns= (CButton*)GetDlgItem(IDC_RADIO_WIDTH_2000NS);
	CButton *pBtn5000ns= (CButton*)GetDlgItem(IDC_RADIO_WIDTH_5000NS);
	CButton *pBtn10000ns= (CButton*)GetDlgItem(IDC_RADIO_WIDTH_10000NS);
	if ((g_sorFileArray.otdrConfig.strPluseWidth.CompareNoCase(_T("3"))) == 0)
	{
		pBtn3ns->SendMessage(BM_CLICK, 0, 0);
	}
	else if ((g_sorFileArray.otdrConfig.strPluseWidth.CompareNoCase(_T("5"))) == 0)
	{
		pBtn5ns->SendMessage(BM_CLICK, 0, 0);
	}
	else if ((g_sorFileArray.otdrConfig.strPluseWidth.CompareNoCase(_T("10"))) == 0)
	{
		pBtn10ns->SendMessage(BM_CLICK, 0, 0);
	}
	else if ((g_sorFileArray.otdrConfig.strPluseWidth.CompareNoCase(_T("20"))) == 0)
	{
		pBtn20ns->SendMessage(BM_CLICK, 0, 0);
	}
	else if ((g_sorFileArray.otdrConfig.strPluseWidth.CompareNoCase(_T("50"))) == 0)
	{
		pBtn50ns->SendMessage(BM_CLICK, 0, 0);
	}
	else if ((g_sorFileArray.otdrConfig.strPluseWidth.CompareNoCase(_T("100"))) == 0)
	{
		pBtn100ns->SendMessage(BM_CLICK, 0, 0);
	}
	else if ((g_sorFileArray.otdrConfig.strPluseWidth.CompareNoCase(_T("200"))) == 0)
	{
		pBtn200ns->SendMessage(BM_CLICK, 0, 0);
	}
	else if ((g_sorFileArray.otdrConfig.strPluseWidth.CompareNoCase(_T("500"))) == 0)
	{
		pBtn500ns->SendMessage(BM_CLICK, 0, 0);
	}
	else if ((g_sorFileArray.otdrConfig.strPluseWidth.CompareNoCase(_T("1000"))) == 0)
	{
		pBtn1000ns->SendMessage(BM_CLICK, 0, 0);
	}
	else if ((g_sorFileArray.otdrConfig.strPluseWidth.CompareNoCase(_T("2000"))) == 0)
	{
		pBtn2000ns->SendMessage(BM_CLICK, 0, 0);
	}
	else if ((g_sorFileArray.otdrConfig.strPluseWidth.CompareNoCase(_T("5000"))) == 0)
	{
		pBtn5000ns->SendMessage(BM_CLICK, 0, 0);
	}
	else if ((g_sorFileArray.otdrConfig.strPluseWidth.CompareNoCase(_T("10000"))) == 0)
	{
		pBtn10000ns->SendMessage(BM_CLICK, 0, 0);
	}
	else if ((g_sorFileArray.otdrConfig.strPluseWidth.CompareNoCase(_T("50000"))) == 0)
	{
		pBtnPulseAuto->SendMessage(BM_CLICK, 0, 0);
	}
	str_net_connect1=IniUtil::ReadResString(_T("IDS_NET_CONNECT"));
}

void CPageTestSetup::RecordSetting()
{
	/*                ���Բ���                                       */
	CString strTestWave;
	CButton *pBtn1310Wave = (CButton*)GetDlgItem(IDC_RADIO_WAVE_1310);
	CButton *pBtn1550Wave = (CButton*)GetDlgItem(IDC_RADIO_WAVE_1550);
	CButton *pBtn2Wave = (CButton*)GetDlgItem(IDC_RADIO_WAVE_DOUBLE);
	if (pBtn1310Wave->GetCheck())
	{
		g_sorFileArray.otdrConfig.strTestWave = _T("1310");
	}
	else if (pBtn1550Wave->GetCheck())
	{
		g_sorFileArray.otdrConfig.strTestWave = _T("1550");
	}
	else if (pBtn2Wave->GetCheck())
	{
		g_sorFileArray.otdrConfig.strTestWave = _T("2000");
	}
	IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("TestWave"), g_sorFileArray.otdrConfig.strTestWave);
	/*                         ����ʱ��                                                */
	int nTestTime = m_spinTestTime.GetPos();
	g_sorFileArray.otdrConfig.strTestTime.Format(_T("%d"), nTestTime);
	IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("TestTime"), g_sorFileArray.otdrConfig.strTestTime);
	/*                         ����ģʽ                                                */
	CButton *pBtnManual = (CButton*)GetDlgItem(IDC_RADIO_MODE_MANUAL);//�ֶ�
	CButton *pBtnAuto = (CButton*)GetDlgItem(IDC_RADIO_MODE_AUTO);//�Զ�
	if (pBtnManual->GetCheck())
	{
		g_sorFileArray.otdrConfig.strTestMode = _T("0");
	}
	else if (pBtnAuto->GetCheck())
	{
		g_sorFileArray.otdrConfig.strTestMode = _T("1");
	}
	IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("TestMode"), g_sorFileArray.otdrConfig.strTestMode);
	/*                                     �Զ�����                                                            */
	CButton *pBtnSaveYes = (CButton*)GetDlgItem(IDC_RADIO_AUTO_SAVE_YES);
	//CButton *pBtnSaveNo = (CButton*)GetDlgItem(IDC_RADIO_AUTO_SAVE_NO);
	if (pBtnSaveYes->GetCheck())
	{
		g_sorFileArray.otdrConfig.strAutoSave = _T("1");
	}
	else 
	{
		g_sorFileArray.otdrConfig.strAutoSave = _T("0");
	}

	//�ֱ���
	CButton *pBtnResolutionStandard = (CButton*)GetDlgItem(IDC_RADIO_RESOLUTION_STANDARD);
	if (pBtnResolutionStandard->GetCheck())
	{
		g_sorFileArray.otdrConfig.strResolution = _T("0");
	}
	else 
	{
		g_sorFileArray.otdrConfig.strResolution = _T("1");
	}
	/*                                       ������Χ                             */
	CButton *pBtn500m= (CButton*)GetDlgItem(IDC_RADIO_SCOPE_500M);
	CButton *pBtn2km= (CButton*)GetDlgItem(IDC_RADIO_SCOPE_2KM);
	CButton *pBtn5km= (CButton*)GetDlgItem(IDC_RADIO_SCOPE_5KM);
	CButton *pBtn10km= (CButton*)GetDlgItem(IDC_RADIO_SCOPE_10KM);
	CButton *pBtn20km= (CButton*)GetDlgItem(IDC_RADIO_SCOPE_20KM);
	CButton *pBtn40km= (CButton*)GetDlgItem(IDC_RADIO_SCOPE_40KM);
	CButton *pBtn80km= (CButton*)GetDlgItem(IDC_RADIO_SCOPE_80KM);
	CButton *pBtn120km= (CButton*)GetDlgItem(IDC_RADIO_SCOPE_120KM);
	CButton *pBtn160km= (CButton*)GetDlgItem(IDC_RADIO_SCOPE_160KM);
	CButton *pBtnBlink = (CButton*)GetDlgItem(IDC_RADIO_RANGE_BLINK);//ä������
	CButton *pBtnRangeAuto= (CButton*)GetDlgItem(IDC_RADIO_RANGE_AUTO);//�Զ�����
	if (pBtn500m->GetCheck())
	{
		g_sorFileArray.otdrConfig.strTestRange = _T("500");
	}
	else if (pBtn2km->GetCheck())
	{
		g_sorFileArray.otdrConfig.strTestRange = _T("2000");
	}
	else if (pBtn5km->GetCheck())
	{
		g_sorFileArray.otdrConfig.strTestRange = _T("5000");
	}
	else if (pBtn10km->GetCheck())
	{
		g_sorFileArray.otdrConfig.strTestRange = _T("10000");
	}
	else if (pBtn20km->GetCheck())
	{
		g_sorFileArray.otdrConfig.strTestRange = _T("20000");
	}
	else if (pBtn40km->GetCheck())
	{
		g_sorFileArray.otdrConfig.strTestRange = _T("40000");
	}
	else if (pBtn80km->GetCheck())
	{
		g_sorFileArray.otdrConfig.strTestRange = _T("80000");
	}
	else if (pBtn120km->GetCheck())
	{
		g_sorFileArray.otdrConfig.strTestRange = _T("120000");
	}
	else if (pBtn160km->GetCheck())
	{
		g_sorFileArray.otdrConfig.strTestRange = _T("160000");
	}
	else if (pBtnBlink->GetCheck())
	{
		g_sorFileArray.otdrConfig.strTestRange = _T("100");
	}
	else if (pBtnRangeAuto->GetCheck())
	{
		g_sorFileArray.otdrConfig.strTestRange = _T("500000");
	}
	IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("strTestRange"), g_sorFileArray.otdrConfig.strTestRange);
	/*                                    ��λ                                 */
	CButton *pBtnUnitKm= (CButton*)GetDlgItem(IDC_RADIO_UNIT_KM);
	CButton *pBtnUnitKfeet= (CButton*)GetDlgItem(IDC_RADIO_UNIT_KFEET);
	CButton *pBtnUnitMiles= (CButton*)GetDlgItem(IDC_RADIO_UNIT_MILES);
	if (pBtnUnitKm->GetCheck())
	{
		g_sorFileArray.otdrConfig.strUnit = _T("m");
	}
	else if (pBtnUnitKfeet->GetCheck())
	{
		g_sorFileArray.otdrConfig.strUnit = _T("ft");
	}
	else if (pBtnUnitMiles->GetCheck())
	{
		g_sorFileArray.otdrConfig.strUnit = _T("mi");
	}
	IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("TestUnit"), g_sorFileArray.otdrConfig.strUnit);
	/*                                  ����ѡ��                                                 */
	CButton *pBtn3ns= (CButton*)GetDlgItem(IDC_RADIO_WIDTH_3NS);
	CButton *pBtn5ns= (CButton*)GetDlgItem(IDC_RADIO_WIDTH_5NS);
	CButton *pBtn10ns= (CButton*)GetDlgItem(IDC_RADIO_WIDTH_10NS);
	CButton *pBtn20ns= (CButton*)GetDlgItem(IDC_RADIO_WIDTH_20NS);
	CButton *pBtn50ns= (CButton*)GetDlgItem(IDC_RADIO_WIDTH_50NS);
	CButton *pBtn100ns= (CButton*)GetDlgItem(IDC_RADIO_WIDTH_100NS);
	CButton *pBtn200ns= (CButton*)GetDlgItem(IDC_RADIO_WIDTH_200NS);
	CButton *pBtn500ns= (CButton*)GetDlgItem(IDC_RADIO_WIDTH_500NS);
	CButton *pBtn1000ns= (CButton*)GetDlgItem(IDC_RADIO_WIDTH_1000NS);
	CButton *pBtn2000ns= (CButton*)GetDlgItem(IDC_RADIO_WIDTH_2000NS);
	CButton *pBtn5000ns= (CButton*)GetDlgItem(IDC_RADIO_WIDTH_5000NS);
	CButton *pBtn10000ns= (CButton*)GetDlgItem(IDC_RADIO_WIDTH_10000NS);
	CButton *pBtnPulseAuto= (CButton*)GetDlgItem(IDC_RADIO_WIDTH_AUTO);	//�Զ�����
	if (pBtn3ns->GetCheck())
	{
		g_sorFileArray.otdrConfig.strPluseWidth = _T("3");
	}
	else if (pBtn5ns->GetCheck())
	{
		g_sorFileArray.otdrConfig.strPluseWidth = _T("5");
	}
	else if (pBtn10ns->GetCheck())
	{
		g_sorFileArray.otdrConfig.strPluseWidth = _T("10");
	}
	else if (pBtn20ns->GetCheck())
	{
		g_sorFileArray.otdrConfig.strPluseWidth = _T("20");
	}
	else if (pBtn50ns->GetCheck())
	{
		g_sorFileArray.otdrConfig.strPluseWidth = _T("50");
	}
	else if (pBtn100ns->GetCheck())
	{
		g_sorFileArray.otdrConfig.strPluseWidth = _T("100");
	}
	else if (pBtn200ns->GetCheck())
	{
		g_sorFileArray.otdrConfig.strPluseWidth = _T("200");
	}
	else if (pBtn500ns->GetCheck())
	{
		g_sorFileArray.otdrConfig.strPluseWidth = _T("500");
	}
	else if (pBtn1000ns->GetCheck())
	{
		g_sorFileArray.otdrConfig.strPluseWidth = _T("1000");
	}
	else if (pBtn2000ns->GetCheck())
	{
		g_sorFileArray.otdrConfig.strPluseWidth = _T("2000");
	}
	else if (pBtn5000ns->GetCheck())
	{
		g_sorFileArray.otdrConfig.strPluseWidth = _T("5000");
	}
	else if (pBtn10000ns->GetCheck())
	{
		g_sorFileArray.otdrConfig.strPluseWidth = _T("10000");
	}
	else if (pBtnPulseAuto->GetCheck())
	{
		g_sorFileArray.otdrConfig.strPluseWidth = _T("50000");
	}
	IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("PluseWidth"), g_sorFileArray.otdrConfig.strPluseWidth);
}

void CPageTestSetup::OnBnClickedBtnOtdr2pc()
{
	FillGetMsg();
	GetConfigData();
}

void CPageTestSetup::OnBnClickedBtnPc2otdr()
{
	RecordSetting(); //��������
	FillSendMsg();
	SendConfigData();
}

void CPageTestSetup::FillGetMsg()
{
	m_strGetMsg.Empty(); //����ַ���
	//��ȡ��������
	m_strGetMsg = NET_TYPE_SETUP_GET;
}

void CPageTestSetup::FillSendMsg()//��䷢����Ϣ�ı�
{
	m_strSendMsg.Empty(); //����ַ���
	//����
	CString strCommand; //������������
	strCommand.Format(_T("2"));
	m_strSendMsg += strCommand;
	//����������
	CString strTestWave; //���Բ���
	strTestWave.Format(_T("TestWave=%s,"), g_sorFileArray.otdrConfig.strTestWave);
	m_strSendMsg += strTestWave;

	CString strTestTime; //����ʱ��
	strTestTime.Format(_T("TestTime=%s,"), g_sorFileArray.otdrConfig.strTestTime);
	m_strSendMsg += strTestTime;
	CString strTestMode; //����ģʽ��0Ϊ�ֶ���1Ϊ�Զ�
	strTestMode.Format(_T("TestMode=%s,"), g_sorFileArray.otdrConfig.strTestMode);
	m_strSendMsg += strTestMode;
	CString strTestRange; //������Χ
	strTestRange.Format(_T("TestRange=%s,"), g_sorFileArray.otdrConfig.strTestRange);
	m_strSendMsg += strTestRange;
	CString strUnit; //��λ
	strUnit.Format(_T("TestUnit=%s,"), g_sorFileArray.otdrConfig.strUnit);
	m_strSendMsg += strUnit;
	CString strPluseWidth; //����ѡ��
	strPluseWidth.Format(_T("PluseWidth=%s,"), g_sorFileArray.otdrConfig.strPluseWidth);
	m_strSendMsg += strPluseWidth;
	CString strResolution; //�ֱ���
	strResolution.Format(_T("Resolution=%s,"), g_sorFileArray.otdrConfig.strResolution);
	m_strSendMsg += strResolution;
	CString strAutoSave; //�Զ�����
	strAutoSave.Format(_T("AutoSave=%s,"), g_sorFileArray.otdrConfig.strAutoSave);
	m_strSendMsg += strAutoSave;
}

BOOL CPageTestSetup::OnApply()
{
	// TODO: �ڴ����ר�ô����/����û���
	RecordSetting();
	return CPropertyPage::OnApply();
}

void CPageTestSetup::SendConfigData()
{
	CClientSocket* pClientSocket = MAIN->m_pRemoteConnDlg->m_pClientSocket;
	if (!pClientSocket)
		return;
	int nSocketStatus = pClientSocket->GetSocketState();
	if (nSocketStatus <= CCESocket::CREATED)
	{
		MessageBox(str_net_connect1, _T("OTDR Viewer"), MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	pClientSocket->SendLine(m_strSendMsg);
}

void CPageTestSetup::GetConfigData()
{
	CClientSocket* pClientSocket = MAIN->m_pRemoteConnDlg->m_pClientSocket;
	if (!pClientSocket)
		return;
	int nSocketStatus = pClientSocket->GetSocketState();
	if (nSocketStatus <= CCESocket::CREATED)
	{
		MessageBox(str_net_connect1, _T("OTDR Viewer"), MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	//���ͻ�ȡ������Ϣ����
	pClientSocket->SendLine(m_strGetMsg);
}

