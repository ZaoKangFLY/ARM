#ifndef __MOTOR_CONTROL_H__
#define __MOTOR_CONTROL_H__
#include "main.h"
#include "tim.h"
#include "pid.h"
#include "usart.h"
#include "gpio.h"
#define PI 3.14159265f
#define limit(a, max, min)                     do { if (a > max) { 	a = max; }\
													else if (a < min) { a = min; } } while (0)
enum{
	CE1,
	CE2,
};


typedef struct pwm //状态指示
{
	float Con_val[2];
	uint16_t Outlier;//跳变限制
	uint16_t Max,Min;

}PwmHandle;


extern  uint8_t  is_cemotor_en ;
extern PwmHandle pwmHandle_1;
extern PwmHandle pwmHandle_2;

void Motor_CeTui_Set_Speed(int _set);
void Motor_CeTui_Set(float _get);

    

float rads_to_deg(float radians);
float deg_to_rad(float degrees);

float COUNT(float x);
void pwmHandle_Init(PwmHandle* pwm,uint16_t out,uint16_t last,uint16_t max,uint16_t min);

#endif
