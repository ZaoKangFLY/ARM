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
#include "pid.h"
#include "tim.h"
#include "usart.h"


/* 1:ʹ��PID����������ʾ���Σ�0��ʹ�ô���ֱ�Ӵ�ӡ����*/
#define  PID_ASSISTANT_EN    			1

/* ����������ֱ���/���� */
#define ENCODER_RESOLUTION              1000

/*Ԥװ��ֵ*/
#define  B_ENCODER_PERIOD      		65535 
#define  S_ENCODER_PERIOD			4294967295

/* ���ٵ�����ٱ� */
#define B_REDUCTION_RATIO                 125
#define S_REDUCTION_RATIO                 25

/*����ᵽ�۴����ȣ������ᴫ����*/
#define  B_TRANS_RATIO         			13
#define  S_TRANS_RATIO        			1//8

void system_init(void);

#endif /* __APP_H__ */

