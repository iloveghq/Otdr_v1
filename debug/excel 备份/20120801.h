#include <math.h>
#define Avg_loss_1310  0.325
#define Avg_loss_1550  0.182
#define Avg_loss_1625  0.192
#define Return_loss_1310_tybe 79.5
#define Return_loss_1550_tybe 81
#define Return_loss_1625_tybe 80.5

Gabor_Result K_Line_count(int puse_point,int wavelength,int pulsewidth,WORD input_data[],Gabor_Result sGabor_Result,float Index_N,float Sample_freaq);
Gabor_Result Acount_Loss(int wavelength,int pulsewidth,WORD input_data[],Gabor_Result sGabor_Result,float Index_N,float Sample_freaq);
Gabor_Result Acount_Total_Loss(int wavelength,int pulsewidth,WORD input_data[],Gabor_Result sGabor_Result,float Index_N,float Sample_freaq);
Gabor_Result Acount_return_loss(int wavelength,int pulsewidth,WORD input_data[],Gabor_Result sGabor_Result,float Index_N,float Sample_freaq,float Return_loss_Tybe);
Gabor_Result K_Line_limite(int wavelength,Gabor_Result sGabor_Result);
//******************************************************************//
//�汾��//
CString Version_number();
int lEventGenerate(int pulseNum,int wavelength,float Sample_freaq);
//******************************************************************//
//     �¼�����
//     L_event:�����¼�������������Ͳ����ֱ����й�ϵ
//     wavelength:���߲���
//     input_data[]��������ԭʼ����
//     paramLen:ԭʼ���ݳ���
//     thresholdUpSet:��ʱ����Ҷ�仯��ֵ����
//     thresholdDownSet:��ʱ����Ҷ�仯��ֵ����
//     dB_Loss:˥������
//     dB_reflex:��������
//     dB_End:ĩ���ж�����
//******************************************************************//
Gabor_Result gabor_generate(int pulsewidth,int wavelength,WORD input_data[],int paramLen,double thresholdUpSet,double thresholdDownSet,float dB_Loss,float dB_reflex,float dB_End,float Sample_freaq,float Index_N,int link_point_number);
/***********************************************************************/
///pulseWidth������
///wavelength:����ֵ��0:1310,1:1550
///input_data[]:��������
///paramLen���������ݳ���
//double thresholdUpSet:����Ҷ�仯��ֵ���ޡ�Ĭ��Ϊ2.08
//double thresholdDownSet����Ҷ�仯��ֵ����.Ĭ��Ϊ2.08
///dB_Loss:˥���ж�����
///dB_reflex�������ж�����
///dB_End��ĩ���ж�����Ĭ��Ϊ3
///Return_loss_Tybe������ɢ��ϵ��
// Sample_freaq:����Ƶ��
/**********************************************************************/
Gabor_Result gabor_arithmetic(int wavelength,int pulseWidth,WORD input_data[],INT paramLen,double thresholdUpSet,double thresholdDownSet,float dB_Loss,float dB_reflex,float dB_End,float Return_loss_Tybe,float Sample_freaq,float Index_N,int link_point_number);
//******************************************************************//
//     ����¼�
//     insertNum:��Ҫ������¼�������ݵ���
//     wavelength:���߲���
//     pulsewidth����������
//     input_data[]��������ԭʼ����
//     Return_loss_Tybe:����ɢ��ϵ��
//     paramLen:ԭʼ���ݳ���
//******************************************************************//
Gabor_Result insertEvent(int insertNum,int wavelength,int pulsewidth,WORD input_data[],int paramLen,Gabor_Result sGabor_Result,float Index_N,float Sample_freaq,float Return_loss_Tybe);
//******************************************************************//
//     ɾ���¼�
//     deleteNum:��ɾ���¼������
//     wavelength:���߲���
//     pulsewidth����������
//     input_data[]��������ԭʼ����
//******************************************************************//
Gabor_Result deleteEvent(int deleteNum,int wavelength,int pulsewidth,WORD input_data[],Gabor_Result sGabor_Result,float Index_N,float Sample_freaq,float Return_loss_Tybe);
//���ڹ��˹�դ���¼���������ÿһ���¼���λ�ö�λΪ����
Gabor_Result Optical_grating(int wavelength,int pulsewidth,WORD input_data[],INT paramLen,Gabor_Result sGabor_Result,float Index_N,float Sample_freaq,float dB_reflex);