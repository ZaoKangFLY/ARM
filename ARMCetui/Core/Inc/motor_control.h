#ifndef __MOTOR_CONTROL_H__
#define __MOTOR_CONTROL_H__
#include "App.h"

#define PI 3.14159265f
#define  Big1_SETCOMPARE(ChannelPulse)         __HAL_TIM_SET_COMPARE(&Big_PWM_htim, Big1_CHANNEL,ChannelPulse);
#define  Big2_SETCOMPARE(ChannelPulse)         __HAL_TIM_SET_COMPARE(&Big_PWM_htim, Big2_CHANNEL,ChannelPulse);
#define  Small1_SETCOMPARE(ChannelPulse)       __HAL_TIM_SET_COMPARE(&Small_PWM_htim, Small1_CHANNEL,ChannelPulse);
#define  Small2_SETCOMPARE(ChannelPulse)       __HAL_TIM_SET_COMPARE(&Small_PWM_htim, Small2_CHANNEL,ChannelPulse);
#define  Ce1_SETCOMPARE(ChannelPulse)           __HAL_TIM_SET_COMPARE(&Ce_PWM_htim , Ce1_CHANNEL,ChannelPulse)   // 设置比较寄存器的值
#define  Ce2_SETCOMPARE(ChannelPulse)           __HAL_TIM_SET_COMPARE(&Ce_PWM_htim , Ce2_CHANNEL,ChannelPulse)  


extern  uint8_t  g_motorEnable;
extern uint8_t    g_cemotorEnable;

void big_set_postion(int16_t _set);
void small_set_postion(int16_t _set);
void motor_cetui_set_postion(int _set);
void cetui_set_postion(float _get);

    
float rads_to_deg(float radians);
float deg_to_rad(float degrees);

float count(float x);


#endif
