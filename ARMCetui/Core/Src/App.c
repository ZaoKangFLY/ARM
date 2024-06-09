#include "App.h"

/*����ϵͳ��ʼ��*/
void system_init()
{
 #if YeHuoPID
 	int32_t target_location = 0;//�趨��ʼֵ
    set_computer_value(SEND_STOP_CMD, CURVES_CH1, NULL, 0);    // ͬ����λ����������ť״̬
	set_computer_value(SEND_STOP_CMD, CURVES_CH2, NULL, 0);    // ͬ����λ����������ť״̬
	set_computer_value(SEND_STOP_CMD, CURVES_CH3, NULL, 0);    // ͬ����λ����������ť״̬
	set_computer_value(SEND_STOP_CMD, CURVES_CH4, NULL, 0);    // ͬ����λ����������ť״̬
    set_computer_value(SEND_TARGET_CMD, CURVES_CH1, &target_location, 1);     // ��ͨ�� 1 ����Ŀ��ֵ
	set_computer_value(SEND_TARGET_CMD, CURVES_CH2, &target_location, 1);     // ��ͨ�� 2 ����Ŀ��ֵ
	set_computer_value(SEND_TARGET_CMD, CURVES_CH3, &target_location, 1);     // ��ͨ�� 3 ����Ŀ��ֵ
	set_computer_value(SEND_TARGET_CMD, CURVES_CH4, &target_location, 1);     // ��ͨ�� 4 ����Ŀ��ֵ
	tim_pwm_disable();
#else
	tim_pwm_enable();
#endif
	uart_enable();
    tim_basic_enable(); 
	tim_econder_enable();	
	/*��ʼ���ṹ��*/					 /*������ֵ,  �����������޷�,    p,i,d,      ������,����ֵ */
	PID_struct_init(&Pid_Jian,POSITION_PID, 500.0f,  1000.0f,200.0f,  0.1f,1.0f,0.0f, 10000.0f,1000.0f);
	PID_struct_init(&Pid_Big,POSITION_PID,  999.0f,  20000.0f,666.0f,  0.175f,0.7731f,0.0f, 10000.0f,2000.0f);//��ʼ�����PID�ṹ��
	PID_struct_init(&Pid_Small,POSITION_PID,999.0f,  4000.0f,666.0f,  0.17f,0.000077f,0.0f, 10000.0f,2000.0f);//��ʼ��С��PID�ṹ�� 
	PID_struct_init(&Pid_Wan,POSITION_PID,  500.0f,  1000.0f,2000.0f,  2.1f,1.0f,0.0f, 10000.0f,1000.0f);  
	
#if YeHuoPID
	float pid1_temp[3] = {Pid_Jian.p, Pid_Jian.i, Pid_Jian.d};
    set_computer_value(SEND_P_I_D_CMD, CURVES_CH1, pid1_temp, 3);     // ��ͨ�� 1���� P I D ֵ
    float pid2_temp[3] = {Pid_Big.p, Pid_Big.i, Pid_Big.d};
    set_computer_value(SEND_P_I_D_CMD, CURVES_CH2, pid2_temp, 3);     // ��ͨ�� 2���� P I D ֵ
	float pid3_temp[3] = {Pid_Small.p, Pid_Small.i, Pid_Small.d};
    set_computer_value(SEND_P_I_D_CMD, CURVES_CH3, pid3_temp, 3);     // ��ͨ�� 3 ���� P I D ֵ
	float pid4_temp[3] = {Pid_Wan.p, Pid_Wan.i, Pid_Wan.d};
    set_computer_value(SEND_P_I_D_CMD, CURVES_CH4, pid4_temp, 3);     // ��ͨ�� 4 ���� P I D ֵ
#endif	

    /*����12���ϵ��ʼ��*/
//    Ce1_SETCOMPARE(1000);//��ɫ��
//    Ce1_SETCOMPARE(1500);
//    Ce2_SETCOMPARE(2000);
//    Ce2_SETCOMPARE(1500);
//	HAL_Delay(2685);//��ʱ��������
//  /*����*/
//	Ce1_SETCOMPARE(1450);
//	Ce2_SETCOMPARE(1525);

	
#if CeShi   
	/*���Իش���PC104Ҫȥ��*/
    Usart_SendString((uint8_t *)"��ʼ����ɣ�\n");
#endif

}
