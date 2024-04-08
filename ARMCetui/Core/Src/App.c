#include "App.h"



/*����ϵͳ��ʼ��*/
void System_Init()
{
	int32_t target_location =0;
	
 #if PID_ASSISTANT_EN
    set_computer_value(SEND_STOP_CMD, CURVES_CH1, NULL, 0);    // ͬ����λ����������ť״̬
    set_computer_value(SEND_TARGET_CMD, CURVES_CH1, &target_location, 1);     // ��ͨ�� 1 ����Ŀ��ֵ
	set_computer_value(SEND_TARGET_CMD, CURVES_CH2, &target_location, 1);     // ��ͨ�� 1 ����Ŀ��ֵ
	TIMx_PWM_disable();
#else
	TIMx_PWM_enable();
	/*���Իش���PC104����Ҫȥ��*/
    //HAL_UART_Transmit(&huart3, (uint8_t *)"��ʼ����ɣ�", 16 , 0xffff);
    Usart_SendString((uint8_t *)"��ʼ����ɣ�");
#endif
	
	Uartx_enable();
	TIMx_econder_enable();
    TIMx_basic_enable();

	/*��ʼ���ṹ��*/							/*(������ֵ,�����޷�,p,i,d,������,����ֵ )*/
	pid_param_init(&Motor_Big,POSITION_PID,999.0f,200.0f,2.1f,1.0f,0.0f,10000.0f,0.001f);//��ʼ�����PID�ṹ��
	pid_param_init(&Motor_Small,POSITION_PID,999.0f,200.0f,2.1f,1.0f,0.0f,10000.0f,100.0f);//��ʼ��С��PID�ṹ�� 
	pid_param_init(&Motor_Ce,POSITION_PID,500.0f,200.0f,2.1f,1.0f,0.0f,10000.0f,100.0f);//��ʼ��С��PID�ṹ�� 
	//PID_struct_init(&Motor_Small,POSITION_PID,999.0f,200.0f,2000.0f,20.5f,50.0f,10000.0f,0.02f);

#if PID_ASSISTANT_EN
    float pid1_temp[3] = {Motor_Big.p, Motor_Big.i, Motor_Big.d};
    set_computer_value(SEND_P_I_D_CMD, CURVES_CH1, pid1_temp, 3);     // ��ͨ�� 1 ���� P I D ֵ
	float pid2_temp[3] = {Motor_Small.p, Motor_Small.i, Motor_Small.d};
    set_computer_value(SEND_P_I_D_CMD, CURVES_CH2, pid2_temp, 3);     // ��ͨ�� 2 ���� P I D ֵ
#endif	


    /*����12���ϵ��ʼ��*/
    Ce1_SETCOMPAER(2000);
    Ce1_SETCOMPAER(1500);
    Ce2_SETCOMPAER(2000);
    Ce2_SETCOMPAER(1500);
	/*����*/
	Ce1_SETCOMPAER(1600);
	Ce2_SETCOMPAER(1600);
	
}
