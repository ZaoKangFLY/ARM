#ifndef __MOTOR_CONTROL_H__
#define __MOTOR_CONTROL_H__
#include "main.h"
#include "tim.h"
#include "pid.h"
#include "usart.h"
#include "gpio.h"
#define PI 3.14159265f
#define limit(a, max, min)                     do { if (a > max) { 	a = max; }\
													else if (a < min) { a = 1500; } } while (0)
enum{
    last= 0,
    now= 1,
	Ce1,
	Ce2,
};


typedef struct pwm //状态指示
{

	float con_val[2];
	float outlier;
	uint8_t mode;
    uint8_t aRxBuf_1[RX_BUF_1_LENGTH];//缓存数组

}PwmHandle;


extern  uint8_t  is_cemotor_en ;


void Motor_CeTui_Set_Speed(int _set);
void Motor_CeTui_Set(float _get);

    

float rads_to_deg(float radians);
float deg_to_rad(float degrees);

float COUNT(float x);


#endif
