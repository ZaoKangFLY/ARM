#ifndef __APP_H__
#define __APP_H__
#include <math.h>
#include <stdlib.h>
#include "main.h"
#include "tim.h"
#include "stm32f4xx.h"
#include "pid.h"
#include "Uart_Init.h"
#include "Tim_Init.h"
#include "usart.h"
#include "protocol.h"



#define PID_ASSISTANT_EN    1   // 1:使用PID调试助手显示波形，0：使用串口直接打印数据
#define      Motor_Small_K        8  //传动比

#define  Big_PWM1_SETCOMPAER(ChannelPulse)      Big_PWM_SETCOMPAER( Big_CHANNEL_1,ChannelPulse)
#define  Big_PWM2_SETCOMPAER(ChannelPulse)      Big_PWM_SETCOMPAER( Big_CHANNEL_2,ChannelPulse)
#define  Small_PWM1_SETCOMPAER(ChannelPulse)    Small_PWM_SETCOMPAER(Small_CHANNEL_1,ChannelPulse)
#define  Small_PWM2_SETCOMPAER(ChannelPulse)    Small_PWM_SETCOMPAER(Small_CHANNEL_2,ChannelPulse)

#define  Ce1_SETCOMPAER(ChannelPulse)           __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1,ChannelPulse)   // 设置比较寄存器的值
#define  Ce2_SETCOMPAER(ChannelPulse)           __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_2,ChannelPulse)  


//extern pid_t Motor_Big;    //创建大臂PID结构体  TIM12-CH1 TIM12-CH2       编码器对应  TIM4
  //创建小臂PID结构体  TIM8-CH3  TIM8-CH4        编码器对应  TIM2
extern  uint8_t  is_motor_en;

void System_Init(void);
void Motor_Small_Set_Speed(float _set);

void Motor_Big_Set_Position(float _set);

void Motor_Small_Set_Position(float _set);
    
void Motor_CeTui_Set_Position(int _set);

float Convert_angle(int j, float x);

float COUNT(float x);

extern pid_t Motor_Small;
extern  int32_t _get;



#endif /* __APP_H__ */

