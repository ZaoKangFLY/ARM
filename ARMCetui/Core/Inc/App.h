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


/* 1:ʹ��PID����������ʾ���Σ�0��ʹ�ô���ֱ�Ӵ�ӡ����*/
#define  PID_ASSISTANT_EN    			1    
/* ����������ֱ���/���� */
#define ENCODER_RESOLUTION              1000
/*Ԥװ��ֵ*/
#define  Big_Econder_Period       		65535 
#define  Small_Econder_Period			4294967295
/* �������ӿڱ�Ƶ�� */
#define ENCODER_MODE                    TIM_ENCODERMODE_TI1//��Ϊ1/2/12
/* ���ٵ�����ٱ� */
#define REDUCTION_RATIO_B                 125
#define REDUCTION_RATIO_S                 25
/*����ᵽС�۴�����*/
#define  Motor_Big_K        			1
#define  Motor_Small_K        			8 

void System_Init(void);


#endif /* __APP_H__ */

