//���define������һЩȫ�ֳ���
//˵������ʽʹ��ʱɾ��MY_DEBUG_TRACE����??????????????????????????

//??????????????????????????????????????????????
//#define MY_DEBUG_TRACE 1//����ʱ����TRACEGһЩ�����ȣ���ʽʹ��ʱɾ��/ע��?wcq101130???????????????????????????	 
///#define MY_DEBUG_TO_LOG 1//�Ƿ�ͨ���ռ�����¼�������ʱ��״̬��������Ҫ��ע�͡�wcq20110801???????????????????????????	 

//��Ϣ��ʾ��
#define MESSAGE(info) MessageBox(info,_T("OTDR Viewer"),MB_OK|MB_ICONEXCLAMATION)//��ʾ�Ի���
#define MESSAGEYN(info) MessageBox(info,_T("OTDR Viewer"),MB_YESNO|MB_ICONQUESTION)//��ʾ�Ի���,�������Ƿ�IDYES/IDNO
#define MESSAGEOC(info) MessageBox(info,_T("OTDR Viewer"),MB_OKCANCEL|MB_ICONQUESTION)//��ʾ�Ի���,������ȷ��ȡ��IDOK/IDCANCEL
#define SET_CHECK(ID,bCheck) ((CButton*)this->GetDlgItem(ID))->SetCheck(bCheck)//check/radioѡ��
#define GET_CHECK(ID) ((CButton*)this->GetDlgItem(ID))->GetCheck()//check/radioѡ��
#define SET_TEXT(ID,STR) this->GetDlgItem(ID)->SetWindowTextW(STR)//check/radioѡ��
#define GET_TEXT(ID,STR) this->GetDlgItem(ID)->GetWindowTextW(STR)//check/radioѡ��
#define ENABLE_ID(ID,bEn) (this->GetDlgItem(ID)->EnableWindow(bEn))//check/radioѡ��
#define SHOW_ID(ID) (this->GetDlgItem(ID)->ShowWindow(SW_SHOW))//��ʾ
#define HIDE_ID(ID) (this->GetDlgItem(ID)->ShowWindow(SW_HIDE))//����

//
#define MAIN ((CMainFrame *)AfxGetMainWnd())//����
//
#define STRING_ROW _T("Low")//
//
#define DEFAULT_WAVEFORM_HEIGHT 490//���ߴ���Ĭ�ϸ߶�
#define DEFAULT_EVENTLIST_HEIGHT_BOTTOM 62//
#define DEFAULT_STATUSBAR_HEIGHT 19 //״̬���߶�
#define DEFAULT_EVENTLIST_DIST 62 //�¼��б���봰�ڵײ��߶�
#define DEFAULT_MAIN_SETTING_WIDTH 250 //�����ô��ڿ��

#define COOR_SPLIT_COUNT_H 10//������10�ȷ�//zll 2013.5.28
#define COOR_SPLIT_COUNT_V 8//������9�ȷ�//zll 2013.5.28 ��10�ȷָ�Ϊ9�ȷ�
#define MAX_CURVE_NUM 500//������8���������� //zll 2012.9.24 ���޸� �ɼ��ض�������
#define NUM_VALIDE_LEAST 100//��TestResultInfo�е����ݸ���<��ֵʱ�����ԣ������л�wcq2011.09.13

//����ߴ�
#define REPORT_HEADER_HEIGHT 45
#define REPORT_WIZARD_WIDTH 489
#define REPORT_WIZARD_HEIGHT 650//589 //zll 2012.10.26�޸�
#define REPORT_WIZARD_BOTTOM_HEIGHT -10
#define REPORT_WIZARD_LEFT_OFFSET -200

//������ͨѶ///////////////////////////////////////////////////////////////////////////////////////
// ���Ÿ��������
#define IOCTL_FRM_READ_VER		                0x04001060
#define IOCTL_FRM_READ_SERIAL			        0x04001061
#define IOCTL_FRM_TEST                          0x04001062
//��������ʱ��
#define IOCTL_FRM_WRITE_SERIAL			        0x08001061

///////////////////////////////////////////////////////////////////////////////
//��Ϣ
///////////////////////////////////////////////////////////////////////////////
#define OTDR_DATA_READY (WM_USER+1000)//�����Ѱ�����׼����
#define WAVEFROM_LEFT_LIST_FRESH (WM_USER+1001)//ˢ�½����б�

#define SOCK_ONRECEIVE (WM_USER+1002)//����Socket��Ϣ
#define SOCK_ONCLOSE (WM_USER+1003)//����Socket��Ϣ

#define SOCK_ONRECEIVE_DATA_READY (WM_USER+1004)//����Socket����

//����һЩ����
#define COLOR_WHITE RGB(255,255,255)//��ɫ
///#define COLOR_DARKGREEN RGB(192,192,192)//����ɫ
#define COLOR_BLACK RGB(0,0,0)//��ɫ
#define COLOR_RED RGB(255,0,0)//��ɫ
#define COLOR_PURPLE RGB(128,0,128)//��ɫ
#define COLOR_YELLOW RGB(255,255,0)//��ɫ����ť�ı���ɫ
#define COLOR_GOLD RGB(255,215,0)//��ɫ,ѡ����Ŀ��ɫ
#define COLOR_BLUEVIOLET RGB(138,43,266)//������������ɫ
#define COLOR_SLATEBLUE RGB(106,90,205)//���Ұ�����ɫ
#define COLOR_DARKGREEN RGB(0,100,0)//����ɫ
#define COLOR_LOWNEEN RGB(124,252,0)//��ƺ��,ѡ����Ŀ��ɫ

#define COLOR_CURVE COLOR_DARKGREEN//������ɫ
/*******************************************************************************/
/* Socket����                                                                     */
/*****************************************************************************/
#define NET_TYPE_SYSTEM _T("1")//����ͨѶ����ϵͳ���� wcq2012.05.18
#define NET_TYPE_SETUP _T("2")//����ͨѶ����������� wcq2012.05.18
#define NET_TYPE_SYSTEM_GET _T("get1")//����ͨѶ�����ȡ��λ��ϵͳ���� wcq2012.05.18
#define NET_TYPE_SETUP_GET _T("get2")//����ͨѶ�����ȡ��λ���������� wcq2012.05.18

/*******************************************************************************/
/* ��λ�������                                                                     */
/*****************************************************************************/
#define UNIT_SHOW_KM _T("m")//AB��λ�õ�ѡ����"Km"ʱ��ʾΪ"m"
#define UNIT_SHOW_KFEET _T("ft")//AB��λ�õ�ѡ����"kfeet"ʱ��ʾΪ"m"
#define UNIT_SHOW_MILE _T("mi")//AB��λ�õ�ѡ����"miles"ʱ��ʾΪ"m"

//����������
#define REPORT_RPT_PATH_REG _T("filepathrpt")//����λ��ע�������
#define REPORT_RPT_DEFAULT_PATH _T("\\default\\curve\\")//Ĭ�ϱ����ļ�·��