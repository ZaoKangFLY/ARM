#include "App.h"



/*控制系统初始化*/
void System_Init()
{
	int32_t target_location =0;
	
 #if PID_ASSISTANT_EN
    set_computer_value(SEND_STOP_CMD, CURVES_CH1, NULL, 0);    // 同步上位机的启动按钮状态
    set_computer_value(SEND_TARGET_CMD, CURVES_CH1, &target_location, 1);     // 给通道 1 发送目标值
	set_computer_value(SEND_TARGET_CMD, CURVES_CH2, &target_location, 1);     // 给通道 1 发送目标值
	TIMx_PWM_disable();
#else
	TIMx_PWM_enable();
	/*调试回传，PC104可能要去掉*/
    //HAL_UART_Transmit(&huart3, (uint8_t *)"初始化完成！", 16 , 0xffff);
    Usart_SendString((uint8_t *)"初始化完成！");
#endif
	
	Uartx_enable();
	TIMx_econder_enable();
    TIMx_basic_enable();

	/*初始化结构体*/							/*(最大输出值,积分限幅,p,i,d,最大误差,死区值 )*/
	pid_param_init(&Motor_Big,POSITION_PID,999.0f,200.0f,2.1f,1.0f,0.0f,10000.0f,0.001f);//初始化大臂PID结构体
	pid_param_init(&Motor_Small,POSITION_PID,999.0f,200.0f,2.1f,1.0f,0.0f,10000.0f,100.0f);//初始化小臂PID结构体 
	pid_param_init(&Motor_Ce,POSITION_PID,500.0f,200.0f,2.1f,1.0f,0.0f,10000.0f,100.0f);//初始化小臂PID结构体 
	//PID_struct_init(&Motor_Small,POSITION_PID,999.0f,200.0f,2000.0f,20.5f,50.0f,10000.0f,0.02f);

#if PID_ASSISTANT_EN
    float pid1_temp[3] = {Motor_Big.p, Motor_Big.i, Motor_Big.d};
    set_computer_value(SEND_P_I_D_CMD, CURVES_CH1, pid1_temp, 3);     // 给通道 1 发送 P I D 值
	float pid2_temp[3] = {Motor_Small.p, Motor_Small.i, Motor_Small.d};
    set_computer_value(SEND_P_I_D_CMD, CURVES_CH2, pid2_temp, 3);     // 给通道 2 发送 P I D 值
#endif	


    /*侧推12的上电初始化*/
    Ce1_SETCOMPAER(2000);
    Ce1_SETCOMPAER(1500);
    Ce2_SETCOMPAER(2000);
    Ce2_SETCOMPAER(1500);
	/*悬浮*/
	Ce1_SETCOMPAER(1600);
	Ce2_SETCOMPAER(1600);
	
}
