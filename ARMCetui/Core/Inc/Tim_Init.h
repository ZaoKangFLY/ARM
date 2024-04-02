#ifndef __TIM_INIT_H__
#define __TIM_INIT_H__
#include "App.h"

#define  Basic_htim                    htim6
#define  Big_Encoder_htim              htim1
#define  Small_Encoder_htim            htim2
#define  Big_PWM_htim                  htim5
#define  Small_PWM_htim                htim5

#define Big_CHANNEL_1                  TIM_CHANNEL_3
#define Big_CHANNEL_2                  TIM_CHANNEL_4
#define Small_CHANNEL_1                TIM_CHANNEL_1
#define Small_CHANNEL_2                TIM_CHANNEL_2

#define  Basic_TIM            		   TIM6
#define  Big_Econder_TIM       		   TIM1
#define  Small_Econder_TIM     		   TIM2//TIM2->CNT=__HAL_TIM_GET_COUNTER(&htim2)
#define  Big_PWM_TIM     		   	   TIM5
#define  Small_PWM_TIM     		       TIM5


#define  Big_PWM1_ENABLE()                      HAL_TIM_PWM_Start(&Big_PWM_htim ,Big_CHANNEL_1)
#define  Big_PWM2_ENABLE()                      HAL_TIM_PWM_Start(&Big_PWM_htim ,Big_CHANNEL_2)
#define  Small_PWM1_ENABLE()                    HAL_TIM_PWM_Start(&Small_PWM_htim,Small_CHANNEL_1)
#define  Small_PWM2_ENABLE()                    HAL_TIM_PWM_Start(&Small_PWM_htim,Small_CHANNEL_2)

#define  Big_PWM1_DISABLE()						HAL_TIM_PWM_Stop(&Big_PWM_htim ,Big_CHANNEL_1)
#define  Big_PWM2_DISABLE()                     HAL_TIM_PWM_Stop(&Big_PWM_htim ,Big_CHANNEL_2)
#define  Small_PWM1_DISABLE()                   HAL_TIM_PWM_Stop(&Small_PWM_htim,Small_CHANNEL_1)
#define  Small_PWM2_DISABLE()                   HAL_TIM_PWM_Stop(&Small_PWM_htim,Small_CHANNEL_2)

#define  Ce_PWM1_ENABLE()                       HAL_TIM_PWM_Start(&htim12,TIM_CHANNEL_1)
#define  Ce_PWM2_ENABLE()                       HAL_TIM_PWM_Start(&htim12,TIM_CHANNEL_2)

#define  Basic_TIM_ENABLE()                     HAL_TIM_Base_Start_IT(&Basic_htim)
#define  Big_Encoder_ENABLE()                   HAL_TIM_Encoder_Start(&Big_Encoder_htim, TIM_CHANNEL_ALL)
#define  Small_Encoder_ENABLE()                 HAL_TIM_Encoder_Start(&Small_Encoder_htim, TIM_CHANNEL_ALL)

/* 清零计数器 */
#define   Small_TIM_SETCOUNTER()                __HAL_TIM_SET_COUNTER(&Small_Encoder_htim, 0)//定义电机编码器初值 Big_Econder_TIM->CNT=value;
#define   Big_TIM_SETCOUNTER()                  __HAL_TIM_SET_COUNTER(&Big_Encoder_htim , 0)


/* 经过倍频之后的总分辨率 */
#if (ENCODER_MODE == TIM_ENCODERMODE_TI12)
  #define ENCODER_TOTAL_RESOLUTION             (ENCODER_RESOLUTION * 4)  /* 4倍频后的总分辨率 */
#else
  #define ENCODER_TOTAL_RESOLUTION             (ENCODER_RESOLUTION * 2)  /* 2倍频后的总分辨率 */
#endif



#define CIRCLE_PULSES    ENCODER_TOTAL_RESOLUTION     // 编码器一圈可以捕获的脉冲

#define CIRCLE_OUTPUT    (CIRCLE_PULSES*REDUCTION_RATIO)  //输出轴的一圈的脉冲数

#define CIRCLE_Samll      (CIRCLE_OUTPUT*Motor_Small_K)  //小臂的一圈的脉冲数


/* 以下两宏仅适用于定时器时钟源TIMxCLK=84MHz，预分频器为：1680-1 的情况 */
#define SET_BASIC_TIM_PERIOD(T)     __HAL_TIM_SET_AUTORELOAD(&Basic_htim, (T)*50 - 1)    // 设置定时器的周期（1~1000ms）
#define GET_BASIC_TIM_PERIOD()     ((__HAL_TIM_GET_AUTORELOAD(&Basic_htim)+1)/50.0)   // 获取定时器的周期，单位ms//是用于获取定时器的自动重装载寄存器

extern int16_t Encoder_Overflow_Count;

void TIMx_PWM_enable(void);
void TIMx_PWM_disable(void);
void TIMx_econder_enable(void);
void TIMx_basic_enable(void);
void Big_PWM_SETCOMPAER(uint32_t channel,int compare);
void Small_PWM_SETCOMPAER(uint32_t channel,int compare);




#endif

