#include "App.h"

/*控制系统初始化*/
void system_init()
{
 #if YeHuoPID
 	int32_t target_location = 0;//设定初始值
    set_computer_value(SEND_STOP_CMD, CURVES_CH1, NULL, 0);    // 同步上位机的启动按钮状态
	set_computer_value(SEND_STOP_CMD, CURVES_CH2, NULL, 0);    // 同步上位机的启动按钮状态
	set_computer_value(SEND_STOP_CMD, CURVES_CH3, NULL, 0);    // 同步上位机的启动按钮状态
	set_computer_value(SEND_STOP_CMD, CURVES_CH4, NULL, 0);    // 同步上位机的启动按钮状态
    set_computer_value(SEND_TARGET_CMD, CURVES_CH1, &target_location, 1);     // 给通道 1 发送目标值
	set_computer_value(SEND_TARGET_CMD, CURVES_CH2, &target_location, 1);     // 给通道 2 发送目标值
	set_computer_value(SEND_TARGET_CMD, CURVES_CH3, &target_location, 1);     // 给通道 3 发送目标值
	set_computer_value(SEND_TARGET_CMD, CURVES_CH4, &target_location, 1);     // 给通道 4 发送目标值
	tim_pwm_disable();
#else
	tim_pwm_enable();
#endif
	uart_enable();
    tim_basic_enable(); 
	tim_econder_enable();	
	/*初始化结构体*/					 /*最大输出值,  分离误差，积分限幅,    p,i,d,      最大误差,死区值 */
	PID_struct_init(&Pid_Jian,POSITION_PID, 500.0f,  1000.0f,200.0f,  0.1f,1.0f,0.0f, 10000.0f,1000.0f);
	PID_struct_init(&Pid_Big,POSITION_PID,  999.0f,  20000.0f,666.0f,  0.175f,0.7731f,0.0f, 10000.0f,2000.0f);//初始化大臂PID结构体
	PID_struct_init(&Pid_Small,POSITION_PID,999.0f,  4000.0f,666.0f,  0.17f,0.000077f,0.0f, 10000.0f,2000.0f);//初始化小臂PID结构体 
	PID_struct_init(&Pid_Wan,POSITION_PID,  500.0f,  1000.0f,2000.0f,  2.1f,1.0f,0.0f, 10000.0f,1000.0f);  
	
#if YeHuoPID
	float pid1_temp[3] = {Pid_Jian.p, Pid_Jian.i, Pid_Jian.d};
    set_computer_value(SEND_P_I_D_CMD, CURVES_CH1, pid1_temp, 3);     // 给通道 1发送 P I D 值
    float pid2_temp[3] = {Pid_Big.p, Pid_Big.i, Pid_Big.d};
    set_computer_value(SEND_P_I_D_CMD, CURVES_CH2, pid2_temp, 3);     // 给通道 2发送 P I D 值
	float pid3_temp[3] = {Pid_Small.p, Pid_Small.i, Pid_Small.d};
    set_computer_value(SEND_P_I_D_CMD, CURVES_CH3, pid3_temp, 3);     // 给通道 3 发送 P I D 值
	float pid4_temp[3] = {Pid_Wan.p, Pid_Wan.i, Pid_Wan.d};
    set_computer_value(SEND_P_I_D_CMD, CURVES_CH4, pid4_temp, 3);     // 给通道 4 发送 P I D 值
#endif	

    /*侧推12的上电初始化*/
//    Ce1_SETCOMPARE(1000);//蓝色反
//    Ce1_SETCOMPARE(1500);
//    Ce2_SETCOMPARE(2000);
//    Ce2_SETCOMPARE(1500);
//	HAL_Delay(2685);//延时后自启动
//  /*悬浮*/
//	Ce1_SETCOMPARE(1450);
//	Ce2_SETCOMPARE(1525);

	
#if CeShi   
	/*调试回传，PC104要去掉*/
    Usart_SendString((uint8_t *)"初始化完成！\n");
#endif

}
