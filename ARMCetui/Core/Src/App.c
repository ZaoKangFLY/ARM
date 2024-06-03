#include "App.h"

/*控制系统初始化*/
void system_init()
{
 #if PID_ASSISTANT_EN
 	int32_t target_location = 0;//设定初始值
    set_computer_value(SEND_STOP_CMD, CURVES_CH1, NULL, 0);    // 同步上位机的启动按钮状态
    set_computer_value(SEND_TARGET_CMD, CURVES_CH1, &target_location, 1);     // 给通道 1 发送目标值
	set_computer_value(SEND_TARGET_CMD, CURVES_CH2, &target_location, 1);     // 给通道 2 发送目标值
	tim_pwm_disable();
#else
	tim_pwm_enable();

#endif
	uart_enable();
	tim_econder_enable();
    tim_basic_enable(); 		
	/*初始化结构体*/							/*(最大输出值,积分限幅,p,i,d,最大误差,死区值 )*/
	pid_param_init(&Pid_Jian,POSITION_PID,500.0f,200.0f,2.1f,1.0f,0.0f,10000.0f,500.0f);//
	pid_param_init(&Pid_Big,POSITION_PID,999.0f,200.0f,2.1f,1.0f,0.0f,10000.0f,500.0f);//初始化大臂PID结构体
	pid_param_init(&Pid_Small,POSITION_PID,999.0f,200.0f,2.1f,1.0f,0.0f,10000.0f,1000.0f);//初始化小臂PID结构体 
	pid_param_init(&Pid_Wan,POSITION_PID,500.0f,200.0f,2.1f,1.0f,0.0f,10000.0f,1000.0f);//初始
	//pid_param_init(&Motor_Ce,POSITION_PID,500.0f,200.0f,2.1f,1.0f,0.0f,10000.0f,100.0f);//初始化小臂PID结构体 
	//PID_struct_init(&Motor_Small,POSITION_PID,999.0f,200.0f,2000.0f,20.5f,50.0f,10000.0f,0.02f);
//	motor_init(&Motor_Jian, &Jian_Encoder_htim, &Pid_Jian, &g_jianEncoderOverflowCount, 
//           J_ENCODER_PERIOD, J_ARM_PULSES_PER_REVOLUTION, motor_jian_fun);
//	motor_init(&Motor_Big, &Big_Encoder_htim, &Pid_Big, &g_bigEncoderOverflowCount, 
//           B_ENCODER_PERIOD, B_ARM_PULSES_PER_REVOLUTION, motor_big_fun);
//	motor_init(&Motor_Small, &Small_Encoder_htim, &Pid_Small, &g_smallEncoderOverflowCount, 
//           S_ENCODER_PERIOD, S_ARM_PULSES_PER_REVOLUTION, motor_small_fun);   
//	motor_init(&Motor_Wan, &Wan_Encoder_htim, &Pid_Wan, &g_wanEncoderOverflowCount, 
//           W_ENCODER_PERIOD, W_ARM_PULSES_PER_REVOLUTION, motor_wan_fun);	   
#if PID_ASSISTANT_EN
    float pid1_temp[3] = {Motor_Big.p, Motor_Big.i, Motor_Big.d};
    set_computer_value(SEND_P_I_D_CMD, CURVES_CH1, pid1_temp, 3);     // 给通道 1 发送 P I D 值
	float pid2_temp[3] = {Motor_Small.p, Motor_Small.i, Motor_Small.d};
    set_computer_value(SEND_P_I_D_CMD, CURVES_CH2, pid2_temp, 3);     // 给通道 2 发送 P I D 值
#endif	

    /*侧推12的上电初始化*/
    Ce1_SETCOMPARE(2000);
    Ce1_SETCOMPARE(1500);
    Ce2_SETCOMPARE(2000);
    Ce2_SETCOMPARE(1500);
	HAL_Delay(1385);//延时后自启动
	/*调试回传，PC104要去掉*/
    Usart_SendString((uint8_t *)"初始化完成！\n");
//	/*悬浮*/
//	Ce1_SETCOMPARE(1600);
//	Ce2_SETCOMPARE(1600);
	
}
