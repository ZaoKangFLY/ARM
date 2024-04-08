#ifndef __APP_H__
#define __APP_H__
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include "main.h"
#include "stm32f4xx.h"
#include "protocol.h"
#include "Uart_Init.h"
#include "Tim_Init.h"
#include "motor_control.h"
#include "pid.h"
#include "tim.h"
#include "usart.h"


/* 1:使用PID调试助手显示波形，0：使用串口直接打印数据*/
#define  PID_ASSISTANT_EN    			1    
/* 编码器物理分辨率/线数 */
#define ENCODER_RESOLUTION              1000
/*预装载值*/
#define  Big_Econder_Period       		65535 
#define  Small_Econder_Period			4294967295
/* 编码器接口倍频数 */
#define ENCODER_MODE                    TIM_ENCODERMODE_TI1//改为1/2/12
/* 减速电机减速比 */
#define REDUCTION_RATIO_B                 125
#define REDUCTION_RATIO_S                 25
/*输出轴到小臂传动比*/
#define  Motor_Big_K        			1
#define  Motor_Small_K        			8 

void System_Init(void);


#endif /* __APP_H__ */

