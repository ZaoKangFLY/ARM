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

#define  YeHuoPID    					0/* 1:ʹ��Ұ��PID����������ʾ����,ע��protocol.c�� case SET_TARGET_CMD:ֻ��һ��ͨ������*/
#define  UVMS    						1/* 1:ʹ��UVMS��λ��*/
#define  VoFa    						0/* 1VoFaֻ��һ��ͨ������*/
#define  CeShi    						0/* 1:��������*/

#define  JiFenFenLi 					0/* 1:���ַ���ʹ��*/
/*�����Ժ����modbusͨ�ţ�����ͨ��Э��ı���Ӧ�Ĵ����ɵ�����ֵ������ֵ���żĴ����ж�д����
���߶���ṹ�����ṹ�壬ͨ���������ṹ�帳ֵ��ʵ����λ���ı�ֵ
*/

/* ���ٵ�����ٱ� */
#define J_REDUCTION_RATIO                 1//25
#define B_REDUCTION_RATIO                 40
#define S_REDUCTION_RATIO                 25
#define W_REDUCTION_RATIO                 1
/*����ᵽ�۴����ȣ������ᴫ����*/
#define  J_TRANS_RATIO  				1
#define  B_TRANS_RATIO         			15
#define  S_TRANS_RATIO        			15
#define  W_TRANS_RATIO  				1
/* ����������ֱ���/���� */
#define J_ENCODER_RESOLUTION              1000
#define B_ENCODER_RESOLUTION              1000
#define S_ENCODER_RESOLUTION              1000
#define W_ENCODER_RESOLUTION              1000
/* ������Ƶ֮����ֱܷ��� ,ENCODER_TOTAL_RESOLUTIONΪת��һȦ���Բ����������Ŀ*/
#define J_ENCODER_TOTAL_RESOLUTION             (J_ENCODER_RESOLUTION * 2)// sConfig.IC1Polarity = BOTHEDGEΪ��
#define B_ENCODER_TOTAL_RESOLUTION             (B_ENCODER_RESOLUTION * 2)
#define S_ENCODER_TOTAL_RESOLUTION             (S_ENCODER_RESOLUTION * 2)
#define W_ENCODER_TOTAL_RESOLUTION             (W_ENCODER_RESOLUTION * 2)
/*�����תһȦ��������������������ٱ���*/
#define J_OUTPUT_SHAFT_PULSES_PER_REVOLUTION    (J_ENCODER_TOTAL_RESOLUTION * J_REDUCTION_RATIO ) //�粿 
#define B_OUTPUT_SHAFT_PULSES_PER_REVOLUTION    (B_ENCODER_TOTAL_RESOLUTION * B_REDUCTION_RATIO ) //��� 
#define S_OUTPUT_SHAFT_PULSES_PER_REVOLUTION    (S_ENCODER_TOTAL_RESOLUTION * S_REDUCTION_RATIO )//С��
#define W_OUTPUT_SHAFT_PULSES_PER_REVOLUTION    (W_ENCODER_TOTAL_RESOLUTION * W_REDUCTION_RATIO ) //��
/*��תһȦ�����������*/ 
#define J_ARM_PULSES_PER_REVOLUTION     (J_OUTPUT_SHAFT_PULSES_PER_REVOLUTION * J_TRANS_RATIO)  //
#define B_ARM_PULSES_PER_REVOLUTION     (B_OUTPUT_SHAFT_PULSES_PER_REVOLUTION * B_TRANS_RATIO)  //
#define S_ARM_PULSES_PER_REVOLUTION     (S_OUTPUT_SHAFT_PULSES_PER_REVOLUTION * S_TRANS_RATIO)  //750000
#define W_ARM_PULSES_PER_REVOLUTION     (W_OUTPUT_SHAFT_PULSES_PER_REVOLUTION * W_TRANS_RATIO)  //





void system_init(void);

#endif /* __APP_H__ */

