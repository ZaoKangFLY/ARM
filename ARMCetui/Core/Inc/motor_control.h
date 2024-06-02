#ifndef __MOTOR_CONTROL_H__
#define __MOTOR_CONTROL_H__
#include "App.h"
#include "pid.h"
#define PI 3.14159265f
typedef struct {
    TIM_HandleTypeDef *encoder_timer;  // ��������ʱ�����   �ṹ��ָ��
    pid_t *pid;                        // PID ���ƽṹ��ָ�� �ṹ��ָ��
    int16_t *encoder_overflow_count;  // �������������  ����
    uint32_t encoder_period;           // ����������  ��
    uint32_t pulses_per_revolution;    // ÿת������   ��
    void (*motor_control_function)(int16_t); // ������ƺ���ָ��  ����
} motor_t;

extern  uint8_t  g_motorEnable;
extern uint8_t    g_cemotorEnable;

extern motor_t Motor_Jian;
extern motor_t Motor_Big;
extern motor_t Motor_Small;
extern motor_t Motor_Wan;

void motor_init(motor_t *motor, TIM_HandleTypeDef *encoder_timer, pid_t *pid, 
                int16_t *encoder_overflow_count, uint32_t encoder_period, 
                uint32_t pulses_per_revolution, void (*motor_control_function)(int16_t));
void set_position(motor_t *motor, int16_t _set) ;
void motor_jian_fun(int16_t pwm);
void motor_big_fun(int16_t pwm);
void motor_small_fun(int16_t pwm);
void motor_wan_fun(int16_t pwm);
void big_set_postion(int16_t _set);
void small_set_postion(int16_t _set);
void zhua_set(uint8_t _set);

void motor_cetui_set_postion(int _set);
void cetui_set_postion(float _get);

    
float rads_to_deg(float radians);
float deg_to_rad(float degrees);

float count(float x);


#endif
