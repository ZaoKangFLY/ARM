#ifndef __MOTOR_CONTROL_H__
#define __MOTOR_CONTROL_H__
#include "App.h"

#define  Big_PWM1_SETCOMPAER(ChannelPulse)      Big_PWM_SETCOMPAER( Big_CHANNEL_1,ChannelPulse)
#define  Big_PWM2_SETCOMPAER(ChannelPulse)      Big_PWM_SETCOMPAER( Big_CHANNEL_2,ChannelPulse)
#define  Small_PWM1_SETCOMPAER(ChannelPulse)    Small_PWM_SETCOMPAER(Small_CHANNEL_1,ChannelPulse)
#define  Small_PWM2_SETCOMPAER(ChannelPulse)    Small_PWM_SETCOMPAER(Small_CHANNEL_2,ChannelPulse)

#define  Ce1_SETCOMPAER(ChannelPulse)           __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1,ChannelPulse)   // 设置比较寄存器的值
#define  Ce2_SETCOMPAER(ChannelPulse)           __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_2,ChannelPulse)  

extern  uint8_t  is_motor_en;

//extern  int32_t _get;


void Motor_Small_Set_Speed(float _set);

void Motor_Big_Set_Position(float _set);

void Motor_Small_Set_Position(float _set);
    
void Motor_CeTui_Set_Position(int _set);

float Convert_angle(int j, float x);

float COUNT(float x);


#endif
