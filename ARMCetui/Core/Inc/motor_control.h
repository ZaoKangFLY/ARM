#ifndef __MOTOR_CONTROL_H__
#define __MOTOR_CONTROL_H__
#include "App.h"
 
 
#define  Big1_SETCOMPAER(ChannelPulse)         __HAL_TIM_SET_COMPARE(&Big_PWM_htim, Big1_CHANNEL,ChannelPulse);
#define  Big2_SETCOMPAER(ChannelPulse)         __HAL_TIM_SET_COMPARE(&Big_PWM_htim, Big2_CHANNEL,ChannelPulse);
#define  Small1_SETCOMPAER(ChannelPulse)       __HAL_TIM_SET_COMPARE(&Small_PWM_htim, Small1_CHANNEL,ChannelPulse);
#define  Small2_SETCOMPAER(ChannelPulse)       __HAL_TIM_SET_COMPARE(&Small_PWM_htim, Small2_CHANNEL,ChannelPulse);
#define  Ce1_SETCOMPAER(ChannelPulse)           __HAL_TIM_SET_COMPARE(&Ce_PWM_htim , Ce1_CHANNEL,ChannelPulse)   // 设置比较寄存器的值
#define  Ce2_SETCOMPAER(ChannelPulse)           __HAL_TIM_SET_COMPARE(&Ce_PWM_htim , Ce2_CHANNEL,ChannelPulse)  


//#define  Big1_SETCOMPAER(ChannelPulse)      Big_PWM_SETCOMPAER( Big_CHANNEL_1,ChannelPulse)
//#define  Big2_SETCOMPAER(ChannelPulse)      Big_PWM_SETCOMPAER( Big_CHANNEL_2,ChannelPulse)
//#define  Small1_SETCOMPAER(ChannelPulse)    Small_PWM_SETCOMPAER(Small_CHANNEL_1,ChannelPulse)
//#define  Small2_SETCOMPAER(ChannelPulse)    Small_PWM_SETCOMPAER(Small_CHANNEL_2,ChannelPulse)


extern  uint8_t  is_motor_en;


void Motor_Small_Set_Speed(float _set);
void Motor_Big_Set_Speed(float _set);
void Motor_CeTui_Set_Speed(int _set);

void Motor_Big_Set_Position(float _set);
void Motor_Small_Set_Position(float _set);
    


float Convert_angle(int j, float x);

float COUNT(float x);


#endif
