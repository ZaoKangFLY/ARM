#include "App.h"

/*����ϵͳ��ʼ��*/
void system_init()
{
 #if PID_ASSISTANT_EN
 	int32_t target_location = 0;//�趨��ʼֵ
    set_computer_value(SEND_STOP_CMD, CURVES_CH1, NULL, 0);    // ͬ����λ����������ť״̬
    set_computer_value(SEND_TARGET_CMD, CURVES_CH1, &target_location, 1);     // ��ͨ�� 1 ����Ŀ��ֵ
	set_computer_value(SEND_TARGET_CMD, CURVES_CH2, &target_location, 1);     // ��ͨ�� 2 ����Ŀ��ֵ
	tim_pwm_disable();
#else
	tim_pwm_enable();
	/*���Իش���PC104����Ҫȥ��*/
    //Usart_SendString((uint8_t *)"��ʼ����ɣ�");
#endif
	uart_enable();
	tim_econder_enable();
    tim_basic_enable(); 		
	/*��ʼ���ṹ��*/							/*(������ֵ,�����޷�,p,i,d,������,����ֵ )*/
	pid_param_init(&Motor_Big,POSITION_PID,500.0f,200.0f,2.1f,1.0f,0.0f,10000.0f,500.0f);//��ʼ�����PID�ṹ��
	pid_param_init(&Motor_Small,POSITION_PID,500.0f,200.0f,2.1f,1.0f,0.0f,10000.0f,1000.0f);//��ʼ��С��PID�ṹ�� 
	//pid_param_init(&Motor_Ce,POSITION_PID,500.0f,200.0f,2.1f,1.0f,0.0f,10000.0f,100.0f);//��ʼ��С��PID�ṹ�� 
	//PID_struct_init(&Motor_Small,POSITION_PID,999.0f,200.0f,2000.0f,20.5f,50.0f,10000.0f,0.02f);
#if PID_ASSISTANT_EN
    float pid1_temp[3] = {Motor_Big.p, Motor_Big.i, Motor_Big.d};
    set_computer_value(SEND_P_I_D_CMD, CURVES_CH1, pid1_temp, 3);     // ��ͨ�� 1 ���� P I D ֵ
	float pid2_temp[3] = {Motor_Small.p, Motor_Small.i, Motor_Small.d};
    set_computer_value(SEND_P_I_D_CMD, CURVES_CH2, pid2_temp, 3);     // ��ͨ�� 2 ���� P I D ֵ
#endif	

    /*����12���ϵ��ʼ��*/
//    Ce1_SETCOMPARE(2000);
//    Ce1_SETCOMPARE(1500);
//    Ce2_SETCOMPARE(2000);
//    Ce2_SETCOMPARE(1500);
//	/*����*/
//	Ce1_SETCOMPARE(1600);
//	Ce2_SETCOMPARE(1600);
	
}
