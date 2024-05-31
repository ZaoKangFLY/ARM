#ifndef __TIM_INIT_H__
#define __TIM_INIT_H__
#include "App.h"

/*句柄*/
#define  Basic_htim                    htim6

#define  Jian_Encoder_htim 			   htim4
#define  Big_Encoder_htim              htim1
#define  Small_Encoder_htim            htim2
#define  Wan_Encoder_htim              htim3
#define  Jian_PWM_htim                 htim8
#define  Big_PWM_htim                  htim5
#define  Small_PWM_htim                htim5
#define  Wan_PWM_htim                  htim9
#define  Zhua_PWM_htim                 htim8
#define  Ce_PWM_htim    		       htim12

/*通道*/
#define Jian1_CHANNEL                 TIM_CHANNEL_1
#define Jian2_CHANNEL                 TIM_CHANNEL_2
#define Big1_CHANNEL                  TIM_CHANNEL_3
#define Big2_CHANNEL                  TIM_CHANNEL_4
#define Small1_CHANNEL                TIM_CHANNEL_1
#define Small2_CHANNEL                TIM_CHANNEL_2
#define Wan1_CHANNEL                  TIM_CHANNEL_1
#define Wan2_CHANNEL                  TIM_CHANNEL_2
#define Zhua1_CHANNEL                 TIM_CHANNEL_3
#define Zhua2_CHANNEL                 TIM_CHANNEL_4
#define Ce1_CHANNEL                   TIM_CHANNEL_1
#define Ce2_CHANNEL                   TIM_CHANNEL_2

/*PWM*/
//使能
#define  Jian1_PWM_ENABLE()                     HAL_TIM_PWM_Start(&Jian_Encoder_htim ,Jian1_CHANNEL  )
#define  Jian2_PWM_ENABLE()                     HAL_TIM_PWM_Start(&Jian_Encoder_htim  ,Jian1_CHANNEL  )
#define  Big1_PWM_ENABLE()                      HAL_TIM_PWM_Start(&Big_PWM_htim ,Big1_CHANNEL)
#define  Big2_PWM_ENABLE()                      HAL_TIM_PWM_Start(&Big_PWM_htim ,Big2_CHANNEL)
#define  Small1_PWM_ENABLE()                    HAL_TIM_PWM_Start(&Small_PWM_htim,Small1_CHANNEL)
#define  Small2_PWM_ENABLE()                    HAL_TIM_PWM_Start(&Small_PWM_htim,Small2_CHANNEL)
#define  Wan1_PWM_ENABLE()                      HAL_TIM_PWM_Start(&Wan_Encoder_htim ,Wan1_CHANNEL)
#define  Wan2_PWM_ENABLE()                      HAL_TIM_PWM_Start(&Wan_Encoder_htim ,Wan2_CHANNEL)
#define  Zhua1_PWM_ENABLE()                     HAL_TIM_PWM_Start(&Zhua_PWM_htim  ,Zhua1_CHANNEL)
#define  Zhua2_PWM_ENABLE()                     HAL_TIM_PWM_Start(&Zhua_PWM_htim ,Zhua2_CHANNEL)
#define  Ce1_PWM_ENABLE()                       HAL_TIM_PWM_Start(&Ce_PWM_htim,Ce1_CHANNEL)
#define  Ce2_PWM_ENABLE()                       HAL_TIM_PWM_Start(&Ce_PWM_htim,Ce2_CHANNEL)
//失能
#define  Jian1_PWM_DISABLE()                     HAL_TIM_PWM_Stop(&Jian_Encoder_htim ,Jian1_CHANNEL  )
#define  Jian2_PWM_DISABLE()                    HAL_TIM_PWM_Stop(&Jian_Encoder_htim  ,Jian1_CHANNEL  )
#define  Big1_PWM_DISABLE()						HAL_TIM_PWM_Stop(&Big_PWM_htim ,Big1_CHANNEL)
#define  Big2_PWM_DISABLE()                     HAL_TIM_PWM_Stop(&Big_PWM_htim ,Big2_CHANNEL)
#define  Small1_PWM_DISABLE()                   HAL_TIM_PWM_Stop(&Small_PWM_htim,Small1_CHANNEL)
#define  Small2_PWM_DISABLE()                   HAL_TIM_PWM_Stop(&Small_PWM_htim,Small2_CHANNEL)
#define  Wan1_PWM_DISABLE()                      HAL_TIM_PWM_Stop(&Wan_Encoder_htim ,Wan1_CHANNEL)
#define  Wan2_PWM_DISABLE()                      HAL_TIM_PWM_Stop(&Wan_Encoder_htim ,Wan2_CHANNEL)
#define  Zhua1_PWM_DISABLE()                     HAL_TIM_PWM_Stop(&Zhua_PWM_htim  ,Zhua1_CHANNEL)
#define  Zhua2_PWM_DISABLE()                    HAL_TIM_PWM_Stop(&Zhua_PWM_htim ,Zhua2_CHANNEL)
#define  Ce1_PWM_DISABLE()                       HAL_TIM_PWM_Stop(&Ce_PWM_htim,Ce1_CHANNEL)
#define  Ce2_PWM_DISABLE()                       HAL_TIM_PWM_Stop(&Ce_PWM_htim,Ce2_CHANNEL)

