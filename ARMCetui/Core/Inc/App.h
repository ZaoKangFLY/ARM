#ifndef __APP_H__
#define __APP_H__
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include "main.h"
#include "stm32f4xx.h"
#include "protocol.h"
#include "uart_init.h"
#include "tim_init.h"
#include "motor_control.h"
#include "tim.h"
#include "usart.h"

#define  YeHuoPID    					0/* 1:使用野火PID调试助手显示波形,注意protocol.c里 case SET_TARGET_CMD:只能一个通道调试*/
#define  UVMS    						1/* 1:使用UVMS上位机*/
#define  VoFa    						0/* 1VoFa只能一个通道调试*/
#define  CeShi    						0/* 1:回显数据*/

#define  JiFenFenLi 					0/* 1:积分分离使能*/
/*建议以后采用modbus通信，可以通过协议改变相应寄存器可调节数值，以下值都放寄存器中读写操作
或者定义结构体放入结构体，通过函数给结构体赋值来实现上位机改变值
*/

/* 减速电机减速比 */
#define J_REDUCTION_RATIO                 1//25
#define B_REDUCTION_RATIO                 40
#define S_REDUCTION_RATIO                 25
#define W_REDUCTION_RATIO                 1
/*输出轴到臂传动比，经过横传齿轮*/
#define  J_TRANS_RATIO  				1
#define  B_TRANS_RATIO         			15
#define  S_TRANS_RATIO        			15
#define  W_TRANS_RATIO  				1
/* 编码器物理分辨率/线数 */
#define J_ENCODER_RESOLUTION              1000
#define B_ENCODER_RESOLUTION              1000
#define S_ENCODER_RESOLUTION              1000
#define W_ENCODER_RESOLUTION              1000
/* 经过倍频之后的总分辨率 ,ENCODER_TOTAL_RESOLUTION为转子一圈可以捕获的脉冲数目*/
#define J_ENCODER_TOTAL_RESOLUTION             (J_ENCODER_RESOLUTION * 2)// sConfig.IC1Polarity = BOTHEDGE为四
#define B_ENCODER_TOTAL_RESOLUTION             (B_ENCODER_RESOLUTION * 2)
#define S_ENCODER_TOTAL_RESOLUTION             (S_ENCODER_RESOLUTION * 2)
#define W_ENCODER_TOTAL_RESOLUTION             (W_ENCODER_RESOLUTION * 2)
/*输出轴转一圈捕获的脉冲数，经过减速比了*/
#define J_OUTPUT_SHAFT_PULSES_PER_REVOLUTION    (J_ENCODER_TOTAL_RESOLUTION * J_REDUCTION_RATIO ) //肩部 
#define B_OUTPUT_SHAFT_PULSES_PER_REVOLUTION    (B_ENCODER_TOTAL_RESOLUTION * B_REDUCTION_RATIO ) //大臂 
#define S_OUTPUT_SHAFT_PULSES_PER_REVOLUTION    (S_ENCODER_TOTAL_RESOLUTION * S_REDUCTION_RATIO )//小臂
#define W_OUTPUT_SHAFT_PULSES_PER_REVOLUTION    (W_ENCODER_TOTAL_RESOLUTION * W_REDUCTION_RATIO ) //腕部
/*臂转一圈捕获的脉冲数*/ 
#define J_ARM_PULSES_PER_REVOLUTION     (J_OUTPUT_SHAFT_PULSES_PER_REVOLUTION * J_TRANS_RATIO)  //
#define B_ARM_PULSES_PER_REVOLUTION     (B_OUTPUT_SHAFT_PULSES_PER_REVOLUTION * B_TRANS_RATIO)  //
#define S_ARM_PULSES_PER_REVOLUTION     (S_OUTPUT_SHAFT_PULSES_PER_REVOLUTION * S_TRANS_RATIO)  //750000
#define W_ARM_PULSES_PER_REVOLUTION     (W_OUTPUT_SHAFT_PULSES_PER_REVOLUTION * W_TRANS_RATIO)  //





void system_init(void);

#endif /* __APP_H__ */