/*定时器中断*/
#define  Basic_TIM_ENABLE()                     HAL_TIM_Base_Start_IT(&Basic_htim)

#define  Jian_Encoder_ENABLE()                  HAL_TIM_Encoder_Start(&Jian_Encoder_htim, TIM_CHANNEL_ALL)
#define  Big_Encoder_ENABLE()                   HAL_TIM_Encoder_Start(&Big_Encoder_htim, TIM_CHANNEL_ALL)
#define  Small_Encoder_ENABLE()                 HAL_TIM_Encoder_Start(&Small_Encoder_htim, TIM_CHANNEL_ALL)
#define  Wan_Encoder_ENABLE()                   HAL_TIM_Encoder_Start(&Wan_Encoder_htim, TIM_CHANNEL_ALL)
              

/* 清零计数器 */
#define   Jian_TIM_SETCOUNTER()                 __HAL_TIM_SET_COUNTER(&Jian_Encoder_htim , 0)
#define   Big_TIM_SETCOUNTER()                  __HAL_TIM_SET_COUNTER(&Big_Encoder_htim , 0)
#define   Small_TIM_SETCOUNTER()                __HAL_TIM_SET_COUNTER(&Small_Encoder_htim, 0)//定义电机编码器初值 Big_Econder_TIM->CNT=value;
#define   Wan_TIM_SETCOUNTER()                  __HAL_TIM_SET_COUNTER(&Wan_Encoder_htim , 0)

#define  Jian1_SETCOMPARE(ChannelPulse)         __HAL_TIM_SET_COMPARE(&Jian_PWM_htim, Jian1_CHANNEL,ChannelPulse);
#define  Jian2_SETCOMPARE(ChannelPulse)         __HAL_TIM_SET_COMPARE(&Jian_PWM_htim, Jian2_CHANNEL,ChannelPulse);
#define  Big1_SETCOMPARE(ChannelPulse)         __HAL_TIM_SET_COMPARE(&Big_PWM_htim, Big1_CHANNEL,ChannelPulse);
#define  Big2_SETCOMPARE(ChannelPulse)         __HAL_TIM_SET_COMPARE(&Big_PWM_htim, Big2_CHANNEL,ChannelPulse);
#define  Small1_SETCOMPARE(ChannelPulse)       __HAL_TIM_SET_COMPARE(&Small_PWM_htim, Small1_CHANNEL,ChannelPulse);
#define  Small2_SETCOMPARE(ChannelPulse)       __HAL_TIM_SET_COMPARE(&Small_PWM_htim, Small2_CHANNEL,ChannelPulse);
#define  Wan1_SETCOMPARE(ChannelPulse)         __HAL_TIM_SET_COMPARE(&Wan_PWM_htim, Wan1_CHANNEL,ChannelPulse);
#define  Wan2_SETCOMPARE(ChannelPulse)         __HAL_TIM_SET_COMPARE(&Wan_PWM_htim, Wan2_CHANNEL,ChannelPulse);
#define  Zhua1_SETCOMPARE(ChannelPulse)         __HAL_TIM_SET_COMPARE(&Zhua_PWM_htim, Zhua1_CHANNEL,ChannelPulse);
#define  Zhua2_SETCOMPARE(ChannelPulse)         __HAL_TIM_SET_COMPARE(&Zhua_PWM_htim, Zhua2_CHANNEL,ChannelPulse);
#define  Ce1_SETCOMPARE(ChannelPulse)           __HAL_TIM_SET_COMPARE(&Ce_PWM_htim , Ce1_CHANNEL,ChannelPulse)   // 设置比较寄存器的值
#define  Ce2_SETCOMPARE(ChannelPulse)           __HAL_TIM_SET_COMPARE(&Ce_PWM_htim , Ce2_CHANNEL,ChannelPulse)  

/* 以下两宏仅适用于定时器时钟源TIMxCLK=84MHz，预分频器为：1680-1 的情况 */
#define SET_BASIC_TIM_PERIOD(T)     __HAL_TIM_SET_AUTORELOAD(&Basic_htim, (T)*50 - 1)    // 设置定时器的周期（1~1000ms）
#define GET_BASIC_TIM_PERIOD()     ((__HAL_TIM_GET_AUTORELOAD(&Basic_htim)+1)/50.0)   // 获取定时器的周期，单位ms//是用于获取定时器的自动重装载寄存器

extern int16_t g_jianEncoderOverflowCount;
extern int16_t g_bigEncoderOverflowCount;
extern int16_t g_smallEncoderOverflowCount;
extern int16_t g_wanEncoderOverflowCount;

void tim_pwm_enable(void);
void tim_pwm_disable(void);
void tim_econder_enable(void);
void tim_basic_enable(void);

#endif

